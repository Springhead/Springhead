#include "Foundation.h"
#pragma hdrstop
#include "UTTimer.h"
#include <windows.h>
#include <mmsystem.h>

namespace Spr {

//----------------------------------------------------------------------------------------------------------
//	UTTimerStub
//
class UTTimerStub{
	unsigned resolution;
	unsigned resolutionMin;
	unsigned resolutionMax;
public:
	typedef std::vector<UTTimerProvider*> Providers;
	Providers providers;
	typedef std::vector<UTTimer*> Timers;
	Timers timers;
	UTTimerStub(): resolution(0), resolutionMin(0), resolutionMax(0){}

public:
	static UTTimerStub& UTTimerStub::Get(){
		static UTTimerStub obj;
		return obj;
	}	
	unsigned ResolutionMin(){
		if (!resolutionMin) GetCaps();
		return resolutionMin;
	}
	unsigned ResolutionMax(){
		if (!resolutionMax) GetCaps();
		return resolutionMax;
	}
#if defined(_WIN32)
	void UpdateResolution(){
		int resOld = resolution;
		resolution = UINT_MAX;
		for(Timers::iterator it = timers.begin(); it!=timers.end(); ++it){
			if ((*it)->IsStarted() && (*it)->GetMode() == UTTimer::MULTIMEDIA && (*it)->GetResolution() < resolution) 
				resolution = (*it)->GetResolution();
		}
		if (resolution == UINT_MAX) resolution = 0;

		if (resOld == resolution) return;
		if (resOld) timeEndPeriod(resOld);
		if (resolution) timeBeginPeriod(resolution);
	}
#else
#error UTMMTimerStub: Not yet implemented.	//	未実装
#endif

protected:
	void GetCaps(){
		TIMECAPS tc;
		if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
			DSTR << "UTTimer::BeginPeriod()  Fail to get resolution." << std::endl;
		}
		resolutionMin = tc.wPeriodMin;
		resolutionMax = tc.wPeriodMax;
	}
};

//----------------------------------------------------------------------------------------------------------
//	UTTimerProvider
//


UTTimerProvider::UTTimerProvider(){
}
UTTimerProvider::~UTTimerProvider(){
	Unregister();
}
void UTTimerProvider::Register(){
	UTTimerStub& stub = UTTimerStub::Get();
	stub.providers.push_back(this);
}
void UTTimerProvider::Unregister(){
	UTTimerStub& stub = UTTimerStub::Get();
	for(UTTimerStub::Providers::iterator it = stub.providers.begin(); it != stub.providers.end(); ++it){
		if (*it == this){
			for(UTTimerStub::Timers::iterator t = stub.timers.begin(); t != stub.timers.end(); ++t){
				if ((*t)->IsStarted() && (*t)->GetMode() == UTTimer::FRAMEWORK && (*t)->provider == this){
					(*t)->Stop();
				}
			}
			stub.providers.erase(it);
			return;
		}
	}
}
void UTTimerProvider::CallIdle(){
	UTTimerStub& stub = UTTimerStub::Get();
	for(UTTimerStub::Timers::iterator it = stub.timers.begin(); it != stub.timers.end(); ++it){
		if ((*it)->GetMode() == UTTimer::IDLE && (*it)->IsStarted()) (*it)->Call();
	}
}



//----------------------------------------------------------------------------------------------------------
//	UTTimer
//

UTTimer::UTTimer(): bStarted(false), bRunning(false), mode(FRAMEWORK),
	func(NULL), arg(NULL), interval(1), resolution(1),
	timerId(0), bStopThread(false), provider(NULL)
{
	UTTimerStub::Get().timers.push_back(this);
};
UTTimer::~UTTimer(){
	Stop();
	UTTimerStub::Timers& timers = UTTimerStub::Get().timers;
	for(UTTimerStub::Timers::iterator it = timers.begin(); it != timers.end(); ++it){
		if (*it == this){
			timers.erase(it);
			break;
		}
	}
};
bool UTTimer::SetMode(Mode m){
	if (mode == m) return true;
	bool b = IsStarted();
	if (b) if (!Stop()) return false;
	mode = m;
	if (b) Start();
	return true;
}

#if defined _WIN32

void SPR_STDCALL UTTimer_MMTimerCallback(unsigned uID, unsigned, unsigned long dwUser, unsigned long, unsigned long){
	((UTTimer*)dwUser)->Call();
}
unsigned long SPR_STDCALL UTTimer_ThreadCallback(void* arg){
	UTTimer* timer = (UTTimer*)arg;
	unsigned long lastCall = timeGetTime();
	while(!timer->bStopThread){
		unsigned long now = timeGetTime();
		unsigned long nextCall = lastCall + timer->GetInterval();
		int delta = (int)nextCall - (int)now;
		if (delta > 0){
			Sleep(delta);
			now = timeGetTime();
		}
 		lastCall = now;
		timer->Call();
	}
	timer->bStopThread = false;
	return 0;
}
bool UTTimer::Start(){
	UTTimerStub& stub =  UTTimerStub::Get();
	if (bStarted) return true;
	heavy = 0;
	if (mode==MULTIMEDIA){
		bStarted = true;
		stub.UpdateResolution();
		timerId = timeSetEvent(interval, resolution, UTTimer_MMTimerCallback, (unsigned long)this, TIME_PERIODIC);
		if (!timerId){
			bStarted = false;
			stub.UpdateResolution();
		}
	}else if(mode==THREAD){
		unsigned long id=0;
		timerId = (unsigned)CreateThread(NULL, 0x1000, UTTimer_ThreadCallback, this, 0, &id);
		if (timerId){
			SetThreadPriority((HANDLE)timerId, THREAD_PRIORITY_TIME_CRITICAL);//THREAD_PRIORITY_ABOVE_NORMAL);
			bStarted = true;
		}
	}else if (mode==FRAMEWORK){
		bStarted = true;
		for(UTTimerStub::Providers::iterator it = stub.providers.begin(); it != stub.providers.end(); ++it){
			if (!*it) continue;
			if ((*it)->StartTimer(this)){
				provider = *it;
				return bStarted;
			}
		}
		bStarted = false;
	}else if (mode==IDLE){
		bStarted = true;
	}
	return bStarted;
}
bool UTTimer::Stop(){
	UTTimerStub& stub =  UTTimerStub::Get();
	if (!bStarted) return true;
	if (mode == MULTIMEDIA){
		timeKillEvent(timerId);
		for(int i=0; bRunning && i<100; i++) Sleep(10); 
		if (bRunning){
			DSTR << "UTTimer MULTIMEDIA mode: Can not stop the timer callback. There may be a dead lock problem." << std::endl;
		}
		timerId = 0;
		bStarted = false;
		stub.UpdateResolution();
	}else if (mode == THREAD){
		bStopThread = true;									//	スレッドの停止を指示
		for(int t=0; t<100 && bStopThread; t++) Sleep(20);	//	停止するまで待ってみる
		if (bStopThread){
			DSTR << "UTTimer THREAD mode: Can not stop the timer thread. There may be a dead lock problem." << std::endl;
		}
		CloseHandle(*(HANDLE*)&timerId);
		timerId = 0;
		bStarted = false;
	}else if (mode == FRAMEWORK){
		bStarted = false;
		if (provider && provider->StopTimer(this)){
			timerId = 0;
			provider = NULL;
		}else{
			bStarted = true;
		}
	}else if (mode==IDLE){
		bStarted = false;
	}
	return !bStarted;
}
#else	//	Windows以外のプラットフォームでの実装

#error UTTimer: Not yet implemented.		//	未実装

#endif

bool UTTimer::SetCallback(TimerFunc* f, void* a){
	bool b = IsRunning();
	if (b) if (!Stop()) return false;
	func = f;
	arg = a;
	if (b) Start();
	return true;
}
bool UTTimer::SetInterval(unsigned i){
	bool b = IsRunning();
	if (b) if (!Stop()) return false;
	interval = i;
	if (b) Start();
	return true;
}
bool UTTimer::SetResolution(unsigned r){
	r = min(r, UTTimerStub::Get().ResolutionMax());
	r = max(r, UTTimerStub::Get().ResolutionMin());
	resolution = r;
	if (bStarted && mode == MULTIMEDIA){
		UTTimerStub::Get().UpdateResolution();
	}
	return true;
}

void UTTimer::Call(){
	if (func && !bRunning){
		bRunning = true;
		func(arg);
		bRunning = false;
	}
}


}	//	namespace Spr

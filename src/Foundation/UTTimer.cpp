#include "Foundation.h"
#pragma hdrstop
#include <Foundation/UTTimer.h>

#ifdef _WIN32
# include <windows.h>
# include <mmsystem.h>
#endif

namespace Spr {;

typedef unsigned int	uint;
typedef unsigned long	ulong;

//----------------------------------------------------------------------------------------------------------
//	UTTimerStub		UTTimerのインスタンスを保持するシングルトン
//
class UTTimerStub{
	///< マルチメディアタイマの分解能
	uint resolution;		///< 現在の設定値
	uint resolutionMin;		///< システムがサポートする最小値
	uint resolutionMax;		///< システムがサポートする最大値
public:
	typedef std::vector<UTTimerProvider*> Providers;
	typedef std::vector< UTRef<UTTimer> > Timers;

	Providers	providers;		///< プロバイダの配列
	Timers		timers;			///< タイマの配列

	UTTimerStub(): resolution(0), resolutionMin(0), resolutionMax(0){}

public:
	/// 唯一のインスタンスを取得
	static UTTimerStub& UTTimerStub::Get(){
		static UTTimerStub obj;
		return obj;
	}
	/// タイマの登録
	void AddTimer(UTTimer* timer){
		timer->timerId = timers.size();
		timers.push_back(timer);
	}
	/// マルチメディアタイマの最小分解能
	uint ResolutionMin(){
		if (!resolutionMin) GetCaps();
		return resolutionMin;
	}
	/// マルチメディアタイマの最大分解能
	uint ResolutionMax(){
		if (!resolutionMax) GetCaps();
		return resolutionMax;
	}
	/** @brief 分解能の再設定
	 */
	void UpdateResolution(){
#if defined(_WIN32)
		int resOld = resolution;
		resolution = UINT_MAX;
		for(Timers::iterator it = timers.begin(); it!=timers.end(); ++it){
			if ((*it)->IsStarted() && (*it)->GetMode() == UTTimerIf::MULTIMEDIA && (*it)->GetResolution() < resolution) 
				resolution = (*it)->GetResolution();
		}
		if (resolution == UINT_MAX) resolution = 0;

		if (resOld == resolution) return;
		if (resOld) timeEndPeriod(resOld);
		if (resolution) timeBeginPeriod(resolution);
#else
# error UTMMTimerStub: Not yet implemented.	//	未実装
#endif
	}

protected:
	void GetCaps(){
#ifdef _WIN32
		TIMECAPS tc;
		if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
			DSTR << "UTTimer::BeginPeriod()  Fail to get resolution." << std::endl;
		}
		resolutionMin = tc.wPeriodMin;
		resolutionMax = tc.wPeriodMax;
#else
# error UTMMTimerStub: Not yet implemented.	//	未実装
#endif
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
			// このプロバイダを利用しているタイマを停止する
			for(UTTimerStub::Timers::iterator t = stub.timers.begin(); t != stub.timers.end(); ++t){
				if ((*t)->IsStarted() && (*t)->GetMode() == UTTimerIf::FRAMEWORK && (*t)->provider == this){
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
		if ((*it)->GetMode() == UTTimerIf::IDLE && (*it)->IsStarted()) (*it)->Call();
	}
}


//----------------------------------------------------------------------------------------------------------
//	UTTimer
//

UTTimer::UTTimer(){
	bStarted	= false;
	bRunning	= false;
	bStopThread = false;
	mode		= UTTimerIf::FRAMEWORK;
	interval	= 1;
	resolution	= 1;
	func		= 0;
	timerIdImpl = 0;
	provider	= 0;

	// 自身をStubに登録
	UTTimerStub::Get().AddTimer(this);
	
}

UTTimer::~UTTimer(){
	Stop();
	/*
	UTTimerStub::Timers& timers = UTTimerStub::Get().timers;
	for(UTTimerStub::Timers::iterator it = timers.begin(); it != timers.end(); ++it){
		if (*it == this){
			timers.erase(it);
			break;
		}
	}*/
}

unsigned SPR_CDECL UTTimerIf::NTimers(){
	return UTTimerStub::Get().timers.size();
}

UTTimerIf* SPR_CDECL UTTimerIf::Get(unsigned i){
	if(0 <= i && i < NTimers())
		return DCAST(UTTimerIf, UTTimerStub::Get().timers[i]);
	return 0;
}

UTTimerIf* SPR_CDECL UTTimerIf::Create(){
	UTTimer* timer = DBG_NEW UTTimer();
	
	return DCAST(UTTimerIf, timer);
}

bool UTTimer::SetMode(UTTimerIf::Mode m){
	if (mode == m)
		return true;
	
	bool started = IsStarted();
	if (started && !Stop())
		return false;

	mode = m;
	return Start();
}

#if defined _WIN32
void SPR_STDCALL UTTimer_MMTimerCallback(uint uID, uint, ulong dwUser, ulong, ulong){
	UTTimerStub::Get().timers[dwUser]->Call();
}

ulong SPR_STDCALL UTTimer_ThreadCallback(void* arg){
	UTTimer* timer = UTTimerStub::Get().timers[(int)arg];
	ulong lastCall = timeGetTime();
	
	while(!timer->bStopThread){
		ulong now = timeGetTime();
		ulong nextCall = lastCall + timer->GetInterval();
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
#else	//	Windows以外のプラットフォームでの実装
# error UTTimer: Not yet implemented.		//	未実装
#endif

bool UTTimer::Start(){
	if (bStarted)
		return true;
	
	UTTimerStub& stub =  UTTimerStub::Get();
	if (mode == UTTimerIf::MULTIMEDIA){
#if defined _WIN32
		bStarted = true;
		stub.UpdateResolution();
		timerIdImpl = timeSetEvent(interval, resolution, UTTimer_MMTimerCallback, timerId, TIME_PERIODIC);
		if (!timerIdImpl){
			bStarted = false;
			stub.UpdateResolution();
		}
#endif
	}
	else if(mode == UTTimerIf::THREAD){
#if defined _WIN32
		ulong id=0;
		timerIdImpl = (uint)CreateThread(NULL, 0x1000, UTTimer_ThreadCallback, (void*)timerId, 0, &id);
		if (timerIdImpl){
			SetThreadPriority((HANDLE)timerIdImpl, THREAD_PRIORITY_TIME_CRITICAL);//THREAD_PRIORITY_ABOVE_NORMAL);
			bStarted = true;
		}
#endif
	}
	else if (mode == UTTimerIf::FRAMEWORK){
		bStarted = true;
		for(UTTimerStub::Providers::iterator it = stub.providers.begin(); it != stub.providers.end(); ++it){
			if (!*it) continue;
			if ((*it)->StartTimer(this)){
				provider = *it;
				return bStarted;
			}
		}
		bStarted = false;
	}
	else if (mode == UTTimerIf::IDLE){
		bStarted = true;
	}
	return bStarted;
}

bool UTTimer::Stop(){
	if (!bStarted)
		return true;

	UTTimerStub& stub =  UTTimerStub::Get();
	
	if (mode == UTTimerIf::MULTIMEDIA){
#ifdef _WIN32
		timeKillEvent(timerIdImpl);
		for(int i=0; bRunning && i<100; i++) Sleep(10); 
		if (bRunning)
			DSTR << "UTTimer MULTIMEDIA mode: Can not stop the timer callback. There may be a dead lock problem." << std::endl;
		timerIdImpl = 0;
		bStarted = false;
		stub.UpdateResolution();
#endif
	}
	else if (mode == UTTimerIf::THREAD){
#ifdef _WIN32
		bStopThread = true;									//	スレッドの停止を指示
		for(int t=0; t<100 && bStopThread; t++) Sleep(20);	//	停止するまで待ってみる
		if (bStopThread)
			DSTR << "UTTimer THREAD mode: Can not stop the timer thread. There may be a dead lock problem." << std::endl;
		CloseHandle(*(HANDLE*)&timerIdImpl);
		timerIdImpl = 0;
		bStarted = false;
#endif
	}
	else if (mode == UTTimerIf::FRAMEWORK){
		bStarted = false;
		if (provider && provider->StopTimer(this)){
			timerIdImpl = 0;
			provider = NULL;
		}
		else{
			bStarted = true;
		}
	}
	else if (mode == UTTimerIf::IDLE){
		bStarted = false;
	}
	return !bStarted;
}

bool UTTimer::SetCallback(UTTimerIf::TimerFunc f){
	if (IsRunning() && !Stop())
		return false;
	func = f;
	return Start();
}

bool UTTimer::SetInterval(uint i){
	if (IsRunning() && !Stop())
		return false;
	interval = i;
	return Start();
}

bool UTTimer::SetResolution(unsigned r){
	r = min(r, UTTimerStub::Get().ResolutionMax());
	r = max(r, UTTimerStub::Get().ResolutionMin());
	resolution = r;
	if (bStarted && mode == UTTimerIf::MULTIMEDIA){
		UTTimerStub::Get().UpdateResolution();
	}
	return true;
}

void UTTimer::Call(){
	if (func && !bRunning){
		bRunning = true;
		(*func)(timerId);
		bRunning = false;
	}
}

}	//	namespace Spr

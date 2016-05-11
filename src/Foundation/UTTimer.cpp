/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTTimer.h>

#ifdef _WIN32
# include <windows.h>
# include <mmsystem.h>
#endif

namespace Spr {;

//----------------------------------------------------------------------------------------------------------
//	UTTimerStub		UTTimerのインスタンスを保持するシングルトン
//
class UTTimerStub{
	///< マルチメディアタイマの分解能
	unsigned int resolution;		///< 現在の設定値
	unsigned int resolutionMin;		///< システムがサポートする最小値
	unsigned int resolutionMax;		///< システムがサポートする最大値
public:
	typedef std::vector<UTTimerProvider*> Providers;
	typedef std::vector< UTRef<UTTimer> > Timers;

	Providers	providers;		///< プロバイダの配列
	Timers		timers;			///< タイマの配列

	UTTimerStub(): resolution(0), resolutionMin(0), resolutionMax(0){}
	~UTTimerStub(){
		for(unsigned i=0; i<timers.size(); ++i) timers[i]->Stop();
	}
public:
	/// 唯一のインスタンスを取得
	static UTTimerStub& UTTimerStub::Get(){
		static UTTimerStub obj;
		return obj;
	}
	/// タイマの登録
	void AddTimer(UTTimer* timer){
		timer->timerId = (unsigned int)timers.size();
		timers.push_back(timer);
	}
	/// マルチメディアタイマの最小分解能
	unsigned int ResolutionMin(){
		if (!resolutionMin) GetCaps();
		return resolutionMin;
	}
	/// マルチメディアタイマの最大分解能
	unsigned int ResolutionMax(){
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
		TIMECAPS tc = { 0, 0 };
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
	// このプロバイダを利用しているタイマを停止する
	for(unsigned i=0; i<stub.timers.size(); ++i){
		UTTimer* t = stub.timers[i];
		if (t->IsStarted() && t->GetMode() == UTTimerIf::FRAMEWORK && t->provider == this){
			t->Stop();
		}
	}
	for(unsigned i=0; i<stub.providers.size(); ++i){
		UTTimerProvider* p = stub.providers[i];
		if (p == this){
			stub.providers.erase(stub.providers.begin()+i);
			break;
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
	UTTimerStub::Timers& timers = UTTimerStub::Get().timers;
	//	ここでAddRefしておかないと、 timers.erase()した瞬間に~UTTimer()に再入してしまう。
	//	newしていないUTTimerの場合、最初から0なので2にしておかないと再入してしまう。
	AddRef(); AddRef();	
	for(unsigned int i=0; i<timers.size(); ++i){
		if (timers[i] == this){
			timers.erase(timers.begin()+i);
			break;
		}
	}
	//	どうせすぐ消えるので意味ないけど、この後の処理で困るように将来なるかもしれないので一応DelRef()呼んどく。
	//	もともと0の場合は、2回DelRef()を呼ぶと-1になってしまうので RefCount()をチェックする。
	DelRef(); 
	if (RefCount()>0) DelRef(); 
}

unsigned SPR_CDECL UTTimerIf::NTimers(){
	return (unsigned)UTTimerStub::Get().timers.size();
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
void (__stdcall UTTimer_MMTimerCallback)(unsigned uID, unsigned, ulong_ptr dwUser, ulong_ptr, ulong_ptr){
	UTTimerStub::Get().timers[(int)dwUser]->Call();
}

unsigned long SPR_STDCALL UTTimer_ThreadCallback(void* arg){
#pragma warning (push)
#pragma warning (disable : 4302 4311)
	UTTimer* timer = UTTimerStub::Get().timers[(int)arg];
#pragma warning (pop)
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
		assert(interval != 0);	// interval == 0だと動かない
		timerIdImpl = timeSetEvent(interval, resolution, UTTimer_MMTimerCallback , timerId, TIME_PERIODIC);
		if (!timerIdImpl){
			bStarted = false;
			stub.UpdateResolution();
		}
#endif
	}
	else if(mode == UTTimerIf::THREAD){
#if defined _WIN32
#pragma warning (push)
#pragma warning (disable : 4302 4311 4312)
		unsigned long id=0;
		timerIdImpl = (unsigned int)CreateThread(NULL, 0x1000, UTTimer_ThreadCallback, (void*)timerId, 0, &id);
		if (timerIdImpl){
			SetThreadPriority((HANDLE)timerIdImpl, THREAD_PRIORITY_TIME_CRITICAL);//THREAD_PRIORITY_ABOVE_NORMAL);
			bStarted = true;
		}
#pragma warning (pop)
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
		// 実行中のコールバックの終了を待つ
		for(int i=0; bRunning && i<100; i++)
			Sleep(10); 
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
		if (provider && provider->StopTimer(this)){
			timerIdImpl = 0;
			provider = NULL;
			bStarted = false;
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

bool UTTimer::SetCallback(UTTimerIf::TimerFunc f, void* a){
	if (IsStarted() && !Stop())
		return false;
	func = f;
	arg = a;
	return Start();
}
UTTimerIf::TimerFunc UTTimer::GetCallback(){
	return func;
}
void* UTTimer::GetCallbackArg(){
	return arg;
}

bool UTTimer::SetInterval(unsigned int i){
	if (IsStarted() && !Stop())
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
		(*func)(timerId, arg);
		bRunning = false;
	}
}

}	//	namespace Spr

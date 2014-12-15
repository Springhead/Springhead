#include "Foundation.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "UTMMTimer.h"
#include <windows.h>
#include <mmsystem.h>

namespace Spr {

int UTMMTimer::count;
unsigned UTMMTimer::resolution=1;
UTMMTimer::UTMMTimer()
	{
	func = NULL;
	idFunc = NULL;
	arg = NULL;
	id	= NULL;
	timerID = 0;
	interval = resolution;
	if (interval==0) interval = 1;
    bRun = false;
	bCreated = false;
	bThread = false;
	hThread = NULL;
	heavy = 0;
	}
UTMMTimer::~UTMMTimer()
	{
	Release();
	}
bool UTMMTimer::Create()
	{
	heavy = 0;
	tick = timeGetTime();
	if (bCreated) return true;
	if (bThread) Release();
	if (count == 0) BeginPeriod();
	count ++;
#if (_MSC_VER >= 1700)
	timerID = timeSetEvent(interval, resolution, TimerCallback, (DWORD_PTR)this, TIME_PERIODIC);
#else
	timerID = timeSetEvent(interval, resolution, TimerCallback, (unsigned long)this, TIME_PERIODIC);
#endif
	bCreated = (timerID != 0);
	return bCreated;
	}
bool UTMMTimer::Thread()
	{
	heavy = 0;
	if (bThread) return true;
	if (bCreated) Release();
	unsigned long id=0;
	bThread = true;
	hThread = CreateThread(NULL, 0x1000, ThreadCallback, this, 0, &id);
	if (hThread){
		bRun = true;
		SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);//THREAD_PRIORITY_ABOVE_NORMAL);
	}
	else bThread = false;
	return bThread;
	}
void UTMMTimer::Release()
	{
	if (bThread)
		{
		bThread = false;
		for(int t=0; t<100 && bRun; t++) Sleep(20);
		CloseHandle(hThread);
		hThread = NULL;
		}
	if (bCreated)
		{
		timeKillEvent(timerID);
		for(int i=0; i<100; i++)
			{
			if (!bRun) break;
			Sleep(10); 
			}
		count --;
		if (count == 0)	EndPeriod();
		bCreated = false;
		}
	}
void UTMMTimer::BeginPeriod()
	{
	TIMECAPS tc;
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		{
		OutputDebugString("UTMMTimer::Resolution()  Fail to get resolution.\n");
		assert(0);
		}
	resolution = min(max(tc.wPeriodMin, resolution), tc.wPeriodMax);
	timeBeginPeriod(resolution);
	}
void UTMMTimer::EndPeriod()
	{
	timeEndPeriod(resolution);
	}
#if (_MSC_VER >= 1700)
void SPR_STDCALL UTMMTimer::TimerCallback(UINT uID, UINT, DWORD_PTR dwUser, DWORD_PTR, DWORD_PTR)
#else
void SPR_STDCALL UTMMTimer::TimerCallback(unsigned uID, unsigned, unsigned long dwUser, unsigned long, unsigned long)
#endif
	{
	UTMMTimer& mmtimer = *(UTMMTimer*)dwUser;
#if 0
    int tick = timeGetTime();
	int delta = tick - mmtimer.tick;
	if (delta > mmtimer.interval+1) mmtimer.heavy ++;
	mmtimer.tick = tick;
#endif
	mmtimer.bRun = true;
	if(mmtimer.func!=NULL)mmtimer.func(mmtimer.arg);
	if(mmtimer.idFunc!=NULL)mmtimer.idFunc(mmtimer.id);
    mmtimer.bRun = false;
	}
unsigned long SPR_STDCALL UTMMTimer::ThreadCallback(void* arg){
	UTMMTimer& mmtimer = *(UTMMTimer*)arg;

	unsigned long lastCall = timeGetTime();
	while(mmtimer.bThread){
		unsigned long now = timeGetTime();
		unsigned long nextCall = lastCall + mmtimer.interval;
		if (int(nextCall - now) > 0){
			Sleep(nextCall - now);
		}
 		lastCall = now;
		
		if(mmtimer.func!=NULL)mmtimer.func(mmtimer.arg);
		if(mmtimer.idFunc!=NULL)mmtimer.idFunc(mmtimer.id);
	}
	mmtimer.bRun = false;
	return 0;
}
unsigned UTMMTimer::Resolution()
	{
	return resolution;
	}
void UTMMTimer::Resolution(unsigned res)
	{
	if (resolution == res) return;
	if (count)
		{
		EndPeriod();
		BeginPeriod();
		}
	}
unsigned UTMMTimer::Interval()
	{
	return interval;
	}
void UTMMTimer::Interval(unsigned i)
	{
	if (i == interval) return;
	interval = i;
	if (bCreated)
		{
		Release();
		Create();
		}
	}
void UTMMTimer::Set(MMTimerFunc* f, void* a)
	{
	func = f;
	arg = a;
	}
void UTMMTimer::Set(MMTimerIdFunc* f,int i)
	{
	idFunc = f;
	id = i;
	}
void UTMMTimer::Init(unsigned int r, unsigned int i, MMTimerFunc* f, void* arg){
	Resolution(r);
	Interval(i);
	Set(f, arg);
}
}	//	namespace Spr

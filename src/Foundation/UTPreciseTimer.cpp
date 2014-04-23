/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#pragma comment(lib, "winmm.lib")

#include "UTPreciseTimer.h"
#include <Base/BaseDebug.h>
#include <Windows.h>
#include <mmsystem.h>

namespace Spr{;
unsigned long UTPreciseTimer::freq;

//生成時に1秒休み，周波数をカウントする
UTPreciseTimer::UTPreciseTimer(): stopWatch(0), startFlag(false){
	cycles2.quadPart = 0;
	if (freq == 0){
#ifndef __BORLANDC__
		Init(50);
		Init();
#else
		freq = 100 * 1000;
        freq *= 1000*1000;
#endif
	}
}

void UTPreciseTimer::Init(int preiod){
	UTLargeInteger cycles;
	unsigned long time = timeGetTime();
    unsigned long lowPart, highPart;
#ifdef _M_IX86
	_asm{
		CPUID
		RDTSC								;// クロックカウンタを読む
		MOV		lowPart,	EAX				;// カウンタを保存
		MOV		highPart,	EDX				;// カウンタを保存
	}
    cycles.lowPart = lowPart;
    cycles.highPart = highPart;
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&cycles);
#endif
	cycles2.quadPart = cycles.quadPart;
	//	1秒待つ
	int deltaTime;
	while(1){;
		deltaTime = timeGetTime() - time;
		if (deltaTime > preiod) break;
	}
#ifdef _M_IX86
	_asm{
		CPUID
		RDTSC								;// クロックカウンタを読む
		MOV		lowPart,	EAX				;// カウンタを保存
		MOV		highPart,	EDX				;// カウンタを保存
	}
    cycles.lowPart = lowPart;
    cycles.highPart = highPart;
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&cycles);
#endif
	freq = unsigned long(cycles.quadPart - cycles2.quadPart);
	freq = unsigned long(freq * (1000.0 / deltaTime));
#ifdef _DEBUG
	DSTR << "UTPreciseTimer CPU freq:" << freq;
#endif
}

void UTPreciseTimer::WaitUS(int time){
    unsigned long lowPart, highPart;
	UTLargeInteger cycles;
#ifdef _M_IX86
	_asm{
		CPUID;
		RDTSC								;// クロックカウンタを読む
		MOV     lowPart, EAX                ;// カウンタを保存
		MOV     highPart, EDX               ;// カウンタを保存
	}
	cycles.lowPart = lowPart;
	cycles.highPart = highPart;
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&cycles);
#endif
	cycles2.quadPart = cycles.quadPart + (__int64)time*freq/1000000;
	do{
#ifdef _M_IX86
		_asm{
			CPUID;
			RDTSC							;// クロックカウンタを読む
			MOV     lowPart, EAX			;// カウンタを保存
			MOV     highPart, EDX			;// カウンタを保存
		}
        cycles.lowPart = lowPart;
        cycles.highPart = highPart;
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&cycles);
#endif
	}while(cycles2.quadPart>cycles.quadPart);
}


int  UTPreciseTimer::CountUS()
{
	int retval=1;
	UTLargeInteger cycles;
#ifdef _M_IX86
	unsigned long lowPart, highPart;
	_asm{
		CPUID;
		RDTSC								;// クロックカウンタを読む
		MOV     lowPart, EAX				;// カウンタを保存
		MOV     highPart, EDX				;// カウンタを保存
	}
	cycles.lowPart = lowPart;
	cycles.highPart = highPart;
#else
	QueryPerformanceCounter((LARGE_INTEGER*)&cycles);
#endif
	retval =  (int)(((cycles.quadPart-cycles2.quadPart)*1000000 / freq) & 0xffffffff);
	cycles2.quadPart = cycles.quadPart;
	return retval;
}


void UTPreciseTimer::CountAndWaitUS(int time)
{
	int elapsedtime;
	elapsedtime = CountUS();
	WaitUS(time - elapsedtime);
}

unsigned long UTPreciseTimer::Start(){
	CountUS();
	startFlag = true;
	return stopWatch;
}
unsigned long UTPreciseTimer::Stop(){
	if (startFlag){
		stopWatch += CountUS();
		startFlag = false;
	}
	return stopWatch;
}
unsigned long UTPreciseTimer::Clear(){
	unsigned long rv = stopWatch;
	stopWatch = 0;
	return rv;
}


}

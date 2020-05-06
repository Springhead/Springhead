/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "WinBasis.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "WBPreciseTimer.h"
#include <MMSystem.h>

namespace Spr{;
DWORD WBPreciseTimer::freq;

//生成時に1秒休み，周波数をカウントする
WBPreciseTimer::WBPreciseTimer(): stopWatch(0), startFlag(false){
	cycles2.QuadPart = 0;
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

void WBPreciseTimer::Init(int preiod){
	LARGE_INTEGER cycles;
	DWORD time = timeGetTime();
    DWORD lowPart, highPart;
	_asm{
		CPUID
		RDTSC								;// クロックカウンタを読む
		MOV		lowPart,	EAX				;// カウンタを保存
		MOV		highPart,	EDX				;// カウンタを保存
	}
    cycles.LowPart = lowPart;
    cycles.HighPart = highPart;
	cycles2.QuadPart = cycles.QuadPart;
	//	1秒待つ
	int deltaTime;
	while(1){;
		deltaTime = timeGetTime() - time;
		if (deltaTime > preiod) break;
	}
	_asm{
		CPUID
		RDTSC								;// クロックカウンタを読む
		MOV		lowPart,	EAX				;// カウンタを保存
		MOV		highPart,	EDX				;// カウンタを保存
	}
    cycles.LowPart = lowPart;
    cycles.HighPart = highPart;
	freq = DWORD(cycles.QuadPart - cycles2.QuadPart);
	freq = DWORD(freq * (1000.0 / deltaTime));
#ifdef _DEBUG
	DSTR << "WBPreciseTimer CPU freq:" << freq << std::endl;
#endif
}

void WBPreciseTimer::WaitUS(int time){
#ifndef __BORLANDC__
    DWORD lowPart, highPart;
	LARGE_INTEGER cycles;
	_asm{
		CPUID;
		RDTSC								;// クロックカウンタを読む
		MOV     lowPart, EAX                ;// カウンタを保存
		MOV     highPart, EDX               ;// カウンタを保存
	}
	cycles.LowPart = lowPart;
	cycles.HighPart = highPart;
	cycles2.QuadPart = cycles.QuadPart + (__int64)time*freq/1000000;
	do{
		_asm{
			CPUID;
			RDTSC							;// クロックカウンタを読む
			MOV     lowPart, EAX			;// カウンタを保存
			MOV     highPart, EDX			;// カウンタを保存
		}
        cycles.LowPart = lowPart;
        cycles.HighPart = highPart;
	}while(cycles2.QuadPart>cycles.QuadPart);
#endif
}


int  WBPreciseTimer::CountUS()
{
	int retval=1;
#ifndef __BORLANDC__
	LARGE_INTEGER cycles;
    DWORD lowPart, highPart;
	_asm{
		CPUID;
		RDTSC								;// クロックカウンタを読む
		MOV     lowPart, EAX				;// カウンタを保存
		MOV     highPart, EDX				;// カウンタを保存
	}
	cycles.LowPart = lowPart;
	cycles.HighPart = highPart;
	retval =  (int)((cycles.QuadPart-cycles2.QuadPart)*1000000 / freq);
	cycles2.QuadPart = cycles.QuadPart;
#endif
	return retval;
}


void WBPreciseTimer::CountAndWaitUS(int time)
{
	int elapsedtime;
	elapsedtime = CountUS();
	WaitUS(time - elapsedtime);
}

DWORD WBPreciseTimer::Start(){
	CountUS();
	startFlag = true;
	return stopWatch;
}
DWORD WBPreciseTimer::Stop(){
	if (startFlag){
		stopWatch += CountUS();
		startFlag = false;
	}
	return stopWatch;
}
DWORD WBPreciseTimer::Clear(){
	DWORD rv = stopWatch;
	stopWatch = 0;
	return rv;
}


}

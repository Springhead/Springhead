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
	_asm{
		CPUID
		RDTSC								;// クロックカウンタを読む
		MOV		lowPart,	EAX				;// カウンタを保存
		MOV		highPart,	EDX				;// カウンタを保存
	}
    cycles.lowPart = lowPart;
    cycles.highPart = highPart;
	cycles2.quadPart = cycles.quadPart;
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
    cycles.lowPart = lowPart;
    cycles.highPart = highPart;
	freq = unsigned long(cycles.quadPart - cycles2.quadPart);
	freq = unsigned long(freq * (1000.0 / deltaTime));
#ifdef _DEBUG
	DSTR << "UTPreciseTimer CPU freq:" << freq;
#endif
}

void UTPreciseTimer::WaitUS(int time){
#ifndef __BORLANDC__
    unsigned long lowPart, highPart;
	UTLargeInteger cycles;
	_asm{
		CPUID;
		RDTSC								;// クロックカウンタを読む
		MOV     lowPart, EAX                ;// カウンタを保存
		MOV     highPart, EDX               ;// カウンタを保存
	}
	cycles.lowPart = lowPart;
	cycles.highPart = highPart;
	cycles2.quadPart = cycles.quadPart + (__int64)time*freq/1000000;
	do{
		_asm{
			CPUID;
			RDTSC							;// クロックカウンタを読む
			MOV     lowPart, EAX			;// カウンタを保存
			MOV     highPart, EDX			;// カウンタを保存
		}
        cycles.lowPart = lowPart;
        cycles.highPart = highPart;
	}while(cycles2.quadPart>cycles.quadPart);
#endif
}


int  UTPreciseTimer::CountUS()
{
	int retval=1;
#ifndef __BORLANDC__
	UTLargeInteger cycles;
    unsigned long lowPart, highPart;
	_asm{
		CPUID;
		RDTSC								;// クロックカウンタを読む
		MOV     lowPart, EAX				;// カウンタを保存
		MOV     highPart, EDX				;// カウンタを保存
	}
	cycles.lowPart = lowPart;
	cycles.highPart = highPart;
	retval =  (int)((cycles.quadPart-cycles2.quadPart)*1000000 / freq);
	cycles2.quadPart = cycles.quadPart;
#endif
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

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#pragma comment(lib, "winmm.lib")

#include "UTQPTimer.h"
#include <Base/BaseDebug.h>
#ifdef	_WIN32
#  include <Windows.h>
#  include <mmsystem.h>
#else
#  include <stdio.h>
#  include <unistd.h>
#  include <sys/time.h>
#endif

namespace Spr{;

#ifdef	__linux__
typedef	unsigned long	DWORD;		// 32-bit unsigned integer
typedef	long		LONG;		// 32-bit signed integer
typedef	long long	LONGLONG;	// 64-bit signed integer
typedef union _LARGE_INTEGER {
	struct {
		DWORD	LowPart;
		LONG	HighPart;
	};
	LONGLONG	QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
static void QueryPerformanceCounter(LARGE_INTEGER* c) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	c->QuadPart = (LONGLONG) (tv.tv_sec * 1000000) + tv.tv_usec;
}
extern unsigned int sleep(unsigned int);	// ??
static int QueryPerformanceFrequency(LARGE_INTEGER* f) {
	LARGE_INTEGER c1, c2;
	unsigned int seconds = 1;
	QueryPerformanceCounter(&c1);
	sleep(seconds);
	QueryPerformanceCounter(&c2);
	f->QuadPart = c2.QuadPart - c1.QuadPart;
	return (int) f->QuadPart;
}
#endif

//周波数を読み出す
UTQPTimer::UTQPTimer(): stopWatch(0), startFlag(false)
{
	LARGE_INTEGER f;
	QueryPerformanceFrequency( &(f) );
	freq.quadPart = f.QuadPart;
}

int UTQPTimer::Freq(){
	return (int)freq.quadPart;
} 
//μ秒単位で待つ
void UTQPTimer::WaitUS(int time)
{
	LARGE_INTEGER time1,time2;

	QueryPerformanceCounter( &time1);
	time2.QuadPart = time1.QuadPart + time * freq.quadPart / 1000000;
	do{
		QueryPerformanceCounter( &time1);
	}while(time1.QuadPart < time2.QuadPart);
}

//前回呼び出されてからの時間をμ秒単位で計測
int  UTQPTimer::CountUS()
{	
	LARGE_INTEGER now;
	int retval;

	QueryPerformanceCounter( &now);
	
	retval =  (int)(((now.QuadPart-lasttime.quadPart)*1000000 / freq.quadPart) & 0xffffffff);
	lasttime.quadPart = now.QuadPart;
	return retval;
}

//指定ループ時間を作成
void UTQPTimer::CountAndWaitUS(int time)
{
	int elapsedtime;
	elapsedtime = CountUS();
	WaitUS(time - elapsedtime);
}


unsigned long UTQPTimer::Start(){
	if(!startFlag){
		CountUS();
		startFlag = true;
	}
	return stopWatch;
}
unsigned long UTQPTimer::GetTime(){
	if (startFlag){
		stopWatch += CountUS();
	}
	return stopWatch;
}

unsigned long UTQPTimer::Stop(){
	if (startFlag){
		stopWatch += CountUS();
		startFlag = false;
	}
	return stopWatch;
}
unsigned long UTQPTimer::Clear(){
	unsigned long rv = stopWatch;
	stopWatch = 0;
	return rv;
}


UTQPTimerFileOut::UTQPTimerFileOut(double u){
	unit = u;
	Init();
}

void UTQPTimerFileOut::Init(){
	Clear();
	Start();
}

void UTQPTimerFileOut::StartPoint(std::string name){
	int id = ResizeDataArea(name);
	names[id].lastTime = GetTime();
}

void UTQPTimerFileOut::EndPoint(std::string name){
	int id = FindIdByName(name);
	if(id == -1) return;
	unsigned long interval = GetTime() - names[id].lastTime;
	data[id].push_back(interval);
}

void UTQPTimerFileOut::Interval(std::string start, std::string name){
	int startId = FindIdByName(start);
	int id = ResizeDataArea(name);
	unsigned long interval = GetTime() - names[startId].lastTime;
	data[id].push_back(interval);
}

void UTQPTimerFileOut::FileOut(std::string filename){
	std::ofstream ofs(filename.c_str());
	if (!ofs){
		DSTR << "Can not open the file : " << filename << std::endl;
		return;
	}

#if (_MSC_VER > 1500)
	ofs.precision(std::numeric_limits< unsigned long >::max_digits10);
#endif
	for (int i = 0; i < (int)names.size(); i++){
		ofs << names[i].name << "\t";
	}
	ofs << std::endl;

	size_t nRow = 0;
	for (size_t j = 0; j < data.size(); j++){
		nRow = nRow < data[j].size() ? data[j].size() : nRow;
	}
	// i:列, j:行
	// data[列][行]
	for (size_t i = 0; i < nRow; i++){
		for (size_t j = 0; j < names.size(); j++){
			// dataの行数よりもiが大きい場合
			if(data[j].size() -1 < i){
				ofs << 0 << "\t";
			}else{
				ofs << data[j][i] * unit << "\t";
			}
		}
		ofs << std::endl;
	}
	ofs.close();
	DSTR << "UTQPTimerFileOut::FileOut complete." << std::endl;
}

int UTQPTimerFileOut::FindIdByName(std::string name){
	for(size_t i = 0; i < names.size(); i++){
		if(names[i].name == name){
			return names[i].id;
		}
	}
	return -1;
}

int UTQPTimerFileOut::ResizeDataArea(std::string name){
	int id;
	id = FindIdByName(name);
	if(id > -1) return id;
	Name newname;
	id = (int)names.size();
	newname.id = id;
	newname.name = name;
	names.push_back(newname);

	std::vector< unsigned long > d;
	data.push_back(d);
	return id;
}

}

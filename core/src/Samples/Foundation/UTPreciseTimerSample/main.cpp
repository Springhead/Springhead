/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*
	UTPreciseTimerSample
	CPUクロックを使った時間の計測
	UTTimerのMultiMediaTimerモードを使って、1msの頻度でコールバック
	コールバックにかかる時間をUTPreceiseTimer、UTQPTimerを使い測定する。
	5秒たったら終了し、結果をCSV形式で出力する。
*/


#include <Springhead.h>
#include <iostream>
#include <ostream>
#include <vector>
#include <GL/glut.h>
#ifdef	_MSC_VER
  #include <conio.h>
  #include <mmsystem.h>
#else
  #include <sys/time.h>
  inline double gettimeofday_sec() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 1e-6;
  }
  #define DWORD unsigned int
  #define timeGetTime()	gettimeofday_sec()
  #include <unistd.h>
  #define Sleep(x) sleep(x/1000)
  #include <stdio.h>
  #define _kbhit() getchar()
#endif
#include <Foundation/UTPreciseTimer.h>
#include <Foundation/UTQPTimer.h>

using namespace Spr;

unsigned int dt = 1;	// 繰り返し間隔ms
int mt = 5000;			// 測定時間ms

UTPreciseTimer pTimer;		// μs単位で計測可能なタイマ
UTQPTimer qTimer;			// マルチスレッド対応版
std::vector< Vec2d > m_time;	// 計測時間

void CPSCounter(double intervalms, double periodms);
void SPR_CDECL CallBack(int id, void* arg){
	Vec2d sec;
#if 1
	// 1ループ間の計測用
	sec.x = pTimer.Stop() * 1e-6;
	sec.y = qTimer.Stop() * 1e-6;
	m_time.push_back(sec);
	pTimer.Clear();
	qTimer.Clear();
	pTimer.Start();
	qTimer.Start();
#else
	// あるアルゴリズムが終了するまでにかかる時間計測用
	pTimer.Clear();
	qTimer.Clear();
	pTimer.Start();
	qTimer.Start();
	for(int i = 0; i < 10; i++)
		std::cout << "Springhead2!" << std::endl;
	sec.x = pTimer.Stop() * 1e-6;
	sec.y = qTimer.Stop() * 1e-6;
	m_time.push_back(sec);
#endif
	//CPSCounter(dt, 1000);
}

// （オプション）CPSを表示する関数
// intervalms:理想割り込み時間 ms
// periodms:計測間隔 ms （1000だと1sec間の平均を表示）
void CPSCounter(double intervalms, double periodms){
	static int cnt=0;
	static DWORD lastTick;
	cnt++;
	double interval = timeGetTime() - lastTick;
	if (interval > periodms) {
		double cps = cnt / interval * 1000.0 / intervalms;
		lastTick = timeGetTime();
		cnt = 0;
		std::cout << "cps : " << cps << std::endl;		// cycle per sec
		std::cout << "sec : " << 1 /cps << std::endl;	// sec
	}
}

int __cdecl main(int argc, char* argv[]){
	pTimer.Init();								// 計測用タイマの初期化

	UTTimerIf* timer1;							// コールバックタイマ
	timer1 = UTTimerIf::Create();				// コールバックタイマの作成
	timer1->SetMode(UTTimerIf::MULTIMEDIA);		// マルチメディアモードに設定
	timer1->SetResolution(1);					//	呼びだし分解能ms
	timer1->SetInterval(dt);					// 呼びだし頻度ms
	timer1->SetCallback(CallBack, NULL);		// 呼びだす関数
	timer1->Start();							// タイマスタート
	std::cout << "Start the mearsurement." << std::endl;
	std::cout << "Pleas wait " << mt * 0.001 << " seconds." << std::endl; 
	Sleep(mt);									// 5sec間計測
	timer1->Stop();								// タイマストップ

	std::cout << "Saving the data to a file." << std::endl;

	// 計測データをcsvで出力
	CSVOUT << "count" << "," << "Precise timer [s]" << "," << "QPTimer [s]" << std::endl;
	for(size_t i = 0; i < m_time.size(); i++){
		CSVOUT << i << "," << m_time[i].x << "," << m_time[i].y << std::endl;
	}

	// 計測完了
	std::cout << "Complete!" << std::endl;
	std::cout << "Press any key to exit." << std::endl;
	while(!_kbhit()){}

	return 0;
}

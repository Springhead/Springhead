#include <Springhead.h>
#include <iostream>
#include <ostream>
#include <vector>
#include <conio.h>
#include <GL/glut.h>
#include <windows.h>
#include <Foundation/UTPreciseTimer.h>

using namespace Spr;

unsigned int dt = 1;	// ms
UTPreciseTimer pTimer;	// μs単位で計測可能なタイマ
std::vector< unsigned long > time;

void CPSCounter(double intervalms, double periodms);
void SPR_CDECL CallBack(int id, void* arg){
	time.push_back(pTimer.Stop());
	//DSTR << time.back() << std::endl;
	pTimer.Clear();
	pTimer.Start();
	//CPSCounter(dt, 1000);
	//Sleep(2000);
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

int _cdecl main(int argc, char* argv[]){
	pTimer.Init();								/// 計測用タイマの初期化

	UTTimerIf* timer1;
	timer1 = UTTimerIf::Create();
	timer1->SetMode(UTTimerIf::MULTIMEDIA);
	timer1->SetResolution(1);					///	呼びだし分解能ms
	timer1->SetInterval(dt);					/// 呼びだし頻度ms
	timer1->SetCallback(CallBack, NULL);		/// 呼びだす関数
	timer1->Start();							/// マルチメディアタイマスタート

	std::cout << "Press any key to exit." << std::endl;

	while(!_kbhit()){}
	timer1->Stop();

	/// 計測データをxlsで出力
	std::ofstream ofs("UTPreciseTimerSample.xls");
	for(int i = 0; i < time.size(); i++){
		ofs << i << "\t" << time[i] << std::endl;
	}

	return 0;
}
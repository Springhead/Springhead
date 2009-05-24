#include <iostream>
#include <conio.h>
#include <GL/glut.h>						/// UTMMtimer.hを使うのに必要
#include "Foundation/UTMMTimer.h"

void CallBackHoge(void* arg){
	std::cout << "hogehoge" << std::endl;
}

void CallBackFuga(void* arg){
	std::cout << "         " << "fugafuga" << std::endl;
}

int _cdecl main(int argc, char* argv[]){
	Spr::UTMMTimer timer1;				/// マルチメディアタイマの宣言
	timer1.Resolution(1);					///	 呼びだし分解能
	timer1.Interval(1000);					/// 呼びだし頻度
	timer1.Set(CallBackHoge, NULL);	/// 呼びだす関数

	Spr::UTMMTimer timer2;
	timer2.Resolution(1);
	timer2.Interval(5000);
	timer2.Set(CallBackFuga, NULL);

	timer1.Create();							/// マルチメディアタイマスタート
	timer2.Create();

	while(!_kbhit()){}
	return 0;
}
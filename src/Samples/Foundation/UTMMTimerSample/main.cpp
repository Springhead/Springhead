#include <iostream>
#include <conio.h>
#include <GL/glut.h>						/// UTMMtimer.hを使うのに必要
#include "Foundation/UTMMTimer.h"

void CallBackHoge(void* arg){
	std::cout << "hogehoge" << std::endl;
}

class Fuga{
public:
	Spr::UTMMTimer timer2;
	static void CallBackFuga(void* arg){
		std::cout << "         " << "fugafuga" << std::endl;
	}
};

int _cdecl main(int argc, char* argv[]){
	Spr::UTMMTimer timer1;				/// マルチメディアタイマの宣言
	timer1.Resolution(1);					///	 呼びだし分解能
	timer1.Interval(1000);					/// 呼びだし頻度
	timer1.Set(CallBackHoge, NULL);	/// 呼びだす関数

	Fuga fuga;									/// クラスFuga
	fuga.timer2.Resolution(1);
	fuga.timer2.Interval(5000);
	fuga.timer2.Set(Fuga::CallBackFuga, NULL);	/// 呼び戻す関数は静的でなければならない

	timer1.Create();							/// マルチメディアタイマスタート
	fuga.timer2.Create();
	
	std::cout << "終了するには何かキーを押してください" << std::endl;
	while(!_kbhit()){}
	return 0;
}
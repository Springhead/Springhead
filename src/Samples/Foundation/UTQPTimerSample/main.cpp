#include <iostream>
#include <conio.h>
#include <GL/glut.h>
#include "Foundation/UTQPTimer.h"
#include <windows.h>

//梶本先生のソースコード

using namespace std;
int _cdecl main(int argc, char* argv[]){
	Spr::UTQPTimer myTimer;
	int i,t;
	
	/*タイマーの周波数を出力．一応の分解能となる*/
	cout<<"タイマーの周波数"<<myTimer.Freq()/1000000<<"MHz"<<endl;

	/*************************************************************/
	cout<<" [例1] 時間待ちのための使い方"<<endl;
	/*************************************************************/
	cout<<"3秒待ちます"<<endl;
	myTimer.WaitUS(3000000);
	cout<<"3秒経過"<<endl;
	myTimer.Clear();
	Sleep(1000);

	/*************************************************************/
	cout<<" [例2] 0.1秒のインターバルタイマーの使い方（主に制御用）"<<endl;
	/*************************************************************/
	myTimer.CountUS();	//(0)CountUS関数を呼ぶことで初期化
	for(i=0;i<10;i++){
		Sleep(50);		//(1)制御用の計算を模擬した50msのSleep
		myTimer.CountAndWaitUS(100000);//(2)前回CountUSまたはCountAndWaitUSが呼ばれてから0.1秒経過するまでアクティブウエイト
		cout<<"0.1秒経過．"<<endl;
	}
	myTimer.Clear();
	Sleep(1000);

	/*************************************************************/
	cout<<" [例3] 時刻記録のための使い方"<<endl;
	/*************************************************************/
	myTimer.Start();
	while(1){
		double time = myTimer.GetTime(); //us単位
		Sleep(10);
		std::cout<<time/1000<<"ms"<<std::endl;
		if(time >1000000){
			myTimer.Stop();
			break;
		}
	}

	std::cout << "終了するには何かキーを押してください" << std::endl;
	while(!_kbhit()){}
	return 0;
}
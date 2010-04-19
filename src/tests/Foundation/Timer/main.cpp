#include <Springhead.h>
#include "Foundation/UTTimer.h"

#include <iostream>
#include <map>
#include <conio.h>
#include <GL/glut.h>
#include <windows.h>
using namespace Spr;

unsigned startTime;

class TimerProviderGL: public UTTimerProvider{
	struct Arg{
		UTTimer* timer;
		bool bStop;
		Arg(UTTimer* t): timer(t), bStop(false){}
	};
	typedef std::vector<Arg*> Args;
	Args args;
	static void Callback(int a){
		Arg* arg = (Arg*)a;
		if (!arg->bStop){
			arg->timer->Call();
			glutTimerFunc(arg->timer->GetInterval(), Callback, a);
		}else{
			delete arg;	//	ここまで arg を delete するわけに行かない。
		}
	}
public:
	TimerProviderGL(){
		Register();		//	タイマーにフレームワークを登録
	}
	~TimerProviderGL(){
		Unregister();	//	タイマーからフレームワークを解除
	}
	bool StartTimer(UTTimer* timer){
		args.push_back(new Arg(timer));
		glutTimerFunc(timer->GetInterval(), Callback, (int)(void*)args.back());
		return true;
	}
	bool StopTimer(UTTimer* timer){
		for(Args::iterator it = args.begin(); it!= args.end(); ++it){
			if ((*it)->timer == timer){
				(*it)->bStop = true;
				args.erase(it);	//	ここで、(*it)を delete してしまうと、タイマーの最後の呼び出しの時に、arg の先が無いので実行時エラーに成ってしまう。
				return true;
			}
		}
		return false;
	}
};

volatile int hogeCount;
void CallBackHoge(void* arg){
	std::cout << "hogehoge" << std::endl;
	hogeCount++;
}
volatile int piyoCount;
void CallBackPiyo(void* arg){
	std::cout << "    piyopiyo" << std::endl;
	piyoCount++;
}

class Fuga{
public:
	UTTimer timer2;
	int count;
	Fuga(): count(0){}
	static void CallBackFuga(void* arg){
		((Fuga*)arg)->Print();
	}
	void Print(){
		count ++;
		std::cout << "         " << "fugafuga " << count << std::endl;
	}
};

UTTimer timer1;				/// タイマの宣言
UTTimer timer3;				/// タイマの宣言
Fuga fuga;					/// クラスFuga


TimerProviderGL providerGL;	//	タイマーにフレームワークを登録 

void idle(){
	providerGL.CallIdle();
	unsigned time = timeGetTime();
	int delta = (int)time - (int)startTime;
	if (delta > 5250){
		DSTR << "hoge:" << hogeCount << "  fuga:" << fuga.count << "  piyo:" << piyoCount << std::endl;
		if (hogeCount != 26) exit(-1);
		if (fuga.count != 10) exit(-2);
		if (piyoCount != 52) exit(-3);
		exit(0);
	}
}
void keyboard(unsigned char key, int x, int y);
void display();
void reshape(int w, int h);

int _cdecl main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
								//	たとえば、FWAppのGLUT版 に providerGL と同じ機能を持たせれば良い

	timer1.SetMode(UTTimer::MULTIMEDIA);
	timer1.SetResolution(20);					///	 呼びだし分解能
	timer1.SetInterval(200);					/// 呼びだし頻度
	timer1.SetCallback(CallBackHoge, NULL);	/// 呼びだす関数

	fuga.timer2.SetMode(UTTimer::FRAMEWORK);
	fuga.timer2.SetResolution(5);
	fuga.timer2.SetInterval(500);
	fuga.timer2.SetCallback(Fuga::CallBackFuga, &fuga);	/// 呼び戻す関数は静的でなければならない

	timer3.SetMode(UTTimer::MULTIMEDIA);
	timer3.SetResolution(10);					///	 呼びだし分解能
	timer3.SetInterval(100);					/// 呼びだし頻度
	timer3.SetCallback(CallBackPiyo, NULL);		/// 呼びだす関数


	startTime = timeGetTime();
	timer1.Start();							/// マルチメディアタイマスタート
	timer3.Start();							/// マルチメディアタイマスタート
	fuga.timer2.Start();
	
	glutCreateWindow("UTTimerSample");
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
void keyboard(unsigned char key, int x, int y){
	timer1.Stop();
	fuga.timer2.Stop();
	exit(0);
}	
void display(){
}
void reshape(int w, int h){
}
#include <Springhead.h>
#include "Foundation/UTTimer.h"

#include <iostream>
#include <map>
#include <conio.h>
#include <GL/glut.h>
#include <windows.h>
using namespace Spr;


class TimerProviderGL: public UTTimerProvider{
	struct Arg{
		UTTimer* timer;
		bool bStop;
		Arg(UTTimer* t): timer(t), bStop(false){}
	};
	typedef std::vector<Arg*> Args;
	Args args;
	static void SPR_CDECL Callback(int a){
		Arg* arg = (Arg*)(size_t)a;
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
		glutTimerFunc(timer->GetInterval(), Callback, (int)(size_t)(void*)args.back());
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

void SPR_CDECL CallBackHoge(int id, void* arg){
	std::cout << "hogehoge:" << id << std::endl;
}

class Fuga{
public:
	UTTimer timer2;
	int count;
	Fuga(): count(0){}
	static void SPR_CDECL CallBackFuga(int id, void* arg){
		((Fuga*)arg)->Print(id);
	}
	void Print(int id){
		count ++;
		std::cout << "         " << "fugafuga:" << id << " - "  << count << std::endl;
	}
};

UTTimer timer1;				/// タイマの宣言
Fuga fuga;									/// クラスFuga


TimerProviderGL providerGL;	//	タイマーにフレームワークを登録 

void SPR_CDECL keyboard(unsigned char key, int x, int y);
void SPR_CDECL display();
void SPR_CDECL reshape(int w, int h);
void SPR_CDECL idle(){
	providerGL.CallIdle();
}

int _cdecl main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
								//	たとえば、FWAppのGLUT版 に providerGL と同じ機能を持たせれば良い

	timer1.SetMode(UTTimerIf::MULTIMEDIA);
	timer1.SetResolution(500);					///	 呼びだし分解能
	timer1.SetInterval(100);					/// 呼びだし頻度
	timer1.SetCallback(CallBackHoge, NULL);	/// 呼びだす関数

	fuga.timer2.SetMode(UTTimerIf::IDLE);
	fuga.timer2.SetResolution(500);
	fuga.timer2.SetInterval(500);
	fuga.timer2.SetCallback(Fuga::CallBackFuga, &fuga);	/// 呼び戻す関数は静的でなければならない

	timer1.Start();							/// マルチメディアタイマスタート
	fuga.timer2.Start();
	
	std::cout << "終了するには何かキーを押してください" << std::endl;

	glutCreateWindow("UTTimerSample");
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

void SPR_CDECL keyboard(unsigned char key, int x, int y){
	timer1.Stop();
	fuga.timer2.Stop();
	exit(0);
}	

void SPR_CDECL display(){
}
void SPR_CDECL reshape(int w, int h){
}
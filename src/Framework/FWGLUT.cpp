#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Framework/SprFWGLUT.h"
#include "Framework/SprFWApp.h"


namespace Spr{;
// FWWinGLUT /////////////////////////////////////////////////////////////////////
void FWWinGLUT::Position(int left, int top){
	glutPositionWindow(left, top); fullscreen = false;
}
void FWWinGLUT::Reshape(int width, int height){
	glutReshapeWindow(width, height); fullscreen = false;
	this->width = width; this->height = height;
}
void FWWinGLUT::SetTitle(UTString t){
	glutSetWindowTitle(t.c_str()); title = t;
}
void FWWinGLUT::FullScreen(){
	glutFullScreen(); fullscreen = true;
}
// FWGLUT /////////////////////////////////////////////////////////////////////
FWGLUTDesc::FWGLUTDesc(){
}

FWGLUT::FWGLUT(){
};

FWGLUT* FWGLUT::instance;
FWGLUT::FWGLUTTimer* FWGLUT::FWGLUTTimer::timerInstance;

FWGLUT::~FWGLUT(){
	FWGLUT::AtExit();
	instance = NULL;
	
}
/** コールバック関数*///////////////////////////////////////////////////////
void FWGLUT::GlutDisplayFunc(){
	instance->fwApp->CallDisplay();	
}

void FWGLUT::GlutReshapeFunc(int w, int h){
	instance->fwApp->CallReshape(w, h);
}
void FWGLUT::GlutIdleFunc(){
	instance->fwApp->CallIdleFunc();
}

void FWGLUT::GlutKeyboardFunc(unsigned char key, int x, int y){
	instance->fwApp->CallKeyboard((int)key, x, y);
}

void FWGLUT::GlutMouseFunc(int button, int state, int x, int y){
	instance->fwApp->CallMouseButton(button, state, x, y);
}
void FWGLUT::GlutMotionFunc(int x, int y){
	instance->fwApp->CallMouseMove(x, y);
}

void FWGLUT::GlutJoystickFunc(unsigned int buttonMask, int x, int y, int z){
	instance->fwApp->CallJoystick(buttonMask, x, y, z);
}

void FWGLUT::AtExit(){
	if(instance && instance->fwApp->vfBridge)
		instance->fwApp->vfBridge->AtExit();
}

/** Init *////////////////////////////////////////////////////////////////

void FWGLUT::Init(int argc, char* argv[]){
	instance = this;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	atexit(FWGLUT::AtExit);
}

/** タイマ *////////////////////////////////////////////////////////////////

void FWGLUT::FWGLUTTimer::SetTimerFunc(GTimerFunc* f){
	func=f;
}
void FWGLUT::FWGLUTTimer::GetTimerFunc(){;
	this->func;
}
/// タイマーを設定する
void FWGLUT::Timer(int i){
	if(i < (int)glutTimer.size()){
		glutTimerFunc(1, glutTimer[i]->func, 0);
	}
}
/// タイマーを作成する
void FWGLUT::AddTimer(){
	glutTimer.push_back(DBG_NEW FWGLUTTimer);
}
/// 最後に作成したタイマーの番号を返す
int FWGLUT::GetTimerNo(){
	return (int)glutTimer.size() - 1;
}
/// i番目のタイマーにTimerfuncを設定する
void FWGLUT::SetTimerFunc(GTimerFunc* f ,int i){
	if(i < (int)glutTimer.size()){
		glutTimer[i]->SetTimerFunc(f);
	}
}
/// mainloopを呼ぶ
void FWGLUT::Loop(int i,double timeStep){
	int timeSteps=1;
	if(!fwApp->instance)return;
	if(!fwApp->instance->GetSdk())return;
	if(fwApp->instance->GetSdk()->GetScene()){
		if(fwApp->instance->GetSdk()->GetScene()->GetPHScene()){
			timeSteps = (int)(fwApp->instance->GetSdk()->GetScene()->GetPHScene()->GetTimeStep() * 1000.0);
			if (timeSteps<1) timeSteps = 1;
		}
	}
	if(!timeStep==0){
		timeSteps=timeStep;
	}
	glutTimerFunc(timeSteps, glutTimer[i]->func, 0);
}
///GLUTによるTimerをスタートする
void FWGLUT::TimerStart(){
	glutIdleFunc(FWGLUT::GlutIdleFunc);
	glutMainLoop();
}
/// タイマーを取得する
FWGLUT::FWGLUTTimer* FWGLUT::GetTimer(int i){
	if(i < (int)glutTimer.size()){
		return glutTimer[i];
	}else{
		return NULL;
	}
}

/** ウィンドウ *////////////////////////////////////////////////////////////////

///	ウィンドウを作成し、ウィンドウ IDを返す
FWWin* FWGLUT::CreateWin(const FWWinDesc& d){
	int wid=0;
	if(d.fullscreen){	//< フルスクリーンの場合のウィンドウ生成
		std::stringstream gameMode;
		gameMode << d.width << "x" << d.height << ":32@60";
		glutGameModeString(gameMode.str().c_str());
#ifdef _MSC_VER
		Sleep(100);
#endif
		wid	= glutEnterGameMode();
	}else{				//< ウィンドウモードの場合の生成
		if (d.parentWindow){
			wid = glutCreateSubWindow(d.parentWindow, d.left, d.top, d.width, d.height);
		}else{
			glutInitWindowSize(d.width, d.height);
			glutInitWindowPosition(d.left, d.top);
			wid = glutCreateWindow(d.title.c_str());
		}
	}
	//	このWindowのglewコンテキストの初期化
	int rv = glewInit();
	//	windowに関連するコールバックの設定
	glutDisplayFunc(FWGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWGLUT::GlutKeyboardFunc);
	glutMouseFunc(FWGLUT::GlutMouseFunc);
	glutMotionFunc(FWGLUT::GlutMotionFunc);
	int pollInterval = 10;	// int pollInterval : glutJoystickFuncを使うときに使う何か．読み込み時間に関係しているらしい．
	glutJoystickFunc(FWGLUT::GlutJoystickFunc, pollInterval);
	
	FWWin* win = DBG_NEW FWWinGLUT(wid, d, fwApp->GetSdk()->CreateRender());
	fwApp->AssignScene(win);
	fwApp->wins.push_back(win);
	return win;
}
///	ウィンドウを破棄する
void FWGLUT::DestroyWin(FWWin* w){
	if (w->fullscreen){
		glutLeaveGameMode();
	}else{
		glutDestroyWindow(w->GetID());
	}
	Wins::iterator it = std::find(fwApp->wins.begin(), fwApp->wins.end(), w);
	assert(it != fwApp->wins.end());
	fwApp->wins.erase(it);
};
///	カレントウィンドウを設定する
void FWGLUT::SetCurrentWin(FWWin* w){
	glutSetWindow(w->GetID());
};
///	カレントウィンドウを返す。
FWWin* FWGLUT::GetCurrentWin(){
	return fwApp->GetWinFromId(glutGetWindow());
}
///カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
void FWGLUT::PostRedisplay(){
	return glutPostRedisplay();
};
/// Shift,Ctrl,Altのステートを返す
int FWGLUT::Modifiers(){
	return glutGetModifiers();
};

}
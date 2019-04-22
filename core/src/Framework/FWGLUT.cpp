/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifdef _MSC_VER
# include <windows.h>
# undef CreateDialog
#endif

#include <SprDefs.h>
#include <GL/glew.h>
#include <GL/glut.h>
#ifdef USE_FREEGLUT
#include <GL/freeglut_ext.h>
#endif

#include <Framework/SprFWApp.h>
#include <Framework/FWGLUT.h>
#include <Framework/FWSdk.h>
#include <Graphics/GRSdk.h>
#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseGLUT.h>
#include <HumanInterface/DRJoyStickGLUT.h>
#include <sstream>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace std;

namespace Spr{;

FWGLUT::FWGLUT(){
	idleFuncFlag = true;
};

FWGLUT::~FWGLUT(){
}

FWGLUT* FWGLUT::GetInstance(){
	return (FWGLUT*)&*(FWGraphicsHandler::instance);
}

/** コールバック関数*///////////////////////////////////////////////////////

void FWGLUT::GlutDisplayFunc(){
	FWApp::GetApp()->Display();	
}

void FWGLUT::GlutReshapeFunc(int w, int h){
	FWWinIf* win = FWApp::GetApp()->GetCurrentWin();
	// 埋め込みGUIコントロールがある場合を想定してビューポートの計算を行う
	int l = 0, t = 0;
	FWGraphicsHandler::instance->CalcViewport(l, t, w, h);
	win->GetRender()->Reshape(Vec2f(l, t), Vec2f(w,h));
	// アプリケーションにも通知（子ウィンドウのリサイズなどのため）
	FWApp::GetApp()->Reshape(w, h);
	// 新しいウィンドウサイズを記憶
	FWWinBase* winBase = win->Cast();
	FWWinBaseDesc desc;
	winBase->FWWinBase::GetDesc(&desc);
	desc.width = w;
	desc.height = h;
	winBase->FWWinBase::SetDesc(&desc);
}

void FWGLUT::GlutTimerFunc(int value){
	UTTimerIf* timer = UTTimerIf::Get(value);
	if(!timer)
		return;
	// タイマーが稼働中ならコールバックを呼び再登録する
	if(timer->IsStarted()){
		glutTimerFunc(timer->GetInterval(), GlutTimerFunc, timer->GetID());
		timer->Call();
	}

}
void FWGLUT::GlutIdleFunc(){
	// ＊以下2系統のIdle処理は統一すべき	tazz

	// UTTimerProviderとしての機能．IDLEモードのタイマのTimerFuncが呼ばれる
	UTTimerProvider::CallIdle();

	// FWApp::IdleFuncを呼ぶ
	FWApp::GetApp()->IdleFunc();
}

void FWGLUT::GlutMouseFunc(int button, int state, int x, int y){
	FWGLUT::GetInstance()->keyMouse->OnMouse(button, state, x, y);
}

void FWGLUT::GlutMotionFunc(int x, int y){
	FWGLUT::GetInstance()->keyMouse->OnMotion(x, y);
}

void FWGLUT::GlutPassiveMotionFunc(int x, int y){
	FWGLUT::GetInstance()->keyMouse->OnPassiveMotion(x, y);
}

void FWGLUT::GlutKeyFunc(unsigned char ch, int x, int y){
	FWGLUT::GetInstance()->keyMouse->OnKey(ch, x, y);
}

void FWGLUT::GlutSpecialKeyFunc(int ch, int x, int y){
	FWGLUT::GetInstance()->keyMouse->OnSpecialKey(ch, x, y);
}

#ifdef USE_FREEGLUT
void FWGLUT::GlutMouseWheelFunc(int wheel, int direction, int x, int y){
	FWGLUT::GetInstance()->keyMouse->OnMouseWheel(wheel, direction, x, y);
}
#endif

void FWGLUT::AtExit(){
	FWApp::GetApp()->AtExit();
}

/** Init *////////////////////////////////////////////////////////////////

void FWGLUT::Init(int argc, char** argv){
	// OpenGLコンテキストバージョンを指定
#ifdef USE_FREEGLUT
	glutInitContextVersion(OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
#endif

	if(argc == 0){
		argc = 1;
		char* dummy[] = {(char*) "", NULL};
		glutInit(&argc, dummy);
	}
	else glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	atexit(FWGLUT::AtExit);

	// UTTimerProviderとして登録
	Register();

	// グラフィクスデバイスを作成
	if (! FWApp::GetApp()->GetSdk()) FWApp::GetApp()->CreateSdk();
	grDevice = FWApp::GetApp()->GetSdk()->GetGRSdk()->CreateDeviceGL();
	
	// キーボード・マウスとジョイスティックデバイスの登録
	HISdkIf* hiSdk = FWApp::GetApp()->GetSdk()->GetHISdk();
	keyMouse = hiSdk->AddRealDevice(DRKeyMouseGLUTIf::GetIfInfoStatic())->Cast();
	joyStick = hiSdk->AddRealDevice(DRJoyStickGLUTIf::GetIfInfoStatic())->Cast();
}

bool FWGLUT::StartTimer(UTTimer* timer){
	glutTimerFunc(timer->GetInterval(), GlutTimerFunc, timer->timerId);
	return true;
}

bool FWGLUT::StopTimer(UTTimer* timer){
	// タイマコールバックにおいて再登録を止めるのでここでは何もしない
	return true;
}

void FWGLUT::StartMainLoop(){
	// CPUが常時100%になる問題あり
	EnableIdleFunc(idleFuncFlag);
	glutMainLoop();
}

void FWGLUT::EnableIdleFunc(bool on){
	idleFuncFlag = on;
	glutIdleFunc(on ? FWGLUT::GlutIdleFunc : NULL);
}
void FWGLUT::EnterGameMode(){
	glutEnterGameMode();
}
void FWGLUT::LeaveGameMode(){
	glutLeaveGameMode();
}

/** ウィンドウ *////////////////////////////////////////////////////////////////

///	ウィンドウを作成し、ウィンドウ IDを返す
FWWinIf* FWGLUT::CreateWin(const FWWinDesc& desc, FWWinIf* parent){
	int wid=0;

	// フルスクリーンの場合のウィンドウ生成
	if(desc.fullscreen){	
		std::stringstream gameMode;
		gameMode << desc.width << "x" << desc.height << ":32@60";
		glutGameModeString(gameMode.str().c_str());
#ifdef _MSC_VER
		Sleep(100);
#endif
		wid	= glutEnterGameMode();
	}
	// ウィンドウモードの場合の生成
	else{
		if(parent){
			wid = glutCreateSubWindow(parent->GetID(), desc.left, desc.top, desc.width, desc.height);
		}
		else{
			glutInitWindowSize(desc.width, desc.height);
			glutInitWindowPosition(desc.left, desc.top);
			wid = glutCreateWindow(desc.title.c_str());
		}
	}
	// このWindowのglewコンテキストの初期化
	//int rv = glewInit();

	// windowに関連するコールバックの設定
	RegisterCallbacks();
	

	// ウィンドウを作成
	FWWin* win = DBG_NEW FWWin();
	win->SetDesc(&desc);
	win->id = wid;
	
	// キーボード・マウスとジョイスティックの仮想デバイスを作成して関連付け
	HISdkIf* hiSdk = FWApp::GetApp()->GetSdk()->GetHISdk();
	HIRealDeviceIf* dr;
	if(desc.useKeyMouse){
		dr = hiSdk->FindRealDevice(DRKeyMouseGLUTIf::GetIfInfoStatic());
		win->keyMouse = dr->Rent(DVKeyMouseIf::GetIfInfoStatic(), "", wid)->Cast();
	}
	if(desc.useJoyStick){
		dr = hiSdk->FindRealDevice(DRJoyStickGLUTIf::GetIfInfoStatic());
		win->joyStick = dr->Rent(DVJoyStickIf::GetIfInfoStatic(), "", wid)->Cast();
	}

	return win->Cast();
}

void FWGLUT::RegisterCallbacks(){
	glutDisplayFunc		 (FWGLUT::GlutDisplayFunc);
	glutReshapeFunc		 (FWGLUT::GlutReshapeFunc);
	glutMouseFunc		 (FWGLUT::GlutMouseFunc);
	glutMotionFunc		 (FWGLUT::GlutMotionFunc);
	glutPassiveMotionFunc(FWGLUT::GlutPassiveMotionFunc);
	glutKeyboardFunc	 (FWGLUT::GlutKeyFunc);
	glutSpecialFunc		 (FWGLUT::GlutSpecialKeyFunc);
#ifdef USE_FREEGLUT
	glutMouseWheelFunc   (FWGLUT::GlutMouseWheelFunc);
#endif
}

///	ウィンドウを破棄する
void FWGLUT::DestroyWin(FWWinIf* w){
	if (w->GetFullScreen()){
		glutLeaveGameMode();
	}
	else{
		glutDestroyWindow(w->GetID());
	}
};
///	カレントウィンドウを設定する
void FWGLUT::SetCurrentWin(FWWinIf* w){
	glutSetWindow(w->GetID());
};
///	カレントウィンドウを返す。
int FWGLUT::GetCurrentWin(){
	return glutGetWindow();
}
///カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
void FWGLUT::PostRedisplay(){
	return glutPostRedisplay();
};
/// Shift,Ctrl,Altのステートを返す
int FWGLUT::GetModifiers(){
	return glutGetModifiers();
};
Vec2i FWGLUT::GetPosition(FWWinBase* win){
	Vec2i pos;
	glutSetWindow(win->id);
	pos.x = glutGet(GLUT_WINDOW_X);
	pos.y = glutGet(GLUT_WINDOW_Y);
	return pos;
}
void FWGLUT::SetPosition(FWWinBase* win, int left, int top){
	glutSetWindow(win->id);
	glutPositionWindow(left, top);
}
Vec2i FWGLUT::GetSize(FWWinBase* win){
	Vec2i sz;
	glutSetWindow(win->id);
	sz.x = glutGet(GLUT_WINDOW_WIDTH);
	sz.y = glutGet(GLUT_WINDOW_HEIGHT);
	return sz;
}
void FWGLUT::SetSize(FWWinBase* win, int width, int height){
	glutSetWindow(win->id);
	glutReshapeWindow(width, height);
}
void FWGLUT::SetTitle(FWWinBase* win, UTString t){
	glutSetWindow(win->id);
	glutSetWindowTitle(t.c_str());
}
void FWGLUT::SetFullScreen(FWWin* win){
	glutSetWindow(win->id);
	glutFullScreen();
}


}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifdef _MSC_VER
#	include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#include <Framework/SprFWApp.h>
#include <Framework/FWGLUT.h>
#include <Framework/FWSdk.h>
#include <Graphics/GRSdk.h>
#include <HumanInterface/HISdk.h>
#include <sstream>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace std;

namespace Spr{;

// FWWinGLUT /////////////////////////////////////////////////////////////////////
void FWWinGLUT::SetPosition(int left, int top){
	glutSetWindow(id);
	glutPositionWindow(left, top);
	fullscreen = false;
}
void FWWinGLUT::SetSize(int width, int height){
	glutSetWindow(id);
	glutReshapeWindow(width, height);
	fullscreen = false;
	this->width = width;
	this->height = height;
}
void FWWinGLUT::SetTitle(UTString t){
	glutSetWindow(id);
	glutSetWindowTitle(t.c_str());
	title = t;
}
void FWWinGLUT::SetFullScreen(){
	glutSetWindow(id);
	glutFullScreen();
	fullscreen = true;
}

// FWGLUT /////////////////////////////////////////////////////////////////////
FWGLUT::FWGLUT(FWApp* a):FWGraphicsAdaptee(a){
	idleFuncFlag = true;
};

FWGLUT* FWGLUT::instance;

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
void FWGLUT::GlutTimerFunc(int value){
	UTTimerIf* timer = UTTimerIf::Get(value);
	if(!timer)
		return;
	timer->Call();

	// タイマーの再設定
	if(instance->timerRestart)
		glutTimerFunc(timer->GetInterval(), GlutTimerFunc, timer->GetID());

}
void FWGLUT::GlutIdleFunc(){
	// ＊以下2系統のIdle処理は統一すべき	tazz

	// UTTimerProviderとしての機能．IDLEモードのタイマのTimerFuncが呼ばれる
	UTTimerProvider::CallIdle();

	// FWApp::IdleFuncを呼ぶ
	instance->fwApp->CallIdleFunc();
}
/*
void FWGLUT::GlutKeyboardFunc(unsigned char key, int x, int y){
	instance->fwApp->CallKeyboard((int)key, x, y);
}
void FWGLUT::GlutSpecialFunc(int key, int x, int y){
	// GlutKeyboardFuncと重複しないようにビットを立てる
	instance->fwApp->CallKeyboard(key | 0x100, x, y);
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
*/
void FWGLUT::AtExit(){
	instance->fwApp->AtExit();
}

/** Init *////////////////////////////////////////////////////////////////

void FWGLUT::Init(int argc, char** argv){
	instance = this;
	if(argc == 0){
		argc = 1;
		char* dummy[] = {"", NULL};
		glutInit(&argc, dummy);
	}
	else glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	atexit(FWGLUT::AtExit);

	// UTTimerProviderとして登録
	Register();

	// グラフィクスデバイスを作成
	grDevice = fwApp->GetSdk()->GetGRSdk()->CreateDeviceGL();
	grDevice->Init();

	// キーボード・マウスとジョイスティックデバイスの登録
	HISdkIf* hiSdk = fwApp->GetSdk()->GetHISdk();
	hiSdk->AddRealDevice(DRKeyMouseGLUTIf::GetIfInfoStatic());
	hiSdk->AddRealDevice(DRJoyStickGLUTIf::GetIfInfoStatic());
}

bool FWGLUT::StartTimer(UTTimer* timer){
	glutTimerFunc(timer->GetInterval(), GlutTimerFunc, timer->timerId);
	timerRestart = true;
	return true;
}

bool FWGLUT::StopTimer(UTTimer* timer){
	timerRestart = false;
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
FWWinIf* FWGLUT::CreateWin(const FWWinDesc& desc){
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
		if (desc.parentWindow){
			wid = glutCreateSubWindow(desc.parentWindow, desc.left, desc.top, desc.width, desc.height);
		}
		else{
			glutInitWindowSize(desc.width, desc.height);
			glutInitWindowPosition(desc.left, desc.top);
			wid = glutCreateWindow(desc.title.c_str());
		}
	}
	// このWindowのglewコンテキストの初期化
	int rv = glewInit();

	// windowに関連するコールバックの設定
	glutDisplayFunc(FWGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWGLUT::GlutReshapeFunc);
	/*
	glutKeyboardFunc(FWGLUT::GlutKeyboardFunc);
	glutSpecialFunc(FWGLUT::GlutSpecialFunc);
	glutMouseFunc(FWGLUT::GlutMouseFunc);
	glutMotionFunc(FWGLUT::GlutMotionFunc);
	int pollInterval = 10;	// int pollInterval : glutJoystickFuncを使うときに使う何か．読み込み時間に関係しているらしい．
	glutJoystickFunc(FWGLUT::GlutJoystickFunc, pollInterval);
	*/

	// ウィンドウを作成
	FWWinGLUT* win = DBG_NEW FWWinGLUT(wid, desc);
	
	// キーボード・マウスとジョイスティックの仮想デバイスを作成して関連付け
	HISdkIf* hiSdk = fwApp->GetSdk()->GetHISdk();
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
FWWinIf* FWGLUT::GetCurrentWin(){
	return fwApp->GetWinFromId(glutGetWindow());
}
///	カレントウィンドウのIDを返す。
int FWGLUT::GetWinFromId(){
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

}
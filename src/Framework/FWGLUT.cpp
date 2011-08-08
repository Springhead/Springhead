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

FWGLUT::FWGLUT(){
	idleFuncFlag = true;
};

FWGLUT::~FWGLUT(){
}

FWGLUT* FWGLUT::GetInstance(){
	return (FWGLUT*)&*(FWGraphicsAdaptee::instance);
}

/** コールバック関数*///////////////////////////////////////////////////////
void FWGLUT::GlutDisplayFunc(){
	FWApp::GetApp()->Display();	
}
void FWGLUT::GlutReshapeFunc(int w, int h){
	FWWinIf* win = FWApp::GetApp()->GetCurrentWin();
	// 埋め込みGUIコントロールがある場合を想定してビューポートの計算を行う
	int l = 0, t = 0;
	FWGraphicsAdaptee::instance->CalcViewport(&l, &t, &w, &h);
	win->GetRender()->Reshape(Vec2f(l, t), Vec2f(w,h));
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
	timer->Call();

	// タイマーの再設定
	if(GetInstance()->timerRestart)
		glutTimerFunc(timer->GetInterval(), GlutTimerFunc, timer->GetID());

}
void FWGLUT::GlutIdleFunc(){
	// ＊以下2系統のIdle処理は統一すべき	tazz

	// UTTimerProviderとしての機能．IDLEモードのタイマのTimerFuncが呼ばれる
	UTTimerProvider::CallIdle();

	// FWApp::IdleFuncを呼ぶ
	FWApp::GetApp()->IdleFunc();
}
void FWGLUT::AtExit(){
	FWApp::GetApp()->AtExit();
}

/** Init *////////////////////////////////////////////////////////////////

void FWGLUT::Init(int argc, char** argv){
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
	DSTR << FWApp::GetApp()->GetSdk() << endl;
	grDevice = FWApp::GetApp()->GetSdk()->GetGRSdk()->CreateDeviceGL();
	grDevice->Init();

	// キーボード・マウスとジョイスティックデバイスの登録
	HISdkIf* hiSdk = FWApp::GetApp()->GetSdk()->GetHISdk();
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
	int rv = glewInit();

	// windowに関連するコールバックの設定
	glutDisplayFunc(FWGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWGLUT::GlutReshapeFunc);

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

void FWGLUT::SetPosition(FWWinBase* win, int left, int top){
	glutSetWindow(win->id);
	glutPositionWindow(left, top);
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
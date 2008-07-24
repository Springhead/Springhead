/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/SprFWAppGLUT.h>
#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>
#include <sstream>
#include <windows.h>

#include <GL/glew.h>
#include <GL/glut.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
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

//-----------------------------------------------------------------------

FWAppGLUT* FWAppGLUT::instance;

FWAppGLUT::~FWAppGLUT(){
	FWAppGLUT::AtExit();
	instance = NULL;
}
void FWAppGLUT::GlutDisplayFunc(){
	FWAppGLUT::instance->CallDisplay();
}

void FWAppGLUT::GlutReshapeFunc(int w, int h){
	FWAppGLUT::instance->CallReshape(w, h);
}
void FWAppGLUT::GlutIdleFunc(){
	FWAppGLUT::instance->CallIdle();
}

void FWAppGLUT::GlutTimerFunc(int id){	
	// タイマ周期＝物理シミュレーションのインターバル
	int timeStep=1;
	if(!FWAppGLUT::instance)return;
	if(!FWAppGLUT::instance->GetSdk())return;
	if(FWAppGLUT::instance->GetSdk()->GetScene()){
		if(FWAppGLUT::instance->GetSdk()->GetScene()->GetPHScene()){
			timeStep = (int)(FWAppGLUT::instance->GetSdk()->GetScene()->GetPHScene()->GetTimeStep() * 1000.0);
			if (timeStep<1) timeStep = 1;
		}
	}
	glutTimerFunc(timeStep, GlutTimerFunc, 0);

	FWAppGLUT::instance->CallStep();
	glutPostRedisplay();
}
void FWAppGLUT::GlutKeyboardFunc(unsigned char key, int x, int y){
	FWAppGLUT::instance->CallKeyboard((int)key, x, y);
}

void FWAppGLUT::GlutMouseFunc(int button, int state, int x, int y){
	FWAppGLUT::instance->CallMouseButton(button, state, x, y);
}
void FWAppGLUT::GlutMotionFunc(int x, int y){
	FWAppGLUT::instance->CallMouseMove(x, y);
}

void FWAppGLUT::GlutJoystickFunc(unsigned int buttonMask, int x, int y, int z){
	FWAppGLUT::instance->CallJoystick(buttonMask, x, y, z);
}

void FWAppGLUT::AtExit(){
	if(FWAppGLUT::instance && FWAppGLUT::instance->vfBridge)
		FWAppGLUT::instance->vfBridge->AtExit();
}

//-----------------------------------------------------------------------

void FWAppGLUT::Init(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	FWAppGL::Init(argc, argv);
	atexit(FWAppGLUT::AtExit);
}

void FWAppGLUT::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
	glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	glutIdleFunc(FWAppGLUT::GlutIdleFunc);
	glutMainLoop();
}

FWWin* FWAppGLUT::CreateWin(const FWWinDesc& d){
	int wid=0;
	if(d.fullscreen){	//< フルスクリーンの場合のウィンドウ生成
		std::stringstream gameMode;
		gameMode << d.width << "x" << d.height << ":32@60";
		glutGameModeString(gameMode.str().c_str());
		Sleep(100);
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
	glutDisplayFunc(FWAppGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWAppGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWAppGLUT::GlutKeyboardFunc);
	glutMouseFunc(FWAppGLUT::GlutMouseFunc);
	glutMotionFunc(FWAppGLUT::GlutMotionFunc);
	int pollInterval = 0.01;	// int pollInterval : glutJoystickFuncを使うときに使う何か．読み込み時間に関係しているらしい．
	glutJoystickFunc(FWAppGLUT::GlutJoystickFunc, pollInterval);
	
	FWWin* win = DBG_NEW FWWinGLUT(wid, d, fwSdk->CreateRender());
	AssignScene(win);
	wins.push_back(win);
	return win;
}
void FWAppGLUT::DestroyWin(FWWin* w){
	if (w->fullscreen){
		glutLeaveGameMode();
	}else{
		glutDestroyWindow(w->GetID());
	}
	Wins::iterator it = std::find(wins.begin(), wins.end(), w);
	assert(it != wins.end());
	wins.erase(it);
}
void FWAppGLUT::SetCurrentWin(FWWin* w){
	glutSetWindow(w->GetID());
}
FWWin* FWAppGLUT::GetCurrentWin(){
	return GetWinFromId(glutGetWindow());
}
void FWAppGLUT::Display(){
	FWAppGL::Display();
	/// ダブルバッファモード時、カレントウィンドウのバッファ交換を行う
	glutSwapBuffers();
}

int FWAppGLUT::GetModifier(){
	return glutGetModifiers();
}

}

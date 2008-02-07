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

#include <GL/glew.h>
#include <GL/glut.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
void FWWinGLUT::Position(int left, int top){
	glutPositionWindow(left, top); fullScreen = false;
}
void FWWinGLUT::Reshape(int width, int height){
	glutReshapeWindow(width, height); fullScreen = false;
}
void FWWinGLUT::SetTitle(UTString t){
	glutSetWindowTitle(t.c_str()); title = t;
}
void FWWinGLUT::FullScreen(){
	glutFullScreen(); fullScreen = true;
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
	// 渡されるidは対応するウィンドウのID
	/*FWWin* win = FWAppGLUT::instance->GetWinFromId(id);
	if (win){
		FWAppGLUT::instance->SetCurrentWin(win);

		FWAppGLUT::instance->CallStep();
		glutPostRedisplay();
	
		// タイマ周期＝物理シミュレーションのインターバル
		int timeStep = (int)(win->GetScene()->GetPHScene()->GetTimeStep() * 1000.0);
		if (timeStep<1) timeStep = 1;
	
		glutTimerFunc(timeStep, GlutTimerFunc, id);
	}*/

	FWAppGLUT::instance->CallStep();
	glutPostRedisplay();

	// タイマ周期＝物理シミュレーションのインターバル
	int timeStep=1;
	if (FWAppGLUT::instance->GetSdk()->GetScene()){
		timeStep = (int)(FWAppGLUT::instance->GetSdk()->GetScene()->GetPHScene()->GetTimeStep() * 1000.0);
		if (timeStep<1) timeStep = 1;
	}
	glutTimerFunc(timeStep, GlutTimerFunc, 0);
}
void FWAppGLUT::GlutKeyboardFunc(unsigned char key, int x, int y){
	FWAppGLUT::instance->CallKeyboard(key, x, y);
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
	if (d.parentWindow){
		wid = glutCreateSubWindow(d.parentWindow, d.left, d.top, d.width, d.height);
	}else{
		glutInitWindowSize(d.width, d.height);
		glutInitWindowPosition(d.left, d.top);
		wid = glutCreateWindow(d.title.c_str());
	}
	int rv = glewInit();
	glutDisplayFunc(FWAppGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWAppGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWAppGLUT::GlutKeyboardFunc);
	// ウィンドウIDを指定してタイマを始動
	//glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, wid);
	
	FWWin* win = new FWWinGLUT(wid, fwSdk->CreateRender());
	AssignScene(win);
	wins.push_back(win);
	return win;
}
void FWAppGLUT::DestroyWin(FWWin* w){
	glutDestroyWindow(w->GetID());
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

}

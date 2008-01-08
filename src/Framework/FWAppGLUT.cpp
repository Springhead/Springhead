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

void FWAppGLUT::GlutTimerFunc(int id){
	FWAppGLUT::instance->CallStep();
	glutPostRedisplay();
	FWSceneIf* fs = FWAppGLUT::instance->GetSdk()->GetScene();
	if(!fs)return;

	PHSceneIf* ps = fs->GetPHScene();
	if(!ps)return;

	int timeStep = (int)(ps->GetTimeStep() * 1000.0);
	if (timeStep<1) timeStep = 1;
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
	windowID = CreateWindow();
	CreateRender();
	glutDisplayFunc(FWAppGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWAppGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWAppGLUT::GlutKeyboardFunc);
	glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	glutMainLoop();
}

void FWAppGLUT::Display(){
	FWAppGL::Display();
	/// ダブルバッファモード時、カレントウィンドウのバッファ交換を行う
	glutSwapBuffers();
}

int FWAppGLUT::CreateWindow(const FWWindowDesc d){
	glutInitWindowSize(d.width, d.height);
	glutInitWindowPosition(d.top, d.left);
	int window = glutCreateWindow("Springhead Application");
	int rv = glewInit();
	return window;
}
}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/SprFWAppGLUI.h>
#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>

#include <GL/glew.h>
#include <GL/glui.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWAppGLUI* FWAppGLUI::instance;

FWAppGLUI::~FWAppGLUI(){
	FWAppGLUI::AtExit();
	instance = NULL;
}

void FWAppGLUI::GluiDisplayFunc(){
	FWAppGLUI::instance->CallDisplay();
}

void FWAppGLUI::GluiReshapeFunc(int w, int h){
	FWAppGLUI::instance->CallReshape(w,h);
}

void FWAppGLUI::GluiTimerFunc(int id){
	FWAppGLUI::instance->CallStep();
	glutPostRedisplay();
	FWSceneIf* fs = FWAppGLUI::instance->GetSdk()->GetScene();
	if(!fs)return;

	PHSceneIf* ps = fs->GetPHScene();
	if(!ps)return;

	int timeStep = (int)(ps->GetTimeStep() * 100.0);
	if(timeStep<1) timeStep = 1;
	glutTimerFunc(timeStep, GluiTimerFunc, 0);
}

void FWAppGLUI::GluiKeyboardFunc(unsigned char key, int x, int y){
	FWAppGLUI::instance->CallKeyboard(key, x, y);
}

void FWAppGLUI::AtExit(){
	if(FWAppGLUI::instance && FWAppGLUI::instance->vfBridge)
		FWAppGLUI::instance->vfBridge->AtExit();
}

//------------------------------------------------------------------
void FWAppGLUI::Init(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	FWAppGL::Init(argc, argv);
	atexit(FWAppGLUI::AtExit);
}

void FWAppGLUI::Start(){
	instance = this;
	windowID = CreateWindow();
	CreateRender();
	glutDisplayFunc(FWAppGLUI::GluiDisplayFunc);
	glutReshapeFunc(FWAppGLUI::GluiReshapeFunc);
	glutKeyboardFunc(FWAppGLUI::GluiKeyboardFunc);
	glutTimerFunc(1, FWAppGLUI::GluiTimerFunc, 0);
	glutMainLoop();
}

void FWAppGLUI::Display(){
	FWAppGL::Display();
	/// change current buffer in double buffer mode
	glutSwapBuffers();
}

int FWAppGLUI::CreateWindow(const FWWindowDesc d){
	glutInitWindowSize(d.width, d.height);
	glutInitWindowPosition(d.top, d.left);
	int window =  glutCreateWindow("Springhead Application");
	int rv = glewInit();
	return window;
}

}
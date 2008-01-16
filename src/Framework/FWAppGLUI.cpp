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
#include <GL/glui.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWAppGLUI* FWAppGLUI::instance;

FWAppGLUIDesc::FWAppGLUIDesc(){
	fromTop				= 50;
	fromLeft			= 30;
	subPosition			= GLUI_SUBWINDOW_RIGHT;
	gluiName			= "Menu";
	createOtherWindow	= true;
}

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
	FWSceneIf* fwScene = FWAppGLUI::instance->GetSdk()->GetScene();
	if(!fwScene)return;

	PHSceneIf* phScene = fwScene->GetPHScene();
	if(!phScene)return;

	int timeStep = (int)(phScene->GetTimeStep() * 100.0);
	if(timeStep<1) timeStep = 1;
	GLUI_Master.set_glutTimerFunc(timeStep, GluiTimerFunc, 0);
}

void FWAppGLUI::GluiIdleFunc(){
	glutPostRedisplay();
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
	if (!windows.size()){
		CreateWin();
		CreateRender(windows.front().id);
	}
	GLUI_Master.set_glutTimerFunc(1, FWAppGLUI::GluiTimerFunc, 0);
	GLUI_Master.set_glutIdleFunc(FWAppGLUI::GluiIdleFunc);

	if (!guis.size()){
		CreateGUI(windows.front().id);
	}
	glutMainLoop();
}

GLUI* FWAppGLUI::CreateGUI(int wid, FWAppGLUIDesc desc){
	GLUI* glui;
	if(!createOtherWindow){
		glui = GLUI_Master.create_glui_subwindow(wid, subPosition);		
	}else{
		glui = GLUI_Master.create_glui(gluiName, 0, fromLeft, fromTop);
		glui->set_main_gfx_window(wid);
	}
	guis.push_back(glui);
	return glui;
}

}
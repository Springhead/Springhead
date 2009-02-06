/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWHapticAppBase.h>
#include <GL/glut.h>
#include <sstream>

#ifdef USE_HDRSTOP
#pragma  hdrstop
#endif

namespace Spr{;

FWHapticAppBase::FWHapticAppBase(){}

void FWHapticAppBase::Init(int argc, char* argv[]){
	FWAppGLUT::Init(argc, argv);
	GetSdk()->Clear();


	FWWinDesc windowDesc;
	windowDesc.title = "(c)Springhead2    FWHapticTest";
	FWWin* window = CreateWin(windowDesc);
	window->scene = GetSdk()->GetScene();
}

void FWHapticAppBase::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWHapticAppBase::Start(){
	instance = this;
	if (!NWin()){
		CreateWin();
		wins.back()->SetScene(GetSdk()->GetScene());
	}
//	glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	glutIdleFunc(FWAppGLUT::GlutIdleFunc);
	glutMainLoop();
}

void FWHapticAppBase::Step(){}

void FWHapticAppBase::Idle(){
	glutPostRedisplay();
}

void FWHapticAppBase::Display(){}

void FWHapticAppBase::Keyboard(int key, int x, int y){}

}
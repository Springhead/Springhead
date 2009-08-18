/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWMultiWinSample.h"
#include <iostream>
#include <sstream>

#define ESC 27


FWMultiWinSample::FWMultiWinSample(){
	nWin = 2;
	std::stringstream sst;		
	for(int i = 0; i < nWin; i++){
		sst <<"Window" << i+1;
		winNames.push_back(sst.str());
		sst.str("");						// バッファをクリアする
		sst.clear(stringstream::goodbit);	// ストリームの状態をクリアする
	}
	sst << "./xFiles/cookie/sceneCookie.x";
	fileName = sst.str();
}

void FWMultiWinSample::Init(int argc, char* argv[]){
	SetGRAdaptee(TypeGLUT);	
	GetGRAdaptee()->Init(argc, argv);
	CreateSdk();
	GetSdk()->Clear();
	GetSdk()->LoadScene(fileName);
	InitCameraView();
	for(int i = 0; i < nWin ; i++){
		FWWinDesc winDesc;
		winDesc.width = 480;
		winDesc.height = 320;
		winDesc.left = 10 + 500 * i;
		winDesc.top	= 30;
		winDesc.parentWindow = 0;
		winDesc.fullscreen = false;
		if(winNames[i].size()){
			winDesc.title = winNames[i];
		}else{
			winDesc.title = "Window";
		}
		FWWin* win = CreateWin(winDesc);
		win->SetScene(GetSdk()->GetScene());
	}
	GTimer* gtimer = CreateTimerFunc();
	gtimer->Interval(GetSdk()->GetScene()->GetPHScene()->GetTimeStep());
	gtimer->Set(CallStep);
	gtimer->Create(GetGRAdaptee());
}
void FWMultiWinSample::CallStep(int id){
	((FWMultiWinSample*)instance)->GetTimerFunc(id)->Loop();
	((FWMultiWinSample*)instance)->Step();
}
void FWMultiWinSample::Step(){
	GetSdk()->GetScene()->Step();
	for(int i = 0; i < NWin(); i++){
		SetCurrentWin(GetWin(i));
		GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
		if (cam && cam->GetFrame()){
			cam->GetFrame()->SetTransform(cameraInfo.view);
		}else{
			GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
		}
		bool bDebug = false;
		if(i == 1) bDebug = true; 
		DebugDisplay(bDebug);
		Display();
//		glutPostRedisplay();
	}
}

void FWMultiWinSample::Display(){
	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();
	glutSwapBuffers();
}

void FWMultiWinSample::DebugDisplay(bool bDebug){
	GetSdk()->SetDebugMode(bDebug);
	GRDebugRenderIf* r = GetCurrentWin()->render->Cast();
	r->SetRenderMode(true, false);
//	r->EnableRenderAxis();
	r->EnableRenderForce(bDebug);
	r->EnableRenderContact(bDebug);
}

void FWMultiWinSample::InitCameraView(){
	///	Affinef 型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.999894 -0.00902454 0.011391 0.00379173)"
		 "(-1.70237e-010 0.783822 0.620986 0.206708)"
		 "(-0.0145326 -0.62092 0.783739 0.260884)"
		 "(     0      0      0      1))"
		);
	issView >> cameraInfo.view;
}

void FWMultiWinSample::Keyboard(int key, int x, int y){
	switch(key){
		case ESC:
		case 'q':
			exit(0);
			break;
		default:
			break;
	}
}
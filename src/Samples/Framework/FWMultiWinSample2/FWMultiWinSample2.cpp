/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWMultiWinSample2.h"
#include <iostream>
#include <sstream>

#define ESC 27

FWMultiWinSample2::FWMultiWinSample2(){
	nWin = 3;
	std::stringstream sst;		
	for(int i = 0; i < nWin; i++){
		sst <<"Window" << i+1;
		winNames.push_back(sst.str());
		sst.str("");						// バッファをクリアする
		sst.clear(stringstream::goodbit);	// ストリームの状態をクリアする
		sst << "./xFiles/scene" << i + 1 << ".x";
		fileNames.push_back(sst.str());
		sst.str("");						
		sst.clear(stringstream::goodbit);	
	}
}

void FWMultiWinSample2::Init(int argc, char* argv[]){
	/// シミュレーションの初期化
	CreateSdk();
	GetSdk()->Clear();
	/// 描画タイプの指定
	SetGRAdaptee(TypeGLUT);	
	GetGRAdaptee()->Init(argc, argv);
	/// ウィンドウの作成とファイルのロード
	for(int i = 0; i < nWin ; i++){
		GetSdk()->LoadScene(fileNames[i]);
		FWWinDesc winDesc;
		winDesc.width = 480;
		winDesc.height = 320;
		winDesc.left = 10 + 500 * (i / 2);
		winDesc.top	= 30 + 360 * (i % 2);
		winDesc.parentWindow = 0;
		winDesc.fullscreen = false;
		if(winNames[i].size()){
			winDesc.title = winNames[i];
		}else{
			winDesc.title = "Window";
		}
		FWWin* win = CreateWin(winDesc);
		win->SetScene(GetSdk()->GetScene(i));
	}

	/// カメラ位置の初期化
	InitCameraView();

	/// コールバックタイマの作成と設定 
	for(int i = 0; i < (int)GetSdk()->NScene(); i++){
		CreateTimer();
	}
}

void FWMultiWinSample2::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWMultiWinSample2::Reset(){
	GetSdk()->Clear();
	DSTR << "Reload the Xfile" << std::endl;
	for(int i = 0; i < nWin ; i++){
		GetSdk()->LoadScene(fileNames[i]);
		GetWin(i)->SetScene(GetSdk()->GetScene(i));
	}
	InitCameraView();	
}

void FWMultiWinSample2::TimerFunc(int i){
	GetSdk()->GetScene(i)->Step();
	SetCurrentWin(GetWin(i));
	GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
	if (cam && cam->GetFrame()){
		cam->GetFrame()->SetTransform(cameraInfo.view);
	}else{
		GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
	} 
	GetGRAdaptee()->PostRedisplay();
}

void FWMultiWinSample2::Display(){
	bool bDebug = true;
	GetSdk()->SetDebugMode(bDebug);
	GRDebugRenderIf* r = GetCurrentWin()->render->Cast();
	r->SetRenderMode(true, true);
//	r->EnableRenderAxis();
	r->EnableRenderForce(bDebug);
	r->EnableRenderContact(bDebug);

	if(!GetCurrentWin()) return;
	GetSdk()->SwitchScene(GetCurrentWin()->GetScene());
	GetSdk()->SwitchRender(GetCurrentWin()->GetRender());
	GetSdk()->Draw();
	glutSwapBuffers();
}

void FWMultiWinSample2::Keyboard(int key, int x, int y){
	switch(key){
		case ESC:
		case 'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		default:
			break;
	}
}
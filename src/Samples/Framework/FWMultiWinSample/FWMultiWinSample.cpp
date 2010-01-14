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
	nWin = 2;								// ウィンドウの数
	std::stringstream sst;
	/// ウィンドウに名前を付ける
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
	/// シミュレーションの初期化
	CreateSdk();
	GetSdk()->Clear();
	/// 描画モードの指定，初期化
	SetGRAdaptee(TypeGLUT);	
	GRInit(argc, argv);
	/// 描画ウィンドウの作成，初期化
	for(int i = 0; i < nWin ; i++){
		GetSdk()->CreateScene(PHSceneDesc(),GRSceneDesc());
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
		win->SetScene(GetSdk()->GetScene(i));
	}
	BuildScene();
	/// カメラ行列の初期化
	InitCameraView();

	/// タイマの作成
	CreateTimer();
}

void FWMultiWinSample::InitCameraView(){
	/// 初期カメラ行列
	std::istringstream issView(
		"((0.999894 -0.00902454 0.011391 0.00379173)"
		 "(-1.70237e-010 0.783822 0.620986 0.206708)"
		 "(-0.0145326 -0.62092 0.783739 0.260884)"
		 "(     0      0      0      1))"
		);
	issView >> cameraInfo.view;
}

void FWMultiWinSample::Reset(){
	GetSdk()->Clear();
	DSTR << "Reload the Xfile" << std::endl;
	GetSdk()->LoadScene(fileName);
	for(int i = 0; i < nWin; i++){
		GetWin(i)->SetScene(GetSdk()->GetScene());
	}
	InitCameraView();
}

void FWMultiWinSample::TimerFunc(int id){
	Step();
	PostRedisplay();
}

void FWMultiWinSample::Step(){
	/// 各ウィンドウに描画 
	for(int i = 0; i < NWin(); i++){
		GetSdk()->GetScene(i)->Step();
		SetCurrentWin(GetWin(i));
		GRCameraIf* cam = GetCurrentWin()->scene->GetGRScene()->GetCamera();
		if (cam && cam->GetFrame()){
			cam->GetFrame()->SetTransform(cameraInfo.view);
		}else{
			GetCurrentWin()->render->SetViewMatrix(cameraInfo.view.inv());
		}
		bool bDebug = true;
		if(i == 1) bDebug = true; 
		DebugDisplay(bDebug);
		Display();
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
	r->EnableRenderWorldAxis(true);
	r->EnableGrid();
}

void FWMultiWinSample::Keyboard(int key, int x, int y){
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

void FWMultiWinSample::BuildScene(){
	PHSceneIf* phScene1 = GetSdk()->GetPHSdk()->GetScene(0);
	CreateJointScene(phScene1);
	PHSceneIf* phScene2 = GetSdk()->GetPHSdk()->GetScene(1);
	CreateJointScene(phScene2);
}

void FWMultiWinSample::CreateJointScene(PHSceneIf* phScene){
	PHSolidDesc sDesc;
	
	CDBoxDesc bDesc;
	bDesc.boxsize = Vec3d(0.01,0.01,0.01);
	CDShapeIf* shapebox = phScene->GetSdk()->CreateShape(bDesc);
	PHBallJointDesc desc;
	{
		desc.poseSocket.Pos()	= Vec3f(-0.01f,0.0f , 0.0f);
		desc.posePlug.Pos()	= Vec3f(0.01f, 0.0f, 0.0f);
		desc.spring			= 3;
		desc.damper		= 2;
	}
	PHSolidIf* rootSolid = phScene->CreateSolid(sDesc);
	rootSolid->AddShape(shapebox);
	rootSolid->SetDynamical(false);
	float posy = 0;
	rootSolid->SetFramePosition(Vec3d(-0, posy, 0));
	for(int i = 1; i < 6; i++){
		PHSolidIf* nodeSolid = phScene->CreateSolid(sDesc);
		nodeSolid->AddShape(shapebox);
		phScene->CreateJoint(rootSolid, nodeSolid, desc);
		nodeSolid->SetFramePosition(Vec3d(posy - 0.01 * i, 1, 0));
		phScene->SetContactMode(rootSolid, nodeSolid, PHSceneDesc::MODE_NONE);
		rootSolid = nodeSolid;
	}
}
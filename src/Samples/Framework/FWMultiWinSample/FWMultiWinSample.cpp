/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWMultiWinSample.h"
#include <iostream>

using namespace std;

FWMultiWinSample::FWMultiWinSample(){
}

void FWMultiWinSample::Init(int argc, char* argv[]){
	/// シミュレーションの初期化
	CreateSdk();
	GetSdk()->Clear();

	/// 描画モードの指定，初期化
	SetGRAdaptee(TypeGLUT);	
	GRInit(argc, argv);

	/// シーンをロード
	GetSdk()->LoadScene("./xFiles/cookie/sceneCookie.x");
	GetSdk()->LoadScene("./xFiles/scene1.x");
	GetSdk()->LoadScene("./xFiles/scene2.x");
	GetSdk()->LoadScene("./xFiles/scene3.x");

	/// 描画ウィンドウの作成，初期化
	FWWinDesc winDesc;
	winDesc.width	= 480;
	winDesc.height	= 320;
	winDesc.left	= 10;
	winDesc.top		= 30;
	winDesc.fullscreen = false;
	
	for(int i = 0; i < 5; i++)
		CreateWin(winDesc);
	
	GetWin(0)->SetTitle("Window1: coockie (PHScene)");
	GetWin(1)->SetTitle("Window2: coockie (GRScene)");
	GetWin(2)->SetTitle("Window3: scene1");
	GetWin(3)->SetTitle("Window4: scene2");
	GetWin(4)->SetTitle("Window5: scene3");

	// ウィンドウの位置決め．タイトルバーやフレーム分のマージンは自分で計算する必要がある
	GetWin(0)->SetPosition(  10,  40);
	GetWin(1)->SetPosition( 510,  40);
	GetWin(2)->SetPosition(  10, 410);
	GetWin(3)->SetPosition( 510, 410);
	GetWin(4)->SetPosition(1010, 410);
	
	GetWin(0)->SetScene(GetSdk()->GetScene(0));
	GetWin(1)->SetScene(GetSdk()->GetScene(0));
	GetWin(2)->SetScene(GetSdk()->GetScene(1));
	GetWin(3)->SetScene(GetSdk()->GetScene(2));
	GetWin(4)->SetScene(GetSdk()->GetScene(3));

	/// 描画モード設定
	GetWin(0)->SetDebugMode(true);
	GetWin(1)->SetDebugMode(false);
	GetWin(2)->SetDebugMode(true);
	GetWin(3)->SetDebugMode(true);
	GetWin(4)->SetDebugMode(true);
	
	for(int i = 0; i < GetSdk()->NScene(); i++){
		FWSceneIf* scene = GetSdk()->GetScene(i);
		scene->SetRenderMode(true, false);
		scene->EnableRenderAxis();
		scene->EnableRenderForce();
		scene->EnableRenderContact();
		scene->EnableRenderGrid();
	}

	/// タイマの作成
	CreateTimer();
}

void FWMultiWinSample::TimerFunc(int id){	
	// 全ウィンドウに対して再描画要求
	for(int i = 0; i < NWin(); i++){
		SetCurrentWin(GetWin(i));
		PostRedisplay();
	}
}

void FWMultiWinSample::Keyboard(int key, int x, int y){
	switch(key){
	case DVKeyCode::ESC:
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

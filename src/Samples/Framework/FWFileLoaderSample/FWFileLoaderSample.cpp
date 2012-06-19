/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWFileLoaderSample.h"

#define USE_SPRFILE
#define ESC 27

FWFileLoaderSample::FWFileLoaderSample(){
#ifdef USE_SPRFILE
	fileName = "./files/sceneSample.spr";	// sprファイル
#else
	fileName = "./files/sceneSample.x";		// xファイル
#endif
}

void FWFileLoaderSample::Init(int argc, char* argv[]){
	CreateSdk();			// SDKの作成
	UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
	GetSdk()->LoadScene(fileName, import);			// ファイルのロード
	GetSdk()->SaveScene("save.spr", import);		// ファイルのセーブテスト
	GRInit(argc, argv);		// ウィンドウマネジャ初期化
	CreateWin();			// ウィンドウを作成
	CreateTimer();			// タイマを作成

	InitCameraView();		// カメラビューの初期化
	GetSdk()->SetDebugMode(false);						// デバックモードの無効化
	GetSdk()->GetScene()->EnableRenderAxis(true);		// 座標軸の表示
	GetSdk()->GetScene()->EnableRenderContact(true);	// 接触領域の表示
}

void FWFileLoaderSample::InitCameraView(){
	Vec3d pos = Vec3d(-0.978414, 11.5185, 24.4473);		// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
}

void FWFileLoaderSample::Reset(){
	GetSdk()->Clear();
	GetSdk()->LoadScene(fileName);
	GetCurrentWin()->SetScene(GetSdk()->GetScene());
}


void FWFileLoaderSample::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			// アプリケーションの終了
			exit(0);
			break;
		case 'r':
			// ファイルの再読み込み
			Reset();
			break;
		case 'w':
			// カメラ位置の初期化
			InitCameraView();	
			break;
		case 'd':				
			{
				// デバック表示
				static bool bDebug = GetSdk()->GetDebugMode();
				if(bDebug)	bDebug = false;
				else		bDebug = true;
				GetSdk()->SetDebugMode(bDebug);
				DSTR << "Debug Mode " << bDebug << std::endl;
				//DSTR << "CameraPosition" << std::endl;
				//DSTR << GetCurrentWin()->GetTrackball()->GetPosition() << std::endl;
			}
			break;
		default:
			break;
	}
}
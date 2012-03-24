/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWSprFileLoader.h"

#define ESC 27

FWSprfileLoader::FWSprfileLoader(){
	fileName = "./SprFiles/sceneSample.spr";
}

void FWSprfileLoader::Init(int argc, char* argv[]){
	/// シミュレーションの初期化
	FWApp::Init();
	UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
	GetSdk()->LoadScene(fileName, import);			// ファイルのロード
	GetSdk()->SaveScene("save.spr", import);		///	ファイルのセーブテスト

	PHSolidIf* solid = GetSdk()->GetScene()->GetPHScene()->CreateSolid();
	CDBoxDesc bd;
	bd.boxsize.clear(1.0);
	solid->AddShape(GetSdk()->GetPHSdk()->CreateShape(bd));
	solid->SetDynamical(false);

	/// カメラビューの初期化
	InitCameraView();
	GetSdk()->GetScene()->EnableRenderAxis(true);
	GetSdk()->GetScene()->EnableRenderContact(true);

}

void FWSprfileLoader::InitCameraView(){
}

void FWSprfileLoader::Reset(){
	GetSdk()->Clear();		
	GetSdk()->LoadScene(fileName);
	GetCurrentWin()->SetScene(GetSdk()->GetScene());
}


void FWSprfileLoader::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			exit(0);
			break;
		case 'r':
			Reset();			// ファイルの再読み込み
			break;
		case 'w':				// カメラ初期化
			InitCameraView();	
			break;
		case 'd':				// デバック表示
			{
				static bool bDebug = true;
				if(bDebug)	bDebug = false;
				else		bDebug = true;
				GetSdk()->SetDebugMode(bDebug);
				DSTR << "Debug Mode " << bDebug << std::endl;
			}
			break;
		default:
			break;
	}
}
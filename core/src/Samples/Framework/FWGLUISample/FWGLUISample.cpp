/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWGLUISample.h"
#include <iostream>
#include <sstream>

FWGLUISample::FWGLUISample(){
}

void FWGLUISample::Init(int argc, char* argv[]){
	/// SDK作成
	CreateSdk();

	/// グラフィクスをGLUIで初期化
	GRInit(argc, argv, TypeGLUI);

	/// Sceneの作成
	GetSdk()->CreateScene();
	
	FWWinDesc windowDesc;
	windowDesc.title = "FWGLUISample";
	CreateWin(windowDesc);
	
	/// タイマを作成
	CreateTimer();
}

void FWGLUISample::CreateObjects(){
	PHSceneIf* phscene = GetSdk()->GetScene(0)->GetPHScene();
	PHSolidDesc desc;
	CDBoxDesc bd;

	// 床(物理法則に従わない，運動が変化しない)
	{
		// 剛体(soFloor)の作成
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		// 形状(shapeFloor)の作成
		bd.boxsize = Vec3f(50, 10, 50);
		CDShapeIf* shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加する
		soFloor->AddShape(shapeFloor);
		soFloor->SetFramePosition(Vec3d(0, -5, 0));
	}

	// 箱(物理法則に従う，運動が変化)
	{
		// 剛体(soBox)の作成
		desc.mass = 0.5;
		desc.inertia *= 0.033;
		PHSolidIf* soBox = phscene->CreateSolid(desc);
		// 形状(shapeBox)の作成
		bd.boxsize = Vec3f(2,2,2);
		CDShapeIf* shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加
		soBox->AddShape(shapeBox);
		soBox->SetFramePosition(Vec3d(0, 10, 0));
	}
}

void FWGLUISample::Keyboard(int key, int x, int y){
	switch (key) {
	case DVKeyCode::ESC:
	case 'q':
		exit(0);
		break;
	default:
		break;
}

}

/*
void FWGLUISample::DesignGUI(){
	panel = glui->add_panel("Sample", true);
	button1 = glui->add_button_to_panel(panel, "Create Box", 1, GLUI_CB(CallButton1));
}

void FWGLUISample::CallButton1(int control){
	((FWGLUISample*)FWApp::instance)->Button1(control);
}

void FWGLUISample::Button1(int control){
	Keyboard('1', 0, 0);
}
*/

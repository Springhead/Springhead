/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageVirtualHuman バーチャルヒューマンのデモプログラム
 Springhead2/src/Samples/Creature/VirtualHuman/main.cpp

\ref バーチャルヒューマンのデモプログラム

\secntion secSpecVirtualHuman
バーチャルヒューマンモデルを生成するデモです．
*/

#include <vector>
#include "../../SampleApp.h"
#include <GL/glut.h>

#include <Creature/CRBodyGenerator/CRBallHumanBodyGen.h>

#ifdef	_MSC_VER
  #pragma  hdrstop
#endif

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class VirtualHumanApp : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_RESET,
		ID_UP,
		ID_DOWN,
		ID_LEFT,
		ID_RIGHT,
	};

	int argc;
	char** argv;

	VirtualHumanApp(){
		appName = "VirtualHuman Sample";
		numScenes   = 1;

		AddAction(MENU_SCENE, ID_UP, "Pointer move Up");
		AddHotKey(MENU_SCENE, ID_UP, 'i');

		AddAction(MENU_SCENE, ID_DOWN, "Pointer move Down");
		AddHotKey(MENU_SCENE, ID_DOWN, 'k');

		AddAction(MENU_SCENE, ID_LEFT, "Pointer move Left");
		AddHotKey(MENU_SCENE, ID_LEFT, 'j');

		AddAction(MENU_SCENE, ID_RIGHT, "Pointer move Right");
		AddHotKey(MENU_SCENE, ID_RIGHT, 'l');
	}
	~VirtualHumanApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		curScene = 0;

		GetFWScene()->GetPHScene()->GetConstraintEngine()->SetBSaveConstraints(true);
		GetCurrentWin()->GetTrackball()->SetPosition(Vec3f(3,3,5));
		GetFWScene()->EnableRenderAxis(true, false, false);
	}

	PHSolidIf*         soTarget;

	// シーン構築
	virtual void BuildScene() {
		PHSdkIf* phSdk = GetFWScene()->GetPHScene()->GetSdk();

		PHSolidDesc descSolid;

		CDSphereDesc descSphere;
		descSphere.radius  = 0.1f;

		// Pointer
		PHSolidIf* so4 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so4->SetFramePosition(Vec3d(1.2,0,0));
		so4->AddShape(phSdk->CreateShape(descSphere));
		so4->SetDynamical(false);

		soTarget = so4;

		CRSdkIf* crSdk = CRSdkIf::CreateSdk();
		CRCreatureDesc descCreature;
		CRCreatureIf* crCreature = crSdk->CreateCreature(descCreature);
		crCreature->AddChildObject(GetFWScene()->GetPHScene());
		CRBallHumanBodyGenDesc descBody;
		CRBallHumanBodyGen bodyGen(descBody);
		CRBodyIf* body = bodyGen.Generate(crCreature);

		body->FindByLabel("waist")->GetPHSolid()->SetDynamical(false);
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE){
			if(id == ID_UP){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.0,  0.1,  0.0));
			}

			if(id == ID_DOWN){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.0, -0.1,  0.0));
			}

			if(id == ID_LEFT){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d(-0.1,  0.0,  0.0));
			}

			if(id == ID_RIGHT){
				Vec3d currPos = soTarget->GetPose().Pos();
				soTarget->SetFramePosition(currPos + Vec3d( 0.1,  0.0,  0.0));
			}
		}

		SampleApp::OnAction(menu, id);
	}
} app;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
#if _MSC_VER <= 1500
#include <GL/glut.h>
#endif
int __cdecl main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

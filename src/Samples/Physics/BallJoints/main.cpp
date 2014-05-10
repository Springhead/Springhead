/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageBallJointSample ボールジョイントのサンプル
 Springhead2/src/Samples/IK/main.cpp

\ref pagePhysics のボールジョイント機能のデモプログラム。
*/

#include "../../SampleApp.h"

#pragma  hdrstop

using namespace Spr;
using namespace std;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class IKSampleApp : public SampleApp{
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
		ID_FORE,
		ID_BACK,
	};

	int argc;
	char** argv;

	IKSampleApp(){
		appName = "IK Sample";
		numScenes   = 1;
	}
	~IKSampleApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]) {
		this->argc = argc;
		this->argv = argv;

		SampleApp::Init(argc, argv);
		ToggleAction(MENU_ALWAYS, ID_RUN);
		ToggleAction(MENU_ALWAYS, ID_DRAW_WIREFRAME);
		curScene = 0;

		GetCurrentWin()->GetTrackball()->SetPosition(Vec3d(50,10,10));
	}

	// シーン構築
	virtual void BuildScene() {
		PHSdkIf* phSdk = GetFWScene()->GetPHScene()->GetSdk();

		PHSolidDesc descSolid;

		CDRoundConeDesc descCapsule;
		descCapsule.radius = Vec2f(0.3, 0.3);
		descCapsule.length = 4;

		// Base Link
		PHSolidIf* so0 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so0->AddShape(phSdk->CreateShape(descCapsule));
		so0->SetDynamical(false);
		PHTreeNodeIf* nd = GetFWScene()->GetPHScene()->CreateRootNode(so0);
		PHTreeNodeIf* rt = nd;

		// Link 1
		PHSolidIf* so1 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so1->AddShape(phSdk->CreateShape(descCapsule));

		// Link 2
		PHSolidIf* so2 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so2->AddShape(phSdk->CreateShape(descCapsule));

		// ----- ----- ----- ----- -----

		{
			PHBallJointDesc descBall;
			descBall.poseSocket.Pos() = Vec3d(0,0, 2);
			descBall.posePlug.Pos()   = Vec3d(0,0,-2);
			descBall.spring = 0;
			descBall.damper = 0;

			// Base <-> Link 1
			PHBallJointIf* jo1  = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descBall)->Cast();
			{
				PHBallJointConeLimitDesc ld;
				jo1->CreateLimit(ld);
				DCAST(PHBallJointConeLimitIf,jo1->GetLimit())->SetSpring(1000);
				DCAST(PHBallJointConeLimitIf,jo1->GetLimit())->SetDamper(  10);
				DCAST(PHBallJointConeLimitIf,jo1->GetLimit())->SetSwingRange(Vec2d(Rad(-10), Rad(5)));
			}

			nd = GetFWScene()->GetPHScene()->CreateTreeNode(nd, so1);
		}

		// Link 1 <-> Link 2
		{
			PHBallJointDesc descBall;
			descBall.poseSocket.Pos() = Vec3d(0,0, 2);
			descBall.posePlug.Pos()   = Vec3d(0,0,-2);
			descBall.spring = 0;
			descBall.damper = 0;

			PHBallJointIf* jo2  = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descBall)->Cast();
			{
				PHBallJointConeLimitDesc ld;
				jo2->CreateLimit(ld);
				DCAST(PHBallJointConeLimitIf,jo2->GetLimit())->SetSpring(1000);
				DCAST(PHBallJointConeLimitIf,jo2->GetLimit())->SetDamper(  10);
				DCAST(PHBallJointConeLimitIf,jo2->GetLimit())->SetSwingRange(Vec2d(Rad(-10), Rad(100)));
			}

			nd = GetFWScene()->GetPHScene()->CreateTreeNode(nd, so2);
		}

		// ----- ----- ----- ----- -----

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);

		rt->Enable(/*/ true /*/ false /**/);
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

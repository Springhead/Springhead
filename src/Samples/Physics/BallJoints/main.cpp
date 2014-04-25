/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageIKSample 逆運動学（IK）機能のデモ
 Springhead2/src/Samples/IK/main.cpp

\ref pagePhysics の逆運動学（IK）機能のデモプログラム。

\secntion secSpecIKSample 仕様
アームの先端剛体の中心位置ががポインタ（球体）の位置になるよう到達運動する．
第一・第二関節ともに可動域制限がかかっている．
- スペースキーでシミュレーション開始．
- 'i'をタイプするとポインタ位置が上に動く．
- 'k'で、下へ
- 'j'で、左へ
- 'l'で、右へ
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

		// Link 1
		PHSolidIf* so1 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so1->AddShape(phSdk->CreateShape(descCapsule));

		// Link 2
		PHSolidIf* so2 = GetFWScene()->GetPHScene()->CreateSolid(descSolid);
		so2->AddShape(phSdk->CreateShape(descCapsule));

		// ----- ----- ----- ----- -----

		PHBallJointDesc descBall;
		descBall.poseSocket.Pos() = Vec3d(0,0, 2);
		descBall.posePlug.Pos()   = Vec3d(0,0,-2);

		// Base <-> Link 1
		PHBallJointIf* jo1  = GetFWScene()->GetPHScene()->CreateJoint(so0, so1, descBall)->Cast();

		// Link 1 <-> Link 2
		PHBallJointIf* jo2  = GetFWScene()->GetPHScene()->CreateJoint(so1, so2, descBall)->Cast();

		// ----- ----- ----- ----- -----

		GetFWScene()->GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
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

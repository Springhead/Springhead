/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
\page pageBoxStack キー入力で箱を生成して積み上げるサンプル
Springhead2/src/Samples/BoxStack

\section secQuitBoxStack 終了基準
- Escか'q'で強制終了。
- ' ', 'm', 'd', 'c', 'v', 'z', 'x'で箱が落ちてくる
	
\section secFlowBoxStack 処理の流れ
- シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。
- 接触エンジンを拘束エンジンに設定する。
- 与えられた条件により⊿t(=0.1)秒後の位置の変化を積分し、OpenGLでシミュレーションする。  
- ユーザのキー入力に対しSolidを発生させる。
*/

#include "../../SampleApp.h"

#pragma hdrstop
using namespace Spr;
using namespace std;


namespace Spr {
	extern int s_methodSW;
}

class MyApp : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_BOX,
		ID_CAPSULE,
		ID_ROUNDCONE,
		ID_SPHERE,
		ID_ROCK,
		ID_BLOCK,
		ID_SHAKE,
		ID_METHOD,
		ID_CCD,
		ID_SHOWTIME,
	};

	PHSolidIf*				soFloor;
	std::vector<PHSolidIf*> soBox;

	bool showColtime;
	int aveCounter;
	int avePool;
	double					floorShakeAmplitude;

public:
	MyApp(){
		appName = "BoxStack";
		floorShakeAmplitude = 0.0;
		aveCounter = 0;
		avePool = 0;

		AddAction(MENU_MAIN, ID_BOX, "drop box");
		AddHotKey(MENU_MAIN, ID_BOX, 'b');
		AddAction(MENU_MAIN, ID_CAPSULE, "drop capsule");
		AddHotKey(MENU_MAIN, ID_CAPSULE, 'c');
		AddAction(MENU_MAIN, ID_ROUNDCONE, "drop round cone");
		AddHotKey(MENU_MAIN, ID_ROUNDCONE, 'r');
		AddAction(MENU_MAIN, ID_SPHERE, "drop sphere");
		AddHotKey(MENU_MAIN, ID_SPHERE, 's');
		AddAction(MENU_MAIN, ID_ROCK, "drop rock");
		AddHotKey(MENU_MAIN, ID_ROCK, 'd');
		AddAction(MENU_MAIN, ID_BLOCK, "drop block");
		AddHotKey(MENU_MAIN, ID_BLOCK, 'e');
		AddAction(MENU_MAIN, ID_SHAKE, "shake floor");
		AddHotKey(MENU_MAIN, ID_SHAKE, 'f');
		AddAction(MENU_MAIN, ID_METHOD, "switch method");
		AddHotKey(MENU_MAIN, ID_METHOD, 'z');
		AddAction(MENU_MAIN, ID_CCD, "switch CCD");
		AddHotKey(MENU_MAIN, ID_CCD, 'x');
		AddAction(MENU_MAIN, ID_SHOWTIME, "Show Time");
		AddHotKey(MENU_MAIN, ID_SHOWTIME, 'n');

	}
	~MyApp(){}

	virtual void BuildScene(){
		soFloor = CreateFloor();
		GetPHScene()->SetGravity(Vec3d(0, -30, 0));
		FWWinIf* win = GetCurrentWin();
		win->GetTrackball()->SetPosition(Vec3f(30, 50, 100)); //注視点設定
	}

	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void OnStep() {

		SampleApp::OnStep();
		// 床を揺らす
		if (soFloor){
			double time = GetFWScene()->GetPHScene()->GetCount() * GetFWScene()->GetPHScene()->GetTimeStep();
			double omega = 2.0 * M_PI;
			soFloor->SetFramePosition(Vec3d(floorShakeAmplitude*sin(time*omega),0,0));			
			soFloor->SetVelocity(Vec3d(floorShakeAmplitude*omega*cos(time*omega),0,0));
		}

		if (showColtime)
		{
			
			if (aveCounter < 100)
			{
				avePool += GetPHScene()->GetConstraintEngine()->GetCollisionTime();
				aveCounter++;
			}
			else {
				int time = avePool/100;
				message = "Collision Time : " + to_string(time);
				aveCounter = 0;
				avePool = 0;
				message = "Collision Time : " + to_string(time);
			}
			
		}
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		SampleApp::OnDraw(render);

		std::ostringstream sstr;
		sstr << "NObj = " << GetFWScene()->GetPHScene()->NSolids();
		render->DrawFont(Vec2f(-21, 23), sstr.str());
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_MAIN){
			Vec3d v, w(0.0, 0.0, 0.2), p(0.5, 20.0, 0.0);
			Quaterniond q = Quaterniond::Rot(Rad(30.0), 'y');

			if(id == ID_BOX){
				Drop(SHAPE_BOX, GRRenderIf::RED, v, w, p, q);
				message = "box dropped.";
			}
			if(id == ID_CAPSULE){
				Drop(SHAPE_CAPSULE, GRRenderIf::GREEN, v, w, p, q);
				message = "capsule dropped.";
			}
			if(id == ID_ROUNDCONE){
				Drop(SHAPE_ROUNDCONE, GRRenderIf::BLUE, v, w, p, q);
				message = "round cone dropped.";
			}
			if(id == ID_SPHERE){
				Drop(SHAPE_SPHERE, GRRenderIf::YELLOW, v, w, p, q);
				message = "sphere dropped.";
			}
			if(id == ID_ROCK){
				Drop(SHAPE_ROCK, GRRenderIf::ORANGE, v, w, p, q);
				message = "random polyhedron dropped.";
			}
			if(id == ID_BLOCK){
				Drop(SHAPE_BLOCK, GRRenderIf::CYAN, v, w, p, q);
				message = "composite block dropped.";
			}
			if(id == ID_SHAKE){
				std::cout << "F: shake floor." << std::endl;
				if(floorShakeAmplitude == 0.0){
					floorShakeAmplitude = 2.5;
					message = "floor shaken.";
				}
				else{
					floorShakeAmplitude = 0;
					message = "floor stopped.";
				}
			}

			if (id == ID_METHOD)
			{
				s_methodSW = (s_methodSW + 1) % 3;
				switch (s_methodSW)
				{
				case 0:
					message = "method : normal";
					break;
				case 1:
					message = "method : accel";
					break;
				case 2:
					message = "method : Gino";
					break;
				case 3:
					message = "method : GJK";
					break;
				default:
					break;
				}
				aveCounter = 0;
				avePool = 0;
				
			}

			if (id == ID_CCD)
			{
				bool CCDstate = !(GetPHScene()->IsCCDEnabled());
				if (CCDstate) message = "CCD : Enable";
				else message = "CCD : Disable";
				GetPHScene()->EnableCCD(CCDstate);
			}

			if (id == ID_SHOWTIME) 
			{
				showColtime = !showColtime;
				GetPHScene()->GetConstraintEngine()->EnableReport(showColtime);
				aveCounter = 0;
				avePool = 0;
			}
		}
		SampleApp::OnAction(menu, id);
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

MyApp app;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int SPR_CDECL main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}


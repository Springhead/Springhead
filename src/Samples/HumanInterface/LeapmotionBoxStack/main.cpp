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

/*int frame=0,timepassed=0,timebase=0;
void fps(){
	frame++; 
	timepassed = glutGet(GLUT_ELAPSED_TIME); 
	if (1000 < timepassed - timebase) {
		std::cout << "   FPS:" << frame*1000.0/(timepassed - timebase);
		timebase = timepassed;		
		frame = 0;
	}
}*/

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
		//ID_TOWER,
		ID_SHAKE,
	};

	PHSolidIf*				soFloor;
	std::vector<PHSolidIf*> soBox;

	double					floorShakeAmplitude;

	HISdkIf*				hiSdk;

public:
	MyApp(){
		appName = "BoxStack";
		floorShakeAmplitude = 0.0;

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
		//AddAction(MENU_MAIN, ID_TOWER, "drop tower");
		//AddHotKey(MENU_MAIN, ID_TOWER, 't');
		AddAction(MENU_MAIN, ID_SHAKE, "shake floor");
		AddHotKey(MENU_MAIN, ID_SHAKE, 'f');

		hiSdk = HISdkIf::CreateSdk();
	}
	~MyApp(){}

	virtual void BuildScene(){
		soFloor = CreateFloor();

		/*
		HILeapDesc descLeap;
		HILeapIf* leap;
		leap = hiSdk->CreateHumanInterface(HILeapIf::GetIfInfoStatic())->Cast();
		leap->Init(&descLeap);
		*/
		
		/*
		HILeapUDPDesc descLeap;
		HILeapUDPIf* leap;
		leap = hiSdk->CreateHumanInterface(HILeapUDPIf::GetIfInfoStatic())->Cast();
		leap->Init(&descLeap);
		

		leap->SetScale(1/10.0);
		leap->SetCenter(Vec3d(0,-8,0));
		//leap->SetRotation(Quaterniond::Rot(Rad(90), 'x'));

		FWSkeletonSensorDesc descSS;
		FWSkeletonSensorIf* skelSensor = GetSdk()->GetScene(0)->CreateSkeletonSensor(descSS);
		skelSensor->AddChildObject(leap);
		*/
	}

	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void OnStep() {
		GetSdk()->GetScene(0)->UpdateSkeletonSensors();

		SampleApp::OnStep();

		// 床を揺らす
		if (soFloor){
			double time = GetFWScene()->GetPHScene()->GetCount() * GetFWScene()->GetPHScene()->GetTimeStep();
			double omega = 2.0 * M_PI;
			soFloor->SetFramePosition(Vec3d(floorShakeAmplitude*sin(time*omega),0,0));			
			soFloor->SetVelocity(Vec3d(floorShakeAmplitude*omega*cos(time*omega),0,0));
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
			/* 不具合ありにつき無効化
			if(id == ID_TOWER){
				const double tower_radius = 10;
				const int tower_height = 5;
				const int numbox = 20;
				double theta;
				for(int i = 0; i < tower_height; i++){
					for(int j = 0; j < numbox; j++){
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
						Drop(SHAPE_BOX, GRRenderIf::BLUE, Vec3d(), Vec3d(), Vec3d(0.5, 20, 0), Quaterniond::Rot(-theta, 'y'));
					}
				}
				message = "tower built.";
			}*/
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
int __cdecl main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}


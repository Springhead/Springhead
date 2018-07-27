/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
Boxstackベースで連続衝突判定の手法を切り替えて試せるサンプル
「z」で連続判定の手法切り替え
「x」で連続衝突と静的竈突の切り替え
「n」で左上に100フレームの平均衝突処理時間がでる（broadを含む)
*/
#include "../../SampleApp.h"
#include <windows.h> 
#include <mmsystem.h>

std::string ftos(double d, char* fmt = "%2.3f") {
	char buf[100];
	snprintf(buf, sizeof(buf), fmt, d);
	return std::string(buf);
}


#pragma hdrstop
using namespace Spr;
using namespace std;

namespace Spr{
	extern int		colcounter;
	extern int s_methodSW; //手法切り替えの変数
}

class MyApp : public SampleApp{
public:
	int frame=0;
	double fps=0;
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_ALL,
		ID_BOX,
		ID_CAPSULE,
		ID_ROUNDCONE,
		ID_SPHERE,
		ID_ELLIPSOID,
		ID_ROCK,
		ID_BLOCK,
		ID_TOWER,
		ID_COIN,
		ID_SHAKE,
		ID_METHOD,
		ID_CCD,
		ID_CLEARTIME,
		ID_SLIDER,
		ID_MEASURE,
		ID_GRAVITY,
		ID_COINSTACK,
		ID_SPHERESHOOT,
		ID_FENCEDROP,
		ID_ROTATION,
	};
	UTString state;

	int aveCounter = 0;
	double avePool = 0;
	double aveNarrow = 0;
	double aveBroad = 0;
	double avePhaseTime[3] = {0,0,0};
	Vec3d floorShakeAmplitude;
	bool bSlider = false;

public:
	virtual void Init(int argc, char* argv[]) {
		shapeScale = 0.01;
		SampleApp::Init(argc, argv);
	}
	void ClearTime() {
		aveCounter = 0;
		avePool = 0;
		aveNarrow = 0;
		aveBroad = 0;
		if (GetPHScene()) GetPHScene()->GetPerformanceMeasure()->ClearCounts();
		memset(avePhaseTime, 0, sizeof(avePhaseTime));
	}
	MyApp(){
		appName = "BoxStack";
		floorShakeAmplitude = Vec3d();
		ClearTime();

		AddAction(MENU_MAIN, ID_ALL, "drop all");
		AddHotKey(MENU_MAIN, ID_ALL, 'a');
		AddAction(MENU_MAIN, ID_BOX, "drop box");
		AddHotKey(MENU_MAIN, ID_BOX, 'b');
		AddAction(MENU_MAIN, ID_CAPSULE, "drop capsule");
		AddHotKey(MENU_MAIN, ID_CAPSULE, 'c');
		AddAction(MENU_MAIN, ID_ROCK, "drop rock");
		AddHotKey(MENU_MAIN, ID_ROCK, 'd');
		AddAction(MENU_MAIN, ID_BLOCK, "drop block");
		AddHotKey(MENU_MAIN, ID_BLOCK, 'e');
		AddAction(MENU_MAIN, ID_FENCEDROP, "fence and objects");
		AddHotKey(MENU_MAIN, ID_FENCEDROP, 'f');
		AddAction(MENU_MAIN, ID_GRAVITY, "gravity");
		AddHotKey(MENU_MAIN, ID_GRAVITY, 'g');
		AddAction(MENU_MAIN, ID_SHAKE, "shake floor");
		AddHotKey(MENU_MAIN, ID_SHAKE, 'h');
		AddAction(MENU_MAIN, ID_SLIDER, "slider");
		AddHotKey(MENU_MAIN, ID_SLIDER, 'i');
		AddAction(MENU_MAIN, ID_COINSTACK, "coine stack");
		AddHotKey(MENU_MAIN, ID_COINSTACK, 'k');
		AddAction(MENU_MAIN, ID_ROUNDCONE, "drop round cone");
		AddHotKey(MENU_MAIN, ID_ROUNDCONE, 'r');
		AddAction(MENU_MAIN, ID_SPHERE, "drop sphere");
		AddHotKey(MENU_MAIN, ID_SPHERE, 's');
		AddAction(MENU_MAIN, ID_ELLIPSOID, "drop Ellipsoid");
		AddHotKey(MENU_MAIN, ID_ELLIPSOID, 'E');
		AddAction(MENU_MAIN, ID_SPHERESHOOT, "shoot sphere");
		AddHotKey(MENU_MAIN, ID_SPHERESHOOT, 'p');
		AddAction(MENU_MAIN, ID_TOWER, "drop tower");
		AddHotKey(MENU_MAIN, ID_TOWER, 't');
		AddAction(MENU_MAIN, ID_COIN, "drop coin");
		AddHotKey(MENU_MAIN, ID_COIN, 'o');
		AddAction(MENU_MAIN, ID_METHOD, "switch method");
		AddHotKey(MENU_MAIN, ID_METHOD, 'z');
		AddAction(MENU_MAIN, ID_CCD, "switch CCD");
		AddHotKey(MENU_MAIN, ID_CCD, 'x');
		AddAction(MENU_MAIN, ID_CLEARTIME, "clear time");
		AddHotKey(MENU_MAIN, ID_CLEARTIME, 'l');
		AddAction(MENU_MAIN, ID_MEASURE, "measure");
		AddHotKey(MENU_MAIN, ID_MEASURE, 'm');
		AddAction(MENU_MAIN, ID_ROTATION, "rotation test");
		AddHotKey(MENU_MAIN, ID_ROTATION, 'R');
	}
	~MyApp(){}

	virtual void BuildScene() {
		shapeBox->SetBoxSize(shapeBox->GetBoxSize() / 2);
		PHSolidIf* soFloor = CreateFloor(false);
		FWWinIf* win = GetCurrentWin();
		win->SetFullScreen();
		win->GetTrackball()->SetTarget(Vec3d(0,0.06,0));
		win->GetTrackball()->SetPosition(0.01*Vec3f(0, 25, 50) * 0.9); //注視点設定
		PHSceneDesc pd;
		GetPHScene()->GetDesc(&pd);
		pd.timeStep = 1.0 / 60;
		pd.contactTolerance = 0.001 * 0.2;
		pd.airResistanceRateForAngularVelocity = 0.98;
		GetPHScene()->SetDesc(&pd);
		PHConstraintEngineDesc ed;
		GetPHScene()->GetConstraintEngine()->GetDesc(&ed);
		ed.freezeThreshold = 0;
		ed.contactCorrectionRate = 0.8;
		//ed.contactCorrectionRate = 0.2;
		ed.numIter = 500;
		GetPHScene()->GetConstraintEngine()->SetDesc(&ed);
		GetFWScene()->EnableRenderAxis(false, false, false);
		GetFWScene()->EnableRenderContact(false);
		GetFWScene()->SetRenderMode();

#if SMALLACCEL
		GetPHScene()->SetGravity(Vec3d(0, -9.8, 0) * 0.1);
#endif
	}
	int sceneCount;
	unsigned long lastTime = timeGetTime() - 30*100;
	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void OnStep() {
		frame++;
		const double FPSUPDATE = 30;
		if (frame == FPSUPDATE) {
			frame = 0;
			unsigned long time = timeGetTime();
			unsigned int delta = time - lastTime;
			lastTime = time;
			fps = FPSUPDATE / (delta * 0.001);
		}
		if (sceneCount) {
			ClearTime();
			double hOut = 0.08;
			switch (sceneCount) {
			case 10:
			case 70:
			case 130:
			case 190:
			case 250:
				for (int i = 0; i < 8; ++i) {
					Drop(SHAPE_BOX, GRRenderIf::RED, Vec3d(), Vec3d(), Vec3d(-0.01, hOut + i*0.015, 0), Quaterniond());
				}
				for (int i = 0; i < 16; ++i) {
					Drop(SHAPE_COIN, GRRenderIf::GOLDENROD, Vec3d(), Vec3d(), Vec3d(0.01, hOut + i*0.015, 0), Quaterniond());
				}
				break;
			case 300:{
				Drop(SHAPE_SPHERE, GRRenderIf::BLUE, Vec3d(), Vec3d(), Vec3d(-0.04, 0.96, 0), Quaterniond());
				int n = GetPHScene()->NSolids();
				GetPHScene()->GetSolids()[n - 1]->SetMass(GetPHScene()->GetSolids()[n - 1]->GetMass() * 10);
				GetPHScene()->GetSolids()[n - 1]->SetInertia(GetPHScene()->GetSolids()[n - 1]->GetInertia() * 10);
				soTable->CompInertia();
				break;
				}
			}
			sceneCount ++;
		}

		SampleApp::OnStep();
		// 床を揺らす
		PHSolidIf* soFloor = GetPHScene()->FindObject("soFloor")->Cast();
		if (soFloor){
			double time = GetFWScene()->GetPHScene()->GetCount() * GetFWScene()->GetPHScene()->GetTimeStep();
			double omega = 2.0 * M_PI * 10;
			soFloor->SetFramePosition(floorShakeAmplitude*sin(time*omega));			
			soFloor->SetVelocity(floorShakeAmplitude*omega*cos(time*omega));
		}

		if (bSlider) {
			for (int i = 0; i < GetPHScene()->NSolids(); ++i) {
				PHSolidIf* s = GetPHScene()->GetSolids()[i];
				if (s->IsDynamical()) {
					if (s->GetPose().Pos().y < -1){
						s->SetVelocity(Vec3d());
						s->SetPose(Posed::Trn(Vec3d(-0.15, 1, 0)) );
					}				
				}
			}
		
		}

		//時間表示
		UTPerformanceMeasure* me = GetPHScene()->GetPerformanceMeasure();
		avePool += me->Time("collision");
		me->ClearCounts();
		UTPerformanceMeasure* meC = UTPerformanceMeasure::Get("Collision");
		aveNarrow += meC->Time("narrow");
		aveBroad += meC->Time("broad");
		avePhaseTime[0] += meC->Time("P1");
		avePhaseTime[1] += meC->Time("P2"); 
		avePhaseTime[2] += meC->Time("P3"); 
		meC->ClearCounts();
		aveCounter++;
		if (aveCounter > 0) {
			message = ftos(fps) + "FPS  "
				+ " count" + to_string(aveCounter) 
				+ "  Total:" + ftos(avePool / aveCounter)
				+ "  Broad:" + ftos(aveBroad / aveCounter)
				+ "  Narrow:" + ftos(aveNarrow / aveCounter)
				+ "  Setup:" + ftos(avePhaseTime[0] / aveCounter)
				+ "  2D Search:" + ftos(avePhaseTime[1] / aveCounter)
				+ "  3D Refinement:" + ftos(avePhaseTime[2] / aveCounter);
		}
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		SampleApp::OnDraw(render);

		std::ostringstream sstr;
		sstr << "NObj = " << GetFWScene()->GetPHScene()->NSolids();
		render->DrawFont(Vec2f(-21, 23), sstr.str());
	}
	PHSolidIf* soTable;
	virtual void OnAction(int menu, int id){
		if (menu == MENU_MAIN) {
			Vec3d v, w(0.0, 0.0, 0.0), p(0.0, 0.2, 0.0);
			Quaterniond q = Quaterniond::Rot(Rad(30.0), 'y');
			if (id == ID_MEASURE){
				ofstream ofs("out.txt");
				ofs << "method\t" "total\t" "broad\t" "narrow\t" "p1\t" "p2\t" "p3" << std::endl;
				for (int s = 0; s < 10; s++) {
					OnAction(MENU_STATE, ID_SAVE_STATE);
					for (int c = 0; c < 3; ++c) {
						ClearTime();
						for (int i = 0; i < 10; ++i) {
							OnAction(MENU_STATE, ID_LOAD_STATE);
							OnStep();
						}
						//ofs << message << std::endl;
						ofs << c << "\t" << avePool / aveCounter << "\t";
						ofs << aveBroad / aveCounter << "\t";
						ofs << aveNarrow / aveCounter;
						for (int i = 0; i < 3; ++i) {
							ofs << "\t" << avePhaseTime[i] / aveCounter * 0.001;
						}
						ofs << std::endl;
						if (c == 0) OnAction(MENU_MAIN, ID_METHOD);
						if (c == 1) OnAction(MENU_MAIN, ID_CCD);
						if (c == 2) {
							OnAction(MENU_MAIN, ID_CCD);
							OnAction(MENU_MAIN, ID_METHOD);
						}
					}
					OnAction(MENU_STATE, ID_LOAD_STATE);
					OnStep();
					Display();
				}
				if (menus[MENU_ALWAYS][ID_RUN].enabled) {
					OnAction(MENU_ALWAYS, ID_RUN);
				}
			}
			if (id == ID_GRAVITY) {
				if (GetPHScene()->GetGravity().norm() < 1) {
					GetPHScene()->SetGravity(Vec3d(0, -9.8, 0));
					state = "gravity = -9.8";
				}
				else {
					GetPHScene()->SetGravity(Vec3d(0, -9.8/10, 0));
					state = "gravity = -0.98";
				}
			}
			if (id == ID_ALL) {
				ClearTime();
				sceneCount = 1;
				//	机の足
				PHSolidIf* floor = GetPHScene()->FindObject("soFloor")->Cast();
				CDCapsuleDesc cd;
				cd.length = 0.04;
				cd.radius = 0.006;
				CDCapsuleIf* cp = GetSdk()->GetPHSdk()->CreateShape(cd)->Cast();
				double tower_radius = 0.04;
				const int numbox = 4;
				double theta;
				int sid = floor->NShape();
				for (int j = 0; j < numbox; j++) {
					theta = j * Rad(360) / (double)numbox + Rad(45);
					floor->AddShape(cp);
					floor->SetShapePose(sid, Posed::Rot(-theta, 'y') * Posed::Trn(Vec3d(tower_radius, cd.length / 2, 0)) * Posed::Rot(Rad(90), 'x'));
					sid++;
				}
				//	机
				CDConvexMeshDesc md;
				const int nDiv = 32;
				double height = 0.005;
				for (int i = 0; i < nDiv; ++i) {
					const double r = 0.06;
					const double h = height;
					md.vertices.push_back(Quaterniond::Rot(Rad(360.0 / nDiv * i), 'y')* Vec3d(r, -h / 2, 0));
					md.vertices.push_back(Quaterniond::Rot(Rad(360.0 / nDiv * i), 'y')*Vec3d(r, +h / 2, 0));
				}
				CDConvexMeshIf* table = GetSdk()->GetPHSdk()->CreateShape(md)->Cast();
				soTable = GetPHScene()->CreateSolid();
				soTable->AddShape(table);
				GetFWScene()->SetSolidMaterial(GRRenderIf::LIGHTCYAN, soTable);
				soTable->SetPose(Posed::Trn(0, cd.length + cd.radius + height / 2, 0));
				soTable->CompInertia();
				soTable->SetMass(soTable->GetMass() * 1e7);
				soTable->SetInertia(soTable->GetInertia() * 1e20);
			}
			if (id == ID_FENCEDROP) {
				ClearTime();
				PHSolidIf* floor = GetPHScene()->FindObject("soFloor")->Cast();
				CDCapsuleDesc cd;
				cd.length = 0.1;
				cd.radius = 0.002;
				CDCapsuleIf* cp = GetSdk()->GetPHSdk()->CreateShape(cd)->Cast();
				double tower_radius = 0.082;
				const int numbox = 30;
				double theta;
				int sid = floor->NShape();
				for (int j = 0; j < numbox; j++) {
					theta = j * Rad(360) / (double)numbox;
					floor->AddShape(cp);
					floor->SetShapePose(sid, Posed::Rot(-theta, 'y') * Posed::Trn(Vec3d(tower_radius, cd.length / 2, 0)) * Posed::Rot(Rad(90), 'x'));
					sid++;
				}
				int num = 10;
				for (int i = 0; i < 2; ++i) {
					for (int j = 0; j < num; ++j) {
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)num;
						Drop(SHAPE_CAPSULE, GRRenderIf::GREEN, v, w, Quaterniond::Rot(-theta, 'y') * Vec3d(0.03, 0.1 + 0.02*(i + 0.5), 0), q);
					}
				}
				for (int i = 0; i < 2; ++i) {
					for (int j = 0; j < num; ++j) {
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)num;
						shapeScale *= 0.4;
						Drop(SHAPE_ROCK, GRRenderIf::ORANGE, v, w, Quaterniond::Rot(-theta, 'y') * Vec3d(0.06, 0.1 + 0.04*(i + 0.5), 0), q);
						shapeScale /= 0.4;
					}
				}
				for (int i = 0; i < 2; ++i) {
					for (int j = 0; j < num; ++j) {
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)num;
						Drop(SHAPE_SPHERE, GRRenderIf::YELLOW, v, w, Quaterniond::Rot(-theta, 'y') * Vec3d(0.02, 0.1 + 0.02*(1.6 + i + 0.5), 0), q);
					}
				}
			}
			if (id == ID_TOWER) {
				//				double tower_radius = 0.032;
				double tower_radius = 0.021;
				const int tower_height = 2;
				const int numbox = 10;
				double theta;
				static int start = 0;
				for (int i = start; i < start + tower_height; i++) {
					for (int j = 0; j < numbox; j++) {
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
						Drop(SHAPE_BOX, GRRenderIf::BLUE, Vec3d(), Vec3d(), Quaterniond::Rot(-theta, 'y') * 
							Vec3d(tower_radius, (i + 0.5)*shapeBox->GetBoxSize().y - 0.001, 0), Quaterniond::Rot(-theta, 'y'));
					}
				}
				start += tower_height;
			}
			if (id == ID_BOX) {
				Drop(SHAPE_BOX, GRRenderIf::RED, v, w, p, q);
				state = "box dropped.";
			}
			if (id == ID_CAPSULE) {
				Drop(SHAPE_CAPSULE, GRRenderIf::GREEN, v, w, p, q);
				state = "capsule dropped.";
			}
			if (id == ID_ROUNDCONE) {
				Drop(SHAPE_ROUNDCONE, GRRenderIf::BLUE, v, w, p, q);
				state = "round cone dropped.";
			}
			if (id == ID_SPHERE) {
				Drop(SHAPE_SPHERE, GRRenderIf::YELLOW, v, w, p, q);
				state = "sphere dropped.";
			}
			if (id == ID_ELLIPSOID) {
				Drop(SHAPE_ELLIPSOID, GRRenderIf::LIGHTGREEN, v, w, p, q);
				state = "ellipsoid dropped.";
			}
			if (id == ID_SPHERESHOOT) {
				static int ct = 0;
				Drop(SHAPE_SPHERE, GRRenderIf::YELLOW, Vec3d(-1 - ct*0.2, 0, 0), w, Vec3d(0.3, shapeSphere->GetRadius()*3, 0), q);
				int n = GetPHScene()->NSolids();
				GetPHScene()->GetSolids()[n - 1]->SetMass(GetPHScene()->GetSolids()[n - 1]->GetMass() * 10);
				GetPHScene()->GetSolids()[n - 1]->SetInertia(GetPHScene()->GetSolids()[n - 1]->GetInertia() * 10);
				ct++;
				state = "sphere shooted.";
			}
			if (id == ID_ROCK) {
				Drop(SHAPE_ROCK, GRRenderIf::ORANGE, v, w, p, q);
				state = "random polyhedron dropped.";
			}
			if (id == ID_BLOCK) {
				Drop(SHAPE_BLOCK, GRRenderIf::CYAN, v, w, p, q);
				state = "composite block dropped.";
			}
			if (id == ID_COIN) {
				Drop(SHAPE_COIN, GRRenderIf::GOLD, v, w, p, q);
				state = "coin dropped.";
			}
			if (id == ID_COINSTACK) {
				int tower_height = 2;
				static int start;
				for (int x = 0; x < 2; ++x) {
					for (int i = start; i < start + tower_height; i++) {
						Drop(SHAPE_COIN, i%2 ? GRRenderIf::GOLDENROD : GRRenderIf::SILVER, Vec3d(), Vec3d(), Vec3d(x / 40.0 - 0.05, (i + 0.5)*0.002 - 0.00018*(i + 1), 0), Quaterniond());
					}
				}
				start += tower_height;
				state = "stack coins";
			}
			if (id == ID_SHAKE) {
				std::cout << "F: shake floor." << std::endl;
				if (floorShakeAmplitude.norm() == 0.0) {
					floorShakeAmplitude = Vec3d(0.8,0,0) * 0.01;
					state = "floor shaken.";
				}
				else {
					floorShakeAmplitude = Vec3d();
					state = "floor stopped.";
				}
			}

			if (id == ID_METHOD)
			{
				s_methodSW = s_methodSW ? 0 : 2;
				switch (s_methodSW)
				{
				case 0:
					state = "method : normal";
					break;
				case 1:
					state = "method : accel";
					break;
				case 2:
					state = "method : Gino";
					break;
				case 3:
					state = "method : GJK";
					break;
				default:
					break;
				}
				ClearTime();
			}

			if (id == ID_CCD)
			{
				bool CCDstate = !(GetPHScene()->IsCCDEnabled());
				if (CCDstate) state = "CCD : Enable";
				else state = "CCD : Disable";
				GetPHScene()->EnableCCD(CCDstate);
				ClearTime();
			}
			if (id == ID_CLEARTIME)
			{
				ClearTime();
			}
			if (id == ID_SLIDER)
			{
				static Vec3d eyeTgt, eyePos;
				PHSolidIf* floor = GetPHScene()->FindObject("soFloor")->Cast();
				bSlider = !bSlider;
				if (bSlider) {
					while (floor->NShape() > 1) {
						floor->DelChildObject(floor->GetShape(floor->NShape() - 1));
					}
					floor->AddChildObject(shapeFloor);
					floor->SetShapePose(0, Posed::Rot(Rad(-45), 'z'));
					floor->SetShapePose(1, Posed::Trn(-0.1, -0.17, 0));
					FWWinIf* win = GetCurrentWin();
					eyeTgt = win->GetTrackball()->GetTarget();
					eyePos = win->GetTrackball()->GetPosition();
					Vec3d shift(0.2, 0, 0);
					win->GetTrackball()->SetTarget(Vec3d(0, 0.06, 0) + shift);
					win->GetTrackball()->SetPosition(0.01*Vec3f(0, 40, 50) * 2 + shift); //注視点設定
				}
				else {
					floor->SetOrientation(Quaterniond());
					FWWinIf* win = GetCurrentWin();
					win->GetTrackball()->SetTarget(eyeTgt);
					win->GetTrackball()->SetPosition(eyePos); 
				}
			}
			if (id == ID_ROTATION) {
				CDBoxDesc bd(Vec3d(0.01, 0.04, 0.01));
				CDShapeIf* sh = GetSdk()->GetPHSdk()->CreateShape(bd);
				PHSolidDesc sd;
				sd.pose.Pos() = Vec3d(-0.01, 0.1, 0);
				PHSolidIf * s = GetPHScene()->CreateSolid(sd);
				s->AddShape(sh);
				sd.pose.Pos() = Vec3d(0.01, 0.1, 0);
				sd.velocity = Vec3d(0.01, 0, 0);
				sd.angVelocity = Vec3d(0, 0, 10);
				s = GetPHScene()->CreateSolid(sd);
				s->AddShape(sh);
				GetPHScene()->SetGravity(Vec3d(0, 0, 0));
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


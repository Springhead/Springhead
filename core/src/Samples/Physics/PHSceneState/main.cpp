/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
\page pagePHSceneState PHScene状態の退避/復帰を確認するサンプル 
Springhead2/src/Samples/PHSceneState

\section secQuitPHSceneState 終了基準
- Escか'q'で強制終了。
	
\section secFlowPHSceneState 処理の流れ
- シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。
- 接触エンジンを拘束エンジンに設定する。
- 与えられた条件により⊿t(=0.1)秒後の位置の変化を積分し、OpenGLでシミュレーションする。  
- Solidの発生と状態の退避/復帰は自動的に行なう。
*/

#include "../../SampleApp.h"
#include <map>
#include <string.h>
#include <vector>
#include "Physics/PHContactDetector.h"

#ifdef _WIN32
# include <windows.h>
#else
# include <unistd.h>
# define Sleep(X) usleep(1000*X)
#endif

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

	int		    ctrlCount;
	int		    eventInterval;
	std::vector<double> snap0, snap1, snap2;
	int		    numSnap0, numSnap1, numSnap2;
	bool		printStepMark;

public:
	MyApp(){
		appName = "PHSceneState";
		floorShakeAmplitude = 0.0;

		eventInterval = 50;
		ctrlCount = 0;
		snap0.clear();
		snap1.clear();
		snap2.clear();
		numSnap0 = 0;
		numSnap1 = 0;
		numSnap2 = 0;
		printStepMark = false;

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
	}
	~MyApp(){}

	virtual void BuildScene(){
		soFloor = CreateFloor();
	}

	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void OnStep() {
		generateEvent(ctrlCount);
		++ctrlCount;
		if (printStepMark) {
			std::cout << ".";
			if ((ctrlCount % eventInterval) == 0) std::cout << endl;
		}

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
				//std::cout << "v: " << v << std::endl;
				//std::cout << "w: " << w << std::endl;
				//std::cout << "p: " << p << std::endl;
				//std::cout << "q: " << q << std::endl;
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

#define	EVENT_k_(step, key) \
	if (count == step * eventInterval) { Keyboard(key, 0, 0); }
#define	EVENTa__(step, action) \
	if (count == step * eventInterval) { action }
#define	EVENTak_(step, key, action) \
	if (count == step * eventInterval) { action Keyboard(key, 0, 0); }
#define	EVENT_ka(step, key, action) \
	if (count == step * eventInterval) { Keyboard(key, 0, 0); action }
#define	EVENTaka(step, key, action1, action2) \
	if (count == step * eventInterval) { action1 Keyboard(key, 0, 0); action2 }


	void generateEvent(int count) {
		// show menu
		EVENTak_( 0, 'h', { std::cout << "generating 5 solids" << std::endl; });
		// generate solids
		EVENTak_( 1, 'd', { std::cout << "  solid #1" << std::endl; });
		EVENTak_( 2, 'd', { std::cout << "  solid #2" << std::endl; });
		EVENTak_( 3, 'd', { std::cout << "  solid #3" << std::endl; });
		EVENTak_( 4, 'd', { std::cout << "  solid #4" << std::endl; });
		EVENTak_( 5, 'd', { std::cout << "  solid #5" << std::endl; });
		// save state #0
		EVENT_k_( 6, '\t');
		EVENT_k_( 6, '\t');
		EVENTaka( 6, 'S', { std::cout << "saving states: #0" << std::endl; },
						  { std::cout << "waiting " << eventInterval << " steps ..." << std::endl; });
		EVENTa__( 6,      { printStepMark = true; });
		// snapshot #0
		EVENTa__( 7,	  { numSnap0 = Snapshot(snap0);
							std::cout << "snapshot #0: " << numSnap0 << " states saved" << std::endl; }) 
		// load states #0
		EVENTaka( 8, 'L', { std::cout << "loading states #0" << std::endl; },
						  { std::cout << "states #0 loaded" << std::endl;
							std::cout << "wainting " << eventInterval << " steps ..." << std::endl; });
		// snapshot #1
		EVENTa__( 9,	  { numSnap1 = Snapshot(snap1);
							std::cout << "snapshot #1: " << numSnap1 << " states saved" << std::endl; }) 
		// load states #0
		EVENTaka(11, 'L', { std::cout << "loading states #1" << std::endl; },
						  { std::cout << "states #1 loaded" << std::endl;
							std::cout << "wainting " << eventInterval << " steps ..." << std::endl; });
		// snapshot #2
		EVENTa__(12,	  { numSnap2 = Snapshot(snap2);
							std::cout << "snapshot #2: " << numSnap2 << " states saved" << std::endl; }) 
		// quit
		EVENTa__(13,	  { int rv1 = Compare(snap0, snap1, "0 and 1");
							int rv2 = Compare(snap0, snap2, "0 and 2");
							std::cout << "comparing state 0 and 1 ..." << (rv1 ? " error " : " OK") << std::endl;
							std::cout << "comparing state 0 and 2 ..." << (rv2 ? " error " : " OK") << std::endl;
							int sec = (rv1 + rv2) ? 20 : 5;
							Sleep(sec * 1000); exit(-(rv1 + rv2)); });
	}

	int Snapshot(std::vector<double>& state) {
		PHSceneIf* scene = GetPHScene();
		int nSolids = scene->NSolids();
		PHSolidIf** solids = scene->GetSolids();

		int items = 0;
		for (int i = 0; i < nSolids; ++i) {
			items += SnapshotR((Object*) solids[i], state);
		}
		//std::cout << items << " items saved" << std::endl;
		std::cout << "vector size: " << state.size() << std::endl;
		return items;
	}
	int SnapshotR(Object* obj, std::vector<double>& state) {
		int items = 0;
		if (DCAST(PHSolidIf, obj)) {
			Vec3d v = ((PHSolidIf*) obj)->GetVelocity();
			Vec3d a = ((PHSolidIf*) obj)->GetAngularVelocity();
			Posed p = ((PHSolidIf*) obj)->GetPose();
			std::cout << "velo: " << v << std::endl;
			std::cout << "angv: " << a << std::endl;
			std::cout << "pose: " << p << std::endl;
			state.push_back(v.x);
			state.push_back(v.y);
			state.push_back(v.z);
			state.push_back(a.x);
			state.push_back(a.y);
			state.push_back(a.z);
			state.push_back(p.w);
			state.push_back(p.x);
			state.push_back(p.y);
			state.push_back(p.z);
			++items;
		}
		size_t n = obj->NChildObject();
		for (size_t i = 0; i < n; ++i) {
			Object* child = (Object*) obj->GetChildObject(i);
			items += SnapshotR(child, state);
		}
		//std::cout << "SnapshotR: # of items: " << items << std::endl;
		return items;
	}
	int Compare(std::vector<double>& st1, std::vector<double>& st2, const char* id) {
		size_t sz1 = st1.size();
		size_t sz2 = st2.size();
		if (sz1 != sz2) {
			std::cout << "Size of states " << id << " differs: " << sz1 << " and " << sz2 << std::endl;
			return 1;
		}
		if (st1 != st2) {
			std::cout << "States " << id << " differs:" << std::endl;
			for (size_t i = 0; i < sz1; ++i) {
				if (st1[i] != st2[i]) {
					std::cout << "  (" << i << ") " << st1[i] << ", " << st2[i] << std::endl;
				}
			}
			return 1;
		}
		return 0;
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


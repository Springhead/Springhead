#include "PhysicsProcess.h"
#include "HapticProcess.h"
#include <GL/glut.h>
#include <iostream>
#include <time.h>
#include "Foundation/UTMMTimer.h"
#include <Physics/PHConstraintEngine.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#define ESC 27

using namespace std;

UTMMTimer timer;

void CallBackHapticProcess(void* arg);
void Synchronize();
void _cdecl Keyboard(unsigned char key, int x, int y);


void CallBackHapticProcess(void* arg){
	if(hprocess.stepcount > 300) return;
	hprocess.Step();
	Synchronize();
};

void StartTimer(){
	timer.Resolution(1);
	timer.Interval(1);
	timer.Set(CallBackHapticProcess, NULL);
	timer.Create();
};

void Synchronize(){
	if(pprocess.bsync){
		// 力覚ポインタの同期
		pprocess.phpointer = hprocess.hpointer;
		pprocess.UpdateHapticPointer();

		// 力覚プロセス->物理プロセス
		// 力覚プロセスでの近傍物体のシミュレーション結果を物理プロセスに反映させる
		for(unsigned i = 0; i < hprocess.expandedObjects.size(); i++){
		// blocalがtrue，blocalがな場合は結果を反映させる
			if(pprocess.expandedObjects[i].flag.blocal && !pprocess.expandedObjects[i].flag.bfirstlocal){
				SpatialVector b = (pprocess.expandedObjects[i].syncInfo.motionCoeff.b + 
					(pprocess.expandedObjects[i].syncInfo.motionCoeff.curb - pprocess.expandedObjects[i].syncInfo.motionCoeff.lastb)) *  pprocess.dt;
				Vec3d v = hprocess.expandedObjects[i].phSolid.GetVelocity() + b.v();
				hprocess.expandedObjects[i].phSolidIf->SetVelocity(v);
				Vec3d w = hprocess.expandedObjects[i].phSolid.GetAngularVelocity() + b.w();
				hprocess.expandedObjects[i].phSolidIf->SetAngularVelocity(w);
				hprocess.expandedObjects[i].phSolidIf->SetCenterPosition(hprocess.expandedObjects[i].phSolid.GetCenterPosition());
				hprocess.expandedObjects[i].phSolidIf->SetOrientation(hprocess.expandedObjects[i].phSolid.GetOrientation());
				pprocess.expandedObjects[i].syncInfo.neighborPoint.test_force_norm = hprocess.expandedObjects[i].syncInfo.neighborPoint.test_force_norm;
			}
		}

		// 物理プロセス->力覚プロセス
		// シーンで新しく生成された剛体を格納
		for(size_t i = hprocess.expandedObjects.size(); i < pprocess.expandedObjects.size(); i++){
			hprocess.expandedObjects.resize(hprocess.expandedObjects.size() + 1);
			hprocess.expandedObjects.back() = pprocess.expandedObjects[i];
		}
		for(unsigned i = 0; i < hprocess.expandedObjects.size(); i++){
			// 初めて取得した近傍物体のみ行う
			if(pprocess.expandedObjects[i].flag.bfirstlocal){
				hprocess.expandedObjects[i].phSolid = pprocess.expandedObjects[i].phSolid;
				pprocess.expandedObjects[i].flag.bfirstlocal = false;
			}
			hprocess.expandedObjects[i].syncInfo = pprocess.expandedObjects[i].syncInfo;
		}

		// 物理プロセスで使用する刻み時間
		//pprocess.phscene->SetTimeStep(0.001f * (float)stepcount);
		//pprocess.dt = 0.001f * (float)stepcount;
		hprocess.stepcount = 0;

		// 同期終了のフラグ
		pprocess.bsync = false;
	}
	if(hprocess.stepcount > hprocess.countmax) {
		DSTR << "too many step" << endl;
		hprocess.stepcount = 0;
		pprocess.bsync = false;
	}
	hprocess.stepcount++;
	pprocess.hapticcount++;
};

void Reset(){
	timer.Release();
	// 自分で作ったvectorを初期化
	pprocess.sceneSolids.clear();
	pprocess.expandedObjects.clear();
	hprocess.expandedObjects.clear();
	pprocess.bsync = false;
	pprocess.calcPhys=true;
	pprocess.hapticcount = 1;
	hprocess.stepcount = 1;

	// sceneのロード
	pprocess.GetSdk()->Clear();															// SDKの作成
	pprocess.GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	PHSceneIf* phscene = pprocess.GetSdk()->GetScene()->GetPHScene(); 
	pprocess.states = ObjectStatesIf::Create();
	pprocess.states2 = ObjectStatesIf::Create();
	pprocess.DesignObject();																// 剛体を作成
	phscene->SetGravity(pprocess.gravity);				
	phscene->SetTimeStep(pprocess.dt);
	phscene->SetNumIteration(pprocess.nIter);
	pprocess.GetCurrentWin()->SetScene(pprocess.GetSdk()->GetScene());
	// pprocess.GetCurrentWin()->SetRender(pprocess.GetSdk()->GetRender());
	pprocess.InitCameraView();
	timer.Create();

	DSTR << "Reset" << endl;
}

void _cdecl Keyboard(unsigned char key, int x, int y){
		switch (key) {
		case ESC:		
		case 'q':
			timer.Release();
			exit(0);
			break;
		case 'r':
			Reset();
			pprocess.Keyboard('x');
			break;
		default:
			pprocess.Keyboard(key);
			hprocess.Keyboard(key);
			break;
	}
};

int _cdecl main(int argc, char* argv[]){
	Sleep(100);
	hprocess.Init();
	pprocess.Init(argc, argv);
	StartTimer();
	glutKeyboardFunc(Keyboard);
	pprocess.Start();
	return 0;
}
#include "BoxStack.h"
#include "HapticProcess.h"
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include "Foundation/UTMMTimer.h"
#include <Physics/PHConstraintEngine.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#define ESC 27

using namespace std;

UTMMTimer timer;
bool bhaptic = true;

void AppStart();
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
	if(bstack.bsync){
		// 力覚ポインタの同期
		bstack.phpointer = hprocess.hpointer;
		bstack.UpdateHapticPointer();

		// 力覚プロセス->物理プロセス
		// 力覚プロセスでの近傍物体のシミュレーション結果を物理プロセスに反映させる
		bstack.hapticsolids.clear();
		for(unsigned i = 0; i < hprocess.neighborObjects.size(); i++){
		// blocalがtrue，blocalがな場合は結果を反映させる
			if(bhaptic){
				if(bstack.neighborObjects[i].blocal && !bstack.neighborObjects[i].bfirstlocal){
					bstack.hapticsolids.push_back(hprocess.neighborObjects[i].phSolid);
					SpatialVector b = (bstack.neighborObjects[i].b + (bstack.neighborObjects[i].curb - bstack.neighborObjects[i].lastb)) *  bstack.dt;
					Vec3d v = hprocess.neighborObjects[i].phSolid.GetVelocity() + b.v();
					hprocess.neighborObjects[i].phSolidIf->SetVelocity(v);
					Vec3d w = hprocess.neighborObjects[i].phSolid.GetAngularVelocity() + b.w();
					hprocess.neighborObjects[i].phSolidIf->SetAngularVelocity(w);
					hprocess.neighborObjects[i].phSolidIf->SetCenterPosition(hprocess.neighborObjects[i].phSolid.GetCenterPosition());
					hprocess.neighborObjects[i].phSolidIf->SetOrientation(hprocess.neighborObjects[i].phSolid.GetOrientation());
					bstack.neighborObjects[i].test_force_norm = hprocess.neighborObjects[i].test_force_norm;
				}
			}
		}

		// 物理プロセス->力覚プロセス
		// シーンで新しく生成された剛体を格納
		for(size_t i = hprocess.neighborObjects.size(); i < bstack.neighborObjects.size(); i++){
			hprocess.neighborObjects.resize(hprocess.neighborObjects.size() + 1);
			hprocess.neighborObjects.back() = bstack.neighborObjects[i];
		}
		for(unsigned i = 0; i < hprocess.neighborObjects.size(); i++){
			// 初めて取得した近傍物体のみ行う
			if(bstack.neighborObjects[i].bfirstlocal){
				hprocess.neighborObjects[i].phSolid = bstack.neighborObjects[i].phSolid;
				bstack.neighborObjects[i].bfirstlocal = false;
			}
			hprocess.neighborObjects[i].closestPoint = bstack.neighborObjects[i].closestPoint;
			hprocess.neighborObjects[i].pointerPoint = bstack.neighborObjects[i].pointerPoint;
			hprocess.neighborObjects[i].face_normal = bstack.neighborObjects[i].face_normal;
			hprocess.neighborObjects[i].last_face_normal = bstack.neighborObjects[i].last_face_normal;
			hprocess.neighborObjects[i].blocal = bstack.neighborObjects[i].blocal;
			hprocess.neighborObjects[i].A = bstack.neighborObjects[i].A;
			hprocess.neighborObjects[i].b = bstack.neighborObjects[i].b;
			hprocess.neighborObjects[i].curb = bstack.neighborObjects[i].curb;
			hprocess.neighborObjects[i].lastb = bstack.neighborObjects[i].lastb;
		}

		// 物理プロセスで使用する刻み時間
		//bstack.phscene->SetTimeStep(0.001f * (float)stepcount);
		//bstack.dt = 0.001f * (float)stepcount;
		hprocess.stepcount = 0;

		// 同期終了のフラグ
		bstack.bsync = false;
	}
	if(hprocess.stepcount > hprocess.countmax) {
		DSTR << "Can't Synchronaize with HapticProcess!!" << endl;
		hprocess.stepcount = 0;
		bstack.bsync = false;
	}
	hprocess.stepcount++;
	bstack.hapticcount++;
};

void Reset(){
	timer.Release();
	// 自分で作ったvectorを初期化
	bstack.soBox.clear();
	bstack.sceneSolids.clear();
	bstack.neighborObjects.clear();
	hprocess.neighborObjects.clear();
	bstack.bsync = false;
	bstack.calcPhys=true;
	bstack.hapticcount = 1;
	hprocess.stepcount = 1;

	// sceneのロード
	bstack.GetSdk()->Clear();															// SDKの作成
	stringstream imputFile;
	imputFile << bstack.filePath << bstack.fileName;
//	DSTR << imputFile << endl;
	bstack.GetSdk()->LoadScene(imputFile.str().c_str());
	bstack.phscene = bstack.GetSdk()->GetScene()->GetPHScene();
	
	//bstack.GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	//bstack.phscene = bstack.GetSdk()->GetScene()->GetPHScene();
	bstack.states = ObjectStatesIf::Create();
	bstack.states2 = ObjectStatesIf::Create();

	bstack.phscene->SetGravity(bstack.gravity);				
	bstack.phscene->SetTimeStep(bstack.dt);
	bstack.phscene->SetNumIteration(bstack.nIter);

	bstack.SetHapticPointer();
	//剛体の材質の設定
	bstack.DesignObject();

	bstack.GetCurrentWin()->SetScene(bstack.GetSdk()->GetScene());
	// bstack.GetCurrentWin()->SetRender(bstack.GetSdk()->GetRender());
	bstack.InitCameraView();
	timer.Create();
	DSTR << "Reset" << endl;
}

void _cdecl Keyboard(unsigned char key, int x, int y){
		switch (key) {
		case ESC:		
		case 'q':
			timer.Release();
			exit(0);
			glutLeaveGameMode();
			break;
		case 'r':
			Reset();
			break;
		case 'h':
			if(bhaptic){
				bhaptic = false;
				DSTR << "Haptic Disconnect" << endl;
			}else{
				bhaptic = true;
				DSTR << "Haptic Connect" << endl;
			}
			break;
		default:
			bstack.Keyboard(key);
			hprocess.Keyboard(key);
			break;
	}
};

int _cdecl main(int argc, char* argv[]){
	Sleep(100);
	hprocess.Init();
	bstack.Init(argc, argv);
	StartTimer();
	glutKeyboardFunc(Keyboard);
	bstack.Start();
	
	return 0;
}
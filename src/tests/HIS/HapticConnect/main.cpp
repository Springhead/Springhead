#include "BoxStack.h"
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

BoxStack bstack;
HapticProcess hprocess;	
UTMMTimer timer;
bool bsync = false;
bool bhaptic = true;
void _cdecl CallBackPhysiclaProcess();
void AppStart();
void CallBackHapticProcess(void* arg);
void Synchronize();
void _cdecl Keyboard(unsigned char key, int x, int y);

void _cdecl CallBackPhysiclaProcess(){
	while(bsync){
		Sleep(1);
	}
	bstack.Display();
	bsync = true;
};

void CallBackHapticProcess(void* arg){
	hprocess.Step();
	Synchronize();
};

void StartTimer(){
	timer.Resolution(1);
	timer.Interval(1);
	timer.Set(CallBackHapticProcess, NULL);
	timer.Create();
};

void AppStart(){
	bstack.Start();
	glutDisplayFunc(CallBackPhysiclaProcess);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
};

void Synchronize(){
	cout << hprocess.stepcount << endl;
	if(bsync){
		//static int count;
		//count++;
		//DSTR << "count" << count << endl;
		//DSTR << bstack.phscene->GetCount() << endl;
		// 力覚ポインタの同期
		bstack.phpointer = hprocess.hpointer;
		bstack.UpdateHapticPointer();

		// 力覚プロセス->物理プロセス
		// 力覚プロセスでの近傍物体のシミュレーション結果を物理プロセスに反映させる
		for(unsigned i = 0; i < hprocess.neighborObjects.size(); i++){
			// blocalがtrueな場合は結果を反映させる
		PHConstraintEngine* engine = bstack.phscene->GetConstraintEngine()->Cast();
		//if(engine->solidPairs.width() > 2){
		//	DSTR << engine->solidPairs.item(0, 2)->shapePairs.item(0,0)->state << ":::"
		//		<< engine->solidPairs.item(0, 2)->solid[1]->GetCenterPosition() << ":::"
		//		<< engine->solidPairs.item(0, 2)->solid[1]->GetVelocity() << ":::"
		//		<<engine->solidPairs.item(0, 2)->shapePairs.item(0,0)->normal << endl;
		//}
			if(bhaptic){
 				if(hprocess.neighborObjects[i].blocal){
					if(bstack.neighborObjects[i].phSolidIf != bstack.soFloor){
						DSTR << "vphysic" << hprocess.neighborObjects[i].phSolidIf->GetVelocity() << "::"<< hprocess.neighborObjects[i].phSolidIf->GetCenterPosition() << endl;
						DSTR << "vhaptic" << hprocess.neighborObjects[i].phSolid.GetVelocity() << "::"<< hprocess.neighborObjects[i].phSolid.GetCenterPosition() << endl;
					}
					hprocess.neighborObjects[i].phSolidIf->SetVelocity(hprocess.neighborObjects[i].phSolid.GetVelocity());// + bstack.neighborObjects[i].b.v() * bstack.dt);
//					hprocess.neighborObjects[i].phSolidIf->SetAngularVelocity(hprocess.neighborObjects[i].phSolid.GetAngularVelocity());
					hprocess.neighborObjects[i].phSolidIf->SetCenterPosition(hprocess.neighborObjects[i].phSolid.GetCenterPosition());
//					hprocess.neighborObjects[i].phSolidIf->SetOrientation(hprocess.neighborObjects[i].phSolid.GetOrientation());	
//					DSTR << "sync" << hprocess.neighborObjects[i].phSolidIf->GetVelocity() << endl;
					//					cout << "----------"<< endl;
					//cout << "scene" << bstack.neighborObjects[i].phSolidIf->GetVelocity() << endl;
//					if(hprocess.neighborObjects.size() > 1)DSTR <<  "local" << hprocess.neighborObjects[1].phSolid.GetVelocity() << endl;
				}
			}
		}

		// 物理プロセス->力覚プロセス
		// シーンで新しく生成された剛体を格納
		for(unsigned i = hprocess.neighborObjects.size(); i < bstack.neighborObjects.size(); i++){
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
			hprocess.neighborObjects[i].direction = bstack.neighborObjects[i].direction;
			hprocess.neighborObjects[i].blocal = bstack.neighborObjects[i].blocal;
			hprocess.neighborObjects[i].A = bstack.neighborObjects[i].A;
			hprocess.neighborObjects[i].b = bstack.neighborObjects[i].b;
		}

		// 物理プロセスで使用する刻み時間
		//bstack.phscene->SetTimeStep(0.001f * (float)stepcount);
		//bstack.dt = 0.001f * (float)stepcount;
		hprocess.stepcount = 0;

		// 同期終了のフラグ
		bsync = false;
	}
	hprocess.stepcount++;
};

void _cdecl Keyboard(unsigned char key, int x, int y){
		switch (key) {
		case ESC:		
		case 'q':
			timer.Release();
			exit(0);
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
		case 'm':
//			DSTR << "Esc, q" << 
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
	AppStart();
	return 0;
}
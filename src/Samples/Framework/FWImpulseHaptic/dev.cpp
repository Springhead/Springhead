#include "dev.h"	
#include <Physics\PHScene.h>
#include <Physics\PHSolid.h>
#include <Physics\PHHapticEngine.h>
#include <Physics\PHPenaltyEngine.h>
#include <Physics\PHConstraintEngine.h>


void MyApp::Init(int argc, char* argv[]){
		FWApp::Init(argc, argv);
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		phscene = GetSdk()->GetScene()->GetPHScene();
		phscene->SetTimeStep(0.05);
		CDBoxDesc bd;
		
		// 床を作成
		PHSolidIf* floor = phscene->CreateSolid();
		floor->SetDynamical(false);
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -0.5, 0.0));
	
		// 箱を作成
		for(int i = 0; i < 10; i++){
			PHSolidIf* box = phscene->CreateSolid();
			box->SetMass(1.0);
			bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
			box->AddShape(phSdk->CreateShape(bd));
			box->SetInertia(box->GetShape(0)->CalcMomentOfInertia() * box->GetMass());
			box->SetFramePosition(Vec3d(0.0, 1.0 * i, 0));
		}

		PHHapticPointerIf* box = phscene->CreateHapticPointer();
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		box->AddShape(phSdk->CreateShape(bd));
		box->SetFramePosition(Vec3d(0.0, 0.2f, 0.0));
		box->SetDynamical(false);
		pointer = box;
		range = 0.5;
		GetSdk()->SetDebugMode(true);
		PHHapticPointer* b = box->Cast();
		b->SetLocalRange(1);

		//box = phscene->CreateHapticPointer();
		//box->AddShape(phSdk->CreateShape(bd));

		DSTR << phscene << std::endl;
		PHHapticEngine* h = phscene->GetHapticEngine()->Cast();
		h->SetRenderMode(PHHapticEngine::IMPULSE);
		h->EnableHaptic(true);

		UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);
		timer->SetResolution(1);					// 分解能(ms)
		timer->SetInterval(1);	// 刻み(ms)
		timerID = timer->GetID();
		timer->Start();		// タイマスタート

}

void MyApp::TimerFunc(int id){
	if(timerID == id){
		phscene->StepHapticLoop();
	}else{
		//UserFunc();
		PHHapticEngine* h = GetCurrentWin()->GetScene()->GetPHScene()->GetHapticEngine()->Cast();
		h->StepSimulation();

		PostRedisplay();
		//DSTR << "Step Scene" << std::endl;
	}
}


void MyApp::UserFunc(){
	PHScene* p = phscene->Cast();
	PHHapticEngine* h = p->GetHapticEngine()->Cast();
	DSTR << h->renderImps.size() << std::endl;

}

void MyApp::ContactAnalysis(){

}

void MyApp::Keyboard(int key, int x, int y){
	double distance = 0.05;
	switch(key){
		case 'q':
			exit(0);
			break;
		//case 'a':
		//	range += 0.5;
		//	break;
		//case 's':
		//	range -= 0.5;
		//	break;
		case 'a':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(-distance, 0.0, 0.0));
			}
			break;
		case 's':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(distance, 0.0, 0.0));
			}
			break;
		case 'w':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(0.0, distance, 0.0));
			}
			break;
		case 'z':
			{
				Vec3d pos = pointer->GetFramePosition();
				pointer->SetFramePosition(pos + Vec3d(0.0, -distance, 0.0));
			}
			break;	
		case ' ':
			{
				
			}
		default:
			break;
	}

}
#include "dev.h"	
#include <Physics\PHScene.h>
#include <Physics\PHSolid.h>
#include <Physics\PHHapticEngine.h>
#include <Physics\PHPenaltyEngine.h>
#include <Physics\PHConstraintEngine.h>
#include <Windows.h>

using namespace Spr;

#define SPIDAR 0
// 0:single, 1:impulsemulti, 2:LD
#define ENGINETYPE 2
#define DEBUG_CON 0
#define DEBUG_RENDER 1

void MyApp::InitInterface(){
	hiSdk = HISdkIf::CreateSdk();

#if SPIDAR
	DRUsb20SimpleDesc usbSimpleDesc;
	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i< 10; ++i){
		usb20Sh4Desc.channel = i;
		hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	DRCyUsb20Sh4Desc cyDesc;
	for(int i=0; i<10; ++i){
		cyDesc.channel = i;
		hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
	}
	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);

	spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidarGDesc("SpidarG6X3R"));
	spg->Calibration();
#else
	spg = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	HIXbox360ControllerIf* con = DCAST(HIXbox360ControllerIf,spg);
	con->Init();
#endif
}


void MyApp::Init(int argc, char* argv[]){
		FWApp::Init(argc, argv);
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		HISdkIf* hiSdk = GetSdk()->GetHISdk();
		InitInterface();
		phscene = GetSdk()->GetScene()->GetPHScene();
		phscene->SetTimeStep(0.05);
		
		CDBoxDesc bd;
		
		// 床を作成
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		bd.material.mu= 0.5;
		bd.material.mu0 = 0.5;
		bd.material.e = 0.0;
		PHSolidIf* floor = phscene->CreateSolid();
		floor->SetDynamical(false);
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, 0.0));
	
		// 箱を作成
		for(int i = 0; i < 1; i++){
			PHSolidIf* box = phscene->CreateSolid();
			sobox = box;
			box->SetMass(0.1);
			bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
			//bd.boxsize = Vec3f(2, 0.2, 2);
			box->AddShape(phSdk->CreateShape(bd));
			box->SetInertia(box->GetShape(0)->CalcMomentOfInertia() * box->GetMass());
			box->SetFramePosition(Vec3d(-0.5, 0.0, 0.0));
		}

		pointer = phscene->CreateHapticPointer();
		CDSphereDesc cd;
		cd.radius = 0.1f;
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		pointer->AddShape(phSdk->CreateShape(bd));
		pointer->SetFramePosition(Vec3d(0.0, 0.0, 0.0));
		pointer->SetDynamical(false);
		pointer->SetIntegrate(false);
		pointer->SetHumanInterface(spg);
		pointer->SetInertia(pointer->GetInertia());
		phscene->SetContactMode(pointer, PHSceneDesc::MODE_NONE);
		PHHapticPointer* b = pointer->Cast();
		b->SetLocalRange(0.5);
		b->SetPosScale(50);
		b->bDebugControl = DEBUG_CON;

		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderHaptic(DEBUG_RENDER);

		PHHapticEngine* h = phscene->GetHapticEngine()->Cast();
		h->EnableHaptic(true);
#if ENGINETYPE == 0
		h->SetHapticEngineType(PHHapticEngine::SINGLE_THREAD);
		phscene->SetTimeStep(0.001);
#elif ENGINETYPE == 1
		h->SetHapticEngineType(PHHapticEngine::MULTI_THREAD);
#elif ENGINETYPE == 2
		h->SetHapticEngineType(PHHapticEngine::LOCAL_DYNAMICS);
#endif
		physicsTimerID = GetTimer(0)->GetID();
		GetTimer(0)->SetMode(UTTimerIf::IDLE);
		timer = CreateTimer(UTTimerIf::MULTIMEDIA);
		timer->SetResolution(1);					// 分解能(ms)
		timer->SetInterval(1);	// 刻み(ms)h
		hapticTimerID = timer->GetID();
		timer->Start();		// タイマスタート
}

#if ENGINETYPE > 0
void MyApp::TimerFunc(int id){
	if(hapticTimerID == id){
		phscene->StepHapticLoop();
	}else{
		//CSVOUT << sobox->GetFramePosition().y << std::endl;
		PHHapticEngine* he = phscene->GetHapticEngine()->Cast();
		he->StepSimulation();
		PostRedisplay();
	}
}
#else
void MyApp::TimerFunc(int id){
	if(hapticTimerID == id){
		phscene->Step();
	}else if(physicsTimerID == id){
		PostRedisplay();
	}
}
#endif

void MyApp::UserFunc(){


}


void MyApp::Keyboard(int key, int x, int y){
	double distance = 0.05;
	switch(key){
		case 'q':
			for(int i = 0; i < NTimers(); i++){
				GetTimer(i)->Stop();
			}
			Sleep(1000);
			exit(0);
			break;
		//case 'a':
		//	range += 0.5;
		//	break;
		//case 's':
		//	range -= 0.5;
		//	break;
		case 'c':
			{
				timer->Stop();
				spg->Calibration();
				Affinef aff;
				GetCurrentWin()->GetRender()->GetViewMatrix(aff);
				DSTR << "ViewMatrix" << std::endl;
				DSTR << aff << std::endl;
				timer->Start();
			}
			break;
		case 'f':
			{
				pointer->EnableForce(true);
			}
			break;
		case 'g':
			{
				pointer->EnableForce(false);
			}
			break;
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
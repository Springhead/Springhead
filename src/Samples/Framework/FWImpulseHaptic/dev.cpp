#include "dev.h"	
#include <Physics\PHScene.h>
#include <Physics\PHSolid.h>
#include <Physics\PHHapticEngine.h>
#include <Physics\PHPenaltyEngine.h>
#include <Physics\PHConstraintEngine.h>
#include <Windows.h>

#include <Physics\PHHapticEngineLD.h>

#define SUSA_DEV

using namespace Spr;

#define SPIDAR 1
// 0:single, 1:impulsemulti, 2:LD
#define ENGINETYPE 1
#define DEBUG_CON 0
#define DEBUG_RENDER 0
#define DIRECT_CON 1

void MyApp::InitInterface(){
	hiSdk = GetSdk()->GetHISdk();

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
	spg2 = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	spg2->Init(&HISpidarGDesc("SpidarG6X3L"));
	spg2->Calibration();
#else
	spg = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	HIXbox360ControllerIf* con = DCAST(HIXbox360ControllerIf,spg);
	spg2 = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	HIXbox360ControllerIf* con2 = DCAST(HIXbox360ControllerIf, spg2);
#endif

}


void MyApp::Init(int argc, char* argv[]){
		FWApp::Init(argc, argv);
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		InitInterface();
		phscene = GetSdk()->GetScene()->GetPHScene();
		phscene->SetTimeStep(0.05);
		
		Vec3d pos = Vec3d(0, 0, 1.21825);
		GetCurrentWin()->GetTrackball()->SetPosition(pos);

		CDBoxDesc bd;
		
		// 床を作成
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		bd.material.mu= 0.0;//0.7;
		bd.material.mu0 = 0.0;//0.7;
		bd.material.e = 0.1;
		PHSolidIf* floor = phscene->CreateSolid();
		floor->SetDynamical(false);
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, 0.0));
	
		// 箱を作成
		for(int i = 0; i < 2; i++){
			PHSolidIf* box = phscene->CreateSolid();
			box->SetMass(0.1 * 10);
			float size = 0.3f;
			bd.boxsize.clear(size);
			box->AddShape(phSdk->CreateShape(bd));
			box->SetInertia(box->GetShape(0)->CalcMomentOfInertia() * box->GetMass());
			box->SetFramePosition(Vec3d(-0.5 - size * i, -0.3, 0.0));
			//box->SetDynamical(false);
		}

		pointer = phscene->CreateHapticPointer();
		CDSphereDesc cd;
		cd.radius = 0.1f;
		cd.material.mu = 0.0;
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		pointer->AddShape(phSdk->CreateShape(cd));
		Posed dpose;
		dpose.Pos() = Vec3d(0.0, -0.35, 0.0);
		pointer->SetDefaultPose(dpose);
		pointer->SetHumanInterface(spg);
		pointer->SetInertia(pointer->GetInertia());
		pointer->SetLocalRange(0.1);
		pointer->SetPosScale(50);
		pointer->SetReflexSpring(5000);
		pointer->SetReflexDamper(0.1);
		pointer->EnableDebugControl(DEBUG_CON);
		pointer->EnableDirectControl(DIRECT_CON);
		pointer->EnableFriction(false);
		//pointer->EnableVibration(true);
		//phscene->SetContactMode(pointer, PHSceneDesc::MODE_NONE);

		//pointer2 = phscene->CreateHapticPointer();
		//pointer2->AddShape(phSdk->CreateShape(bd));
		//pointer2->SetDefaultPose(Posed());
		//pointer2->SetHumanInterface(spg2);
		//pointer2->SetInertia(pointer->GetInertia());
		//pointer2->SetLocalRange(0.5);
		//pointer2->SetPosScale(50);
		//pointer2->SetReflexSpring(5000);
		//pointer2->SetReflexDamper(0.1);
		//pointer2->EnableFriction(false);
		//pointer2->EnableVibration(true);

		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderHaptic(DEBUG_RENDER);

		PHHapticEngineIf* h = phscene->GetHapticEngine();
		h->EnableHapticEngine(true);
		//render->EnableMultiPoints(false);
#if ENGINETYPE == 0
		h->SetHapticEngineMode(PHHapticEngine::SINGLE_THREAD);
		phscene->SetTimeStep(0.001);
#elif ENGINETYPE == 1
		h->SetHapticEngineMode(PHHapticEngine::MULTI_THREAD);
		phscene->SetTimeStep(0.02);
#elif ENGINETYPE == 2
		h->SetHapticEngineMode(PHHapticEngine::LOCAL_DYNAMICS);
		phscene->SetTimeStep(0.02);
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

			//{
			//	PHHapticEngine*   h  = phscene->GetHapticEngine()->Cast();
			//	if (h) {
			//		PHHapticEngineLD* hi = h->engineImp->Cast();
			//		if (hi) {
			//			hi->states->ReleaseState(phscene);
			//		}
			//	}
			//}

			exit(0);
			break;
		case '1':
			{
				pointer->SetHapticRenderMode(PHHapticPointerDesc::PENALTY);
				DSTR << "Penalty mode" << std::endl;
				break;
			}
		case '2':
			{
				pointer->SetHapticRenderMode(PHHapticPointerDesc::CONSTRAINT);
				DSTR << "Constraint mode" << std::endl;
				break;
			}
		case 'c':
			{
				timer->Stop();
				spg->Calibration();
				spg2->Calibration();
				DSTR << "CameraPosition" << std::endl;
				DSTR << GetCurrentWin()->GetTrackball()->GetPosition() << std::endl;
				timer->Start();
			}
			break;
		case 'f':
			{
				if(pointer) pointer->EnableForce(true);
				if(pointer2) pointer2->EnableForce(true);
			}
			break;
		case 'g':
			{
				if(pointer)pointer->EnableForce(false);
				if(pointer2)pointer2->EnableForce(false);
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
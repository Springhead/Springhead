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
	hiSdk = GetSdk()->GetHISdk();
	//hiSdk = HISdkIf::CreateSdk();
	//hiSdk->Print(DSTR);

#if SPIDAR
	DRUsb20SimpleDesc usbSimpleDesc;
	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	//DRUsb20Sh4Desc usb20Sh4Desc;
	//for(int i=0; i< 10; ++i){
	//	usb20Sh4Desc.channel = i;
	//	hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	//}
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
	spg2 = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	HIXbox360ControllerIf* con2 = DCAST(HIXbox360ControllerIf, spg2);
	con2->Init();
}


void MyApp::Init(int argc, char* argv[]){
		FWApp::Init(argc, argv);
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		InitInterface();
		phscene = GetSdk()->GetScene()->GetPHScene();
		phscene->SetTimeStep(0.05);
		
		//GetCurrentWin()->GetTrackball()->SetMode(true);
		Vec3d pos = Vec3d(0, 0, 1.21825);
		GetCurrentWin()->GetTrackball()->SetPosition(pos);

		CDBoxDesc bd;
		
		// 床を作成
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		bd.material.mu= 0.7;
		bd.material.mu0 = 0.7;
		bd.material.e = 0.1;
		PHSolidIf* floor = phscene->CreateSolid();
		floor->SetDynamical(false);
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, 0.0));
	
		// 箱を作成
		for(int i = 0; i < 1; i++){
			PHSolidIf* box = phscene->CreateSolid();
			sobox = box;
			box->SetMass(0.5);
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
		pointer->AddShape(phSdk->CreateShape(cd));
		pointer->SetDefaultPose(Posed());
		pointer->SetHumanInterface(spg);
		pointer->SetInertia(pointer->GetInertia());
		pointer->SetLocalRange(0.5);
		pointer->SetPosScale(50);
		pointer->SetReflexSpring(5000);
		pointer->SetReflexDamper(0.1);
		pointer->EnableDebugControl(DEBUG_CON);
		pointer->EnableFriction(true);
		pointer->EnableVibration(true);
		phscene->SetContactMode(pointer, PHSceneDesc::MODE_NONE);

		pointer2 = phscene->CreateHapticPointer();
		pointer2->AddShape(phSdk->CreateShape(cd));
		pointer2->SetDefaultPose(Posed());
		pointer2->SetHumanInterface(spg2);
		pointer2->SetInertia(pointer->GetInertia());
		pointer2->SetLocalRange(0.5);
		pointer2->SetPosScale(50);
		pointer2->SetReflexSpring(5000);
		pointer2->SetReflexDamper(0.1);
		pointer2->EnableFriction(true);
		pointer2->EnableVibration(true);
		phscene->SetContactMode(pointer2, PHSceneDesc::MODE_NONE);

		GetSdk()->SetDebugMode(true);
		GetSdk()->GetScene()->EnableRenderHaptic(DEBUG_RENDER);

		PHHapticEngineIf* h = phscene->GetHapticEngine();
		h->EnableHapticEngine(true);
		PHHapticRenderIf* render = h->GetHapticRender();
		render->SetHapticRenderMode(PHHapticRenderDesc::CONSTRAINT);
		//render->SetHapticRenderMode(PHHapticRenderDesc::PENALTY);
		render->EnableMultiPoints(false);
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
			exit(0);
			break;
		case '1':
			{
				phscene->GetHapticEngine()->GetHapticRender()->SetHapticRenderMode(PHHapticRenderDesc::PENALTY);
				DSTR << "Penalty mode" << std::endl;
				break;
			}
		case '2':
			{
				phscene->GetHapticEngine()->GetHapticRender()->SetHapticRenderMode(PHHapticRenderDesc::CONSTRAINT);
				DSTR << "Constraint mode" << std::endl;
				break;
			}
		case 'c':
			{
				timer->Stop();
				spg->Calibration();
				DSTR << "CameraPosition" << std::endl;
				DSTR << GetCurrentWin()->GetTrackball()->GetPosition() << std::endl;
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
#include "MyApp.h"	

using namespace Spr;

// 0:SPIDAR-G6, 1:XBoxController
#define HUMAN_INTERFACE 0
// 0:single, 1:impulsemulti, 2:LD
#define ENGINETYPE 2

void MyApp::InitInterface(){
	HISdkIf* hiSdk = GetSdk()->GetHISdk();

#if HUMAN_INTERFACE == 0
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
#elif HUMAN_INTERFACE == 1
	spg = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	HIXbox360ControllerIf* con = DCAST(HIXbox360ControllerIf,spg);
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
		GetSdk()->SetDebugMode(true);
		
		// 床を作成
		CDBoxDesc bd;
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		bd.material.mu = 0.5;
		bd.material.mu0 = 0.6;
		PHSolidIf* floor = phscene->CreateSolid();
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, 0.0));
		floor->SetDynamical(false);
	
		// 箱を作成
		PHSolidIf* soBox = phscene->CreateSolid();
		soBox->SetMass(0.3f);
		bd.boxsize.clear(0.4f);
		soBox->AddShape(phSdk->CreateShape(bd));
		soBox->SetInertia(soBox->GetShape(0)->CalcMomentOfInertia() * soBox->GetMass());
		soBox->SetFramePosition(Vec3d(-0.5 , -0.35, 0.0));

		CDSphereDesc cd;
		cd.radius = 0.1f;
		cd.material.mu = 0.4;
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		pointer = phscene->CreateHapticPointer();
		pointer->AddShape(phSdk->CreateShape(cd));
		Posed defaultPose;
		defaultPose.Pos() = Vec3d(0.0, -0.35, 0.0);
		pointer->SetDefaultPose(defaultPose);
		pointer->SetHumanInterface(spg);
		pointer->SetInertia(pointer->GetShape(0)->CalcMomentOfInertia());
		pointer->SetLocalRange(0.1);
		pointer->SetPosScale(50);
		pointer->SetReflexSpring(5000);
		pointer->SetReflexDamper(0.1 * 0.0);
		pointer->EnableFriction(false);

		PHHapticEngineIf* he = phscene->GetHapticEngine();
		he->EnableHapticEngine(true);

#if ENGINETYPE == 0
		he->SetHapticEngineMode(PHHapticEngineDesc::SINGLE_THREAD);
		phscene->SetTimeStep(0.001);
#elif ENGINETYPE == 1
		he->SetHapticEngineMode(PHHapticEngineDesc::MULTI_THREAD);
		phscene->SetTimeStep(0.02);
#elif ENGINETYPE == 2
		he->SetHapticEngineMode(PHHapticEngineDesc::LOCAL_DYNAMICS);
		phscene->SetTimeStep(0.02);
#endif
		physicsTimerID = GetTimer(0)->GetID();
		GetTimer(0)->SetMode(UTTimerIf::IDLE);
		UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);
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
		PHHapticEngineIf* he = phscene->GetHapticEngine()->Cast();
		he->StepPhysicsSimulation();
		PostRedisplay();
	}
}
#else
void MyApp::TimerFunc(int id){
	if(hapticTimerID == id){
		//static float time = 0;
		//time += 0.001;
		//CSVOUT << time << "," << sobox->GetFramePosition().x << std::endl;
		phscene->Step();
	}else if(physicsTimerID == id){
		PostRedisplay();
	}
}
#endif


void MyApp::Keyboard(int key, int x, int y){
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Stop();
	switch(key){
		case 'q':
			exit(0);
			break;
		case '1':
			{
				DSTR << "Penalty mode" << std::endl;
				pointer->SetHapticRenderMode(PHHapticPointerDesc::PENALTY);
				break;
			}
		case '2':
			{
				DSTR << "Constraint mode" << std::endl;
				pointer->SetHapticRenderMode(PHHapticPointerDesc::CONSTRAINT);
				break;
			}
		case 'c':
			{
				spg->Calibration();
				DSTR << "CameraPosition" << std::endl;
				DSTR << GetCurrentWin()->GetTrackball()->GetPosition() << std::endl;
			}
			break;
		case 'f':
			{
				if(pointer){
					static bool bForce = false;
					if(!bForce){
						bForce = true;
						pointer->EnableForce(bForce);
						DSTR << "Force: ON" << std::endl;
					}else{
						bForce = false;
						pointer->EnableForce(bForce);					
						DSTR << "Force: OFF" << std::endl;
					}
				}
			}
			break;
		case 'v':
			{
				if(pointer){
					static bool bVibration = false;
					if(!bVibration){
						bVibration = true;
						pointer->EnableVibration(bVibration);
						DSTR << "Vibration: ON" << std::endl;
					}else{
						bVibration = false;
						pointer->EnableVibration(bVibration);					
						DSTR << "Vibration: ON" << std::endl;
					}
				}			
			}
			break;
		case 'd':
			{
				static bool bDebug = false;
				if(!bDebug){
					bDebug = true;
					GetSdk()->GetScene()->EnableRenderHaptic(bDebug);
					DSTR << "Debug Mode: ON" << std::endl;
				}else{
					bDebug = false;
					GetSdk()->GetScene()->EnableRenderHaptic(bDebug);				
					DSTR << "Debug Mode: OFF" << std::endl;
				}
			}
			break;
		case'a':
			{
				if(pointer){
					float spring = pointer->GetReflexSpring();
					spring += 100.0;
					pointer->SetReflexSpring(spring);
					DSTR << "Spring: " << spring << std::endl;
				}
			}
			break;
		case'z':
			{
				if(pointer){
					float spring = pointer->GetReflexSpring();
					spring -= 100.0;
					pointer->SetReflexSpring(spring);
					DSTR << "Spring: " << spring << std::endl;
				}
			}
			break;
		case ' ':
			{
				CDBoxDesc bd;
				bd.boxsize.clear(0.4f);
				PHSolidIf* box = phscene->CreateSolid();
				box->SetMass(0.3f);
				box->AddShape(GetSdk()->GetPHSdk()->CreateShape(bd));
				box->SetInertia(box->GetShape(0)->CalcMomentOfInertia() * box->GetMass());
				box->SetFramePosition(Vec3d(-0.5, 1.0, 0.0));
			}
		default:
			break;
	}
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Start();
}
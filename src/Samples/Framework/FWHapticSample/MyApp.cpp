#include "MyApp.h"	

using namespace Spr;

// ヒューマンインタフェースの切り替え
// 0:SPIDAR-G6, 1:XBoxController
#define HUMAN_INTERFACE 2
// 力覚エンジンの切り替え
// 0:single, 1:impulsemulti, 2:LD
#define ENGINETYPE 2

void MyApp::InitInterface(){
	HISdkIf* hiSdk = GetSdk()->GetHISdk();

#if HUMAN_INTERFACE == 0
	// x86
	DRUsb20SimpleDesc usbSimpleDesc;
	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i< 10; ++i){
		usb20Sh4Desc.channel = i;
		hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	// x64
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
#elif HUMAN_INTERFACE == 2
	spg = hiSdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
	spg->Init(NULL);
#endif
}

void MyApp::Init(int argc, char* argv[]){
		FWApp::Init(argc, argv);							// アプリケーションの初期化
		InitInterface();									// インタフェースの初期化
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();				// シェイプ作成のためにPHSdkへのポインタをとってくる
		phscene = GetSdk()->GetScene()->GetPHScene();		// 剛体作成のためにPHSceneへのポインタをとってくる
		
		Vec3d pos = Vec3d(0, 0, 1.21825);					// カメラ初期位置
		GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
		GetSdk()->SetDebugMode(true);						// デバック表示の有効化
			
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

		// 力覚ポインタの作成
		pointer = phscene->CreateHapticPointer();	// 力覚ポインタの作成
		CDSphereDesc cd;
		cd.radius = 0.1f;
		cd.material.mu = 0.4;
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		//pointer->AddShape(phSdk->CreateShape(bd));	// シェイプの追加
		pointer->AddShape(phSdk->CreateShape(cd));	// シェイプの追加
		Posed defaultPose;
		defaultPose.Pos() = Vec3d(0.0, -0.35, 0.0);
		pointer->SetDefaultPose(defaultPose);		// 力覚ポインタ初期姿勢の設定
		pointer->SetHumanInterface(spg);			// インタフェースの設定
		pointer->SetInertia(pointer->GetShape(0)->CalcMomentOfInertia());	// 慣性テンソルの設定
		pointer->SetLocalRange(0.1);				// 局所シミュレーション範囲の設定
		pointer->SetPosScale(50);					// 力覚ポインタの移動スケールの設定
		pointer->SetReflexSpring(5000);				// バネ係数の設定
		pointer->SetReflexDamper(0.1 * 0.0);		// ダンパ係数の設定
		pointer->EnableFriction(false);				// 摩擦を有効にするかどうか

		PHHapticEngineIf* he = phscene->GetHapticEngine();	// 力覚エンジンをとってくる
		he->EnableHapticEngine(true);						// 力覚エンジンの有効化

#if ENGINETYPE == 0
		// シングルスレッドモード
		he->SetHapticEngineMode(PHHapticEngineDesc::SINGLE_THREAD);
		phscene->SetTimeStep(0.001);
#elif ENGINETYPE == 1
		// マルチスレッドモード
		he->SetHapticEngineMode(PHHapticEngineDesc::MULTI_THREAD);
		phscene->SetTimeStep(0.02);
#elif ENGINETYPE == 2
		// 局所シミュレーションモード
		he->SetHapticEngineMode(PHHapticEngineDesc::LOCAL_DYNAMICS);
		phscene->SetTimeStep(0.02);
#endif
		physicsTimerID = GetTimer(0)->GetID();					// 物理スレッドのタイマIDの取得
		GetTimer(0)->SetMode(UTTimerIf::IDLE);					// 物理スレッドのタイマをIDLEモードに設定
		UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 力覚スレッド用のマルチメディアタイマを作成
		timer->SetResolution(1);		// 分解能(ms)
		timer->SetInterval(1);			// 刻み(ms)h
		hapticTimerID = timer->GetID();	// 力覚スレッドのタイマIDの取得
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
		phscene->Step();
	}else if(physicsTimerID == id){
		PostRedisplay();
	}
}
#endif


void MyApp::Keyboard(int key, int x, int y){
	// 各スレッドの共有メモリのアクセス違反回避のために全てのタイマをとめる
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Stop();
	switch(key){
		case 'q':
			// アプリケーションの終了
			exit(0);
			break;
		case '1':
			{
				// レンダリングモードをPenaltyに
				DSTR << "Penalty mode" << std::endl;
				pointer->SetHapticRenderMode(PHHapticPointerDesc::PENALTY);
				break;
			}
		case '2':
			{
				// レンダリングモードをConstraintに
				DSTR << "Constraint mode" << std::endl;
				pointer->SetHapticRenderMode(PHHapticPointerDesc::CONSTRAINT);
				break;
			}
		case 'c':
			{
				// インタフェースのキャリブレーション
				spg->Calibration();
				DSTR << "CameraPosition" << std::endl;
				DSTR << GetCurrentWin()->GetTrackball()->GetPosition() << std::endl;
			}
			break;
		case 'f':
			{
				// 力覚提示のON/OFF
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
				// 振動提示のON/OFF
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
				// デバック表示のON/OFF
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
				// バネ係数を100増やす
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
				// バネ係数を100減らす
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
				// 新たに剛体を生成する
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
	// 全てのタイマを始動
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Start();
}
/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include "FWHapticSample.h"	

using namespace Spr;

FWHapticSample::FWHapticSample(){
//	_crtBreakAlloc = 75154;
	pdt = 0.02f;
	hdt = 0.001f;
#ifdef  _DEBUG
	engineType = MULTI;										// SINGLE, MULTI, LD
#else
	engineType = SINGLE;									// SINGLE, MULTI, LD
#endif
	humanInterface = SPIDAR;
}

void FWHapticSample::BuildScene(){
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();				// シェイプ作成のためにPHSdkへのポインタをとってくる
		phscene = GetSdk()->GetScene()->GetPHScene();		// 剛体作成のためにPHSceneへのポインタをとってくる
		
		Vec3d pos = Vec3d(0, 0, 1.21825);					// カメラ初期位置
		GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
		GetSdk()->SetDebugMode(true);						// デバック表示の有効化
			
		// 床を作成
		CDBoxDesc bd;
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		bd.material.mu = 0.5f;
		bd.material.mu0 = 0.6f;
		PHSolidIf* floor = phscene->CreateSolid();
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -1.0, 0.0));
		floor->SetDynamical(false);
	
		// 箱を作成
		PHSolidIf* soBox = phscene->CreateSolid();
		soBox->SetMass(0.3f);
		bd.boxsize.clear(0.4f);
		soBox->AddShape(phSdk->CreateShape(bd));
		soBox->SetInertia(soBox->GetShape(0)->CalcMomentOfInertia() * (1/soBox->GetShape(0)->CalcVolume()) * (float)soBox->GetMass());
		soBox->SetFramePosition(Vec3d(-0.5 , -0.35, 0.0));
		soBox->SetName("soBox");

		// 力覚ポインタの作成
		pointer = phscene->CreateHapticPointer();	// 力覚ポインタの作成
		CDSphereDesc cd;
		cd.radius = 0.1f;
		cd.material.mu = 0.4f;
		bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
		//pointer->AddShape(phSdk->CreateShape(bd));	// シェイプの追加
		pointer->AddShape(phSdk->CreateShape(cd));	// シェイプの追加
		Posed defaultPose;
		defaultPose.Pos() = Vec3d(0.0, -0.35, 0.0);	
		pointer->SetDefaultPose(defaultPose);		// 力覚ポインタ初期姿勢の設定
		pointer->SetInertia(pointer->GetShape(0)->CalcMomentOfInertia() * (1/pointer->GetShape(0)->CalcVolume()));	// 慣性テンソルの設定
		pointer->SetLocalRange(0.1f);				// 局所シミュレーション範囲の設定
		pointer->SetPosScale(50);					// 力覚ポインタの移動スケールの設定
		pointer->SetReflexSpring(5000);				// バネ係数の設定
		pointer->SetReflexDamper(0.1 * 0.0);		// ダンパ係数の設定
		pointer->EnableFriction(false);				// 摩擦を有効にするかどうか
		//pointer->EnableDebugControl(true);		// キーボードから力覚ポインタを動かす機能カーソルでx、y方向に移動可能
		pointer->SetName("hpPointer");
		FWHapticPointerIf* fwPointer = GetSdk()->GetScene()->CreateHapticPointer();	// HumanInterfaceと接続するためのオブジェクトを作成
		fwPointer->SetHumanInterface(spg);		// HumanInterfaceの設定
		fwPointer->SetPHHapticPointer(pointer); // PHHapticPointerIfの設定

	//	hase	ポインタに重力を加えるべきか？ GravityEngineをOFFにしておくと、ポインタの振動がなくなる。
	//	phscene->GetGravityEngine()->Enable(false);
}

void FWHapticSample::InitInterface(){
	HISdkIf* hiSdk = GetSdk()->GetHISdk();

	if(humanInterface == SPIDAR){
		// x86
		DRUsb20SimpleDesc usbSimpleDesc;
		hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
		DRUsb20Sh4Desc usb20Sh4Desc;
		for(int i=0; i< 10; ++i){
			usb20Sh4Desc.channel = i;
			hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
		}
		//// x64
		//DRCyUsb20Sh4Desc cyDesc;
		//for(int i=0; i<10; ++i){
		//	cyDesc.channel = i;
		//	hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
		//}
		//hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
		//hiSdk->Print(DSTR);
		//hiSdk->Print(std::cout);

		spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
		spg->Init(&HISpidarGDesc("SpidarG6X3R"));
		spg->Calibration();
	}else if(humanInterface == XBOX){
		spg = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	}else if(humanInterface == FALCON){
		spg = hiSdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
		spg->Init(NULL);
	}
}

void FWHapticSample::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);							// アプリケーションの初期化
	InitInterface();									// インタフェースの初期化
	BuildScene();										// オブジェクトの作成
	PHHapticEngineIf* he = phscene->GetHapticEngine();	// 力覚エンジンをとってくる
	he->Enable(true);						            // 力覚エンジンの有効化

	if(engineType == SINGLE){
		// シングルスレッドモード
		he->SetHapticEngineMode(PHHapticEngineDesc::SINGLE_THREAD);
		phscene->SetTimeStep(hdt);
	}else if(engineType == MULTI){
		// マルチスレッドモード
		he->SetHapticEngineMode(PHHapticEngineDesc::MULTI_THREAD);
		phscene->SetTimeStep(pdt);
	}else if(engineType == LD){
		// 局所シミュレーションモード
		he->SetHapticEngineMode(PHHapticEngineDesc::LOCAL_DYNAMICS);
		phscene->SetTimeStep(pdt);
	}
		physicsTimerID = GetTimer(0)->GetID();					// 物理スレッドのタイマIDの取得
		GetTimer(0)->SetMode(UTTimerIf::IDLE);					// 物理スレッドのタイマをIDLEモードに設定
		UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 力覚スレッド用のマルチメディアタイマを作成
		timer->SetResolution(1);			// 分解能(ms)
		timer->SetInterval(unsigned int(hdt * 1000));		// 刻み(ms)h
		hapticTimerID = timer->GetID();		// 力覚スレッドのタイマIDの取得
		timer->Start();						// タイマスタート
}


void FWHapticSample::TimerFunc(int id){
	if(engineType == 0){
		if(hapticTimerID == id){
			GetSdk()->GetScene()->UpdateHapticPointers();
			phscene->Step();
		}else if(physicsTimerID == id){
			PostRedisplay();
		}
	}else if(engineType > 0){
		if(hapticTimerID == id){
			GetSdk()->GetScene()->UpdateHapticPointers();
			phscene->StepHapticLoop();
		}else{
			PHHapticEngineIf* he = phscene->GetHapticEngine();
			he->StepPhysicsSimulation();
			PostRedisplay();
		}
	}
}

void FWHapticSample::Keyboard(int key, int x, int y){
	// 各スレッドの共有メモリのアクセス違反回避のために全てのタイマをとめる
	float dr = 0.01f;
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Stop();
	switch(key){
		case 'q':
			// アプリケーションの終了
			exit(0);
			break;

		case 't':
			{
				PHSolidIf* box = GetSdk()->GetScene()->GetPHScene()->FindObject("soBox")->Cast();
				GetSdk()->GetScene()->GetPHScene()->SetContactMode(pointer, box, PHSceneDesc::MODE_NONE);
				break;
			}
		case 'y':
			{
				PHSolidIf* box = GetSdk()->GetScene()->GetPHScene()->FindObject("soBox")->Cast();
				GetSdk()->GetScene()->GetPHScene()->SetContactMode(pointer, box, PHSceneDesc::MODE_LCP);
				break;
			}

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
				GetSdk()->GetScene()->GetPHScene()->GetHapticEngine()->ReleaseState();
				CDBoxDesc bd;
				bd.boxsize.clear(0.4f);
				PHSolidIf* box = phscene->CreateSolid();
				box->SetMass(0.3f);
				box->AddShape(GetSdk()->GetPHSdk()->CreateShape(bd));
				box->SetInertia(box->GetShape(0)->CalcMomentOfInertia() * (1/box->GetShape(0)->CalcVolume()) * (float)box->GetMass());
				box->SetFramePosition(Vec3d(-0.5, 1.0, 0.0));
			}
		case 356: // left
			{
				Vec3d pos = pointer->GetFramePosition();
				pos.x -= dr;
				pointer->SetFramePosition(pos);
			}
			break;
		case 358: // right
			{
				Vec3d pos = pointer->GetFramePosition();
				pos.x += dr;
				pointer->SetFramePosition(pos);
			}
			break;		
		case 357: // up
			{
				Vec3d pos = pointer->GetFramePosition();
				pos.y += dr;
				pointer->SetFramePosition(pos);
			}
			break;			
		case 359: // down
			{
				Vec3d pos = pointer->GetFramePosition();
				pos.y -= dr;
				pointer->SetFramePosition(pos);
			}
			break;	
		default:
			break;
	}
	// 全てのタイマを始動
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Start();
}

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
	bPause = false;
}
void FWHapticSample::BuildScene(){
		PHSdkIf* phSdk = GetSdk()->GetPHSdk();				// シェイプ作成のためにPHSdkへのポインタをとってくる
		phscene = GetSdk()->GetScene()->GetPHScene();		// 剛体作成のためにPHSceneへのポインタをとってくる

		Vec3d pos = Vec3d(0, 0, 0.1);						// カメラ初期位置
		GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
		GetSdk()->SetDebugMode(true);						// デバック表示の有効化
		GetSdk()->GetScene()->EnableRenderHaptic(true);		//	力覚デバッグ表示ON

		// 床を作成
		CDBoxDesc bd;
		bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
		PHSolidIf* floor = phscene->CreateSolid();
		floor->AddShape(phSdk->CreateShape(bd));
		floor->SetFramePosition(Vec3d(0, -0.015 - bd.boxsize.y/2, 0.0));
		floor->SetDynamical(false);
	
		// 箱を作成
#if 1
		PHSolidIf* soBox = phscene->CreateSolid();
		bd.boxsize.clear(0.04f);
		bd.material.density = 10000;
		soBox->AddShape(phSdk->CreateShape(bd));
		soBox->CompInertia();
		soBox->SetCenterPosition(Vec3d(-0.05 , -0.035, 0.0));
		soBox->SetName("soBox");
#endif
		// 力覚ポインタの作成
		pointer = phscene->CreateHapticPointer();			// 力覚ポインタの作成
		CDSphereDesc cd;									//　半径1cmの球
		cd.radius = 0.01f;
		bd.boxsize = Vec3f(0.02f, 0.02f, 0.02f);			//	１辺2cmの直方体
		CDShapeIf* shape = phSdk->CreateShape(cd);			//	どちらかを作る
		shape->SetDensity(0.006f / shape->CalcVolume());	//	指の重さは大体 6g
		pointer->AddShape(shape);	// シェイプの追加
		pointer->SetShapePose(0, Posed(Vec3d(), Quaterniond::Rot(Rad(10), 'z')));
		pointer->SetDefaultPose(Posed());					//	力覚ポインタ初期姿勢の設定
		pointer->CompInertia();								//	質量と慣性テンソルを密度から計算
		pointer->SetLocalRange(0.02f);						//	局所シミュレーション範囲の設定
		pointer->SetPosScale(1.0f);							//	力覚ポインタの移動スケールの設定
		pointer->SetFrictionSpring(500.0f);					//	DynamicProxyの際の摩擦計算に使うバネ係数
		pointer->SetReflexSpring(3000.0f);					//	力覚レンダリング用のバネ
		pointer->SetReflexDamper(0.0f);						//	力覚レンダリング用のダンパ
		pointer->SetRotationReflexSpring(30.0f);			//	力覚レンダリング用の回転バネ
		pointer->SetRotationReflexDamper(0.0f);				//	力覚レンダリング用の回転ダンパ
		pointer->SetName("hpPointer");
		pointer->EnableRotation(false);
		pointer->EnableFriction(true);
		pointer->EnableVibration(true);
		pointer->SetHapticRenderMode(PHHapticPointerDesc::DYNAMIC_PROXY);
		pointer->EnableTimeVaryFriction(true);
		FWHapticPointerIf* fwPointer = GetSdk()->GetScene()->CreateHapticPointer();	// HumanInterfaceと接続するためのオブジェクトを作成
		fwPointer->SetHumanInterface(device);		// HumanInterfaceの設定
		fwPointer->SetPHHapticPointer(pointer);		// PHHapticPointerIfの設定
}

void FWHapticSample::InitInterface(){
	HISdkIf* hiSdk = GetSdk()->GetHISdk();
	//	実デバイスの追加
	//	for SPIDAR
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

	//	インタフェースの取得
	device = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
#ifdef	_MSC_VER
	if (device->Init(&HISpidarGDesc("SpidarG6X3F"))) {
#else
	HISpidarGDesc tmpdesc = HISpidarGDesc((char*) "SpidarG6X3F");
	if (device->Init(&tmpdesc)) {
#endif
		device->Calibration();
	}else{	//	XBOX
		device = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
		if (!device->Init(NULL)) {
			device = hiSdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
			if (!device->Init(NULL)) {
				device = hiSdk->CreateHumanInterface(HIHapticDummyIf::GetIfInfoStatic())->Cast();
				device->Init(NULL);
				dummyDevice = device->Cast();
			}
		}
	}
	hiSdk->Print(DSTR);
	hiSdk->Print(std::cout);
}

void FWHapticSample::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);							// アプリケーションの初期化
	InitInterface();									// インタフェースの初期化
	BuildScene();										// オブジェクトの作成
	PHHapticEngineIf* he = phscene->GetHapticEngine();	// 力覚エンジンをとってくる
	he->Enable(true);						            // 力覚エンジンの有効化

#if 0	// シングルスレッドモード
	he->SetHapticEngineMode(PHHapticEngineDesc::SINGLE_THREAD);
	phscene->SetTimeStep(hdt);
#elif 0
	// マルチスレッドモード
	he->SetHapticEngineMode(PHHapticEngineDesc::MULTI_THREAD);
	phscene->SetTimeStep(pdt);
#else
	// 局所シミュレーションモード
	he->SetHapticStepMode(PHHapticEngineDesc::LOCAL_DYNAMICS);
	phscene->SetTimeStep(pdt);
#endif
	physicsTimerID = GetTimer(0)->GetID();					// 物理スレッドのタイマIDの取得
	GetTimer(0)->SetMode(UTTimerIf::IDLE);					// 物理スレッドのタイマをIDLEモードに設定
		
#if 0	//	Multi Media timer or Thread
	UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 力覚スレッド用のマルチメディアタイマを作成
	timer->SetResolution(1);			// 分解能(ms)
	timer->SetInterval(unsigned int(hdt * 1000));		// 刻み(ms)h
	hapticTimerID = timer->GetID();		// 力覚スレッドのタイマIDの取得
	timer->Start();						// タイマスタート
#else
	UTTimerIf* timer = CreateTimer(UTTimerIf::THREAD);	// 力覚スレッド用のマルチメディアタイマを作成
	timer->SetResolution(1);			// 分解能(ms)
	timer->SetInterval((unsigned int)(hdt * 1000));		// 刻み(ms)h
	hapticTimerID = timer->GetID();		// 力覚スレッドのタイマIDの取得
	timer->Start();						// タイマスタート
#endif
}


void FWHapticSample::TimerFunc(int id){
	if(phscene->GetHapticEngine()->GetHapticStepMode() == PHHapticEngineDesc::SINGLE_THREAD){
		if(hapticTimerID == id){
			GetSdk()->GetScene()->UpdateHapticPointers();
			phscene->Step();
		}else if(physicsTimerID == id){
			PostRedisplay();
		}
	}else{	//	multi thread
		if(hapticTimerID == id){
			GetSdk()->GetScene()->UpdateHapticPointers();
			if (!bPause) {
				phscene->StepHapticLoop();
				phscene->StepHapticSync();
			}
		}else{
			PHHapticEngineIf* he = phscene->GetHapticEngine();
			if (!bPause) {
				he->StepPhysicsSimulation();
			}
			PostRedisplay();
		}
	}
}

void FWHapticSample::Keyboard(int key, int x, int y){
	// 各スレッドの共有メモリのアクセス違反回避のために全てのタイマをとめる
	float dr = 0.001f;
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Stop();
	switch(key){
		case 'q':
		case 0x1b:
		// アプリケーションの終了
			exit(0);
			break;
		case 'T':
			{
				PHSolidIf* box = GetSdk()->GetScene()->GetPHScene()->FindObject("soBox")->Cast();
				GetSdk()->GetScene()->GetPHScene()->SetContactMode(pointer, box, PHSceneDesc::MODE_NONE);
				break;
			}
		case 'Y':
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
		case '3':
		{
			// レンダリングモードをDynamics Constraintに
			DSTR << "Dynamics Constraint mode" << std::endl;
			pointer->SetHapticRenderMode(PHHapticPointerDesc::DYNAMIC_PROXY);
			break;
		}
		case 't':	//	time vary friction
			pointer->EnableTimeVaryFriction(!pointer->IsTimeVaryFriction());
			std::cout << "timeVaryFriction:" << pointer->IsTimeVaryFriction() << std::endl;
			break;
		case 'c':
			{
				// インタフェースのキャリブレーション
				device->Calibration();
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
					if(!pointer->IsVibration()){
						pointer->EnableVibration(true);
						DSTR << "Vibration: ON" << std::endl;
					}else{
						pointer->EnableVibration(false);					
						DSTR << "Vibration: OFF" << std::endl;
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
					float spr = pointer->GetReflexSpring();
					spr += 100.0;
					pointer->SetReflexSpring(spr);
					DSTR << "Spring: " << spr << std::endl;
				}
			}
			break;
		case'z':
			{
				// バネ係数を100減らす
				if(pointer){
					float spr = pointer->GetReflexSpring();
					spr -= 100.0;
					pointer->SetReflexSpring(spr);
					DSTR << "Spring: " << spr << std::endl;
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
		case 'P':
			bPause = !bPause;
			break;
		case 'p':
		{
			static int count;
			phscene->StepHapticLoop();
			phscene->StepHapticSync();
			if (count %20 == 0) phscene->GetHapticEngine()->StepPhysicsSimulation();
			count++;
			break;
		}

		case DVKeyCode::LEFT:
			if (dummyDevice){
				Posed p = dummyDevice->GetPose();
				p.PosX() -= dr;
				dummyDevice->SetPose(p);
			}
			break;
		case DVKeyCode::RIGHT:
			if (dummyDevice) {
				Posed p = dummyDevice->GetPose();
				p.PosX() += dr;
				dummyDevice->SetPose(p);
			}
			break;		
		case DVKeyCode::UP:
			if (dummyDevice) {
				Posed p = dummyDevice->GetPose();
				p.PosY() += dr;
				dummyDevice->SetPose(p);
			}
			break;			
		case DVKeyCode::DOWN:
			if (dummyDevice) {
				Posed p = dummyDevice->GetPose();
				p.PosY() -= dr;
				dummyDevice->SetPose(p);
			}
			break;
		default:
			break;
	}
	// 全てのタイマを始動
	for(int i = 0; i < NTimers(); i++)	GetTimer(i)->Start();
}

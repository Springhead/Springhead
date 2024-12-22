/*
 *  Copyright (c) 2003-2020, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
\page pageFWHaptic SPIDARを用いた力覚インタラクションのサンプル
	Springhead2/core/src/Samples/Framework/FWHaptic

\section secQuitBoxStack 終了基準
- Escか'q'で強制終了。

*/

#include "../../SampleApp.h"

#pragma hdrstop
using namespace Spr;
using namespace std;

class MyApp : public SampleApp{
public:
	PHSolidIf* soFloor = NULL;			//	A solod for the floor
	std::vector<PHSolidIf*> soBox;		//	solid falling on the floor
	PHHapticPointerIf* pointer = NULL;	//	The haptic pointer
	UTRef<HIBaseIf> device = NULL;		//	The haptic interface device
	HIHapticDummyIf* dummyDevice = NULL;//	Dummy human interface operated by keyboard
	double pdt = 0.02f;					//	Delta t for physics simulation
	double hdt = 0.001f;				//	Delta t for haptic rendering
	UTTimerIf* hapticTimer = NULL;

	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_CALIB,
		ID_TIME_VARY_FRICTION,
		ID_FEEDBACK_FORCE,
		ID_FRICTION,
		ID_BOX,
		ID_CAPSULE,
		ID_ROUNDCONE,
		ID_SPHERE,
		ID_ELLIPSOID,
		ID_ROCK,
		ID_BLOCK,
		ID_SHAKE,
	};

private:
	//double ShapeScale() { return 0.006; }
	double ShapeScale() { return 0.02; }
public:
	MyApp(){
		appName = "FWHaptic";

		AddAction(MENU_MAIN, ID_CALIB, "Calibrate human interface");
		AddHotKey(MENU_MAIN, ID_CALIB, 'c');
		AddAction(MENU_MAIN, ID_FEEDBACK_FORCE, "Ouput force feedback").enabled = false;
		AddHotKey(MENU_MAIN, ID_FEEDBACK_FORCE, 'o');
		AddAction(MENU_MAIN, ID_TIME_VARY_FRICTION, "Time vary static frction").enabled = false;
		AddHotKey(MENU_MAIN, ID_TIME_VARY_FRICTION, 't');
		AddAction(MENU_MAIN, ID_FRICTION, "Frictoin").enabled = true;
		AddHotKey(MENU_MAIN, ID_FRICTION, 'f');

		AddAction(MENU_MAIN, ID_BOX, "drop box");
		AddHotKey(MENU_MAIN, ID_BOX, 'b');
		AddAction(MENU_MAIN, ID_CAPSULE, "drop capsule");
		AddHotKey(MENU_MAIN, ID_CAPSULE, 'a');
		AddAction(MENU_MAIN, ID_ROUNDCONE, "drop round cone");
		AddHotKey(MENU_MAIN, ID_ROUNDCONE, 'r');
		AddAction(MENU_MAIN, ID_SPHERE, "drop sphere");
		AddHotKey(MENU_MAIN, ID_SPHERE, 's');
		AddAction(MENU_MAIN, ID_ELLIPSOID, "drop ellipsoid");
		AddHotKey(MENU_MAIN, ID_ELLIPSOID, 'E');
		AddAction(MENU_MAIN, ID_ROCK, "drop rock");
		AddHotKey(MENU_MAIN, ID_ROCK, 'd');
		AddAction(MENU_MAIN, ID_BLOCK, "drop block");
		AddHotKey(MENU_MAIN, ID_BLOCK, 'e');
	}
	~MyApp(){}
	virtual void Cleanup() {
		for (int i = 0; i != NTimers(); ++i) GetTimer(i)->Stop();
	}

	virtual void Init(int argc, char* argv[]) {
		SampleApp::Init(argc, argv);
		PHSceneIf* phscene = GetPHScene();
		PHHapticEngineIf* he = phscene->GetHapticEngine();	// 力覚エンジンをとってくる
		he->Enable(true);						            // 力覚エンジンの有効化

		//--------------------------------------------
		//	Set timer mode for physics simulation (SampleApp::Init() already created a timer for it)
#if 0	// Single thread mode
		he->SetHapticStepMode(PHHapticEngineDesc::SINGLE_THREAD);
		phscene->SetTimeStep(hdt);
#elif 0
		// Multi thread mode
		he->SetHapticStepMode(PHHapticEngineDesc::MULTI_THREAD);
		phscene->SetTimeStep(pdt);
#elif 0
		// Local dynamics mode (multi thread)
		he->SetHapticStepMode(PHHapticEngineDesc::LOCAL_DYNAMICS);
		phscene->SetTimeStep(pdt);
#elif 1
		// Local dynamics 6DOF mode (multi thread)
		he->SetHapticStepMode(PHHapticEngineDesc::LOCAL_DYNAMICS6DOF);
		phscene->SetTimeStep(pdt);
#endif
		timer->SetMode(UTTimerIf::IDLE);
		timer->SetInterval(1000*pdt);
		EnableIdleFunc(true);

		//--------------------------------------------
		//	Create timer for haptic rendering
#if 0	//		use multi media timer
		hapticTimer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 力覚スレッド用のマルチメディアタイマを作成
		hapticTimer->SetResolution(1);			// 分解能(ms)
		hapticTimer->SetInterval(unsigned int(hdt * 1000));		// 刻み(ms)h
		hapticTimer->Start();						// タイマスタート
#else	//		or thread
		hapticTimer = CreateTimer(UTTimerIf::THREAD);			// 力覚スレッド用のマルチメディアタイマを作成 自動的にStart()する。
		hapticTimer->SetResolution(1);							// 分解能(ms)
		hapticTimer->SetInterval((unsigned int)(hdt * 1000));	// 刻み(ms)h
		//	hapticTimer->Stop();
#endif
	}

	///	Initialize human interface (SPIDAR)
	void InitHumanInterface() {
		HISdkIf* hiSdk = GetSdk()->GetHISdk();
		//	Driver for SPIDAR driver with SH4 in Win32(x86)
#ifdef USE_CLOSED_SRC
		DRUsb20Sh4Desc usb20Sh4Desc;
		for (int i = 0; i < 10; ++i) {
			usb20Sh4Desc.channel = i;
			hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
		}
		//	Driver for SPIDAR driver with SH4 in Win64(x64)
		DRCyUsb20Sh4Desc cyDesc;
		for (int i = 0; i < 10; ++i) {
			cyDesc.channel = i;
			hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
		}
#endif
		//	Driver for UART SPIDAR driver.
		DRUARTMotorDriverDesc uartDesc;
		hiSdk->AddRealDevice(DRUARTMotorDriverIf::GetIfInfoStatic(), &uartDesc);
		hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
		//	Create human interface device

		//	Try to create SPIDAR and calibrate it
		device = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
		DRUARTMotorDriverIf* ud = (DRUARTMotorDriverIf*) hiSdk->FindRealDevice(DRUARTMotorDriverIf::GetIfInfoStatic());
		if (ud->NMotor() >= 8 && device->Init(&HISpidarGDesc("SpidarG6X4R"))) {
			device->Calibration();
			((UTRef<HISpidarGIf>)device)->SetWeight(0.3f, 1.0f, 6.0f * 0);
		}
		else if (device->Init(&HISpidarGDesc("SpidarG6X3F"))) {
			device->Calibration();
		}
		else {	//	If failed, try to create an XBOX
			device = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
			if (!device->Init(NULL)) {	//	If failed, try to create a Novient Falcon
				device = hiSdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
				if (!device->Init(NULL)) {	//	If failed, try to create a dummy device (operate by keybaord)
					device = hiSdk->CreateHumanInterface(HIHapticDummyIf::GetIfInfoStatic())->Cast();
					device->Init(NULL);
					dummyDevice = device->Cast();
				}
			}
		}
		//	show the results
		hiSdk->Print(DSTR);
		hiSdk->Print(std::cout);
	}
	virtual void BuildScene(){
		InitHumanInterface();

		GetFWScene()->SetRenderMode(true, false);				///< ソリッド描画，ワイヤフレーム描画
		GetFWScene()->EnableRenderAxis(false, false, false);	///< 座標軸
		GetFWScene()->SetAxisStyle(FWSceneIf::AXIS_ARROWS);		///< 座標軸のスタイル
		GetFWScene()->EnableRenderForce(false, false);			///< 力
		GetFWScene()->EnableRenderContact(false);				///< 接触断面

		//	view point settings
		Vec3d pos = Vec3d(0, 0.2, 0.4);					//	initial camera position
		GetCurrentWin()->GetTrackball()->SetPosition(pos);	//	setting it to the trackball handler for mouse gesture
		GetSdk()->SetDebugMode(true);						//	show debug info
		GetSdk()->GetScene()->EnableRenderHaptic(true);		//	show debug info for haptic rendering

		PHSdkIf* phSdk = GetSdk()->GetPHSdk();
		PHSceneIf* phscene = GetPHScene();
		phscene->SetGravity(Vec3d(0, -9.8*0.1, 0));
#if 1	//	Floor
		soFloor = CreateFloor(false);
		soFloor->SetFramePosition(Vec3d(0, -0.03, 0));
#endif

		// Make the haptic pointer
		pointer = phscene->CreateHapticPointer();
		//　Create a shapre for haptic pointer
		CDSphereDesc cd;
		cd.radius = 0.01f;
		cd.material.mu = cd.material.mu0 = 0;				//	No friction
		//CDBoxDesc bd;
		//bd.boxsize = Vec3f(0.02f, 0.02f, 0.02f);
		CDShapeIf* shape = phSdk->CreateShape(cd);			//	Create a shape. Choose cd (for a sphere of 2cm) or bd (for a cube pf 2cm)
		shape->SetDensity(0.006f / shape->CalcVolume());	//	Mass of haptic pointer (6g), approximating a finger tip.
		pointer->AddShape(shape);	// シェイプの追加
		pointer->SetShapePose(0, Posed(Vec3d(), Quaterniond::Rot(Rad(10), 'z')));

		//	Setting of the haptic pointer
		pointer->SetDefaultPose(Posed());
		pointer->CompInertia();								//	compute mass and intertia from the density
		pointer->SetLocalRange(0.02f);						//	range of local simulation or haptic rendering
		pointer->SetPosScale(1.0f);							//	Scaling of haptic pointer's translational motion.
		pointer->SetFrictionSpring(100.0f);					//	Coefficient of spring for fricition computation in the dynamic proxy mode
		pointer->SetReflexSpring(1000.0f);					//	Coefficient of spring for haptic rendering (of normal force)
		pointer->SetReflexDamper(0.0f);						//	Coefficient of damper for haptic rendering (of normal force)
		pointer->SetRotationReflexSpring(30.0f);			//	Coefficient of rotaional spring for haptic rendering (of normal force)
		pointer->SetRotationReflexDamper(0.0f);				//	Coefficient of rotaional damper for haptic rendering (of normal force)
		pointer->SetName("hpPointer");						//	name of haptic pointer solid in the phscene
		pointer->EnableRotation(true);						//	use rotation of haptic pointer or not
		pointer->EnableFriction(true);						//	Render friction or not
		pointer->EnableVibration(true);						//	Add vibration when impulse occur during haptic rendering.
		pointer->SetHapticRenderMode(PHHapticPointerDesc::CONSTRAINT);
															//	Haptic rendering mode. CONSTRAINT is usual one. DYNAMIC_RPOXY has mass and make stick-slip and vibraiton by friction.
		pointer->EnableTimeVaryFriction(false);				//	Friction model (use time vary (stopping time increase static friction) or not.

		//	Contect pointer to human interface device
		FWHapticPointerIf * fwPointer = GetSdk()->GetScene()->CreateHapticPointer();	// a syncronizer between device and pointer
		fwPointer->SetHumanInterface(device);		// Set human interface to the syncronizer
		fwPointer->SetPHHapticPointer(pointer);		// set haptic pointer to the syncronizer
	}
	virtual void TimerFunc(int id) {
		PHSceneIf* phscene = GetPHScene();
		if (!phscene) return;
		if (phscene->GetHapticEngine()->GetHapticStepMode() == PHHapticEngineDesc::SINGLE_THREAD) {
			if (hapticTimer->GetID() == id) {
				GetSdk()->GetScene()->UpdateHapticPointers();
				if (menus[MENU_ALWAYS][ID_RUN].enabled) {
					phscene->Step();
				}
			}
			else if (timer->GetID()== id) {
				PostRedisplay();
			}
		}
		else {	//	multi thread
			if (hapticTimer->GetID() == id) {
				GetSdk()->GetScene()->UpdateHapticPointers();
				if (menus[MENU_ALWAYS][ID_RUN].enabled) {
					phscene->StepHapticLoop();
					phscene->StepHapticSync();
				}
			}
			else {
				PHHapticEngineIf* he = phscene->GetHapticEngine();
				if (menus[MENU_ALWAYS][ID_RUN].enabled) {
					he->StepPhysicsSimulation();
				}
				PostRedisplay();
			}
		}
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		SampleApp::OnDraw(render);

		std::ostringstream sstr;
		sstr << "NObj = " << GetFWScene()->GetPHScene()->NSolids();
		render->DrawFont(Vec2f(-21, 23), sstr.str());
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_MAIN){
			Vec3d v, w(0.0, 0.0, 0.2), p(Vec3d(0.5, 20.0, 0.0) * ShapeScale());
			static Quaterniond q = Quaterniond::Rot(Rad(0.0), 'y');
			q = Quaterniond::Rot(Rad(90), 'y') * q;

			if (id == ID_CALIB) {
				device->Calibration();
				message = "Human interface device calibrated.";
			}
			if (id == ID_TIME_VARY_FRICTION) {
				if (ToggleAction(MENU_MAIN, ID_TIME_VARY_FRICTION)) {
					pointer->EnableTimeVaryFriction(true);
				}
				else {
					pointer->EnableTimeVaryFriction(false);
				}
				//message = "timeVaryFriction:" + pointer->IsTimeVaryFriction();
			}
			if (id == ID_FEEDBACK_FORCE) {
				if (ToggleAction(MENU_MAIN, ID_FEEDBACK_FORCE)) {
					if (pointer) pointer->EnableForce(true);
				}
				else {
					if (pointer) pointer->EnableForce(false);
				}
			}
			if (id == ID_FRICTION) {
				static PHMaterial matOrg[2];
				if (ToggleAction(MENU_MAIN, ID_FRICTION)) {
					if (pointer && soFloor) {
						PHMaterial mat = matOrg[0] = pointer->GetShape(0)->GetMaterial();
						mat.mu = 0;  mat.mu0 = 0;
						pointer->GetShape(0)->SetMaterial(mat);
						mat = matOrg[1] = soFloor->GetShape(0)->GetMaterial();
						mat.mu = 0;  mat.mu0 = 0;
						soFloor->GetShape(0)->SetMaterial(mat);
					}
				}
				else {
					if (pointer && soFloor) {
						pointer->GetShape(0)->SetMaterial(matOrg[0]);
						soFloor->GetShape(0)->SetMaterial(matOrg[1]);
					}
				}
			}
			if(id == ID_BOX){
				Drop(SHAPE_BOX, GRRenderIf::RED, v, w, p, q);
				message = "box dropped.";
			}
			if(id == ID_CAPSULE){
				Drop(SHAPE_CAPSULE, GRRenderIf::GREEN, v, w, p, q);
				message = "capsule dropped.";
			}
			if(id == ID_ROUNDCONE){
				Drop(SHAPE_ROUNDCONE, GRRenderIf::BLUE, v, w, p, q);
				message = "round cone dropped.";
			}
			if (id == ID_SPHERE) {
				Drop(SHAPE_SPHERE, GRRenderIf::YELLOW, v, w, p, q);
				message = "sphere dropped.";
			}
			if (id == ID_ELLIPSOID) {
				Drop(SHAPE_ELLIPSOID, GRRenderIf::LIGHTGREEN, v, w, p, q);
				message = "sphere dropped.";
			}
			if(id == ID_ROCK){
				Drop(SHAPE_ROCK, GRRenderIf::ORANGE, v, w, p, q);
				message = "random polyhedron dropped.";
			}
			if(id == ID_BLOCK){
				Drop(SHAPE_BLOCK, GRRenderIf::CYAN, v, w, p, q);
				message = "composite block dropped.";
			}
			if (message == "") {
				Action& act = menus[menu][id];
				if (act.message[1] == "") {
					message = act.message[0] + (act.enabled ? " enabled." : " disabled.");
				}
				else {
					message = act.message[!act.enabled];
				}
			}
		}
		SampleApp::OnAction(menu, id);
	}
	virtual void Keyboard(int key, int x, int y) {
		//	stop all timers to manipulate sceen safely
		for (int i = 0; i < NTimers(); i++)	GetTimer(i)->Stop();

		float dr = 0.001f;
		switch(key){
		case DVKeyCode::LEFT:
			if (dummyDevice) {
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
		case DVKeyCode::PAGE_UP:
			if (dummyDevice) {
				Posed p = dummyDevice->GetPose();
				p.PosZ() -= dr;
				dummyDevice->SetPose(p);
			}
			break;
		case DVKeyCode::PAGE_DOWN:
			if (dummyDevice) {
				Posed p = dummyDevice->GetPose();
				p.PosZ() += dr;
				dummyDevice->SetPose(p);
			}
			break;
		default:
			PHHapticEngineIf* he = GetPHScene()->GetHapticEngine();
			he->ReleaseState();
			SampleApp::Keyboard(key, x, y);
			break;
		}
		//	start all timers to manipulate sceen safely
		for (int i = 0; i < NTimers(); i++)	GetTimer(i)->Start();
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

MyApp app;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int SPR_CDECL main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

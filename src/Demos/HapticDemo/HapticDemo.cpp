#include "HapticDemo.h"
#include "Samplemodel.h"
#include <iostream>
#include <sstream>
#include <Framework/FWInteractScene.h>

using namespace std;

#define SPIDAR 1;

FWLDHapticSample::FWLDHapticSample(){
}

void FWLDHapticSample::Init(int argc, char* argv[]){
	/// Sdkの初期化，シーンの作成
	CreateSdk();
	GetSdk()->Clear();										// SDKの初期化
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.02);	// 刻みの設定
	//GetSdk()->GetScene()->GetPHScene()->SetGravity(Vec3d());
	/// 描画モードの設定
	SetGRAdaptee(TypeGLUT);									// GLUTモードに設定
	GetGRAdaptee()->Init(argc, argv);						// Sdkの作成

	/// 描画Windowの作成，初期化
	FWWinDesc windowDesc;									// GLのウィンドウディスクリプタ
	windowDesc.title = "Springhead2";						// ウィンドウのタイトル
	CreateWin(windowDesc);									// ウィンドウの作成
	InitWindow();											// ウィンドウの初期化
	InitCameraView();										// カメラビューの初期化

	/// HumanInterfaceの初期化
	InitHumanInterface();

	/// InteractSceneの作成
	FWInteractSceneDesc desc;
	desc.fwScene = GetSdk()->GetScene();					// fwSceneに対するinteractsceneを作る
	desc.mode = LOCAL_DYNAMICS;								// humaninterfaceのレンダリングモードの設定
	desc.hdt = 0.001;										// マルチレートの場合の更新[s]
	CreateINScene(desc);									// interactSceneの作成

	/// 物理シミュレーションする剛体を作成
	BuildScene();
	BuildPointer();

	/// タイマの作成，設定
	FWTimer* timer = CreateTimer(MMTimer);
	timer->SetInterval(1);
	timer->SetResolution(1);
}
void FWLDHapticSample::InitCameraView(){
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWLDHapticSample::InitHumanInterface(){
	/// HISdkの作成
	CreateHISdk();

	DRUsb20SimpleDesc usbSimpleDesc;
	GetHISdk()->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.number = i;
		GetHISdk()->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	GetHISdk()->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	GetHISdk()->Init();
	GetHISdk()->Print(DSTR);

#if SPIDAR
	/// SPIDARG6を2台使う場合
	UTRef<HISpidarGIf> spg[2];
	for(size_t i = 0; i < 1; i++){
		spg[i] = GetHISdk()->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
		if(i == 0) spg[i]->Init(&HISpidarGDesc("SpidarG6X3R"));
		if(i == 1) spg[i]->Init(&HISpidarGDesc("SpidarG6X3L"));
		AddHI(spg[i]);
	}
#else
	/// SPIDAR4Dを使う場合
	UTRef<HISpidar4If> spg[2];
	for(size_t i = 0; i < 2; i++){
		spg[i] = GetHISdk()->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
		if(i == 0) spg[i]->Init(&HISpidar4Desc("SpidarR",Vec4i(1,2,3,4)));
		if(i == 1) spg[i]->Init(&HISpidar4Desc("SpidarL",Vec4i(5,6,7,8)));
		AddHI(spg[i]);
	}
#endif
}
void FWLDHapticSample::BuildPointer(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;


	/// ポインタ
	{	
		for(int i= 0; i < 1; i++){
			PHSolidIf* soPointer = phscene->CreateSolid(desc);
			CDSphereDesc sd;
			sd.radius = 0.5;//1.0;
			CDSphereIf* shapePointer = DCAST(CDSphereIf,  GetSdk()->GetPHSdk()->CreateShape(sd));
			soPointer->AddShape(shapePointer);
			soPointer->SetDynamical(false);
			soPointer->GetShape(0)->SetStaticFriction(1.0);
			soPointer->GetShape(0)->SetDynamicFriction(1.0);
			GetSdk()->GetScene()->GetPHScene()->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);
			FWInteractPointerDesc idesc;			// interactpointerのディスクリプタ
			idesc.pointerSolid = soPointer;			// soPointerを設定
			idesc.humanInterface = GetHI(i);		// humaninterfaceを設定
			idesc.springK = 10;						// haptic renderingのバネ係数
			idesc.damperD = 0.05;					// haptic renderingのダンパ係数
		#if SPIDAR
			idesc.posScale = 300;					// soPointerの可動域の設定(～倍)
			idesc.forceScale = 1.0;
		#else
			idesc.posScale = 60;					// soPointerの可動域の設定(～倍)
		#endif
			idesc.localRange = 1.0;					// LocalDynamicsを使う場合の近傍範囲
			if(i==0) idesc.defaultPosition = Posed(1,0,0,0,5,0,0); //ポインタの初期位置
			if(i==1) idesc.defaultPosition = Posed(1,0,0,0,5,0,0);
			GetINScene()->CreateINPointer(idesc);	// interactpointerの作成
		}
	}
}

void FWLDHapticSample::BuildScene(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;
	CDBoxDesc bd;

	/// 床(物理法則に従わない，運動が変化しない)
	{
		/// 剛体(soFloor)の作成
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		/// 形状(shapeFloor)の作成
		bd.boxsize = Vec3f(50, 10, 50);
		CDShapeIf* shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd);
		/// 剛体に形状を付加する
		soFloor->AddShape(shapeFloor);
		soFloor->GetShape(0)->SetVibration(5,80,100);
		soFloor->SetFramePosition(Vec3d(0, -10, 0));
	}

	{	
		PH3ElementBallJointDesc desc;
		//PHBallJointDesc desc;
		{
			desc.poseSocket.Pos()	= Vec3f(0.0f,0.0f , -1.3f);
			desc.posePlug.Pos()		= Vec3f(0.0f,0.0f , 1.3f);
			desc.spring				= 100;
			desc.damper				= 20;
			desc.secondDamper		= 1000;
			desc.hardnessRate		= 1;
			desc.yieldStress		= 0;
			desc.type				=PH3ElementBallJointDesc::deformationType::Plastic;

		}
		PHSolidIf* rootSolid = CreateCapsule(GetSdk());
		rootSolid->SetMass(0.001);
		rootSolid->SetDynamical(false);
		double posy = 15;
		Vec3d pos = Vec3d(0, posy, 0);
		rootSolid->SetFramePosition(pos);
		rootSolid->SetOrientation(Quaterniond().Rot(Rad(-90),Vec3d(1,0,0)));

		//PHTreeNodeIf* root=GetSdk()->GetScene()->GetPHScene()->CreateRootNode(rootSolid,PHRootNodeDesc());
	
		for(int i = 1; i < 6; i++){
			PHSolidIf* nodeSolid = CreateCapsule(GetSdk());
			nodeSolid->SetMass(0.001);
			PHJointIf* joint=GetSdk()->GetScene()->GetPHScene()->CreateJoint(rootSolid, nodeSolid, desc);
			if(i==6){
			//	Balljoint=DCAST(PH3ElementBallJointIf,joint);//naga
				//nodeSolid->SetDynamical(false);
			}
			nodeSolid->SetFramePosition(Vec3d(0, posy - 2.4 * i, 0));
			nodeSolid->SetOrientation(Quaterniond().Rot(Rad(-90),Vec3d(1,0,0)));
			GetSdk()->GetScene()->GetPHScene()->SetContactMode(rootSolid, nodeSolid, PHSceneDesc::MODE_NONE);
			//root=GetSdk()->GetScene()->GetPHScene()->CreateTreeNode(root,nodeSolid);
			rootSolid = nodeSolid;
		}
	}

	//PHSolidIf* sSolid=CreateSphere(GetSdk());
	PHSolidIf* sSolid=CreateBox(GetSdk());
	//sSolid->SetDynamical(false);
	sSolid->SetFramePosition(Vec3d(0.0,0.1,0.0));

}

void FWLDHapticSample::IdleFunc(){
	/// シミュレーションを進める(interactsceneがある場合はそっちを呼ぶ)
	FWAppHaptic::instance->GetINScene()->Step();
	//Balljoint->GetDefomationMode();//naga
	GetGRAdaptee()->PostRedisplay();
}						

void FWLDHapticSample::Reset(){

	//MTimerRelease();

	///// Sdkの初期化，シーンの作成
	//CreateSdk();
	//GetSdk()->Clear();										// SDKの初期化
	//GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());	// Sceneの作成
	//GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.02);	// 刻みの設定
	///// HumanInterfaceの初期化
	//InitHumanInterface();
	///// InteractSceneの作成
	//FWInteractSceneDesc desc;
	//desc.fwScene = GetSdk()->GetScene();					// fwSceneに対するinteractsceneを作る
	//desc.mode = LOCAL_DYNAMICS;								// humaninterfaceのレンダリングモードの設定
	//desc.hdt = 0.001;										// マルチレートの場合の更新[s]
	//CreateINScene(desc);									// interactSceneの作成
	///// 物理シミュレーションする剛体を作成
	//BuildScene();
	//BuildPointer();
	////描画Windowの初期化
	//GetCurrentWin()->SetScene(GetSdk()->GetScene());
	//InitCameraView();

	//MTimerStart();


}

void FWLDHapticSample::Keyboard(int key, int x, int y){
	FWAppHaptic::Keyboard(key , x, y);
	PHSceneIf* phscene=GetSdk()->GetPHSdk()->GetScene(0);

	switch (key) {
		case 'r':
			{
				Reset();
			}
			break;
		case ' ':
			{
				PHSolidIf* solid=CreateBox(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(),solid, PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		case 'v':
			{
				CreateCapsule(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(), PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		case 'b':
			{
				CreateSphere(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(), PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		case 'n':
			{
				CreateRoundCone(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(), PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		case 'm':
			{
				CreatePolyhedron(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(), PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		case 'x':
			{
				CreateWall(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(), PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		case 'z':
			{
				CreateTower(GetSdk());
				for(int i=0;i<GetINScene()->NINPointers();i++){
					phscene->SetContactMode(GetINScene()->GetINPointer(i)->GetPointerSolid(), PHSceneDesc::MODE_NONE);
				}
				DSTR << "Nobj:" << phscene->NSolids() << endl;
			}
			break;
		default:
			break;
	}
}

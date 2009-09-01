#include "FWLDHapticSample.h"
#include <iostream>
#include <sstream>
#include <Framework/FWInteractScene.h>

using namespace std;

FWLDHapticSample::FWLDHapticSample(){
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

	/// SPIDAR4Dを使う場合
	UTRef<HISpidar4If> spg = GetHISdk()->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidar4Desc("SpidarR",Vec4i(1,2,3,4)));
	UTRef<HISpidar4If> spg2 = GetHISdk()->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	spg2->Init(&HISpidar4Desc("SpidarL",Vec4i(5,6,7,8)));
	AddHI(spg);
	AddHI(spg2);
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
		soFloor->SetFramePosition(Vec3d(0, -10, 0));
	}

	/// 箱(物理法則に従う，運動が変化)
	{
		/// 剛体(soBox)の作成
		desc.mass = 0.05;
		desc.inertia = 0.333 * Matrix3d::Unit();
		PHSolidIf* soBox = phscene->CreateSolid(desc);
		/// 形状(shapeBox)の作成
		bd.boxsize = Vec3f(4,4,4);
		CDShapeIf* shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd);
		/// 剛体に形状を付加
		soBox->AddShape(shapeBox);
		soBox->GetShape(0)->SetStaticFriction(2.0);
		soBox->GetShape(0)->SetDynamicFriction(1.0);
		soBox->GetShape(0)->SetVibration(5,80,300);
		soBox->SetFramePosition(Vec3d(0, 10, 0));
	}
}
void FWLDHapticSample::BuildPointer(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;
	/// ポインタ
	{	
		for(int i= 0; i < 2; i++){
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
			idesc.springK = 10;//0.8						// haptic renderingのバネ係数
			idesc.damperD = 0.1;//0.01					// haptic renderingのダンパ係数
			idesc.posScale = 300;					// soPointerの可動域の設定(～倍)
			idesc.localRange = 1.0;					// LocalDynamicsを使う場合の近傍範囲
			if(i==0) idesc.position =Posed(1,0,0,0,5,0,0); //idesc.position.Ori()=Quaterniond::Rot(Rad(90.0),'z');
			if(i==1) idesc.position =Posed(1,0,0,0,-5,0,0);//idesc.position.Ori()=Quaterniond::Rot(Rad(90.0),'z');
			GetINScene()->CreateINPointer(idesc);	// interactpointerの作成
		}
	}
}

void FWLDHapticSample::Keyboard(int key, int x, int y){
	FWAppHaptic::Keyboard(key , x, y);
}

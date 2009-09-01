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
<<<<<<< .mine
#if 0
=======

>>>>>>> .r4330
	/// SPIDARG6を2台使う場合
	UTRef<HISpidarGIf> spg[2];
	for(size_t i = 0; i < 2; i++){
		spg[i] = GetHISdk()->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
		if(i == 0) spg[i]->Init(&HISpidarGDesc("SpidarG6X3R"));
		if(i == 1) spg[i]->Init(&HISpidarGDesc("SpidarG6X3L"));
		AddHI(spg[i]);
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
			if(i==0) idesc.position =Posed(1,0,0,0,5,0,0);	// 初期位置の設定
			if(i==1) idesc.position =Posed(1,0,0,0,-5,0,0);
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
		soFloor->GetShape(0)->SetVibration(10,80,100);
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
		soBox->GetShape(0)->SetVibration(10,80,300);
		soBox->SetFramePosition(Vec3d(0, 10, 0));
	}
}


void FWLDHapticSample::Keyboard(int key, int x, int y){
<<<<<<< .mine
	switch (key) {
		case ESC:
		case 'q':
			exit(0);
			break;
		case 'd':
			bDrawInfo = !bDrawInfo;
			break;
		case 'c':
			{
				MTimerRelease();
				for(int i = 0; i < GetINScene()->NINPointers(); i++){
					GetINScene()->GetINPointer(i)->Calibration();
				}
				MTimerStart();
			}
			break;
		case 'f':
			{
				static bool bf = false;
				bf = !bf;
				for(int i = 0; i < GetINScene()->NINPointers(); i++){
					GetINScene()->GetINPointer(i)->EnableForce(bf);
				}
				if(bf){
					DSTR << "Enable Force Feedback" << std::endl;
				}else{
					DSTR << "Disable Force Feedback" << std::endl;
				}
			}
			break;
		//case 'o':
		//	{
		//		static bool bv = false;
		//		bv = !bv;
		//		for(int i = 0; i < GetINScene()->NINPointers(); i++){
		//			GetINScene()->GetINPointer(i)->EnableVibration(bv);
		//		}
		//		if(bv){
		//			DSTR << "Enable Vibration Feedback" << std::endl;
		//		}else{
		//			DSTR << "Disable Vibration Feedback" << std::endl;
		//		}
		//	}
		//	break;
		default:
			break;
	}
=======
	FWAppHaptic::Keyboard(key , x, y);
>>>>>>> .r4330
}

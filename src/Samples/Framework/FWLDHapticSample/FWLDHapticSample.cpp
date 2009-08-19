#include "FWLDHapticSample.h"
//#include "SampleModel.h"
#include <iostream>
#include <sstream>
#include <GL/glut.h>
#include "Foundation/UTMMTimer.h"
#include <Framework/FWInteractScene.h>

#define ESC 27

using namespace std;

FWLDHapticSample::FWLDHapticSample(){
	bDrawInfo = false;
}

void FWLDHapticSample::InitCameraView(){
	//	Affinef 型が持つ、ストリームから行列を読み出す機能を利用して視点行列を初期化
	std::istringstream issView(
		"((0.9996 0.0107463 -0.0261432 -0.389004)"
		"(-6.55577e-010 0.924909 0.380188 5.65711)"
		"(0.0282657 -0.380037 0.92454 13.7569)"
		"(     0      0      0      1))"
	);
	issView >> cameraInfo.view;
}

void FWLDHapticSample::BuildScene(){
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	PHSolidDesc desc;
	CDBoxDesc bd;

	// 床(物理法則に従わない，運動が変化しない)
	{
		// 剛体(soFloor)の作成
		desc.mass = 1e20f;
		desc.inertia *= 1e30f;
		PHSolidIf* soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
		soFloor->SetDynamical(false);
		soFloor->SetGravity(false);
		// 形状(shapeFloor)の作成
		bd.boxsize = Vec3f(50, 10, 50);
		CDShapeIf* shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加する
		soFloor->AddShape(shapeFloor);
		soFloor->SetFramePosition(Vec3d(0, -10, 0));
	}

	// 箱(物理法則に従う，運動が変化)
	{
		// 剛体(soBox)の作成
		desc.mass = 0.05;
		desc.inertia *= 0.033;
		PHSolidIf* soBox = phscene->CreateSolid(desc);
		// 形状(shapeBox)の作成
		bd.boxsize = Vec3f(2,2,2);
		CDShapeIf* shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd);
		// 剛体に形状を付加
		soBox->AddShape(shapeBox);
		soBox->SetFramePosition(Vec3d(0, 10, 0));
	
		// ポインタ
		soBox = phscene->CreateSolid(desc);
		CDSphereDesc sd;
		sd.radius = 0.5;//1.0;
		CDSphereIf* shapePointer = DCAST(CDSphereIf,  GetSdk()->GetPHSdk()->CreateShape(sd));
		soBox->AddShape(shapePointer);
		soBox->SetDynamical(false);
		GetSdk()->GetScene()->GetPHScene()->SetContactMode(soBox, PHSceneDesc::MODE_NONE);
		FWInteractPointerDesc idesc;
		idesc.pointerSolid = soBox;;
		idesc.humanInterface = GetHI(0); 
		idesc.springK = 10;
		idesc.damperD = 0.0;
		idesc.posScale = 300;
		idesc.localRange = 0.7 * 10;
		GetInteractScene()->CreateInteractPointer(idesc);
	}
}

void FWLDHapticSample::Keyboard(int key, int x, int y){
	switch (key) {
		case ESC:
		case 'q':
			Exit();
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			bDrawInfo = !bDrawInfo;
			break;
		//case '1':
		//	DSTR << "box" << std::endl;
		//	CreateBox(GetSdk());
		//	break;
		//case '2':
		//	DSTR << "sphere" << std::endl;
		//	CreateSphere(GetSdk());
		//	break;
		//case '3':
		//	DSTR << "capsule" << std::endl;
		//	CreateCapsule(GetSdk());
		//	break;
		//case '4':
		//	DSTR << "roundcone" << std::endl;
		//	CreateRoundCone(GetSdk());
		//	break;
		default:
			break;
	}
}
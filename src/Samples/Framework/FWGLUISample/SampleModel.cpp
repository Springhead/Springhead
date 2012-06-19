/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "SampleModel.h"

PHSolidIf* CreateBox(FWSdkIf* fwSdk){
	// 剛体の作成
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia *= 0.033;
	PHSolidIf* soBox = fwSdk->GetScene(0)->GetPHScene()->CreateSolid(desc);
	// 形状の作成
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2,2,2);
	CDShapeIf* shapeBox = fwSdk->GetPHSdk()->CreateShape(bd);
	// 剛体に形状を付加
	soBox->AddShape(shapeBox);
	soBox->SetFramePosition(Vec3d(0, 10, 0));
	return soBox;
}

PHSolidIf* CreateSphere(FWSdkIf* fwSdk){
	// 剛体の作成
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia *= 0.03;
	PHSolidIf* soSphere = fwSdk->GetScene(0)->GetPHScene()->CreateSolid(desc);
	// 形状の作成
	CDSphereDesc sd;
	sd.radius = 2.0;
	CDShapeIf* shapeSphere = fwSdk->GetPHSdk()->CreateShape(sd);
	// 剛体に形状を付加する
	soSphere->AddShape(shapeSphere);
	soSphere->SetFramePosition(Vec3d(0, 15, 0));
	return soSphere;
}

PHSolidIf* CreateCapsule(FWSdkIf* fwSdk){
	// 剛体の作成
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia *= 1;
	PHSolidIf*  soCapsule = fwSdk->GetScene(0)->GetPHScene()->CreateSolid(desc);
	// 形状の作成
	CDCapsuleDesc cd;
	cd.length = 3.0;
	cd.radius = 1.0;
	CDShapeIf* shapeCapsule = fwSdk->GetPHSdk()->CreateShape(cd);
	// 剛体に形状を付加する
	soCapsule->AddShape(shapeCapsule);
	soCapsule->SetFramePosition(Vec3d(0, 15, 0));
	return soCapsule;
}

PHSolidIf* CreateRoundCone(FWSdkIf* fwSdk){
	// 剛体の作成
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia *= 1;
	PHSolidIf*  soRCone = fwSdk->GetScene(0)->GetPHScene()->CreateSolid(desc);
	// 形状の作成
	CDRoundConeDesc rd;
	rd.length = 3.0;
	rd.radius[0] = 1.5;
	rd.radius[1] = 0.5;
	CDShapeIf* shapeRCone = fwSdk->GetPHSdk()->CreateShape(rd);
	// 剛体に形状を付加する
	soRCone->AddShape(shapeRCone);
	soRCone->SetFramePosition(Vec3d(0, 15, 0));
	return soRCone;
}
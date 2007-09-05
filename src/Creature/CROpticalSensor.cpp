/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CROpticalSensor.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
bool CROpticalSensor::IsVisible(PHSolidIf* solid){
	return true;
}

IF_OBJECT_IMP(CROpticalSensor, CRSensor);

void CROpticalSensor::Init(){
	CRSensor::Init();

	/// 依存する他オブジェクトの取得
	soLEye = creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_EYE);
	soREye = creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_EYE);
	internalScene = creature->GetInternalScene();

	/// InternalSceneの組み立て
	for(int i=0; i<phScene->NSolids(); i++) {
		PHSolidIf* solid = DCAST(PHSceneIf, creature->GetScene())->GetSolids()[i];
		// internalScene->CreateObject(ほにゃらら)
	}
}

void CROpticalSensor::Step(){
	CRSensor::Init();

	Vec3f dirL = soLEye->GetPose().Ori() * Vec3f(0,0,-1);
	Vec3f dirR = soREye->GetPose().Ori() * Vec3f(0,0,-1);
	Vec3f visualAxis = ((dirL + dirR) * 0.5f).unit();

	for(int i=0; i<phScene->NSolids(); i++) {
		PHSolidIf* solid = phScene->GetSolids()[i];
		if (IsVisible(solid)) {
			Vec3f position    = solid->GetPose().Pos();
			Vec3f velocity    = solid->GetVelocity();
			Vec3f angVelocity = solid->GetAngularVelocity();

			float r = 1.0f + (position - ((soLEye->GetPose().Pos() + soREye->GetPose().Pos())*0.5)).norm();

			Vec3f dir = (position - ((soLEye->GetPose().Pos() + soREye->GetPose().Pos())*0.5)).unit();
			Vec3f letinalPos = position - (visualAxis * PTM::dot(visualAxis,position));
			
			float trnAmmount = (velocity - (visualAxis * PTM::dot(velocity,visualAxis))).norm() * 3.0f / r;
			float divAmmount = abs(PTM::dot(velocity,visualAxis)) * 2.0f / r;
			float rotAmmount = abs(PTM::dot(angVelocity,visualAxis));

			CRISAttractiveObjectIf* ao = DCAST(CRISAttractiveObjectIf, internalScene->FindObject(solid, Vec3f(0,0,0)));
			ao->SetAttractiveness(ao->GetAttractiveness() + (trnAmmount + divAmmount + rotAmmount));
		}
	}
}
}

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
IF_OBJECT_IMP(CROpticalSensor, CRSensor);

void CROpticalSensor::Init(){
	CRSensor::Init();

	/// 依存する他オブジェクトの取得
	soLEye = creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_EYE);
	soREye = creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_EYE);
	internalScene = creature->GetInternalScene();

	/// InternalSceneの組み立て
	for (int i=0; i<phScene->NSolids(); i++) {
		CRISAttractiveObjectDesc desc;
		{
			desc.solid        = DCAST(PHSceneIf, creature->GetScene())->GetSolids()[i];
			desc.position     = Vec3f(0,0,0);
			desc.bottomupAttr = 0.0f;
		}
		internalScene->CreateInternalSceneObject(desc);
	}

	/// 自己に属する剛体をあらかじめ取得
	for (int i=0; i<CRHingeHumanBodyDesc::SO_NSOLIDS; i++) {
		selfSolids.insert(creature->GetBody()->GetSolid(i));
	}
}

void CROpticalSensor::Step(){
	CRSensor::Init();

	Vec3f dirL = soLEye->GetPose().Ori() * Vec3f(0,0,-1);
	Vec3f dirR = soREye->GetPose().Ori() * Vec3f(0,0,-1);
	Vec3f visualAxis = ((dirL + dirR) * 0.5f).unit();

	for(int i=0; i<phScene->NSolids(); i++) {
		PHSolidIf* solid = phScene->GetSolids()[i];
		if (IsVisible(solid) && !IsSelfSolid(solid)) {
			CRISAttractiveObjectIf* ao = DCAST(CRISAttractiveObjectIf, internalScene->FindObject(solid, Vec3f(0,0,0)));
			if (ao) {
				// Where-Howストリーム
				Vec3f position    = solid->GetPose().Pos();
				Vec3f velocity    = solid->GetVelocity();
				Vec3f angVelocity = solid->GetAngularVelocity();

				float r = 1.0f + (position - ((soLEye->GetPose().Pos() + soREye->GetPose().Pos())*0.5)).norm();

				Vec3f dir = (position - ((soLEye->GetPose().Pos() + soREye->GetPose().Pos())*0.5)).unit();
				Vec3f letinalPos = position - (visualAxis * PTM::dot(visualAxis,position));
				
				float trnAmmount = (velocity - (visualAxis * PTM::dot(velocity,visualAxis))).norm() * 3.0f / r;
				float divAmmount = abs(PTM::dot(velocity,visualAxis)) * 2.0f / r;
				float rotAmmount = abs(PTM::dot(angVelocity,visualAxis));

				ao->AddBottomupAttr(trnAmmount + divAmmount + rotAmmount);

				// Whatストリーム
				if (IsInCenter(solid)) {
					ao->DecUncertainty();
				}
			}
		}
	}
}

Vec2d CROpticalSensor::Vec3ToAngle(Vec3d v){
	double D1 = sqrt(v.Y()*v.Y()+v.Z()*v.Z());
	double D2 = sqrt(v.X()*v.X()+v.Z()*v.Z());
	return(Vec2d( atan2( v.Y()/D1, -v.Z()/D1), atan2(-v.X()/D2, -v.Z()/D2) ));
}

bool CROpticalSensor::IsInside(PHSolidIf* solid, double rangeIn, double rangeOut, double rangeVert){
	Vec2d angleL = Vec3ToAngle(soLEye->GetPose().Ori().Inv() * (solid->GetPose().Pos()-soLEye->GetPose().Pos()));
	if ((-rangeIn<angleL.Y() && angleL.Y()<rangeOut) && (-rangeVert<angleL.X() && angleL.X()<rangeVert)) {
		return true;
	}

	Vec2d angleR = Vec3ToAngle(soREye->GetPose().Ori().Inv() * (solid->GetPose().Pos()-soREye->GetPose().Pos()));
	if ((-rangeOut<angleR.Y() && angleR.Y()<rangeIn) && (-rangeVert<angleR.X() && angleR.X()<rangeVert)) {
		return true;
	}

	return false;
}

bool CROpticalSensor::IsVisible(PHSolidIf* solid){
	return IsInside(solid, Rad(30), Rad(50), Rad(45));
}

bool CROpticalSensor::IsInCenter(PHSolidIf* solid){
	return IsInside(solid, Rad(5), Rad(5), Rad(5));
}

bool CROpticalSensor::IsSelfSolid(PHSolidIf* solid){
	return(!(selfSolids.find(solid)==selfSolids.end()));
}
}

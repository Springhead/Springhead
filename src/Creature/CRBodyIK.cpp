/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBodyIK.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 制御点
IF_OBJECT_IMP(CRIKControl, SceneObject);

IF_OBJECT_IMP(CRIKControlPos, CRIKControl);
IF_OBJECT_IMP(CRIKControlOri, CRIKControl);
IF_OBJECT_IMP(CRIKControlForce, CRIKControl);

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 制御対象
IF_OBJECT_IMP(CRIKMovable, SceneObject);

Matrix3d CRIKMovable::CalcJacobian(CRIKControlIf* control){
	return Matrix3d();
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovableSolidPos, CRIKMovable);

Matrix3d CRIKMovableSolidPos::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}

	CRIKControlForceIf* cpForce;
	if (cpForce = DCAST(CRIKControlForceIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovableSolidOri, CRIKMovable);

Matrix3d CRIKMovableSolidOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}

	CRIKControlForceIf* cpForce;
	if (cpForce = DCAST(CRIKControlForceIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovableBallJointOri, CRIKMovable);

Matrix3d CRIKMovableBallJointOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}

	CRIKControlForceIf* cpForce;
	if (cpForce = DCAST(CRIKControlForceIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovableBallJointTorque, CRIKMovable);

Matrix3d CRIKMovableBallJointTorque::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}

	CRIKControlForceIf* cpForce;
	if (cpForce = DCAST(CRIKControlForceIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovable3HingeJointOri, CRIKMovable);

Matrix3d CRIKMovable3HingeJointOri::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}

	CRIKControlForceIf* cpForce;
	if (cpForce = DCAST(CRIKControlForceIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
}

// --- --- --- --- ---
IF_OBJECT_IMP(CRIKMovable3HingeJointTorque, CRIKMovable);

Matrix3d CRIKMovable3HingeJointTorque::CalcJacobian(CRIKControlIf* control){
	CRIKControlPosIf* cpPos;
	if (cpPos = DCAST(CRIKControlPosIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
 
	CRIKControlOriIf* cpOri;
	if (cpOri = DCAST(CRIKControlOriIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}

	CRIKControlForceIf* cpForce;
	if (cpForce = DCAST(CRIKControlForceIf,control)){
		return Matrix3d(); /// 適切なのを返す
	}
}

}

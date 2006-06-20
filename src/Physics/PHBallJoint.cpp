/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHBallJoint
IF_OBJECT_IMP(PHBallJoint, PHJoint1D)

void PHBallJoint::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHBallJointDesc& descBall = (const PHBallJointDesc&)desc;
	max_angle = descBall.max_angle;
	SetMotorTorque(descBall.torque);
}

Quaterniond PHBallJoint::GetPosition(){
	return qjrel;
}

Vec3d PHBallJoint::GetVelocity(){
	return wjrel;
}

void PHBallJoint::CompConstraintJacobian(){
	dim_d = 3;
	dim_c = 3;
	Ad.clear();
	Ac.clear();
	for(int i = 0; i < 2; i++){
		Jdv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jdv[i].SUBMAT(3, 0, 3, 3) = Jwv[i];
		Jdw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		Jdw[i].SUBMAT(3, 0, 3, 3) = Jww[i];
		Jcv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jcw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		if(solid[i]->solid->IsDynamical()){
			Tdv[i] = Jdv[i] * solid[i]->minv;
			Tdw[i] = Jdw[i] * solid[i]->Iinv;
			Tcv[i].SUBMAT(0, 0, 3, 3) = Tdv[i].SUBMAT(0, 0, 3, 3);
			Tcw[i].SUBMAT(0, 0, 3, 3) = Tdw[i].SUBMAT(0, 0, 3, 3);
			for(int j = 0; j < 6; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
			Ac.SUBVEC(0, 3) += Ad.SUBVEC(0, 3);
		}
	}
}

void PHBallJoint::CompBias(double dt){
	
}

void PHBallJoint::CompError(double dt){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}

void PHBallJoint::ProjectionDynamics(double& f, int k){
	
}

void PHBallJoint::ProjectionCorrection(double& F, int k){
	
}

}

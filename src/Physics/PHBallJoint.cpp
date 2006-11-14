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
IF_OBJECT_IMP(PHBallJoint, PHJoint)

void PHBallJoint::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHBallJointDesc& descBall = (const PHBallJointDesc&)desc;
	max_angle = descBall.max_angle;
	SetMotorTorque(descBall.torque);
}

void PHBallJoint::CompBias(){
	double dtinv = 1.0 / scene->GetTimeStep();
	db.v = engine->correctionRate * Xjrel.r * dtinv;
	db.w.clear();
}

/*void PHBallJoint::CompError(double dt){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}*/

void PHBallJoint::Projection(double& f, int k){
	
}

void PHBallJointNode::CompJointJacobian(){

}
void PHBallJointNode::CompJointCoriolisAccel(){

}
void PHBallJointNode::CompRelativePosition(){

}
void PHBallJointNode::CompRelativeVelocity(){

}
	
}

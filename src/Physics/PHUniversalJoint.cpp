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
// PHUniversalJoint
IF_OBJECT_IMP(PHUniversalJoint, PHJoint);

void PHUniversalJoint::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHUniversalJointDesc& descUniversal = (const PHUniversalJointDesc&)desc;
}

void PHUniversalJoint::CompBias(){

}

/*void PHUniversalJoint::CompError(double dt){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}*/

void PHUniversalJoint::Projection(double& f, int k){
	
}

void PHUniversalJointNode::CompJointJacobian(){

}
void PHUniversalJointNode::CompJointCoriolisAccel(){

}
void PHUniversalJointNode::CompRelativePosition(){

}
void PHUniversalJointNode::CompRelativeVelocity(){

}

}

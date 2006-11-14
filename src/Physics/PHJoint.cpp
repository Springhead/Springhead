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
// PHJoint
IF_OBJECT_IMP_ABST(PHJoint, PHConstraint);

//----------------------------------------------------------------------------
// PHJoint1D
IF_OBJECT_IMP_ABST(PHJoint1D, PHJoint);

PHJoint1D::PHJoint1D(){
	position = velocity = torque = 0.0;
	fill(constr, constr + 6, true);
}	

void PHJoint1D::SetDesc(const PHJointDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHJoint1DDesc& desc1D = (const PHJoint1DDesc&)desc;
	lower = desc1D.lower;
	upper = desc1D.upper;
	spring = desc1D.spring;
	origin = desc1D.origin;
	damper = desc1D.damper;
	SetMotorTorque(desc1D.torque);
}

void PHJoint1D::CompDof(){
	on_lower = on_upper = false;
	if(lower < upper){
		double theta = GetPosition();
		on_lower = (theta <= lower);
		on_upper = (theta >= upper);
	}
	if(on_lower || on_upper || mode == MODE_VELOCITY || spring != 0.0 || damper != 0.0){
		constr[axis_index] = true;
		//dim_d = 6;
		//dim_c = 5;
	}
	else{
		constr[axis_index] = false;
		//dim_d = 5;
		//dim_c = 5;
	}
}
	
}

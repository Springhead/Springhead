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
}	

void PHJoint1D::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHJoint1DDesc& desc1D = (const PHJoint1DDesc&)desc;
	lower = desc1D.lower;
	upper = desc1D.upper;
	spring = desc1D.spring;
	origin = desc1D.origin;
	damper = desc1D.damper;
	SetMotorTorque(desc1D.torque);
}

void PHJoint1D::SetConstrainedIndex(bool* con){
	//可動範囲
	onLower = onUpper = false;
	if(lower < upper){
		double theta = GetPosition();
		onLower = (theta <= lower);
		onUpper = (theta >= upper);
	}
	// 各自由度を拘束するかどうか
	//  関節軸に対応する自由度は可動範囲にかかっている場合，バネ・ダンパが設定されている場合にtrue
	//  それ以外の自由度はABA関節ではない場合にtrue	
	for(int i = 0; i < 6; i++){
		if(i == axisIndex[0])
			 con[i] = (onLower || onUpper || mode == MODE_VELOCITY || spring != 0.0 || damper != 0.0);
		else con[i] = true;
	}
}

void PHJoint1D::Projection(double& f, int k){
	if(k == axisIndex[0]){
		if(onLower)
			f = max(0.0, f);
		if(onUpper)
			f = min(0.0, f);
	}
}
	
}

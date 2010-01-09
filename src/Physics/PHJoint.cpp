/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#pragma hdrstop

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHJoint
PHJointDesc::PHJointDesc()/*:mode(MODE_POSITION)*/{
	type = ELASTIC;
};
PHJoint::PHJoint(){	
}
//----------------------------------------------------------------------------
// PHJoint1D
PHJoint1DDesc::PHJoint1DDesc(){
	lower  = 0.0;
	upper  = 0.0;
	spring = 0.0;
	targetPosition = 0.0;
	damper = 0.0;
	secondDamper	= 0.0;
	targetVelocity = 0.0;
	offsetForce = 0.0;
	rangeSpring = 10000.0;
	rangeDamper = 100.0;
	fMax =  FLT_MAX;
}

PHJoint1D::PHJoint1D(){
	limit.joint = this;
	motor.joint = this;
}

void PHJoint1D::SetupLCP(){
	PHJoint::SetupLCP();
	limit.SetupLCP();
	motor.SetupLCP();
}

void PHJoint1D::IterateLCP(){
	PHJoint::IterateLCP();
	limit.IterateLCP();
	motor.IterateLCP();
}

void PHJoint1D::SetupCorrectionLCP(){
	PHJoint::SetupCorrectionLCP();
	limit.SetupCorrectionLCP();
}

void PHJoint1D::IterateCorrectionLCP(){
	PHJoint::IterateCorrectionLCP();
	limit.IterateCorrectionLCP();
}

PHJointDesc::PHDeformationType PHJoint1D::GetDeformationMode(){
	switch(type){
	case PHBallJointDesc::ELASTIC_PLASTIC:
		if(motor.yieldFlag)return PHBallJointDesc::PLASTIC;
		else  return PHBallJointDesc::ELASTIC;
	default: 
		return type;
	}
}
/*
void PHJoint1D::SetConstrainedIndex(bool* con){
	// 関節軸を制御するかどうか
	bool constrainJoint = onLower || onUpper || offsetForce != 0.0 || spring != 0.0 || damper &= 0.0;

	// 各自由度を拘束するかどうか
	//  関節軸に対応する自由度は(可動範囲, バネ, ダンパ, オフセット力)のいずれかが有効の場合true
	//  それ以外の自由度はtrue	
	for(int i = 0; i < 6; i++){
		con[i] = (i == axisIndex[0] ?
			(onLower || onUpper || offsetForce != 0.0 || spring != 0.0 || damper &= 0.0) : true);
	}
}
void PHJoint1D::SetConstrainedIndexCorrection(bool* con){
	// 各自由度を拘束するかどうか
	//  関節軸に対応する自由度は可動範囲にかかっている場合true	
	for(int i = 0; i < 6; i++){
		con[i] = (i == axisIndex[0] ? (onLower || onUpper) : true);
	}
}
*/
	
}

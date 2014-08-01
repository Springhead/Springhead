/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;
namespace Spr{;

//-------------------------------------------------------------------------------------------------

bool PH1DJoint::AddChildObject(ObjectIf* o) {
	PH1DJointLimit* l = o->Cast();
	PH1DJointMotor* m = o->Cast();
	if(l){
		limit = l;
		limit->joint = this;
		return true;
	}
	if(m){
		motor = m;
		motor->joint = this;
		return true;
	}
	return PHConstraint::AddChildObject(o);
}

size_t PH1DJoint::NChildObject() const {
	return (limit?1:0) + (motor?1:0) + PHConstraint::NChildObject();
}

ObjectIf* PH1DJoint::GetChildObject(size_t i) {
	if(limit){
		if(i == 0)
			return limit->Cast();
		i--;
	}
	if(motor){
		if(i == 0)
			return motor->Cast();
		i--;
	}
	return PHConstraint::GetChildObject(i);
}

PH1DJointLimitIf* PH1DJoint::CreateLimit(const PH1DJointLimitDesc& desc) {
	PH1DJointLimitIf* l = GetScene()->CreateObject(PH1DJointLimitIf::GetIfInfoStatic(), &desc)->Cast();
	if(l)
		AddChildObject(l);
	return l;
}

//-------------------------------------------------------------------------------------------------

PHPointToPointMate::PHPointToPointMate(const PHPointToPointMateDesc& desc){
	SetDesc(&desc);
	movableAxes.Enable(3);
	movableAxes.Enable(4);
	movableAxes.Enable(5);
}
void PHPointToPointMate::CompBias(){
	double dtinv = GetScene()->GetTimeStepInv();
	db.v() = Xjrel.r * dtinv;
	db *= engine->velCorrectionRate;
}

//-------------------------------------------------------------------------------------------------

PHPointToLineMate::PHPointToLineMate(const PHPointToLineMateDesc& desc){
	SetDesc(&desc);
	movableAxes.Enable(2);
	movableAxes.Enable(3);
	movableAxes.Enable(4);
	movableAxes.Enable(5);
}
void PHPointToLineMate::CompBias(){
	double dtinv = GetScene()->GetTimeStepInv();
	db.v() = Xjrel.r * dtinv + vjrel.v();
	db *= engine->velCorrectionRate;
}

//-------------------------------------------------------------------------------------------------

PHPointToPlaneMate::PHPointToPlaneMate(const PHPointToPlaneMateDesc& desc){
	SetDesc(&desc);
	movableAxes.Enable(0);
	movableAxes.Enable(1);
	movableAxes.Enable(3);
	movableAxes.Enable(4);
	movableAxes.Enable(5);
	range = desc.range;
}
void PHPointToPlaneMate::SetupAxisIndex() {
	PHConstraint::SetupAxisIndex();

	if(range[0] > range[1])
		return;
	
	double z = Xjrel.r.z;

	if(range[0] == range[1]){
		axes.Enable(2);
		diff = z - range[0];
		return;
	}

	if(z <= range[0]){
		onLower = true;
		diff = z - range[0];
		axes.Enable(2);
		fMinDt[2] = 0.0;
	}
	else if(z >= range[1]){
		onUpper = true;
		diff = z - range[1];
		axes.Enable(2);
		fMaxDt[2] = 0.0;
	}
	else{
		axes.Disable(2);
	}

	targetAxes = axes;
}

void PHPointToPlaneMate::CompBias(){
	double dtinv = GetScene()->GetTimeStepInv();
	db[2] += engine->velCorrectionRate * (diff * dtinv);
}

//-------------------------------------------------------------------------------------------------

PHLineToLineMate::PHLineToLineMate(const PHLineToLineMateDesc& desc){
	SetDesc(&desc);
	movableAxes.Enable(2);
	movableAxes.Enable(3);
	movableAxes.Enable(4);
	movableAxes.Enable(5);
}
void PHLineToLineMate::CompBias(){
	double dtinv = GetScene()->GetTimeStepInv();
	Vec3d diff = Xjrel.r * dtinv + vjrel.v();
	Quaterniond qarc;
	qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1));
	db.v() = Vec3d(diff.x, diff.y, 0.0);
	db.w() = -(qarc.Theta() * dtinv) * qarc.Axis() + vjrel.w();
	db *= engine->velCorrectionRate;
}

//-------------------------------------------------------------------------------------------------

PHPlaneToPlaneMate::PHPlaneToPlaneMate(const PHPlaneToPlaneMateDesc& desc){
	SetDesc(&desc);
	movableAxes.Enable(0);
	movableAxes.Enable(1);
	movableAxes.Enable(5);
}
void PHPlaneToPlaneMate::CompBias(){
	double dtinv = GetScene()->GetTimeStepInv();
	Quaterniond qarc;
	qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1));
	db.v() = Xjrel.r * dtinv + vjrel.v();
	db.w() = -(qarc.Theta() * dtinv) * qarc.Axis() + vjrel.w();
	db *= engine->velCorrectionRate;
}

}


/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
PHJointDesc::PHJointDesc():mode(MODE_POSITION){
};
PHJoint::PHJoint(){	
}
//----------------------------------------------------------------------------
// PHJoint1D
PHJoint1DDesc::PHJoint1DDesc():lower(0.0), upper(0.0), spring(0.0), origin(0.0), damper(0.0), desiredVelocity(0.0), torque(0.0), offsetForce(0.0), fMax(FLT_MAX), fMin(-FLT_MAX){
}

PHJoint1D::PHJoint1D(){
	fMaxDt = DBL_MAX;
	fMinDt = -DBL_MAX;
	onLower = false;
	onUpper = false;
}	

void PHJoint1D::AfterSetDesc(){
	if (GetScene()){
		fMinDt = fMin * GetScene()->GetTimeStep();
		fMaxDt = fMax * GetScene()->GetTimeStep();
	}
	PHJointND<1>::AfterSetDesc();
}

void PHJoint1D::SetConstrainedIndex(bool* con){
	//可動範囲．毎ステップfalseに初期化して，その後の判定でtrueにする
	onLower = onUpper = false;
	if(lower < upper){
		double theta = GetPosition();
		onLower = (theta <= lower);
		onUpper = (theta >= upper);
	//	DSTR << onUpper << endl;
	}
	// 各自由度を拘束するかどうか
	//  関節軸に対応する自由度は可動範囲にかかっている場合，バネ・ダンパが設定されている場合にtrue
	//  それ以外の自由度はABA関節ではない場合にtrue	
	for(int i = 0; i < 6; i++){
		if(i == axisIndex[0])
			con[i] = (onLower || onUpper || mode == PHJointDesc::MODE_VELOCITY || mode == PHJointDesc::MODE_TRAJ || spring != 0.0 || damper != 0.0);
		else con[i] = true;
	}
}
void PHJoint1D::SetConstrainedIndexCorrection(bool* con){
	// 各自由度を拘束するかどうか
	//  関節軸に対応する自由度は可動範囲にかかっている場合true	
	for(int i = 0; i < 6; i++){
		if(i == axisIndex[0])
			 con[i] = (onLower || onUpper);
		else con[i] = true;
	}
}

void PHJoint1D::Projection(double& f, int k){
	if(k == axisIndex[0]){
		if(onLower)
			f = max(0.0, f);
		if(onUpper)
			f = min(0.0, f);
		if(fMaxDt < f)
			f = max(fMaxDt, f);
		if(f < fMinDt)
			f = min(fMaxDt, f);
	}
}

void PHJoint1D::ProjectionCorrection(double& F, int k){
	if(k == axisIndex[0]){
		if(onLower)
			F = max(0.0, F);
		if(onUpper)
			F = min(0.0, F);
	}
}

	
}

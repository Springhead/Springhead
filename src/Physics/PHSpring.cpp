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

PHSpringDesc::PHSpringDesc(){
	springOri = damperOri = 0.0;
}

//----------------------------------------------------------------------------
// PHSpring
PHSpring::PHSpring(const PHSpringDesc& desc){
	SetDesc(&desc);
}

void PHSpring::SetDesc(const void* desc){
	PHConstraint::SetDesc(desc);
	const PHSpringDesc& descSpring = *(const PHSpringDesc*)desc;
	spring = descSpring.spring;
	damper = descSpring.damper;
	springOri = descSpring.springOri;
	damperOri = descSpring.damperOri;
}

void PHSpring::SetConstrainedIndex(bool* con){
	for(int i=0; i<3; ++i) con[i] = (damper[i] != 0.0 || spring[i] != 0.0);
	con[3] = con[4] = con[5] = (damperOri != 0.0 || springOri != 0.0);
}
void PHSpring::SetConstrainedIndexCorrection(bool* con){
	con[0] = con[1] = con[2] = con[3] = con[4] = con[5] = false;
}

void PHSpring::CompBias(){
	//rjrel
	double dtinv = 1.0 / GetScene()->GetTimeStep(), tmp;
	for(int i = 0; i < 3; i++){
		if (!constr[i]) continue;
		tmp = 1.0 / (damper[i] + spring[i] * GetScene()->GetTimeStep());
		dA[i] = tmp * dtinv;
		db[i] = spring[i] * Xjrel.r[i] * tmp;
	}

	// Žp¨‚É‘Î‚·‚éƒoƒl
	if(springOri != 0.0 || damperOri != 0.0){
		Quaterniond diff =  Xjrel.q; // * targetPosition.Inv();
		Vec3d prop = diff.RotationHalf();
		double tmpInv = damperOri + springOri * GetScene()->GetTimeStep();
		if (tmpInv > 1e-30){
			tmp = 1.0/tmpInv;
			dA.w() = Vec3d(tmp * dtinv, tmp * dtinv, tmp * dtinv);
			db.w() = springOri * (prop) * tmp;
		}
	}
}

}

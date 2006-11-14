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
// PHSpring
IF_OBJECT_IMP(PHSpring, PHConstraint)

void PHSpring::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHSpringDesc& descSpring = (const PHSpringDesc&)desc;
	spring = descSpring.spring;
	damper = descSpring.damper;
}

void PHSpring::CompBias(){
	//rjrel
	double dtinv = 1.0 / scene->GetTimeStep(), tmp;
	for(int i = 0; i < 3; i++){
		if(damper[i] == 0.0 && spring[i] == 0.0){
			constr[i] = false;
			continue;
		}
		constr[i] = true;
		tmp = 1.0 / (damper[i] + spring[i] * scene->GetTimeStep());
		dA.v[i] = tmp * dtinv;
		db.v[i] = spring[i] * Xjrel.r[i] * tmp;
	}
	//DSTR << "spring" << fv << rjrel.y * spring.y * dt << endl;
}

}

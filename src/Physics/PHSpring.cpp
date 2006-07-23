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

void PHSpring::CompJacobian(bool bCompAngular){
	PHConstraint::CompJacobian(bCompAngular);
/*	double k = 0.4;
	for(int i = 0; i < 2; i++){
		Jvv[i] *= k;
		Jvw[i] *= k;
		Jwv[i] *= k;
		Jww[i] *= k;
	}
*/
}
void PHSpring::SetDesc(const PHConstraintDesc& desc){
	PHConstraint::SetDesc(desc);
	const PHSpringDesc& descSpring = (const PHSpringDesc&)desc;
	spring = descSpring.spring;
	damper = descSpring.damper;
}

/*void PHSpring::CompConstraintJacobian(){
	dim_d = 3;
	dim_c = 0;
	Ad.clear();
	Ac.clear();
	for(int i = 0; i < 2; i++){
		Jdv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		//Jdv[i].SUBMAT(3, 0, 3, 3) = Jwv[i];
		Jdw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		//Jdw[i].SUBMAT(3, 0, 3, 3) = Jww[i];
		if(solid[i]->solid->IsDynamical()){
			Tdv[i] = Jdv[i] * solid[i]->minv;
			Tdw[i] = Jdw[i] * solid[i]->Iinv;
			for(int j = 0; j < dim_d; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
		}
	}
	f.clear();
	F.clear();
}*/

void PHSpring::CompBias(double dt, double correction_rate){
	//rjrel
	double dtinv = 1.0 / dt, tmp;
	for(int i = 0; i < 3; i++){
		if(damper[i] == 0.0 && spring[i] == 0.0){
			constr[i] = false;
			continue;
		}
		constr[i] = true;
		tmp = 1.0 / (damper[i] + spring[i] * dt);
		dAv[i] = tmp * dtinv;
		dbv[i] = spring[i] * rjrel[i] * tmp;
	}
	DSTR << "spring" << fv << rjrel.y * spring.y * dt << endl;
}

}

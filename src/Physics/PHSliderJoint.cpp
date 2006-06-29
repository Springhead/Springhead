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
// PHSliderJoint
IF_OBJECT_IMP(PHSliderJoint, PHJoint1D)

double PHSliderJoint::GetPosition(){
	//DSTR<<rjrel.z <<endl;
	return rjrel.z;
}

double PHSliderJoint::GetVelocity(){
	return vjrel.z;
}

/*void PHSliderJoint::CompConstraintJacobian(){
	CompDof();
	Ad.clear();
	Ac.clear();
	for(int i = 0; i < 2; i++){
		Jdv[i].SUBMAT(0, 0, 2, 3) = Jvv[i].SUBMAT(0, 0, 2, 3);
		Jdv[i].SUBMAT(2, 0, 3, 3) = Jwv[i];
		Jdv[i].row(5) = Jvv[i].row(2);
		Jdw[i].SUBMAT(0, 0, 2, 3) = Jvw[i].SUBMAT(0, 0, 2, 3);
		Jdw[i].SUBMAT(2, 0, 3, 3) = Jww[i];
		Jdw[i].row(5) = Jvw[i].row(2);

		Jcv[i].SUBMAT(0, 0, 2, 3) = Jvv[i].SUBMAT(0, 0, 2, 3);
		Jcv[i].SUBMAT(2, 0, 3, 3) = Jqv[i];
		Jcv[i].row(5) = Jvv[i].row(2);
		Jcw[i].SUBMAT(0, 0, 2, 3) = Jvw[i].SUBMAT(0, 0, 2, 3);
		Jcw[i].SUBMAT(2, 0, 3, 3) = Jqw[i];
		Jcw[i].row(5) = Jvw[i].row(2);

		if(solid[i]->solid->IsDynamical()){
			Tdv[i] = Jdv[i] * solid[i]->minv;
			Tdw[i] = Jdw[i] * solid[i]->Iinv;
			Tcv[i].SUBMAT(0, 0, 2, 3) = Tdv[i].SUBMAT(0, 0, 2, 3);
			Tcv[i].SUBMAT(2, 0, 3, 3) = Jqv[i] * solid[i]->minv;
			Tcv[i].row(5) = Tdv[i].row(5);
			Tcw[i].SUBMAT(0, 0, 2, 3) = Tdw[i].SUBMAT(0, 0, 2, 3);
			Tcw[i].SUBMAT(2, 0, 3, 3) = Jqw[i] * solid[i]->Iinv;
			Tcw[i].row(5) = Tdw[i].row(5);
			for(int j = 0; j < 6; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
			Ac.SUBVEC(0, 2) += Ad.SUBVEC(0, 2);
			for(int j = 2; j < 5; j++)
				Ac[j] += Jcv[i].row(j) * Tcv[i].row(j) + Jcw[i].row(j) * Tcw[i].row(j);
			Ac[5] += Ad[5];
		}
	}
}*/

void PHSliderJoint::CompBias(double dt, double correction_rate){
	CompDof();
	double dtinv = 1.0 / dt;
	Vec3d v = correction_rate * rjrel * dtinv;
	bv.x += v.x;
	bv.y += v.y;
	if(mode == MODE_VELOCITY){
		bv.z -= vel_d;
	}
	else if(spring != 0.0 || damper != 0.0){
		double diff = GetPosition() - origin;
		double tmp = 1.0 / (damper + spring * dt);
		Av.z += tmp / dt;
		bv.z += spring * (diff) * tmp;
	}
	bw += (correction_rate * qjrel.Theta() * dtinv) * qjrel.Axis();
}

/*void PHSliderJoint::CompError(double dt){
	B[0] = rjrel.x;
	B[1] = rjrel.y;
	B[2] = qjrel.x;
	B[3] = qjrel.y;
	B[4] = qjrel.z;
	if(on_upper)
		B[5] = rjrel.z - upper;
	if(on_lower)
		B[5] = rjrel.z - lower;
}*/

void PHSliderJoint::Projection(double& f, int k){
	if(k == 2){
		if(on_lower)
			f = max(0.0, f);
		if(on_upper)
			f = min(0.0, f);
	}
}

/*void PHSliderJoint::ProjectionCorrection(double& F, int k){
	if(k == 5){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}*/

}


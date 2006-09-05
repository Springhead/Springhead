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
// PHHingeJoint
IF_OBJECT_IMP(PHHingeJoint, PHJoint1D)

double PHHingeJoint::GetPosition(){
	//軸方向の拘束は合致しているものと仮定して角度を見る
	double theta = qjrel.Theta();
	if(qjrel.Axis().Z() < 0.0)
		theta = -theta;
	return theta;
}

double PHHingeJoint::GetVelocity(){
	return wjrel[2];
}

/*void PHHingeJoint::CompConstraintJacobian(){
	CompDof();
	Ad.clear();
	Ac.clear();
	for(int i = 0; i < 2; i++){
		Jdv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jdv[i].SUBMAT(3, 0, 3, 3) = Jwv[i];
		Jdw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		Jdw[i].SUBMAT(3, 0, 3, 3) = Jww[i];
		Jcv[i].SUBMAT(0, 0, 3, 3) = Jvv[i];
		Jcv[i].SUBMAT(3, 0, 3, 3) = Jqv[i];
		Jcw[i].SUBMAT(0, 0, 3, 3) = Jvw[i];
		Jcw[i].SUBMAT(3, 0, 3, 3) = Jqw[i];
		if(solid[i]->solid->IsDynamical()){
			Tdv[i] = Jdv[i] * solid[i]->minv;
			Tdw[i] = Jdw[i] * solid[i]->Iinv;
			Tcv[i].SUBMAT(0, 0, 3, 3) = Tdv[i].SUBMAT(0, 0, 3, 3);
			Tcv[i].SUBMAT(3, 0, 3, 3) = Jqv[i] * solid[i]->minv;
			Tcw[i].SUBMAT(0, 0, 3, 3) = Tdw[i].SUBMAT(0, 0, 3, 3);
			Tcw[i].SUBMAT(3, 0, 3, 3) = Jqw[i] * solid[i]->Iinv;
			for(int j = 0; j < 6; j++)
				Ad[j] += Jdv[i].row(j) * Tdv[i].row(j) + Jdw[i].row(j) * Tdw[i].row(j);
			Ac.SUBVEC(0, 3) += Ad.SUBVEC(0, 3);
			for(int j = 3; j < 6; j++)
				Ac[j] += Jcv[i].row(j) * Tcv[i].row(j) + Jcw[i].row(j) * Tcw[i].row(j);
		}
	}
}*/

void PHHingeJoint::CompBias(double dt, double correction_rate){
	CompDof();
	double dtinv = 1.0 / dt;
	dbv = correction_rate * rjrel * dtinv;
	Vec3d w = qjrel.AngularVelocity((qjrel - Quaterniond()) * dtinv);
	w *= correction_rate;

	//	hase	これを入れたほうが、若干安定化する。
	//	本来速度をその都度修正しているので、PD制御ではないのだけど、
	//	shrinkRatio が高い場合、拘束力が残るため、Dを入れると安定化する。
	w += correction_rate * wjrel;

	dbw.x = w.x;
	dbw.y = w.y;
	//bw.z += w.z;
	double diff;
	if(mode == MODE_VELOCITY){
		dbw.z = -vel_d;
	}
	else if(spring != 0.0 || damper != 0.0){
		diff = GetPosition() - origin;
		while(diff >  M_PI) diff -= 2 * M_PI;
		while(diff < -M_PI) diff += 2 * M_PI;
		double tmp = 1.0 / (damper + spring * dt);
		dAw.z = tmp * dtinv;
		dbw.z = spring * (diff) * tmp;
	}
	//DSTR << "hinge" << fv << fw << diff * spring * dt << endl;
}

/*void PHHingeJoint::CompError(double dt){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}*/

void PHHingeJoint::Projection(double& f, int k){
	if(k == 5){
		if(on_lower)
			f = max(0.0, f);
		else if(on_upper)
			f = min(0.0, f);
	}
}

/*void PHHingeJoint::ProjectionCorrection(double& F, int k){
	if(k == 5){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}*/

}

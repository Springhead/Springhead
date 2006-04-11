#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

//----------------------------------------------------------------------------
// PHHingeJoint
//OBJECTIMP(PHHingeJoint, PHJoint1D)
IF_IMP(PHHingeJoint, PHJoint1D)

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

void PHHingeJoint::CompConstraintJacobian(){
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
}

void PHHingeJoint::CompBias(double dt){
	if(mode == MODE_VELOCITY){
		b[5] -= vel_d;
	}
	else if(spring != 0.0 || damper != 0.0){
		double diff = GetPosition() - origin;
		if(diff >  M_PI) diff -= 2 * M_PI;
		if(diff < -M_PI) diff += 2 * M_PI;
		double tmp = 1.0 / (damper + spring * dt);
		Ad[5] += tmp / dt;
		b[5] += spring * (diff) * tmp;
		/*
		バネダンパと外力の運動方程式：
		#mimetex(  f=kx+bv+f_e  )
		#mimetex(  v_{t+1} = f_{t+1} \Delta t + v_t  )
		#mimetex(  v_{t+1} = (kx_{t+1}+bv_{t+1}+f_e_{t+1}) \Delta t + v_t  )
		#mimetex(  x_{t+1} = v_{t+1} \Delta t + x_t  )
		~
		#mimetex(  v_{t+1} = (k(v_{t+1}\Delta t+x_t) +bv_{t+1}+f_e_{t+1}) \Delta t + v_t  )
		速度の更新の式：
		#mimetex(  (1-k\Delta t^2 - b \Delta t)v_{t+1} = (f_e_{t+1}+k x_t)\Delta t + v_t)
		なので，関節につながる２剛体に加わる力を，関節トルクに変換して，
		拘束条件としてあたえる vnext を更新すれば，外力を考慮したダンパになる？
		*/
	}
}

void PHHingeJoint::CompError(){
	B.SUBVEC(0, 3) = rjrel;
	B.SUBVEC(3, 3) = qjrel.V();
}

void PHHingeJoint::ProjectionDynamics(double& f, int k){
	if(k == 5){
		if(on_lower)
			f = max(0.0, f);
		if(on_upper)
			f = min(0.0, f);
	}
}

void PHHingeJoint::ProjectionCorrection(double& F, int k){
	if(k == 5){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}

}

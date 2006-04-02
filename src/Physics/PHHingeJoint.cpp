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
void PHHingeJoint::CompDof(){
	on_lower = on_upper = false;
	if(lower < upper){
		double theta = GetPosition();
		on_lower = (theta <= lower);
		on_upper = (theta >= upper);
	}
	if(on_lower || on_upper || mode == MODE_POSITION){
		dim_v = 3;
		dim_w = 3;
		dim_q = 3;
	}
	if(mode == MODE_VELOCITY || spring != 0.0 || damper != 0.0){
		dim_v = 3;
		dim_w = 3;
		dim_q = 2;
	}
	else{
		dim_v = 3;
		dim_w = 2;
		dim_q = 2;
	}
}
void PHHingeJoint::CompMotorForce(){
	fw[2] = torque;
}
void PHHingeJoint::CompBias(double dt){
	if(mode == MODE_VELOCITY){
		bw[2] -= vel_d;
	}
	else if(spring != 0.0 || damper != 0.0){
		double diff = GetPosition() - origin;
		if(diff >  M_PI) diff -= 2 * M_PI;
		if(diff < -M_PI) diff += 2 * M_PI;
		double tmp = 1.0 / (damper + spring * dt);
		Aw[2] += tmp / dt;
		bw[2] += spring * (diff) * tmp;
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
void PHHingeJoint::Projectionfw(double& f, int k){
	if(k == 2){
		if(on_lower)
			f = max(0.0, f);
		if(on_upper)
			f = min(0.0, f);
	}
}
void PHHingeJoint::ProjectionFq(double& F, int k){
	if(k == 2){
		if(on_lower)
			F = max(0.0, F);
		if(on_upper)
			F = min(0.0, F);
	}
}

}
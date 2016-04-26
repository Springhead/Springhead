/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHJointMotor.h>
#include <Physics/PHJoint.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PHSpring.h>
#include <Physics/PHConstraintEngine.h>

#include <iomanip>

using namespace PTM;
using namespace std;
namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHNDJointMotor

// バネ・ダンパ係数を0と見なす閾値
const double epsilon = 1e-10;
const double inf     = 1e+10;

template<int NDOF>
void PHNDJointMotor<NDOF>::SetupAxisIndex(){
	PHNDJointMotorParam<NDOF> p; GetParams(p);
	axes.Clear();
	for(int n = 0; n < joint->movableAxes.size(); ++n) {
		if (p.spring[n] >= epsilon || p.damper[n] >= epsilon || std::abs(p.offsetForce[n]) >= epsilon){
			axes.Enable(n);
			joint->targetAxes.Enable(joint->movableAxes[n]);
		}
	}
}	

template<int NDOF>
void PHNDJointMotor<NDOF>::Setup(){
	PHNDJointMotorParam<NDOF> p; GetParams(p);
	PHSceneIf* scene = joint->GetScene();
	double dt = scene->GetTimeStep();
	
	for(int n = 0; n < joint->movableAxes.size(); ++n){
		int j = joint->movableAxes[n];
		double fmax = std::min(
			(j < 3 ? scene->GetMaxForce() : scene->GetMaxMoment()),
			joint->GetMaxForce());
		fMaxDt[n]  =  (fmax * dt);
		fMinDt[n]  = -(fmax * dt);
	}

	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];

		b [j] = joint->b [joint->movableAxes[j]];
		dv[j] = joint->dv[joint->movableAxes[j]];

		f [j] *= axes.IsContinued(j) ? joint->engine->shrinkRate : 0;
	}

	if (p.spring.norm() < epsilon && p.damper.norm() < epsilon) {
		// オフセット力のみ有効の場合は拘束力初期値に設定するだけでよい
		for(int n = 0; n < joint->movableAxes.size(); ++n){
			f [n] = p.offsetForce[n];
			dA[n] = inf;
			db[n] = inf;
		}
	}
	else {
		VecNd sd = p.secondDamper;
		bool bHasSecondDamper = true;
		for (int i=0; i<NDOF; ++i) {
			if (sd[i] > FLT_MAX*0.1) {
				bHasSecondDamper = false;
			}
		}
		if (!bHasSecondDamper) {
			// 第二ダンパを無視（バネダンパ：弾性変形）
			CompBiasElastic();
		} else {
			// 第二ダンパ有効（三要素モデル）
			if (p.yieldStress > (FLT_MAX * 0.1)) {
				// 降伏応力無効（塑性変形）
				CompBiasPlastic();
			} else {
				// 降伏応力有効（弾塑性変形）
				CheckYielded();
				if (p.bYielded) {
					CompBiasPlastic();
				} else {
					CompBiasElastic();
				}
			}
		}
	}

	for(int n = 0; n < axes.size(); ++n){
		int j = axes[n];
		A   [n] = joint->A[joint->movableAxes[j]];
		if(A[n] + dA[n] < inf)
			 Ainv[n] = 1.0 / (A[n] + dA[n]);
		else Ainv[n] = 0.0;
	}

	SetParams(p);

}

template<int NDOF>
bool PHNDJointMotor<NDOF>::Iterate(){
	bool updated = false;
	for (int n=0; n<axes.size(); ++n) {
		int i = axes[n];
		int j = joint->movableAxes[i];
		if(!joint->dv_changed[j])
			continue;

		dv  [i] = joint->dv[j];
		res [i] = b[i] + db[i] + dA[i]*f[i] + dv[i];
		fnew[i] = f[i] - joint->engine->accelSOR * Ainv[i] * res[i];
	
		fnew[i] = min(max(fMinDt[i], fnew[i]), fMaxDt[i]);

		df[i] = fnew[i] - f[i];
		f [i] = fnew[i];

		if(std::abs(df[i]) > joint->engine->dfEps){
			updated = true;
			CompResponseDirect(df[i], i);
		}
	}
	return updated;
}

template<int NDOF>
void PHNDJointMotor<NDOF>::CompResponse(double df, int i){
	joint->CompResponse(df, joint->movableAxes[i]);
}

template<int NDOF>
void PHNDJointMotor<NDOF>::CompResponseDirect(double df, int i){
	joint->CompResponseDirect(df, joint->movableAxes[i]);
}

/// 弾性変形用のCompBias
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBiasElastic() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double dt = joint->GetScene()->GetTimeStep();
	VecNd propV = GetPropV();
	
	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];
		
		double K   = p.spring[j];
		double D   = p.damper[j];
		if(K < epsilon && D < epsilon){
			dA[j] = inf;
			db[j] = inf;
		}
		else{
			double tmp = 1.0 / (D + K*dt);
			dA[j] = tmp * (1.0/dt);
			db[j] = tmp * (-K*propV[j] - D*p.targetVelocity[j] - p.offsetForce[j]);
		}
	}

	SetParams(p);
}

/// 塑性変形用のCompBias
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBiasPlastic() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double dt = joint->GetScene()->GetTimeStep();

	for(int n = 0; n < axes.size(); ++n) {
		int j = axes[n];
		
		// こっちだけhardnessRate掛けてるけどいいの？
		double K   = p.spring      [j] * p.hardnessRate;
		double D   = p.damper      [j] * p.hardnessRate;
		double D2  = p.secondDamper[j] * p.hardnessRate;
		double tmp = D+D2+K*dt;

		int i = joint->movableAxes[j];
		newXs[i] = ((D+D2)/tmp)*p.xs[i] + (D2*dt/tmp) * joint->vjrel[i];

		dA[j] = tmp/(D2*(K*dt + D)) * (1.0/dt);
		db[j] = K/(K*dt + D)*(p.xs[i]);
	}

	// 弾塑性変形：TargetPositionを変更して残留変位を残す
	if (p.yieldStress < (FLT_MAX * 0.1)) {
		if (joint->vjrel.w().norm() < 0.01) {
			PH1DJoint* jnt1D = joint->Cast();
			if(jnt1D)
				jnt1D->SetTargetPosition(jnt1D->GetPosition());
			PHBallJoint* ball = joint->Cast();
			if(ball)
				ball->SetTargetPosition(ball->GetPosition());
		}
	}

	p.xs = newXs;

	SetParams(p);
}

/// 降伏したかどうか
template<int NDOF>
void PHNDJointMotor<NDOF>::CheckYielded() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	for(int n = 0; n < axes.size(); n++){
		int j = axes[n];
		p.fAvg[j] = (0.8 * p.fAvg[j]) + (0.2 * f[j] * joint->GetScene()->GetTimeStepInv());
	}

	if(p.bYielded) {
		if (p.fAvg.norm() < p.yieldStress) {
			if (NDOF==3) {
				Vec3d angle = newXs.w();
				Quaterniond qForct;
				qForct.FromEuler(Vec3d(angle.y, angle.z, angle.x));
				DCAST(PHBallJoint,joint)->SetTargetPosition( qForct.Inv() * joint->Xjrel.q );
			}   // NDOF=1や，NDOF=6(PHSpring）の場合は未実装 <!!>
			p.bYielded = false;
		}
	} else {
		if (p.fAvg.norm() > p.yieldStress) {
			p.bYielded = true;
		}
	}

	SetParams(p);
}

// テンプレートの明示的実体化（PHNDJointMotorのメンバ関数をcppで定義できるようにするための措置）
template class PHNDJointMotor<1>;
template class PHNDJointMotor<3>;
template class PHNDJointMotor<6>;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PH1DJointMotor

/// propVを計算する
PTM::TVector<1,double> PH1DJointMotor::GetPropV() {
	PH1DJoint* j = joint->Cast();
	PTM::TVector<1,double> propV;
	propV[0] = -1.0 * j->GetDeviation();
	return propV;
}

/// パラメータを取得する
void PH1DJointMotor::GetParams(PHNDJointMotorParam<1>& p) {
	PH1DJoint* j = joint->Cast();
	p.fAvg[0]           = j->fAvg[0];
	p.xs                = j->xs;
	p.bYielded          = j->bYielded;
	p.spring[0]         = j->spring;
	p.damper[0]         = j->damper;
	p.secondDamper[0]   = j->secondDamper;
	p.targetVelocity[0] = j->targetVelocity;
	p.offsetForce[0]    = j->offsetForce;
	p.yieldStress       = j->yieldStress;
	p.hardnessRate      = j->hardnessRate;
}

/// パラメータを反映する
void PH1DJointMotor::SetParams(PHNDJointMotorParam<1>& p) {
	PH1DJoint* j = joint->Cast();
	j->fAvg[0]  = p.fAvg[0];
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHBallJointMotor

/// propVを計算する
PTM::TVector<3,double> PHBallJointMotor::GetPropV() {
	PHBallJoint* j = joint->Cast();
	Quaterniond pQ = j->targetPosition * joint->Xjrel.q.Inv();
	return((PTM::TVector<3,double>)(pQ.RotationHalf()));
}

/// パラメータを取得する
void PHBallJointMotor::GetParams(PHNDJointMotorParam<3>& p) {
	PHBallJoint* j = joint->Cast();
	p.fAvg           = j->fAvg.w();
	p.xs             = j->xs;
	p.bYielded       = j->bYielded;
	p.spring         = PTM::TVector<3,double>(Vec3d(1,1,1) * j->spring);
	p.damper         = PTM::TVector<3,double>(Vec3d(1,1,1) * j->damper);
	p.secondDamper   = PTM::TVector<3,double>(j->secondDamper);
	p.targetVelocity = PTM::TVector<3,double>(j->targetVelocity);
	p.offsetForce    = PTM::TVector<3,double>(j->offsetForce);
	p.yieldStress    = j->yieldStress;
	p.hardnessRate   = j->hardnessRate;
}

/// パラメータを反映する
void PHBallJointMotor::SetParams(PHNDJointMotorParam<3>& p) {
	PHBallJoint* j = joint->Cast();
	j->fAvg.w() = p.fAvg;
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHSpringMotor

/// propVを計算する
PTM::TVector<6,double> PHSpringMotor::GetPropV() {
	Vec6d propV;
	Quaterniond diff = joint->Xjrel.q.Inv();
	propV.SUBVEC(0,3) = -joint->Xjrel.r;
	propV.SUBVEC(3,3) = diff.RotationHalf();
	return propV;
}

/// パラメータを取得する
void PHSpringMotor::GetParams(PHNDJointMotorParam<6>& p) {
	PHSpring* j = joint->Cast();
	p.fAvg.SUBVEC(0,3)                        = j->fAvg.v();
	p.fAvg.SUBVEC(3,3)                        = j->fAvg.w();
	p.xs                                      = j->xs;
	p.bYielded                                = j->bYielded;
	for(int i=0;i<3;++i){ p.spring[i]         = j->spring[i]; }
	for(int i=0;i<3;++i){ p.spring[i+3]       = j->springOri; }
	for(int i=0;i<3;++i){ p.damper[i]         = j->damper[i]; }
	for(int i=0;i<3;++i){ p.damper[i+3]       = j->damperOri; }
	for(int i=0;i<3;++i){ p.secondDamper[i]   = j->secondDamper[i]; }
	for(int i=0;i<3;++i){ p.secondDamper[i+3] = j->secondDamperOri; }
	for(int i=0;i<6;++i){ p.targetVelocity[i] = 0.0; } // PHSpringには無い
	for(int i=0;i<6;++i){ p.offsetForce[i]    = 0.0; } // PHSpringには無い
	p.yieldStress    = j->yieldStress;
	p.hardnessRate   = j->hardnessRate;
}

/// パラメータを反映する
void PHSpringMotor::SetParams(PHNDJointMotorParam<6>& p) {
	PHSpring* j = joint->Cast();
	for (int i=0; i<6; ++i) { j->fAvg[i]=p.fAvg[i]; }
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

}

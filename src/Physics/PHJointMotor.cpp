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

/// 拘束軸を決定する
template<int NDOF>
void PHNDJointMotor<NDOF>::SetupAxisIndex() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	// jointの可動自由度を拘束する
	for (int n=0; n<joint->nMovableAxes; ++n) {
		int i = joint->movableAxes[n];

		double epsilon = 1e-10;
		if (p.spring[n] > epsilon || p.damper[n] > epsilon) {
			// 拘束軸を有効にする
			joint->axes.Enable(i);
		}

		// Projection用のMax / Minを設定する
		double dt = joint->GetScene()->GetTimeStep();
		joint->fMaxDt[i]  =  (joint->GetMaxForce() * dt);
		joint->fMinDt[i]  = -(joint->GetMaxForce() * dt);
	}

	SetParams(p);
}

/// dA, dbを計算する
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBias() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double epsilon = 1e-10;
	if (p.spring.norm() < epsilon && p.damper.norm() < epsilon) {
		// オフセット力のみ有効の場合は拘束力初期値に設定するだけでよい
		for (int n=0; n<joint->nMovableAxes; ++n) {
			PTM::TVector<NDOF,double> oF = p.offsetForce;
			joint->f[joint->movableAxes[n]] = min(oF[n], joint->GetMaxForce());
		}
	} else {
		VecNd sd = p.secondDamper; bool bHasSecondDamper = true;
		for (int i=0; i<NDOF; ++i) { if (sd[i] > FLT_MAX*0.1) { bHasSecondDamper = false; } }
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

	SetParams(p);
}

/// 弾性変形用のCompBias
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBiasElastic() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double dt = joint->GetScene()->GetTimeStep();
	PTM::TVector<NDOF,double> propV = GetPropV();
	for (int n=0; n<NDOF; ++n) {
		int i = joint->movableAxes[n];
		if (joint->axes.IsEnabled(i)) {
			double K   = p.spring[n];
			double D   = p.damper[n];
			double tmp = 1.0 / (D + K*dt);
			joint->dA[i] = tmp * (1.0/dt);
			joint->db[i] = tmp * (-K*propV[n] - D*p.targetVelocity[n] - p.offsetForce[n]);
		}
	}

	SetParams(p);
}

/// 塑性変形用のCompBias
template<int NDOF>
void PHNDJointMotor<NDOF>::CompBiasPlastic() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	double dt = joint->GetScene()->GetTimeStep();
	for (int n=0; n<NDOF; ++n) {
		int i = joint->movableAxes[n];
		if (joint->axes.IsEnabled(i)) {
			// こっちだけhardnessRate掛けてるけどいいの？
			double K   = p.spring[n] * p.hardnessRate;
			double D   = p.damper[n] * p.hardnessRate;
			double D2  = p.secondDamper[n] * p.hardnessRate;
			double tmp = D+D2+K*dt;

			newXs[i] = ((D+D2)/tmp)*p.xs[i] + (D2*dt/tmp) * joint->vjrel[i];

			joint->dA[i] = tmp/(D2*(K*dt + D)) * (1.0/dt);
			joint->db[i] = K/(K*dt + D)*(p.xs[i]);
		}
	}

	// 弾塑性変形：TargetPositionを変更して残留変位を残す
	if (p.yieldStress < (FLT_MAX * 0.1)) {
		if (joint->vjrel.w().norm() < 0.01) {
			if (DCAST(PH1DJoint,joint)) {
				DCAST(PH1DJoint,joint)->SetTargetPosition(DCAST(PH1DJoint,joint)->GetPosition());
			}
			if (DCAST(PHBallJoint,joint)) {
				DCAST(PHBallJoint,joint)->SetTargetPosition(DCAST(PHBallJoint,joint)->GetPosition());
			}
		}
	}

	p.xs = newXs;

	SetParams(p);
}

/// 降伏したかどうか
template<int NDOF>
void PHNDJointMotor<NDOF>::CheckYielded() {
	PHNDJointMotorParam<NDOF> p; GetParams(p);

	VecNd motorf;
	for (int n=0; n<NDOF; ++n) { motorf[n] = joint->f[joint->movableAxes[n]]; }
	p.fAvg = (0.8 * p.fAvg) + (0.2 * motorf / joint->GetScene()->GetTimeStep());

	if (p.bYielded) {
		if (p.fAvg.norm() < p.yieldStress) {
			if (NDOF==3) {
				Vec3d angle = newXs.w();
				Quaterniond qForct; qForct.FromEuler(Vec3d(angle.y, angle.z, angle.x));
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
	//propV[0] = j->targetPosition - DCAST(PH1DJoint,joint)->GetPosition();
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
	j->fAvg.w() = (Vec3d)(p.fAvg);
	j->xs       = p.xs;
	j->bYielded = p.bYielded;
	// 上記以外の変数は特に反映する必要はない．
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHSpringMotor

/// propVを計算する
PTM::TVector<6,double> PHSpringMotor::GetPropV() {
	SpatialVector propV;
	propV.v() = -joint->Xjrel.r;
	Quaterniond diff = joint->Xjrel.q.Inv();
	propV.w() = diff.RotationHalf();
	return((PTM::TVector<6,double>)(propV));
}

/// パラメータを取得する
void PHSpringMotor::GetParams(PHNDJointMotorParam<6>& p) {
	PHSpring* j = joint->Cast();
	p.fAvg                                    = j->fAvg;
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

/*
 *  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHHingeJoint.h>
#include <Physics/PHConstraintEngine.h>

using namespace PTM;
using namespace std;

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHHingeJointNode

void PHHingeJointNode::CompJointJacobian(){
	J.clear();
	J.col(0)[5] = 1.0;
	PHTreeNode1D::CompJointJacobian();
}

void PHHingeJointNode::CompJointCoriolisAccel(){
	cj.clear();
}

void PHHingeJointNode::CompRelativeVelocity(){
	PH1DJoint* j = GetJoint();
	j->vjrel.v() = Vec3d();
	j->vjrel.w() = Vec3d(0.0, 0.0, j->velocity[0]);
}

void PHHingeJointNode::CompRelativePosition(){
	PH1DJoint* j = GetJoint();
	j->Xjrel.q = Quaterniond::Rot(j->position[0], 'z');
	j->Xjrel.r.clear();
}

void PHHingeJointNode::UpdateJointPosition(double dt){
	PHTreeNode1D::UpdateJointPosition(dt);
	if(DCAST(PHHingeJoint, GetJoint())->IsCyclic()){
		double p = GetJoint()->position[0];
		while(p >  M_PI) p -= 2 * M_PI;
		while(p < -M_PI) p += 2 * M_PI;
		GetJoint()->position[0] = p;
	}
}

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// PHHingeJoint

PHHingeJoint::PHHingeJoint(const PHHingeJointDesc& desc) {
	SetDesc(&desc);
	
	movableAxes.Enable(5);
}

// ----- エンジンから呼び出される関数

void PHHingeJoint::UpdateJointState(){
	//軸方向の拘束は合致しているものと仮定して角度を見る
	position[0] = Xjrel.q.Theta();

	// -π～πの範囲に収める
	//position[0] = ( (position[0] / (2*M_PI)) - floor(position[0] / (2*M_PI)) ) * (2*M_PI);
	//if (position[0] > M_PI) { position[0] -= 2 * M_PI; }
	if(cyclic){
		/*
		while(position[0] >  M_PI)
			position[0] -= 2 * M_PI;
		while(position[0] < -M_PI)
			position[0] += 2 * M_PI;
			*/
		position[0] = fmod(position[0], 2*M_PI);
		if (position[0] > M_PI) { position[0] -= 2*M_PI; }
	}

	if (Xjrel.q.Axis().Z() < 0.0) { position = -position; }
	velocity[0] = vjrel[5];
}

// ----- PHConstraintの派生クラスで実装される機能

void PHHingeJoint::CompBias(){
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	
	// 拘束誤差補正のためのバイアス
	if (engine->numIterCorrection==0){ // Correctionを速度LCPで行う場合
		//	次のステップでの位置の誤差の予測値が0になるような速度を設定
		//	dv * dt = x + v*dt
		db.v() = Xjrel.r * dtinv + vjrel.v();

		//	角度の誤差を0にするような回転角度を求める。
		Quaterniond qarc;
		qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1)); // 軸を一致させるような回転
		db.w() = -(qarc.Theta() * dtinv) * qarc.Axis() + vjrel.w();
		db *= engine->velCorrectionRate;
	}

	// 親クラスのCompBias．motor,limitのCompBiasが呼ばれるので最後に呼ぶ
	PH1DJoint::CompBias();
}

void PHHingeJoint::CompError(){
	B.v() = Xjrel.r;

	Quaterniond qarc;
	qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1)); // 軸を一致させるような回転
	B.w() = -qarc.Theta() * qarc.Axis();
}

// ----- インタフェースの実装

double PHHingeJoint::GetDeviation(){
	double diff = PH1DJoint::GetDeviation();
	if(cyclic){
		while(diff >  M_PI)
			diff -= 2 * M_PI;
		while(diff < -M_PI)
			diff += 2 * M_PI;
	}
	return diff;
}

}

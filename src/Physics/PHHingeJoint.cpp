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

//----------------------------------------------------------------------------
// PHHingeJoint
PHHingeJoint::PHHingeJoint(const PHHingeJointDesc& desc){
	SetDesc(&desc);
	movableAxes[0] = 5;
}

void PHHingeJoint::UpdateJointState(){
	//軸方向の拘束は合致しているものと仮定して角度を見る
	position[0] = Xjrel.q.Theta();
	if(Xjrel.q.Axis().Z() < 0.0)
		position = -position;
	velocity[0] = vjrel.w().z;
}

double PHHingeJoint::GetDeviation(){
	// 不連続なトルク変化を避けるため (ゼンマイのようにいくらでも巻けるように削除)。 07/07/26
	//// ↑むしろこのコードがあることで不連続なトルク変化が避けられているのでは？と思い復活． 08/10/07 mitake
	//// ↓これやらないと発振するので戻す．物理シミュレータは一意に値が指定出来ないと計算無理じゃないの？ 09/06/01 toki
	// while(diff >  M_PI) diff -= 2 * M_PI;
	// while(diff < -M_PI) diff += 2 * M_PI;
	// ↓こっちのほうがはやいので変更  09/07/06 mitake
	double diff = PHJoint1D::GetDeviation();
	diff = ( (diff / (2*M_PI)) - floor(diff / (2*M_PI)) ) * (2*M_PI);
	if (diff > M_PI) { diff -= 2 * M_PI; }
	return diff;
}

void PHHingeJoint::CompBias(){
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	
	// 拘束誤差補正のためのバイアス
	if (engine->numIterCorrection==0){	//	Correction を速度LCPで行う場合
		//	次のステップでの位置の誤差の予測値が0になるような速度を設定
		//	dv * dt = x + v*dt
		db.v() = Xjrel.r * dtinv + vjrel.v();
		//	角度の誤差を0にするような回転角度を求める。
		Quaterniond qarc;
		qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1)); // 軸を一致させるような回転
		db.w() = -(qarc.Theta() * dtinv) * qarc.Axis() + vjrel.w();
		db *= engine->velCorrectionRate;
	}

}

void PHHingeJoint::CompError(){
	B.v() = Xjrel.r;

	//	B.w() = Xjrel.q.V();
	//	B.w().z = 0.0;
	Quaterniond qarc;
	qarc.RotationArc(Xjrel.q * Vec3d(0,0,1), Vec3d(0,0,1)); // 軸を一致させるような回転
	B.w() = -qarc.Theta() * qarc.Axis();
}



//-----------------------------------------------------------------------------
void PHHingeJointNode::CompJointJacobian(){
	J.col(0).sub_vector(PTM::TSubVectorDim<0, 3>()).clear();
	J.col(0).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(0.0, 0.0, 1.0);
	PHTreeNode1D::CompJointJacobian();
}

void PHHingeJointNode::CompJointCoriolisAccel(){
	cj.clear();
}

void PHHingeJointNode::CompRelativeVelocity(){
	PHJoint1D* j = GetJoint();
	j->vjrel.v().clear();
	j->vjrel.w() = Vec3d(0.0, 0.0, j->velocity[0]);
}

void PHHingeJointNode::CompRelativePosition(){
	PHJoint1D* j = GetJoint();
	j->Xjrel.q = Quaterniond::Rot(j->position[0], 'z');
	j->Xjrel.r.clear();
}

}

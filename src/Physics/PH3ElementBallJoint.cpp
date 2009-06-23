/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 * 
 *　SwingDirの制限は入っていません。必要な人が頑張って実装してください(by toki 2007.12.05)
 *
 */
#include "Physics.h"
//#include "../../../Project\personal\Matunaga\HapticJointTest\BoxStack.h"
#include <Math.h>
#pragma hdrstop

using namespace PTM;
using namespace std;

namespace Spr{;
//----------------------------------------------------------------------------
// PH3ElementBallJointDesc

PH3ElementBallJointDesc::PH3ElementBallJointDesc(){
	
	PHBallJointDesc();
	secondDamper		  = 0.0;
	yieldStress			=0.0;			// 降伏応力
	hardnessRate		=1.0;		// 降伏応力以下の場合に二個目のダンパ係数に掛ける比率

}

//----------------------------------------------------------------------------
// PH3ElementBallJoint
// コンストラクタ
PH3ElementBallJoint::PH3ElementBallJoint(const PH3ElementBallJointDesc& desc){
	PHBallJoint::PHBallJoint();
}

// オーバーライドされているのでPH3ElementBallJointを読んだ時には
// PHBallJoint::CompBias()ではなくてこの関数だけこっちが呼ばれる。
void PH3ElementBallJoint::CompBias(){
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	
	db.v() = Xjrel.r * dtinv;		//	並進誤差の解消のため、速度に誤差/dtを加算, Xjrel.r: ソケットに対するプラグの位置
	db.v() *= engine->velCorrectionRate;

	Quaterniond propQ = goal * Xjrel.q.Inv();	
	Vec3d propV = propQ.RotationHalf();

	// 可動域制限がかかっている場合はpropの座標を変換して考えないといけない。
	if (anyLimit){
		propV = Jcinv * propV;
	}

	// バネダンパが入っていたら構築する
	if (spring != 0.0 || damper != 0.0 || secondDamper!=0.0){
		//3要素モデル
		double dtinv = 1.0 / GetScene()->GetTimeStep(), tmp;
		double D1 = damper;
		double D2 = secondDamper;
		double K = spring;
		double h = GetScene()->GetTimeStep();
		
		//降伏応力以下ではジョイントを固くする
		if(f.w().norm()>=yieldStress ){
			K = spring;
			D1 = damper;
			D2=secondDamper;
		}else{
			K = spring;
			D1 = damper;
			D2= secondDamper*hardnessRate;
		}
		//3要素モデルの計算
		ws=vjrel;	//バネとダンパの並列部の速さ
		tmp = D1+D2+K*h;

		xs[1] = ((D1+D2)/tmp)*xs[0] + (D2*h/tmp)*ws;	//バネとダンパの並列部の距離の更新
		
		for(int i=0;i<3;i++){
		dA.w()[i]= tmp/(D2*(K*h+D1)) * dtinv /I[i];
		}
		db.w() = K/(K*h+D1)*(xs[0].w()) ;
	
		xs[0]=xs[1];	//バネとダンパの並列部の距離のステップを進める
	}else{
			//dA.w().clear();
			db.w().clear();
	}
	
	Vec3d vJc = Jc * vjrel.w();
	// 可動域フラグの指定onLimit[0]: swing, onLimit[1]: twist
	// nowTheta[0]: swing, nowTheta[1]: twist
	// 可動域制限を越えていたら、dA:関節を柔らかくする成分を0にする、db:侵入してきた分だけ元に戻す	
	// x軸方向に拘束をかける場合	
	if(onLimit[0].onLower){
		dA.w()[0] = 0;
		db.w()[0] = (nowTheta[0] - limitSwing[0]) * dtinv * engine->velCorrectionRate;
	}
	
	else if(onLimit[0].onUpper){
		dA.w()[0] = 0;
		db.w()[0] = (nowTheta[0] - limitSwing[1]) * dtinv * engine->velCorrectionRate;
	}

	//z軸方向に拘束をかける場合
	if(onLimit[1].onLower && (vJc.z < 0)){
		dA.w()[2] = 0;
		db.w()[2] = (nowTheta[1] - limitTwist[0]) * dtinv * engine->velCorrectionRate;
	}
	else if(onLimit[1].onUpper && (vJc.z > 0)){
		dA.w()[2] = 0;
		db.w()[2] = (nowTheta[1] - limitTwist[1]) * dtinv * engine->velCorrectionRate;
	}
}

void PH3ElementBallJointNode::CompJointJacobian(){
	PHBallJoint* j = GetJoint();
	//SwingTwist& angle = (SwingTwist&)(j->position);
	//angle.Jacobian(Jst);
	//Matrix3d test = Jst * Jcinv;
	Quaterniond q = j->Xjrel.q;
	for(int i = 0; i < 3; i++)
		J.col(i).sub_vector(PTM::TSubVectorDim<0,3>()).clear();
	/*J[0].w() = 2.0 * Vec3d(-q.x, -q.y, -q.z);
	J[1].w() = 2.0 * Vec3d( q.w,  q.z, -q.y);
    J[2].w() = 2.0 * Vec3d(-q.z,  q.w,  q.x);
    J[3].w() = 2.0 * Vec3d( q.y, -q.x,  q.w);*/
	J.col(0).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(1.0, 0.0, 0.0);
	J.col(1).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(0.0, 1.0, 0.0);
	J.col(2).sub_vector(PTM::TSubVectorDim<3, 3>()) = Vec3d(0.0, 0.0, 1.0);
	PHTreeNodeND<3>::CompJointJacobian();
}

}

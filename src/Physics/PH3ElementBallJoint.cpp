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
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;

namespace Spr{;
//----------------------------------------------------------------------------
// PH3ElementBallJointDesc

PH3ElementBallJointDesc::PH3ElementBallJointDesc(){
	
	PHBallJointDesc();
	secondDamper		  = 0.0;

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
	if (spring != 0.0 || damper != 0.0){
		double dtinv = 1.0 / GetScene()->GetTimeStep(), tmp;
		double D1 = damper;
		double D2 = secondDamper;
		double K = spring;
		double h = GetScene()->GetTimeStep();
		
		ws=vjrel;	//バネのダンパの並列部の速さ
		tmp = D2-D1+K*h;
		xs[1] = ((D2-D1)/tmp)*xs[0] + (D2*h/(D2-D1))*ws;	//バネとダンパの並列部の距離の更新		
		dA.w()[0]= (D2-D1)*(D2-D1)/(D1*D2*tmp) * dtinv;
		dA.w()[1]= (D2-D1)*(D2-D1)/(D1*D2*tmp) * dtinv;
		dA.w()[2]= (D2-D1)*(D2-D1)/(D1*D2*tmp) * dtinv;

		db.w() = K*(D2-D1)*(D2-D1)/(D2*tmp*tmp)*(xs[0].w()) ;
		
		xs[0]=xs[1];	//バネとダンパの並列部の距離のステップを進める

	}
	else{
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

}

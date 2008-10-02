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
// PHBallJointDesc

PHBallJointDesc::PHBallJointDesc(){

	spring			= 0.0;
	damper			= 0.0;
	limitSwing[0]	= FLT_MAX;
	limitSwing[1]	= FLT_MAX;
	limitTwist[0]	= FLT_MAX;
	limitTwist[1]	= FLT_MAX;	
	limitDir		= Vec3d(0.0, 0.0, 1.0);
	goal			= Quaterniond(1, 0, 0, 0);
	fMax			= FLT_MAX;
	fMin			= -FLT_MAX;
	desiredVelocity = Quaterniond(1, 0, 0, 0);
}

//----------------------------------------------------------------------------
// PHBallJoint
PHBallJoint::PHBallJoint(const PHBallJointDesc& desc){
	SetDesc(&desc);
	axisIndex[0] = 3;
	axisIndex[1] = 4;
	axisIndex[2] = 5;
	
	// limit上に居るかどうかのフラグの初期化
	for(int i=0; i<3; ++i){
		onLimit[i].onLower = false;
		onLimit[i].onUpper = false;
	}
	anyLimit = false;
	

}

void PHBallJoint::AfterSetDesc(){
	if (GetScene()){
		fMinDt = fMin * GetScene()->GetTimeStep();
		fMaxDt = fMax * GetScene()->GetTimeStep();
	}
	PHJointND<3>::AfterSetDesc();
}

void PHBallJoint::UpdateJointState(){
	// Jc.Ez() : Socketに対するPlugの向いている方向(旧currentVector)
	Jc.Ez() = Xjrel.q * Vec3f(0.0, 0.0, 1.0);
	if((onLimit[0].onLower || onLimit[0].onUpper) && (Jc.Ez() != limitDir)){
		Jc.Ex() = cross(Jc.Ez(),(Jc.Ez() - limitDir)).unit();
		Jc.Ey() = cross(Jc.Ez(), Jc.Ex());
		Jcinv   = Jc.trans();
	}
}

void PHBallJoint::SetConstrainedIndex(bool* con){

	// -----bool* conの意味----------------------------------
	// 拘束する軸についてのフラグ
	// con[0]-con[2]:並進運動（x,y,z）
	// con[3]-con[5]:回転運動（x軸まわり,y軸まわり,z軸まわり）
	// true:拘束する、false:拘束しない
	//-------------------------------------------------------

	con[0] = con[1] = con[2] = true;				
	con[3] = con[4] = con[5] = false;
	
	// 現在のSocketとPlugとの間の角度を計算

	nowTheta[0]	= acos(dot(limitDir, Jc.Ez()));			///< Swing角の計算	
	
	Quaterniond qSwing;
	Vec3d half =  0.5 * (Vec3d(0.0, 0.0, 1.0) + Jc.Ez());	
	double l = half.norm();
	if (l>1e-20){
		half /= l;
		qSwing.V() = cross(half, Vec3d(0,0,1));
		qSwing.w = sqrt(1-qSwing.V().square());
	}
	else{
		qSwing.V() = Vec3d(1,0,0);
		qSwing.w = 0;
	}

	Quaterniond twistQ = qSwing * Xjrel.q;
	nowTheta[1] = twistQ.Theta();						///< Twist角の計算を行っている	
	if (twistQ.z < 0)
		nowTheta[1]  *= -1;								///< Twist回転軸が反対を向くことがあるのでその対策
	
	if(nowTheta[1] < Rad(-180)) nowTheta[1] += Rad(360);
	if(nowTheta[1] > Rad( 180)) nowTheta[1] -= Rad(360);
	
	// 可動域制限にかかっているかの判定
	// swing角の可動域制限の確認
	if (limitSwing[0]!=FLT_MAX && nowTheta[0] < limitSwing[0])
		onLimit[0].onLower = true;
	else if(limitSwing[1]!=FLT_MAX && nowTheta[0] > limitSwing[1])
		onLimit[0].onUpper = true;
	else{
		onLimit[0].onLower = false;
		onLimit[0].onUpper = false;
	}	

	// twist角の可動域制限の確認
	if(limitTwist[0]!=FLT_MAX && nowTheta[1] < limitTwist[0])
		onLimit[1].onLower = true;
	else if(limitTwist[1]!=FLT_MAX && nowTheta[1] > limitTwist[1])
		onLimit[1].onUpper = true;
	else{
		onLimit[1].onLower = false;
		onLimit[1].onUpper = false;
	}

	// どこかが可動域制限にかかっているとtrue
	if((onLimit[0].onUpper || onLimit[0].onLower) ||
	   (onLimit[1].onUpper || onLimit[1].onLower))  
	   anyLimit = true;
	else anyLimit = false;

	// 上の計算を踏まえて毎回、回転軸の拘束条件の更新をする
	con[3] = onLimit[0].onUpper || onLimit[0].onLower || spring != 0.0 || damper != 0.0;
	con[4] = spring != 0.0	    || damper != 0.0;
	con[5] = onLimit[1].onUpper || onLimit[1].onLower || spring != 0.0 || damper != 0.0;
}

// ヤコビアンの角速度部分を座標変換してSwingTwist角の時間変化率へのヤコビアンにする
void PHBallJoint::ModifyJacobian(){
	//J[0].wv()剛体0の速度から相対角速度へ変換するヤコビアン
	//J[0].ww()剛体0の速度から相対角速度へ変換するヤコビアン
	//J[1].wv()剛体1の速度から相対角速度へ変換するヤコビアン
	//J[1].ww()剛体1の速度から相対角速度へ変換するヤコビアン
	if(anyLimit){
		J[0].wv() = Jcinv * J[0].wv();
		J[0].ww() = Jcinv * J[0].ww();
		J[1].wv() = Jcinv * J[1].wv();
		J[1].ww() = Jcinv * J[1].ww();
	}
}

void PHBallJoint::CompBias(){
	
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	
	db.v() = Xjrel.r * dtinv;		//	並進誤差の解消のため、速度に誤差/dtを加算, Xjrel.r: ソケットに対するプラグの位置のズレ
	db.v() *= engine->velCorrectionRate;
	// 位置制御の時の計算
	Quaterniond propQ = goal * Xjrel.q.Inv();	// Xjrel.qの目標goalとXjrel.qの実際の角度の差をQuaternionで取得
	/*******************************************************************************************************
	足りない角度の差を回転軸ベクトルに変換．propV(田崎さんの論文でいうq[t])に対してdb.w()を計算している.
	自然長が0[rad]で，propV[rad]伸びた時に対しての角度バネを構成していると考えればいい．
	********************************************************************************************************/
	Vec3d propV = propQ.RotationHalf();			

	// 可動域制限がかかっている場合はpropの座標を変換して考えないといけない。
	if (anyLimit){
		propV = Jcinv * propV;
	}
	if(mode == MODE_VELOCITY){
		// この辺の目標軌道関数の微分とかの計算ってこれでいいんだろうか･･･？
		db.w()		= -Jcinv * desiredVelocity.RotationHalf();
	}
	else if(mode == MODE_TRAJECTORY_TRACKING){
		preQd		= qd;
		qd			= goal.RotationHalf();
		preQdDot	= desiredVelocity;
		qdDot		= (qd - preQd) / GetScene()->GetTimeStep();
		qdWDot		= (qdDot - preQdDot) / GetScene()->GetTimeStep();
	}
	// バネダンパが入っていたら構築する
	if (spring != 0.0 || damper != 0.0){
		double tmp = 1.0 / (damper + spring * GetScene()->GetTimeStep());
		/*****************************************************************************
		w[t+1] = (A+dA) * λ[t+1] + (b+db)
		のAの内，バネダンパを構成するのは，BallJointの場合は回転の三軸についてなので，
		SpatialVector dA.w()にtmp = (D + K⊿t)^{-1}を入れている．
		dtinvをさらにかけるのは,pptなど表記とは違い，A=JM^{-1}J^T ⊿tのうち，
		プログラム中で計算しているのは，A = JM^{-1}J^Tだけで，あとから
		(A + dA)⊿tをしているため
		******************************************************************************/
		dA.w() = tmp * dtinv * Vec3d(1.0, 1.0, 1.0);

		/****
		軌道追従制御のdb，慣性行列の扱いは相対加速度に対して，親剛体への反作用方向と子剛体への作用方向にかかる
		拘束は回転方向なので，慣性行列のうちの慣性モーメントだけ引っ張ってくる
		****/
		if(mode == MODE_TRAJECTORY_TRACKING){
			db.w() = tmp * ( (spring * -(qd - Xjrel.q.Inv().RotationHalf()))
						  + (solid[0]->GetInertia() * qdWDot)
						  - (solid[1]->GetInertia() * qdWDot)
						  + (damper * -qdDot) );
		}
		/**/
		// 位置制御のbの追加部分，ちゃんと動くけどマイナスが付くのはbが小さくなる方向に動かしたいから
		else db.w() = -tmp * spring * propV;
	}
	else{
		//dA.w().clear();
		db.w().clear();
	}
	
	// vJc : Jcによって写像される拘束座標系から見たPlugの角速度
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

void PHBallJoint::CompError(){
	B.v() = Xjrel.r;
	
	B.w()[0] = (onLimit[0].onLower ? (nowTheta[0] - limitSwing[0]) :
	onLimit[0].onUpper ? (nowTheta[0] - limitSwing[1]) : 0.0);

}

void PHBallJoint::Projection(double& f, int k){
	//今の力積の状態に対して何らかの操作を与える．
	//k:con[k]のkの部分(0～2:並進，3～5:回転)、fはそれに対応する力λのこと
	//力λ = 0 にすることで関節の拘束が解除される．
	//拘束条件が1→0に戻る時にLCPのλ(トルク)を無理やり0にしてw（速度・角速度）を求められるようにする．
	//< fMaxDt, fMinDtの条件では関節を拘束したいわけではないので，単なる上書きを行う．

	if (k==3){
		if(onLimit[0].onLower)
			f = max(0.0, f);
		else if(onLimit[0].onUpper)
			f = min(0.0, f);
		else if(fMaxDt < f)
			f = fMaxDt;
		else if(f < fMinDt)
			f = fMinDt;
	}

	if (k==5){
		if(onLimit[1].onLower)
			f = max(0.0, f);
		else if(onLimit[1].onUpper)
			f = min(0.0, f);
		else if(fMaxDt < f)
			f = fMaxDt;
		else if(f < fMinDt)
			f = fMinDt;
	}

}

//----------------------------------------------------------------------------
// PHBallJointNode
void PHBallJointNode::CompJointJacobian(){
	PHBallJoint* j = GetJoint();
	//SwingTwist& angle = (SwingTwist&)(j->position);
	//angle.Jacobian(Jst);
	//Matrix3d test = Jst * Jcinv;
	Quaterniond q = j->Xjrel.q;
	for(int i = 0; i < 3; i++)
		J.col(i).SUBVEC(0, 3).clear();
	/*J[0].w() = 2.0 * Vec3d(-q.x, -q.y, -q.z);
	J[1].w() = 2.0 * Vec3d( q.w,  q.z, -q.y);
    J[2].w() = 2.0 * Vec3d(-q.z,  q.w,  q.x);
    J[3].w() = 2.0 * Vec3d( q.y, -q.x,  q.w);*/
	J.col(0).SUBVEC(3, 3) = Vec3d(1.0, 0.0, 0.0);
	J.col(1).SUBVEC(3, 3) = Vec3d(0.0, 1.0, 0.0);
	J.col(2).SUBVEC(3, 3) = Vec3d(0.0, 0.0, 1.0);
	PHTreeNodeND<3>::CompJointJacobian();
}

void PHBallJointNode::CompJointCoriolisAccel(){
	//PHBallJoint* j = GetJoint();
	//cj.v().clear();
	//((SwingTwist&)(j->position)).Coriolis(cj.w(), j->velocity);
	//cj.w.clear();
	cj.clear();		//関節座標をquaternionにとる場合コリオリ項は0
}

void PHBallJointNode::UpdateJointPosition(double dt){
	PHBallJoint* j = GetJoint();
	j->Xjrel.q += j->Xjrel.q.Derivative(j->vjrel.w()) * dt;
	j->Xjrel.q.unitize();
}

void PHBallJointNode::CompRelativePosition(){
	PHBallJoint* j = GetJoint();
	j->Xjrel.r.clear();
	//j->Xjrel.qはUpdateJointPositionで更新済み
}
void PHBallJointNode::CompRelativeVelocity(){
	PHBallJoint* j = GetJoint();
	j->vjrel.v().clear();
	//j->vjrel.w() = ((Quaterniond&)j->position).AngularVelocity((Quaterniond&)j->velocity);
	j->vjrel.w() = j->velocity;
}

void PHBallJointNode::ModifyJacobian(){
	PHBallJoint* j = GetJoint();
	Jq = j->Jcinv;
}

void PHBallJointNode::CompBias(){
	dA.clear();
	db.clear();
}

void PHBallJointNode::Projection(double& f, int k){
	PHBallJoint* j = GetJoint();
	
	for(int i=0; k<2; k++){
		if(j->onLimit[i].onLower)
			f = max(0.0, f);
		if(j->onLimit[i].onUpper)
			f = min(0.0, f);
	}
}

}

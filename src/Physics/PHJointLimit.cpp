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

PHJointLimit1D::PHJointLimit1D(){
	f = F = 0.0;
	onLower = onUpper = false;
	A = Ainv = dA = b = db = 0;
}

void PHJointLimit1D::SetupLCP(){
	//可動範囲の判定
	onLower = onUpper = false;
	double l = joint->lower, u = joint->upper;
	double theta = joint->GetPosition();
	double diff;
	if(l < u){
		if(theta <= l){
			onLower = true;
			diff = joint->GetPosition() - l;
		}
		if(theta >= u){
			onUpper = true;
			diff = joint->GetPosition() - u;
		}
	}
	if(onLower || onUpper){
		double tmp = 1.0 / (joint->rangeDamper + joint->rangeSpring * joint->GetScene()->GetTimeStep());
		dA = tmp * joint->GetScene()->GetTimeStepInv();
		db = tmp * (joint->rangeSpring * diff);
		A  = joint->A[joint->axisIndex[0]];
		b  = joint->b[joint->axisIndex[0]];
		Ainv = 1.0 / (A + dA);
		f *= joint->engine->shrinkRate;

		joint->CompResponse(f, 0);
	}
	else f = 0.0;
}

void PHJointLimit1D::IterateLCP(){
	if(!onLower && !onUpper)
		return;

	int j = joint->axisIndex[0];
	double fnew = f - joint->engine->accelSOR * Ainv * (dA * f + b + db
			 + joint->J[0].row(j) * joint->solid[0]->dv + joint->J[1].row(j) * joint->solid[1]->dv);	

	if(onLower)
		fnew = max(0.0, fnew);
	if(onUpper)
		fnew = min(0.0, fnew);

	joint->CompResponse(fnew - f, 0);
	f = fnew;
}

void PHJointLimit1D::SetupCorrectionLCP(){

}

void PHJointLimit1D::IterateCorrectionLCP(){
	if(onLower)
		F = max(0.0, F);
	if(onUpper)
		F = min(0.0, F);
}

///////////////////////////////////////////////////////////////////////////////

PHBallJointLimit::PHBallJointLimit(){
	// limit上に居るかどうかのフラグの初期化
	for(int i=0; i<3; ++i){
		onLimit[i].onLower = false;
		onLimit[i].onUpper = false;
	}
	anyLimit = false;
	limitCount[0]	= 0;
	limitCount[1]	= 0;
}

bool PHBallJointLimit::SetConstLine(char* fileName , int i){
	limitCount[i] = 0;
	std::fstream ConstData(fileName,std::ios::in); //読み込みでファイルを開く
	if( ConstData.fail()){
		std::cout<<"error"<<std::endl;
		return false;
	}
	char txt[10];
	ConstData >> txt;
	while( !ConstData.eof()){
		if(i == 1){
			ConstData
				>> limitLine.SwingUp[limitCount[i]][0]
				>> limitLine.SwingUp[limitCount[i]][1]
				>> limitLine.SwingUp[limitCount[i]][2]
				>> limitLine.SwingUp[limitCount[i]][3]
				>> limitLine.SwingUp[limitCount[i]][4];
		}
		else if(i == 0){
			ConstData
				>> limitLine.SwingLow[limitCount[i]][0]
				>> limitLine.SwingLow[limitCount[i]][1]
				>> limitLine.SwingLow[limitCount[i]][2]
				>> limitLine.SwingLow[limitCount[i]][3]
				>> limitLine.SwingLow[limitCount[i]][4];
		}
		for(int j = 0; j<5;j++){
			if(i==1)
				limitLine.SwingUp[limitCount[i]][j] = Rad(limitLine.SwingUp[limitCount[i]][j]);
			if(i==0)			
				limitLine.SwingLow[limitCount[i]][j] = Rad(limitLine.SwingLow[limitCount[i]][j]);
		}
		if(limitCount[i] < 15) limitCount[i]++;
	}
	limitCount[i]--;
	if(i == 0 && limitCount[0] != 0){
		if(limitLine.SwingLow[limitCount[i]-1][0] != limitLine.SwingLow[0][0] || limitLine.SwingLow[limitCount[i]-1][1] != limitLine.SwingLow[0][1] || limitLine.SwingLow[limitCount[i]-1][2] != limitLine.SwingLow[0][2]
		|| limitLine.SwingLow[limitCount[i]-1][3] != limitLine.SwingLow[0][3] || limitLine.SwingLow[limitCount[i]-1][4] != limitLine.SwingLow[0][4]){
			if(limitLine.SwingLow[limitCount[i]-1][0] < Rad(360)){
				limitCount[i]++;
			}
			for(int j = 1; j < 5; j++){
				limitLine.SwingLow[limitCount[i] - 1][j] = limitLine.SwingLow[0][j];
			}
			limitLine.SwingLow[limitCount[i] - 1][0] = Rad(360);
		}
	}

	if(i == 1 && limitCount[1] != 0){
		if(limitLine.SwingUp[limitCount[i]-1][0] != limitLine.SwingUp[0][0] || limitLine.SwingUp[limitCount[i]-1][1] != limitLine.SwingUp[0][1] || limitLine.SwingUp[limitCount[i]-1][2] != limitLine.SwingUp[0][2]
		|| limitLine.SwingUp[limitCount[i]-1][3] != limitLine.SwingUp[0][3] || limitLine.SwingUp[limitCount[i]-1][4] != limitLine.SwingUp[0][4]){
			if(limitLine.SwingUp[limitCount[i]-1][0] < Rad(360)){
				limitCount[i]++;
			}
			for(int j = 1; j < 5; j++){
				limitLine.SwingUp[limitCount[i] - 1][j] = limitLine.SwingUp[0][j];
			}
			limitLine.SwingUp[limitCount[i] - 1][0] = Rad(360);
		}
	}

	onLimit[0][i] = false;
	onLimit[1][i] = false;
	return true;
}

// 重複コードを関数化
inline void CalcFrame(Matrix3d& R, const Vec3d& zdir, const Vec3d& xdir){
	R.Ez() = zdir;
	R.Ex() = xdir;
	R.Ey() = cross(R.Ez(), R.Ex());
	R.Ex() = cross(R.Ey(), R.Ez());		
}
inline Vec3d CalcDir(double theta, double phi){
	return Vec3d(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

void PHBallJointLimit::CheckLimit(){
	/*
	// -----bool* conの意味----------------------------------
	// 拘束する軸についてのフラグ
	// con[0]-con[2]:並進運動（x,y,z）
	// con[3]-con[5]:回転運動（x軸まわり,y軸まわり,z軸まわり）
	// true:拘束する、false:拘束しない
	//-------------------------------------------------------

	//con[0] = con[1] = con[2] = true;				
	//con[3] = con[4] = con[5] = false;
	*/

	// 可動域のチェック
	// 現在のSocketとPlugとの間の角度を計算
	double cosLange = dot(joint->limitDir, joint->Jc.Ez());
	if(cosLange > 1.0){
		cosLange = 1.0;
	}
	else if(cosLange < -1.0){
		cosLange = -1.0;
	}
	nowTheta[0]	= acos(cosLange);			///< Swing角の計算	

	Vec3d PolarCoord;
	PolarCoord = joint->Jc.Ez() * limDir.trans();					///< 倒れる方向の計算開始
	if(PolarCoord.x == 0){
		if(PolarCoord.y >= 0) nowTheta[2] = M_PI / 2;
		else nowTheta[2] = 3 * M_PI / 2;
	}
	else {
		nowTheta[2] = atan(PolarCoord.y / PolarCoord.x);
	}
	if (PolarCoord.x < 0) nowTheta[2] += M_PI;								///< 0 <= nowTheta[2] <= 2πの範囲に変更
	else if(PolarCoord.x > 0 && PolarCoord.y < 0) nowTheta[2] += 2 * M_PI;

	Quaterniond qSwing;
	Vec3d half =  0.5 * (Vec3d(0.0, 0.0, 1.0) + joint->Jc.Ez());	
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

	Quaterniond twistQ = qSwing * joint->Xjrel.q;
	nowTheta[1] = twistQ.Theta();						///< Twist角の計算を行っている	
	if (twistQ.z < 0)
		nowTheta[1]  *= -1;								///< Twist回転軸が反対を向くことがあるのでその対策
	
	if(nowTheta[1] < Rad(-180)) nowTheta[1] += Rad(360);
	if(nowTheta[1] > Rad( 180)) nowTheta[1] -= Rad(360);
	
	// 可動域制限にかかっているかの判定
	// swing角の可動域制限の確認
	if (limitCount[0] == 0 && (!onLimit[0].onUpper)){
		if (joint->limitSwing[0]<(FLT_MAX*0.1) /*joint->limitSwing[0]!=FLT_MAX*/ && nowTheta[0] < joint->limitSwing[0]){
			onLimit[0].onLower = true;
			Irrupt = nowTheta[0] - joint->limitSwing[0];
		}
		else onLimit[0].onLower = false;
	}	
	if(limitCount[1] == 0 && !onLimit[0].onLower){
		if(joint->limitSwing[1]<(FLT_MAX*0.1)/*joint->limitSwing[1]!=FLT_MAX*/ && nowTheta[0] > joint->limitSwing[1]){
			onLimit[0].onUpper = true;
			Irrupt = nowTheta[0] - joint->limitSwing[1];
		}
		else onLimit[0].onUpper = false;
	}
	// twist角の可動域制限の確認
	if(joint->limitTwist[0]<(FLT_MAX*0.1)/*joint->limitTwist[0] != FLT_MAX*/ && nowTheta[1] < joint->limitTwist[0])
		onLimit[1].onLower = true;
	else if(joint->limitTwist[1]<(FLT_MAX*0.1)/*joint->limitTwist[1] != FLT_MAX*/ && nowTheta[1] > joint->limitTwist[1])
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

	if(anyLimit){
		//ヤコビアンの取得＆更新
		J[0] = joint->J[0];
		J[1] = joint->J[1];
		J[0].wv() = joint->Jcinv * joint->J[0].wv();
		J[0].ww() = joint->Jcinv * joint->J[0].ww();
		J[1].wv() = joint->Jcinv * joint->J[1].wv();
		J[1].ww() = joint->Jcinv * joint->J[1].ww();
	}

	/* conの設定は不要になった
	// 上の計算を踏まえて毎回、回転軸の拘束条件の更新をする
	if(mode == PHJointDesc::MODE_POSITION){
		con[3] = onLimit[0].onUpper || onLimit[0].onLower || spring != 0.0 || damper != 0.0;
		con[4] = spring != 0.0	    || damper != 0.0;
		con[5] = onLimit[1].onUpper || onLimit[1].onLower || spring != 0.0 || damper != 0.0;
	} else if (mode == PHJointDesc::MODE_VELOCITY || mode == PHJointDesc::MODE_TRAJ){
		con[3] = true;
		con[4] = true;
		con[5] = true;
	} else {
		con[3] = onLimit[0].onUpper || onLimit[0].onLower;
		con[4] = false;
		con[5] = onLimit[1].onUpper || onLimit[1].onLower;
	}
	*/
}

void PHBallJointLimit::SetupLCP(){
	// 可動範囲チェック
	CheckLimit();

	// vJc : Jcによって写像される拘束座標系から見たPlugの角速度
	Vec3d vJc = joint->Jc * joint->vjrel.w();
	double dtinv = joint->GetScene()->GetTimeStepInv();
	double corRate = joint->engine->GetVelCorrectionRate();

	// 可動域フラグの指定onLimit[0]: swing, onLimit[1]: twist
	// nowTheta[0]: swing, nowTheta[1]: twist
	// 可動域制限を越えていたら、dA:関節を柔らかくする成分を0にする、db:侵入してきた分だけ元に戻す	

	// 以下はPHBallJointにもともとあったコード
	dA.clear();
	db.clear();
	// Swing角可動範囲
	if(onLimit[0].onLower){
		dA[0] = 0;
		db[0] = (nowTheta[0] - joint->limitSwing[0]) * dtinv * corRate;
	}
	else if(onLimit[0].onUpper){
		db[0] = (nowTheta[0] - joint->limitSwing[1] + 0.001) * dtinv * corRate;
	}

	// Twist角可動範囲
	if(onLimit[1].onLower && (vJc.z < 0)){
		dA[2] = 0;
		db[2] = (nowTheta[1] - joint->limitTwist[0]) * dtinv * corRate;
	}
	else if(onLimit[1].onUpper && (vJc.z > 0)){
		dA[2] = 0;
		db[2] = (nowTheta[1] - joint->limitTwist[1]) * dtinv * corRate;
	}
	for(int i = 0; i < 3; i++){
		A[i]  = joint->A[joint->axisIndex[i]];	//Aの回転成分を取得
		b[i]  = joint->b[joint->axisIndex[i]];
	}

	A = CompResponseMatrix();
	b = (J[0] * joint->solid[0]->v + J[1] * joint->solid[1]->v).w();
	for(int i = 0; i < 3; i++){
		Ainv[i] = 1.0 / (A[i] + dA[i]);
		joint->f[i] *= joint->engine->shrinkRate;
		joint->CompResponse(joint->f[i], i);
	}
	if(anyLimit)limitLine.SwingLow[0][0] = 1;
}

void PHBallJointLimit::Projection(double& f, int k){
	if (k == 0){
		if(onLimit[0].onLower && limitCount[0] == 0)
			f = max(0.0, f);
		else if(onLimit[0].onUpper || onLimit[0].onLower)	///< 誤りではないか？
			f = min(0.0, f);
	}
	if (k == 2){
		if(onLimit[1].onLower)
			f = max(0.0, f);
		else if(onLimit[1].onUpper)
			f = min(0.0, f);
	}
}

void PHBallJointLimit::IterateLCP(){
	//今の力積の状態に対して何らかの操作を与える．
	//k:con[k]のkの部分(0～2:並進，3～5:回転)、fはそれに対応する力λのこと
	//力λ = 0 にすることで関節の拘束が解除される．
	//拘束条件が1→0に戻る時にLCPのλ(トルク)を無理やり0にしてw（速度・角速度）を求められるようにする．
	//< fMaxDt, fMinDtの条件では関節を拘束したいわけではないので，単なる上書きを行う．

	if(!anyLimit)
		return;

	if(limitLine.SwingLow[0][0] == 1){
		limitLine.SwingLow[0][0] = 0;
		joint->solid[1]->dv.w() = Vec3d(0.0,0.0,0.0);
	}
	Vec3d fnew;
	for(int i = 0; i < 3; i++){
		int j = joint->axisIndex[i];
		fnew[i] = joint->limitf[i] - joint->engine->accelSOR * Ainv[i] * (dA[i] * joint->limitf[i] + b[i] + db[i]
			+ J[0].row(j) * joint->solid[0]->dv + J[1].row(j) * joint->solid[1]->dv);	
	}

	DSTR <<joint->limitf << endl;
	for(int i = 0; i < 3; i++){
		int j = joint->axisIndex[i];
		Projection(fnew[i], i);
//		joint->CompResponse(fnew[i] - joint->limitf[i], i);	

		SpatialVector dfs;
		for(int k = 0; k < 2; k++){
			if(!joint->solid[k]->IsDynamical() || !joint->IsInactive(k))continue;
			if(joint->solid[k]->IsArticulated()){
				(Vec6d&)dfs = J[k].row(i+3) * (fnew[i] - joint->limitf[i]);
				joint->solid[k]->treeNode->CompResponse(dfs, true, false);
			}
			else joint->solid[k]->dv += T[k].row(i+3) * (fnew[i] - joint->limitf[i]);
		}

		joint ->limitf[i] = fnew[i];
	}
}

Vec3d PHBallJointLimit::CompResponseMatrix(){
	int i, j;
	SpatialVector A;
	A.clear();
	PHRootNode* root[2];
	if(joint->solid[0]->IsArticulated())
		root[0] = joint->solid[0]->treeNode->GetRootNode();
	if(joint->solid[1]->IsArticulated())
		root[1] = joint->solid[1]->treeNode->GetRootNode();

	SpatialVector df;
	for(i = 0; i < 2; i++){
		if(joint->solid[i]->IsDynamical()){
			if(joint->solid[i]->IsArticulated()){
				for(j = 0; j < 6; j++){
					(Vec6d&)df = J[i].row(j);
					joint->solid[i]->treeNode->CompResponse(df, false, false);
					A[j] += J[i].row(j) * joint->solid[i]->treeNode->da;
					int ic = !i;
					//もう片方の剛体も同一のツリーに属する場合はその影響項も加算
					if(joint->solid[ic]->IsArticulated() && root[i] == root[ic])
						A[j] += J[ic].row(j) * joint->solid[ic]->treeNode->da;
				}
			}
			else{
				// T = M^-1 * J^T
				T[i].vv() = J[i].vv() * joint->solid[i]->minv;
				T[i].vw() = J[i].vw() * joint->solid[i]->Iinv;
				T[i].wv() = J[i].wv() * joint->solid[i]->minv;
				T[i].ww() = J[i].ww() * joint->solid[i]->Iinv;
				for(j = 0; j < 6; j++)
					// A == 論文中のJ * M^-1 * J^T, Gauss Seidel法のD
					A[j] += J[i].row(j) * T[i].row(j);
			}
		}
	}
	/** 最大の対角要素との比がepsよりも小さい対角要素がある場合，
		数値的不安定性の原因となるのでその成分は拘束対象から除外する
		＊epsを大きくとると，必要な拘束まで無効化されてしまうので、調整は慎重に。
	 */
	const double eps = 0.000001, epsabs = 1.0e-10;
	double Amax = 0.0, Amin;
	for(j = 0; j < 6; j++)
		if(joint->constr[j] && A[j] > Amax)
			Amax = A[j];
	Amin = Amax * eps;
	for(j = 0; j < 3; j++){
		if(!joint->constr[j+3])continue;
		if(A[j+3] < Amin || A[j+3] < epsabs){
			joint->constr[j+3] = false;
			DSTR << j << "-th constraint ill-conditioned! disabled." << endl;
		}
		else
			Ainv[j] = 1.0 / (A[j+3] + dA[j]);
	}
	return A.w();
}

}

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
#include <fstream>
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
	PoleTwist		= Vec2d(FLT_MAX,FLT_MAX);
	offset			= Vec3d();
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
	fMaxDt			= FLT_MAX;
	fMinDt			= -FLT_MAX;
	limitCount[0]	= 0;
	limitCount[1]	= 0;

}

void PHBallJoint::AfterSetDesc(){
	if (GetScene()){
		fMinDt = fMin * GetScene()->GetTimeStep();
		fMaxDt = fMax * GetScene()->GetTimeStep();
	}
	PHJointND<3>::AfterSetDesc();
	limitDir /= limitDir.norm();
	if(limitDir != Vec3d(0.0,0.0,1.0))
		limDir.Ex() = cross(Vec3d(0.0,0.0,1.0),limitDir);
	else limDir.Ex() = Vec3d(1.0,0.0,0.0);
	limDir.Ey() = cross(limitDir,limDir.Ex());
	limDir.Ez() = limitDir;
	BefPos   = Vec3d(0.0,0.0,0.0);

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
	
	// 可動域のチェック
	// 現在のSocketとPlugとの間の角度を計算
	nowTheta[0]	= acos(dot(limitDir, Jc.Ez()));			///< Swing角の計算	

	Vec3d PolarCoord;
	PolarCoord = Jc.Ez() * limDir.trans();					///< 倒れる方向の計算開始
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

	//swing角の可動域制限の確認
	if (limitCount[1] != 0){
		FunNum = 1;
		while(LimitLine.SwingUp[FunNum][0] < nowTheta[2]){				//現在の姿勢が拘束曲線のどの区間にあるのか調べる。
			FunNum++;
			if(limitCount[1] < FunNum) break;
		}
		double Herx,Swing;
		// スイング方向の拘束座標の制限値を求める
		Herx = (nowTheta[2] - LimitLine.SwingUp[FunNum-1][0]) / (LimitLine.SwingUp[FunNum][0] - LimitLine.SwingUp[FunNum-1][0]);				//スプライン曲線から拘束座標を求める
		limitSwing[1] = (2 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum][1] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
					+ (3 * LimitLine.SwingUp[FunNum][1] - 3 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx * Herx
					+ LimitLine.SwingUp[FunNum-1][4] * Herx + LimitLine.SwingUp[FunNum-1][1];
		if(nowTheta[0] >= limitSwing[1]){
			onLimit[0].onUpper = true;					//可動域を外れている場合には拘束を行う

			double SerchStep=0.001;		//近傍点を求めるための繰り返し計算のステップ幅
			int k = 30;					//近傍点を求めるための繰り返し計算のステップ数
			double IrruptUp;			//近傍点までの最短距離
			double minDif;			//近傍点のスイング方位角

			SerchStep = (nowTheta[2] - BefPos[2]) / 25;
			if(SerchStep > 0.005) SerchStep = 0.005;
			else if (SerchStep < -0.005) SerchStep = -0.005;
			int FunStack;				//拘束に利用する関数の番号を取得しておく

			Vec3d Neighbor;				//近傍点
			FunStack = FunNum;
			minDif = nowTheta[2];		//現段階での近傍点候補のスイング方位角の保持
			nowTheta[2] = BefPos[2];
			Neighbor.x = sin(limitSwing[1]) * cos(nowTheta[2]);			//ソケット座標系での近傍点の座標
			Neighbor.y = sin(limitSwing[1]) * sin(nowTheta[2]);
			Neighbor.z = cos(limitSwing[1]);
			Irrupt = (cross(Neighbor,Jc.Ez())).norm();				//現在の値と近傍点の角度の差　sin(Rad)

			// ステップごとに変化させていく
			for(int i = 0;i < k;i++){
				nowTheta[2] += SerchStep;
				if(nowTheta[2] >= Rad(360)){		//スイング方位角の範囲の制限 0<nowTheta[2]<360
					nowTheta[2] = 0;
					FunNum = 1;
				}
				else if(nowTheta[2] < 0){
					nowTheta[2] = Rad(360);
					FunNum = limitCount[1] - 1;
				}

				while(nowTheta[2] > LimitLine.SwingUp[FunNum][0]){ //拘束の曲線の関数の区間を超えていたら次の区間へ
					FunNum++;
				}
				while(nowTheta[2] < LimitLine.SwingUp[FunNum-1][0]){
					FunNum--;
				}
				Herx = (nowTheta[2] - LimitLine.SwingUp[FunNum-1][0]) / (LimitLine.SwingUp[FunNum][0] - LimitLine.SwingUp[FunNum-1][0]);	//範囲を0-1にしたときのx座標
				Swing = (2 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum][1] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
							+ (3 * LimitLine.SwingUp[FunNum][1] - 3 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx * Herx
							+ LimitLine.SwingUp[FunNum-1][4] * Herx + LimitLine.SwingUp[FunNum-1][1];	//計算位置でのSwing角の制限
				Neighbor.x = sin(Swing) * cos(nowTheta[2]);		//近傍点の座標（中心からのベクトル）を計算
				Neighbor.y = sin(Swing) * sin(nowTheta[2]);
				Neighbor.z = cos(Swing);
				IrruptUp = (cross(Neighbor,Jc.Ez())).norm();		//現在の値との距離のcosを求める
				if(Irrupt > IrruptUp){								//前の位置より近い近傍点を得られたら交換する
					Irrupt = IrruptUp;
					minDif = nowTheta[2];
					FunStack = FunNum;
				}
			}
			nowTheta[2] = minDif;			//近傍点のスイング方位角での拘束位置へのベクトルを計算する
			FunNum = FunStack;
			Herx = (nowTheta[2] - LimitLine.SwingUp[FunNum-1][0]) / (LimitLine.SwingUp[FunNum][0] - LimitLine.SwingUp[FunNum-1][0]);
			if(Herx > 1 || Herx < 0){
				FunNum = 1;
				while(LimitLine.SwingUp[FunNum][0] < nowTheta[2]){
					FunNum++;
					if(limitCount[1] < FunNum) break;
				}
				Herx = (nowTheta[2] - LimitLine.SwingUp[FunNum-1][0]) / (LimitLine.SwingUp[FunNum][0] - LimitLine.SwingUp[FunNum-1][0]);
			}
			limitSwing[1] = (2 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum][1] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingUp[FunNum][1] - 3 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingUp[FunNum-1][4] * Herx + LimitLine.SwingUp[FunNum-1][1];
			Neighbor.x = sin(limitSwing[1]) * cos(nowTheta[2]);		//近傍点の座標（中心からのベクトル）を計算
			Neighbor.y = sin(limitSwing[1]) * sin(nowTheta[2]);
			Neighbor.z = cos(limitSwing[1]);
			Irrupt = (cross(Neighbor,Jc.Ez())).norm();				//近傍点までの最短となる角度を求める
			Irrupt = asin(Irrupt);
			//ここまで
			//接線方向を求める
			tanLine.x = 3 * (2 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum][1] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx
							+ 2 * (3 * LimitLine.SwingUp[FunNum][1] - 3 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx
							+ LimitLine.SwingUp[FunNum-1][4]; //θ'の計算
			tanLine.z = -tanLine.x * sin(limitSwing[1]);
			tanLine.x *= cos(limitSwing[1]);
			tanLine.y = tanLine.x * sin(nowTheta[2]) + Neighbor.x;
			tanLine.x = tanLine.x * cos(nowTheta[2]) - Neighbor.y;
			tanLine = tanLine.unit();

			// 座標変換をすぐに行う
			Jc.Ez() = Xjrel.q * Vec3f(0.0, 0.0, 1.0);
			Jc.Ex() = tanLine;
			Jc.Ey() = cross(Jc.Ez(), Jc.Ex());
			Jc.Ex() = cross(Jc.Ey(), Jc.Ez());
			Jcinv   = Jc.trans();
		}
		else{
		onLimit[0].onUpper = false;
		}
	}
	else{ onLimit[0].onUpper = false;
	}
//Swing角の最小値を決める
	if (limitCount[0] != 0 && !onLimit[0].onUpper){
		FunNum = 1;
		while(LimitLine.SwingLow[FunNum][0] < nowTheta[2]){
			FunNum++;
			if(limitCount[0] < FunNum) break;
		}
		double Herx,Swing;
		// スイング方向の拘束座標の制限値を求める
		Herx = (nowTheta[2] - LimitLine.SwingLow[FunNum-1][0]) / (LimitLine.SwingLow[FunNum][0] - LimitLine.SwingLow[FunNum-1][0]);
		limitSwing[0] = (2 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum][1] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
					+ (3 * LimitLine.SwingLow[FunNum][1] - 3 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
					+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][1];
		if(nowTheta[0] <= limitSwing[0]){

			onLimit[0].onLower = true;
			double SerchStep = 0.001;
			int k = 30;
			SerchStep = (nowTheta[2] - BefPos[2]) / 25;
			if(SerchStep > 0.005) SerchStep = 0.005;
			else if (SerchStep < -0.005) SerchStep = -0.005;
			double IrruptUp;
			double minDif;
			int FunStack;
			Vec3d Neighbor;

			//近傍点を求める
			FunNum = 1;
			while(LimitLine.SwingLow[FunNum][0] < nowTheta[2]){
				FunNum++;
				if(limitCount[0] < FunNum + 1) break;
			}
			FunStack = FunNum;
			minDif = nowTheta[2];
			nowTheta[2] = BefPos[2];
			Herx = (nowTheta[2] - LimitLine.SwingLow[FunNum-1][0]) / (LimitLine.SwingLow[FunNum][0] - LimitLine.SwingLow[FunNum-1][0]);
			limitSwing[0] = (2 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum][1] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingLow[FunNum][1] - 3 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][1];
			Neighbor.x = sin(limitSwing[0]) * cos(nowTheta[2]);			//ソケット座標系での近傍点の座標
			Neighbor.y = sin(limitSwing[0]) * sin(nowTheta[2]);
			Neighbor.z = cos(limitSwing[0]);
			Irrupt = (cross(Neighbor,Jc.Ez())).norm();				//現在の値と近傍点の変移角度　sin(Rad)
			// ステップごとに変化させていく
			for(int i = 0;i < k;i++){
				nowTheta[2] += SerchStep;
				if(nowTheta[2] >= Rad(360)){		//xの範囲の制限
					nowTheta[2] = 0;
					FunNum = 1;
				}
				else if(nowTheta[2] < 0){
					nowTheta[2] = Rad(360);
					FunNum = limitCount[0] - 1;
				}
				while(nowTheta[2] > LimitLine.SwingLow[FunNum][0]){
					FunNum++;	//区間を超えていたら次の区間へ
				}
				while(nowTheta[2] < LimitLine.SwingLow[FunNum-1][0]){
					FunNum--;
				}
				Herx = (nowTheta[2] - LimitLine.SwingLow[FunNum-1][0]) / (LimitLine.SwingLow[FunNum][0] - LimitLine.SwingLow[FunNum-1][0]);	//範囲を0-1にしたときのx座標
				Swing = (2 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum][1] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
							+ (3 * LimitLine.SwingLow[FunNum][1] - 3 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
							+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][1];	//計算位置でのSwing角の制限
				Neighbor.x = sin(Swing) * cos(nowTheta[2]);		//近傍点の座標（中心からのベクトル）を計算
				Neighbor.y = sin(Swing) * sin(nowTheta[2]);
				Neighbor.z = cos(Swing);
				IrruptUp = (cross(Neighbor,Jc.Ez())).norm();		//現在の値との距離のsinを求める
				if(Irrupt > IrruptUp){
					Irrupt = IrruptUp;
					minDif = nowTheta[2];
					FunStack = FunNum;
				}
			}
			nowTheta[2] = minDif;			//近傍点のスイング方位角での拘束位置へのベクトルを計算する
			FunNum = FunStack;
			Herx = (nowTheta[2] - LimitLine.SwingLow[FunNum-1][0]) / (LimitLine.SwingLow[FunNum][0] - LimitLine.SwingLow[FunNum-1][0]);
			if(Herx > 1 || Herx < 0){
				FunNum = 1;
				while(LimitLine.SwingLow[FunNum][0] < nowTheta[2]){
					FunNum++;
					if(limitCount[1] < FunNum) break;
				}
				Herx = (nowTheta[2] - LimitLine.SwingLow[FunNum-1][0]) / (LimitLine.SwingLow[FunNum][0] - LimitLine.SwingLow[FunNum-1][0]);
			}
			limitSwing[1] = (2 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum][1] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingLow[FunNum][1] - 3 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][1];
			Neighbor.x = sin(limitSwing[1]) * cos(nowTheta[2]);		//近傍点の座標（中心からのベクトル）を計算
			Neighbor.y = sin(limitSwing[1]) * sin(nowTheta[2]);
			Neighbor.z = cos(limitSwing[1]);
			Irrupt = (cross(Neighbor,Jc.Ez())).norm();				//近傍点までの最短となる角度を求める
			Irrupt = asin(Irrupt);
			//ここまで
			//接線方向を求める
			tanLine.x = 3 * (2 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum][1] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx
							+ 2 * (3 * LimitLine.SwingLow[FunNum][1] - 3 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx
							+ LimitLine.SwingLow[FunNum-1][4]; //θ'の計算
			tanLine.z = -tanLine.x * sin(limitSwing[1]);
			tanLine.x *= cos(limitSwing[1]);
			tanLine.y = tanLine.x * sin(nowTheta[2]) + Neighbor.x;
			tanLine.x = tanLine.x * cos(nowTheta[2]) - Neighbor.y;
			tanLine = tanLine.unit();

			// 座標変換をすぐに行う
			Jc.Ez() = Xjrel.q * Vec3f(0.0, 0.0, 1.0);
			Jc.Ex() = -tanLine;
			Jc.Ey() = cross(Jc.Ez(), Jc.Ex());
			Jc.Ex() = cross(Jc.Ey(), Jc.Ez());
			Jcinv   = Jc.trans();
		}
		else{
		onLimit[0].onLower = false;
		}
	}
	else{onLimit[0].onLower = false;}
	BefPos = nowTheta;

	// swing角の可動域制限の確認
	if (limitCount[0] == 0 && (!onLimit[0].onUpper)){
		if (limitSwing[0]!=FLT_MAX && nowTheta[0] < limitSwing[0]){
			onLimit[0].onLower = true;
			Irrupt = nowTheta[0] - limitSwing[0];
		}
		else{
		onLimit[0].onLower = false;
		}
	}	
	if(limitCount[1] == 0 && !onLimit[0].onLower){
		if(limitSwing[1]!=FLT_MAX && nowTheta[0] > limitSwing[1]){
			onLimit[0].onUpper = true;
			Irrupt = nowTheta[0] - limitSwing[1];
		}
		else onLimit[0].onUpper = false;
	}
	//ツイスト角の可動域制限の確認
	if(PoleTwist.upper != FLT_MAX || PoleTwist.lower != FLT_MAX){
		double Herx,Theta;
		Vec2d LSwing,LimTwistL,LimTwistU;
		if(limitCount[1] != 0){		//スイング角の最大値があるとき最大値と最大ツイスト角を求める
			FunNum = 1;
			while(LimitLine.SwingUp[FunNum][0] < nowTheta[2]){
				FunNum++;
				if(limitCount[1] < FunNum) break;
			}
			Herx = (nowTheta[2] - LimitLine.SwingUp[FunNum-1][0]) / (LimitLine.SwingUp[FunNum][0] - LimitLine.SwingUp[FunNum-1][0]);
			LimTwistU.upper = (2 * LimitLine.SwingUp[FunNum-1][3] - 2 * LimitLine.SwingUp[FunNum][3] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingUp[FunNum][3] - 3 * LimitLine.SwingUp[FunNum-1][3] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingUp[FunNum-1][4] * Herx + LimitLine.SwingUp[FunNum-1][3];
			LSwing[1] = (2 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum][1] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingUp[FunNum][1] - 3 * LimitLine.SwingUp[FunNum-1][1] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingUp[FunNum-1][4] * Herx + LimitLine.SwingUp[FunNum-1][1];
			LimTwistU.lower = (2 * LimitLine.SwingUp[FunNum-1][2] - 2 * LimitLine.SwingUp[FunNum][2] + LimitLine.SwingUp[FunNum][4] + LimitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingUp[FunNum][2] - 3 * LimitLine.SwingUp[FunNum-1][2] - 2 * LimitLine.SwingUp[FunNum-1][4] - LimitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingUp[FunNum-1][4] * Herx + LimitLine.SwingUp[FunNum-1][2];
		}
		else{
			LimTwistU.upper = PoleTwist.upper;
			LSwing[1] = M_PI;
			LimTwistU.lower = PoleTwist.lower;
		}		
		//スイング角の最小値があるとき
		if(limitCount[0] != 0){
			FunNum = 1;
			while(LimitLine.SwingUp[FunNum][0] < nowTheta[2]){
				FunNum++;
				if(limitCount[1] < FunNum) break;
			}
			Herx = (nowTheta[2] - LimitLine.SwingLow[FunNum-1][0]) / (LimitLine.SwingLow[FunNum][0] - LimitLine.SwingLow[FunNum-1][0]);
			LimTwistL.lower = (2 * LimitLine.SwingLow[FunNum-1][2] - 2 * LimitLine.SwingLow[FunNum][2] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingLow[FunNum][2] - 3 * LimitLine.SwingLow[FunNum-1][2] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][2];
			LSwing[0] = (2 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum][1] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingLow[FunNum][1] - 3 * LimitLine.SwingLow[FunNum-1][1] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][1];
			LimTwistL.upper = (2 * LimitLine.SwingLow[FunNum-1][3] - 2 * LimitLine.SwingLow[FunNum][3] + LimitLine.SwingLow[FunNum][4] + LimitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * LimitLine.SwingLow[FunNum][3] - 3 * LimitLine.SwingLow[FunNum-1][3] - 2 * LimitLine.SwingLow[FunNum-1][4] - LimitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ LimitLine.SwingLow[FunNum-1][4] * Herx + LimitLine.SwingLow[FunNum-1][3];
		}
		else{
			LimTwistL.upper = PoleTwist.upper;
			LSwing[0] = 0;
			LimTwistL.lower = PoleTwist.lower;
		}
		if(nowTheta[0] > LSwing[1]) Theta = LSwing[1];
		else if(nowTheta[0] < LSwing[0]) Theta = LSwing[0];
		else Theta = nowTheta[0];
		for(int i = 0;i<2;i++){
			if(PoleTwist[i] != FLT_MAX){
				if(LSwing.lower - LSwing.upper){
					limitTwist[i] = ((LimTwistL[i] - LimTwistU[i]) * Theta + (LSwing.lower * LimTwistU[i] - LSwing.upper * LimTwistL[i])) / (LSwing.lower - LSwing.upper);
				}
				else if(i==0) limitTwist[i] = min(LimTwistL[i] , LimTwistU[i]);
				else limitTwist[i] = max(LimTwistL[i] , LimTwistU[i]);
			}
		}
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

void PHBallJoint::ControlCheck(double dtinv){
	// 位置制御の計算
	/*******************************************************************************************************
	足りない角度の差を回転軸ベクトルに変換．propV(田崎さんの論文でいうq[t])に対してdb.w()を計算している.
	自然長が0[rad]で，propV[rad]伸びた時に対しての角度バネを構成していると考えればいい．
	********************************************************************************************************/
	Quaterniond propQ = goal * Xjrel.q.Inv();	// Xjrel.qの目標goalとXjrel.qの実際の角度の差をQuaternionで取得
	Vec3d propV = propQ.RotationHalf();
	// 可動域制限がかかっている場合はpropの座標を変換して考えないといけない。
	if (anyLimit)
		propV = Jcinv * propV;

	if(mode == PHJointDesc::MODE_VELOCITY){
		if(anyLimit)
			db.w() = -Jcinv * desiredVelocity;
		else
			db.w() = - desiredVelocity;
	}else if(mode == PHJointDesc::MODE_TRAJ){
		if(anyLimit)
			db.w() = -Jcinv * (desiredVelocity + spring * propV);
		else
			db.w() = - (desiredVelocity + spring * propV);
	}else if (mode == PHJointDesc::MODE_POSITION){
		// バネダンパが入っていたら構築する
		if (spring != 0.0 || damper != 0.0){
			double tmp = 1.0 / (damper + spring * GetScene()->GetTimeStep());
			/*****************************************************************************
			w[t+1] = (A+dA) * λ[t+1] + (b+db)
			のAの内，バネダンパを構成するのは，BallJointの場合は回転の三軸についてなので，
			SpatialVector dA.w()にtmp = (D + K⊿t)^{-1}を入れている．
			dtinvをさらにかけるのは,pptなど表記とは違い，A=JM^{-1}J^T ⊿tのうち，
			プログラム中で計算しているのは，A = JM^{-1}J^Tだけ．
			あとから(A + dA)⊿tをしているため
			******************************************************************************/
			dA.w() = tmp * dtinv * Vec3d(1.0, 1.0, 1.0);

			/****
			田崎さんの論文の式25のdbに相当する．
			位置制御のみであれば，以下の式の1行目のみ．
			軌道追従制御では残りの2行もふくむ．offsetには外で計算してきた合成慣性テンソルを代入する
			****/
			db.w() = tmp * (- spring * propV
						 -    damper * desiredVelocity
						 -    offset);
		}
	}
}

void PHBallJoint::MovableCheck(double dtinv){
	// vJc : Jcによって写像される拘束座標系から見たPlugの角速度
	Vec3d vJc = Jc * vjrel.w();

	// 可動域フラグの指定onLimit[0]: swing, onLimit[1]: twist
	// nowTheta[0]: swing, nowTheta[1]: twist
	// 可動域制限を越えていたら、dA:関節を柔らかくする成分を0にする、db:侵入してきた分だけ元に戻す	
	// x軸方向に拘束をかける場合	
	if(onLimit[0].onLower && limitCount[0] == 0){
		double fCheck;
		double dbLCP = (Irrupt) * dtinv * engine->GetVelCorrectionRate();	//LCPによって加わる力
		double bSame = b[3] + J[0].row(3) * solid[0]->dv + J[1].row(3) * solid[1]->dv;
		fCheck = (A.w().x + dA.w().x) * A.w()[0] *													//拘束しない時の速度と、拘束するときの速度の差を計算 
					(A.w()[0] * (dA.w()[0] * f.w()[0] + db.w()[0]) - dA.w()[0] * (dbLCP + bSame));	//dA,db以外は過去の値を用いている
		if(fCheck < 0 ){																			//拘束違反をしないなら拘束しない
			db.w()[0] += dbLCP;
		}
		else{
			dA.w()[0] = 0;
			db.w()[0] = dbLCP;
		}
	}
	
	else if(onLimit[0].onUpper || onLimit[0].onLower){
		double fCheck;
		double dbLCP = (Irrupt) * dtinv * engine->GetVelCorrectionRate();
		double bSame = b[3] + J[0].row(3) * solid[0]->dv + J[1].row(3) * solid[1]->dv;
		fCheck = (A.w().x + dA.w().x) * A.w()[0] *
					(A.w()[0] * (dA.w()[0] * f.w()[0] + db.w()[0]) - dA.w()[0] * (dbLCP + bSame));
		if(fCheck > 0 ){
			db.w()[0] += dbLCP;
		}
		else{
			dA.w()[0] = 0;
			db.w()[0] = dbLCP;
		}
	}

	//z軸方向に拘束をかける場合
	if(onLimit[1].onLower && (vJc.z < 0)){
		double fCheck;
		double dbLCP = (nowTheta[1] - limitTwist[0]) * dtinv * engine->GetVelCorrectionRate();
		double bSame = b[5] + J[0].row(5) * solid[0]->dv + J[1].row(5) * solid[1]->dv;
		fCheck = (A.w().z + dA.w().z) * A.w()[2] *
					(A.w()[2] * (dA.w()[2] * f.w()[2] + db.w()[2]) - dA.w()[2] * (dbLCP + bSame));
		if(fCheck > 0 ){
			db.w()[2] += dbLCP;
		}
		else{
			dA.w()[2] = 0;
			db.w()[2] = dbLCP;
		}
	}

	else if(onLimit[1].onUpper && (vJc.z > 0)){
		double fCheck;
		double dbLCP = (nowTheta[1] - limitTwist[1]) * dtinv * engine->GetVelCorrectionRate();
		double bSame = b[5] + J[0].row(5) * solid[0]->dv + J[1].row(5) * solid[1]->dv;
		fCheck = (A.w().z + dA.w().z) * A.w()[2] *
					(A.w()[2] * (dA.w()[2] * f.w()[2] + db.w()[2]) - dA.w()[2] * (dbLCP + bSame));	
		if(fCheck > 0 ){																			
			db.w()[2] += dbLCP;
		}
		else{
			dA.w()[2] = 0;
			db.w()[2] = dbLCP;
		}
	}
}
void PHBallJoint::CompBias(){
	double dtinv = 1.0 / GetScene()->GetTimeStep();
	
	db.v() = Xjrel.r * dtinv;		//	並進誤差の解消のため、速度に誤差/dtを加算, Xjrel.r: ソケットに対するプラグの位置のズレ
	db.v() *= engine->velCorrectionRate;

	// 関節の制御を行う場合，行列A,ベクトルbを補正
	ControlCheck(dtinv);
	// 関節の可動域は制御よりも優先される．
	// 条件に応じて行列A,ベクトルbを補正
	MovableCheck(dtinv);
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
	Quaterniond propQ = goal * Xjrel.q.Inv();
	Vec3d propV = propQ.RotationHalf();
	if (k==3){
		if(onLimit[0].onLower && limitCount[0] == 0)
			f = max(0.0, f);
		else if(onLimit[0].onUpper||onLimit[0].onLower)
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
bool PHBallJoint::SetConstLine(char* fileName , int i){
	limitCount[i] = 0;
	std::fstream ConstData(fileName,std::ios::in); //読み込みでファイルを開く
	if( ConstData.fail()){std::cout<<"error"<<std::endl; return false;}
	char txt[10];
	ConstData >> txt;
	while( !ConstData.eof()){
		if(i == 1){
			ConstData >> LimitLine.SwingUp[limitCount[i]][0] >> LimitLine.SwingUp[limitCount[i]][1] >> LimitLine.SwingUp[limitCount[i]][2] >> LimitLine.SwingUp[limitCount[i]][3] >> LimitLine.SwingUp[limitCount[i]][4];
		}
		else if(i == 0){
			ConstData >> LimitLine.SwingLow[limitCount[i]][0] >> LimitLine.SwingLow[limitCount[i]][1] >> LimitLine.SwingLow[limitCount[i]][2] >> LimitLine.SwingLow[limitCount[i]][3] >> LimitLine.SwingLow[limitCount[i]][4];
		}
		for(int j = 0; j<5;j++){
			if(i==1)
				LimitLine.SwingUp[limitCount[i]][j] = Rad(LimitLine.SwingUp[limitCount[i]][j]);
			if(i==0)			
				LimitLine.SwingLow[limitCount[i]][j] = Rad(LimitLine.SwingLow[limitCount[i]][j]);
		}
		if(limitCount[i] < 15) limitCount[i]++;
	}
	limitCount[i]--;
	if(i == 0 && limitCount[0] != 0){
		if(LimitLine.SwingLow[limitCount[i]-1][0] != LimitLine.SwingLow[0][0] || LimitLine.SwingLow[limitCount[i]-1][1] != LimitLine.SwingLow[0][1] || LimitLine.SwingLow[limitCount[i]-1][2] != LimitLine.SwingLow[0][2]
		|| LimitLine.SwingLow[limitCount[i]-1][3] != LimitLine.SwingLow[0][3] || LimitLine.SwingLow[limitCount[i]-1][4] != LimitLine.SwingLow[0][4]){
			if(LimitLine.SwingLow[limitCount[i]-1][0] < Rad(360)){
				limitCount[i]++;
			}
			for(int j = 1; j < 5; j++){
				LimitLine.SwingLow[limitCount[i] - 1][j] = LimitLine.SwingLow[0][j];
			}
			LimitLine.SwingLow[limitCount[i] - 1][0] = Rad(360);
		}
	}

	if(i == 1 && limitCount[1] != 0){
		if(LimitLine.SwingUp[limitCount[i]-1][0] != LimitLine.SwingUp[0][0] || LimitLine.SwingUp[limitCount[i]-1][1] != LimitLine.SwingUp[0][1] || LimitLine.SwingUp[limitCount[i]-1][2] != LimitLine.SwingUp[0][2]
		|| LimitLine.SwingUp[limitCount[i]-1][3] != LimitLine.SwingUp[0][3] || LimitLine.SwingUp[limitCount[i]-1][4] != LimitLine.SwingUp[0][4]){
			if(LimitLine.SwingUp[limitCount[i]-1][0] < Rad(360)){
				limitCount[i]++;
			}
			for(int j = 1; j < 5; j++){
				LimitLine.SwingUp[limitCount[i] - 1][j] = LimitLine.SwingUp[0][j];
			}
			LimitLine.SwingUp[limitCount[i] - 1][0] = Rad(360);
		}
	}

	onLimit[0][i] = false;
	onLimit[1][i] = false;
	return true;
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

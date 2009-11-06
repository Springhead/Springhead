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
	/**
		花岡さんへの伝言：

		処理内容が把握できなかったので動作未確認状態で一旦コミットします。

		デスクリプタの内容を書き換えている部分がありますが、デスクリプタは読み取り専用です。
		定数ではなく状態であるならば場所を移すべきです。	tazz
	
		conの設定は不要になりました

		同じ処理は関数化してコードを軽量化してください。お願いします。
	*/

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
	nowTheta[0]	= acos(dot(joint->limitDir, joint->Jc.Ez()));			///< Swing角の計算	

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

	//swing角の可動域制限の確認
	if (limitCount[1] != 0){
		FunNum = 1;
		while(limitLine.SwingUp[FunNum][0] < nowTheta[2]){				//現在の姿勢が拘束曲線のどの区間にあるのか調べる。
			FunNum++;
			if(limitCount[1] < FunNum) break;
		}
		double Herx,Swing;
		// スイング方向の拘束座標の制限値を求める
		Herx = (nowTheta[2] - limitLine.SwingUp[FunNum-1][0]) / (limitLine.SwingUp[FunNum][0] - limitLine.SwingUp[FunNum-1][0]);				//スプライン曲線から拘束座標を求める
		
		joint->limitSwing[1] = (2 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum][1] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
					+ (3 * limitLine.SwingUp[FunNum][1] - 3 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx * Herx
					+ limitLine.SwingUp[FunNum-1][4] * Herx + limitLine.SwingUp[FunNum-1][1];
		if(nowTheta[0] >= joint->limitSwing[1]){
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
			Neighbor = CalcDir(joint->limitSwing[1], nowTheta[2]);	//ソケット座標系での近傍点の座標
			Irrupt = (cross(Neighbor, joint->Jc.Ez())).norm();				//現在の値と近傍点の角度の差　sin(Rad)

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

				while(nowTheta[2] > limitLine.SwingUp[FunNum][0]){ //拘束の曲線の関数の区間を超えていたら次の区間へ
					FunNum++;
				}
				while(nowTheta[2] < limitLine.SwingUp[FunNum-1][0]){
					FunNum--;
				}
				Herx = (nowTheta[2] - limitLine.SwingUp[FunNum-1][0]) / (limitLine.SwingUp[FunNum][0] - limitLine.SwingUp[FunNum-1][0]);	//範囲を0-1にしたときのx座標
				Swing = (2 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum][1] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
							+ (3 * limitLine.SwingUp[FunNum][1] - 3 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx * Herx
							+ limitLine.SwingUp[FunNum-1][4] * Herx + limitLine.SwingUp[FunNum-1][1];	//計算位置でのSwing角の制限
				Neighbor = CalcDir(Swing, nowTheta[2]);				//近傍点の座標（中心からのベクトル）を計算
				IrruptUp = (cross(Neighbor, joint->Jc.Ez())).norm();		//現在の値との距離のcosを求める
				if(Irrupt > IrruptUp){								//前の位置より近い近傍点を得られたら交換する
					Irrupt = IrruptUp;
					minDif = nowTheta[2];
					FunStack = FunNum;
				}
			}
			nowTheta[2] = minDif;			//近傍点のスイング方位角での拘束位置へのベクトルを計算する
			FunNum = FunStack;
			Herx = (nowTheta[2] - limitLine.SwingUp[FunNum-1][0]) / (limitLine.SwingUp[FunNum][0] - limitLine.SwingUp[FunNum-1][0]);
			if(Herx > 1 || Herx < 0){
				FunNum = 1;
				while(limitLine.SwingUp[FunNum][0] < nowTheta[2]){
					FunNum++;
					if(limitCount[1] < FunNum) break;
				}
				Herx = (nowTheta[2] - limitLine.SwingUp[FunNum-1][0]) / (limitLine.SwingUp[FunNum][0] - limitLine.SwingUp[FunNum-1][0]);
			}
			joint->limitSwing[1]
				= (2 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum][1] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
				+ (3 * limitLine.SwingUp[FunNum][1] - 3 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx * Herx
				+ limitLine.SwingUp[FunNum-1][4] * Herx + limitLine.SwingUp[FunNum-1][1];
			Neighbor = CalcDir(joint->limitSwing[1], nowTheta[2]);	//近傍点の座標（中心からのベクトル）を計算
			Irrupt = (cross(Neighbor, joint->Jc.Ez())).norm();				//近傍点までの最短となる角度を求める
			Irrupt = asin(Irrupt);
			//ここまで
			//接線方向を求める
			tanLine.x = 3 * (2 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum][1] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx
							+ 2 * (3 * limitLine.SwingUp[FunNum][1] - 3 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx
							+ limitLine.SwingUp[FunNum-1][4]; //θ'の計算
			tanLine.z = -tanLine.x * sin(joint->limitSwing[1]);
			tanLine.x *= cos(joint->limitSwing[1]);
			tanLine.y = tanLine.x * sin(nowTheta[2]) + Neighbor.x;
			tanLine.x = tanLine.x * cos(nowTheta[2]) - Neighbor.y;
			tanLine.unitize();

			// 座標変換をすぐに行う
			CalcFrame(joint->Jc, joint->Xjrel.q * Vec3d(0.0, 0.0, 1.0), tanLine);
			joint->Jcinv = joint->Jc.trans();
		}
		else{
			onLimit[0].onUpper = false;
		}
	}
	else{
		onLimit[0].onUpper = false;
	}
	//Swing角の最小値を決める
	if (limitCount[0] != 0 && !onLimit[0].onUpper){
		FunNum = 1;
		while(limitLine.SwingLow[FunNum][0] < nowTheta[2]){
			FunNum++;
			if(limitCount[0] < FunNum) break;
		}
		double Herx,Swing;
		// スイング方向の拘束座標の制限値を求める
		Herx = (nowTheta[2] - limitLine.SwingLow[FunNum-1][0]) / (limitLine.SwingLow[FunNum][0] - limitLine.SwingLow[FunNum-1][0]);
		joint->limitSwing[0] = (2 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum][1] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
					+ (3 * limitLine.SwingLow[FunNum][1] - 3 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
					+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][1];
		if(nowTheta[0] <= joint->limitSwing[0]){

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
			while(limitLine.SwingLow[FunNum][0] < nowTheta[2]){
				FunNum++;
				if(limitCount[0] < FunNum + 1) break;
			}
			FunStack = FunNum;
			minDif = nowTheta[2];
			nowTheta[2] = BefPos[2];
			Herx = (nowTheta[2] - limitLine.SwingLow[FunNum-1][0]) / (limitLine.SwingLow[FunNum][0] - limitLine.SwingLow[FunNum-1][0]);
			joint->limitSwing[0] = (2 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum][1] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingLow[FunNum][1] - 3 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][1];
			Neighbor = CalcDir(joint->limitSwing[0], nowTheta[2]);	//ソケット座標系での近傍点の座標
			Irrupt = (cross(Neighbor, joint->Jc.Ez())).norm();				//現在の値と近傍点の変移角度　sin(Rad)
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
				while(nowTheta[2] > limitLine.SwingLow[FunNum][0]){
					FunNum++;	//区間を超えていたら次の区間へ
				}
				while(nowTheta[2] < limitLine.SwingLow[FunNum-1][0]){
					FunNum--;
				}
				Herx = (nowTheta[2] - limitLine.SwingLow[FunNum-1][0]) / (limitLine.SwingLow[FunNum][0] - limitLine.SwingLow[FunNum-1][0]);	//範囲を0-1にしたときのx座標
				Swing = (2 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum][1] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
							+ (3 * limitLine.SwingLow[FunNum][1] - 3 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
							+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][1];	//計算位置でのSwing角の制限
				Neighbor = CalcDir(Swing, nowTheta[2]);				//近傍点の座標（中心からのベクトル）を計算		
				IrruptUp = (cross(Neighbor, joint->Jc.Ez())).norm();		//現在の値との距離のsinを求める
				if(Irrupt > IrruptUp){
					Irrupt = IrruptUp;
					minDif = nowTheta[2];
					FunStack = FunNum;
				}
			}
			nowTheta[2] = minDif;			//近傍点のスイング方位角での拘束位置へのベクトルを計算する
			FunNum = FunStack;
			Herx = (nowTheta[2] - limitLine.SwingLow[FunNum-1][0]) / (limitLine.SwingLow[FunNum][0] - limitLine.SwingLow[FunNum-1][0]);
			if(Herx > 1 || Herx < 0){
				FunNum = 1;
				while(limitLine.SwingLow[FunNum][0] < nowTheta[2]){
					FunNum++;
					if(limitCount[1] < FunNum) break;
				}
				Herx = (nowTheta[2] - limitLine.SwingLow[FunNum-1][0]) / (limitLine.SwingLow[FunNum][0] - limitLine.SwingLow[FunNum-1][0]);
			}
			joint->limitSwing[1] = (2 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum][1] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingLow[FunNum][1] - 3 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][1];
			Neighbor = CalcDir(joint->limitSwing[1], nowTheta[2]);	//近傍点の座標（中心からのベクトル）を計算
			Irrupt = (cross(Neighbor, joint->Jc.Ez())).norm();				//近傍点までの最短となる角度を求める
			Irrupt = asin(Irrupt);
			//ここまで
			//接線方向を求める
			tanLine.x = 3 * (2 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum][1] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx
							+ 2 * (3 * limitLine.SwingLow[FunNum][1] - 3 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx
							+ limitLine.SwingLow[FunNum-1][4]; //θ'の計算
			tanLine.z = -tanLine.x * sin(joint->limitSwing[1]);
			tanLine.x *= cos(joint->limitSwing[1]);
			tanLine.y = tanLine.x * sin(nowTheta[2]) + Neighbor.x;
			tanLine.x = tanLine.x * cos(nowTheta[2]) - Neighbor.y;
			tanLine.unitize();

			// 座標変換をすぐに行う
			CalcFrame(joint->Jc, joint->Xjrel.q * Vec3f(0.0, 0.0, 1.0), -tanLine);
			joint->Jcinv = joint->Jc.trans();
		}
		else{
			onLimit[0].onLower = false;
		}
	}
	else{
		onLimit[0].onLower = false;
	}
	BefPos = nowTheta;

	// swing角の可動域制限の確認
	if (limitCount[0] == 0 && (!onLimit[0].onUpper)){
		if (joint->limitSwing[0]!=FLT_MAX && nowTheta[0] < joint->limitSwing[0]){
			onLimit[0].onLower = true;
			Irrupt = nowTheta[0] - joint->limitSwing[0];
		}
		else onLimit[0].onLower = false;
	}	
	if(limitCount[1] == 0 && !onLimit[0].onLower){
		if(joint->limitSwing[1]!=FLT_MAX && nowTheta[0] > joint->limitSwing[1]){
			onLimit[0].onUpper = true;
			Irrupt = nowTheta[0] - joint->limitSwing[1];
		}
		else onLimit[0].onUpper = false;
	}
	//ツイスト角の可動域制限の確認
	if(joint->poleTwist.upper != FLT_MAX || joint->poleTwist.lower != FLT_MAX){
		double Herx,Theta;
		Vec2d LSwing,LimTwistL,LimTwistU;
		if(limitCount[1] != 0){		//スイング角の最大値があるとき最大値と最大ツイスト角を求める
			FunNum = 1;
			while(limitLine.SwingUp[FunNum][0] < nowTheta[2]){
				FunNum++;
				if(limitCount[1] < FunNum) break;
			}
			Herx = (nowTheta[2] - limitLine.SwingUp[FunNum-1][0]) / (limitLine.SwingUp[FunNum][0] - limitLine.SwingUp[FunNum-1][0]);
			LimTwistU.upper = (2 * limitLine.SwingUp[FunNum-1][3] - 2 * limitLine.SwingUp[FunNum][3] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingUp[FunNum][3] - 3 * limitLine.SwingUp[FunNum-1][3] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ limitLine.SwingUp[FunNum-1][4] * Herx + limitLine.SwingUp[FunNum-1][3];
			LSwing[1] = (2 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum][1] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingUp[FunNum][1] - 3 * limitLine.SwingUp[FunNum-1][1] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ limitLine.SwingUp[FunNum-1][4] * Herx + limitLine.SwingUp[FunNum-1][1];
			LimTwistU.lower = (2 * limitLine.SwingUp[FunNum-1][2] - 2 * limitLine.SwingUp[FunNum][2] + limitLine.SwingUp[FunNum][4] + limitLine.SwingUp[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingUp[FunNum][2] - 3 * limitLine.SwingUp[FunNum-1][2] - 2 * limitLine.SwingUp[FunNum-1][4] - limitLine.SwingUp[FunNum][4]) * Herx * Herx
						+ limitLine.SwingUp[FunNum-1][4] * Herx + limitLine.SwingUp[FunNum-1][2];
		}
		else{
			LimTwistU.upper = joint->poleTwist.upper;
			LSwing[1] = M_PI;
			LimTwistU.lower = joint->poleTwist.lower;
		}		
		//スイング角の最小値があるとき
		if(limitCount[0] != 0){
			FunNum = 1;
			while(limitLine.SwingUp[FunNum][0] < nowTheta[2]){
				FunNum++;
				if(limitCount[1] < FunNum) break;
			}
			Herx = (nowTheta[2] - limitLine.SwingLow[FunNum-1][0]) / (limitLine.SwingLow[FunNum][0] - limitLine.SwingLow[FunNum-1][0]);
			LimTwistL.lower = (2 * limitLine.SwingLow[FunNum-1][2] - 2 * limitLine.SwingLow[FunNum][2] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingLow[FunNum][2] - 3 * limitLine.SwingLow[FunNum-1][2] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][2];
			LSwing[0] = (2 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum][1] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingLow[FunNum][1] - 3 * limitLine.SwingLow[FunNum-1][1] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][1];
			LimTwistL.upper = (2 * limitLine.SwingLow[FunNum-1][3] - 2 * limitLine.SwingLow[FunNum][3] + limitLine.SwingLow[FunNum][4] + limitLine.SwingLow[FunNum-1][4]) * Herx * Herx * Herx
						+ (3 * limitLine.SwingLow[FunNum][3] - 3 * limitLine.SwingLow[FunNum-1][3] - 2 * limitLine.SwingLow[FunNum-1][4] - limitLine.SwingLow[FunNum][4]) * Herx * Herx
						+ limitLine.SwingLow[FunNum-1][4] * Herx + limitLine.SwingLow[FunNum-1][3];
		}
		else{
			LimTwistL.upper = joint->poleTwist.upper;
			LSwing[0] = 0;
			LimTwistL.lower = joint->poleTwist.lower;
		}
		if(nowTheta[0] > LSwing[1]) Theta = LSwing[1];
		else if(nowTheta[0] < LSwing[0]) Theta = LSwing[0];
		else Theta = nowTheta[0];
		for(int i = 0;i<2;i++){
			if(joint->poleTwist[i] != FLT_MAX){
				if(LSwing.lower - LSwing.upper){
					joint->limitTwist[i] = ((LimTwistL[i] - LimTwistU[i]) * Theta + (LSwing.lower * LimTwistU[i] - LSwing.upper * LimTwistL[i])) / (LSwing.lower - LSwing.upper);
				}
				else if(i==0) joint->limitTwist[i] = min(LimTwistL[i] , LimTwistU[i]);
				else joint->limitTwist[i] = max(LimTwistL[i] , LimTwistU[i]);
			}
		}
	}

	// twist角の可動域制限の確認
	if(joint->limitTwist[0] != FLT_MAX && nowTheta[1] < joint->limitTwist[0])
		onLimit[1].onLower = true;
	else if(joint->limitTwist[1] != FLT_MAX && nowTheta[1] > joint->limitTwist[1])
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
	// x軸方向に拘束をかける場合	
	if(onLimit[0].onUpper || onLimit[0].onLower){
		double dbLCP = (Irrupt) * dtinv * corRate;
		double fCheck;
		//fCheckは何を表しているのですか？
		//double bSame = b[3] + J[0].row(3) * solid[0]->dv + J[1].row(3) * solid[1]->dv;
		//fCheck = (A.w().x + dA.w().x) * A.w()[0] *
		//			(A.w()[0] * (dA.w()[0] * f.w()[0] + db.w()[0]) - dA.w()[0] * (dbLCP + bSame));
		if(fCheck > 0 ){
			db[0] += dbLCP;
		}
		else{
			dA[0] = 0;
			db[0] = dbLCP;
		}
	}

	//z軸方向に拘束をかける場合
	if(onLimit[1].onLower && (vJc.z < 0)){
		double dbLCP = (nowTheta[1] - joint->limitTwist[0]) * dtinv * corRate;
		double fCheck;
		//double bSame = b[5] + J[0].row(5) * solid[0]->dv + J[1].row(5) * solid[1]->dv;
		//fCheck = (A.w().z + dA.w().z) * A.w()[2] *
		//			(A.w()[2] * (dA.w()[2] * f.w()[2] + db.w()[2]) - dA.w()[2] * (dbLCP + bSame));
		if(fCheck > 0 ){
			db[2] += dbLCP;
		}
		else{
			dA[2] = 0;
			db[2] = dbLCP;
		}
	}
	if(onLimit[1].onUpper && (vJc.z > 0)){
		double dbLCP = (nowTheta[1] - joint->limitTwist[1]) * dtinv * corRate;
		double fCheck;
		//double bSame = b[5] + J[0].row(5) * solid[0]->dv + J[1].row(5) * solid[1]->dv;
		//fCheck = (A.w().z + dA.w().z) * A.w()[2] *
		//			(A.w()[2] * (dA.w()[2] * f.w()[2] + db.w()[2]) - dA.w()[2] * (dbLCP + bSame));	
		if(fCheck > 0 ){																			
			db[2] += dbLCP;
		}
		else{
			dA[2] = 0;
			db[2] = dbLCP;
		}
	}

	// limDir : Z軸がlimitDirに向く座標系．X軸がSocketのZ軸と直交するようにとる
	//limitDir.unitize();
	limDir.Ez() = joint->limitDir.unit();
	if(limDir.Ez() != Vec3d(0.0,0.0,1.0))		///< NG: Vec3d比較
		 limDir.Ex() = cross(Vec3d(0.0,0.0,1.0),limDir.Ez());
	else limDir.Ex() = Vec3d(1.0,0.0,0.0);
	limDir.Ey() = cross(limDir.Ez(), limDir.Ex());
	BefPos.clear();

	// 可動域フラグの指定onLimit[0]: swing, onLimit[1]: twist
	// nowTheta[0]: swing, nowTheta[1]: twist
	// 可動域制限を越えていたら、dA:関節を柔らかくする成分を0にする、db:侵入してきた分だけ元に戻す	

/*
	// 以下はPHBallJointにもともとあったコード
	dA.clear();
	db.clear();
	// Swing角可動範囲
	if(onLimit[0].onLower){
		dA[0] = 0;
		db[0] = (nowTheta[0] - joint->limitSwing[0]) * dtinv * corRate;
	}
	else if(onLimit[0].onUpper){
		dA[0] = 0;
		db[0] = (nowTheta[0] - joint->limitSwing[1]) * dtinv * corRate;
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
		A[i]  = joint->A[joint->axisIndex[i]];
		b[i]  = joint->b[joint->axisIndex[i]];
		Ainv[i] = 1.0 / (A[i] + dA[i]);
		f[i] *= joint->engine->shrinkRate;
		joint->CompResponse(f[i], i);
	}
*/
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

	Vec3d fnew;
	for(int i = 0; i < 3; i++){
		int j = joint->axisIndex[i];
		fnew[i] = joint->limitf[i] - joint->engine->accelSOR * Ainv[i] * (dA[i] * joint->limitf[i] + b[i] + db[i]
				+ joint->J[0].row(j) * joint->solid[0]->dv + joint->J[1].row(j) * joint->solid[1]->dv);	

		Projection(fnew[i], i);
		
		joint->CompResponse(fnew[i] - joint->limitf[i], i);
		joint->limitf[i] = fnew[i];
	}
	
}

}
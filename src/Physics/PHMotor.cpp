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

PHMotor1D::PHMotor1D(){
	f = 0.0;
}

void PHMotor1D::SetupLCP(){
	fMinDt = joint->fMin * joint->GetScene()->GetTimeStep();
	fMaxDt = joint->fMax * joint->GetScene()->GetTimeStep();

	// オフセット力のみ有効の場合は拘束力初期値に設定するだけでよい
	if(joint->spring == 0.0 && joint->damper == 0.0){
		dA = db = 0.0;
		f = joint->offsetForce;
	}
	else{
		A = joint->A[joint->axisIndex[0]];
		b = joint->b[joint->axisIndex[0]];

		if(joint->secondDamper == 0.0){
			double tmp = 1.0 / (joint->damper + joint->spring * joint->GetScene()->GetTimeStep());
			dA = tmp * joint->GetScene()->GetTimeStepInv();
			db = tmp * (joint->spring * joint->GetDeviation()
				- joint->damper * joint->targetVelocity - joint->offsetForce);
		}
		else{
			//3要素モデルの設定
			double dtinv = joint->GetScene()->GetTimeStepInv(), tmp, tmpA, tmpB;
			double D1 = joint->damper;
			double D2 = joint->secondDamper;
			double K = joint->spring;
			double h = joint->GetScene()->GetTimeStep();
			
			ws = joint->vjrel;	//バネのダンパの並列部の速さ
			tmp = D2-D1+K*h;
			xs[1] = ((D2-D1)/tmp)*xs[0] + (D2*h/(D2-D1))*ws;	//バネとダンパの並列部の距離の更新	
			tmpA = (D2-D1)*(D2-D1)/(D1*D2*tmp) ;
			tmpB = K*(D2-D1)*(D2-D1)/(D2*tmp*tmp)*(xs[0].w().z);
			dA = tmpA * dtinv;
			db = tmpB * (K * joint->GetDeviation() - D1 * joint->targetVelocity - joint->offsetForce);
			xs[0] = xs[1];	//バネとダンパの並列部の距離のステップを進める
		}
		Ainv = 1.0 / (A + dA);
		f *= joint->engine->shrinkRate;
	}
			
	// 拘束力初期値による速度変化量を計算
	joint->CompResponse(f, 0);
}

void PHMotor1D::IterateLCP(){
	if(joint->spring == 0.0 && joint->damper == 0.0)
		return;

	int j = joint->axisIndex[0];
	double fnew = f - joint->engine->accelSOR * Ainv * (dA * f + b + db
			 + joint->J[0].row(j) * joint->solid[0]->dv + joint->J[1].row(j) * joint->solid[1]->dv);

	// トルク制限
	fnew = min(max(fMinDt, fnew), fMaxDt);
	joint->CompResponse(fnew - f, 0);
	f = fnew;
}

////////////////////////////////////////////////////////////////////////////////////////

PHBallJointMotor::PHBallJointMotor(){
	yieldFlag	=  false;
	fMaxDt		=  FLT_MAX;
	fMinDt		= -FLT_MAX;
}

void PHBallJointMotor::ElasticDeformation(){
	double tmp = 1.0 / (joint->damper + joint->spring * dt);
	Vec3d I = joint->I, v0 = joint->targetVelocity, f0 = joint->offsetForce;
	for(int i=0;i<3;i++){
		dA[i] = tmp * dtinv * I[i];		
		db[i] = tmp * (- K * I[i] * propV[i] - D * I[i] * v0[i] - f0[i]);
	}
}

void PHBallJointMotor::PlasticDeformation(){
	//塑性変形(3要素モデル)
	D  *= joint->hardnessRate;
	D2 *= joint->hardnessRate;
	K  *= joint->hardnessRate;
	double tmp = D+D2+K*dt;
	ws = joint->vjrel;	//バネとダンパの並列部の速さ

	xs[1] = ((D+D2)/tmp)*xs[0] + (D2*dt/tmp)*ws;	//バネとダンパの並列部の距離の更新
	for(int i=0;i<3;i++){
		dA[i]= tmp/(D2*(K*dt+D)) * dtinv /I[i];
	}
	db = K/(K*dt+D)*(xs[0].w()) ;
	
	if(joint->type==PHBallJointDesc::Mix){
		if(ws.w().norm()<0.01){
			yieldFlag = false;
			joint->SetTargetPosition(joint->Xjrel.q);
		}
	}
	xs[0]=xs[1];	//バネとダンパの並列部の距離のステップを進める
}

void PHBallJointMotor::SetupLCP(){
	fMinDt = joint->fMin * joint->GetScene()->GetTimeStep();
	fMaxDt = joint->fMax * joint->GetScene()->GetTimeStep();
	dt		= joint->GetScene()->GetTimeStep();
	dtinv	= joint->GetScene()->GetTimeStepInv();
	D  = joint->damper;
	D2 = joint->secondDamper;
	K  = joint->spring;
	I  = joint->I;
	
	// オフセット力のみ有効の場合は拘束力初期値に設定するだけでよい
	if(K == 0.0 && D == 0.0){
		dA.clear();
		db.clear();
		f = joint->offsetForce;
	}
	else{
		// 位置制御の計算
		/*******************************************************************************************************
		足りない角度の差を回転軸ベクトルに変換．propV(田崎さんの論文でいうq[t])に対してdb.w()を計算している.
		自然長が0[rad]で，propV[rad]伸びた時に対しての角度バネを構成していると考えればいい．
		********************************************************************************************************/
		propQ = joint->targetPosition * joint->Xjrel.q.Inv();	// Xjrel.qの目標targetPositionとXjrel.qの実際の角度の差をQuaternionで取得
		propV = propQ.RotationHalf();

		A = joint->A.w();
		b = joint->b.w();
		
		if(joint->secondDamper == 0.0){
			/*****************************************************************************
			w[t+1] = (A+dA) * λ[t+1] + (b+db)
			のAの内，バネダンパを構成するのは，BallJointの場合は回転の三軸についてなので，
			SpatialVector dA.w()にtmp = (D + K⊿t)^{-1}を入れている．
			dtinvをさらにかけるのは,pptなど表記とは違い，A=JM^{-1}J^T ⊿tのうち，
			プログラム中で計算しているのは，A = JM^{-1}J^Tだけ．
			あとから(A + dA)⊿tをしているため
			******************************************************************************/
			double tmp = 1.0 / (D + K * dt);
			dA = tmp * dtinv * Vec3d(1.0, 1.0, 1.0);

			/****
			田崎さんの論文の式25のdbに相当する．
			位置制御のみであれば，以下の式の1行目のみ．
			軌道追従制御では残りの2行もふくむ．offsetには外で計算してきた合成慣性テンソルを代入する
			****/
			db = tmp * (- K * propV - D * joint->targetVelocity - joint->offsetForce);

			/*
			// 可動域制限がかかっている場合はpropの座標を変換して考えないといけない。
			if (anyLimit)
				propV = Jcinv * propV;

			if(mode == PHJointDesc::MODE_VELOCITY){
				if(anyLimit)
					db.w() = -Jcinv * targetVelocity;
				else
					db.w() = - targetVelocity;
			}else if(mode == PHJointDesc::MODE_TRAJ){
				if(anyLimit)
					db.w() = -Jcinv * (targetVelocity + spring * propV);
				else
					db.w() = - (targetVelocity + spring * propV);
			}else if (mode == PHJointDesc::MODE_POSITION){
				// バネダンパが入っていたら構築する
				if (spring != 0.0 || damper != 0.0){
					double tmp = 1.0 / (damper + spring * GetScene()->GetTimeStep());
				}
			}*/
		}
		else{
			//fの平均値を計算
			fs.push_back(f);
			if(fs.size()>5){
				vector<Vec3d>::iterator startIterator;
				startIterator = fs.begin();
				fs.erase( startIterator );
				fNorm=0;
				for(size_t i=0;i<fs.size();i++){
					fNorm+=fs[i].norm()/(fs.size()-1);
				}
			}
			//物体の形状を考慮したバネダンパを設定する場合
			if(I[0]!=1&&I[1]!=1&&I[2]!=1){
				//物体の変形に使用する場合
				/*x軸，y軸回りの変形(曲げ）
					I(断面2次モーメント),E(ヤング率),T(トルク),l(剛体間の距離)としたとき
	　				T=EIθ/l
				  z軸回りの変形（ねじり）
					G(せん断弾性係数),v(ポワソン比)
					G=E/2(1+v)
					T=GIθ/l 
				*/
				double v=0.3;		//ポワソン比は0.3ぐらいが多い

				//四角形の場合
				if(I[0]>I[1]){
					I[2]=I[1]*4/(2*(1+v));
				}else{
					I[2]=I[0]*4/(2*(1+v));
				}
			}

			if(fNorm > joint->yieldStress){
				yieldFlag = true;
			}
			switch(joint->type){
			case PHBallJointDesc::Mix:	//3:Mix 初期値
				if(yieldFlag)
					 PlasticDeformation();	//塑性変形
				else ElasticDeformation();	//弾性変形
				break;
			case PHBallJointDesc::Elastic:	//0:Elastic
				ElasticDeformation();
				break;
			case PHBallJointDesc::Plastic:	//1:Plastic
			default:
				PlasticDeformation();
			}
		}
		for(int i = 0; i < 3; i++)
			Ainv[i] = 1.0 / (A[i] + dA[i]);
		f *= joint->engine->shrinkRate;
	}
}

void PHBallJointMotor::IterateLCP(){
	if(K == 0.0 && D == 0.0)
		return;

	Vec3d fnew;
	for(int i = 0; i < 3; i++){
		int j = joint->axisIndex[i];
		fnew[i] = f[i] - joint->engine->accelSOR * Ainv[i] * (dA[i] * f[i] + b[i] + db[i]
				+ joint->J[0].row(j) * joint->solid[0]->dv + joint->J[1].row(j) * joint->solid[1]->dv);	

		if(fMaxDt < fnew[i])
			fnew[i] = fMaxDt;
		else if(f[i] < fMinDt)
			f[i] = fMinDt;
		
		joint->CompResponse(fnew[i] - f[i], i);
		f[i] = fnew[i];
	}
	
}

}
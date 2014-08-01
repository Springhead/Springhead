/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRReachController.h>
#include <Physics/SprPHScene.h>

#include <float.h>

#include <GL/glut.h>

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRReachController::Reset() {
	/*
	time      =  0;
	reachTime = -1;
	viaTime   = -1;
	*/

	if (bLookatMode) {
		Vec3d tipPos = GetTipPos();
		Vec3d tipDir = GetTipDir();
		double distance = (finalPos - tipPos).norm();
		currPos = tipPos + (tipDir * distance);
		bForceRestart = true;
		Step();
	} else {
		Vec3d tipPos = GetTipPos();
		currPos = tipPos;
		bForceRestart = true;
		Step();
	}
}

void CRReachController::Step() {
	if (ikEffs.size()==0) { return; }

	PHSceneIf* phScene = DCAST(PHSceneIf,ikEffs[0]->GetSolid()->GetScene());
	Vec3d tipOrigin = GetTipPos();

	/// --- マージンをとった目標位置を計算
	Vec3d marginalPos;
	if (!bLookatMode) {
		Vec3d dir = (currPos - finalPos);
		double distance = dir.norm();
		if (distance > 1e-5) { dir.unitize(); }
		if (distance > margin) {
			marginalPos = finalPos + (dir * margin);
		} else {
			if (distance < innerMargin) {
				marginalPos = finalPos + (dir * innerMargin);
			} else {
				marginalPos = currPos;
			}
		}
	} else {
		double cosThetaInit = ((currPos-tipOrigin).unit()) * ((finalPos-tipOrigin).unit());
		if (cosThetaInit < cos(margin)) {
			double s0=0, s1=1;
			for (int n=0; n<10; ++n) {
				double s2 = (s0 + s1) / 2;
				marginalPos = currPos + (finalPos - currPos) * s2;
				double cosTheta = ((marginalPos-tipOrigin).unit()) * ((finalPos-tipOrigin).unit());
				if (cosTheta > cos(margin)) { s1 = s2; } else { s0 = s2; }
			}
		} else {
			marginalPos = currPos;
		}
	}

	/// --- マージン後目標位置の変化速度を算出
	Vec3d mp;
	if (bLookatMode) {
		mp = (marginalPos - tipOrigin).unit() + tipOrigin;
	} else {
		mp = marginalPos;
	}

	Vec3d vMarginalPos = (mp - lastMarginalPos) / phScene->GetTimeStep();
	double alpha = 0.5;
	vMarginalPosLPF = alpha*vMarginalPos + (1-alpha)*vMarginalPosLPF;
	lastMarginalPos = mp;

	/// --- 必要に応じて目標位置の更新・再スタートの判断
	bool bRestart = false;
	if (bWaitingTargetSpeedDown) {
		if (vMarginalPosLPF.norm() < restartVel) {
			bWaitingTargetSpeedDown = false;
			targPos = marginalPos;
			targVel = finalVel;
			bRestart = true;
		}
	} else {
		if (vMarginalPos.norm() > waitVel) {
			bWaitingTargetSpeedDown = true;
		} else {
			targPos = marginalPos;
			targVel = finalVel;
		}
	}

	if (bForceRestart) {
		bWaitingTargetSpeedDown = false;
		targPos = marginalPos;
		targVel = finalVel;
		bRestart = true;
		bForceRestart = false;
	}

	/// --- 再スタート
	if (bRestart) {
		if (bFinished) {
			// 到達運動してない状態からスタートする場合はIKエンドエフェクタの内部位置を初期状態とする
			if (!bLookatMode) {
				currPos = GetTipPos();
			}
			currVel = Vec3d();
		}

		// 現在位置から滑らかに接続する
		initPos = currPos; initVel  = currVel;

		// 到達目標時刻のセット
		if (bLookatMode) {
			// -- なす角に応じて到達目標時刻をセットする
			Vec3d tipOrigin = GetTipPos();
			double theta = acos( ((currPos-tipOrigin).unit()) * ((finalPos-tipOrigin).unit()) );
			reachTime = abs(theta) / averageSpeed;
		} else {
			// -- 軌道長に応じて到達目標時刻をセットする
			reachTime = (targPos - currPos).norm() / averageSpeed; // GetLengthのためにはとりあえずreachTimeが必要なので
			reachTime = this->GetLength() / averageSpeed;
		}

		// 使う手を選択（手が複数あり、なおかつnumUseHandsが手の本数より小さい正の数の場合だけ）
		if (ikEffs.size() > 1 && numUseHands > 0 && numUseHands < ikEffs.size()) {
			ikEffUseFlags.resize(ikEffs.size());
			for (size_t i=0; i<ikEffUseFlags.size(); ++i) { ikEffUseFlags[i] = true; }

			for (size_t i=0; i<ikEffUseFlags.size(); ++i) {
				if (baseJoints[i]) {
					Posed pp; baseJoints[i]->GetPlugPose(pp);
					Vec3d joPos = baseJoints[i]->GetPlugSolid()->GetPose() * pp.Pos();
					double distance = (finalPos - joPos).norm();
					std::cout << baseJoints[i]->GetName() << " : " << distance << std::endl;
				}
			}

			// <!!>
			// i番目の手を使わない場合はikEffUseFlags[i]をfalseにする。
			// 何本の手をfalseにするかは numUseHands によって決める。
			// （未実装）
		}

		// 運動開始
		time = 0;
		bFinished = false;
	}

	/// --- 到達運動の計算と実行
	if (!bFinished) {
		Vec6d point = GetTrajectory(time / reachTime);
		currPos = point.r;
		currVel = point.v;

		if (bLookatMode) {
			for (size_t i=0; i<ikEffs.size(); ++i) {
				ikEffs[i]->SetTargetLookat(currPos); // <!!> Offset付けられるようにする？
			}
		} else {
			for (size_t i=0; i<ikEffs.size(); ++i) {
				if (!(i<ikEffUseFlags.size()) || ikEffUseFlags[i]) {
					ikEffs[i]->SetTargetPosition(currPos); // <!!> Offset付けられるようにする
					// ikEffs[i]->SetTargetVelocity(currVel);
				}
			}
		}
	}

	// FrontKeep();

	if (time <= reachTime) {
		time += phScene->GetTimeStep();
	} else {
		bFinished = true;
	}
}

Vec6d CRReachController::GetTrajectory(float s) {
	Vec6d point;

	if ((0.0 <= s) && (s <= 1.0) && ((initPos - targPos).norm() > 1e-5)) {

		for (int i=0; i<3; ++i) {
			PTM::TMatrixRow<6,6,double> A;
			PTM::TMatrixRow<6,1,double> b, x;

			if (viaTime > 0) {
				double t0 = 0, tF = reachTime, tV = viaTime * reachTime;
				A.row(0)=Vec6d(1, t0, pow(t0,2),   pow(t0,3),    pow(t0,4),    pow(t0,5)); b[0][0]=initPos[i];
				A.row(1)=Vec6d(0,  1,      2*t0, 3*pow(t0,2),  4*pow(t0,3),  5*pow(t0,4)); b[1][0]=initVel[i];
				A.row(2)=Vec6d(1, tV, pow(tV,2),   pow(tV,3),    pow(tV,4),    pow(tV,5)); b[2][0]=viaPos[i];
				A.row(3)=Vec6d(1, tF, pow(tF,2),   pow(tF,3),    pow(tF,4),    pow(tF,5)); b[3][0]=targPos[i];
				A.row(4)=Vec6d(0,  1,      2*tF, 3*pow(tF,2),  4*pow(tF,3),  5*pow(tF,4)); b[4][0]=targVel[i];
				A.row(5)=Vec6d(0,  0,         2,        6*tF, 12*pow(tF,2), 20*pow(tF,3)); b[5][0]=0;
			} else {
				double t0 = 0, tF = reachTime;
				A.row(0)=Vec6d(1, t0, pow(t0,2),   pow(t0,3),    pow(t0,4),    pow(t0,5)); b[0][0]=initPos[i];
				A.row(1)=Vec6d(0,  1,      2*t0, 3*pow(t0,2),  4*pow(t0,3),  5*pow(t0,4)); b[1][0]=initVel[i];
				A.row(2)=Vec6d(0,  0,         2,        6*t0, 12*pow(t0,2), 20*pow(t0,3)); b[2][0]=0;
				A.row(3)=Vec6d(1, tF, pow(tF,2),   pow(tF,3),    pow(tF,4),    pow(tF,5)); b[3][0]=targPos[i];
				A.row(4)=Vec6d(0,  1,      2*tF, 3*pow(tF,2),  4*pow(tF,3),  5*pow(tF,4)); b[4][0]=targVel[i];
				A.row(5)=Vec6d(0,  0,         2,        6*tF, 12*pow(tF,2), 20*pow(tF,3)); b[5][0]=0;
			}

			x = A.inv() * b;

			Vec6d pi; for(int n=0; n<6; ++n){ pi[n] = x[n][0]; }

			double t = s * reachTime;
			point.r[i] = pi * Vec6d(1, t, pow(t,2),   pow(t,3),    pow(t,4),    pow(t,5));
			point.v[i] = pi * Vec6d(0, 1,      2*t, 3*pow(t,2),  4*pow(t,3),  5*pow(t,4));
		}

	} else {
		point.r = targPos;
		point.v = targVel;
	}

	return point;
}

double CRReachController::GetLength() {
	if ((initPos - targPos).norm() < 1e-5) {
		return 0;
	}

	double length = 0;
	Vec3d  lastpos;
	for (int t_=0; t_<10; ++t_) {
		double t = (double)t_ / 10.0;
		Vec6d point = GetTrajectory(t);
		if (t_!=0) {
			length += (point.r - lastpos).norm();
		}
		lastpos = point.r;
	}

	return length;
}

void CRReachController::FrontKeep() {
	/*
	//初期化
	PHSceneIf* phScene = DCAST(PHSceneIf,ikEff->GetSolid()->GetScene());
	double s;
	Quaterniond qt;

	//到達姿勢の取得
	if(this->time == 0){
		states->SaveState(phScene);

		ikEff->EnableOrientationControl(enabled);
		ikEff->SetOriCtlMode(PHIKEndEffectorDesc::MODE_LOOKAT);


		ikEff->SetTargetLookat(finalPos);
		ikEff->SetTargetLocalDirection(Vec3d(0,0,-1));

		ikEff->SetTargetPosition(currPos);

		phScene->GetIKEngine()->SetNumIter(30);
		phScene->GetIKEngine()->Step();

		tempori = ikEff->GetSolidTempPose().Ori();
		
		
		std::cout << "ori" << ikEff->GetSolidTempPose().Ori() << std::endl;

		states->LoadState(phScene);
	}

	//到達姿勢までの補間を計算
	float tf = this->reachTime / this->reachTime;
	float t =  this->time / this->reachTime;

	if(this->reachTime == 0){
		s = 0;
	}
	if(t <= oricontTimeRatio){
		ikEff->SetOriCtlMode(PHIKEndEffectorDesc::MODE_QUATERNION);
		s = (oricontTimeRatio - t)/oricontTimeRatio;
	}else if(oricontTimeRatio < t){
		ikEff->SetOriCtlMode(PHIKEndEffectorDesc::MODE_LOOKAT);
		s = 0;
	}
	
	double Ratio = 1 - ( 10 * pow(s,3) - 15 * pow(s,4) + 6 * pow(s,5) );

	Vec3d rot = tempori.RotationHalf();
	rot = Ratio * rot;
	qt = Quaterniond::Rot(rot);

	//姿勢をIKにセット
	ikEff->SetTargetOrientation(qt);
	ikEff->EnableOrientationControl(enabled);
	*/
}

void CRReachController::Draw() {
	glLineWidth(2);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_LINE_STRIP);

	double currS;
	if (reachTime!=0) {
		currS = time / reachTime;
	} else {
		currS = 0;
	}

	for (int i=0; i<20; ++i) {
		double s = ((double)i)/20.0;

		if (s < currS) {
			glColor4f(1.0, 0.2, 0.2, 1.0);
		} else {
			glColor4f(0.2, 0.2, 1.0, 1.0);
		}

		Vec6f point = GetTrajectory(s);
		glVertex3f(point.r.x, point.r.y, point.r.z);
	}
	glEnd();

	Vec3d p1=initPos, p2=initPos+(initVel*0.1), p3=targPos, p4=targPos+(targVel*0.1);
	glBegin(GL_LINES);
	glColor4f(1.0, 1.0, 0.2, 1.0);
	glVertex3f(p1.x, p1.y, p1.z); glVertex3f(p2.x, p2.y, p2.z);
	glColor4f(0.2, 1.0, 1.0, 1.0);
	glVertex3f(p3.x, p3.y, p3.z); glVertex3f(p4.x, p4.y, p4.z);
	glEnd();

	glEnable(GL_DEPTH_TEST);
}

}
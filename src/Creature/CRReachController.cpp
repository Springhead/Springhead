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

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRReachController::Step() {
	PHSceneIf* phScene = DCAST(PHSceneIf,ikEff->GetSolid()->GetScene());

	// 手先位置・速度
	Vec3d tipPos = ikEff->GetSolid()->GetPose() * ikEff->GetTargetLocalPosition();
	Vec3d tipVel = ikEff->GetSolid()->GetVelocity() + (ikEff->GetSolid()->GetAngularVelocity() % tipPos);

	// マージン内外判定
	if (posInOut==CRReachControllerState::CRREACH_OUT) {
		if ((targetPos - tipPos).norm() <= posInnerMargin) {
			posInOut = CRReachControllerState::CRREACH_IN;
		}
	} else {
		if ((targetPos - tipPos).norm() >= posOuterMargin) {
			posInOut = CRReachControllerState::CRREACH_OUT;
		}
	}

	// マージン考慮済みの一時的な目標位置とその移動速度を計算
	if (posInOut==CRReachControllerState::CRREACH_OUT) {
		tempTargetPos = ((tipPos - targetPos).unit() * posInnerMargin) + targetPos;
	} else {
		tempTargetPos = tipPos;
	}
	Vec3d tempTargetVel = (tempTargetPos - lastTempTargetPos) / phScene->GetTimeStep();
	lastTempTargetPos = tempTargetPos;

	// 制御状態の更新
	if (posStat==CRReachControllerState::CRREACH_WAIT) {
		if ((time - posWaitStarted > minWait) && (tempTargetVel.norm() <= averageSpeed*0.9)) {
			// Calculate End Position
			int nIterOrig = phScene->GetIKEngine()->GetNumIter();
			states->SaveState(phScene);
			ikEff->SetTargetPosition(tempTargetPos);
			phScene->GetIKEngine()->SetNumIter(100);
			phScene->GetIKEngine()->Step();
			pos1 = ikEff->GetSolidTempPose().Pos();
			states->LoadState(phScene);
			phScene->GetIKEngine()->SetNumIter(nIterOrig);

			// Save Target Positions At Start Time
			tempTargetPosAtStartTime = tempTargetPos;
			pos1AtStartTime = pos1;

			// Start Trajectory Motion
			posStat = CRReachControllerState::CRREACH_TRAJ;
			time = 0;
			pos0 = tipPos;
			vel0 = tipVel*0.1; vel1 = Vec3d();
			currPos = pos0; currVel = vel0;
			reachTime = GetLength() / averageSpeed;
		}
	} else if (posStat==CRReachControllerState::CRREACH_TRAJ) {
		if (time > reachTime) {
			posStat = CRReachControllerState::CRREACH_FOLLOW;
			viaTime = -1;
		}
	} else if (posStat==CRReachControllerState::CRREACH_FOLLOW) {
		if (tempTargetVel.norm() > averageSpeed) {
			posStat = CRReachControllerState::CRREACH_WAIT;
			posWaitStarted = time;
		}
	} else {
		posStat = CRReachControllerState::CRREACH_WAIT;
	}

	// 到達運動の計算と実行
	if (posStat==CRReachControllerState::CRREACH_WAIT) {
		std::cout << "WAIT" << std::endl;
		// Proceed Wait Timer
		time += phScene->GetTimeStep();

	} else if (posStat==CRReachControllerState::CRREACH_TRAJ) {
		std::cout << "TRAJ" << std::endl;
		// Proceed Trajectory Timer
		time += phScene->GetTimeStep();

		// Update Trajectory End Point
		if (tempTargetVel.norm() < averageSpeed) {
			int nIterOrig = phScene->GetIKEngine()->GetNumIter();
			states->SaveState(phScene);
			ikEff->SetTargetPosition(tempTargetPos);
			phScene->GetIKEngine()->SetNumIter(100);
			phScene->GetIKEngine()->Step();
			pos1 = ikEff->GetSolidTempPose().Pos();
			states->LoadState(phScene);
			phScene->GetIKEngine()->SetNumIter(nIterOrig);

			// pos1 = pos1AtStartTime + (tempTargetPos - tempTargetPosAtStartTime);
		}

		// Update Position
		currPos = GetTrajectory(time / reachTime);
		ikEff->SetTargetPosition(currPos);

	} else if (posStat==CRReachControllerState::CRREACH_FOLLOW) {
		int nIterOrig = phScene->GetIKEngine()->GetNumIter();
		states->SaveState(phScene);
		ikEff->SetTargetPosition(tempTargetPos);
		phScene->GetIKEngine()->SetNumIter(100);
		phScene->GetIKEngine()->Step();
		Vec3d p = ikEff->GetSolidTempPose().Pos();
		states->LoadState(phScene);
		phScene->GetIKEngine()->SetNumIter(nIterOrig);

		ikEff->SetTargetPosition(p);

	}


#if 0
	/// --- 到達運動の判定と開始・停止
	Vec3d currEEfPos = ikEff->GetSolid()->GetPose() * ikEff->GetTargetLocalPosition();

	if ((lastStartedFinalPos - finalPos).norm() > restartDistance) {
		// 最終目標位置が許容範囲以上に変化したので到達運動を最初からやり直す
		currPos = currEEfPos;
		currVel = Vec3d();

		int nIterOrig = phScene->GetIKEngine()->GetNumIter();
		states->SaveState(phScene);
		ikEff->SetTargetPosition(finalPos);
		phScene->GetIKEngine()->SetNumIter(100);
		phScene->GetIKEngine()->Step();
		Vec3d tempPos = ikEff->GetSolidTempPose().Pos();
		states->LoadState(phScene);
		phScene->GetIKEngine()->SetNumIter(nIterOrig);


		std::cout << "come1" << std::endl;
		SetTargetPos(tempPos);
		SetTargetVel(finalVel);
		AutoStart();

		lastStartedFinalPos = finalPos;

		//std::cout << "length,reachTime:" <<(finalPos - currEEfPos).norm()<<","<< this->reachTime << std::endl;
	} else if ((targPos - finalPos).norm() > 1e-4) {
		/*
		if (reachTime <= time) {
			// とりあえず現在設定されている到達運動が完了したが、目標到達位置がその間に変わってしまった場合
			std::cout << "come2" << std::endl;
			currVel = Vec3d(); // <!!>
			// currPos = currEEfPos; // <!!>
			SetTargetPos(finalPos);
			SetTargetVel(finalVel);
			AutoStart();
		}
		*/

	} else {
		// 最終目標位置に届いていないので改めて到達運動を行う
		/*
		if (reachTime+reachTimeMargin <= time) {
			if ((finalPos - currEEfPos).norm() < acceptablePosError) { return; }
			std::cout << "come3" << std::endl;
			// currPos = currEEfPos;
			currVel = Vec3d(); // <!!>
			SetTargetPos(finalPos);
			SetTargetVel(finalVel);
			AutoStart();
		}
		*/
	}
	
	/// --- 到達運動の計算と実行

	if (time <= reachTime) {
		currPos = GetTrajectory(time / reachTime);
		ikEff->SetTargetPosition(currPos);
		// ikEff->SetTargetVelocity(currVel);

		// FrontKeep();
	}


	time += phScene->GetTimeStep();
#endif
}

Vec3d CRReachController::GetTrajectory(float s) {
	Vec3d pos;

	if ((pos0 - pos1).norm() > 1e-5) {

		for (int i=0; i<3; ++i) {
			PTM::TMatrixRow<6,6,double> A;
			PTM::TMatrixRow<6,1,double> b, x;

			if (viaTime > 0) {
				double t0 = 0, tF = 1, tV = viaTime / reachTime;
				A.row(0)=Vec6d(1, t0, pow(t0,2),   pow(t0,3),    pow(t0,4),    pow(t0,5)); b[0][0]=pos0[i];
				A.row(1)=Vec6d(0,  1,      2*t0, 3*pow(t0,2),  4*pow(t0,3),  5*pow(t0,4)); b[1][0]=vel0[i];
				A.row(2)=Vec6d(1, tV, pow(tV,2),   pow(tV,3),    pow(tV,4),    pow(tV,5)); b[2][0]=viaPos[i];
				A.row(3)=Vec6d(1, tF, pow(tF,2),   pow(tF,3),    pow(tF,4),    pow(tF,5)); b[3][0]=pos1[i];
				A.row(4)=Vec6d(0,  1,      2*tF, 3*pow(tF,2),  4*pow(tF,3),  5*pow(tF,4)); b[4][0]=vel1[i];
				A.row(5)=Vec6d(0,  0,         2,        6*tF, 12*pow(tF,2), 20*pow(tF,3)); b[5][0]=0;
			} else {
				double t0 = 0, tF = 1;
				A.row(0)=Vec6d(1, t0, pow(t0,2),   pow(t0,3),    pow(t0,4),    pow(t0,5)); b[0][0]=pos0[i];
				A.row(1)=Vec6d(0,  1,      2*t0, 3*pow(t0,2),  4*pow(t0,3),  5*pow(t0,4)); b[1][0]=vel0[i];
				A.row(2)=Vec6d(0,  0,         2,        6*t0, 12*pow(t0,2), 20*pow(t0,3)); b[2][0]=0;
				A.row(3)=Vec6d(1, tF, pow(tF,2),   pow(tF,3),    pow(tF,4),    pow(tF,5)); b[3][0]=pos1[i];
				A.row(4)=Vec6d(0,  1,      2*tF, 3*pow(tF,2),  4*pow(tF,3),  5*pow(tF,4)); b[4][0]=vel1[i];
				A.row(5)=Vec6d(0,  0,         2,        6*tF, 12*pow(tF,2), 20*pow(tF,3)); b[5][0]=0;
			}

			x = A.inv() * b;

			Vec6d pi; for(int n=0; n<6; ++n){ pi[n] = x[n][0]; }

			pos[i] = pi * Vec6d(1, s, pow(s,2),   pow(s,3),    pow(s,4),    pow(s,5));
		}

	} else {
		pos = pos1;
	}

	return pos;
}

double CRReachController::GetLength() {
	double length = 0;
	Vec3d  lastpos;
	for (int t_=0; t_<10; ++t_) {
		double t = (double)t_ / 10.0;
		Vec3d pos = GetTrajectory(t);
		if (t_!=0) { length += (pos - lastpos).norm(); }
		lastpos = pos;
	}
	return length;
}

void CRReachController::AutoStart() {
	/*
	if (this->reachTime <= 0) {
		// 待機中：手先の現在の位置を初期状態とする
		Vec3d eefPos  = ikEff->GetTargetLocalPosition();
		PHSolidIf* so = ikEff->GetSolid();
		currPos  = so->GetPose() * eefPos;
		currVel  = Vec3d(); // so->GetVelocity() + (so->GetAngularVelocity() % eefPos);
		currOri  = so->GetPose().Ori();
		currAVel = Vec3d(); // so->GetAngularVelocity();
	}

	// 現在位置から滑らかに接続する
	initPos = currPos; initVel  = currVel;
	initOri = currOri; initAVel = currAVel;

	// 軌道長に応じて到達目標時刻をセットする
	//this->reachTime = this->GetLength() / averageSpeed;
	this->reachTime = (targPos - currPos).norm() / averageSpeed;
	this->time      = 0;
	*/
}


void CRReachController::FrontKeep() {
	/*

	//初期化
	// states = ObjectStatesIf::Create();
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

}
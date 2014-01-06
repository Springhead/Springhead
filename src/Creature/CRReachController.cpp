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

	/// --- 到達運動の判定と開始・停止
	Vec3d currEEfPos = ikEff->GetSolid()->GetPose() * ikEff->GetTargetLocalPosition();

	if ((targPos - finalPos).norm() > restartDistance) {
		// 最終目標位置が許容範囲以上に変化したので到達運動を最初からやり直す
		// currPos = currEEfPos;
		double dist = (finalPos - currPos).norm();
		SetTargetPos(finalPos);
		SetTargetVel(finalVel);
		Start(dist / averageSpeed);

	} else if ((targPos - finalPos).norm() > 1e-4) {
		if (reachTime <= time) {
			// とりあえず現在設定されている到達運動が完了したが、目標到達位置がその間に変わってしまった場合
			double dist = (finalPos - currPos).norm();
			SetTargetPos(finalPos);
			SetTargetVel(finalVel);
			Start(dist / averageSpeed);
		}

	} else {
		if ((finalPos - currEEfPos).norm() < acceptablePosError) { return; }

		// 最終目標位置に届いていないので改めて到達運動を行う
		if (reachTime+reachTimeMargin <= time) {
			// currPos = currEEfPos;
			double dist = (finalPos - currPos).norm();
			SetTargetPos(finalPos);
			SetTargetVel(finalVel);
			Start(dist / averageSpeed);
		}
	}

	/// --- 到達運動の計算と実行

	if (time <= reachTime) {
		if ((initPos - targPos).norm() > 1e-5) {

			for (int i=0; i<3; ++i) {
				PTM::TMatrixRow<6,6,double> A;
				PTM::TMatrixRow<6,1,double> b, x;

				if (viaTime > 0) {
					double t0 = 0, tF = reachTime, tV = viaTime;
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

				double t = time;
				currPos[i] = pi * Vec6d(1, t, pow(t,2),   pow(t,3),    pow(t,4),    pow(t,5));
				currVel[i] = pi * Vec6d(0, 1,      2*t, 3*pow(t,2),  4*pow(t,3),  5*pow(t,4));
				// currAcc[i] = pi * Vec6d(0, 0,        2,        6*t, 12*pow(t,2), 20*pow(t,3));
			}

		} else {
			currPos = targPos;
			currVel = targVel;
		}

		ikEff->SetTargetPosition(currPos);
		// ikEff->SetTargetVelocity(currVel);
	}

	time += phScene->GetTimeStep();
}

}

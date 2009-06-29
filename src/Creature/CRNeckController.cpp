/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRNeckController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
void CRNeckController::LookAt(Vec3f pos, Vec3f vel, float attractiveness){
	/*
	this->pos = pos;
	this->vel = vel;
	this->attractiveness = attractiveness;
	*/
}

void CRNeckController::Init(){
	/*
	CRController::Init();

	cpHead = NULL;

	for (int i=0; i<creature->NBodies(); ++i) {
		CRHingeHumanBodyGenIf* hingeBody = DCAST(CRHingeHumanBodyGenIf, creature->GetBody(i));
		if (hingeBody) {
			soHead  = hingeBody->GetSolid(CRHingeHumanBodyGenDesc::SO_HEAD);
			soNeck  = hingeBody->GetSolid(CRHingeHumanBodyGenDesc::SO_NECK);
			soChest = hingeBody->GetSolid(CRHingeHumanBodyGenDesc::SO_CHEST);
			
			joNeckHeadX    = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyGenDesc::JO_NECK_HEAD_X));
			joChestNeckY   = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyGenDesc::JO_CHEST_NECK_Y));
			joAbdomenChest = DCAST(PHHingeJointIf, hingeBody->GetJoint(CRHingeHumanBodyGenDesc::JO_ABDOMEN_CHEST));
		}

		CRBallHumanBodyIf* ballBody = DCAST(CRBallHumanBodyIf, creature->GetBody(i));
		if (ballBody) {
			soHead  = ballBody->GetSolid(CRBallHumanBodyDesc::SO_HEAD);
			soChest = ballBody->GetSolid(CRBallHumanBodyDesc::SO_CHEST);
			cpHead  = DCAST(PHIKOriCtlIf,ballBody->GetControlPoint(2*CRBallHumanBodyDesc::SO_HEAD+1));
		}
	}

	origX = 0.0;
	origZ = 0.0;
	*/
}

void CRNeckController::Step(){
	#if 0
	CRController::Step();

	if (cpHead) {
		/*/
		/// 頭からみたターゲットの位置（グローバル）
		Vec3d dt  = (pos - soHead->GetPose().Pos());
		/// 胸剛体上の座標系に変換
		Vec3d etL = (soChest->GetPose().Ori().Inv() * dt).unit();
		/// y軸周りの回転角を算出
		Vec3d exz = etL; exz[1] = 0; exz = exz.unit();
		double tY = acos(PTM::dot(exz, Vec3d(0,0,-1)));
		if (exz[0] > 0) { tY = -tY; }
		/// x軸周りの回転角を算出
		Vec3d eyz = Quaterniond::Rot(-tY,'y') * etL;
		double tX = acos(PTM::dot(eyz, Vec3d(0,0,-1)));
		if (eyz[1] < 0) { tX = -tX; }
		/// 頭の回転を計算（胸剛体ローカル）
		Quaterniond qHL = Quaterniond::Rot(tY,'y') * Quaterniond::Rot(tX,'x');
		/// 頭の回転を計算（グローバル）
		Quaterniond qHG = soChest->GetPose().Ori() * qHL;

		cpHead->SetGoal(qHG);
		/**/

		/**/
		Vec3d rot = PTM::cross(soHead->GetPose().Ori()*Vec3d(0,0,-1), (pos-(soHead->GetPose().Pos())).unit());
		Quaterniond qt = Quaterniond::Rot(rot.norm(), rot.unit());
		cpHead->SetGoal(qt*soHead->GetPose().Ori());
		/**/

		/*
		std::cout << rot << std::endl;
		std::cout << qt*soHead->GetPose().Ori() << std::endl;
		std::cout << soHead->GetPose().Ori() << std::endl;
		*/

	} else {
		/// 頭基準座標系でのターゲットの位置
		Vec3d vecTargetFromHead = soHead->GetPose().Ori().Inv() * (pos - soHead->GetPose().Pos());
		/// Quaternionに変換
		Quaterniond qtnTargetFromHead;
		qtnTargetFromHead.RotationArc(vecTargetFromHead.unit(), Vec3d(0,0,-1));
		/// Euler角に変換
		Vec3d eulTargetFromHead;
		qtnTargetFromHead.ToEuler(eulTargetFromHead);
		double Z = eulTargetFromHead.Z(), X = eulTargetFromHead.X();

		/// Attractivenessに応じた割合に修正
		if (attractiveness < lowerAttractiveness) {
			X = origX;
			Z = origZ;
		} else if (attractiveness < upperAttractiveness) {
			X = origX + ((X-origX) * (attractiveness-lowerAttractiveness) / (upperAttractiveness-lowerAttractiveness));
			Z = origZ + ((Z-origZ) * (attractiveness-lowerAttractiveness) / (upperAttractiveness-lowerAttractiveness));
		} else {
			origX = X;
			origZ = Z;
		}

		float alpha = (float)0.9;
		origX = origX*alpha + X*(1.0-alpha);
		origZ = origZ*alpha + Z*(1.0-alpha);

		Vec3f dir = (pos - soChest->GetPose()*Vec3f(0,0,-10)).unit();
		soChest->AddForce(dir *  10.0f, soChest->GetPose()*Vec3f(0,0,-10));
		soChest->AddForce(dir * -10.0f, soChest->GetPose()*Vec3f(0,0, 10));

		/// 関節の目標値をセット
		double lower, upper;
		if (joNeckHeadX->GetChildObject(0)==soNeck) {
			joNeckHeadX->GetRange(lower, upper);
			if (lower < upper) {
				if (-Z < lower) {
					joNeckHeadX->SetSpringOrigin(lower);
				} else if (upper < -Z) {
					joNeckHeadX->SetSpringOrigin(upper);
				} else {
					joNeckHeadX->SetSpringOrigin(-Z);
				}
			} else {
				joNeckHeadX->SetSpringOrigin(-Z);
			}

			joChestNeckY->GetRange(lower, upper);
			if (lower < upper) {
				if (-X < lower) {
					joChestNeckY->SetSpringOrigin(lower);
				} else if (upper < -X) {
					joChestNeckY->SetSpringOrigin(upper);
				} else {
					joChestNeckY->SetSpringOrigin(-X);
				}
			} else {
				joChestNeckY->SetSpringOrigin(-X);
			}
		} else {
			joNeckHeadX->GetRange(lower, upper);
			if (lower < upper) {
				if (Z < lower) {
					joNeckHeadX->SetSpringOrigin(lower);
				} else if (upper < Z) {
					joNeckHeadX->SetSpringOrigin(upper);
				} else {
					joNeckHeadX->SetSpringOrigin(Z);
				}
			} else {
				joNeckHeadX->SetSpringOrigin(Z);
			}

			joChestNeckY->GetRange(lower, upper);
			if (lower < upper) {
				if (X < lower) {
					joChestNeckY->SetSpringOrigin(lower);
				} else if (upper < X) {
					joChestNeckY->SetSpringOrigin(upper);
				} else {
					joChestNeckY->SetSpringOrigin(X);
				}
			} else {
				joChestNeckY->SetSpringOrigin(X);
			}
		}
	}

	/*
	Quaterniond qLook;
	qLook.RotationArc(pos.unit(), Vec3d(0,0,-1));
	qLook = qLook * soChest->GetPose().Ori();

	PHBallJointDesc ballDesc   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0, 0.1, 0);
	ballDesc.posePlug.Ori()    = Quaternionf::Rot(Rad(0), 'y');
	ballDesc.poseSocket.Pos()  = Vec3d(0,-0.08,0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'y');
	ballDesc.spring            = 100.0;
	ballDesc.origin            = qLook;
	ballDesc.damper            = 50.0;
	joNeck->SetDesc(&ballDesc);
	*/
	#endif
}
}

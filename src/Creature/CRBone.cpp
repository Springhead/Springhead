/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBone.h>
#include <Creature/CRCreature.h>

#include <Physics/SprPHScene.h>
#include <Creature/SprCRBody.h>
#include <Creature/SprCREngine.h>

namespace Spr{;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

CRBoneIf* CRBone::GetParentBone() {
	if (actuator) {
		PHIKActuatorIf* paAct = actuator->GetParent();
		if (paAct) {
			for (int i=0; i<DCAST(CRCreatureIf,GetScene())->NBodies(); ++i) {
				CRBodyIf* body = DCAST(CRCreatureIf,GetScene())->GetBody(i);
				CRBoneIf* bone = body->FindByIKActuator(paAct);
				if (bone) { return bone; }
			}
		}
	}
	return NULL;
}

int CRBone::NChildBones() {
	int count = 0;
	if (actuator) {
		for (size_t i=0; i<actuator->NChildObject(); ++i) {
			PHIKActuatorIf* child = actuator->GetChildObject(i)->Cast();
			if (child) {
				for (int i=0; i<DCAST(CRCreatureIf,GetScene())->NBodies(); ++i) {
					CRBodyIf* body = DCAST(CRCreatureIf,GetScene())->GetBody(i);
					CRBoneIf* bone = body->FindByIKActuator(child);
					if (bone) { count++; }
				}
			}
		}
	}
	return count;
}

CRBoneIf* CRBone::GetChildBone(int number) {
	int count = -1;
	if (actuator) {
		for (int i=0; (size_t)i<actuator->NChildObject(); ++i) {
			PHIKActuatorIf* child = actuator->GetChildObject(i)->Cast();
			if (child) {
				for (int i=0; i<DCAST(CRCreatureIf,GetScene())->NBodies(); ++i) {
					CRBodyIf* body = DCAST(CRCreatureIf,GetScene())->GetBody(i);
					CRBoneIf* bone = body->FindByIKActuator(child);
					if (bone) {
						count++;
						if (count==number) { return bone; }
					}
				}
			}
		}
	}
	return NULL;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 軌道運動
void CRBone::SetOriginSolid(PHSolidIf* solid) {
	originSolid = solid;
}

void CRBone::AddTrajectoryNode(CRTrajectoryNode node, bool clear) {
	if (clear) {
		ClearTrajectory();
		PHSceneIf* phScene = solid->GetScene()->Cast();
		if (phScene) {
			this->time = phScene->GetTimeStep();
		}
	}

	bPlan = true;
	bool bAdded = false;
	for (std::deque<CRTrajectoryNode>::iterator it=trajNodes.begin(); it!=trajNodes.end(); ++it) {
		if (node.time <= it->time) { trajNodes.insert(it, node); bAdded=true; break; }
	}
	if (!bAdded) { trajNodes.push_back(node); }

	Plan();
	bPlan    = false;
	bChanged = true;
}

CRTrajectoryNode CRBone::GetTrajectoryNode(int i) {
	return trajNodes[i];
}

CRTrajectoryNode CRBone::GetTrajectoryNodeAt(float time) {
	/// セグメントが存在しない場合
	if        (trajNodes.size() == 1) {
		return trajNodes[0];
	} else if (trajNodes.size() <= 0) {
		CRTrajectoryNode node;
		node.time = 0.0f;
		return node;
	}

	/// 時刻がレンジ外の場合
	if (time < trajNodes[0].time)      { return trajNodes[0]; } 
	if (trajNodes.back().time <= time) { return trajNodes[trajNodes.size()-1]; }

	/// 時刻tに対応するセグメントを見つける
	size_t segment=0;
	for (; segment<trajNodes.size()-1; ++segment) {
		if (trajNodes[segment].time <= time && time < trajNodes[segment+1].time) { break; }
	}

	/// 当該セグメントから軌道を計算する
	/*
	Vec3d  r0 = trajNodes[segment].pose.Pos();
	Vec6d  v0 = trajNodes[segment].dpose;
	Vec3d  a0 = Vec3d();

	Vec3d  rf = trajNodes[segment+1].pose.Pos();
	Vec6d  vf = trajNodes[segment+1].dpose;
	Vec3d  af = Vec3d();
	double tf = trajNodes[segment+1].time - trajNodes[segment].time;
	*/

	Vec3d  rt = Vec3d();
	Vec3d  vt = Vec3d();
	Vec3d  at = Vec3d();

	float   t = time - trajNodes[segment].time;

	/**/
	for (int i=0; i<3; ++i) {
		rt[i] = trajNodes[segment].coeff[i] * Vec6d(1, t, pow(t,2),   pow(t,3),    pow(t,4),    pow(t,5));
		vt[i] = trajNodes[segment].coeff[i] * Vec6d(0, 1,      2*t, 3*pow(t,2),  4*pow(t,3),  5*pow(t,4));
		at[i] = trajNodes[segment].coeff[i] * Vec6d(0, 0,        2,        6*t, 12*pow(t,2), 20*pow(t,3));
	}

	/*/

	// 線形補間
	if (tf > 1e-3) {
		rt = (rf - r0) * (t / tf) + r0;
	} else {
		rt = r0;
	}
	/**/

	CRTrajectoryNode node;
	node.time = time;
	node.r    = rt;
	node.v    = vt;
	node.a    = at;

	return node;
}

void CRBone::SetTrajectoryNode(int i, CRTrajectoryNode node) {
	if (0<=i && (size_t)i<trajNodes.size()) {
		trajNodes[i] = node;
	} else {
		std::cout << "CRBone::SetTrajectoryNode() : Out of Range : " << i << std::endl; // <!!>
	}
}

CRTrajectoryNode CRBone::GetCurrentNode() {
	return current;
}

void CRBone::ClearTrajectory(bool apply) {
	CRTrajectoryNode current = GetCurrentNode();
	current.time = 0.0f;
	this->time   = 0.0f;

	// <!!>
	/**/
	if (solid && endeffector && apply) {
		Vec3d eefpos = endeffector->GetTargetLocalPosition();
		current.r = solid->GetPose() * eefpos;
		// current.v = solid->GetVelocity() + (solid->GetAngularVelocity() % eefpos);
		// current.a = eefAcc;
		endeffector->SetTargetPosition(current.r);
	}
	/**/

	trajNodes.clear();
	trajNodes.push_back(current);
	bCleared = true;
	bChanged = false;
}

void CRBone::StepTrajectory() {
	if (trajNodes.size() <= 1) { current.time=0.0f; time=0.0f; return; }
	if (trajNodes[trajNodes.size()-1].time < time) { ClearTrajectory(); return; }

	current = GetTrajectoryNodeAt(time);

	PHSceneIf* phScene = DCAST(PHSceneIf,solid->GetScene());
	PHSolidIf* soDebug = phScene->FindObject("soDebug")->Cast();
	if (soDebug) {
		soDebug->SetFramePosition(current.r);
	}


	if (!bCtlPos) {
		Vec3d tlp = endeffector ? endeffector->GetTargetLocalPosition() : Vec3d();
		initPos  = solid->GetPose() * tlp;
		finalPos = current.r;
		bCtlPos  = true;
		if (endeffector) {
			endeffector->Enable(true);
			endeffector->EnablePositionControl(true);
		}
	} else {
		finalPos = current.r;
	}
	if (originSolid) {
		initPos  = originSolid->GetPose().Inv() * initPos;
		finalPos = originSolid->GetPose().Inv() * finalPos;
	}


	if (endeffector) { endeffector->SetTargetPosition(current.r); }

	time += phScene->GetTimeStep();


	Vec3d eefpos = endeffector->GetTargetLocalPosition();
	Vec3d eefVel = solid->GetVelocity() + (solid->GetAngularVelocity() % eefpos);
	eefAcc = (eefVel - eefLastVel) / phScene->GetTimeStep();
	eefLastVel   = eefVel;
}

void CRBone::Plan() {
	if (trajNodes.size() <= 1) { return; }

	for (size_t segment=0; segment<trajNodes.size()-1; ++segment) {
		PlanSegment(trajNodes[segment], trajNodes[segment+1]);
	}
}

void CRBone::PlanSegment(CRTrajectoryNode &from, CRTrajectoryNode &to) {
	// Calc Coeff
	Vec3d  r0 = from.r;
	Vec6d  v0 = from.v;
	Vec3d  a0 = from.a;
	double t0 = 0.0;

	Vec3d  r1 = to.r;
	Vec6d  v1 = to.v;
	Vec3d  a1 = to.a;
	double t1 = to.time - from.time;

	for (int i=0; i<3; ++i) {
		PTM::TMatrixRow<6,6,double> A;
		A.row(0) = Vec6d( 1,  t0,  pow(t0,2),    pow(t0,3),     pow(t0,4),     pow(t0,5)); // r0
		A.row(1) = Vec6d( 0,   1,       2*t0,  3*pow(t0,2),   4*pow(t0,3),   5*pow(t0,4)); // v0
		A.row(2) = Vec6d( 0,   0,          2,         6*t0,  12*pow(t0,2),  20*pow(t0,3)); // a0
		A.row(3) = Vec6d( 1,  t1,  pow(t1,2),    pow(t1,3),     pow(t1,4),     pow(t1,5)); // r1
		A.row(4) = Vec6d( 0,   1,       2*t1,  3*pow(t1,2),   4*pow(t1,3),   5*pow(t1,4)); // v1
		A.row(5) = Vec6d( 0,   0,          2,         6*t1,  12*pow(t1,2),  20*pow(t1,3)); // a1

		PTM::TMatrixRow<6,1,double> b;
		b[0][0] = r0[i];
		b[1][0] = v0[i];
		b[2][0] = a0[i];
		b[3][0] = r1[i];
		b[4][0] = v1[i];
		b[5][0] = a1[i];

		PTM::TMatrixRow<6,1,double> pi = A.inv() * b;
		for (int n=0; n<6; ++n) { from.coeff[i][n] = pi[n][0]; }
	}
}

}
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
	if (clear) { ClearTrajectory(); }

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
	if (time < trajNodes[0].time)                  { return trajNodes[0]; } 
	if (trajNodes[trajNodes.size()-1].time < time) { return trajNodes[trajNodes.size()-1]; }

	/// 時刻tに対応するセグメントを見つける
	size_t segment=0;
	for (; segment<trajNodes.size()-1; ++segment) {
		if (trajNodes[segment].time <= time && time < trajNodes[segment+1].time) { break; }
	}

	/// 当該セグメントから軌道を計算する
	Vec3d  r0 = trajNodes[segment].pose.Pos();
	Vec6d  v0 = trajNodes[segment].dpose;
	Vec3d  a0 = Vec3d();

	Vec3d  rf = trajNodes[segment+1].pose.Pos();
	Vec6d  vf = trajNodes[segment+1].dpose;
	Vec3d  af = Vec3d();
	double tf = trajNodes[segment+1].time - trajNodes[segment].time;

	Vec3d  r1 = trajNodes[segment].viapose.Pos();
	double t1 = trajNodes[segment].viatime;

	Vec3d  rt = Vec3d();
	Vec3d  vt = Vec3d();

	float   t = time - trajNodes[segment].time;

	/**/
	for (int i=0; i<3; ++i) {
		Vec4d  pi = trajNodes[segment].coeff[i];
		if (t < t1) {
			rt[i] = r0[i] + v0[i]*t + 0.5*a0[i]*t*t
				+   pi[0]*pow(t,3) +   pi[1]*pow(t,4) +   pi[2]*pow(t,5);
			vt[i] =         v0[i]   +     a0[i]*t
				+ 3*pi[0]*pow(t,2) + 4*pi[1]*pow(t,3) + 5*pi[2]*pow(t,4);
		} else {
			rt[i] = r0[i] + v0[i]*t + 0.5*a0[i]*t*t
				+   pi[0]*pow(t,3) +   pi[1]*pow(t,4) +   pi[2]*pow(t,5)
				+   (pow(t-t1,5)*pi[3]/120.0);
			vt[i] =         v0[i]   +     a0[i]*t
				+ 3*pi[0]*pow(t,2) + 4*pi[1]*pow(t,3) + 5*pi[2]*pow(t,4)
				+ (5*pow(t-t1,4)*pi[3]/120.0);
		}
	}
	/**/

	// 線形補間
	// rt = (rf - r0) * (t / tf) + r0;

	CRTrajectoryNode node;
	node.time = time;
	node.pose.Pos() = rt;
	for (int i=0; i<3; ++i) { node.dpose[i] = vt[i]; }

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

void CRBone::ClearTrajectory() {
	CRTrajectoryNode current = GetCurrentNode();
	current.time = 0.0f;
	this->time   = 0.0f;

	// <!!>
	if (solid && endeffector) {
		current.pose = solid->GetPose() * endeffector->GetTargetLocalPosition();
	}

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
		soDebug->SetFramePosition(current.pose.Pos());
	}


	if (!bCtlPos) {
		Vec3d tlp = endeffector ? endeffector->GetTargetLocalPosition() : Vec3d();
		initPos  = solid->GetPose() * tlp;
		finalPos = current.pose.Pos();
		bCtlPos  = true;
		if (endeffector) {
			endeffector->Enable(true);
			endeffector->EnablePositionControl(true);
		}
	} else {
		finalPos = current.pose.Pos();
	}
	if (originSolid) {
		initPos  = originSolid->GetPose().Inv() * initPos;
		finalPos = originSolid->GetPose().Inv() * finalPos;
	}


	if (endeffector) { endeffector->SetTargetPosition(current.pose.Pos()); }

	time += phScene->GetTimeStep();
}

void CRBone::Plan() {
	if (trajNodes.size() <= 1) { return; }

	for (size_t segment=0; segment<trajNodes.size()-1; ++segment) {
		trajNodes[segment].viapose.Pos() = (trajNodes[segment].pose.Pos() + trajNodes[segment+1].pose.Pos()) * 0.5;
		trajNodes[segment].viatime       =  (trajNodes[segment+1].time - trajNodes[segment].time) * 0.5;
		PlanSegment(trajNodes[segment], trajNodes[segment+1]);

		// <!!> ちょっとNaiveすぎるかも．要改善
		CRTrajectoryNode candidates[300];
		for (int i=0; i<300; ++i) {
			candidates[i] = trajNodes[segment];
			Vec2d theta = Vec2d(
				(((double)rand()/(double)RAND_MAX)*1.0 - 0.5) * 3.14159,
				(((double)rand()/(double)RAND_MAX)*2.0 - 1.0) * 3.14159
				);
			Quaterniond rot = Quaterniond::Rot(theta[1],'y') * Quaterniond::Rot(theta[0],'x');
			candidates[i].viapose.Pos() += (rot * (candidates[i].viapose.Pos() - trajNodes[segment].pose.Pos()));
			candidates[i].viatime = (((double)rand()/(double)RAND_MAX)*0.2 + 0.4) * (trajNodes[segment+1].time - trajNodes[segment].time);

			PlanSegment(candidates[i], trajNodes[segment+1]);
			if (candidates[i].length < trajNodes[segment].length) {
				trajNodes[segment] = candidates[i];
			}
		}		
	}
}

void CRBone::PlanSegment(CRTrajectoryNode &from, CRTrajectoryNode &to) {
	// Calc Coeff
	Vec3d  r0 = from.pose.Pos();
	Vec6d  v0 = from.dpose;
	Vec3d  a0 = Vec3d();

	Vec3d  rf = to.pose.Pos();
	Vec6d  vf = to.dpose;
	Vec3d  af = Vec3d();
	double tf = to.time - from.time;

	Vec3d  r1 = from.viapose.Pos(); // (r0 + rf) * 0.5;
	double t1 = from.viatime;       // tf * 0.5;

	Vec3d  rt = Vec3d();

	for (int i=0; i<3; ++i) {
		PTM::TMatrixRow<4,4,double> A;
		A.row(0) = Vec4d(    pow(t1,3),    pow(t1,4),    pow(t1,5), 0                   );
		A.row(1) = Vec4d(    pow(tf,3),    pow(tf,4),    pow(tf,5), pow(tf-t1,5)/120.0  );
		A.row(2) = Vec4d(  3*pow(tf,2),  4*pow(tf,3),  5*pow(tf,4), pow(tf-t1,4)/24.0   );
		A.row(3) = Vec4d(  6*pow(tf,1), 12*pow(tf,2), 20*pow(tf,3), pow(tf-t1,3)/6.0    );

		PTM::TMatrixRow<4,1,double> b;
		b[0][0] = r1[i] - (r0[i] + v0[i]*t1 + 0.5*a0[i]*t1*t1);
		b[1][0] = rf[i] - (r0[i] + v0[i]*tf + 0.5*a0[i]*tf*tf);
		b[2][0] = vf[i] - (v0[i]            +     a0[i]*tf);
		b[3][0] = af[i] -                         a0[i];

		PTM::TMatrixRow<4,1,double> pi = A.inv() * b;
		for (int n=0; n<4; ++n) { from.coeff[i][n] = pi[n][0]; }
	}

	// Get Total Jerk
	Vec3d  pos  = r0;
	double jerk = 0.0;
	int sep = 20;
	for (int s=0; s<sep; ++s) {
		double t  = ((double)s / (double)sep) * tf;
		Vec3d  jk = Vec3d();

		for (int i=0; i<3; ++i) {
			Vec4d  pi = from.coeff[i];
			if (t < t1) {
				jk[i] = a0[i] + 6*pi[0]*t + 12*pi[1]*pow(t,2) + 20*pi[2]*pow(t,3);
			} else {
				jk[i] = a0[i] + 6*pi[0]*t + 12*pi[1]*pow(t,2) + 20*pi[2]*pow(t,3)
					+ (20*pow(t-t1,3)*pi[3]/120.0);
			}
		}

		jerk += jk.norm();
		pos = rt;
	}
	if (jerk != 0) { from.length = jerk; }
}

}
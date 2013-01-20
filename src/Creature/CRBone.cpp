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
// ãOìπâ^ìÆ
void CRBone::SetOriginSolid(PHSolidIf* solid) {
	originSolid = solid;
}

void CRBone::AddTrajectoryNode(CRTrajectoryNode node, bool clear) {
	// std::cout << this->GetPHSolid()->GetName() << " node Added" << std::endl;
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
	/// ÉZÉOÉÅÉìÉgÇ™ë∂ç›ÇµÇ»Ç¢èÍçá
	if        (trajNodes.size() == 1) {
		return trajNodes[0];
	} else if (trajNodes.size() <= 0) {
		CRTrajectoryNode node;
		node.time = 0.0f;
		return node;
	}

	/// éûçèÇ™ÉåÉìÉWäOÇÃèÍçá
	if (time < trajNodes[0].time)                  { return trajNodes[0]; } 
	if (trajNodes[trajNodes.size()-1].time < time) { return trajNodes[trajNodes.size()-1]; }

	/// éûçètÇ…ëŒâûÇ∑ÇÈÉZÉOÉÅÉìÉgÇå©Ç¬ÇØÇÈ
	size_t segment=0;
	for (; segment<trajNodes.size()-1; ++segment) {
		if (trajNodes[segment].time <= time && time < trajNodes[segment+1].time) { break; }
	}

	/// ìñäYÉZÉOÉÅÉìÉgÇ©ÇÁãOìπÇåvéZÇ∑ÇÈ
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

	// ê¸å`ï‚ä‘
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

	trajNodes.clear();
	trajNodes.push_back(current);
	bCleared = true;
	bChanged = false;
}

void CRBone::StepTrajectory() {
	/// Ç∆ÇËÇ†Ç¶Ç∏ç°ÇÕìÆÇ©Ç≥Ç»Ç¢ÅBå„Ç≈Enableä÷êîÇ¬ÇØÇÈÇ»ÇËÇ∑ÇÈÇ±Ç∆ <!!>
	/**/
	if (trajNodes.size() <= 1) { current.time=0.0f; time=0.0f; return; }
	if (trajNodes[trajNodes.size()-1].time < time) { ClearTrajectory(); return; }

	current = GetTrajectoryNodeAt(time);

	PHSceneIf* phScene = DCAST(PHSceneIf,solid->GetScene());
	PHSolidIf* soDebug = phScene->FindObject("soDebug")->Cast();
	if (soDebug) {
		soDebug->SetFramePosition(current.pose.Pos());
	}

	SetTargetPos(current.pose.Pos());
	if (endeffector) { endeffector->SetTargetPosition(current.pose.Pos()); }

	time += phScene->GetTimeStep();
	/**/

	// Ç±Ç±Ç‹Ç≈






	// <!!>


	/*
	// --
	PHSolidIf* soDebug = DCAST(CRCreatureIf,GetScene())->GetPHScene()->FindObject("soDebug")->Cast();
	if (soDebug) {
		DCAST(CRCreatureIf,GetScene())->GetPHScene()->SetContactMode(soDebug, PHSceneDesc::MODE_NONE);
	}
	// --

	double dt = DCAST(CRCreatureIf,GetScene())->GetPHScene()->GetTimeStep();

	// à íuêßå‰
	Vec3f finalPosAbs=finalPos, initPosAbs=initPos;
	if (originSolid) {
		initPosAbs  = originSolid->GetPose() * initPosAbs;
		finalPosAbs = originSolid->GetPose() * finalPosAbs;
	}

	if (bCtlPos && bEnable) {
		time += (float)dt;

		//	ê≥ãKâªÇµÇΩéûçè (0..1)
		float s = std::min( time / timeLimit , 1.0f );

		//	ïRÇÃí∑Ç≥Ç∆ë¨ìx
		double length = 1 - (10*pow(s,3) - 15*pow(s,4) + 6*pow(s,5));
		double deltaLength = -30*(pow(s,2) - 2*pow(s,3) + pow(s,4));
		if (length<0) {
			length = 0;
			deltaLength = 0;
		}

		Vec3d tlp = Vec3d(); if (endeffector) { tlp = endeffector->GetTargetLocalPosition(); }
		Vec3f dir = (solid->GetPose() * tlp)-finalPosAbs;
		if (dir.norm() != 0) { dir /= dir.norm(); }

		Vec3d currPos = finalPosAbs + dir*(finalPosAbs - initPosAbs).norm()*length;
		if (endeffector) { endeffector->SetTargetPosition(currPos); }
		if (soDebug)     { soDebug->SetFramePosition(initPosAbs);   }

		if (time > timeLimit) {
			bCtlPos = false;
		}
	}

	if (bEnable && time > timeLimit) {
		if (endeffector) { endeffector->SetTargetPosition(finalPosAbs); }
	}

	// épê®êßå‰
	if (bCtlOri && bEnable) {
		bCtlOri = false; // ñ¢é¿ëï
	}
	*/
}

void CRBone::Plan() {
	if (trajNodes.size() <= 1) { return; }

	/*
	std::cout << trajNodes.size()-1 << " segments." << std::endl;
	for (size_t segment=0; segment<trajNodes.size()-1; ++segment) {
		std::cout << segment << " : " << trajNodes[segment].pose.Pos() << " - " << trajNodes[segment+1].pose.Pos() << std::endl;
	}
	*/

	for (size_t segment=0; segment<trajNodes.size()-1; ++segment) {
		trajNodes[segment].viapose.Pos() = (trajNodes[segment].pose.Pos() + trajNodes[segment+1].pose.Pos()) * 0.5;
		trajNodes[segment].viatime       =  (trajNodes[segment+1].time - trajNodes[segment].time) * 0.5;
		PlanSegment(trajNodes[segment], trajNodes[segment+1]);

		// std::cout << "Before : " << trajNodes[segment].viatime << ", " << trajNodes[segment].length << std::endl;

		// <!!> ÇøÇÂÇ¡Ç∆NaiveÇ∑Ç¨ÇÈÇ©Ç‡ÅDóvâ¸ëP
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

		// std::cout << "After  : " << trajNodes[segment].viatime << ", " << trajNodes[segment].length << std::endl;
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

/// ----- ÇªÇÃÇ§ÇøObsoleteÇ…Ç∑ÇÈÇ©Ç‡ÅÉÇ±Ç±Ç©ÇÁÅÑ -----

void CRBone::SetTargetPos(Vec3d pos) {
	if (!bCtlPos) {
		Vec3d tlp = endeffector ? endeffector->GetTargetLocalPosition() : Vec3d();
		initPos  = solid->GetPose() * tlp;
		finalPos = pos;
		bCtlPos  = true;
		if (endeffector) {
			endeffector->Enable(true);
			endeffector->EnablePositionControl(true);
		}
	} else {
		finalPos = pos;
	}
	if (originSolid) {
		initPos  = originSolid->GetPose().Inv() * initPos;
		finalPos = originSolid->GetPose().Inv() * finalPos;
	}
}

Vec3d CRBone::GetTargetPos() {
	return finalPos;
}

void CRBone::SetTargetOri(Quaterniond ori) {
	finalOri = ori;
	bCtlOri  = true;
	if (endeffector) {
		endeffector->Enable(true);
		endeffector->EnableOrientationControl(true);
	}
}

Vec3d CRBone::GetTargetOri() {
	return finalOri;
}

void CRBone::SetTargetPose(Posed pose) {
	SetTargetPos(pose.Pos());
	SetTargetOri(pose.Ori());
}

void CRBone::SetTimeLimit(float timeLimit) {
	this->timeLimit = timeLimit;
}

void CRBone::Start() {
	if ((!bEnable && !bPause) || (time > timeLimit)) {
		time = 0.0f;
	}
	if (endeffector) { endeffector->Enable(true); }
	bEnable = true;
	bPause = false;
}

void CRBone::Pause() {
	bEnable = false;
	bPause  = true;
}

void CRBone::Stop() {
	bEnable = false;
	bPause  = false;
	if (endeffector) { endeffector->Enable(false); }
	time = 0.0f;
}

/// ----- ÇªÇÃÇ§ÇøObsoleteÇ…Ç∑ÇÈÇ©Ç‡ÅÉÇ±Ç±Ç‹Ç≈ÅÑ -----

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ï®ëÃíTçı
void CRBone::SetViewArea(Posed relativePose, double horizRange, double vertRange) {
	this->relativePose = relativePose;
	this->horizRange   = horizRange;
	this->vertRange    = vertRange;
}

void CRBone::StepSearchArea() {
	if (horizRange > 0 && vertRange > 0) {
		foundSolids.clear();
		PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
		for (int i=0; i<phScene->NSolids(); ++i) {
			PHSolidIf* so = phScene->GetSolids()[i];
			if (so->NShape() == 0) { continue; }

			Vec3d localPos = (solid->GetPose() * relativePose).Inv() * so->GetPose().Pos();
			if (localPos.norm() != 0) {
				localPos = localPos.unit();
			}

			Vec2d theta = Vec2d(atan2(localPos.X(), localPos.Z()), atan2(localPos.Y(), localPos.Z()));
			theta.Y() *= (horizRange / vertRange);
			if (theta.norm() < (horizRange / 2.0)) {
				foundSolids.push_back(so);
			}
		}
	}
}

int CRBone::NVisibleSolids() {
	return (int)(foundSolids.size());
}

PHSolidIf* CRBone::GetVisibleSolid(int i) {
	if (0 <= i && i < (int)foundSolids.size()) {
		return foundSolids[i];
	} else {
		return NULL;
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ê⁄êGàÍóó
int CRBone::NTouches() {
	return (int)(touchList.size());
}

Vec3f CRBone::GetContactForce(int i) {
	return touchList[i].force;
}

double CRBone::GetContactArea(int i) {
	return touchList[i].area;
}

Vec3f CRBone::GetContactPosition(int i) {
	return touchList[i].position;
}

PHSolidIf* CRBone::GetContactSolid(int i) {
	return touchList[i].solid;
}

void CRBone::StepListContact() {
	PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
	touchList.clear();

	int stepCount = phScene->GetCount();

	PHSolidPairForLCPIf* solidPair;
	PHShapePairForLCPIf* shapePair;

	// é©ï™ÇÃçÑëÃî‘çÜÇíTÇ∑
	int num_my_solid;
	for (int i=0; i<phScene->NSolids(); ++i) {
		if (phScene->GetSolids()[i] == solid) {
			num_my_solid = i;
		}
	}
	
	for (int i=0; i<phScene->NSolids(); ++i) {
		solidPair = NULL;
		shapePair = NULL;

		PHSolidIf* soTarget = phScene->GetSolids()[i];

		int i1, i2;
		PHSolidIf *s1, *s2;
		if (i < num_my_solid) {
			i1 = i;
			i2 = num_my_solid;
			s1 = soTarget;
			s2 = solid;
		} else if (num_my_solid < i) {
			i1 = num_my_solid;
			i2 = i;
			s1 = solid;
			s2 = soTarget;
		} else {
			continue;
		}

		solidPair = phScene->GetSolidPair(i1, i2);
		PHConstraintIf* constraint = phScene->GetConstraintEngine()->GetContactPoints()->FindBySolidPair(s1, s2);

		if (!solidPair || !constraint) { continue; }
		Vec3d force = phScene->GetConstraintEngine()->GetContactPoints()->GetTotalForce(s1, s2);

		for (int c1=0; c1<s1->NShape(); ++c1) {
			for (int c2=0; c2<s2->NShape(); ++c2) {
				if (shapePair = solidPair->GetShapePair(c1, c2)) {
					int       contactStat      = solidPair->GetContactState(c1, c2);
					unsigned  lastContactCount = solidPair->GetLastContactCount(c1, c2);
					
					if (contactStat==1 || (contactStat==2 && (lastContactCount==stepCount-1))) {
						// ê⁄êGñ êœÇãÅÇﬂÇÈ
						double area = 0;
						if (shapePair->NSectionVertexes() > 3) {
							Vec3d v0 = shapePair->GetSectionVertex(0);
							for (int v=2; v<shapePair->NSectionVertexes(); ++v) {
								Vec3d v1 = shapePair->GetSectionVertex(v-1);
								Vec3d v2 = shapePair->GetSectionVertex(v);

								double a=(v2-v0).norm(), b=(v1-v0).norm(), c=(v2-v1).norm();
								double s = (a+b+c)/2.0;
								area += sqrt(s*(s-a)*(s-b)*(s-c));
							}
						}

						// ê⁄êGèÓïÒÇÃí«â¡
						CRTouchInfo contact;
						contact.position = solidPair->GetCommonPoint(c1, c2);
						contact.solid    = soTarget;
						contact.force    = force;
						contact.area     = area;

						touchList.push_back(contact);
					}
				}
			}
		}
	}
}

}
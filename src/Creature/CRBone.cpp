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

void CRBone::StepTrajectory() {
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

		Vec3f dir = (solid->GetPose() * endeffector->GetTargetLocalPosition())-finalPosAbs;
		if (dir.norm() != 0) { dir /= dir.norm(); }

		Vec3d currPos = finalPosAbs + dir*(finalPosAbs - initPosAbs).norm()*length;
		endeffector->SetTargetPosition(currPos);
		if (soDebug) { soDebug->SetFramePosition(initPosAbs); }

		if (time > timeLimit) {
			bCtlPos = false;
		}
	}

	if (bEnable && time > timeLimit) {
		endeffector->SetTargetPosition(finalPosAbs);
	}

	// épê®êßå‰
	if (bCtlOri && bEnable) {
		bCtlOri = false; // ñ¢é¿ëï
	}
}

void CRBone::Start() {
	if ((!bEnable && !bPause) || (time > timeLimit)) {
		time = 0.0f;
	}
	endeffector->Enable(true);
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
	endeffector->Enable(false);
	time = 0.0f;
}

void CRBone::Plan() {
	// ñ¢é¿ëï
}

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
	return foundSolids.size();
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
int CRBone::NContacts() {
	return contactList.size();
}

Vec3f CRBone::GetContactForce(int i) {
	return contactList[i].force;
}

double CRBone::GetContactArea(int i) {
	return contactList[i].area;
}

Vec3f CRBone::GetContactPosition(int i) {
	return contactList[i].position;
}

PHSolidIf* CRBone::GetContactSolid(int i) {
	return contactList[i].solid;
}

void CRBone::StepListContact() {
	PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
	contactList.clear();

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
						ContactInfo contact;
						contact.position = solidPair->GetCommonPoint(c1, c2);
						contact.solid    = soTarget;
						contact.force    = force;
						contact.area     = area;

						contactList.push_back(contact);
					}
				}
			}
		}
	}
}

}
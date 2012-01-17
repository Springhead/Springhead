/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyPart.h>

#include <Physics/PHIKActuator.h>

#include <Creature/SprCRCreature.h>
#include <Creature/SprCRBody.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSdk.h>

namespace Spr{;
//-------------------------------------------------------------------------------------------------

size_t CRSolid::NChildObject() const {
	if (solid) { return 1; } else { return 0; }
}
ObjectIf* CRSolid::GetChildObject(size_t i) {
	if (i==0) { return solid; }
	else { return NULL; }
}
bool CRSolid::AddChildObject(ObjectIf* o) {
	solid = o->Cast();
	return(solid!=NULL);
}
bool CRSolid::DelChildObject(ObjectIf* o) {
	if (o==solid) {
		solid = NULL;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------

size_t CRIKSolid::NChildObject() const {
	return( (solid ? 1 : 0) + (ikEndEffector ? 1 : 0) );
}

ObjectIf* CRIKSolid::GetChildObject(size_t i) {
	if (solid) {
		if (i==0) {
			return solid;
		} else if (i==1) {
			return ikEndEffector;
		}
	} else {
		if (i==0) {
			return ikEndEffector;
		}
	}
	return NULL;
}

bool CRIKSolid::AddChildObject(ObjectIf* o) {
	PHSolidIf* so = o->Cast();
	if (so) { solid = so; return true; }

	PHIKEndEffectorIf* ikef = o->Cast();
	if (ikef) { ikEndEffector = ikef; return true; }
		
	return false;
}

bool CRIKSolid::DelChildObject(ObjectIf* o) {
	PHSolidIf* so = o->Cast();
	if (so && so==solid) { solid = NULL; return true; }

	PHIKEndEffectorIf* eef = o->Cast();
	if (eef && eef==ikEndEffector) { ikEndEffector = NULL; return true; }

	return false;
}

void CRIKSolid::Step() {
	StepTrajectory();
	StepSearchArea();
	StepListContact();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ãOìπâ^ìÆ
void CRIKSolid::SetOriginSolid(PHSolidIf* solid) {
	originSolid = solid;
}

void CRIKSolid::SetTargetPos(Vec3d pos) {
	if (!bCtlPos) {
		initPos  = solid->GetPose() * ikEndEffector->GetTargetLocalPosition();
		finalPos = pos;
		bCtlPos  = true;
		ikEndEffector->Enable(true);
		ikEndEffector->EnablePositionControl(true);
	} else {
		finalPos = pos;
	}
	if (originSolid) {
		initPos  = originSolid->GetPose().Inv() * initPos;
		finalPos = originSolid->GetPose().Inv() * finalPos;
	}
}

void CRIKSolid::SetTargetOri(Quaterniond ori) {
	finalOri = ori;
	bCtlOri  = true;
	ikEndEffector->Enable(true);
	ikEndEffector->EnableOrientationControl(true);
}

void CRIKSolid::SetTargetPose(Posed pose) {
	SetTargetPos(pose.Pos());
	SetTargetOri(pose.Ori());
}

void CRIKSolid::SetTimeLimit(float timeLimit) {
	this->timeLimit = timeLimit;
}

void CRIKSolid::StepTrajectory() {
	// --
	PHSolidIf* soDebug = DCAST(CRCreatureIf,GetScene())->GetPHScene()->FindObject("soDebug")->Cast();
	if (soDebug) {
		DCAST(CRCreatureIf,GetScene())->GetPHScene()->SetContactMode(soDebug, PHSceneDesc::MODE_NONE);
	}
	// --

	double dt = DCAST(CRCreatureIf,GetScene())->GetPHScene()->GetTimeStep();

	// à íuêßå‰
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

		Vec3f finalPosAbs=finalPos, initPosAbs=initPos;
		if (originSolid) {
			initPosAbs  = originSolid->GetPose() * initPosAbs;
			finalPosAbs = originSolid->GetPose() * finalPosAbs;
		}

		Vec3f dir = (solid->GetPose() * ikEndEffector->GetTargetLocalPosition())-finalPosAbs;
		if (dir.norm() != 0) { dir /= dir.norm(); }

		Vec3d currPos = finalPosAbs + dir*(finalPosAbs - initPosAbs).norm()*length;
		ikEndEffector->SetTargetPosition(currPos);
		if (soDebug) { soDebug->SetFramePosition(currPos); }
		// if (soDebug) { soDebug->SetFramePosition(finalPosAbs); }

		if (time > timeLimit) {
			bCtlPos = false;
			ikEndEffector->Enable(false);
		}
	}

	// épê®êßå‰
	if (bCtlOri && bEnable) {
		bCtlOri = false; // ñ¢é¿ëï
	}
}

void CRIKSolid::Start() {
	if (!bEnable && !bPause) {
		time = 0.0f;
	}
	ikEndEffector->Enable(true);
	bEnable = true;
	bPause = false;
}

void CRIKSolid::Pause() {
	bEnable = false;
	bPause  = true;
}

void CRIKSolid::Stop() {
	bEnable = false;
	bPause  = false;
	ikEndEffector->Enable(false);
	time = 0.0f;
}

void CRIKSolid::Plan() {
	// ñ¢é¿ëï
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ï®ëÃíTçı
void CRIKSolid::SetViewArea(Posed relativePose, double horizRange, double vertRange) {
	this->relativePose = relativePose;
	this->horizRange   = horizRange;
	this->vertRange    = vertRange;
}

void CRIKSolid::StepSearchArea() {
	if (horizRange > 0 && vertRange > 0) {
		foundSolids.clear();
		PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
		for (size_t i=0; i<phScene->NSolids(); ++i) {
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

int CRIKSolid::NVisibleSolids() {
	return foundSolids.size();
}

PHSolidIf* CRIKSolid::GetVisibleSolid(int i) {
	if (0 <= i && i < foundSolids.size()) {
		return foundSolids[i];
	} else {
		return NULL;
	}
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// ê⁄êGàÍóó
int CRIKSolid::NContacts() {
	return contactList.size();
}

Vec3f CRIKSolid::GetContactForce(int i) {
	return contactList[i].force;
}

double CRIKSolid::GetContactArea(int i) {
	return contactList[i].area;
}

Vec3f CRIKSolid::GetContactPosition(int i) {
	return contactList[i].position;
}

PHSolidIf* CRIKSolid::GetContactSolid(int i) {
	return contactList[i].solid;
}

void CRIKSolid::StepListContact() {
	PHSceneIf* phScene = DCAST(CRCreatureIf,GetScene())->GetPHScene();
	contactList.clear();

	int stepCount = phScene->GetCount();

	PHSolidPairForLCPIf* solidPair;
	PHShapePairForLCPIf* shapePair;

	// é©ï™ÇÃçÑëÃî‘çÜÇíTÇ∑
	int num_my_solid;
	for (size_t i=0; i<phScene->NSolids(); ++i) {
		if (phScene->GetSolids()[i] == solid) {
			num_my_solid = i;
		}
	}
	
	for (size_t i=0; i<phScene->NSolids(); ++i) {
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

		for (size_t c1=0; c1<s1->NShape(); ++c1) {
			for (size_t c2=0; c2<s2->NShape(); ++c2) {
				if (shapePair = solidPair->GetShapePair(c1, c2)) {
					int       contactStat      = solidPair->GetContactState(c1, c2);
					unsigned  lastContactCount = solidPair->GetLastContactCount(c1, c2);
					
					if (contactStat==1 || (contactStat==2 && (lastContactCount==stepCount-1))) {
						// ê⁄êGñ êœÇãÅÇﬂÇÈ
						double area = 0;
						if (shapePair->NSectionVertexes() > 3) {
							Vec3d v0 = shapePair->GetSectionVertex(0);
							for (size_t v=2; v<shapePair->NSectionVertexes(); ++v) {
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

//-------------------------------------------------------------------------------------------------

void CRJoint::SetSpringRatio(double springRatio, double damperRatio) {
	if (PHBallJointIf* bj = joint->Cast()) {
		if (spring < 0) {
			spring = bj->GetSpring();
		}
		if (damper < 0) {
			damper = bj->GetDamper();
		}
		bj->SetSpring(spring*springRatio); bj->SetDamper(damper*damperRatio);
		std::cout << "bj_ssr : " << spring * springRatio << std::endl;
	}
	if (PHHingeJointIf* hj = joint->Cast()) {
		if (spring < 0) {
			spring = hj->GetSpring();
		}
		if (damper < 0) {
			damper = hj->GetDamper();
		}
		hj->SetSpring(spring*springRatio); hj->SetDamper(damper*damperRatio);
		std::cout << "hj_ssr : " << spring * springRatio << std::endl;
	}
}

size_t CRJoint::NChildObject() const {
	if (joint) { return 1; } else { return 0; }
}
ObjectIf* CRJoint::GetChildObject(size_t i) {
	if (i==0) { return joint; } else { return NULL; }
}
bool CRJoint::AddChildObject(ObjectIf* o) {
	joint = o->Cast();
	return(joint!=NULL);
}
bool CRJoint::DelChildObject(ObjectIf* o) {
	if (o==joint) { joint = NULL; return true; }
	return false;
}

//-------------------------------------------------------------------------------------------------

void CRIKJoint::SetIKActuator(PHIKActuatorIf* ikAct) {
	ikActuator = ikAct;
	ikSpring = ikAct->GetSpring();
	ikDamper = ikAct->GetDamper();
}
void CRIKJoint::SetSpringRatio(double springRatio, double damperRatio) {
	if (PHBallJointIf* bj = joint->Cast()) {
		if (spring < 0) {
			spring = bj->GetSpring();
		}
		if (damper < 0) {
			damper = bj->GetDamper();
		}
		bj->SetSpring(spring*springRatio); bj->SetDamper(damper*damperRatio);
		DCAST(PHIKBallActuator,ikActuator)->jSpring = spring*springRatio;
		DCAST(PHIKBallActuator,ikActuator)->jDamper = damper*damperRatio;
		std::cout << "bj_ssr : " << spring * springRatio << std::endl;
	}
	if (PHHingeJointIf* hj = joint->Cast()) {
		if (spring < 0) {
			spring = hj->GetSpring();
		}
		if (damper < 0) {
			damper = hj->GetDamper();
		}
		hj->SetSpring(spring*springRatio); hj->SetDamper(damper*damperRatio);
		DCAST(PHIKHingeActuator,ikActuator)->jSpring = spring*springRatio;
		DCAST(PHIKHingeActuator,ikActuator)->jDamper = damper*damperRatio;
		std::cout << "hj_ssr : " << spring * springRatio << std::endl;
	}
}

void CRIKJoint::SetIKSpringRatio(double springRatio, double damperRatio) {
	if (ikSpring < 0) {
		ikSpring = ikActuator->GetSpring();
	}
	if (ikDamper < 0) {
		ikDamper = ikActuator->GetDamper();
	}
	ikActuator->SetSpring(ikSpring*springRatio); ikActuator->SetDamper(ikDamper*damperRatio);
}

size_t CRIKJoint::NChildObject() const {
	return( (joint ? 1 : 0) + (ikActuator ? 1 : 0) );
}

ObjectIf* CRIKJoint::GetChildObject(size_t i) {
	if (joint) {
		if (i==0) {
			return joint;
		} else if (i==1) {
			return ikActuator;
		}
	} else {
		if (i==0) {
			return ikActuator;
		}
	}
	return NULL;
}

bool CRIKJoint::AddChildObject(ObjectIf* o) {
	PHJointIf* jo = o->Cast();
	if (jo) { joint = jo; return true; }

	PHIKActuatorIf* ikact = o->Cast();
	if (ikact) { ikActuator = ikact; return true; }
		
	return false;
}

bool CRIKJoint::DelChildObject(ObjectIf* o) {
	PHJointIf* jo = o->Cast();
	if (jo && jo==joint) { joint = NULL; return true; }

	PHIKActuatorIf* act = o->Cast();
	if (act && act==ikActuator) { ikActuator = NULL; return true; }

	return false;
}

}
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBody.h>
#include <Physics/PHSolid.h>
#include <Physics/PHIKActuator.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

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

//-------------------------------------------------------------------------------------------------

Vec3d CRBody::GetCenterOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;
	/// 重心を求めるときに使うi番目までのブロックの中心座標
	Vec3d  centerPosOfBlocks = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0; i<NSolids(); i++){
		if(solids[i] && solids[i]->GetPHSolid()){
			centerPosOfBlocks = centerPosOfBlocks + solids[i]->GetPHSolid()->GetCenterPosition() * solids[i]->GetPHSolid()->GetMass();
			totalWeight = totalWeight + solids[i]->GetPHSolid()->GetMass();
		}
	}

	return centerPosOfBlocks / totalWeight;
}

double CRBody::GetSumOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;

	for(int i = 0; i<NSolids(); i++){
		if(solids[i])
			totalWeight = totalWeight + solids[i]->GetPHSolid()->GetMass(); 
	}

	return totalWeight;
}

Matrix3d CRBody::CalcBoxInertia(Vec3d boxsize, double mass){
	double i_xx = 1.0 / 12.0 * (boxsize[1] * boxsize[1] + boxsize[2] * boxsize[2]) * mass;
	double i_yy = 1.0 / 12.0 * (boxsize[2] * boxsize[2] + boxsize[0] * boxsize[0]) * mass;
	double i_zz = 1.0 / 12.0 * (boxsize[0] * boxsize[0] + boxsize[1] * boxsize[1]) * mass;

	return Matrix3d(i_xx, 0.0,  0.0, 
					0.0,  i_yy, 0.0, 
					0.0,  0.0,  i_zz);
}

ObjectIf* CRBody::GetChildObject(size_t i) {
	if (i < solids.size()) {
		return solids[i];
	} else if (i < solids.size() + joints.size()) {
		return joints[i - solids.size()];
	} else {
		return NULL;
	}
}

bool CRBody::AddChildObject(ObjectIf* o){
	CRSolidIf* s = DCAST(CRSolidIf, o);
	if (s){
		if (std::find(solids.begin(), solids.end(), s) == solids.end()){
			solids.push_back(s);
			DCAST(SceneObject, s)->SetScene(DCAST(CRBodyIf,this)->GetScene());
			return true;
		}
	}

	CRJointIf* j = DCAST(CRJointIf, o);
	if (j){
		if (std::find(joints.begin(), joints.end(), j) == joints.end()){
			joints.push_back(j);
			DCAST(SceneObject, j)->SetScene(DCAST(CRBodyIf,this)->GetScene());
			return true;
		}
	}

	return false;
}

bool CRBody::DelChildObject(ObjectIf* o){
	CRSolidIf* s = DCAST(CRSolidIf, o);
	if (s){
		CRSolids::iterator it = std::find(solids.begin(), solids.end(), s);
		if(it != solids.end()){
			solids.erase(it);
			return true;
		}
	}

	CRJointIf* j = DCAST(CRJointIf, o);
	if (j){
		CRJoints::iterator it = std::find(joints.begin(), joints.end(), j);
		if(it != joints.end()){
			joints.erase(it);
			return true;
		}
	}

	return false;
}

}
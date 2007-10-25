/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRGrabController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRGrabController, CRController);

void CRGrabController::Init(){
	CRController::Init();

	CRHingeHumanBodyIf* hiBody=NULL;
	for (int i=0; i<creature->NBodies(); ++i) {
		if (!hiBody) {
			hiBody = DCAST(CRHingeHumanBodyIf, creature->GetBody(i));
		}
	}

	soWaist = hiBody->GetSolid(CRHingeHumanBodyDesc::SO_WAIST);

	CRHingeHumanBodyDesc descHIBody;
	hiBody->GetDesc(&descHIBody);
	reachableDistance = (descHIBody.upperArmLength + descHIBody.lowerArmLength + (descHIBody.handLength / 2.0f) + descHIBody.chestHeight + descHIBody.abdomenHeight);
	shoulderHeightFromWaist = descHIBody.waistHeight/2.0 + descHIBody.abdomenHeight + descHIBody.chestHeight;

	reachLeft  = NULL;
	reachRight = NULL;
	reachChest = NULL;
	for (int i=0; i<creature->NControllers(); i++) {
		if (!reachLeft) {
			CRReachingControllerIf* r = DCAST(CRReachingControllerIf, creature->GetController(i));
			if (r && r->GetSolid()==hiBody->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_HAND)) {
				reachLeft = r;
			}
		}
		if (!reachRight) {
			CRReachingControllerIf* r = DCAST(CRReachingControllerIf, creature->GetController(i));
			if (r && r->GetSolid()==hiBody->GetSolid(CRHingeHumanBodyDesc::SO_RIGHT_HAND)) {
				reachRight = r;
			}
		}
		if (!reachChest) {
			CRReachingControllerIf* r = DCAST(CRReachingControllerIf, creature->GetController(i));
			if (r && r->GetSolid()==hiBody->GetSolid(CRHingeHumanBodyDesc::SO_CHEST)) {
				reachChest = r;
			}
		}
	}

	controlState = CRGrabControllerIf::CRGC_STANDBY;
}

void CRGrabController::Step(){
	if (controlState==CRGrabControllerIf::CRGC_REACH) {
		if (reachLeft->IsReached() && reachRight->IsReached()) {
			grabSpring.first->Enable(true);
			grabSpring.second->Enable(true);
			controlState = CRGrabControllerIf::CRGC_REACH_COMPLETE;
		}
	} else if (controlState==CRGrabControllerIf::CRGC_UPHOLD) {
		/*
		++waittime;
		if (waittime > 100) {
			reachChest->Reset();
			controlState = CRGrabControllerIf::CRGC_UPHOLD_COMPLETE;
		}
		*/
		/*
		if (reachChest->IsReached()) {
			controlState = CRGrabControllerIf::CRGC_UPHOLD_COMPLETE;
		}
		*/
		if (reachLeft->IsReached() && reachRight->IsReached()) {
			controlState = CRGrabControllerIf::CRGC_UPHOLD_COMPLETE;
		}
	} else if (controlState==CRGrabControllerIf::CRGC_UPHOLD_COMPLETE) {
		reachLeft->SetTargetPos(soWaist->GetPose()*Vec3f(-targetRadius, targetRadius*2.0, -targetRadius*1.2));
		reachRight->SetTargetPos(soWaist->GetPose()*Vec3f( targetRadius, targetRadius*2.0, -targetRadius*1.2));
	} else if (controlState==CRGrabControllerIf::CRGC_PLACE) {
		if (reachLeft->IsReached() && reachRight->IsReached()) {
			AbortAll();
			controlState = CRGrabControllerIf::CRGC_PLACE_COMPLETE;
		}
	}
}

bool CRGrabController::Reach(PHSolidIf* solid, float radius){
	if (!IsReachable(solid)) {
		return false;
	}
	
	targetSolid  = solid;
	targetRadius = radius;

	Vec3f reachPointDirL;
	Vec3f w2t = (targetSolid->GetPose().Pos() - soWaist->GetPose().Pos());
	reachPointDirL = Vec3f(w2t.Z(), 0, -w2t.X()).unit();
	if (PTM::cross(w2t, reachPointDirL).Y() < 0) {
		reachPointDirL = -reachPointDirL;
	}

	GrabSpringMap::iterator it = grabSpringMap.find(targetSolid);
	if (it==grabSpringMap.end()) {
		PHSpringDesc descSpring;
		descSpring.bEnabled = false;
		descSpring.spring   = Vec3d(1,1,1) * 500;
		descSpring.damper   = Vec3d(1,1,1) *  10;
		descSpring.poseSocket.Pos() = Vec3f(0,0,0);

		descSpring.posePlug.Pos()   = targetSolid->GetPose().Ori().Inv() *  reachPointDirL*targetRadius*0.95 - Vec3f(0,0.25,0);
		PHJointIf* sprL = phScene->CreateJoint(reachLeft->GetSolid(), targetSolid, descSpring);

		descSpring.posePlug.Pos()   = targetSolid->GetPose().Ori().Inv() * -reachPointDirL*targetRadius*0.95 - Vec3f(0,0.25,0);
		PHJointIf* sprR = phScene->CreateJoint(reachRight->GetSolid(), targetSolid, descSpring);

		SpringPair sp;
		sp.first  = DCAST(PHSpringIf,sprL);
		sp.second = DCAST(PHSpringIf,sprR);
		grabSpringMap[targetSolid] = sp;

		grabSpring = sp;
	} else {
		grabSpring = it->second;
	}

	// –{“–‚Í‚±‚±‚Å‘Ì‚ð‚â‚í‚ç‚©‚­‚·‚é

	reachLeft->SetTarget(targetSolid->GetPose().Pos() + reachPointDirL*targetRadius*1.0, solid->GetVelocity(), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(+90),'y')*soWaist->GetOrientation(), Vec3d(0,0,0), 1.5, -1);
	reachRight->SetTarget(targetSolid->GetPose().Pos() - reachPointDirL*targetRadius*1.0, solid->GetVelocity(), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(-90),'y')*soWaist->GetOrientation(), Vec3d(0,0,0), 1.5, -1);
	//reachLeft->SetTarget(targetSolid->GetPose().Pos() + reachPointDirL*targetRadius*1.2, solid->GetVelocity(), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(-90),'y'), Vec3d(0,0,0), 1.5, -1);
	//reachRight->SetTarget(targetSolid->GetPose().Pos() - reachPointDirL*targetRadius*1.2, solid->GetVelocity(), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(+90),'y'), Vec3d(0,0,0), 1.5, -1);
	//reachLeft->SetTarget(targetSolid->GetPose().Pos() + reachPointDirL*targetRadius*1.05  + Vec3f(0,0.25,0), solid->GetVelocity(), 1.0, -1);
	//reachRight->SetTarget(targetSolid->GetPose().Pos() - reachPointDirL*targetRadius*1.05 + Vec3f(0,0.25,0), solid->GetVelocity(), 1.0, -1);
	reachChest->SetTarget(targetSolid->GetPose().Pos(), Vec3f(0,0,0), 1.5, -1);
	
	controlState = CRGrabControllerIf::CRGC_REACH;
	return true;
}

bool CRGrabController::IsReachable(PHSolidIf* solid){
	return IsReachable(solid, 1.0f);
}

bool CRGrabController::IsReachable(PHSolidIf* solid, float safety){
	float distance = (solid->GetPose().Pos() - soWaist->GetPose().Pos()).norm();
	return(distance < (reachableDistance*safety));
}

bool CRGrabController::IsReachComplete(){
	return(controlState==CRGrabControllerIf::CRGC_REACH_COMPLETE);
}

bool CRGrabController::Uphold(){
	if (!IsUpholdable()) {
		return false;
	}

	/*
	reachLeft->Reset();
	reachRight->Reset();
	*/
	reachLeft->SetTarget(soWaist->GetPose()*Vec3f(-targetRadius, targetRadius*2.0, -targetRadius*1.2), Vec3f(0,0,0), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(-90),'y'), Vec3d(0,0,0), 2.0, -1);
	reachRight->SetTarget(soWaist->GetPose()*Vec3f( targetRadius, targetRadius*2.0, -targetRadius*1.2), Vec3f(0,0,0), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(+90),'y'), Vec3d(0,0,0), 2.0, -1);
	reachChest->SetTarget(soWaist->GetPose()*Vec3f(0,shoulderHeightFromWaist,0), Vec3f(0,0,0), 2.0, -1);

	waittime = 0;

	/*
	reachChest->Reset();
	reachLeft->SetTarget(soWaist->GetPose()*Vec3f(-targetRadius, targetRadius*2.0, -targetRadius*1.2), Vec3f(0,0,0), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(-90),'y'), Vec3d(0,0,0), 2.0, -1);
	reachRight->SetTarget(soWaist->GetPose()*Vec3f( targetRadius, targetRadius*2.0, -targetRadius*1.2), Vec3f(0,0,0), Quaterniond::Rot(Rad(+90),'x')*Quaterniond::Rot(Rad(+90),'y'), Vec3d(0,0,0), 2.0, -1);
	*/
	
	controlState = CRGrabControllerIf::CRGC_UPHOLD;
	return true;
}

bool CRGrabController::IsUpholdable(){
	return(controlState==CRGrabControllerIf::CRGC_REACH_COMPLETE);
}

bool CRGrabController::IsUpholdComplete(){
	return(controlState==CRGrabControllerIf::CRGC_UPHOLD_COMPLETE);
}

bool CRGrabController::Place(Vec3d pos){
	if (!IsPlaceable(pos)) {
		return false;
	}

	Vec3f reachPointDirL;
	Vec3f w2t = (pos - soWaist->GetPose().Pos());
	reachPointDirL = Vec3f(w2t.Z(), 0, -w2t.X()).unit();
	if (PTM::cross(w2t, reachPointDirL).Y() < 0) {
		reachPointDirL = -reachPointDirL;
	}

	reachLeft->SetTarget(pos + reachPointDirL*targetRadius*1.2, Vec3f(0,0,0), 1.5, -1);
	reachRight->SetTarget(pos - reachPointDirL*targetRadius*1.2, Vec3f(0,0,0), 1.5, -1);
	reachChest->SetTarget(pos, Vec3f(0,0,0), 1.5, -1);

	controlState = CRGrabControllerIf::CRGC_PLACE;

	return true;
}

bool CRGrabController::IsPlaceable(Vec3d pos){
	return IsPlaceable(pos, 1.0f);
}

bool CRGrabController::IsPlaceable(Vec3d pos, float safety){
	if (controlState!=CRGrabControllerIf::CRGC_UPHOLD_COMPLETE && controlState!=CRGrabControllerIf::CRGC_PLACE) {
		return false;
	}
	float distance = (pos - soWaist->GetPose().Pos()).norm();
	return(distance < (reachableDistance*safety));
}

bool CRGrabController::IsPlaceComplete(){
	return(controlState==CRGrabControllerIf::CRGC_PLACE_COMPLETE
		|| controlState==CRGrabControllerIf::CRGC_STANDBY);
}

void CRGrabController::Abort(){
	if (controlState==CRGrabControllerIf::CRGC_REACH) {
		AbortAll();
	} else if (controlState==CRGrabControllerIf::CRGC_UPHOLD) {
		reachChest->Reset();
		controlState = CRGrabControllerIf::CRGC_REACH_COMPLETE;
	} else if (controlState==CRGrabControllerIf::CRGC_PLACE) {
		reachLeft->Reset();
		reachRight->Reset();
		reachChest->Reset();
		if (reachLeft->IsReached() && reachRight->IsReached()) {
			targetSolid = NULL;
			targetRadius = 0.0f;
			placePos = Vec3f(0,0,0);
			grabSpring.first = NULL;
			grabSpring.second = NULL;
			controlState = CRGrabControllerIf::CRGC_PLACE_COMPLETE;
		}
	}
}

void CRGrabController::AbortAll(){
	if (grabSpring.first && grabSpring.second) {
		grabSpring.first->Enable(false);
		grabSpring.second->Enable(false);
	}
	reachLeft->Reset();
	reachRight->Reset();
	reachChest->Reset();
	targetSolid = NULL;
	targetRadius = 0.0f;
	placePos = Vec3f(0,0,0);
	grabSpring.first = NULL;
	grabSpring.second = NULL;
	controlState = CRGrabControllerIf::CRGC_STANDBY;
}

CRGrabControllerIf::CRGCControlState CRGrabController::GetControlState(){
	return controlState;
}
}

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

	CRReachingControllerIf *reach1=NULL, *reach2=NULL;
	for (int i=0; i<creature->NControllers(); i++) {
		if (!reach1) {
			reach1 = DCAST(CRReachingControllerIf, creature->GetController(i));
		} else if (!reach2) {
			reach2 = DCAST(CRReachingControllerIf, creature->GetController(i));
		}
	}

	if (reach1 && reach2) {
		if (reach1->GetSolid() == creature->GetBody()->GetSolid(CRHingeHumanBodyDesc::SO_LEFT_HAND)) {
			reachLeft = reach1;  reachRight = reach2;
		} else {
			reachLeft = reach2;  reachRight = reach1;
		}
	}
}

void CRGrabController::Step(){
	if (grabState == CRGrabControllerIf::GS_STANDBY) {
	} else if (grabState == CRGrabControllerIf::GS_GRAB_START) {
		if (reachLeft->IsReached() && reachRight->IsReached()) {
			grabState = CRGrabControllerIf::GS_GRAB;
		}
	} else if (grabState == CRGrabControllerIf::GS_GRAB) {
		if (bMoveto) {
			grabState = CRGrabControllerIf::GS_MOVE;
			reachLeft->SetTarget(movetoPos+Vec3d(-radius*0.9f,-radius*0.3f,0), Vec3f(0,0,0), Quaterniond::Rot(Rad(+100),'z')*Quaterniond::Rot(Rad(90),'x'), Vec3d(0,0,0), 0.5, -1);
			reachRight->SetTarget(movetoPos+Vec3d(+radius*0.9f,-radius*0.3f,0), Vec3f(0,0,0), Quaterniond::Rot(Rad(-100),'z')*Quaterniond::Rot(Rad(90),'x'), Vec3d(0,0,0), 0.5, -1);
		}
	} else if (grabState == CRGrabControllerIf::GS_MOVE) {
		if (reachLeft->IsReached() && reachRight->IsReached()) {
			Ungrab();
		}
	}
}

void CRGrabController::Grab(PHSolidIf* solid, float radius){
	this->grabSolid = solid;
	this->radius    = radius;
	reachLeft->SetTarget(grabSolid->GetPose()*Vec3d(-radius*0.9f,-radius*0.3f,0), solid->GetVelocity(), Quaterniond::Rot(Rad(+100),'z')*Quaterniond::Rot(Rad(90),'x'), Vec3d(0,0,0), 0.5, -1);
	reachRight->SetTarget(grabSolid->GetPose()*Vec3d(+radius*0.9f,-radius*0.3f,0), solid->GetVelocity(), Quaterniond::Rot(Rad(-100),'z')*Quaterniond::Rot(Rad(90),'x'), Vec3d(0,0,0), 0.5, -1);
	grabState = CRGrabControllerIf::GS_GRAB_START;
}

void CRGrabController::Ungrab(){
	reachLeft->Reset();
	reachRight->Reset();
	bMoveto = false;
	grabState = CRGrabControllerIf::GS_STANDBY;
}

void CRGrabController::MoveTo(Vec3f pos){
	movetoPos = pos;
	bMoveto = true;
}

CRGrabControllerIf::GrabState CRGrabController::GetGrabState(){
	return grabState;
}
}

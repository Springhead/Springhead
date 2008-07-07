/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRFourLegsTinyAnimalBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRFourLegsTinyAnimalBodyDesc::CRFourLegsTinyAnimalBodyDesc(){
		
	jointOrder = SOCKET_PARENT;

	bodyHeight    = 0.2298;
	bodyBreadth   = 0.3067;
	bodyThickness = 0.2307;

	frontLegsBreadth    = 1.4020 - 1.2253 + 1.2253 - 1.0142;
	frontLegsHeight   = 0.2887;
	frontLegsThickness = 0.2118;

	rearLegsBreadth       = 0.2544;
	rearLegsHeight      = 0.0994;
	rearLegsThickness    = 0.0619;

	springFront   = 100.0;  damperFront   =  50.0;
	springRear    = 100.0;  damperRear    =  50.0;

	// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
	rangeFrontSwing   = Vec2d(Rad(-60.0) , Rad(60.0));
	rangeFrontTwist	  = Vec2d(Rad(-60.0) , Rad(60.0));
	rangeRearSwing    = Vec2d(Rad(-60.0) , Rad(60.0));
	rangeRearTwist	  = Vec2d(Rad(-60.0) , Rad(60.0));
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsTinyAnimalBody


// --- --- ---
void CRFourLegsTinyAnimalBody::Init(){
	CRBody::Init();
	CreateBody();
}

// --- --- ---
void CRFourLegsTinyAnimalBody::CreateBody(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass     = 5.0;
	solids[SO_BODY]   = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(bodyBreadth, bodyHeight, bodyThickness);
	solids[SO_BODY]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_BODY]->SetFramePosition(Vec3f(0,0,0));

	solids[SO_BODY]->SetDynamical(false);
}

void CRFourLegsTinyAnimalBody::CreateFrontLegs(LREnum lr){
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = 1.5;
	if(lr == RIGHTPART){
		solids[SO_RIGHT_FRONT_LEG] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_RIGHT_FRONT_LEG]->AddShape(phSdk->CreateShape(boxDesc));
	}
	else{
		solids[SO_LEFT_FRONT_LEG] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_LEFT_FRONT_LEG]->AddShape(phSdk->CreateShape(boxDesc));
	}
	// 体幹と前脚間の関節
	{
		ballDesc.poseSocket.Pos() = Vec3d(lr * bodyBreadth / 2.0, bodyHeight / 2.0, 0);
		ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'x');
		ballDesc.posePlug.Pos() = Vec3d(0, -frontLegsHeight / 2.0, 0);
		ballDesc.posePlug.Ori() = Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring = springFront;
		ballDesc.damper = damperFront;
	}
	if(lr == RIGHTPART){
		joints[JO_BODY_RIGHT_FRONT_LEG] = CreateJoint(solids[SO_BODY], solids[SO_RIGHT_FRONT_LEG], ballDesc);
		phScene->SetContactMode(solids[SO_BODY], solids[SO_RIGHT_FRONT_LEG], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_FRONT_LEG] =  CreateJoint(solids[SO_BODY], solids[SO_LEFT_FRONT_LEG], ballDesc);
		phScene->SetContactMode(solids[SO_BODY], solids[SO_LEFT_FRONT_LEG], PHSceneDesc::MODE_NONE);
	}
}

void CRFourLegsTinyAnimalBody::CreateRearLegs(LREnum lr){
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = 1.5;
	if(lr == RIGHTPART){
		solids[SO_RIGHT_REAR_LEG] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_RIGHT_REAR_LEG]->AddShape(phSdk->CreateShape(boxDesc));
	}
	else{
		solids[SO_LEFT_REAR_LEG] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_LEFT_REAR_LEG]->AddShape(phSdk->CreateShape(boxDesc));
	}
	// 体幹と前脚間の関節
	{
		ballDesc.poseSocket.Pos() = Vec3d(lr * bodyBreadth / 2.0, -bodyHeight / 2.0, 0);
		ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'x');
		ballDesc.posePlug.Pos()	  = Vec3d(0, -rearLegsHeight / 2.0, 0);
		ballDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring = springFront;
		ballDesc.damper = damperFront;
	}
	if(lr == RIGHTPART){
		joints[JO_BODY_RIGHT_REAR_LEG] = CreateJoint(solids[SO_BODY], solids[SO_RIGHT_REAR_LEG], ballDesc);
		phScene->SetContactMode(solids[SO_BODY], solids[SO_RIGHT_REAR_LEG], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_REAR_LEG] =  CreateJoint(solids[SO_BODY], solids[SO_LEFT_REAR_LEG], ballDesc);
		phScene->SetContactMode(solids[SO_BODY], solids[SO_LEFT_REAR_LEG], PHSceneDesc::MODE_NONE);
	}
}

// --- --- ---
void CRFourLegsTinyAnimalBody::InitLegs(){
	CreateFrontLegs(LEFTPART);
	CreateFrontLegs(RIGHTPART);
	CreateRearLegs(LEFTPART);
	CreateRearLegs(RIGHTPART);
}

// --- --- ---
void CRFourLegsTinyAnimalBody::InitContact(){
	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	for (unsigned int i=0; i<solids.size(); ++i) {
		for (unsigned int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i], solids[j], PHSceneDesc::MODE_NONE);
			}
		}
	}

	// 自分以外にすでにBodyが居ればそのBodyに属する剛体とのContactも切る
	for (int i=0; i<creature->NBodies(); ++i) {
		CRBodyIf* body = creature->GetBody(i);
		if (DCAST(CRFourLegsTinyAnimalBodyIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}
}

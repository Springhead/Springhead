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

	bodyBreadth   = 2.5;
	bodyHeight    = 3.8;
	bodyThickness = 0.6;

	frontLegsBreadth    = 0.9;
	frontLegsHeight		= 1.2;
	frontLegsThickness	= 0.9;

	rearLegsBreadth     = 0.9;
	rearLegsHeight      = 1.2;
	rearLegsThickness   = 0.9;

	springFront   = 1.0;  damperFront   =  5.0;
	springRear    = 1.0;  damperRear    =  5.0;

	// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
	rangeFrontSwing   = Vec2d(Rad(-60.0) , Rad(60.0));
	rangeFrontTwist	  = Vec2d(Rad(-60.0) , Rad(60.0));
	rangeRearSwing    = Vec2d(Rad(-60.0) , Rad(60.0));
	rangeRearTwist	  = Vec2d(Rad(-60.0) , Rad(60.0));

	noLegs = false;
	noHead = false;
	onlyOneLeg = false;
	hingeDebug = false;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsTinyAnimalBody


// --- --- ---
void CRFourLegsTinyAnimalBody::Init(){
	CRBody::Init();
}

// --- --- ---
void CRFourLegsTinyAnimalBody::InitBody(){
	CreateBody();
}

void CRFourLegsTinyAnimalBody::CreateBody(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass		= 5.0;
	solids[SO_BODY]		= phScene->CreateSolid(solidDesc);
	boxDesc.boxsize		= Vec3f(bodyBreadth, bodyHeight, bodyThickness);
	solids[SO_BODY]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_BODY]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_BODY]->SetOrientation(Quaterniond::Rot(Rad(-90), 'x'));
	solids[SO_BODY]->SetName("soWaist");
	solids[SO_BODY]->SetDynamical(false);
}
void CRFourLegsTinyAnimalBody::InitFrontLeg0(LREnum lr){	
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = 1.5;
	if(lr == RIGHTPART){
		solids[SO_RIGHT_FRONT_LEG_0] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_RIGHT_FRONT_LEG_0]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_RIGHT_FRONT_LEG_0]->SetName("soRightFrontLeg0");
	}
	else{
		solids[SO_LEFT_FRONT_LEG_0] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_LEFT_FRONT_LEG_0]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_LEFT_FRONT_LEG_0]->SetName("soLeftFrontLeg0");
	}
	// 体幹と前脚間の関節
	{
		ballDesc.poseSocket.Pos()	= Vec3d(lr * bodyBreadth / 2.0, bodyHeight / 2.0, 0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, -frontLegsHeight / 2.0, 0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.goal				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springFront;
		ballDesc.damper				= damperFront;
	}
	if(lr == RIGHTPART){
		joints[JO_BODY_RIGHT_FRONT_LEG_0] = CreateJoint(solids[SO_BODY], solids[SO_RIGHT_FRONT_LEG_0], ballDesc);
		joints[JO_BODY_RIGHT_FRONT_LEG_0]->SetName("joRightShoulder");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_RIGHT_FRONT_LEG_0], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_FRONT_LEG_0] =  CreateJoint(solids[SO_BODY], solids[SO_LEFT_FRONT_LEG_0], ballDesc);
		joints[JO_BODY_LEFT_FRONT_LEG_0]->SetName("joLeftShoulder");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_LEFT_FRONT_LEG_0], PHSceneDesc::MODE_NONE);
	}
}
void CRFourLegsTinyAnimalBody::InitFrontLeg1(LREnum lr){
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = 1.5;
	if(lr == RIGHTPART){
		solids[SO_RIGHT_FRONT_LEG_1] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_RIGHT_FRONT_LEG_1]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_RIGHT_FRONT_LEG_1]->SetName("soRightFrontLeg1");
	}
	else{
		solids[SO_LEFT_FRONT_LEG_1] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_LEFT_FRONT_LEG_1]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_LEFT_FRONT_LEG_1]->SetName("soLeftFrontLeg1");
	}
	// 体幹と前脚間の関節
	{
		ballDesc.poseSocket.Pos()	= Vec3d(0, frontLegsHeight/2.0, 0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, -frontLegsHeight / 2.0, 0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.goal				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springFront;
		ballDesc.damper				= damperFront;
	}
	if(lr == RIGHTPART){
		joints[JO_BODY_RIGHT_FRONT_LEG_1] = CreateJoint(solids[SO_RIGHT_FRONT_LEG_0], solids[SO_RIGHT_FRONT_LEG_1], ballDesc);
		joints[JO_BODY_RIGHT_FRONT_LEG_1]->SetName("joRightElbow");
		phScene->SetContactMode(solids[SO_RIGHT_FRONT_LEG_0], solids[SO_RIGHT_FRONT_LEG_1], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_FRONT_LEG_1] =  CreateJoint(solids[SO_LEFT_FRONT_LEG_0], solids[SO_LEFT_FRONT_LEG_1], ballDesc);
		joints[JO_BODY_LEFT_FRONT_LEG_1]->SetName("joLeftElbow");
		phScene->SetContactMode(solids[SO_LEFT_FRONT_LEG_0], solids[SO_LEFT_FRONT_LEG_1], PHSceneDesc::MODE_NONE);
	}

}
void CRFourLegsTinyAnimalBody::InitRearLeg0(LREnum lr){
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = 1.5;
	if(lr == RIGHTPART){
		solids[SO_RIGHT_REAR_LEG_0] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_RIGHT_REAR_LEG_0]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_RIGHT_REAR_LEG_0]->SetName("soRightRearLeg0");
	}
	else{
		solids[SO_LEFT_REAR_LEG_0] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_LEFT_REAR_LEG_0]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_LEFT_REAR_LEG_0]->SetName("soLeftRearLeg0");
	}
	// 体幹と後脚間の関節
	{
		ballDesc.poseSocket.Pos()	= Vec3d(lr * bodyBreadth / 2.0, -bodyHeight / 2.0, 0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, -rearLegsHeight / 2.0, 0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.goal				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springFront;
		ballDesc.damper				= damperFront;
	}
	if(lr == RIGHTPART){
		joints[JO_BODY_RIGHT_REAR_LEG_0] = CreateJoint(solids[SO_BODY], solids[SO_RIGHT_REAR_LEG_0], ballDesc);
		joints[JO_BODY_RIGHT_REAR_LEG_0]->SetName("joRightHip");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_RIGHT_REAR_LEG_0], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_REAR_LEG_0] =  CreateJoint(solids[SO_BODY], solids[SO_LEFT_REAR_LEG_0], ballDesc);
		joints[JO_BODY_LEFT_REAR_LEG_0]->SetName("joLeftHip");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_LEFT_REAR_LEG_0], PHSceneDesc::MODE_NONE);
	}
}
void CRFourLegsTinyAnimalBody::InitRearLeg1(LREnum lr){
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = 1.5;
	if(lr == RIGHTPART){
		solids[SO_RIGHT_REAR_LEG_1] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_RIGHT_REAR_LEG_1]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_RIGHT_REAR_LEG_1]->SetName("soRightRearLeg1");
	}
	else{
		solids[SO_LEFT_REAR_LEG_1] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_LEFT_REAR_LEG_1]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_LEFT_REAR_LEG_1]->SetName("soLeftRearLeg1");
	}
	// 体幹と後脚間の関節
	{
		ballDesc.poseSocket.Pos()	= Vec3d(0, rearLegsHeight / 2.0, 0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, -rearLegsHeight / 2.0, 0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.goal				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springFront;
		ballDesc.damper				= damperFront;
	}
	if(lr == RIGHTPART){
		joints[JO_BODY_RIGHT_REAR_LEG_1] = CreateJoint(solids[SO_RIGHT_REAR_LEG_0], solids[SO_RIGHT_REAR_LEG_1], ballDesc);
		joints[JO_BODY_RIGHT_REAR_LEG_1]->SetName("joRightRearElbow");
		phScene->SetContactMode(solids[SO_RIGHT_REAR_LEG_0], solids[SO_RIGHT_REAR_LEG_1], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_REAR_LEG_1] =  CreateJoint(solids[SO_LEFT_REAR_LEG_0], solids[SO_LEFT_REAR_LEG_1], ballDesc);
		joints[JO_BODY_LEFT_REAR_LEG_1]->SetName("joLeftRearElbow");
		phScene->SetContactMode(solids[SO_LEFT_REAR_LEG_0], solids[SO_LEFT_REAR_LEG_1], PHSceneDesc::MODE_NONE);
	}
}

void CRFourLegsTinyAnimalBody::CreateFrontLegs(LREnum lr){
	InitFrontLeg0(lr);
	InitFrontLeg1(lr);
}

void CRFourLegsTinyAnimalBody::CreateRearLegs(LREnum lr){
	InitRearLeg0(lr);
	InitRearLeg1(lr);
}

// --- --- ---
void CRFourLegsTinyAnimalBody::InitLegs(){
	if(! noLegs){
		CreateFrontLegs(RIGHTPART);
		if(! onlyOneLeg){
			CreateFrontLegs(LEFTPART);
			CreateRearLegs(RIGHTPART);
			CreateRearLegs(LEFTPART);
		}
	}
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

void CRFourLegsTinyAnimalBody::InitControlMode(PHJointDesc::PHControlMode m){
	for(int i = 0; i < joints.size(); i++){
		if(joints[i]){joints[i]->SetMode(m);}
	}
}
}

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

	bodyBreadth   = 0.5;
	bodyHeight    = 0.8;
	bodyThickness = 0.6;

	frontLegsBreadth    = 0.1;
	frontLegsHeight		= 1.0;
	frontLegsThickness	= 0.1;

	rearLegsBreadth     = 0.1;
	rearLegsHeight      = 1.0;
	rearLegsThickness   = 0.1;

	springFront   = 10.0;  damperFront   =  5.0;
	springRear    = 10.0;  damperRear    =  5.0;

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
	if(! noHead) CreateHead();
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

void CRFourLegsTinyAnimalBody::CreateHead(){
	PHSolidDesc		solidDesc;
	
	// Solid
	solidDesc.mass				= 0.1;
	solids[SO_HEAD]				= phScene->CreateSolid(solidDesc);
	solids[SO_HEAD]->SetName("soHead");

	if(hingeDebug){
		PHHingeJointDesc hingeDesc;
		CDBoxDesc boxDesc;
		Vec3d size = Vec3d(0.5, 1.5, 0.7);
		boxDesc.boxsize = size;
		solids[SO_HEAD]->AddShape(phSdk->CreateShape(boxDesc));
		hingeDesc.poseSocket.Pos()	= Vec3f(0.0, bodyHeight/2, 0.0);
		hingeDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(90), 'y');
		hingeDesc.posePlug.Pos()	= Vec3f(0.0, -size.y / 2, 0.0);
		hingeDesc.posePlug.Ori()	= Quaterniond::Rot(Rad(90), 'y');
		hingeDesc.spring				= 100;
		hingeDesc.damper				= 50;
		joints[JO_BODY_HEAD] = CreateJoint(solids[SO_BODY], solids[SO_HEAD], hingeDesc);
	}
	else{
		CDSphereDesc	sphereDesc;
		PHBallJointDesc	ballDesc;
		sphereDesc.radius			= 0.3;
		solids[SO_HEAD]->AddShape(phSdk->CreateShape(sphereDesc));
		ballDesc.poseSocket.Pos()	= Vec3f(0.0, bodyHeight / 2.0, 0.0);
		ballDesc.spring				= 100;
		ballDesc.damper				= 50;
		joints[JO_BODY_HEAD] = CreateJoint(solids[SO_BODY], solids[SO_HEAD], ballDesc);
	}
	joints[JO_BODY_HEAD]->SetName("joNeck");
	phScene->SetContactMode(solids[SO_BODY], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
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
		solids[SO_RIGHT_FRONT_LEG]->SetName("soRightFrontLeg");
	}
	else{
		solids[SO_LEFT_FRONT_LEG] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(frontLegsBreadth, frontLegsHeight, frontLegsThickness);
		solids[SO_LEFT_FRONT_LEG]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_LEFT_FRONT_LEG]->SetName("soLeftFrontLeg");
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
		joints[JO_BODY_RIGHT_FRONT_LEG] = CreateJoint(solids[SO_BODY], solids[SO_RIGHT_FRONT_LEG], ballDesc);
		joints[JO_BODY_RIGHT_FRONT_LEG]->SetName("joRightShoulder");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_RIGHT_FRONT_LEG], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_FRONT_LEG] =  CreateJoint(solids[SO_BODY], solids[SO_LEFT_FRONT_LEG], ballDesc);
		joints[JO_BODY_LEFT_FRONT_LEG]->SetName("joLeftShoulder");
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
		solids[SO_RIGHT_REAR_LEG]->SetName("soRightRearLeg");
	}
	else{
		solids[SO_LEFT_REAR_LEG] = phScene->CreateSolid(solidDesc);
		boxDesc.boxsize  = Vec3f(rearLegsBreadth, rearLegsHeight, rearLegsThickness);
		solids[SO_LEFT_REAR_LEG]->AddShape(phSdk->CreateShape(boxDesc));
		solids[SO_LEFT_REAR_LEG]->SetName("soLeftRearLeg");
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
		joints[JO_BODY_RIGHT_REAR_LEG] = CreateJoint(solids[SO_BODY], solids[SO_RIGHT_REAR_LEG], ballDesc);
		joints[JO_BODY_RIGHT_REAR_LEG]->SetName("joRightHip");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_RIGHT_REAR_LEG], PHSceneDesc::MODE_NONE);
	}
	else{
		joints[JO_BODY_LEFT_REAR_LEG] =  CreateJoint(solids[SO_BODY], solids[SO_LEFT_REAR_LEG], ballDesc);
		joints[JO_BODY_LEFT_REAR_LEG]->SetName("joLeftHip");
		phScene->SetContactMode(solids[SO_BODY], solids[SO_LEFT_REAR_LEG], PHSceneDesc::MODE_NONE);
	}
}

// --- --- ---
void CRFourLegsTinyAnimalBody::InitLegs(){
	if(! noLegs){
		CreateFrontLegs(RIGHTPART);
		/*if(! onlyOneLeg){*/
			CreateFrontLegs(LEFTPART);
			/*CreateRearLegs(RIGHTPART);
			CreateRearLegs(LEFTPART);
		}*/
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
}

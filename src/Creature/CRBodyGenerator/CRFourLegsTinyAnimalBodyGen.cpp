/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyGenerator/CRFourLegsTinyAnimalBodyGen.h>
#include <Collision/SprCDShape.h>
#include <Physics/SprPHSdk.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSolid.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRFourLegsTinyAnimalBodyGenDesc::CRFourLegsTinyAnimalBodyGenDesc(){

	jointType	= HINGE_MODE;
	shapeMode	= MODE_CAPSULE;//MODE_BOX;
	
	// 体幹に関するパラメータ
	bodyBreadth   = 2.5;
	bodyHeight    = 3.8;
	bodyThickness = 0.6;

	// 脚のRounConeに関するパラメータ
	upperSizes = Vec2d(0.2, 0.4);
	lowerSizes = Vec2d(0.2, 0.3);
	upperLength = 1.0;
	lowerLength = 1.4;

	// 質量に関するパラメータ
	massFF		= 1.5;
	massFL		= 1.5;
	massRF		= 1.5;
	massRL		= 1.5;
	massBody	= 10;

	// バネ・ダンパ係数
	springFront   = 1.0;  damperFront   =  5.0;
	springRear    = 1.0;  damperRear    =  5.0;

	// IF (jointType == BALL_MODE) THEN
	// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
	rangeFrontSwing   = Vec2d(FLT_MAX, FLT_MAX);
	rangeFrontTwist	  = Vec2d(FLT_MAX, FLT_MAX);
	rangeRearSwing    = Vec2d(FLT_MAX, FLT_MAX);
	rangeRearTwist	  = Vec2d(FLT_MAX, FLT_MAX);

	// ELSE IF(jointType == HINGE_MODE) THEN
	// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
	rangeUpperFront = Vec2d(FLT_MAX, FLT_MAX);
	rangeLowerFront = Vec2d(FLT_MAX, FLT_MAX);
	rangeUpperRear  = Vec2d(FLT_MAX, FLT_MAX);
	rangeLowerRear  = Vec2d(FLT_MAX, FLT_MAX);

	// デフォルトで四脚作る
	rfLeg = true;
	lfLeg = true;
	rrLeg = true;
	lrLeg = true;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsTinyAnimalBodyGen


// --- --- ---
void CRFourLegsTinyAnimalBodyGen::Init(){
	CRBodyGen::Init();
}

// --- --- ---
PHSolidIf* CRFourLegsTinyAnimalBodyGen::InitBody(){
	return CreateBody();
}

PHSolidIf* CRFourLegsTinyAnimalBodyGen::CreateBody(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass	= massBody;
	solids.push_back(phScene->CreateSolid(solidDesc));
	boxDesc.boxsize	= Vec3f(bodyBreadth, bodyHeight, bodyThickness);
	solids.back()->AddShape(phSdk->CreateShape(boxDesc));

	solids.back()->SetFramePosition(Vec3f(0,0,0));
	solids.back()->SetOrientation(Quaterniond::Rot(Rad(-90), 'x'));
	solids.back()->SetName("soWaist");
	solids.back()->SetDynamical(false);

	return solids.back();
}
PHSolidIf* CRFourLegsTinyAnimalBodyGen::InitFrontLeg0(LREnum lr, PHSolidIf* sBody){	
	PHSolidDesc			solidDesc;

	// Solid
	solidDesc.mass   = massFL;
	solids.push_back(phScene->CreateSolid(solidDesc));
	if(shapeMode == MODE_ROUNDCONE){
		CDRoundConeDesc	 rcDesc;
		rcDesc.radius	 = upperSizes;
		rcDesc.length	 = upperLength;
		solids.back()->AddShape(phSdk->CreateShape(rcDesc));
	} else if(shapeMode == MODE_BOX){
		CDBoxDesc boxDesc;
		boxDesc.boxsize = Vec3d(upperSizes[1], upperSizes[1], upperLength);
		solids.back()->AddShape(phSdk->CreateShape(boxDesc));
	} else if(shapeMode == MODE_CAPSULE){
		CDCapsuleDesc capDesc;
		capDesc.radius = upperSizes[1];
		capDesc.length = upperLength;
		solids.back()->AddShape(phSdk->CreateShape(capDesc));
	} else if(shapeMode == MODE_SPHERE){
		CDSphereDesc sphDesc;
		sphDesc.radius = (float)upperSizes[1];
		solids.back()->AddShape(phSdk->CreateShape(sphDesc));
	}
	// 体幹と前脚間の関節
	if(jointType == HINGE_MODE){
		PHHingeJointDesc hingeDesc;
		hingeDesc.poseSocket.Pos()	= Vec3d(-lr * bodyBreadth / 2.0, -bodyHeight / 2.0, 0);
		hingeDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.posePlug.Pos()	= Vec3d(0, 0, upperLength / 2.0);
		hingeDesc.posePlug.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.targetPosition			= 0;
		hingeDesc.spring			= springFront;
		hingeDesc.damper			= damperFront;
		hingeDesc.upper				= rangeUpperFront.upper;
		hingeDesc.lower				= rangeUpperFront.lower;
		joints.push_back(phScene->CreateJoint(sBody, solids.back(), hingeDesc));
	} else if(jointType == BALL_MODE){
		PHBallJointDesc		ballDesc;
		ballDesc.poseSocket.Pos()	= Vec3d(-lr * bodyBreadth / 2.0, -bodyHeight / 2.0, 0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, 0, upperLength / 2.0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.targetPosition				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springFront;
		ballDesc.damper				= damperFront;
		ballDesc.limitSwing			= rangeFrontSwing;
		ballDesc.limitTwist			= rangeFrontTwist;
		joints.push_back(phScene->CreateJoint(sBody, solids.back(), ballDesc));
	}
	phScene->SetContactMode(sBody, solids.back(), PHSceneDesc::MODE_NONE);

	if(lr == RIGHTPART){
		solids.back()->SetName("soRightFrontLeg0");
		joints.back()->SetName("joRightShoulder");
	} else{
		solids.back()->SetName("soLeftFrontLeg0");
		joints.back()->SetName("joLeftShoulder");
	}

	return solids.back();
}
void CRFourLegsTinyAnimalBodyGen::InitFrontLeg1(LREnum lr, PHSolidIf* sLeg0){
	
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = massFF;

	solids.push_back(phScene->CreateSolid(solidDesc));
	if(shapeMode == MODE_ROUNDCONE){
		CDRoundConeDesc		rcDesc;
		rcDesc.radius = lowerSizes;
		rcDesc.length = lowerLength;
		solids.back()->AddShape(phSdk->CreateShape(rcDesc));
	} else if(shapeMode == MODE_BOX){
		CDBoxDesc boxDesc;
		boxDesc.boxsize = Vec3d(lowerSizes[1], lowerSizes[1], lowerLength);
		solids.back()->AddShape(phSdk->CreateShape(boxDesc));
	} else if(shapeMode == MODE_CAPSULE){
		CDCapsuleDesc capDesc;
		capDesc.radius = lowerSizes[1];
		capDesc.length = lowerLength;
		solids.back()->AddShape(phSdk->CreateShape(capDesc));
	} else if(shapeMode == MODE_SPHERE){
		CDSphereDesc sphDesc;
		sphDesc.radius = (float)lowerSizes[1];
		solids.back()->AddShape(phSdk->CreateShape(sphDesc));
	} 
	// 前脚-前足間の関節
	if(jointType == HINGE_MODE){
		PHHingeJointDesc hingeDesc;
		hingeDesc.poseSocket.Pos()	= Vec3d(0, 0, -upperLength/2.0);
		hingeDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.posePlug.Pos()	= Vec3d(0, 0, lowerLength / 2.0);
		hingeDesc.posePlug.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.targetPosition			= 0;
		hingeDesc.spring			= springFront;
		hingeDesc.damper			= damperFront;
		hingeDesc.upper				= rangeLowerFront.upper;
		hingeDesc.lower				= rangeLowerFront.lower;
		joints.push_back(phScene->CreateJoint(sLeg0, solids.back(), hingeDesc));
	} else if(jointType == BALL_MODE){
		ballDesc.poseSocket.Pos()	= Vec3d(0, 0, -upperLength/2.0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, 0, lowerLength / 2.0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.targetPosition				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springFront;
		ballDesc.damper				= damperFront;
		ballDesc.limitSwing			= rangeFrontSwing;
		ballDesc.limitTwist			= rangeFrontTwist;
		joints.push_back(phScene->CreateJoint(sLeg0, solids.back(), ballDesc));
	}
	phScene->SetContactMode(sLeg0, solids.back(), PHSceneDesc::MODE_NONE);

	if(lr == RIGHTPART){
		solids.back()->SetName("soRightFrontLeg1");
		joints.back()->SetName("joRightElbow");
	} else{
		solids.back()->SetName("soLeftFrontLeg1");
		joints.back()->SetName("joLeftElbow");
	}

}
PHSolidIf* CRFourLegsTinyAnimalBodyGen::InitRearLeg0(LREnum lr, PHSolidIf* sBody){
	
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = massRL;

	solids.push_back(phScene->CreateSolid(solidDesc));
	if(shapeMode == MODE_ROUNDCONE){
		CDRoundConeDesc		rcDesc;
		rcDesc.radius = upperSizes;
		rcDesc.length = upperLength;
		solids.back()->AddShape(phSdk->CreateShape(rcDesc));
	} else if(shapeMode == MODE_BOX){
		CDBoxDesc boxDesc;
		boxDesc.boxsize = Vec3d(upperSizes[1], upperSizes[1], upperLength);
		solids.back()->AddShape(phSdk->CreateShape(boxDesc));
	} else if(shapeMode == MODE_CAPSULE){
		CDCapsuleDesc capDesc;
		capDesc.radius = upperSizes[1];
		capDesc.length = upperLength;
		solids.back()->AddShape(phSdk->CreateShape(capDesc));
	} else if(shapeMode == MODE_SPHERE){
		CDSphereDesc sphDesc;
		sphDesc.radius = (float)upperSizes[1];
		solids.back()->AddShape(phSdk->CreateShape(sphDesc));
	}
	// 体幹と後脚間の関節
	if(jointType == HINGE_MODE){
		PHHingeJointDesc hingeDesc;
		hingeDesc.poseSocket.Pos()	= Vec3d(-lr * bodyBreadth / 2.0, bodyHeight / 2.0, 0);
		hingeDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.posePlug.Pos()	= Vec3d(0, 0, upperLength / 2.0);
		hingeDesc.posePlug.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.targetPosition			= 0;
		hingeDesc.spring			= springRear;
		hingeDesc.damper			= damperRear;
		hingeDesc.upper				= rangeUpperRear.upper;
		hingeDesc.lower				= rangeUpperRear.lower;
		joints.push_back(phScene->CreateJoint(sBody, solids.back(), hingeDesc));
	}else if(jointType == BALL_MODE){
		ballDesc.poseSocket.Pos()	= Vec3d(-lr * bodyBreadth / 2.0, bodyHeight / 2.0, 0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, 0, upperLength / 2.0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.targetPosition				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springRear;
		ballDesc.damper				= damperRear;
		ballDesc.limitSwing			= rangeRearSwing;
		ballDesc.limitTwist			= rangeRearTwist;
		joints.push_back(phScene->CreateJoint(sBody, solids.back(), ballDesc));
	}
	phScene->SetContactMode(sBody, solids.back(), PHSceneDesc::MODE_NONE);

	if(lr == RIGHTPART){
		solids.back()->SetName("soRightRearLeg0");
		joints.back()->SetName("joRightHip");
	} else{
		solids.back()->SetName("soLeftRearLeg0");
		joints.back()->SetName("joLeftHip");
	}

	return solids.back();
}
void CRFourLegsTinyAnimalBodyGen::InitRearLeg1(LREnum lr, PHSolidIf* sLeg0){
	
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// Solid
	solidDesc.mass   = massRF;
	solids.push_back(phScene->CreateSolid(solidDesc));
	if(shapeMode == MODE_ROUNDCONE){
		CDRoundConeDesc		rcDesc;
		rcDesc.radius = lowerSizes;
		rcDesc.length = lowerLength;
		solids.back()->AddShape(phSdk->CreateShape(rcDesc));
	} else if(shapeMode == MODE_BOX){
		CDBoxDesc boxDesc;
		boxDesc.boxsize = Vec3d(lowerSizes[1], lowerSizes[1], lowerLength);
		solids.back()->AddShape(phSdk->CreateShape(boxDesc));
	} else if(shapeMode == MODE_CAPSULE){
		CDCapsuleDesc capDesc;
		capDesc.radius = lowerSizes[1];
		capDesc.length = lowerLength;
		solids.back()->AddShape(phSdk->CreateShape(capDesc));
	} else if(shapeMode == MODE_SPHERE){
		CDSphereDesc sphDesc;
		sphDesc.radius = (float)lowerSizes[1];
		solids.back()->AddShape(phSdk->CreateShape(sphDesc));
	}
	//後脚-後足間の関節
	if(jointType == HINGE_MODE){
		PHHingeJointDesc hingeDesc;
		hingeDesc.poseSocket.Pos()	= Vec3d(0, 0, -upperLength/2.0);
		hingeDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.posePlug.Pos()	= Vec3d(0, 0, lowerLength / 2.0);
		hingeDesc.posePlug.Ori()	= Quaterniond::Rot(Rad(-90), 'y');
		hingeDesc.targetPosition			= 0;
		hingeDesc.spring			= springRear;
		hingeDesc.damper			= damperRear;
		hingeDesc.upper				= rangeLowerRear.upper;
		hingeDesc.lower				= rangeLowerRear.lower;
		joints.push_back(phScene->CreateJoint(sLeg0, solids.back(), hingeDesc));
	} else if(jointType == BALL_MODE){
		ballDesc.poseSocket.Pos()	= Vec3d(0, 0, -upperLength / 2.0);
		ballDesc.poseSocket.Ori()	= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.posePlug.Pos()		= Vec3d(0, 0,  lowerLength / 2.0);
		ballDesc.posePlug.Ori()		= Quaterniond::Rot(Rad(-90), 'x');
		ballDesc.targetPosition				= Quaterniond::Rot(Rad(90), 'x');
		ballDesc.spring				= springRear;
		ballDesc.damper				= damperRear;
		ballDesc.limitSwing			= rangeRearSwing;
		ballDesc.limitTwist			= rangeRearTwist;
		joints.push_back(phScene->CreateJoint(sLeg0, solids.back(), ballDesc));
	}
	phScene->SetContactMode(sLeg0, solids.back(), PHSceneDesc::MODE_NONE);
	if(lr == RIGHTPART){
		solids.back()->SetName("soRightRearLeg1");
		joints.back()->SetName("joRightRearElbow");
	} else{
		solids.back()->SetName("soLeftRearLeg1");
		joints.back()->SetName("joLeftRearElbow");
	}
}

void CRFourLegsTinyAnimalBodyGen::CreateFrontLegs(LREnum lr, PHSolidIf* sBody){
	PHSolidIf* leg0 = InitFrontLeg0(lr, sBody);
	InitFrontLeg1(lr, leg0);
}

void CRFourLegsTinyAnimalBodyGen::CreateRearLegs(LREnum lr, PHSolidIf* sBody){
	PHSolidIf* leg0 = InitRearLeg0(lr, sBody);
	InitRearLeg1(lr, leg0);
}

// --- --- ---
void CRFourLegsTinyAnimalBodyGen::InitLegs(PHSolidIf* sBody){
	if(rfLeg) CreateFrontLegs(RIGHTPART, sBody);
	if(lfLeg) CreateFrontLegs(LEFTPART,  sBody);
	if(rrLeg) CreateRearLegs (RIGHTPART, sBody);
	if(lrLeg) CreateRearLegs (LEFTPART,  sBody);
}

// --- --- ---
void CRFourLegsTinyAnimalBodyGen::InitContact(){
	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	for (unsigned int i=0; i<solids.size(); ++i) {
		for (unsigned int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i], solids[j], PHSceneDesc::MODE_NONE);
			}
		}
	}
}

}

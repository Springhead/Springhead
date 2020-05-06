/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyGenerator/CRFourLegsAnimalBodyGen.h>
#include <Collision/SprCDShape.h>
#include <Physics/SprPHSdk.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSolid.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRFourLegsAnimalBodyGenDesc::CRFourLegsAnimalBodyGenDesc(bool enableRange, bool enableFMax){

	jointOrder = SOCKET_PARENT;

	//---------------------------------------------------------
	// number of the joints or solids
	joNBallJoints	= 0;
	joNHingeJoints	= 0;
	flagRange		= enableRange;
	flagFMax		= enableFMax;
	//---------------------------------------------------------
	// sizes of solids
	waistBreadth			= 0.5;		waistHeight				= 0.8;		waistThickness				= 0.6;
	chestBreadth			= 0.6;		chestHeight				= 1.5;		chestThickness				= 0.7;
	tailBreadth				= 0.1;		tailHeight				= 0.2;		tailThickness				= 0.1;
	neckBreadth				= 0.3;		neckHeight				= 1.2;		neckThickness				= 0.2;
	headBreadth				= 0.25;		headHeight				= 0.7;		headThickness				= 0.4;
	breastboneBreadth		= 0.25;		breastboneHeight		= 0.5;		breastboneThickness			= 0.25; 
	radiusBreadth			= 0.2;		radiusHeight			= 0.7;		radiusThickness				= 0.2;
	frontCannonBoneBreadth	= 0.15;		frontCannonBoneHeight	= 0.6;		frontCannonBoneThickness	= 0.15;
	frontToeBreadth			= 0.26;		frontToeHeight			= 0.1;		frontToeThickness			= 0.25;
	femurBreadth			= 0.25;		femurHeight				= 0.65;		femurThickness				= 0.25;
	tibiaBreadth			= 0.2;		tibiaHeight				= 0.7;		tibiaThickness				= 0.2;
	rearCannonBoneBreadth	= 0.15;		rearCannonBoneHeight    = 0.65;		rearCannonBoneThickness		= 0.15;
	rearToeBreadth			= 0.26;		rearToeHeight			= 0.1;		rearToeThickness			= 0.25;
	//------------------------------------------------------------------
	// spring and damper of ball joints
	springWaistChest   =  1.0;/*500.0;*/ damperWaistChest	= 200.0;
	springWaistTail    =  10.0;/* 10.0;*/ damperWaistTail	=	0.1;
	springTail		   =  10.0;/* 10.0;*/ damperTail			=	0.1;
	springChestNeck    =  30.0;/*100.0;*/ damperChestNeck	=  20.0;
	springNeckHead	   =  10.0;/*100.0;*/ damperNeckHead		=  20.0;
	springShoulder	   =  20.0;/* 50.0;*/ damperShoulder		=  20.0;
	springFrontAnkle   = 10.0;/*1000.0;*/ damperFrontAnkle	=  20.0;
	springHip		   =  30.0;/* 50.0;*/ damperHip			=  20.0;
	springRearAnkle	   = 10.0;/*1000.0;*/ damperRearAnkle	=  20.0;
	//-------------------------------------------------------------------
	// spring and damper of hinge joints
	springElbow		   =   10.0;/*50.0;*/ damperElbow		= 20.0;
	springFrontKnee	   =   10.0;/*50.0;*/ damperFrontKnee	= 20.0;
	springStifle	   =   10.0;/*50.0;*/ damperStifle		= 20.0;
	springRearKnee	   =   10.0;/*50.0;*/ damperRearKnee		= 20.0;
	//-------------------------------------------------------------------
	// ball joints are conrtroled to these directions
	goalWaistTail	  = Quaterniond::Rot(Rad(  60), 'x');
	goalTail		  = Quaterniond::Rot(Rad(  20), 'x');
	goalChestNeck	  = Quaterniond::Rot(Rad(  60), 'x');
	goalNeckHead	  = Quaterniond::Rot(Rad(- 90), 'x');
	goalShoulder	  = Quaterniond::Rot(Rad(-120), 'x');
	goalHip			  = Quaterniond::Rot(Rad(- 65), 'x');
	//-------------------------------------------------------------------
	// hinge joints are controled to these directions
	originElbow		  = Rad(40);
	originFrontKnee	  = Rad(0);
	originStifle	  = Rad(-45);
	originRearKnee	  = Rad(20);
	//-------------------------------------------------------------------
	// Range of ball joints
	limitSwingWaistChest.upper	= Rad(5);
	limitTwistWaistChest.lower	= Rad(-5);
	limitTwistWaistChest.upper	= Rad(5);

	limitSwingWaistTail.upper	= Rad(10);
	limitTwistWaistTail.lower	= Rad(-5);
	limitTwistWaistTail.upper	= Rad(5);

	limitSwingTail.upper		= Rad(20);
	limitTwistTail.lower		= Rad(-5);
	limitTwistTail.upper		= Rad(5);
	
	limitSwingChestNeck.upper	= Rad(180);
	limitTwistChestNeck.lower	= Rad(-5);
	limitTwistChestNeck.upper	= Rad(5);

	limitSwingNeckHead.upper	= Rad(150);
	limitTwistNeckHead.lower	= Rad(-10);
	limitTwistNeckHead.upper	= Rad(10);

	limitSwingShoulder.upper	= Rad(180);
	limitTwistShoulder.lower	= Rad(-10);
	limitTwistShoulder.upper	= Rad(10);

	limitSwingFrontAnkle.upper	= Rad(5);
	limitTwistFrontAnkle.lower	= Rad(-1);
	limitTwistFrontAnkle.upper	= Rad(1);

	limitSwingHip.upper			= Rad(180);
	limitTwistHip.lower			= Rad(-10);
	limitTwistHip.upper			= Rad(10);

	limitSwingRearAnkle.upper	= Rad(5);
	limitTwistRearAnkle.lower	= Rad(-1);
	limitTwistRearAnkle.upper	= Rad(1);
	//-----------------------------------------------------------------------------------
	// Range of hinge joints (Vec2d(lower, upper)  lower>upperのとき可動域制限無効)
	rangeElbow		  = Vec2d(Rad(	 0), Rad(+180));
	rangeFrontKnee	  = Vec2d(Rad(-180), Rad(	0));
	rangeStifle		  = Vec2d(Rad(-180), Rad(	0));
	rangeRearKnee	  = Vec2d(Rad(	 0), Rad(+180));
	//-----------------------------------------------------------------------------------
	// fMax[N]
	fMaxWaistChest		= 1000;
	fMaxChestNeck		= 1000;
	fMaxNeckHead		= 1000;
	fMaxWaistTail		= 1000;
	fMaxTail12			= 1000;
	fMaxTail23			= 1000;
	fMaxLeftShoulder	= 100;
	fMaxLeftElbow		= 50;
	fMaxLeftFrontKnee	= 50;
	fMaxLeftFrontAnkle	= 100;
	fMaxLeftHip			= 100;
	fMaxLeftStifle		= 50;
	fMaxLeftRearKnee	= 50;
	fMaxLeftRearAnkle	= 100;
	fMaxRightShoulder	= 100;
	fMaxRightElbow		= 50;
	fMaxRightFrontKnee	= 50;
	fMaxRightFrontAnkle	= 100;
	fMaxRightHip		= 100;
	fMaxRightStifle		= 50;
	fMaxRightRearKnee	= 50;
	fMaxRightRearAnkle	= 100;
	//-----------------------------------------------------------------------------------
	// materialMu of All Solids 
	materialMu = (float)1.0;
	//-----------------------------------------------------------------------------------
	// Do you want to make no legs or head?
	noLegs = false;
	noHead = false;

	//-----------------------------------------------------------------------------------
	// Dynamical 
	dynamicalMode = false;
	//-----------------------------------------------------------------------------------
	// Total Mass
	totalMass = 1.0;
}	
	
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsAnimalBodyGen

// --- --- ---
void CRFourLegsAnimalBodyGen::InitBody(){
	CreateWaist();
	CreateChest();
	if(! noLegs && ! noHead) CreateTail();
}

void CRFourLegsAnimalBodyGen::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	// define the solid(it has no shape)
	solidDesc.mass   = totalMass * VSolid(SO_WAIST) / VSolids();
	solids[SO_WAIST] = phScene->CreateSolid(solidDesc);
	solids[SO_WAIST]->SetName("soWaist");
	// define the shape of the solid.
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_WAIST]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	//solids[SO_WAIST]->SetOrientation(Quaterniond::Rot(Rad(-90), 'x'));

	//重力切るか
	solids[SO_WAIST]->SetDynamical(dynamicalMode);

}

void CRFourLegsAnimalBodyGen::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass   = totalMass * VSolid(SO_CHEST) / VSolids();
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	solids[SO_CHEST]->SetName("soChest");
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_CHEST]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));
	
	// Joint -- [p]Waist - [c]Chest
	ballDesc                  = PHBallJointDesc();
	ballDesc.poseSocket.Pos() = Vec3f(0.0, waistHeight / 2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.posePlug.Pos()   = Vec3f(0.0, -chestHeight / 2.0, 0.0);
	ballDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.spring           = springWaistChest;
	ballDesc.damper           = damperWaistChest;
	ballDesc.targetPosition			  = goalWaistChest;

	if(flagRange){
	ballDesc.limitSwing		  = limitSwingWaistChest;
	ballDesc.limitTwist		  = limitTwistWaistChest;	
	}
	if(flagFMax){
		ballDesc.fMax = fMaxWaistChest * phScene->GetTimeStep();
	}

	//----------------------------------------------------------------------------
	// PHScene::CreateJoint([p], [c], desc)で使う。
	// <memo>: [絶滅API]CRBody::CreateJoint([c], [p], desc)と宣言が逆なので注意！
	//----------------------------------------------------------------------------
	joints[JO_WAIST_CHEST]    = phScene->CreateJoint(solids[SO_WAIST], solids[SO_CHEST], ballDesc);		
	joints[JO_WAIST_CHEST]->SetName("joWaistChest");

	phScene->SetContactMode(solids[SO_WAIST], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBodyGen::CreateTail(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solids
	// define the existance
	solidDesc.mass	= totalMass * VSolid(SO_TAIL1) / VSolids();
	solids[SO_TAIL1] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL1]->SetName("soTail1");
	solidDesc.mass  = totalMass * VSolid(SO_TAIL2) / VSolids();
	solids[SO_TAIL2] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL2]->SetName("soTail2");
	solidDesc.mass	= totalMass * VSolid(SO_TAIL3) / VSolids();
	solids[SO_TAIL3] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL3]->SetName("soTail3");
	
	// define the shape
	boxDesc.boxsize = Vec3f(tailBreadth, tailHeight, tailThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[SO_TAIL1]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL2]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL3]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL1]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solids[SO_TAIL1]->GetMass()));
	solids[SO_TAIL2]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solids[SO_TAIL2]->GetMass()));
	solids[SO_TAIL3]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solids[SO_TAIL3]->GetMass()));

	// define the connection
	// [p]waist - [c]tail1
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, -waistHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, tailHeight/2.0, 0.1);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring            = springWaistTail;
	ballDesc.damper            = damperWaistTail;
	ballDesc.targetPosition			   = goalWaistTail;

	if(flagRange){
	ballDesc.limitSwing		   = limitSwingWaistTail;
	ballDesc.limitTwist		   = limitTwistWaistTail;
	}
	if(flagFMax){
		ballDesc.fMax = fMaxWaistTail * phScene->GetTimeStep();
	}

	joints[JO_WAIST_TAIL]	   = phScene->CreateJoint(solids[SO_WAIST], solids[SO_TAIL1], ballDesc);
	joints[JO_WAIST_TAIL]->SetName("joWaistTail");

	//[p]tail1 - [c]tail2
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, tailHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.targetPosition			   = goalTail;

	if(flagRange){
	ballDesc.limitSwing		   = limitSwingTail;
	ballDesc.limitTwist		   = limitTwistTail;
	}
	if(flagFMax){
		ballDesc.fMax = fMaxTail12 * phScene->GetTimeStep();
	}

	joints[JO_TAIL_12]		   = phScene->CreateJoint(solids[SO_TAIL1], solids[SO_TAIL2], ballDesc);
	joints[JO_TAIL_12]->SetName("joTail12");

	//[p]tail2 - [c]tail3
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, tailHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.targetPosition			   = goalTail;

	if(flagRange){
	ballDesc.limitSwing		   = limitSwingTail;
	ballDesc.limitTwist		   = limitTwistTail;
	}
	if(flagFMax){
		ballDesc.fMax = fMaxTail23 * phScene->GetTimeStep();
	}

	joints[JO_TAIL_23]		   = phScene->CreateJoint(solids[SO_TAIL2], solids[SO_TAIL3], ballDesc);
	joints[JO_TAIL_23]->SetName("joTail23");
	// define the position
	
	phScene->SetContactMode(solids[SO_TAIL1], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_TAIL2], solids[SO_TAIL1], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_TAIL3], solids[SO_TAIL2], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRFourLegsAnimalBodyGen::InitHead(){
	if(! noHead){
		CreateNeck();
		CreateHead();
	}
}

void CRFourLegsAnimalBodyGen::CreateNeck(){
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// define the solid
	solidDesc.mass = totalMass * VSolid(SO_NECK) / VSolids();
	solids[SO_NECK] = phScene->CreateSolid(solidDesc);
	solids[SO_NECK]->SetName("soNeck");

	// define the shape
	boxDesc.boxsize = Vec3f(neckBreadth, neckHeight, neckThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[SO_NECK]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_NECK]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	// define the joint  [p]chest - [c]neck
	ballDesc.poseSocket.Pos() = Vec3f(0.0, chestHeight/2.0,  0.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(150), 'x');
	ballDesc.posePlug.Pos()   = Vec3f(0.0, -neckHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(150), 'x');
	ballDesc.spring			  = springChestNeck;
	ballDesc.damper			  = damperChestNeck;
	ballDesc.targetPosition			  = goalChestNeck;

	if(flagRange){
	ballDesc.limitSwing		  = limitSwingChestNeck;
	ballDesc.limitTwist		  = limitTwistChestNeck;
	}
	if(flagFMax){
		ballDesc.fMax = fMaxChestNeck * phScene->GetTimeStep();
	}

	joints[JO_CHEST_NECK] = phScene->CreateJoint(solids[SO_CHEST], solids[SO_NECK], ballDesc);
	joints[JO_CHEST_NECK]->SetName("joChestNeck");

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_NECK], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBodyGen::CreateHead(){

// BallJoint ver.

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// define the solid
	solidDesc.mass = totalMass * VSolid(SO_HEAD) / VSolids();
	solids[SO_HEAD] = phScene->CreateSolid(solidDesc);
	solids[SO_HEAD]->SetName("soHead");
	// define the shape
	boxDesc.boxsize = Vec3f(headBreadth, headHeight, headThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_HEAD]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	// define the connection  [p]neck - [c]head
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, neckHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, -headHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.spring            = springNeckHead;
	ballDesc.damper            = damperNeckHead;
	ballDesc.targetPosition			   = goalNeckHead;
	
	if(flagRange){
	ballDesc.limitSwing		   = limitSwingNeckHead;
	ballDesc.limitTwist		   = limitTwistNeckHead;
	}
	if(flagFMax){
		ballDesc.fMax = fMaxNeckHead * phScene->GetTimeStep();
	}

	joints[JO_NECK_HEAD]	   = phScene->CreateJoint(solids[SO_NECK], solids[SO_HEAD], ballDesc);
	joints[JO_NECK_HEAD]->SetName("joNeckHead");

	phScene->SetContactMode(solids[SO_NECK], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
	

}

// --- --- ---
void CRFourLegsAnimalBodyGen::InitFrontLegs(){

	if(! noLegs){
		CreateBreastBone(LEFTPART);
		CreateRadius(LEFTPART);
		CreateFrontCannonBone(LEFTPART);
		CreateFrontToeBones(LEFTPART);
	}

	if(! noLegs){
		CreateBreastBone(RIGHTPART);
		CreateRadius(RIGHTPART);
		CreateFrontCannonBone(RIGHTPART);
		CreateFrontToeBones(RIGHTPART);
	}
	phScene->SetContactMode(solids[SO_LEFT_BREASTBONE], solids[SO_RIGHT_BREASTBONE], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBodyGen::CreateBreastBone(LREnum lr){

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;
	int soBreastbone;
	int joShoulder;
	

	if(lr == LEFTPART){
		soBreastbone  = SO_LEFT_BREASTBONE;
		joShoulder    = JO_LEFT_SHOULDER;
	}

	else if(lr == RIGHTPART){
		soBreastbone  = SO_RIGHT_BREASTBONE;
		joShoulder    = JO_RIGHT_SHOULDER;
	}
	else {

	}
	// [p]chest - [c]breastbone
	solidDesc.mass = totalMass * VSolid(soBreastbone) / VSolids();
	solids[soBreastbone] = phScene->CreateSolid(solidDesc);
	
	boxDesc.boxsize = Vec3f(breastboneBreadth, breastboneHeight, breastboneThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soBreastbone]->AddShape(phSdk->CreateShape(boxDesc));
	if(lr == LEFTPART){
		solids[soBreastbone]->SetName("soLeftBreastbone");
	} else{
		solids[soBreastbone]->SetName("soRightBreastbone");
	}
	solids[soBreastbone]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));
	
	ballDesc.poseSocket.Pos() = Vec3f(lr*chestBreadth/2.0, chestHeight/2.2, chestThickness/3.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.posePlug.Pos()	  = Vec3f(-lr*breastboneBreadth/2.0, -breastboneHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.spring			  = springShoulder;
	ballDesc.damper			  = damperShoulder;
	ballDesc.targetPosition			  = goalShoulder;
	
	if(flagRange){
	ballDesc.limitSwing		  = limitSwingShoulder;
	ballDesc.limitTwist		  = limitTwistShoulder;
	}
	if(flagFMax){
		ballDesc.fMax = (lr == LEFTPART) ? fMaxLeftShoulder * phScene->GetTimeStep() : fMaxRightShoulder * phScene->GetTimeStep();
	}

	//----------------------------------------------------------------------------
	// phScene->CreateJoint([p], [c], desc)で使う。
	// <memo>: [絶滅API]CRBody::CreateJoint([c], [p], desc)と宣言が逆なので注意！
	//----------------------------------------------------------------------------
	joints[joShoulder] = phScene->CreateJoint(solids[SO_CHEST], solids[soBreastbone], ballDesc);
	if(lr == LEFTPART)
		joints[joShoulder]->SetName("joLeftShoulder");
	else
		joints[joShoulder]->SetName("joRightShoulder");

	phScene->SetContactMode(solids[SO_CHEST], solids[soBreastbone], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_NECK], solids[soBreastbone], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBodyGen::CreateRadius(LREnum lr){

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;
	int soRadius, soBreastbone;
	int joElbow;

	if(lr == LEFTPART){
		soRadius	 = SO_LEFT_RADIUS;
		soBreastbone = SO_LEFT_BREASTBONE;
		joElbow		 = JO_LEFT_ELBOW; 
	}

	else if(lr == RIGHTPART){
		soRadius	 = SO_RIGHT_RADIUS;
		soBreastbone = SO_RIGHT_BREASTBONE;
		joElbow		 = JO_RIGHT_ELBOW;
	}

	solidDesc.mass = totalMass * VSolid(soRadius) / VSolids();;
	solids[soRadius] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soRadius]->SetName("soLeftRadius");
	else
		solids[soRadius]->SetName("soRightRadius");

	boxDesc.boxsize = Vec3f(radiusBreadth, radiusHeight, radiusThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soRadius]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soRadius]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	//[p]breastbone - [c]radius
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, breastboneHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -radiusHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springElbow;
	hingeDesc.damper		   = damperElbow;
	hingeDesc.targetPosition		   = originElbow;

	if(flagRange){
	hingeDesc.lower			   = rangeElbow[0];
	hingeDesc.upper			   = rangeElbow[1];
	}
	if(flagFMax){
		hingeDesc.fMax = (lr == LEFTPART) ? fMaxLeftElbow * phScene->GetTimeStep() : fMaxRightElbow * phScene->GetTimeStep();
	}

	joints[joElbow] = phScene->CreateJoint(solids[soBreastbone], solids[soRadius], hingeDesc);
	if(lr == LEFTPART)
		joints[joElbow]->SetName("joLeftElbow");
	else
		joints[joElbow]->SetName("joRightElbow");

	phScene->SetContactMode(solids[soBreastbone], solids[soRadius], PHSceneDesc::MODE_NONE);
}	

void CRFourLegsAnimalBodyGen::CreateFrontCannonBone(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soCannonBone, soRadius;
	int joKnee;

	if(lr == LEFTPART){
		soCannonBone = SO_LEFT_FRONT_CANNON_BONE;
		soRadius	 = SO_LEFT_RADIUS;
		joKnee		 = JO_LEFT_FRONT_KNEE;
	}

	else if(lr == RIGHTPART){
		soCannonBone = SO_RIGHT_FRONT_CANNON_BONE;
		soRadius	 = SO_RIGHT_RADIUS;
		joKnee		 = JO_RIGHT_FRONT_KNEE;
	}

	solidDesc.mass = totalMass * VSolid(soCannonBone) / VSolids();
	solids[soCannonBone] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soCannonBone]->SetName("soLeftFrontCannonBone");
	else
		solids[soCannonBone]->SetName("soRightFrontCannonBone");

	boxDesc.boxsize = Vec3f(frontCannonBoneBreadth, frontCannonBoneHeight, frontCannonBoneThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soCannonBone]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soCannonBone]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	// [p]Radius - [c]CannonBone
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, radiusHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -frontCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springFrontKnee;
	hingeDesc.damper		   = damperFrontKnee;
	hingeDesc.targetPosition		   = originFrontKnee;
	
	if(flagRange){
	hingeDesc.lower			   = rangeFrontKnee[0];
	hingeDesc.upper			   = rangeFrontKnee[1];
	}
	if(flagFMax){
		hingeDesc.fMax = (lr == LEFTPART) ? fMaxLeftFrontKnee * phScene->GetTimeStep() : fMaxRightFrontKnee * phScene->GetTimeStep();
	}

	joints[joKnee] = phScene->CreateJoint(solids[soRadius], solids[soCannonBone], hingeDesc);
	if(lr == LEFTPART)
		joints[joKnee]->SetName("joLeftFrontKnee");
	else
		joints[joKnee]->SetName("joRightFrontKnee");

	phScene->SetContactMode(solids[soRadius], solids[soCannonBone], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBodyGen::CreateFrontToeBones(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHBallJointDesc  ballDesc;
	int soToe, soCannonBone;
	int joAnkle;


	if(lr == LEFTPART){
		soToe		 = SO_LEFT_FRONT_TOE;
		soCannonBone = SO_LEFT_FRONT_CANNON_BONE;
		joAnkle		 = JO_LEFT_FRONT_ANKLE;
	}

	else if(lr == RIGHTPART){
		soToe		 = SO_RIGHT_FRONT_TOE;
		soCannonBone = SO_RIGHT_FRONT_CANNON_BONE;
		joAnkle		 = JO_RIGHT_FRONT_ANKLE;
	}
	//　[p]frontCannonBone - [c]frontToe
	solidDesc.mass = totalMass * VSolid(soToe) / VSolids();
	solids[soToe] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soToe]->SetName("soLeftFrontToe");
	else
		solids[soToe]->SetName("soRightFrontToe");

	boxDesc.boxsize = Vec3f(frontToeBreadth, frontToeHeight, frontToeThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soToe]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	ballDesc.poseSocket.Pos() = Vec3f(0.0, frontCannonBoneHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.posePlug.Pos()   = Vec3f(0.0, -frontToeHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.spring			  = springFrontAnkle;
	ballDesc.damper			  = damperFrontAnkle;
	ballDesc.targetPosition			  = goalFrontAnkle;
	
	if(flagRange){
	ballDesc.limitSwing		  = limitSwingFrontAnkle;
	ballDesc.limitTwist		  = limitTwistFrontAnkle;
	}
	if(flagFMax){
		ballDesc.fMax = (lr ==LEFTPART) ? fMaxLeftFrontAnkle * phScene->GetTimeStep() : fMaxRightFrontAnkle * phScene->GetTimeStep();
	}

	joints[joAnkle] = phScene->CreateJoint(solids[soCannonBone], solids[soToe], ballDesc);
	if(lr == LEFTPART)
		joints[joAnkle]->SetName("joLeftFrontAnkle");
	else
		joints[joAnkle]->SetName("joRightFrontAnkle");

	phScene->SetContactMode(solids[soCannonBone], solids[soToe], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRFourLegsAnimalBodyGen::InitRearLegs(){
	
	if (!noLegs) {
		CreateFemur(LEFTPART);
		CreateTibia(LEFTPART);
		CreateRearCannonBone(LEFTPART);
		CreateRearToeBones(LEFTPART);
	}
	
	if (!noLegs) {
		CreateFemur(RIGHTPART);
		CreateTibia(RIGHTPART);
		CreateRearCannonBone(RIGHTPART);
		CreateRearToeBones(RIGHTPART);
	}
	
	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	phScene->SetContactMode(solids[SO_LEFT_FEMUR], solids[SO_RIGHT_FEMUR], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBodyGen::CreateFemur(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHBallJointDesc  ballDesc;
	int soFemur;
	int joHip;
	if(lr == LEFTPART){
		soFemur	 = SO_LEFT_FEMUR;
		joHip    = JO_LEFT_HIP;
	}

	else if(lr == RIGHTPART){
		soFemur	 = SO_RIGHT_FEMUR;
		joHip    = JO_RIGHT_HIP;	
	}
	
	// [p]waist - [c]femur
	solidDesc.mass = totalMass * VSolid(soFemur) / VSolids();
	solids[soFemur] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soFemur]->SetName("soLeftFemur");
	else
		solids[soFemur]->SetName("soRighFemur");
    
	boxDesc.boxsize = Vec3f(femurBreadth, femurHeight, femurThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soFemur]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soFemur]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	ballDesc.poseSocket.Pos() = Vec3f(lr*waistBreadth/2.0, -waistHeight/2.0, waistThickness/3.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.posePlug.Pos()	  = Vec3f(-lr*femurBreadth/2.0, -femurHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.spring			  = springHip;
	ballDesc.damper			  = damperHip;
	ballDesc.targetPosition			  = goalHip;

	if(flagRange){
	ballDesc.limitSwing		  = limitSwingHip;
	ballDesc.limitTwist		  = limitTwistHip;
	}
	if(flagFMax){
		ballDesc.fMax = (lr == LEFTPART) ? fMaxLeftHip * phScene->GetTimeStep() : fMaxRightHip * phScene->GetTimeStep();
	}

	joints[joHip] = phScene->CreateJoint(solids[SO_WAIST], solids[soFemur], ballDesc);
	if(lr == LEFTPART)
		joints[joHip]->SetName("joLeftHip");
	else
		joints[joHip]->SetName("joRightHip");

	phScene->SetContactMode(solids[SO_WAIST], solids[soFemur], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBodyGen::CreateTibia(LREnum lr){
	
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;
	int soTibia, soFemur;
	int joStifle;

	if(lr == LEFTPART){
		soTibia	 = SO_LEFT_TIBIA;
		soFemur  = SO_LEFT_FEMUR;
		joStifle = JO_LEFT_STIFLE; 
	}

	else if(lr == RIGHTPART){
		soTibia	 = SO_RIGHT_TIBIA;
		soFemur  = SO_RIGHT_FEMUR;
		joStifle = JO_RIGHT_STIFLE;
	}

	solidDesc.mass = totalMass * VSolid(soTibia) / VSolids();
	solids[soTibia] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soTibia]->SetName("soLeftTibia");
	else
		solids[soTibia]->SetName("soRightTibia");

	boxDesc.boxsize = Vec3f(tibiaBreadth, tibiaHeight, tibiaThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soTibia]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soTibia]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	//[p]femur - [c]tibia
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, femurHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -tibiaHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springStifle;
	hingeDesc.damper		   = damperStifle;
	hingeDesc.targetPosition		   = originStifle;
	
	if(flagRange){
	hingeDesc.lower			   = rangeStifle[0];
	hingeDesc.upper			   = rangeStifle[1];
	}
	if(flagFMax){
		hingeDesc.fMax = (lr ==LEFTPART) ? fMaxLeftStifle * phScene->GetTimeStep() : fMaxRightStifle * phScene->GetTimeStep();
	}

	joints[joStifle] = phScene->CreateJoint(solids[soFemur], solids[soTibia], hingeDesc);
	if(lr == LEFTPART)
		joints[joStifle]->SetName("joLeftStifle");
	else
		joints[joStifle]->SetName("joRightStifle");

	phScene->SetContactMode(solids[soFemur], solids[soTibia], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBodyGen::CreateRearCannonBone(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soCannonBone, soTibia;
	int joKnee;

	if(lr == LEFTPART){
		soCannonBone = SO_LEFT_REAR_CANNON_BONE;
		soTibia		 = SO_LEFT_TIBIA;
		joKnee		 = JO_LEFT_REAR_KNEE;
	}

	else if(lr == RIGHTPART){
		soCannonBone = SO_RIGHT_REAR_CANNON_BONE;
		soTibia		 = SO_RIGHT_TIBIA;
		joKnee		 = JO_RIGHT_REAR_KNEE;
	}

	solidDesc.mass = totalMass * VSolid(soCannonBone) / VSolids();
	solids[soCannonBone] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soCannonBone]->SetName("soLeftRearCannonBone");
	else
		solids[soCannonBone]->SetName("soRightRearCannonBone");
    
	boxDesc.boxsize = Vec3f(rearCannonBoneBreadth, rearCannonBoneHeight, rearCannonBoneThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soCannonBone]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soCannonBone]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));
	
	// [p]Tibia - [c]CannonBone
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, tibiaHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -rearCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springRearKnee;
	hingeDesc.damper		   = damperRearKnee;
	hingeDesc.targetPosition		   = originRearKnee;
	
	if(flagRange){
	hingeDesc.lower			   = rangeRearKnee[0];
	hingeDesc.upper			   = rangeRearKnee[1];
	}
	if(flagFMax){
		hingeDesc.fMax = (lr ==LEFTPART) ? fMaxLeftRearKnee * phScene->GetTimeStep() : fMaxRightRearKnee * phScene->GetTimeStep();
	}

	joints[joKnee] = phScene->CreateJoint(solids[soTibia], solids[soCannonBone], hingeDesc);
	if(lr == LEFTPART)
		joints[joKnee]->SetName("joLeftRearKnee");
	else
		joints[joKnee]->SetName("joRightRearKnee");

	phScene->SetContactMode(solids[soTibia], solids[soCannonBone], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBodyGen::CreateRearToeBones(LREnum lr){
	
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;
	int soToe, soCannonBone;
	int joAnkle;

	if(lr == LEFTPART){
		soToe		 = SO_LEFT_REAR_TOE;
		soCannonBone = SO_LEFT_REAR_CANNON_BONE;
		joAnkle		 = JO_LEFT_REAR_ANKLE;
	}

	else if(lr == RIGHTPART){
		soToe		 = SO_RIGHT_REAR_TOE;
		soCannonBone = SO_RIGHT_REAR_CANNON_BONE;
		joAnkle		 = JO_RIGHT_REAR_ANKLE;
	}

	//　[p]rearCannonBone - [c]rearToe
	solidDesc.mass = totalMass * VSolid(soToe) / VSolids();
	solids[soToe] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soToe]->SetName("soLeftRearToe");
	else
		solids[soToe]->SetName("soRightRearToe");

	boxDesc.boxsize = Vec3f(rearToeBreadth, rearToeHeight, rearToeThickness);
	boxDesc.material.mu		= materialMu;
	boxDesc.material.mu0	= materialMu;
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));
	solids[soToe]->SetInertia(CalcBoxInertia(boxDesc.boxsize, solidDesc.mass));

	ballDesc.poseSocket.Pos()  = Vec3f(0.0, rearCannonBoneHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, rearToeHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring		       = springRearAnkle;
	ballDesc.damper			   = damperRearAnkle;
	ballDesc.targetPosition			   = goalRearAnkle;
	
	if(flagRange){
	ballDesc.limitSwing		   = limitSwingRearAnkle;
	ballDesc.limitTwist		   = limitTwistRearAnkle;
	}
	if(flagFMax){
		ballDesc.fMax = (lr ==LEFTPART) ? fMaxLeftRearAnkle * phScene->GetTimeStep() : fMaxRightRearAnkle * phScene->GetTimeStep();
	}

	joints[joAnkle] = phScene->CreateJoint(solids[soCannonBone], solids[soToe], ballDesc);
	if(lr == LEFTPART)
		joints[joAnkle]->SetName("joLeftRearAnkle");
	else
		joints[joAnkle]->SetName("joRightRearAnkle");

	phScene->SetContactMode(solids[soCannonBone], solids[soToe], PHSceneDesc::MODE_NONE);

}
// --- --- ---
void CRFourLegsAnimalBodyGen::InitEyes(){
//	CreateEye(LEFTPART);
//	CreateEye(RIGHTPART);
}

void CRFourLegsAnimalBodyGen::CreateEye(LREnum lr){
	
	if(lr == LEFTPART){
	
	}

	else if(lr == RIGHTPART){
	
	}

}

// --- --- ---
void CRFourLegsAnimalBodyGen::InitContact(){
	
	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	for (unsigned int i=0; i<solids.size(); ++i) {
		for (unsigned int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i], solids[j], PHSceneDesc::MODE_NONE);
			}
		}
	}

	/*
	// 自分以外にすでにBodyが居ればそのBodyに属する剛体とのContactも切る
	for (int i=0; i<creature->NBodies(); ++i) {
		CRBodyIf* body = creature->GetBody(i);
		if (DCAST(CRFourLegsAnimalBodyGenIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
	*/

}
/*
void CRFourLegsAnimalBodyGen::InitControlMode(PHJointDesc::PHControlMode m){
	int njoints = joints.size();
	for(int i = 0; i < njoints; i++){
		if(joints[i])joints[i]->SetMode(m);
	}
}
*/
void CRFourLegsAnimalBodyGen::SetUpperBody(){
	upperBody.push_back(solids[CRFourLegsAnimalBodyGenDesc::SO_WAIST]);
	upperBody.push_back(solids[CRFourLegsAnimalBodyGenDesc::SO_CHEST]);
	upperBody.push_back(solids[CRFourLegsAnimalBodyGenDesc::SO_NECK]);
	upperBody.push_back(solids[CRFourLegsAnimalBodyGenDesc::SO_HEAD]);
}

Vec3d CRFourLegsAnimalBodyGen::GetUpperCenterOfMass(){
	
	SetUpperBody();

	/// ボディ上体の重量小計
	double totalWeightOfUpperPart = 0;
	/// ボディ上体の中心座標
	Vec3d  centerPosOfUpperBlocks = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0; i<(int)upperBody.size(); i++){
		if(upperBody[i]){
			centerPosOfUpperBlocks = centerPosOfUpperBlocks + upperBody[i]->GetCenterPosition() * upperBody[i]->GetMass();
			totalWeightOfUpperPart = totalWeightOfUpperPart + upperBody[i]->GetMass(); 
		}
	}

	return centerPosOfUpperBlocks / totalWeightOfUpperPart;
}
int CRFourLegsAnimalBodyGen::NBallJoints(){
	int counterNBallJoint = 0;
	int njoints = joints.size();
	for(int i = 0; i < njoints; i++){
		if(DCAST(PHBallJointIf, joints[i]))
			counterNBallJoint ++;
	}
	joNBallJoints = counterNBallJoint;
	return joNBallJoints;
}	

int CRFourLegsAnimalBodyGen::NHingeJoints(){
	int counterNHingeJoint = 0;
	int njoints = joints.size();
	for(int i = 0; i < njoints; i++){
		if(DCAST(PHHingeJointIf, joints[i]))
			 counterNHingeJoint ++;
	}
	joNHingeJoints = counterNHingeJoint;
	return joNHingeJoints;
}

double CRFourLegsAnimalBodyGen::VSolid(int i){
	if(i == SO_WAIST)						 return waistBreadth		   * waistHeight		   * waistThickness;
	else if(i == SO_CHEST)					 return chestBreadth		   * chestHeight		   * chestThickness;
	else if(i == SO_TAIL1)					 return tailBreadth			   * tailHeight			   * tailThickness;
	else if(i == SO_TAIL2)					 return tailBreadth			   * tailHeight			   * tailThickness;
	else if(i == SO_TAIL3)					 return tailBreadth			   * tailHeight			   * tailThickness;
	else if(i == SO_NECK)					 return neckBreadth			   * neckHeight			   * neckThickness;
	else if(i == SO_HEAD)					 return headBreadth			   * headHeight			   * headThickness;
	else if(i == SO_LEFT_BREASTBONE)		 return breastboneBreadth	   * breastboneHeight	   * breastboneThickness;
	else if(i == SO_LEFT_RADIUS)			 return radiusBreadth		   * radiusHeight		   * radiusThickness;
	else if(i == SO_LEFT_FRONT_CANNON_BONE)  return frontCannonBoneBreadth * frontCannonBoneHeight * frontCannonBoneThickness;
	else if(i == SO_LEFT_FRONT_TOE)			 return frontToeBreadth		   * frontToeHeight		   * frontToeThickness;
	else if(i == SO_RIGHT_BREASTBONE)		 return breastboneBreadth	   * breastboneHeight	   * breastboneThickness;
	else if(i == SO_RIGHT_RADIUS)			 return radiusBreadth		   * radiusHeight		   * radiusThickness;
	else if(i == SO_RIGHT_FRONT_CANNON_BONE) return frontCannonBoneBreadth * frontCannonBoneHeight * frontCannonBoneThickness;
	else if(i == SO_RIGHT_FRONT_TOE)		 return frontToeBreadth		   * frontToeHeight		   * frontToeThickness;
	else if(i == SO_LEFT_FEMUR)				 return femurBreadth		   * femurHeight		   * femurThickness;
	else if(i == SO_LEFT_TIBIA)				 return tibiaBreadth		   * tibiaHeight		   * tibiaThickness;
	else if(i == SO_LEFT_REAR_CANNON_BONE)	 return rearCannonBoneBreadth  * rearCannonBoneHeight  * rearCannonBoneThickness;
	else if(i == SO_LEFT_REAR_TOE)			 return rearToeBreadth		   * rearToeHeight		   * rearToeThickness;
	else if(i == SO_RIGHT_FEMUR)			 return femurBreadth		   * femurHeight		   * femurThickness;
	else if(i == SO_RIGHT_TIBIA)			 return tibiaBreadth		   * tibiaHeight		   * tibiaThickness;
	else if(i == SO_RIGHT_REAR_CANNON_BONE)	 return rearCannonBoneBreadth  * rearCannonBoneHeight  * rearCannonBoneThickness;
	else if(i == SO_RIGHT_REAR_TOE)			 return rearToeBreadth		   * rearToeHeight		   * rearToeThickness;
	else {
		DSTR << "【error】: Invalid value i." << std::endl;
		std::cout << "【error】: Invalid value i." << std::endl;
		return -1;
	}
}

double CRFourLegsAnimalBodyGen::VSolids(){
	double vSum = 0;
	for(int i = 0; i < NSolids(); i++){
		if (VSolid(i) != -1) vSum += VSolid(i);
		else {
			DSTR << "【error】: Invalid value i = " << i << std::endl;
			std::cout << "【error】: Invalid value i = " << i << std::endl;
			return -1;
		}
	}
	return vSum;
}

double CRFourLegsAnimalBodyGen::GetTotalMass(){
	return totalMass;
}

void CRFourLegsAnimalBodyGen::SetTotalMass(double value){
	totalMass = value;
	for(int i = 0; i < NSolids(); i++){
		solids[i]->SetMass(totalMass * VSolid(i) / VSolids());	
	}		
}

double CRFourLegsAnimalBodyGen::GetLegLength(int i){
	
	if((i == LEG_RIGHT_FRONT)||(i == LEG_LEFT_FRONT))    return ( breastboneHeight
																+ radiusHeight
																+ frontCannonBoneHeight);
	else if((i == LEG_RIGHT_REAR)||(i == LEG_LEFT_REAR)) return ( femurHeight 
																+ tibiaHeight
																+ rearCannonBoneHeight);
	else{
		std::cout	<< "CRFourLegsAnimalBodyGen::GetLegLength(int i)"	<< std::endl;
		std::cout	<< "Unexpected param i : " << i					<< std::endl;
		DSTR		<< "CRFourLegsAnimalBodyGen::GetLegLength(int i)"	<< std::endl;
		DSTR		<< "Unexpected param i : " << i					<< std::endl;
		return -1;
	}
}

}//end of the namespace Spr;


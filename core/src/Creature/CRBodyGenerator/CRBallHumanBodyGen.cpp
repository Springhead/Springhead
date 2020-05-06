/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyGenerator/CRBallHumanBodyGen.h>
#include <Collision/SprCDShape.h>
#include <Physics/SprPHSdk.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSolid.h>
#include <Creature/SprCRBody.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRBallHumanBodyGenDesc::CRBallHumanBodyGenDesc(){
	bodyMass = 1.0f;  /// デフォルトがこれなのはどうも変だが(mitake)

	// 高さに関するデータ
	double hBody			= 1.7219; // 最大身長
	double hCervicale		= 1.4564; // 頸椎高
	double hSubsternale		= 1.2253; // 胸骨下縁高
	double hWaist			= 1.0281; // 胴囲高
	double hIliocristal		= 1.0142; // 腸骨稜高
	double hOmphalion		= 1.0036; // 臍高
	double hTrochanterion	= 0.8757; // 転子高
	double hTibiale			= 0.4420; // 脛骨上縁高
	double hCalf			= 0.3150; // 下腿最大囲高

	// 幅に関するデータ
	double wBideltoid		= 0.4562; // 肩幅
	double wChest			= 0.2887; // 胸部横径
	double wWaist			= 0.2611; // 胴部横径
	double wBicristal		= 0.2722; // 腸骨稜幅
	double wBitrochanteric	= 0.3067; // 大転子間幅

	waistBreadth    = wBicristal;
	waistLength     = hOmphalion - (hIliocristal - (waistBreadth/2));
	abdomenBreadth  = wWaist;
	abdomenLength   = hSubsternale - hOmphalion;
	chestBreadth    = wChest;
	chestLength     = hCervicale - hSubsternale - (chestBreadth/2);

	bideltoidBreadth = wBideltoid;

	headBreadth = 0.1619;
	headHeight  = 0.2387;
	
	neckLength   = 1.5796 - 1.4564; // 耳の高さ - 頸椎高
	neckDiameter = 0.3563 / 3.1415;

	double occiputToNeckFront  = 0.1301;
	neckPosZ     = headBreadth/2 - (occiputToNeckFront - neckDiameter/2);
	neckPosY     = -neckDiameter/2;

	bigonialBreadth = 0.1117;

	upperArmLength   = 0.3406;
	upperArmDiameter = 0.2808 / 3.1415;

	lowerArmLength   = 0.2766;
	lowerArmDiameter = 0.1677 / 3.1415;

	handLength    = 0.1908;
	handBreadth   = 0.1018;
	handThickness = 0.0275;

	upperLegLength   = 0.4092;
	upperLegDiameter = 0.5289 / 3.1415;
	interLegDistance = wBitrochanteric - upperLegDiameter;
	legPosY = -(hIliocristal - hTrochanterion - waistBreadth/2);

	lowerLegLength   = 0.3946;
	lowerLegDiameter = 0.1109;
	kneeDiameter     = 0.1076;
	ankleDiameter    = 0.2194 / 3.1415;
	calfDiameter     = 0.3623 / 3.1415;
	calfPosY         = hTibiale - hCalf;
	calfPosZ         = (calfDiameter/2 - kneeDiameter/2)*0.9;

	footLength       = 0.2544;
	footBreadth      = 0.0994;
	footThickness    = 0.0619;
	ankleToeDistance = 0.2010;

	vertexToEyeHeight     = 0.1255;
	occiputToEyeDistance  = 0.1688;
	eyeDiameter           = 0.0240;
	interpupillaryBreadth = 0.0619;

	springWaistAbdomen = -1;        damperWaistAbdomen = -1;
	springAbdomenChest = -1;        damperAbdomenChest = -1;
	springChestNeck    = -1;        damperChestNeck    = -1;
	springNeckHead     = -1;        damperNeckHead     = -1;
	springShoulder     = -1;        damperShoulder     = -1;
	springElbow        = -1;        damperElbow        = -1;
	springWrist        = -1;        damperWrist        = -1;
	springWaistLeg     = -1;        damperWaistLeg     = -1;
	springKnee         = -1;        damperKnee         = -1;
	springAnkle        = -1;        damperAnkle        = -1;
	springEye          = 100.0;  damperEye             =  5000.0;

	shapeType  = CRBallHumanBodyGenDesc::HST_ROUNDCONE;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRBallHumanBodyGen


// --- --- ---
CRBodyIf* CRBallHumanBodyGen::Generate(CRCreatureIf* crCreature){
	CRBodyGen::Generate(crCreature);

	GenerateBody();
	GenerateHead();
	GenerateArms();
	GenerateEyes();
	GenerateLegs();

	InitSolidPose();
	InitContact();
	InitIK();

	return crBody;
}

// --- --- ---
void CRBallHumanBodyGen::GenerateBody(){
	CreateWaist();
	CreateAbdomen();
	CreateChest();
}

void CRBallHumanBodyGen::CreateWaist(){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	Posed              pose1, pose2, pose3;

	// Solid
	solidDesc.mass     = 0.17 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_WAIST]   = CreateSolid(solidDesc);
	solids[SO_WAIST]->SetLabel("waist");
	CreateIKEndEffector(SO_WAIST);

	// Shape
	int sn=0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc    rcDesc;

		// 腰～腹
		rcDesc.radius[0]   = waistBreadth/2;
		rcDesc.radius[1]   = abdomenBreadth/2;
		rcDesc.length      = waistLength;
		solids[SO_WAIST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[SO_WAIST]->GetPHSolid()->SetShapePose(sn,pose2*pose1);
		sn++;

		// 腰～足
		double cX = interLegDistance/2;
		double cY = -legPosY;
		double theta = atan(cX/cY);
		rcDesc.radius[0]   = upperLegDiameter/2;
		rcDesc.radius[1]   = waistBreadth/2;
		rcDesc.length      = sqrt(cX*cX + cY*cY);
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,-rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		/// -- 左足
		pose3=Posed(); pose3.Ori() = Quaterniond::Rot(-theta,'z');
		solids[SO_WAIST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		solids[SO_WAIST]->GetPHSolid()->SetShapePose(sn,pose3*pose2*pose1);
		sn++;
		/// -- 右足
		pose3=Posed(); pose3.Ori() = Quaterniond::Rot(theta,'z');
		solids[SO_WAIST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		solids[SO_WAIST]->GetPHSolid()->SetShapePose(sn,pose3*pose2*pose1);
		sn++;
	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc        boxDesc;

		boxDesc.boxsize  = Vec3f(waistBreadth, waistLength+waistBreadth, waistBreadth);
		solids[SO_WAIST]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	solids[SO_WAIST]->GetPHSolid()->SetFramePosition(Vec3f(0,0,0));
}

void CRBallHumanBodyGen::CreateAbdomen(){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass     = 0.028 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_ABDOMEN] = CreateSolid(solidDesc);
	solids[SO_ABDOMEN]->SetLabel("abdomen");
	CreateIKEndEffector(SO_ABDOMEN);

	// Shape
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc    rcDesc;
		rcDesc.radius[0]   = abdomenBreadth/2;
		rcDesc.radius[1]   = abdomenBreadth/2;
		rcDesc.length      = abdomenLength;
		solids[SO_ABDOMEN]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[SO_ABDOMEN]->GetPHSolid()->SetShapePose(0,pose);

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc          boxDesc;
		boxDesc.boxsize    = Vec3f(abdomenBreadth, abdomenLength+abdomenBreadth, abdomenBreadth);
		solids[SO_ABDOMEN]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		
	}

	// Joint -- [p]Waist-[c]Abdomen
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0,  waistLength, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -abdomenLength / 2.0, 0);
	// ballDesc.limitDir         = Vec3d(0,0,1);
	// ballDesc.limitTwist       = Vec2d(Rad(-45), Rad(45));
	// ballDesc.limitSwing       = Vec2d(Rad(0), Rad(45));
	SetJointSpringDamper(ballDesc, springWaistAbdomen, damperWaistAbdomen, solids[SO_WAIST]->GetPHSolid()->GetMass());
	joints[JO_WAIST_ABDOMEN] = CreateJoint(solids[SO_ABDOMEN], solids[SO_WAIST], ballDesc);
	CreateIKActuator(JO_WAIST_ABDOMEN);

	phScene->SetContactMode(solids[SO_ABDOMEN]->GetPHSolid(), solids[SO_WAIST]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

void CRBallHumanBodyGen::CreateChest(){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;
	Posed              pose1,pose2,pose3;

	// Solid
	solidDesc.mass   = 0.252 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_CHEST] = CreateSolid(solidDesc);
	solids[SO_CHEST]->SetLabel("chest");
	CreateIKEndEffector(SO_CHEST);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc    rcDesc;

		// 中心～首付根
		rcDesc.radius[0] = chestBreadth/2;
		rcDesc.radius[1] = neckDiameter/2;
		rcDesc.length    = chestBreadth/2;
		solids[SO_CHEST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[SO_CHEST]->GetPHSolid()->SetShapePose(sn,pose2*pose1);
		sn++;

		// 中心～腹
		rcDesc.radius[0] = abdomenBreadth/2;
		rcDesc.radius[1] = chestBreadth/2;
		rcDesc.length    = chestLength;
		solids[SO_CHEST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,-rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[SO_CHEST]->GetPHSolid()->SetShapePose(sn,pose2*pose1);
		sn++;

		// 中心～肩
		double sX = bideltoidBreadth/2 - upperArmDiameter/2;
		double sY = chestBreadth/2 - upperArmDiameter/2;
		double theta = atan(sX/sY);
		rcDesc.radius[0] = chestBreadth/2;
		rcDesc.radius[1] = upperArmDiameter/2;
		rcDesc.length    = sqrt(sX*sX + sY*sY);
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		/// -- 左肩
		pose3=Posed(); pose3.Ori() = Quaterniond::Rot(theta,'z');
		solids[SO_CHEST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		solids[SO_CHEST]->GetPHSolid()->SetShapePose(sn,pose3*pose2*pose1);
		sn++;
		// -- 右肩
		pose3=Posed(); pose3.Ori() = Quaterniond::Rot(-theta,'z');
		solids[SO_CHEST]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		solids[SO_CHEST]->GetPHSolid()->SetShapePose(sn,pose3*pose2*pose1);
		sn++;
	
	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc          boxDesc;

		boxDesc.boxsize  = Vec3f(chestBreadth, chestLength+neckDiameter/2, chestBreadth);
		solids[SO_CHEST]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	// Joint -- [p]Abdomen-[c]Chest
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0,  abdomenLength/2, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -chestLength, 0);
	//ballDesc.limitDir         = Vec3d(0,0,1);
	//ballDesc.limitTwist       = Vec2d(Rad(-45), Rad(45));
	//ballDesc.limitSwing       = Vec2d(Rad(0), Rad(45));
	SetJointSpringDamper(ballDesc, springAbdomenChest, damperAbdomenChest, solids[SO_ABDOMEN]->GetPHSolid()->GetMass());
	joints[JO_ABDOMEN_CHEST]  = CreateJoint(solids[SO_CHEST], solids[SO_ABDOMEN], ballDesc);
	CreateIKActuator(JO_ABDOMEN_CHEST);

	phScene->SetContactMode(solids[SO_CHEST]->GetPHSolid(), solids[SO_ABDOMEN]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBodyGen::GenerateHead(){
	CreateNeck();
	CreateHead();
}

void CRBallHumanBodyGen::CreateNeck(){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass  = 0.028 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_NECK] = CreateSolid(solidDesc);
	solids[SO_NECK]->SetLabel("neck");
	CreateIKEndEffector(SO_NECK);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDCapsuleDesc      capsuleDesc;

		capsuleDesc.radius = neckDiameter/2;
		capsuleDesc.length = neckLength;
		solids[SO_NECK]->GetPHSolid()->AddShape(phSdk->CreateShape(capsuleDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
		solids[SO_NECK]->GetPHSolid()->SetShapePose(0,pose);
		sn++;

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc boxDesc;
		boxDesc.boxsize = Vec3d(neckDiameter, neckLength+neckDiameter, neckDiameter);
		solids[SO_NECK]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	// Joint -- [p]Chest-[c]Neck
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0, chestBreadth / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
	//ballDesc.limitDir         = Vec3d(0,0,1);
	//ballDesc.limitTwist       = Vec2d(Rad(-45), Rad(45));
	//ballDesc.limitSwing       = Vec2d(Rad(0), Rad(30));
	SetJointSpringDamper(ballDesc, springChestNeck, damperChestNeck, solids[SO_CHEST]->GetPHSolid()->GetMass());
	joints[JO_CHEST_NECK] = CreateJoint(solids[SO_NECK], solids[SO_CHEST], ballDesc);
	CreateIKActuator(JO_CHEST_NECK);

	phScene->SetContactMode(solids[SO_NECK]->GetPHSolid(), solids[SO_CHEST]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

void CRBallHumanBodyGen::CreateHead(){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc       solidDesc;
	PHBallJointDesc   ballDesc;

	// Solid
	solidDesc.mass    = 0.07 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_HEAD]   = CreateSolid(solidDesc);
	solids[SO_HEAD]->SetLabel("head");
	CreateIKEndEffector(SO_HEAD);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc   rcDesc;

		rcDesc.radius[0] = bigonialBreadth / 2.0;
		rcDesc.radius[1] = headBreadth / 2.0;
		double Z = headBreadth/2 - bigonialBreadth/2 ;
		double Y = headHeight - headBreadth/2 - bigonialBreadth/2;
		double theta = atan(Y/Z);
		rcDesc.length = sqrt(Z*Z + Y*Y);
		solids[SO_HEAD]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose1=Posed(); pose1.Pos() = Vec3d(0,0,-rcDesc.length/2);
		Posed pose2=Posed(); pose2.Ori() = Quaterniond::Rot(-theta,'x');
		solids[SO_HEAD]->GetPHSolid()->SetShapePose(0,pose2*pose1);
		sn++;

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc boxDesc;

		boxDesc.boxsize = Vec3d(headBreadth, headHeight, headBreadth);
		solids[SO_HEAD]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	} 

	// Joint -- [p]Neck-[c]Head
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0, neckLength / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, neckPosY, neckPosZ);
	//ballDesc.limitDir         = Vec3d(0,0,1);
	//ballDesc.limitTwist       = Vec2d(Rad(-45), Rad(45));
	//ballDesc.limitSwing       = Vec2d(Rad(0), Rad(20));
	SetJointSpringDamper(ballDesc, springNeckHead, damperNeckHead, solids[SO_NECK]->GetPHSolid()->GetMass());
	joints[JO_NECK_HEAD] = CreateJoint(solids[SO_HEAD], solids[SO_NECK], ballDesc);
	CreateIKActuator(JO_NECK_HEAD);

	phScene->SetContactMode(solids[SO_HEAD]->GetPHSolid(), solids[SO_NECK]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBodyGen::GenerateArms(){
	CreateUpperArm(LEFTPART);
	CreateLowerArm(LEFTPART);
	CreateHand(LEFTPART);

	CreateUpperArm(RIGHTPART);
	CreateLowerArm(RIGHTPART);
	CreateHand(RIGHTPART);
}

void CRBallHumanBodyGen::CreateUpperArm(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyGenDesc::CRHumanSolids soNUpperArm;
	CRBallHumanBodyGenDesc::CRHumanJoints joNShoulder;
	if (lr==LEFTPART) {
		soNUpperArm   = SO_LEFT_UPPER_ARM;
		joNShoulder   = JO_LEFT_SHOULDER;
	} else {
		soNUpperArm   = SO_RIGHT_UPPER_ARM;
		joNShoulder   = JO_RIGHT_SHOULDER;
	}

	// Solid
	solidDesc.mass      = 0.04 * bodyMass;
	solidDesc.inertia   = Matrix3d::Unit() * solidDesc.mass;
	solids[soNUpperArm] = CreateSolid(solidDesc);
	solids[soNUpperArm]->SetLabel((lr==LEFTPART) ? "left_upper_arm" : "right_upper_arm");
	CreateIKEndEffector(soNUpperArm);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc    rcDesc;

		rcDesc.radius[0]    = lowerArmDiameter / 2.0;
		rcDesc.radius[1]    = upperArmDiameter / 2.0;
		rcDesc.length       = upperArmLength;
		solids[soNUpperArm]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-lr*90),'y');
		solids[soNUpperArm]->GetPHSolid()->SetShapePose(0,pose);
		sn++;

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc boxDesc;

		boxDesc.boxsize = Vec3d(upperArmLength, upperArmDiameter, upperArmDiameter);
		solids[soNUpperArm]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	// Joint -- Shoulder ([p]Chest-[c]UpperArm)
	ballDesc                    = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(lr*(bideltoidBreadth/2.0 - upperArmDiameter/2.0), chestBreadth/2.0-upperArmDiameter/2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(-lr*upperArmLength/2.0, 0, 0);
	// ballDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90),'y');
	// ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90),'y');
	SetJointSpringDamper(ballDesc, springShoulder, damperShoulder, solids[SO_CHEST]->GetPHSolid()->GetMass());
	joints[joNShoulder]       = CreateJoint(solids[soNUpperArm], solids[SO_CHEST], ballDesc);
	solids[soNUpperArm]->AddChildObject(joints[joNShoulder]->GetPHJoint());
	CreateIKActuator(joNShoulder);

	phScene->SetContactMode(solids[soNUpperArm]->GetPHSolid(), solids[SO_CHEST]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

void CRBallHumanBodyGen::CreateLowerArm(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRBallHumanBodyGenDesc::CRHumanSolids soNUpperArm, soNLowerArm;
	CRBallHumanBodyGenDesc::CRHumanJoints joNElbow;
	if (lr==LEFTPART) {
		soNUpperArm = SO_LEFT_UPPER_ARM; soNLowerArm = SO_LEFT_LOWER_ARM;
		joNElbow = JO_LEFT_ELBOW;
	} else {
		soNUpperArm = SO_RIGHT_UPPER_ARM; soNLowerArm = SO_RIGHT_LOWER_ARM;
		joNElbow = JO_RIGHT_ELBOW;
	}

	// Solid
	solidDesc.mass      = 0.02 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNLowerArm] = CreateSolid(solidDesc);
	solids[soNLowerArm]->SetLabel((lr==LEFTPART) ? "left_lower_arm" : "right_lower_arm");
	CreateIKEndEffector(soNLowerArm);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc    rcDesc;

		rcDesc.radius[0]    = lowerArmDiameter / 2.0;
		rcDesc.radius[1]    = lowerArmDiameter / 2.0;
		rcDesc.length       = lowerArmLength;
		solids[soNLowerArm]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));

		/*
		CDCapsuleDesc      capsuleDesc;

		capsuleDesc.radius  = lowerArmDiameter / 2.0;
		capsuleDesc.length  = lowerArmLength;
		solids[soNLowerArm]->GetPHSolid()->AddShape(phSdk->CreateShape(capsuleDesc));
		*/
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-lr*90),'y');
		solids[soNLowerArm]->GetPHSolid()->SetShapePose(0,pose);
		sn++;

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc boxDesc;

		boxDesc.boxsize = Vec3d(lowerArmLength, lowerArmDiameter, lowerArmDiameter);
		solids[soNLowerArm]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	// Joint -- Elbow ([p]UpperArm-[c]LowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(lr*upperArmLength/2.0, 0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(lr*90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(-lr*lowerArmLength/2.0, 0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(lr*90), 'x');
	SetJointSpringDamper(hingeDesc, springElbow, damperElbow, solids[soNUpperArm]->GetPHSolid()->GetMass());
	hingeDesc.targetPosition           = Rad(0);
	joints[joNElbow] = CreateJoint(solids[soNLowerArm], solids[soNUpperArm], hingeDesc);
	CreateIKActuator(joNElbow);

	phScene->SetContactMode(solids[soNLowerArm]->GetPHSolid(), solids[soNUpperArm]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

void CRBallHumanBodyGen::CreateHand(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyGenDesc::CRHumanSolids soNLowerArm, soNHand;
	CRBallHumanBodyGenDesc::CRHumanJoints joNWrist;
	if (lr==LEFTPART) {
		soNLowerArm = SO_LEFT_LOWER_ARM; soNHand = SO_LEFT_HAND;
		joNWrist = JO_LEFT_WRIST; 
	} else {
		soNLowerArm = SO_RIGHT_LOWER_ARM; soNHand = SO_RIGHT_HAND;
		joNWrist = JO_RIGHT_WRIST; 
	}

	// Solid
	solidDesc.mass     = 0.05 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNHand]    = CreateSolid(solidDesc);
	solids[soNHand]->SetLabel((lr==LEFTPART) ? "left_hand" : "right_hand");
	CreateIKEndEffector(soNHand);

	boxDesc.boxsize    = Vec3f(handLength, handThickness, handBreadth);
	solids[soNHand]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Wrist ([p]LowerArm-[c]Hand)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(lr*lowerArmLength/2.0, 0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(-lr*handLength/2.0, 0, 0);
	SetJointSpringDamper(ballDesc, springWrist, damperWrist, solids[soNLowerArm]->GetPHSolid()->GetMass());
	joints[joNWrist]          = CreateJoint(solids[soNHand], solids[soNLowerArm], ballDesc);
	CreateIKActuator(joNWrist);

	phScene->SetContactMode(solids[soNHand]->GetPHSolid(), solids[soNLowerArm]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBodyGen::GenerateEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRBallHumanBodyGen::CreateEye(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyGenDesc::CRHumanSolids soNEye;
	CRBallHumanBodyGenDesc::CRHumanJoints joNEye;
	if (lr==LEFTPART) {
		soNEye = SO_LEFT_EYE;
		joNEye = JO_LEFT_EYE;
	} else {
		soNEye = SO_RIGHT_EYE;
		joNEye = JO_RIGHT_EYE;
	}

	// Solid
	solidDesc.mass     = 0.001 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * 0.001;
	solids[soNEye]     = CreateSolid(solidDesc);
	solids[soNEye]->SetLabel((lr==LEFTPART) ? "left_eye" : "right_eye");
	CreateIKEndEffector(soNEye);

	sphereDesc.radius  = (float)(eyeDiameter/2.0);
	solids[soNEye]->GetPHSolid()->AddShape(phSdk->CreateShape(sphereDesc));
	sphereDesc.radius  = (float)(eyeDiameter/4.0);
	solids[soNEye]->GetPHSolid()->AddShape(phSdk->CreateShape(sphereDesc));
	Posed pose=Posed(); pose.Pos() = Vec3d(0,0,-eyeDiameter/3.0);
	solids[soNEye]->GetPHSolid()->SetShapePose(1,pose);

	// Joint -- Eye ([p]Head-[c]Eye)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(lr*interpupillaryBreadth/2.0, headHeight/2.0 - vertexToEyeHeight, -headBreadth/2.0);
	ballDesc.spring           = springEye;
	ballDesc.damper           = damperEye;
	joints[joNEye]            = CreateJoint(solids[soNEye], solids[SO_HEAD], ballDesc);

	phScene->SetContactMode(solids[soNEye]->GetPHSolid(), solids[SO_HEAD]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBodyGen::GenerateLegs(){
	CreateUpperLeg(LEFTPART);
	CreateLowerLeg(LEFTPART);
	CreateFoot(LEFTPART);

	CreateUpperLeg(RIGHTPART);
	CreateLowerLeg(RIGHTPART);
	CreateFoot(RIGHTPART);
}

void CRBallHumanBodyGen::CreateUpperLeg(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyGenDesc::CRHumanSolids soNUpperLeg;
	CRBallHumanBodyGenDesc::CRHumanJoints joNWaistLeg;
	if (lr==LEFTPART) {
		soNUpperLeg = SO_LEFT_UPPER_LEG;
		joNWaistLeg = JO_LEFT_WAIST_LEG;
	} else {
		soNUpperLeg = SO_RIGHT_UPPER_LEG;
		joNWaistLeg = JO_RIGHT_WAIST_LEG;
	}

	// Solid
	solidDesc.mass      = 0.06 * bodyMass;
	solidDesc.inertia   = Matrix3d::Unit() * solidDesc.mass;
	solids[soNUpperLeg] = CreateSolid(solidDesc);
	solids[soNUpperLeg]->SetLabel((lr==LEFTPART) ? "left_upper_leg" : "right_upper_leg");
	CreateIKEndEffector(soNUpperLeg);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc   rcDesc;

		rcDesc.radius[0]    = kneeDiameter / 2.0;
		rcDesc.radius[1]    = upperLegDiameter / 2.0;
		rcDesc.length       = upperLegLength;
		solids[soNUpperLeg]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[soNUpperLeg]->GetPHSolid()->SetShapePose(0,pose);

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc boxDesc;

		boxDesc.boxsize = Vec3d(upperLegDiameter, upperLegLength, upperLegDiameter);
		solids[soNUpperLeg]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	// Joint -- WaistLeg ([p]Waist-[c]UpperLeg)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(lr*interLegDistance/2.0, legPosY, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, upperLegLength / 2.0, 0.0);
	SetJointSpringDamper(ballDesc, springWaistLeg, damperWaistLeg, solids[SO_WAIST]->GetPHSolid()->GetMass());
	joints[joNWaistLeg]       = CreateJoint(solids[soNUpperLeg], solids[SO_WAIST], ballDesc);
	CreateIKActuator(joNWaistLeg);

	phScene->SetContactMode(solids[soNUpperLeg]->GetPHSolid(), solids[SO_WAIST]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

void CRBallHumanBodyGen::CreateLowerLeg(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;
	Posed              pose1,pose2,pose3;
	double             theta;

	CRBallHumanBodyGenDesc::CRHumanSolids soNUpperLeg, soNLowerLeg;
	CRBallHumanBodyGenDesc::CRHumanJoints joNKnee;
	if (lr==LEFTPART) {
		soNUpperLeg = SO_LEFT_UPPER_LEG; soNLowerLeg = SO_LEFT_LOWER_LEG;
		joNKnee = JO_LEFT_KNEE;
	} else {
		soNUpperLeg = SO_RIGHT_UPPER_LEG; soNLowerLeg = SO_RIGHT_LOWER_LEG;
		joNKnee = JO_RIGHT_KNEE;
	}

	// Solid
	solidDesc.mass      = 0.05 * bodyMass;
	solidDesc.inertia   = Matrix3d::Unit() * solidDesc.mass;
	solids[soNLowerLeg] = CreateSolid(solidDesc);
	solids[soNLowerLeg]->SetLabel((lr==LEFTPART) ? "left_lower_leg" : "right_lower_leg");
	CreateIKEndEffector(soNLowerLeg);

	// Shape
	int sn = 0;
	if (shapeType == CRBallHumanBodyGenDesc::HST_ROUNDCONE) {
		CDRoundConeDesc   rcDesc;

		rcDesc.radius[0]    = calfDiameter/2;
		rcDesc.radius[1]    = kneeDiameter/2;
		rcDesc.length       = sqrt(calfPosY*calfPosY + calfPosZ*calfPosZ);
		theta               = atan(calfPosZ / calfPosY);
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,-rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90)+theta,'x');
		pose3=Posed(); pose3.Pos() = Vec3d(0,lowerLegLength/2,0);
		solids[soNLowerLeg]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		solids[soNLowerLeg]->GetPHSolid()->SetShapePose(sn,pose3*pose2*pose1);
		sn++;

		double cY = lowerLegLength - calfPosY;
		rcDesc.radius[0]    = ankleDiameter/2;
		rcDesc.radius[1]    = calfDiameter/2;
		rcDesc.length       = sqrt(cY*cY + calfPosZ*calfPosZ);
		theta               = atan(calfPosZ / cY);
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90)-theta,'x');
		pose3=Posed(); pose3.Pos() = Vec3d(0,-lowerLegLength/2,0);
		solids[soNLowerLeg]->GetPHSolid()->AddShape(phSdk->CreateShape(rcDesc));
		solids[soNLowerLeg]->GetPHSolid()->SetShapePose(sn,pose3*pose2*pose1);
		sn++;

	} else if (shapeType == CRBallHumanBodyGenDesc::HST_BOX) {
		CDBoxDesc boxDesc;

		boxDesc.boxsize = Vec3d(calfDiameter, lowerLegLength, calfDiameter);
		solids[soNLowerLeg]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));
		sn++;

	}

	// Joint -- Knee ([p]UpperLeg-[c]LowerLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerLegLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springKnee, damperKnee, solids[soNUpperLeg]->GetPHSolid()->GetMass());
	joints[joNKnee] = CreateJoint(solids[soNLowerLeg], solids[soNUpperLeg], hingeDesc);
	CreateIKActuator(joNKnee);

	phScene->SetContactMode(solids[soNLowerLeg]->GetPHSolid(), solids[soNUpperLeg]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

void CRBallHumanBodyGen::CreateFoot(LREnum lr){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyGenDesc::CRHumanSolids soNLowerLeg, soNFoot;
	CRBallHumanBodyGenDesc::CRHumanJoints joNAnkle;
	if (lr==LEFTPART) {
		soNLowerLeg = SO_LEFT_LOWER_LEG; soNFoot = SO_LEFT_FOOT;
		joNAnkle = JO_LEFT_ANKLE;
	} else {
		soNLowerLeg = SO_RIGHT_LOWER_LEG; soNFoot = SO_RIGHT_FOOT;
		joNAnkle = JO_RIGHT_ANKLE;
	}

	// Solid
	solidDesc.mass   = 0.01 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNFoot]  = CreateSolid(solidDesc);
	solids[soNFoot]->SetLabel((lr==LEFTPART) ? "left_foot" : "right_foot");
	CreateIKEndEffector(soNFoot);

	boxDesc.boxsize  = Vec3f(footBreadth, footThickness, footLength);
	solids[soNFoot]->GetPHSolid()->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Ankle ([p]LowerLeg-[c]Foot)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0.0, -lowerLegLength / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, footThickness / 2.0, (ankleToeDistance - footLength/2.0));
	SetJointSpringDamper(ballDesc, springAnkle, damperAnkle, solids[soNLowerLeg]->GetPHSolid()->GetMass());
	joints[joNAnkle]          = CreateJoint(solids[soNFoot], solids[soNLowerLeg], ballDesc);
	CreateIKActuator(joNAnkle);

	phScene->SetContactMode(solids[soNFoot]->GetPHSolid(), solids[soNLowerLeg]->GetPHSolid(), PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBodyGen::InitContact(){
	PHSceneIf*	phScene	= crCreature->GetPHScene();
	PHSdkIf*	phSdk	= phScene->GetSdk();

	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	// 念のため今は残してあるが下のコードがあれば不要な気がする．(mitake)
	// phScene->SetContactMode(solids[SO_LEFT_UPPER_LEG], solids[SO_RIGHT_UPPER_LEG], PHSceneDesc::MODE_NONE);

	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	/**/
	for (unsigned int i=0; i<solids.size(); ++i) {
		for (unsigned int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i]->GetPHSolid(), solids[j]->GetPHSolid(), PHSceneDesc::MODE_NONE);
			}
		}
	}
	/**/

	/*
	phScene->SetContactMode(solids[SO_WAIST],   solids[SO_LEFT_UPPER_ARM], PHSceneDesc::MODE_LCP);
	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_LEFT_UPPER_ARM], PHSceneDesc::MODE_LCP);

	phScene->SetContactMode(solids[SO_WAIST],   solids[SO_LEFT_LOWER_ARM], PHSceneDesc::MODE_LCP);
	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_LEFT_LOWER_ARM], PHSceneDesc::MODE_LCP);
	phScene->SetContactMode(solids[SO_CHEST],   solids[SO_LEFT_LOWER_ARM], PHSceneDesc::MODE_LCP);

	phScene->SetContactMode(solids[SO_WAIST],   solids[SO_RIGHT_UPPER_ARM], PHSceneDesc::MODE_LCP);
	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_RIGHT_UPPER_ARM], PHSceneDesc::MODE_LCP);

	phScene->SetContactMode(solids[SO_WAIST],   solids[SO_RIGHT_LOWER_ARM], PHSceneDesc::MODE_LCP);
	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_RIGHT_LOWER_ARM], PHSceneDesc::MODE_LCP);
	phScene->SetContactMode(solids[SO_CHEST],   solids[SO_RIGHT_LOWER_ARM], PHSceneDesc::MODE_LCP);
	*/

	// 自分以外にすでにBodyが居ればそのBodyに属する剛体とのContactも切る
	/*/
	for (int i=0; i<creature->NBodies(); ++i) {
		CRBodyIf* body = creature->GetBody(i);
		if (DCAST(CRBallHumanBodyGenIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
	/**/
}

void CRBallHumanBodyGen::InitSolidPose(){
	solids[0]->GetPHSolid()->SetOrientation(Quaterniond(6.12323e-017, 0, 1, 0));
	solids[1]->GetPHSolid()->SetOrientation(Quaterniond(9.32919e-008, 7.83565e-006, 1, 1.05206e-005));
	solids[2]->GetPHSolid()->SetOrientation(Quaterniond(1.18068e-007, 8.70229e-006, 1, 1.45582e-005));
	solids[3]->GetPHSolid()->SetOrientation(Quaterniond(1.01858e-007, 6.72754e-006, 1, 2.09876e-005));
	solids[4]->GetPHSolid()->SetOrientation(Quaterniond(9.33107e-008, 6.72772e-006, 1, 2.46642e-005));
	solids[5]->GetPHSolid()->SetOrientation(Quaterniond::Rot(Rad(180),'y'));
	solids[6]->GetPHSolid()->SetOrientation(Quaterniond::Rot(Rad(180),'y'));
	solids[7]->GetPHSolid()->SetOrientation(Quaterniond::Rot(Rad(180),'y'));
	solids[8]->GetPHSolid()->SetOrientation(Quaterniond(6.56423e-010, 5.72576e-013, 1, 3.69463e-006));
	solids[9]->GetPHSolid()->SetOrientation(Quaterniond(-3.10898e-009, -7.08281e-009, 1, 7.32513e-006));
	solids[10]->GetPHSolid()->SetOrientation(Quaterniond(-3.11148e-009, -7.08281e-009, 1, 1.09413e-005));
	solids[11]->GetPHSolid()->SetOrientation(Quaterniond(-0.0194578, -0.00260339, 0.990938, -0.13288));
	solids[12]->GetPHSolid()->SetOrientation(Quaterniond::Rot(Rad(180),'y'));
	solids[13]->GetPHSolid()->SetOrientation(Quaterniond::Rot(Rad(180),'y'));
	solids[14]->GetPHSolid()->SetOrientation(Quaterniond::Rot(Rad(180),'y'));
	solids[15]->GetPHSolid()->SetOrientation(Quaterniond(6.56427e-010, -6.6942e-013, 1, 3.69463e-006));
	solids[16]->GetPHSolid()->SetOrientation(Quaterniond(-5.75832e-009, 9.62738e-010, 1, 7.32515e-006));
	solids[17]->GetPHSolid()->SetOrientation(Quaterniond(-5.76085e-009, 9.6274e-010, 1, 1.09413e-005));
	solids[18]->GetPHSolid()->SetOrientation(Quaterniond(0.0142138, 0.0019135, 0.991027, -0.132892));

	/*
	for (int i=0; i<SO_NSOLIDS; ++i) {
		DCAST(PHIKOriCtlIf,ikEndEffectors[2*SO_NSOLIDS+i])->SetTargetPosition(solids[i]->GetPose().Ori());
	}
	*/

	solids[0]->GetPHSolid()->SetFramePosition(Vec3d(0, 0, 0)); 
	solids[1]->GetPHSolid()->SetFramePosition(Vec3d(1.17329e-006, 0.235371, 2.50466e-006)); 
	solids[2]->GetPHSolid()->SetFramePosition(Vec3d(3.93824e-006, 0.432906, 6.99487e-006)); 
	solids[3]->GetPHSolid()->SetFramePosition(Vec3d(6.8488e-006, 0.638779, 1.38837e-005)); 
	solids[4]->GetPHSolid()->SetFramePosition(Vec3d(8.45627e-006, 0.757086, 0.00757778)); 
	solids[5]->GetPHSolid()->SetFramePosition(Vec3d(-0.3537, 0.532354, 1.0018e-005)); 
	solids[6]->GetPHSolid()->SetFramePosition(Vec3d(-0.6623, 0.532005, 9.94903e-006)); 
	solids[7]->GetPHSolid()->SetFramePosition(Vec3d(-0.896, 0.531835, 9.659e-006)); 
	solids[8]->GetPHSolid()->SetFramePosition(Vec3d(-0.0691705, -0.207, -1.55974e-006)); 
	solids[9]->GetPHSolid()->SetFramePosition(Vec3d(-0.0691705, -0.6089, -5.97062e-006)); 
	solids[10]->GetPHSolid()->SetFramePosition(Vec3d(-0.0691705, -0.837152, 0.0737905)); 
	solids[11]->GetPHSolid()->SetFramePosition(Vec3d(-0.0309416, 0.750932, 0.0885275)); 
	solids[12]->GetPHSolid()->SetFramePosition(Vec3d(0.353713, 0.532349, 9.75737e-006)); 
	solids[13]->GetPHSolid()->SetFramePosition(Vec3d(0.662313, 0.531966, 9.76959e-006)); 
	solids[14]->GetPHSolid()->SetFramePosition(Vec3d(0.896013, 0.531771, 9.96697e-006)); 
	solids[15]->GetPHSolid()->SetFramePosition(Vec3d(0.0691705, -0.207, -1.55975e-006)); 
	solids[16]->GetPHSolid()->SetFramePosition(Vec3d(0.0691705, -0.6089, -5.97065e-006)); 
	solids[17]->GetPHSolid()->SetFramePosition(Vec3d(0.0691705, -0.837152, 0.0737905)); 
	solids[18]->GetPHSolid()->SetFramePosition(Vec3d(0.0309584, 0.750932, 0.0885275));
}

void CRBallHumanBodyGen::InitIK() {
	{
		int s[] = {SO_ABDOMEN, SO_CHEST, SO_NECK, SO_HEAD, SO_RIGHT_EYE};
		int j[] = {JO_WAIST_ABDOMEN, JO_ABDOMEN_CHEST, JO_CHEST_NECK, JO_NECK_HEAD, JO_RIGHT_EYE};
		for (int n=4; n>=0; --n) {
			for (int m=n; m>=0; --m) {
				joints[j[m]]->GetIKActuator()->AddChildObject(solids[s[n]]->GetIKEndEffector());
			}
		}
	}

	{
		int s[] = {SO_ABDOMEN, SO_CHEST, SO_NECK, SO_HEAD, SO_LEFT_EYE};
		int j[] = {JO_WAIST_ABDOMEN, JO_ABDOMEN_CHEST, JO_CHEST_NECK, JO_NECK_HEAD, JO_LEFT_EYE};
		for (int n=4; n>=0; --n) {
			for (int m=n; m>=0; --m) {
				joints[j[m]]->GetIKActuator()->AddChildObject(solids[s[n]]->GetIKEndEffector());
			}
		}
	}

	{
		int s[] = {SO_ABDOMEN, SO_CHEST, SO_RIGHT_UPPER_ARM, SO_RIGHT_LOWER_ARM, SO_RIGHT_HAND};
		int j[] = {JO_WAIST_ABDOMEN, JO_ABDOMEN_CHEST, JO_RIGHT_SHOULDER, JO_RIGHT_ELBOW, JO_RIGHT_WRIST};
		for (int n=4; n>=0; --n) {
			for (int m=n; m>=0; --m) {
				joints[j[m]]->GetIKActuator()->AddChildObject(solids[s[n]]->GetIKEndEffector());
			}
		}
	}

	{
		int s[] = {SO_ABDOMEN, SO_CHEST, SO_LEFT_UPPER_ARM, SO_LEFT_LOWER_ARM, SO_LEFT_HAND};
		int j[] = {JO_WAIST_ABDOMEN, JO_ABDOMEN_CHEST, JO_LEFT_SHOULDER, JO_LEFT_ELBOW, JO_LEFT_WRIST};
		for (int n=4; n>=0; --n) {
			for (int m=n; m>=0; --m) {
				joints[j[m]]->GetIKActuator()->AddChildObject(solids[s[n]]->GetIKEndEffector());
			}
		}
	}
}

void CRBallHumanBodyGen::SetJointSpringDamper(PHBallJointDesc &ballDesc, double springOrig, double damperOrig, double actuatorMass){
	ballDesc.spring =   10.0;
	ballDesc.damper =    5.0;
	/*
	if (springOrig > 0 && damperOrig > 0) {
		ballDesc.spring = springOrig;
		ballDesc.damper = damperOrig;
	} else {
		double dt = phScene->GetTimeStep();
		ballDesc.spring = spring*2 * actuatorMass / (dt*dt);
		ballDesc.damper = damper   * actuatorMass /  dt;
	}
	*/
}

void CRBallHumanBodyGen::SetJointSpringDamper(PHHingeJointDesc &hingeDesc, double springOrig, double damperOrig, double actuatorMass){
	hingeDesc.spring =   5.0;
	hingeDesc.damper =   2.5;
}

void CRBallHumanBodyGen::CreateIKActuator(int n) {
	/*
	if (DCAST(PHBallJointIf,joints[n])) {
		PHIKBallActuatorDesc descIKAct;
		descIKAct.spring = 6000.0;
		descIKAct.damper =   50.0;
		ikActuators[n] = phScene->CreateIKActuator(descIKAct);
		ikActuators[n]->AddChildObject(joints[n]->Cast());
	} else if (DCAST(PHHingeJointIf,joints[n])) {
		PHIKHingeActuatorDesc descIKAct;
		descIKAct.spring =  600.0;
		descIKAct.damper =    5.0;
		ikActuators[n] = phScene->CreateIKActuator(descIKAct);
		ikActuators[n]->AddChildObject(joints[n]->Cast());
	}
	*/
}

void CRBallHumanBodyGen::CreateIKEndEffector(int n) {
	/*
	PHIKEndEffectorDesc descIKEEf;
	descIKEEf.targetLocalPosition   = Vec3d(0,0,0);
	ikEndEffectors[n] = phScene->CreateIKEndEffector(descIKEEf);
	ikEndEffectors[n]->AddChildObject(solids[n]);
	ikEndEffectors[n]->Enable(false);
	*/
}

}

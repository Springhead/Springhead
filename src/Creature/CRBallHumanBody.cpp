/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBallHumanBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRBallHumanBodyDesc::CRBallHumanBodyDesc(){
	bodyMass = 1.0f;  /// デフォルトがこれなのはどうも変だが(mitake)

	waistHeight    = 0.2298;
	waistBreadth   = 0.3067;
	waistThickness = 0.2307;

	abdomenHeight    = 1.2253 - 1.0142;
	abdomenBreadth   = 0.2611;
	abdomenThickness = 0.1882;

	chestHeight    = 1.4020 - 1.2253;
	chestBreadth   = 0.2887;
	chestThickness = 0.2118;

	neckLength   = 1.7219 - 1.4564 - 0.1732;
	neckDiameter = 0.3563 / 3.1415;

	headBreadth = 0.1619;
	headHeight  = 0.2387;

	upperArmLength   = 0.3406;
	upperArmDiameter = 0.2808 / 3.1415;

	lowerArmLength   = 0.2766;
	lowerArmDiameter = 0.1677 / 3.1415;

	handLength    = 0.1908;
	handBreadth   = 0.1018;
	handThickness = 0.0275;

	upperLegLength   = 0.4092;
	upperLegDiameter = 0.1682;
	interLegDistance = upperLegDiameter + 0.03; /// 適当

	lowerLegLength   = 0.3946;
	lowerLegDiameter = 0.1109;

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
	springEye          = 100000.0;  damperEye          =  2000.0;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRBallHumanBody


// --- --- ---
void CRBallHumanBody::Init(){
	CRBody::Init();
}

// --- --- ---
void CRBallHumanBody::InitBody(){
	CreateWaist();
	CreateAbdomen();
	CreateChest();
}

void CRBallHumanBody::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass     = 0.17 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
}

void CRBallHumanBody::CreateAbdomen(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass     = 0.028 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_ABDOMEN] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(abdomenBreadth, abdomenHeight, abdomenThickness);
	solids[SO_ABDOMEN]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Waist-[c]Abdomen
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0,  waistHeight / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -abdomenHeight / 2.0, 0);
	SetJointSpringDamper(ballDesc, springWaistAbdomen, damperWaistAbdomen, solids[SO_WAIST]->GetMass());
	// ballDesc.spring           = springWaistAbdomen;
	// ballDesc.damper           = damperWaistAbdomen;
	joints[JO_WAIST_ABDOMEN] = CreateJoint(solids[SO_ABDOMEN], solids[SO_WAIST], ballDesc);

	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRBallHumanBody::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass   = 0.252 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Abdomen-[c]Chest
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0,  abdomenHeight / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	SetJointSpringDamper(ballDesc, springAbdomenChest, damperAbdomenChest, solids[SO_ABDOMEN]->GetMass());
	// ballDesc.spring           = springAbdomenChest;
	// ballDesc.damper           = damperAbdomenChest;
	joints[JO_ABDOMEN_CHEST]  = CreateJoint(solids[SO_CHEST], solids[SO_ABDOMEN], ballDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_ABDOMEN], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBody::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRBallHumanBody::CreateNeck(){
	CDCapsuleDesc      capsuleDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass  = 0.028 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_NECK] = phScene->CreateSolid(solidDesc);
	capsuleDesc.radius = neckDiameter/2;
	capsuleDesc.length = neckLength;
	solids[SO_NECK]->AddShape(phSdk->CreateShape(capsuleDesc));
	Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
	solids[SO_NECK]->SetShapePose(0,pose);

	// Joint -- [p]Chest-[c]Neck
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0, chestHeight / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
	SetJointSpringDamper(ballDesc, springChestNeck, damperChestNeck, solids[SO_CHEST]->GetMass());
	// ballDesc.spring           = springChestNeckX;
	// ballDesc.damper           = damperChestNeckX;
	joints[JO_CHEST_NECK] = CreateJoint(solids[SO_NECK], solids[SO_CHEST], ballDesc);

	phScene->SetContactMode(solids[SO_NECK], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRBallHumanBody::CreateHead(){
	CDCapsuleDesc     capsuleDesc;
	PHSolidDesc       solidDesc;
	PHBallJointDesc   ballDesc;

	// Solid
	solidDesc.mass    = 0.07 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[SO_HEAD]   = phScene->CreateSolid(solidDesc);
	capsuleDesc.radius = (float)(headBreadth / 2.0);
	capsuleDesc.length = (float)(headHeight - headBreadth);
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(capsuleDesc));
	Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
	solids[SO_HEAD]->SetShapePose(0,pose);

	// Joint -- [p]Neck-[c]Head
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0, neckLength / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -headHeight / 2.0, 0);
	SetJointSpringDamper(ballDesc, springNeckHead, damperNeckHead, solids[SO_NECK]->GetMass());
	// ballDesc.spring           = springNeckHeadX;
	// ballDesc.damper           = damperNeckHeadX;
	joints[JO_NECK_HEAD] = CreateJoint(solids[SO_HEAD], solids[SO_NECK], ballDesc);

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBody::InitArms(){
	CreateUpperArm(LEFTPART);
	CreateLowerArm(LEFTPART);
	CreateHand(LEFTPART);

	CreateUpperArm(RIGHTPART);
	CreateLowerArm(RIGHTPART);
	CreateHand(RIGHTPART);
}

void CRBallHumanBody::CreateUpperArm(LREnum lr){
	CDCapsuleDesc      capsuleDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNUpperArm;
	CRBallHumanBodyDesc::CRHumanJoints joNShoulder;
	if (lr==LEFTPART) {
		soNUpperArm   = SO_LEFT_UPPER_ARM;
		joNShoulder   = JO_LEFT_SHOULDER;
	} else {
		soNUpperArm   = SO_RIGHT_UPPER_ARM;
		joNShoulder   = JO_RIGHT_SHOULDER;
	}

	// Solid
	solidDesc.mass      = 0.04 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNUpperArm] = phScene->CreateSolid(solidDesc);
	capsuleDesc.radius  = upperArmDiameter / 2.0;
	capsuleDesc.length  = upperArmLength;
	solids[soNUpperArm]->AddShape(phSdk->CreateShape(capsuleDesc));
	Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
	solids[soNUpperArm]->SetShapePose(0,pose);

	// Joint -- Shoulder ([p]Chest-[c]UpperArm)
	ballDesc                    = PHBallJointDesc();
	ballDesc.posePlug.Pos() = Vec3d(lr*chestBreadth/2.0 + lr*(upperArmDiameter/2.0*1.414), chestHeight/2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, upperArmLength / 2.0, 0.0);
	SetJointSpringDamper(ballDesc, springShoulder, damperShoulder, solids[SO_CHEST]->GetMass());
	// ballDesc.spring           = springShoulderZ;
	// ballDesc.damper           = damperShoulderZ;
	joints[joNShoulder]       = CreateJoint(solids[soNUpperArm], solids[SO_CHEST], ballDesc);

	phScene->SetContactMode(solids[soNUpperArm], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRBallHumanBody::CreateLowerArm(LREnum lr){
	CDCapsuleDesc      capsuleDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNUpperArm, soNLowerArm;
	CRBallHumanBodyDesc::CRHumanJoints joNElbow;
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
	solids[soNLowerArm] = phScene->CreateSolid(solidDesc);
	capsuleDesc.radius  = lowerArmDiameter / 2.0;
	capsuleDesc.length  = lowerArmLength;
	solids[soNLowerArm]->AddShape(phSdk->CreateShape(capsuleDesc));
	Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
	solids[soNLowerArm]->SetShapePose(0,pose);

	// Joint -- Elbow ([p]UpperArm-[c]LowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerArmLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'y');
	SetJointSpringDamper(hingeDesc, springElbow, damperElbow, solids[soNUpperArm]->GetMass());
	// hingeDesc.spring           = springElbow;
	// hingeDesc.damper           = damperElbow;
	hingeDesc.origin           = Rad(0);
	joints[joNElbow] = CreateJoint(solids[soNLowerArm], solids[soNUpperArm], hingeDesc);

	phScene->SetContactMode(solids[soNLowerArm], solids[soNUpperArm], PHSceneDesc::MODE_NONE);
}

void CRBallHumanBody::CreateHand(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNLowerArm, soNHand;
	CRBallHumanBodyDesc::CRHumanJoints joNWrist;
	if (lr==LEFTPART) {
		soNLowerArm = SO_LEFT_LOWER_ARM; soNHand = SO_LEFT_HAND;
		joNWrist = JO_LEFT_WRIST; 
	} else {
		soNLowerArm = SO_RIGHT_LOWER_ARM; soNHand = SO_RIGHT_HAND;
		joNWrist = JO_RIGHT_WRIST; 
	}

	// Solid
	solidDesc.mass   = 0.01 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNHand]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(handBreadth, handLength, handThickness);
	solids[soNHand]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Wrist ([p]LowerArm-[c]Hand)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0.0, -lowerArmLength / 2.0, 0);
	ballDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.poseSocket.Pos() = Vec3d(0, handLength / 2.0, 0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	SetJointSpringDamper(ballDesc, springWrist, damperWrist, solids[soNLowerArm]->GetMass());
	// ballDesc.spring           = springWristY;
	// ballDesc.damper           = damperWristY;
	joints[joNWrist]          = CreateJoint(solids[soNHand], solids[soNLowerArm], ballDesc);

	phScene->SetContactMode(solids[soNHand], solids[soNLowerArm], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBody::InitEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRBallHumanBody::CreateEye(LREnum lr){
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNEye;
	CRBallHumanBodyDesc::CRHumanJoints joNEye;
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
	solids[soNEye]     = phScene->CreateSolid(solidDesc);
	sphereDesc.radius  = (float)(eyeDiameter/2.0);
	solids[soNEye]->AddShape(phSdk->CreateShape(sphereDesc));
	sphereDesc.radius  = (float)(eyeDiameter/4.0);
	solids[soNEye]->AddShape(phSdk->CreateShape(sphereDesc));
	Posed pose=Posed(); pose.Pos() = Vec3d(0,0,eyeDiameter/3.0);
	solids[soNEye]->SetShapePose(1,pose);

	// Joint -- Eye ([p]Head-[c]Eye)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(lr*interpupillaryBreadth/2.0, headHeight/2.0 - vertexToEyeHeight, headBreadth/2.0);
	ballDesc.spring           = springEye;
	ballDesc.damper           = damperEye;
	joints[joNEye]            = CreateJoint(solids[soNEye], solids[SO_HEAD], ballDesc);

	phScene->SetContactMode(solids[soNEye], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBody::InitLegs(){
	CreateUpperLeg(LEFTPART);
	CreateLowerLeg(LEFTPART);
	CreateFoot(LEFTPART);

	CreateUpperLeg(RIGHTPART);
	CreateLowerLeg(RIGHTPART);
	CreateFoot(RIGHTPART);
}

void CRBallHumanBody::CreateUpperLeg(LREnum lr){
	CDCapsuleDesc      capsuleDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNWaistLeg, soNUpperLeg;
	CRBallHumanBodyDesc::CRHumanJoints joNWaistLeg;
	if (lr==LEFTPART) {
		soNUpperLeg = SO_LEFT_UPPER_LEG;
		joNWaistLeg = JO_LEFT_WAIST_LEG;
	} else {
		soNUpperLeg = SO_RIGHT_UPPER_LEG;
		joNWaistLeg = JO_RIGHT_WAIST_LEG;
	}

	// Solid
	solidDesc.mass      = 0.06 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNUpperLeg] = phScene->CreateSolid(solidDesc);
	capsuleDesc.radius  = upperLegDiameter / 2.0;
	capsuleDesc.length  = upperLegLength;
	solids[soNUpperLeg]->AddShape(phSdk->CreateShape(capsuleDesc));
	Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
	solids[soNUpperLeg]->SetShapePose(0,pose);

	// Joint -- WaistLeg ([p]Waist-[c]UpperLeg)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(lr*interLegDistance/2.0, -waistHeight/2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, upperLegLength / 2.0, 0.0);
	SetJointSpringDamper(ballDesc, springWaistLeg, damperWaistLeg, solids[SO_WAIST]->GetMass());
	// ballDesc.spring           = springWaistLegZ;
	// ballDesc.damper           = damperWaistLegZ;
	joints[joNWaistLeg]       = CreateJoint(solids[soNUpperLeg], solids[SO_WAIST], ballDesc);

	phScene->SetContactMode(solids[soNUpperLeg], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRBallHumanBody::CreateLowerLeg(LREnum lr){
	CDCapsuleDesc      capsuleDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNUpperLeg, soNLowerLeg;
	CRBallHumanBodyDesc::CRHumanJoints joNKnee;
	if (lr==LEFTPART) {
		soNUpperLeg = SO_LEFT_UPPER_LEG; soNLowerLeg = SO_LEFT_LOWER_LEG;
		joNKnee = JO_LEFT_KNEE;
	} else {
		soNUpperLeg = SO_RIGHT_UPPER_LEG; soNLowerLeg = SO_RIGHT_LOWER_LEG;
		joNKnee = JO_RIGHT_KNEE;
	}

	// Solid
	solidDesc.mass      = 0.05 * bodyMass;
	solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
	solids[soNLowerLeg] = phScene->CreateSolid(solidDesc);
	capsuleDesc.radius  = lowerLegDiameter / 2.0;
	capsuleDesc.length  = lowerLegLength;
	solids[soNLowerLeg]->AddShape(phSdk->CreateShape(capsuleDesc));
	Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
	solids[soNLowerLeg]->SetShapePose(0,pose);

	// Joint -- Knee ([p]UpperLeg-[c]LowerLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerLegLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springKnee, damperKnee, solids[soNUpperLeg]->GetMass());
	// hingeDesc.spring           = springKnee;
	// hingeDesc.damper           = damperKnee;
	joints[joNKnee] = CreateJoint(solids[soNLowerLeg], solids[soNUpperLeg], hingeDesc);

	phScene->SetContactMode(solids[soNLowerLeg], solids[soNUpperLeg], PHSceneDesc::MODE_NONE);
}

void CRBallHumanBody::CreateFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	CRBallHumanBodyDesc::CRHumanSolids soNLowerLeg, soNFoot;
	CRBallHumanBodyDesc::CRHumanJoints joNAnkle;
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
	solids[soNFoot]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(footBreadth, footThickness, footLength);
	solids[soNFoot]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Ankle ([p]LowerLeg-[c]Foot)
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0.0, -lowerLegLength / 2.0, 0);
	ballDesc.poseSocket.Pos() = Vec3d(0, footThickness / 2.0, (footLength/2.0 - ankleToeDistance));
	SetJointSpringDamper(ballDesc, springAnkle, damperAnkle, solids[soNLowerLeg]->GetMass());
	// ballDesc.spring           = springAnkleY;
	// ballDesc.damper           = damperAnkleY;
	joints[joNAnkle]          = CreateJoint(solids[soNFoot], solids[soNLowerLeg], ballDesc);

	phScene->SetContactMode(solids[soNFoot], solids[soNLowerLeg], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRBallHumanBody::InitContact(){
	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	// 念のため今は残してあるが下のコードがあれば不要な気がする．(mitake)
	// phScene->SetContactMode(solids[SO_LEFT_UPPER_LEG], solids[SO_RIGHT_UPPER_LEG], PHSceneDesc::MODE_NONE);

	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	/**/
	for (unsigned int i=0; i<solids.size(); ++i) {
		for (unsigned int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i], solids[j], PHSceneDesc::MODE_NONE);
			}
		}
	}
	/**/

	// 自分以外にすでにBodyが居ればそのBodyに属する剛体とのContactも切る
	/*/
	for (int i=0; i<creature->NBodies(); ++i) {
		CRBodyIf* body = creature->GetBody(i);
		if (DCAST(CRBallHumanBodyIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
	/**/
}

void CRBallHumanBody::SetJointSpringDamper(PHBallJointDesc &ballDesc, double springOrig, double damperOrig, double actuatorMass){
	ballDesc.spring = 1;//100000;
	ballDesc.damper = 0.02;//  2000;
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

void CRBallHumanBody::SetJointSpringDamper(PHHingeJointDesc &hingeDesc, double springOrig, double damperOrig, double actuatorMass){
	hingeDesc.spring = 1;//100000;
	hingeDesc.damper = 0.02;//  2000;
}
}

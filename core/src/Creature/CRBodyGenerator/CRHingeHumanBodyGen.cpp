/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyGenerator/CRHingeHumanBodyGen.h>
#include <Collision/SprCDShape.h>
#include <Physics/SprPHSdk.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSolid.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRHingeHumanBodyGenDesc::CRHingeHumanBodyGenDesc(){
	jointOrder = SOCKET_PARENT; // そのうちなくす

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
	neckDiameter = 0.1442;

	headDiameter = 0.2387;

	upperArmLength   = 0.3406;
	upperArmDiameter = 0.2808 / 3.1415;

	lowerArmLength   = 0.2766;
	lowerArmDiameter = 0.1677 / 3.1415;

	handLength    = 0.1908;
	handBreadth   = 0.1018;
	handThickness = 0.0275;

	upperLegLength   = 0.4092;
	upperLegDiameter = 0.1682;
	interLegDistance = waistBreadth - upperLegDiameter;

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

	spring = 0.2;             damper = 0.6;

	springWaistAbdomen = -1;  damperWaistAbdomen = -1;
	springAbdomenChest = -1;  damperAbdomenChest = -1;
	springChestNeckX   = -1;  damperChestNeckX   = -1;
	springChestNeckY   = -1;  damperChestNeckY   = -1;
	springChestNeckZ   = -1;  damperChestNeckZ   = -1;
	springNeckHeadX    = -1;  damperNeckHeadX    = -1;
	springNeckHeadZ    = -1;  damperNeckHeadZ    = -1;
	springShoulderZ    = -1;  damperShoulderZ    = -1;
	springShoulderX    = -1;  damperShoulderX    = -1;
	springShoulderY    = -1;  damperShoulderY    = -1;
	springElbow        = -1;  damperElbow        = -1;
	springWristY       = -1;  damperWristY       = -1;
	springWristX       = -1;  damperWristX       = -1;
	springWristZ       = -1;  damperWristZ       = -1;
	springWaistLegZ    = -1;  damperWaistLegZ    = -1;
	springWaistLegX    = -1;  damperWaistLegX    = -1;
	springWaistLegY    = -1;  damperWaistLegY    = -1;
	springKnee         = -1;  damperKnee         = -1;
	springAnkleY       = -1;  damperAnkleY       = -1;
	springAnkleX       = -1;  damperAnkleX       = -1;
	springAnkleZ       = -1;  damperAnkleZ       = -1;

	springEyeY         = 500.0;  damperEyeY      =  5.0;
	springEyeX         = 500.0;  damperEyeX      =  5.0;

	posRightUpperArm = Vec3d(0,0,0);

	// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
	rangeWaistAbdomen = Vec2d(Rad(+360) , Rad(-360));
	rangeAbdomenChest = Vec2d(Rad(+360) , Rad(-360));
	rangeChestNeckX   = Vec2d(Rad(+360) , Rad(-360));
	rangeChestNeckY   = Vec2d(Rad(+360) , Rad(-360));
	rangeChestNeckZ   = Vec2d(Rad(+360) , Rad(-360));
	rangeNeckHeadX    = Vec2d(Rad(+360) , Rad(-360));
	rangeNeckHeadZ    = Vec2d(Rad(+360) , Rad(-360));
	rangeShoulderZ    = Vec2d(Rad(+360) , Rad(-360));
	rangeShoulderX    = Vec2d(Rad(+360) , Rad(-360));
	rangeShoulderY    = Vec2d(Rad(+360) , Rad(-360));
	rangeElbow        = Vec2d(Rad(+360) , Rad(-360));
	rangeWristY       = Vec2d(Rad(+360) , Rad(-360));
	rangeWristX       = Vec2d(Rad(+360) , Rad(-360));
	rangeWristZ       = Vec2d(Rad(+360) , Rad(-360));
	rangeWaistLegZ    = Vec2d(Rad(+360) , Rad(-360));
	rangeWaistLegX    = Vec2d(Rad(+360) , Rad(-360));
	rangeWaistLegY    = Vec2d(Rad(+360) , Rad(-360));
	rangeKnee         = Vec2d(Rad(+360) , Rad(-360));
	rangeAnkleY       = Vec2d(Rad(+360) , Rad(-360));
	rangeAnkleX       = Vec2d(Rad(+360) , Rad(-360));
	rangeAnkleZ       = Vec2d(Rad(+360) , Rad(-360));
	rangeRightEyeY    = Vec2d(Rad( -30) , Rad( +45));
	rangeEyeX         = Vec2d(Rad( -45) , Rad( +45));
	rangeRightEyeY    = Vec2d(Rad(+360) , Rad(-360));
	rangeEyeX         = Vec2d(Rad(+360) , Rad(-360));

	noLegs = false;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeHumanBodyGen


// --- --- ---
void CRHingeHumanBodyGen::Init(){
	
}

// --- --- ---
void CRHingeHumanBodyGen::InitBody(){
	CreateWaist();
	CreateAbdomen();
	CreateChest();
}

void CRHingeHumanBodyGen::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass     = 0.17;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_WAIST]->SetOrientation(Quaterniond::Rot(Rad(0), 'y'));
}

void CRHingeHumanBodyGen::CreateAbdomen(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass     = 0.028;
	solids[SO_ABDOMEN] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(abdomenBreadth, abdomenHeight, abdomenThickness);
	solids[SO_ABDOMEN]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Waist-[c]Abdomen
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,  waistHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -abdomenHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	SetJointSpringDamper(hingeDesc, springWaistAbdomen, damperWaistAbdomen, solids[SO_WAIST]->GetMass());
	// hingeDesc.spring           = springWaistAbdomen;
	// hingeDesc.damper           = damperWaistAbdomen;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeWaistAbdomen[0];
	hingeDesc.upper            = rangeWaistAbdomen[1];
	joints[JO_WAIST_ABDOMEN] = CreateJoint(solids[SO_ABDOMEN], solids[SO_WAIST], hingeDesc);

	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeHumanBodyGen::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass   = 0.252;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Abdomen-[c]Chest
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,  abdomenHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springAbdomenChest, damperAbdomenChest, solids[SO_ABDOMEN]->GetMass());
	// hingeDesc.spring           = springAbdomenChest;
	// hingeDesc.damper           = damperAbdomenChest;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeAbdomenChest[0];
	hingeDesc.upper            = rangeAbdomenChest[1];
	joints[JO_ABDOMEN_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_ABDOMEN], hingeDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_ABDOMEN], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHumanBodyGen::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRHingeHumanBodyGen::CreateNeck(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass  = 0.028;
	solids[SO_NECK] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize = Vec3f(neckDiameter/1.414, neckLength, neckDiameter/1.414);
	solids[SO_NECK]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.014;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[SO_CHEST_NECK_XZ] = phScene->CreateSolid(solidDesc);
	solids[SO_CHEST_NECK_ZY] = phScene->CreateSolid(solidDesc);

	// Joint -- [p]Chest-[c]Neck
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, chestHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springChestNeckX, damperChestNeckX, solids[SO_CHEST]->GetMass());
	// hingeDesc.spring           = springChestNeckX;
	// hingeDesc.damper           = damperChestNeckX;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeChestNeckX[0];
	hingeDesc.upper            = rangeChestNeckX[1];
	joints[JO_CHEST_NECK_X] = CreateJoint(solids[SO_CHEST_NECK_XZ], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	SetJointSpringDamper(hingeDesc, springChestNeckZ, damperChestNeckZ, solids[SO_CHEST]->GetMass());
	// hingeDesc.spring           = springChestNeckZ;
	// hingeDesc.damper           = damperChestNeckZ;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeChestNeckZ[0];
	hingeDesc.upper            = rangeChestNeckZ[1];
	joints[JO_CHEST_NECK_Z] = CreateJoint(solids[SO_CHEST_NECK_ZY], solids[SO_CHEST_NECK_XZ], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	SetJointSpringDamper(hingeDesc, springChestNeckY, damperChestNeckY, solids[SO_CHEST]->GetMass());
	// hingeDesc.spring           = springChestNeckY;
	// hingeDesc.damper           = damperChestNeckY;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeChestNeckY[0];
	hingeDesc.upper            = rangeChestNeckY[1];
	joints[JO_CHEST_NECK_Y] = CreateJoint(solids[SO_NECK], solids[SO_CHEST_NECK_ZY], hingeDesc);

	phScene->SetContactMode(solids[SO_NECK], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRHingeHumanBodyGen::CreateHead(){
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass    = 0.07;
	solids[SO_HEAD]   = phScene->CreateSolid(solidDesc);
	sphereDesc.radius = (float)(headDiameter / 2.0);
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(sphereDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.035;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[SO_NECK_HEAD_XZ] = phScene->CreateSolid(solidDesc);

	// Joint -- [p]Neck-[c]Head
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, neckLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springNeckHeadX, damperNeckHeadX, solids[SO_NECK]->GetMass());
	// hingeDesc.spring           = springNeckHeadX;
	// hingeDesc.damper           = damperNeckHeadX;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeNeckHeadX[0];
	hingeDesc.upper            = rangeNeckHeadX[1];
	joints[JO_NECK_HEAD_X] = CreateJoint(solids[SO_NECK_HEAD_XZ], solids[SO_NECK], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -headDiameter / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	SetJointSpringDamper(hingeDesc, springNeckHeadZ, damperNeckHeadZ, solids[SO_NECK]->GetMass());
	// hingeDesc.spring           = springNeckHeadZ;
	// hingeDesc.damper           = damperNeckHeadZ;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeNeckHeadZ[0];
	hingeDesc.upper            = rangeNeckHeadZ[1];
	joints[JO_NECK_HEAD_Z] = CreateJoint(solids[SO_HEAD], solids[SO_NECK_HEAD_XZ], hingeDesc);

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHumanBodyGen::InitArms(){
	CreateUpperArm(LEFTPART);
	CreateLowerArm(LEFTPART);
	CreateHand(LEFTPART);

	CreateUpperArm(RIGHTPART);
	CreateLowerArm(RIGHTPART);
	CreateHand(RIGHTPART);
}

void CRHingeHumanBodyGen::CreateUpperArm(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNShoulderZX, soNShoulderXY, soNUpperArm;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNShoulderZ, joNShoulderX, joNShoulderY;
	if (lr==LEFTPART) {
		soNShoulderZX = SO_LEFT_SHOULDER_ZX; soNShoulderXY = SO_LEFT_SHOULDER_XY; soNUpperArm   = SO_LEFT_UPPER_ARM;
		joNShoulderZ  = JO_LEFT_SHOULDER_Z;  joNShoulderX  = JO_LEFT_SHOULDER_X;  joNShoulderY  = JO_LEFT_SHOULDER_Y;
	} else {
		soNShoulderZX = SO_RIGHT_SHOULDER_ZX; soNShoulderXY = SO_RIGHT_SHOULDER_XY; soNUpperArm   = SO_RIGHT_UPPER_ARM;
		joNShoulderZ  = JO_RIGHT_SHOULDER_Z;  joNShoulderX  = JO_RIGHT_SHOULDER_X;  joNShoulderY  = JO_RIGHT_SHOULDER_Y;
	}

	// Solid
	solidDesc.mass      = 0.04;
	solids[soNUpperArm] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(upperArmDiameter, upperArmLength, upperArmDiameter);
	solids[soNUpperArm]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.02;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNShoulderZX] = phScene->CreateSolid(solidDesc);
	solids[soNShoulderXY] = phScene->CreateSolid(solidDesc);

	// Joint -- Shoulder ([p]Chest-[c]UpperArm)
	hingeDesc                  = PHHingeJointDesc();
	if (posRightUpperArm==Vec3d(0,0,0)) {
		hingeDesc.posePlug.Pos() = Vec3d(lr*chestBreadth/2.0 + lr*(upperArmDiameter/2.0*1.414), chestHeight/2.0, 0);
	} else {
		hingeDesc.posePlug.Pos() = Vec3d(lr*posRightUpperArm.x, posRightUpperArm.y+chestHeight/2.0, posRightUpperArm.z);
	}
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	SetJointSpringDamper(hingeDesc, springShoulderZ, damperShoulderZ, solids[SO_CHEST]->GetMass());
	// hingeDesc.spring           = springShoulderZ;
	// hingeDesc.damper           = damperShoulderZ;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeShoulderZ[0]) : (-rangeShoulderZ[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeShoulderZ[1]) : (-rangeShoulderZ[0]));
	joints[joNShoulderZ]       = CreateJoint(solids[soNShoulderZX], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springShoulderX, damperShoulderX, solids[SO_CHEST]->GetMass());
	// hingeDesc.spring           = springShoulderX;
	// hingeDesc.damper           = damperShoulderX;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeShoulderX[0];
	hingeDesc.upper            = rangeShoulderX[1];
	joints[joNShoulderX]       = CreateJoint(solids[soNShoulderXY], solids[soNShoulderZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, upperArmLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightUpperArm.w, lr*oriRightUpperArm.x, oriRightUpperArm.y, oriRightUpperArm.z).Inv() * Quaterniond::Rot(Rad(-90), 'x');
	SetJointSpringDamper(hingeDesc, springShoulderY, damperShoulderY, solids[SO_CHEST]->GetMass());
	// hingeDesc.spring           = springShoulderY;
	// hingeDesc.damper           = damperShoulderY;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeShoulderY[0]) : (-rangeShoulderY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeShoulderY[1]) : (-rangeShoulderY[0]));
	joints[joNShoulderY]       = CreateJoint(solids[soNUpperArm], solids[soNShoulderXY], hingeDesc);

	phScene->SetContactMode(solids[soNUpperArm], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRHingeHumanBodyGen::CreateLowerArm(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNUpperArm, soNLowerArm;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNElbow;
	if (lr==LEFTPART) {
		soNUpperArm = SO_LEFT_UPPER_ARM; soNLowerArm = SO_LEFT_LOWER_ARM;
		joNElbow = JO_LEFT_ELBOW;
	} else {
		soNUpperArm = SO_RIGHT_UPPER_ARM; soNLowerArm = SO_RIGHT_LOWER_ARM;
		joNElbow = JO_RIGHT_ELBOW;
	}

	// Solid
	solidDesc.mass      = 0.02;
	solids[soNLowerArm] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(lowerArmDiameter, lowerArmLength, lowerArmDiameter);
	solids[soNLowerArm]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Elbow ([p]UpperArm-[c]LowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerArmLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightLowerArm.w, lr*oriRightLowerArm.x, oriRightLowerArm.y, oriRightLowerArm.z).Inv() * Quaterniond::Rot(Rad(0), 'y');
	SetJointSpringDamper(hingeDesc, springElbow, damperElbow, solids[soNUpperArm]->GetMass());
	// hingeDesc.spring           = springElbow;
	// hingeDesc.damper           = damperElbow;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeElbow[0]) : (-rangeElbow[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeElbow[1]) : (-rangeElbow[0]));
	joints[joNElbow] = CreateJoint(solids[soNLowerArm], solids[soNUpperArm], hingeDesc);

	phScene->SetContactMode(solids[soNLowerArm], solids[soNUpperArm], PHSceneDesc::MODE_NONE);
}

void CRHingeHumanBodyGen::CreateHand(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNLowerArm, soNWristYX, soNWristXZ, soNHand;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNWristY, joNWristX, joNWristZ;
	if (lr==LEFTPART) {
		soNLowerArm = SO_LEFT_LOWER_ARM; soNWristYX = SO_LEFT_WRIST_YX; soNWristXZ = SO_LEFT_WRIST_XZ; soNHand = SO_LEFT_HAND;
		joNWristY = JO_LEFT_WRIST_Y; joNWristX = JO_LEFT_WRIST_X; joNWristZ = JO_LEFT_WRIST_Z; 
	} else {
		soNLowerArm = SO_RIGHT_LOWER_ARM; soNWristYX = SO_RIGHT_WRIST_YX; soNWristXZ = SO_RIGHT_WRIST_XZ; soNHand = SO_RIGHT_HAND;
		joNWristY = JO_RIGHT_WRIST_Y; joNWristX = JO_RIGHT_WRIST_X; joNWristZ = JO_RIGHT_WRIST_Z; 
	}

	// Solid
	solidDesc.mass   = 0.01;
	solids[soNHand]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(handBreadth, handLength, handThickness);
	solids[soNHand]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.005;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNWristYX] = phScene->CreateSolid(solidDesc);
	solids[soNWristXZ] = phScene->CreateSolid(solidDesc);

	// Joint -- Wrist ([p]LowerArm-[c]Hand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -lowerArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	SetJointSpringDamper(hingeDesc, springWristY, damperWristY, solids[soNLowerArm]->GetMass());
	// hingeDesc.spring           = springWristY;
	// hingeDesc.damper           = damperWristY;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeWristY[0]) : (-rangeWristY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeWristY[1]) : (-rangeWristY[0]));
	joints[joNWristY]          = CreateJoint(solids[soNWristYX], solids[soNLowerArm], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springWristX, damperWristX, solids[soNLowerArm]->GetMass());
	// hingeDesc.spring           = springWristX;
	// hingeDesc.damper           = damperWristX;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeWristX[0];
	hingeDesc.upper            = rangeWristX[1];
	joints[joNWristX]          = CreateJoint(solids[soNWristXZ], solids[soNWristYX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, handLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightHand.w, lr*oriRightHand.x, oriRightHand.y, oriRightHand.z).Inv() * Quaterniond::Rot(Rad(0), 'z');
	SetJointSpringDamper(hingeDesc, springWristZ, damperWristZ, solids[soNLowerArm]->GetMass());
	// hingeDesc.spring           = springWristZ;
	// hingeDesc.damper           = damperWristZ;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeWristZ[0]) : (-rangeWristZ[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeWristZ[1]) : (-rangeWristZ[0]));
	joints[joNWristZ]          = CreateJoint(solids[soNHand], solids[soNWristXZ], hingeDesc);

	phScene->SetContactMode(solids[soNHand], solids[soNLowerArm], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHumanBodyGen::InitEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRHingeHumanBodyGen::CreateEye(LREnum lr){
	CDBoxDesc          boxDesc;
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNEyeYX, soNEye;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNEyeY, joNEyeX;
	if (lr==LEFTPART) {
		soNEyeYX = SO_LEFT_EYE_YX; soNEye = SO_LEFT_EYE;
		joNEyeY = JO_LEFT_EYE_Y; joNEyeX = JO_LEFT_EYE_X;
	} else {
		soNEyeYX = SO_RIGHT_EYE_YX; soNEye = SO_RIGHT_EYE;
		joNEyeY = JO_RIGHT_EYE_Y; joNEyeX = JO_RIGHT_EYE_X;
	}

	// Solid
	solidDesc.mass     = 0.001;
	solidDesc.inertia  = Matrix3d::Unit() * 0.001;
	solids[soNEye]     = phScene->CreateSolid(solidDesc);
	sphereDesc.radius  = (float)eyeDiameter;
	solids[soNEye]->AddShape(phSdk->CreateShape(sphereDesc));
	boxDesc.boxsize    = Vec3f(0.015,0.015,eyeDiameter+0.029);
	solids[soNEye]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.0005;
	solidDesc.inertia = Matrix3d::Unit() * 0.001;
	solids[soNEyeYX] = phScene->CreateSolid(solidDesc);

	// Joint -- Eye ([p]Head-[c]Eye)
	hingeDesc                  = PHHingeJointDesc();
	//hingeDesc.posePlug.Pos()   = Vec3d(lr*interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -occiputToEyeDistance+headDiameter/2.0);
	hingeDesc.posePlug.Pos()   = Vec3d(lr*interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -headDiameter/2.0+eyeDiameter/2.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springEyeY;
	hingeDesc.damper           = damperEyeY;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeRightEyeY[0]) : (-rangeRightEyeY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeRightEyeY[1]) : (-rangeRightEyeY[0]));
	joints[joNEyeY]            = CreateJoint(solids[soNEyeYX], solids[SO_HEAD], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring           = springEyeX;
	hingeDesc.damper           = damperEyeX;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeEyeX[0];
	hingeDesc.upper            = rangeEyeX[1];
	joints[joNEyeX] = CreateJoint(solids[soNEye], solids[soNEyeYX], hingeDesc);

	phScene->SetContactMode(solids[soNEye], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHumanBodyGen::InitLegs(){
	if (!noLegs) {
		CreateUpperLeg(LEFTPART);
		CreateLowerLeg(LEFTPART);
	}
	CreateFoot(LEFTPART);

	if (!noLegs) {
		CreateUpperLeg(RIGHTPART);
		CreateLowerLeg(RIGHTPART);
	}
	CreateFoot(RIGHTPART);
}

void CRHingeHumanBodyGen::CreateUpperLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNWaistLegZX, soNWaistLegXY, soNUpperLeg;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNWaistLegZ, joNWaistLegX, joNWaistLegY;
	if (lr==LEFTPART) {
		soNWaistLegZX = SO_LEFT_WAIST_LEG_ZX; soNWaistLegXY = SO_LEFT_WAIST_LEG_XY; soNUpperLeg   = SO_LEFT_UPPER_LEG;
		joNWaistLegZ  = JO_LEFT_WAIST_LEG_Z;  joNWaistLegX  = JO_LEFT_WAIST_LEG_X;  joNWaistLegY  = JO_LEFT_WAIST_LEG_Y;
	} else {
		soNWaistLegZX = SO_RIGHT_WAIST_LEG_ZX; soNWaistLegXY = SO_RIGHT_WAIST_LEG_XY; soNUpperLeg   = SO_RIGHT_UPPER_LEG;
		joNWaistLegZ  = JO_RIGHT_WAIST_LEG_Z;  joNWaistLegX  = JO_RIGHT_WAIST_LEG_X;  joNWaistLegY  = JO_RIGHT_WAIST_LEG_Y;
	}

	// Solid
	solidDesc.mass      = 0.06;
	solids[soNUpperLeg] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(upperLegDiameter, upperLegLength, upperLegDiameter);
	solids[soNUpperLeg]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.03;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNWaistLegZX] = phScene->CreateSolid(solidDesc);
	solids[soNWaistLegXY] = phScene->CreateSolid(solidDesc);

	// Joint -- WaistLeg ([p]Waist-[c]UpperLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(lr*interLegDistance/2.0, -waistHeight/2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	SetJointSpringDamper(hingeDesc, springWaistLegZ, damperWaistLegZ, solids[SO_WAIST]->GetMass());
	// hingeDesc.spring           = springWaistLegZ;
	// hingeDesc.damper           = damperWaistLegZ;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeWaistLegZ[0]) : (-rangeWaistLegZ[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeWaistLegZ[1]) : (-rangeWaistLegZ[0]));
	joints[joNWaistLegZ]       = CreateJoint(solids[soNWaistLegZX], solids[SO_WAIST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springWaistLegX, damperWaistLegX, solids[SO_WAIST]->GetMass());
	// hingeDesc.spring           = springWaistLegX;
	// hingeDesc.damper           = damperWaistLegX;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeWaistLegX[0];
	hingeDesc.upper            = rangeWaistLegX[1];
	joints[joNWaistLegX]       = CreateJoint(solids[soNWaistLegXY], solids[soNWaistLegZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, upperLegLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	SetJointSpringDamper(hingeDesc, springWaistLegY, damperWaistLegY, solids[SO_WAIST]->GetMass());
	// hingeDesc.spring           = springWaistLegY;
	// hingeDesc.damper           = damperWaistLegY;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeWaistLegY[0]) : (-rangeWaistLegY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeWaistLegY[1]) : (-rangeWaistLegY[0]));
	joints[joNWaistLegY]       = CreateJoint(solids[soNUpperLeg], solids[soNWaistLegXY], hingeDesc);

	phScene->SetContactMode(solids[soNUpperLeg], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeHumanBodyGen::CreateLowerLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNUpperLeg, soNLowerLeg;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNKnee;
	if (lr==LEFTPART) {
		soNUpperLeg = SO_LEFT_UPPER_LEG; soNLowerLeg = SO_LEFT_LOWER_LEG;
		joNKnee = JO_LEFT_KNEE;
	} else {
		soNUpperLeg = SO_RIGHT_UPPER_LEG; soNLowerLeg = SO_RIGHT_LOWER_LEG;
		joNKnee = JO_RIGHT_KNEE;
	}

	// Solid
	solidDesc.mass      = 0.05;
	solids[soNLowerLeg] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(lowerLegDiameter, lowerLegLength, lowerLegDiameter);
	solids[soNLowerLeg]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Knee ([p]UpperLeg-[c]LowerLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerLegLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	SetJointSpringDamper(hingeDesc, springKnee, damperKnee, solids[soNUpperLeg]->GetMass());
	// hingeDesc.spring           = springKnee;
	// hingeDesc.damper           = damperKnee;
	hingeDesc.targetPosition           = Rad(-20);    // hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeKnee[0];
	hingeDesc.upper            = rangeKnee[1];
	joints[joNKnee] = CreateJoint(solids[soNLowerLeg], solids[soNUpperLeg], hingeDesc);

	phScene->SetContactMode(solids[soNLowerLeg], solids[soNUpperLeg], PHSceneDesc::MODE_NONE);
}

void CRHingeHumanBodyGen::CreateFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanBodyGenDesc::CRHumanSolids soNLowerLeg, soNAnkleYX, soNAnkleXZ, soNFoot;
	CRHingeHumanBodyGenDesc::CRHumanJoints joNAnkleY, joNAnkleX, joNAnkleZ;
	if (lr==LEFTPART) {
		soNLowerLeg = SO_LEFT_LOWER_LEG; soNAnkleYX = SO_LEFT_ANKLE_YX; soNAnkleXZ = SO_LEFT_ANKLE_XZ; soNFoot = SO_LEFT_FOOT;
		joNAnkleY = JO_LEFT_ANKLE_Y; joNAnkleX = JO_LEFT_ANKLE_X; joNAnkleZ = JO_LEFT_ANKLE_Z; 
	} else {
		soNLowerLeg = SO_RIGHT_LOWER_LEG; soNAnkleYX = SO_RIGHT_ANKLE_YX; soNAnkleXZ = SO_RIGHT_ANKLE_XZ; soNFoot = SO_RIGHT_FOOT;
		joNAnkleY = JO_RIGHT_ANKLE_Y; joNAnkleX = JO_RIGHT_ANKLE_X; joNAnkleZ = JO_RIGHT_ANKLE_Z; 
	}

	// Solid
	solidDesc.mass   = 0.01;
	solids[soNFoot]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(footBreadth, footThickness, footLength);
	solids[soNFoot]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.005;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNAnkleYX] = phScene->CreateSolid(solidDesc);
	solids[soNAnkleXZ] = phScene->CreateSolid(solidDesc);

	if (!noLegs) {
		// Joint -- Ankle ([p]LowerLeg-[c]Foot)
		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Pos()   = Vec3d(0.0, -lowerLegLength / 2.0, 0);
		hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
		hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
		SetJointSpringDamper(hingeDesc, springAnkleY, damperAnkleY, solids[soNLowerLeg]->GetMass());
		// hingeDesc.spring           = springAnkleY;
		// hingeDesc.damper           = damperAnkleY;
		hingeDesc.targetPosition           = Rad(0);
		hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeAnkleY[0]) : (-rangeAnkleY[1]));
		hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeAnkleY[1]) : (-rangeAnkleY[0]));
		joints[joNAnkleY]          = CreateJoint(solids[soNAnkleYX], solids[soNLowerLeg], hingeDesc);

		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
		hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
		SetJointSpringDamper(hingeDesc, springAnkleX, damperAnkleX, solids[soNLowerLeg]->GetMass());
		// hingeDesc.spring           = springAnkleX;
		// hingeDesc.damper           = damperAnkleX;
		hingeDesc.targetPosition           = Rad(0);
		hingeDesc.lower            = rangeAnkleX[0];
		hingeDesc.upper            = rangeAnkleX[1];
		joints[joNAnkleX]          = CreateJoint(solids[soNAnkleXZ], solids[soNAnkleYX], hingeDesc);

		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
		hingeDesc.poseSocket.Pos() = Vec3d(0, footThickness / 2.0, (ankleToeDistance - footLength/2.0));
		hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
		SetJointSpringDamper(hingeDesc, springAnkleZ, damperAnkleZ, solids[soNLowerLeg]->GetMass());
		// hingeDesc.spring           = springAnkleZ;
		// hingeDesc.damper           = damperAnkleZ;
		hingeDesc.targetPosition           = Rad(0);
		hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeAnkleZ[0]) : (-rangeAnkleZ[1]));
		hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeAnkleZ[1]) : (-rangeAnkleZ[0]));
		joints[joNAnkleZ]          = CreateJoint(solids[soNFoot], solids[soNAnkleXZ], hingeDesc);
	}

	phScene->SetContactMode(solids[soNFoot], solids[soNLowerLeg], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHumanBodyGen::InitContact(){
	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	// 念のため今は残してあるが下のコードがあれば不要な気がする．(mitake)
	phScene->SetContactMode(solids[SO_LEFT_UPPER_LEG], solids[SO_RIGHT_UPPER_LEG], PHSceneDesc::MODE_NONE);

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
		if (DCAST(CRHingeHumanBodyGenIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
	*/
}

void CRHingeHumanBodyGen::SetJointSpringDamper(PHHingeJointDesc &hingeDesc, double springOrig, double damperOrig, double actuatorMass){
	if (springOrig > 0 && damperOrig > 0) {
		hingeDesc.spring = springOrig;
		hingeDesc.damper = damperOrig;
	} else {
		double dt = phScene->GetTimeStep();
		hingeDesc.spring = spring*2 * actuatorMass / (dt*dt);
		hingeDesc.damper = damper   * actuatorMass /  dt;
	}
}

// --- --- ---
void CRHingeHumanBodyGen::SetUpperBodyStiffness(float stiffness){
	DCAST(PHHingeJointIf, joints[JO_WAIST_ABDOMEN])->SetSpring(springWaistAbdomen * stiffness);
	DCAST(PHHingeJointIf, joints[JO_WAIST_ABDOMEN])->SetDamper(damperWaistAbdomen * stiffness);

	DCAST(PHHingeJointIf, joints[JO_ABDOMEN_CHEST])->SetSpring(springAbdomenChest * stiffness);
	DCAST(PHHingeJointIf, joints[JO_ABDOMEN_CHEST])->SetDamper(damperAbdomenChest * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Z])->SetSpring(springShoulderZ * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Z])->SetDamper(damperShoulderZ * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_X])->SetSpring(springShoulderX * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_X])->SetDamper(damperShoulderX * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Y])->SetSpring(springShoulderY * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Y])->SetDamper(damperShoulderY * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_ELBOW])->SetSpring(springElbow * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_ELBOW])->SetDamper(damperElbow * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Y])->SetSpring(springWristY * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Y])->SetDamper(damperWristY * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_X])->SetSpring(springWristX * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_X])->SetDamper(damperWristX * stiffness);

	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Z])->SetSpring(springWristZ * stiffness);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Z])->SetDamper(damperWristZ * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Z])->SetSpring(springShoulderZ * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Z])->SetDamper(damperShoulderZ * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_X])->SetSpring(springShoulderX * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_X])->SetDamper(damperShoulderX * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Y])->SetSpring(springShoulderY * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Y])->SetDamper(damperShoulderY * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_ELBOW])->SetSpring(springElbow * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_ELBOW])->SetDamper(damperElbow * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Y])->SetSpring(springWristY * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Y])->SetDamper(damperWristY * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_X])->SetSpring(springWristX * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_X])->SetDamper(damperWristX * stiffness);

	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Z])->SetSpring(springWristZ * stiffness);
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Z])->SetDamper(damperWristZ * stiffness);
}

void CRHingeHumanBodyGen::KeepUpperBodyPose(){
	DCAST(PHHingeJointIf, joints[JO_WAIST_ABDOMEN])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_WAIST_ABDOMEN])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_ABDOMEN_CHEST])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_ABDOMEN_CHEST])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Z])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_SHOULDER_Z])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_X])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_SHOULDER_X])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Y])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_SHOULDER_Y])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_ELBOW])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_ELBOW])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Y])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_WRIST_Y])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_X])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_WRIST_X])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Z])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_RIGHT_WRIST_Z])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Z])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_SHOULDER_Z])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_X])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_SHOULDER_X])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Y])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_SHOULDER_Y])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_ELBOW])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_ELBOW])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Y])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_WRIST_Y])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_X])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_WRIST_X])->GetPosition());
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Z])->SetTargetPosition(DCAST(PHHingeJointIf,joints[JO_LEFT_WRIST_Z])->GetPosition());
}

void CRHingeHumanBodyGen::ResetUpperBodyPose(){
	DCAST(PHHingeJointIf, joints[JO_WAIST_ABDOMEN])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_ABDOMEN_CHEST])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Z])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_X])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_SHOULDER_Y])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_ELBOW])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Y])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_X])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_RIGHT_WRIST_Z])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Z])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_X])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_SHOULDER_Y])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_ELBOW])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Y])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_X])->SetTargetPosition(0);
	DCAST(PHHingeJointIf, joints[JO_LEFT_WRIST_Z])->SetTargetPosition(0);
}
}

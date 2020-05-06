/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyGenerator/CRTrunkFootHumanBodyGen.h>
#include <Collision/SprCDShape.h>
#include <Physics/SprPHSdk.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSolid.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRTrunkFootHumanBodyGenDesc::CRTrunkFootHumanBodyGenDesc(){

	jointOrder = PLUG_PARENT;

	waistHeight    = 0.2298;
	waistBreadth   = 0.3067;
	waistThickness = 0.2307;

	chestHeight    = 1.4020 - 1.2253 + 1.2253 - 1.0142;
	chestBreadth   = 0.2887;
	chestThickness = 0.2118;

	neckLength   = 1.7219 - 1.4564 - 0.1732;
	headDiameter = 0.2387;

	footLength       = 0.2544;
	footBreadth      = 0.0994;
	footThickness    = 0.0619;

	springWaistChest   = 100.0;  damperWaistChest   =  50.0;
	springChestHead    = 100.0;  damperChestHead    =  50.0;

	// Vec2d(lower, upper)  lower>upperのとき可動域制限無効
	rangeWaistChest   = Vec2d(Rad(0.0) , Rad(0.01));
	rangeChestHead    = Vec2d(Rad(0.0) , Rad(0.01));
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRTrunkFootHumanBodyGen


// --- --- ---
void CRTrunkFootHumanBodyGen::Init(){
	
}

// --- --- ---
void CRTrunkFootHumanBodyGen::InitBody(){
	CreateWaist();
	CreateChest();
	CreateHead();
}

void CRTrunkFootHumanBodyGen::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass     = 0.17;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	// boxDesc.boxsize = Vec3f(0.2307, 0.2298, 0.3067);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	// solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(0), 'y'));
}

void CRTrunkFootHumanBodyGen::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	// solidDesc.mass   = 0.252;
	solidDesc.mass   = 0.44;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	// boxDesc.boxsize  = Vec3f(0.2, 0.3879, 0.2749);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	//腰部位と上半身の間の関節
	{
		PHHingeJointDesc hingeDesc;
		// hingeDesc.posePlug.Pos() = Vec3d(0,0,0);
		hingeDesc.posePlug.Pos() = Vec3d(0,waistHeight / 2.0,0);
		hingeDesc.posePlug.Ori() = Quaterniond::Rot(Rad(0), 'x');
		// hingeDesc.poseSocket.Pos() = Vec3d(0, -(chestHeight/2.0 + waistHeight/2.0), 0);
		hingeDesc.poseSocket.Pos() = Vec3d(0, -chestHeight/2.0, 0);
		hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');

		hingeDesc.targetPosition =   0.0;
		hingeDesc.spring = 500.0;
		hingeDesc.damper =  50.0;

		joints[JO_WAIST_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_WAIST], hingeDesc);
	}

	/*
	PHBallJointDesc ballDesc;
    ballDesc.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);
	ballDesc.posePlug.Ori() = Quaternionf::Rot(Rad(0), 'x');
	//ballDesc.poseSocket.Pos() = Vec3d(0, -0.32, 0.0);
	ballDesc.poseSocket.Pos() = Vec3d(0, -(chestHeight/2.0 + waistHeight/2.0), 0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'x');
	joints[JO_WAIST_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_WAIST], ballDesc);
	DCAST(PHBallJointIf, joints[JO_WAIST_CHEST])->SetSwingRange(0.01);
	DCAST(PHBallJointIf, joints[JO_WAIST_CHEST])->SetTwistRange(0.0, 0.01);
	*/

	/*
	// Joint -- [p]Waist-[c]Chest
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,  chestHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springWaistChest;
	hingeDesc.damper           = damperWaistChest;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeWaistChest[0];
	hingeDesc.upper            = rangeWaistChest[1];
	joints[JO_WAIST_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_WAIST], hingeDesc);
	*/

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRTrunkFootHumanBodyGen::CreateHead(){
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	// solidDesc.mass    = 0.07;
	solidDesc.mass    = 0.178;
	solids[SO_HEAD]   = phScene->CreateSolid(solidDesc);
	sphereDesc.radius = (float)(headDiameter / 2.0);
	// sphereDesc.radius = 0.2387/2.0;
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(sphereDesc));

	//上半身と頭部の間の関節
	hingeDesc                  = PHHingeJointDesc();
    // hingeDesc.posePlug.Pos()   = Vec3d(0.0, 0.0, 0.0);
    hingeDesc.posePlug.Pos()   = Vec3d(0.0, chestHeight/2.0 + neckLength/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
	// hingeDesc.poseSocket.Pos() = Vec3d(0, -0.36, 0.0);
	// hingeDesc.poseSocket.Pos() = Vec3d(0, -(chestHeight/2.0 + neckLength + headDiameter/2.0), 0.0);
	hingeDesc.poseSocket.Pos() = Vec3d(0, -headDiameter/2.0 - neckLength/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');

	hingeDesc.targetPosition =   0.0;
	hingeDesc.spring = 500.0;
	hingeDesc.damper =  50.0;
	/*
    hingeDesc.lower = 0.0;
	hingeDesc.upper = 0.01;
	*/
    joints[JO_CHEST_HEAD] = CreateJoint(solids[SO_HEAD], solids[SO_CHEST], hingeDesc);

	/*
	// Joint -- [p]Chest-[c]Head
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, chestHeight / 2.0 + neckLength, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos()   = Vec3d(0, -headDiameter, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springChestHead;
	hingeDesc.damper           = damperChestHead;
	hingeDesc.targetPosition           = Rad(0);
	hingeDesc.lower            = rangeChestHead[0];
	hingeDesc.upper            = rangeChestHead[1];
	joints[JO_CHEST_HEAD] = CreateJoint(solids[SO_HEAD], solids[SO_CHEST], hingeDesc);
	*/

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRTrunkFootHumanBodyGen::InitLegs(){
	CreateFoot(LEFTPART);
	CreateFoot(RIGHTPART);
}

void CRTrunkFootHumanBodyGen::CreateFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRTrunkFootHumanBodyGenDesc::CRHumanSolids soNFoot;
	if (lr==LEFTPART) {
		soNFoot = SO_LEFT_FOOT;
	} else {
		soNFoot = SO_RIGHT_FOOT;
	}

	// Solid
	solidDesc.mass   = 0.01;
	solids[soNFoot]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(footBreadth, footThickness, footLength);
    // boxDesc.boxsize = Vec3f(0.2544, 0.0619, 0.0994);
	solids[soNFoot]->AddShape(phSdk->CreateShape(boxDesc));
}

// --- --- ---
void CRTrunkFootHumanBodyGen::InitContact(){
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
		if (DCAST(CRTrunkFootHumanBodyGenIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
	*/
}
}

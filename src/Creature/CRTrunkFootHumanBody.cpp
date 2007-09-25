#include "CRTrunkFootHumanBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRTrunkFootHumanBody
IF_OBJECT_IMP(CRTrunkFootHumanBody, CRBody);

// --- --- ---
void CRTrunkFootHumanBody::Init(){
	CRBody::Init();
}

// --- --- ---
void CRTrunkFootHumanBody::InitBody(){
	CreateWaist();
	CreateChest();
	CreateHead();
}

void CRTrunkFootHumanBody::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass     = 0.17;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(0), 'y'));
}

void CRTrunkFootHumanBody::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass   = 0.252;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Waist-[c]Chest
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,  chestHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springWaistChest;
	hingeDesc.damper           = damperWaistChest;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeWaistChest[0];
	hingeDesc.upper            = rangeWaistChest[1];
	joints[JO_WAIST_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_WAIST], hingeDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRTrunkFootHumanBody::CreateHead(){
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass    = 0.07;
	solids[SO_HEAD]   = phScene->CreateSolid(solidDesc);
	sphereDesc.radius = headDiameter / 2.0;
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(sphereDesc));

	// Joint -- [p]Chest-[c]Head
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, chestHeight / 2.0 + neckLength, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos()   = Vec3d(0, -headDiameter, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springChestHead;
	hingeDesc.damper           = damperChestHead;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeChestHead[0];
	hingeDesc.upper            = rangeChestHead[1];
	joints[JO_CHEST_HEAD] = CreateJoint(solids[SO_HEAD], solids[SO_CHEST], hingeDesc);

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRTrunkFootHumanBody::InitLegs(){
	CreateFoot(LEFTPART);
	CreateFoot(RIGHTPART);
}

void CRTrunkFootHumanBody::CreateFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRTrunkFootHumanBodyDesc::CRHumanSolids soNFoot;
	if (lr==LEFTPART) {
		soNFoot = SO_LEFT_FOOT;
	} else {
		soNFoot = SO_RIGHT_FOOT;
	}

	// Solid
	solidDesc.mass   = 0.01;
	solids[soNFoot]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(footBreadth, footThickness, footLength);
	solids[soNFoot]->AddShape(phSdk->CreateShape(boxDesc));
}

// --- --- ---
void CRTrunkFootHumanBody::InitContact(){
	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	for (int i=0; i<solids.size(); ++i) {
		for (int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i], solids[j], PHSceneDesc::MODE_NONE);
			}
		}
	}

	// 自分以外にすでにBodyが居ればそのBodyに属する剛体とのContactも切る
	for (int i=0; i<creature->NBodies(); ++i) {
		CRBodyIf* body = creature->GetBody(i);
		if (DCAST(CRTrunkFootHumanBodyIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}
}

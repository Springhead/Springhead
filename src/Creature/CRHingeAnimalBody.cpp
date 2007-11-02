#include "CRHingeAnimalBody.h"

#define GRAVITYMODE 1

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeAnimalBody
IF_OBJECT_IMP(CRHingeAnimalBody, CRBody);
	
// --- --- ---
void CRHingeAnimalBody::Init(){
	CRBody::Init();
}

// --- --- ---
void CRHingeAnimalBody::InitBody(){
	CreateWaist();
	CreateChest();
	CreateTail();
}

void CRHingeAnimalBody::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	// define the solid(it has no shape)
	solidDesc.mass     = 0.17;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	// define the shape of the solid.
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	// define the position.
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(-90), 'x'));

	//重力切るか
#if GRAVITYMODE
	solids[SO_WAIST]->SetFramePosition(Vec3f(0.0, 1.5, 0.0));
	solids[SO_WAIST]->SetDynamical(false);
#endif
}

void CRHingeAnimalBody::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass   = 0.252;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Waist-[c]Chest
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0, waistHeight / 2.0, 0);
	ballDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'y');
	ballDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'y');
	ballDesc.spring           = springWaistChest;
	ballDesc.damper           = damperWaistChest;
	ballDesc.origin           = Quaterniond::Rot(Rad(0), 'y');										//Quotaniondのoriginってどうやって指定するの？
	ballDesc.swingUpper       = rangeWaistChest;
	joints[JO_WAIST_CHEST]  = CreateJoint(solids[SO_CHEST], solids[SO_WAIST], ballDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeAnimalBody::CreateTail(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solids
	// define the existance
	solidDesc.mass	= 0.01;
	solids[SO_TAIL1] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL2] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL3] = phScene->CreateSolid(solidDesc);
	
	// define the shape
	boxDesc.boxsize = Vec3f(tailBreath, tailHeight, tailThickness);
	solids[SO_TAIL1]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL2]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL3]->AddShape(phSdk->CreateShape(boxDesc));

	// define the connection
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -waistHeight/2.0, 0.1);
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.spring            = springTailWaist;
	ballDesc.damper            = damperTailWaist;
//	ballDesc.origin            = Rad(0);
	ballDesc.swingUpper        = rangeTailWaist;
	joints[JO_TAIL_WAIST]	   = CreateJoint(solids[SO_TAIL1], solids[SO_WAIST], ballDesc);

	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
//	ballDesc.origin            = Rad(0);
	ballDesc.swingUpper        = rangeTail;
	joints[JO_TAIL_12]		   = CreateJoint(solids[SO_TAIL2], solids[SO_TAIL1], ballDesc);
	
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
//	ballDesc.origin            = Rad(0);
	ballDesc.swingUpper        = rangeTail;
	joints[JO_TAIL_23]		   = CreateJoint(solids[SO_TAIL3], solids[SO_TAIL2], ballDesc);

	// define the position
	
	//phScene->SetContactMode(solids[SO_TAIL1], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeAnimalBody::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRHingeAnimalBody::CreateNeck(){
	
}

void CRHingeAnimalBody::CreateHead(){
	
}

// --- --- ---
void CRHingeAnimalBody::InitFrontLegs(){
	CreateBreastBone(LEFTPART);
	CreateRadius(LEFTPART);
	CreateFrontCannonBone(LEFTPART);
	CreateFrontToeBones(LEFTPART);

	CreateBreastBone(RIGHTPART);
	CreateRadius(RIGHTPART);
	CreateFrontCannonBone(RIGHTPART);
	CreateFrontToeBones(RIGHTPART);

}

void CRHingeAnimalBody::CreateBreastBone(LREnum lr){
	
}

void CRHingeAnimalBody::CreateRadius(LREnum lr){
	
}

void CRHingeAnimalBody::CreateFrontCannonBone(LREnum lr){
	
}

void CRHingeAnimalBody::CreateFrontToeBones(LREnum lr){

}


// --- --- ---
void CRHingeAnimalBody::InitRearLegs(){
	if (!noLegs) {
		CreateFemur(LEFTPART);
		CreateTibia(LEFTPART);
	}
	CreateRearCannonBone(LEFTPART);
	CreateRearToeBones(LEFTPART);

	if (!noLegs) {
		CreateFemur(RIGHTPART);
		CreateTibia(RIGHTPART);
	}
	CreateRearCannonBone(RIGHTPART);
	CreateRearToeBones(RIGHTPART);

	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	phScene->SetContactMode(solids[SO_LEFT_FEMUR], solids[SO_RIGHT_FEMUR], PHSceneDesc::MODE_NONE);
}

void CRHingeAnimalBody::CreateFemur(LREnum lr){

}

void CRHingeAnimalBody::CreateTibia(LREnum lr){
	
}

void CRHingeAnimalBody::CreateRearCannonBone(LREnum lr){

}

void CRHingeAnimalBody::CreateRearToeBones(LREnum lr){

}
// --- --- ---
void CRHingeAnimalBody::InitEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRHingeAnimalBody::CreateEye(LREnum lr){
	
}

// --- --- ---
void CRHingeAnimalBody::InitContact(){
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
		if (DCAST(CRHingeAnimalBodyIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}
}

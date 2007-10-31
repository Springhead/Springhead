#include "CRHingeMammalBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeMammalBody
IF_OBJECT_IMP(CRHingeMammalBody, CRBody);

// --- --- ---
void CRHingeMammalBody::Init(){
	CRBody::Init();
}

// --- --- ---
void CRHingeMammalBody::InitBody(){
	CreateWaist();
	CreateAbdomen();
	CreateChest();
	CreateTail();
}

void CRHingeMammalBody::CreateWaist(){
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
	solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(0), 'y'));
}

void CRHingeMammalBody::CreateAbdomen(){
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
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -abdomenHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springWaistAbdomen;
	hingeDesc.damper           = damperWaistAbdomen;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeWaistAbdomen[0];
	hingeDesc.upper            = rangeWaistAbdomen[1];
	joints[JO_WAIST_ABDOMEN] = CreateJoint(solids[SO_ABDOMEN], solids[SO_WAIST], hingeDesc);

	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateChest(){
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
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springAbdomenChest;
	hingeDesc.damper           = damperAbdomenChest;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeAbdomenChest[0];
	hingeDesc.upper            = rangeAbdomenChest[1];
	joints[JO_ABDOMEN_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_ABDOMEN], hingeDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_ABDOMEN], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateTail(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solids
	// define the existance
	solidDesc.mass	= 0.01;
	solids[SO_TAIL1] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL2] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL3] = phScene->CreateSolid(solidDesc);
	// define the shape
	boxDesc.boxsize = Vec3f();
	solids[SO_TAIL1]->AddShape(phSdk->CreateShape(boxDesc));
	boxDesc.boxsize = Vec3f();
	solids[SO_TAIL2]->AddShape(phSdk->CreateShape(boxDesc));
	boxDesc.boxsize = Vec3f();
	solids[SO_TAIL3]->AddShape(phSdk->CreateShape(boxDesc));
	// define the connection
	// define the position
	

}

// --- --- ---
void CRHingeMammalBody::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRHingeMammalBody::CreateNeck(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass  = 0.028;
	solids[SO_NECK] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize = Vec3f(neckDiameter/1.414, neckLength, neckDiameter/1.414);
	solids[SO_NECK]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[SO_CHEST_NECK_XZ] = phScene->CreateSolid(solidDesc);
	solids[SO_CHEST_NECK_ZY] = phScene->CreateSolid(solidDesc);

	// Joint -- [p]Chest-[c]Neck
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, chestHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springChestNeckX;
	hingeDesc.damper           = damperChestNeckX;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeChestNeckX[0];
	hingeDesc.upper            = rangeChestNeckX[1];
	joints[JO_CHEST_NECK_X] = CreateJoint(solids[SO_CHEST_NECK_XZ], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springChestNeckZ;
	hingeDesc.damper           = damperChestNeckZ;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeChestNeckZ[0];
	hingeDesc.upper            = rangeChestNeckZ[1];
	joints[JO_CHEST_NECK_Z] = CreateJoint(solids[SO_CHEST_NECK_ZY], solids[SO_CHEST_NECK_XZ], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springChestNeckY;
	hingeDesc.damper           = damperChestNeckY;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeChestNeckY[0];
	hingeDesc.upper            = rangeChestNeckY[1];
	joints[JO_CHEST_NECK_Y] = CreateJoint(solids[SO_NECK], solids[SO_CHEST_NECK_ZY], hingeDesc);

	phScene->SetContactMode(solids[SO_NECK], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateHead(){
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	// Solid
	solidDesc.mass    = 0.07;
	solids[SO_HEAD]   = phScene->CreateSolid(solidDesc);
	sphereDesc.radius = headDiameter / 2.0;
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(sphereDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[SO_NECK_HEAD_XZ] = phScene->CreateSolid(solidDesc);

	// Joint -- [p]Neck-[c]Head
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, neckLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springNeckHeadX;
	hingeDesc.damper           = damperNeckHeadX;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeNeckHeadX[0];
	hingeDesc.upper            = rangeNeckHeadX[1];
	joints[JO_NECK_HEAD_X] = CreateJoint(solids[SO_NECK_HEAD_XZ], solids[SO_NECK], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -headDiameter / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springNeckHeadZ;
	hingeDesc.damper           = damperNeckHeadZ;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeNeckHeadZ[0];
	hingeDesc.upper            = rangeNeckHeadZ[1];
	joints[JO_NECK_HEAD_Z] = CreateJoint(solids[SO_HEAD], solids[SO_NECK_HEAD_XZ], hingeDesc);

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeMammalBody::InitFrontLegs(){
	CreateFrontUpperLeg(LEFTPART);
	CreateFrontLowerLeg(LEFTPART);
	CreateFrontFoot(LEFTPART);

	CreateFrontUpperLeg(RIGHTPART);
	CreateFrontLowerLeg(RIGHTPART);
	CreateFrontFoot(RIGHTPART);
}

void CRHingeMammalBody::CreateFrontUpperLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNShoulderZX, soNShoulderXY, soNUpperArm;
	CRHingeMammalBodyDesc::CRMammalJoints joNShoulderZ, joNShoulderX, joNShoulderY;
	if (lr==LEFTPART) {
		soNShoulderZX = SO_LEFT_SHOULDER_ZX; soNShoulderXY = SO_LEFT_SHOULDER_XY; soNUpperArm   = SO_LEFT_FRONT_UPPER_LEG;
		joNShoulderZ  = JO_LEFT_SHOULDER_Z;  joNShoulderX  = JO_LEFT_SHOULDER_X;  joNShoulderY  = JO_LEFT_SHOULDER_Y;
	} else {
		soNShoulderZX = SO_RIGHT_SHOULDER_ZX; soNShoulderXY = SO_RIGHT_SHOULDER_XY; soNUpperArm   = SO_RIGHT_FRONT_UPPER_LEG;
		joNShoulderZ  = JO_RIGHT_SHOULDER_Z;  joNShoulderX  = JO_RIGHT_SHOULDER_X;  joNShoulderY  = JO_RIGHT_SHOULDER_Y;
	}

	// Solid
	solidDesc.mass      = 0.04;
	solids[soNUpperArm] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(frontUpperLegDiameter, frontUpperLegLength, frontUpperLegDiameter);
	solids[soNUpperArm]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNShoulderZX] = phScene->CreateSolid(solidDesc);
	solids[soNShoulderXY] = phScene->CreateSolid(solidDesc);

	// Joint -- Shoulder ([p]Chest-[c]UpperArm)
	hingeDesc                  = PHHingeJointDesc();
	if (posRightFrontUpperLeg==Vec3d(0,0,0)) {
		hingeDesc.posePlug.Pos() = Vec3d(lr*chestBreadth/2.0 + lr*(frontUpperLegDiameter/2.0*1.414), chestHeight/2.0, 0);
	} 
	else {
		hingeDesc.posePlug.Pos() = Vec3d(lr*posRightFrontUpperLeg.x, posRightFrontUpperLeg.y+chestHeight/2.0, posRightFrontUpperLeg.z);
	}
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springShoulderZ;
	hingeDesc.damper           = damperShoulderZ;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeShoulderZ[0]) : (-rangeShoulderZ[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeShoulderZ[1]) : (-rangeShoulderZ[0]));
	joints[joNShoulderZ]       = CreateJoint(solids[soNShoulderZX], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springShoulderX;
	hingeDesc.damper           = damperShoulderX;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeShoulderX[0];
	hingeDesc.upper            = rangeShoulderX[1];
	joints[joNShoulderX]       = CreateJoint(solids[soNShoulderXY], solids[soNShoulderZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, frontUpperLegLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightFrontUpperLeg.w, lr*oriRightFrontUpperLeg.x, oriRightFrontUpperLeg.y, oriRightFrontUpperLeg.z).Inv() * Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springShoulderY;
	hingeDesc.damper           = damperShoulderY;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeShoulderY[0]) : (-rangeShoulderY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeShoulderY[1]) : (-rangeShoulderY[0]));
	joints[joNShoulderY]       = CreateJoint(solids[soNUpperArm], solids[soNShoulderXY], hingeDesc);

	phScene->SetContactMode(solids[soNUpperArm], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateFrontLowerLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNUpperArm, soNLowerArm;
	CRHingeMammalBodyDesc::CRMammalJoints joNElbow;
	if (lr==LEFTPART) {
		soNUpperArm = SO_LEFT_FRONT_UPPER_LEG; soNLowerArm = SO_LEFT_FRONT_LOWER_LEG;
		joNElbow = JO_LEFT_ELBOW;
	} else {
		soNUpperArm = SO_RIGHT_FRONT_UPPER_LEG; soNLowerArm = SO_RIGHT_FRONT_LOWER_LEG;
		joNElbow = JO_RIGHT_ELBOW;
	}

	// Solid
	solidDesc.mass      = 0.02;
	solids[soNLowerArm] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(frontLowerLegDiameter, frontLowerLegLength, frontLowerLegDiameter);
	solids[soNLowerArm]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Elbow ([p]UpperArm-[c]LowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -frontUpperLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, frontLowerLegLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightFrontLowerLeg.w, lr*oriRightFrontLowerLeg.x, oriRightFrontLowerLeg.y, oriRightFrontLowerLeg.z).Inv() * Quaternionf::Rot(Rad(0), 'y');
	hingeDesc.spring           = springElbow;
	hingeDesc.damper           = damperElbow;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeElbow[0]) : (-rangeElbow[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeElbow[1]) : (-rangeElbow[0]));
	joints[joNElbow] = CreateJoint(solids[soNLowerArm], solids[soNUpperArm], hingeDesc);

	phScene->SetContactMode(solids[soNLowerArm], solids[soNUpperArm], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateFrontFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNLowerArm, soNWristYX, soNWristXZ, soNHand;
	CRHingeMammalBodyDesc::CRMammalJoints joNWristY, joNWristX, joNWristZ;
	if (lr==LEFTPART) {
		soNLowerArm = SO_LEFT_FRONT_LOWER_LEG; soNWristYX = SO_LEFT_FRONT_ANKLE_YX; soNWristXZ = SO_LEFT_FRONT_ANKLE_XZ; soNHand = SO_LEFT_FRONT_FOOT;
		joNWristY = JO_LEFT_FRONT_ANKLE_Y; joNWristX = JO_LEFT_FRONT_ANKLE_X; joNWristZ = JO_LEFT_FRONT_ANKLE_Z; 
	} else {
		soNLowerArm = SO_RIGHT_FRONT_LOWER_LEG; soNWristYX = SO_RIGHT_FRONT_ANKLE_YX; soNWristXZ = SO_RIGHT_FRONT_ANKLE_XZ; soNHand = SO_RIGHT_FRONT_FOOT;
		joNWristY = JO_RIGHT_FRONT_ANKLE_Y; joNWristX = JO_RIGHT_FRONT_ANKLE_X; joNWristZ = JO_RIGHT_FRONT_ANKLE_Z; 
	}

	// Solid
	solidDesc.mass   = 0.01;
	solids[soNHand]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(frontFootBreadth, frontFootLength, frontFootThickness);
	solids[soNHand]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNWristYX] = phScene->CreateSolid(solidDesc);
	solids[soNWristXZ] = phScene->CreateSolid(solidDesc);

	// Joint -- Wrist ([p]LowerArm-[c]Hand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -frontLowerLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springFrontAnkleY;
	hingeDesc.damper           = damperFrontAnkleY;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeFrontAnkleY[0]) : (-rangeFrontAnkleY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeFrontAnkleY[1]) : (-rangeFrontAnkleY[0]));
	joints[joNWristY]          = CreateJoint(solids[soNWristYX], solids[soNLowerArm], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springFrontAnkleX;
	hingeDesc.damper           = damperFrontAnkleX;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeFrontAnkleX[0];
	hingeDesc.upper            = rangeFrontAnkleX[1];
	joints[joNWristX]          = CreateJoint(solids[soNWristXZ], solids[soNWristYX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, frontFootLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightHand.w, lr*oriRightHand.x, oriRightHand.y, oriRightHand.z).Inv() * Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springFrontAnkleZ;
	hingeDesc.damper           = damperFrontAnkleZ;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeFrontAnkleZ[0]) : (-rangeFrontAnkleZ[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeFrontAnkleZ[1]) : (-rangeFrontAnkleZ[0]));
	joints[joNWristZ]          = CreateJoint(solids[soNHand], solids[soNWristXZ], hingeDesc);

	phScene->SetContactMode(solids[soNHand], solids[soNLowerArm], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeMammalBody::InitRearLegs(){
	if (!noLegs) {
		CreateRearUpperLeg(LEFTPART);
		CreateRearLowerLeg(LEFTPART);
	}
	CreateRearFoot(LEFTPART);

	if (!noLegs) {
		CreateRearUpperLeg(RIGHTPART);
		CreateRearLowerLeg(RIGHTPART);
	}
	CreateRearFoot(RIGHTPART);

	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	phScene->SetContactMode(solids[SO_LEFT_REAR_UPPER_LEG], solids[SO_RIGHT_REAR_UPPER_LEG], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateRearUpperLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNWaistLegZX, soNWaistLegXY, soNUpperLeg;
	CRHingeMammalBodyDesc::CRMammalJoints joNWaistLegZ, joNWaistLegX, joNWaistLegY;
	if (lr==LEFTPART) {
		soNWaistLegZX = SO_LEFT_WAIST_LEG_ZX; soNWaistLegXY = SO_LEFT_WAIST_LEG_XY; soNUpperLeg   = SO_LEFT_REAR_UPPER_LEG;
		joNWaistLegZ  = JO_LEFT_WAIST_LEG_Z;  joNWaistLegX  = JO_LEFT_WAIST_LEG_X;  joNWaistLegY  = JO_LEFT_WAIST_LEG_Y;
	} 
	else {
		soNWaistLegZX = SO_RIGHT_WAIST_LEG_ZX; soNWaistLegXY = SO_RIGHT_WAIST_LEG_XY; soNUpperLeg   = SO_RIGHT_REAR_UPPER_LEG;
		joNWaistLegZ  = JO_RIGHT_WAIST_LEG_Z;  joNWaistLegX  = JO_RIGHT_WAIST_LEG_X;  joNWaistLegY  = JO_RIGHT_WAIST_LEG_Y;
	}

	// Solid
	solidDesc.mass      = 0.06;
	solids[soNUpperLeg] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(rearUpperLegDiameter, rearUpperLegLength, rearUpperLegDiameter);
	solids[soNUpperLeg]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNWaistLegZX] = phScene->CreateSolid(solidDesc);
	solids[soNWaistLegXY] = phScene->CreateSolid(solidDesc);

	// Joint -- WaistLeg ([p]Waist-[c]UpperLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(lr*rearInterLegDistance/2.0, -waistHeight/2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springWaistLegZ;
	hingeDesc.damper           = damperWaistLegZ;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeWaistLegZ[0]) : (-rangeWaistLegZ[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeWaistLegZ[1]) : (-rangeWaistLegZ[0]));
	joints[joNWaistLegZ]       = CreateJoint(solids[soNWaistLegZX], solids[SO_WAIST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springWaistLegX;
	hingeDesc.damper           = damperWaistLegX;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeWaistLegX[0];
	hingeDesc.upper            = rangeWaistLegX[1];
	joints[joNWaistLegX]       = CreateJoint(solids[soNWaistLegXY], solids[soNWaistLegZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, rearUpperLegLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springWaistLegY;
	hingeDesc.damper           = damperWaistLegY;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeWaistLegY[0]) : (-rangeWaistLegY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeWaistLegY[1]) : (-rangeWaistLegY[0]));
	joints[joNWaistLegY]       = CreateJoint(solids[soNUpperLeg], solids[soNWaistLegXY], hingeDesc);

	phScene->SetContactMode(solids[soNUpperLeg], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateRearLowerLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNUpperLeg, soNLowerLeg;
	CRHingeMammalBodyDesc::CRMammalJoints joNKnee;
	if (lr==LEFTPART) {
		soNUpperLeg = SO_LEFT_REAR_UPPER_LEG; soNLowerLeg = SO_LEFT_REAR_LOWER_LEG;
		joNKnee = JO_LEFT_KNEE;
	} else {
		soNUpperLeg = SO_RIGHT_REAR_UPPER_LEG; soNLowerLeg = SO_RIGHT_REAR_LOWER_LEG;
		joNKnee = JO_RIGHT_KNEE;
	}

	// Solid
	solidDesc.mass      = 0.05;
	solids[soNLowerLeg] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize     = Vec3f(rearLowerLegDiameter, rearLowerLegLength, rearLowerLegDiameter);
	solids[soNLowerLeg]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- Knee ([p]UpperLeg-[c]LowerLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -rearUpperLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, rearLowerLegLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springKnee;
	hingeDesc.damper           = damperKnee;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeKnee[0];
	hingeDesc.upper            = rangeKnee[1];
	joints[joNKnee] = CreateJoint(solids[soNLowerLeg], solids[soNUpperLeg], hingeDesc);

	phScene->SetContactMode(solids[soNLowerLeg], solids[soNUpperLeg], PHSceneDesc::MODE_NONE);
}

void CRHingeMammalBody::CreateRearFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNLowerLeg, soNAnkleYX, soNAnkleXZ, soNFoot;
	CRHingeMammalBodyDesc::CRMammalJoints joNAnkleY, joNAnkleX, joNAnkleZ;
	if (lr==LEFTPART) {
		soNLowerLeg = SO_LEFT_REAR_LOWER_LEG; soNAnkleYX = SO_LEFT_REAR_ANKLE_YX; soNAnkleXZ = SO_LEFT_REAR_ANKLE_XZ; soNFoot = SO_LEFT_REAR_FOOT;
		joNAnkleY = JO_LEFT_REAR_ANKLE_Y; joNAnkleX = JO_LEFT_REAR_ANKLE_X; joNAnkleZ = JO_LEFT_REAR_ANKLE_Z; 
	} else {
		soNLowerLeg = SO_RIGHT_REAR_LOWER_LEG; soNAnkleYX = SO_RIGHT_REAR_ANKLE_YX; soNAnkleXZ = SO_RIGHT_REAR_ANKLE_XZ; soNFoot = SO_RIGHT_REAR_FOOT;
		joNAnkleY = JO_RIGHT_REAR_ANKLE_Y; joNAnkleX = JO_RIGHT_REAR_ANKLE_X; joNAnkleZ = JO_RIGHT_REAR_ANKLE_Z; 
	}

	// Solid
	solidDesc.mass   = 0.01;
	solids[soNFoot]  = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(rearFootBreadth, rearFootThickness, rearFootLength);
	solids[soNFoot]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNAnkleYX] = phScene->CreateSolid(solidDesc);
	solids[soNAnkleXZ] = phScene->CreateSolid(solidDesc);

	if (!noLegs) {
		// Joint -- Ankle ([p]LowerLeg-[c]Foot)
		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Pos()   = Vec3d(0.0, -rearLowerLegLength / 2.0, 0);
		hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
		hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
		hingeDesc.spring           = springRearAnkleY;
		hingeDesc.damper           = damperRearAnkleY;
		hingeDesc.origin           = Rad(0);
		hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeRearAnkleY[0]) : (-rangeRearAnkleY[1]));
		hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeRearAnkleY[1]) : (-rangeRearAnkleY[0]));
		joints[joNAnkleY]          = CreateJoint(solids[soNAnkleYX], solids[soNLowerLeg], hingeDesc);

		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
		hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
		hingeDesc.spring           = springRearAnkleX;
		hingeDesc.damper           = damperRearAnkleX;
		hingeDesc.origin           = Rad(0);
		hingeDesc.lower            = rangeRearAnkleX[0];
		hingeDesc.upper            = rangeRearAnkleX[1];
		joints[joNAnkleX]          = CreateJoint(solids[soNAnkleXZ], solids[soNAnkleYX], hingeDesc);

		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
		hingeDesc.poseSocket.Pos() = Vec3d(0, rearFootThickness / 2.0, (rearAnkleToeDistance - rearFootLength/2.0));
		hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
		hingeDesc.spring           = springRearAnkleZ;
		hingeDesc.damper           = damperRearAnkleZ;
		hingeDesc.origin           = Rad(0);
		hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeRearAnkleZ[0]) : (-rangeRearAnkleZ[1]));
		hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeRearAnkleZ[1]) : (-rangeRearAnkleZ[0]));
		joints[joNAnkleZ]          = CreateJoint(solids[soNFoot], solids[soNAnkleXZ], hingeDesc);
	}

	phScene->SetContactMode(solids[soNFoot], solids[soNLowerLeg], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeMammalBody::InitEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRHingeMammalBody::CreateEye(LREnum lr){
	CDBoxDesc          boxDesc;
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeMammalBodyDesc::CRMammalSolids soNEyeYX, soNEye;
	CRHingeMammalBodyDesc::CRMammalJoints joNEyeY, joNEyeX;
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
	sphereDesc.radius  = eyeDiameter;
	solids[soNEye]->AddShape(phSdk->CreateShape(sphereDesc));
	boxDesc.boxsize    = Vec3f(0.015,0.015,eyeDiameter+0.029);
	solids[soNEye]->AddShape(phSdk->CreateShape(boxDesc));

	// non-shaped Solid
	solidDesc.mass    = 0.001;
	solidDesc.inertia = Matrix3d::Unit() * 0.001;
	solids[soNEyeYX] = phScene->CreateSolid(solidDesc);

	// Joint -- Eye ([p]Head-[c]Eye)
	hingeDesc                  = PHHingeJointDesc();
	//hingeDesc.posePlug.Pos()   = Vec3d(lr*interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -occiputToEyeDistance+headDiameter/2.0);
	hingeDesc.posePlug.Pos()   = Vec3d(lr*interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -headDiameter/2.0+eyeDiameter/2.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springEyeY;
	hingeDesc.damper           = damperEyeY;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = ((lr==RIGHTPART) ? (rangeRightEyeY[0]) : (-rangeRightEyeY[1]));
	hingeDesc.upper            = ((lr==RIGHTPART) ? (rangeRightEyeY[1]) : (-rangeRightEyeY[0]));
	joints[joNEyeY]            = CreateJoint(solids[soNEyeYX], solids[SO_HEAD], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springEyeX;
	hingeDesc.damper           = damperEyeX;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = rangeEyeX[0];
	hingeDesc.upper            = rangeEyeX[1];
	joints[joNEyeX] = CreateJoint(solids[soNEye], solids[soNEyeYX], hingeDesc);

	phScene->SetContactMode(solids[soNEye], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeMammalBody::InitContact(){
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
		if (DCAST(CRHingeMammalBodyIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}
}

#include "CRHingeHuman.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeHuman
IF_OBJECT_IMP(CRHingeHuman, SceneObject);

// --- --- ---

PHJointIf* CRHingeHuman::CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc){
	PHJointIf* joint;
	if (jointOrder == PLUG_PARENT) {
		joint = phScene->CreateJoint(soChild, soParent, desc);
	} else { // SOCKET_PARENT
		Posed pp=desc.posePlug, ps=desc.poseSocket;
		desc.posePlug=ps; desc.poseSocket=pp;
		joint = phScene->CreateJoint(soParent, soChild, desc);
	}
	return joint;
}

// --- --- ---

void CRHingeHuman::Init(){
	phScene = DCAST(PHSceneIf, GetScene());
	phSdk   = DCAST(PHSdkIf, phScene->GetSdk());

	InitBody();
	InitHead();
	InitArms();
	InitEyes();
	InitLegs();

	// FeatherStone法の使用（暫定）
	/*
	PHTreeNodeIf* rhand   = phScene->CreateRootNode(solids[SO_RIGHT_HAND], PHRootNodeDesc());
	PHTreeNodeIf* rwrixz  = phScene->CreateTreeNode(rhand, solids[SO_RIGHT_WRIST_XZ], PHTreeNodeDesc());
	PHTreeNodeIf* rwriyx  = phScene->CreateTreeNode(rwrixz, solids[SO_RIGHT_WRIST_YX], PHTreeNodeDesc());
	PHTreeNodeIf* rlarm   = phScene->CreateTreeNode(rwriyx, solids[SO_RIGHT_LOWER_ARM], PHTreeNodeDesc());
	PHTreeNodeIf* ruarm   = phScene->CreateTreeNode(rlarm, solids[SO_RIGHT_UPPER_ARM], PHTreeNodeDesc());
	PHTreeNodeIf* rshldxy = phScene->CreateTreeNode(ruarm, solids[SO_RIGHT_SHOULDER_XY], PHTreeNodeDesc());
	PHTreeNodeIf* rshldzx = phScene->CreateTreeNode(rshldxy, solids[SO_RIGHT_SHOULDER_ZX], PHTreeNodeDesc());
	PHTreeNodeIf* rchest  = phScene->CreateTreeNode(rshldzx, solids[SO_CHEST], PHTreeNodeDesc());
	*/

	/*
	PHTreeNodeIf* waist   = phScene->CreateRootNode(solids[SO_WAIST], PHRootNodeDesc());
	PHTreeNodeIf* abdomen = phScene->CreateTreeNode(waist, solids[SO_ABDOMEN], PHTreeNodeDesc());
	PHTreeNodeIf* chest   = phScene->CreateTreeNode(abdomen, solids[SO_CHEST], PHTreeNodeDesc());
	PHTreeNodeIf* neckxz  = phScene->CreateTreeNode(chest, solids[SO_CHEST_NECK_XZ], PHTreeNodeDesc());
	PHTreeNodeIf* neckzy  = phScene->CreateTreeNode(neckxz, solids[SO_CHEST_NECK_ZY], PHTreeNodeDesc());
	PHTreeNodeIf* neck    = phScene->CreateTreeNode(neckzy, solids[SO_NECK], PHTreeNodeDesc());
	PHTreeNodeIf* headxz  = phScene->CreateTreeNode(neck, solids[SO_NECK_HEAD_XZ], PHTreeNodeDesc());
	PHTreeNodeIf* head    = phScene->CreateTreeNode(neck, solids[SO_HEAD], PHTreeNodeDesc());

	PHTreeNodeIf* rshldzx = phScene->CreateTreeNode(chest, solids[SO_RIGHT_SHOULDER_ZX], PHTreeNodeDesc());
	PHTreeNodeIf* rshldxy = phScene->CreateTreeNode(rshldzx, solids[SO_RIGHT_SHOULDER_XY], PHTreeNodeDesc());
	PHTreeNodeIf* ruarm   = phScene->CreateTreeNode(rshldxy, solids[SO_RIGHT_UPPER_ARM], PHTreeNodeDesc());
	PHTreeNodeIf* rlarm   = phScene->CreateTreeNode(ruarm, solids[SO_RIGHT_LOWER_ARM], PHTreeNodeDesc());
	PHTreeNodeIf* rwriyx  = phScene->CreateTreeNode(rlarm, solids[SO_RIGHT_WRIST_YX], PHTreeNodeDesc());
	PHTreeNodeIf* rwrixz  = phScene->CreateTreeNode(rwriyx, solids[SO_RIGHT_WRIST_XZ], PHTreeNodeDesc());
	PHTreeNodeIf* rhand   = phScene->CreateTreeNode(rwrixz, solids[SO_RIGHT_HAND], PHTreeNodeDesc());

	PHTreeNodeIf* lshldzx = phScene->CreateTreeNode(chest, solids[SO_LEFT_SHOULDER_ZX], PHTreeNodeDesc());
	PHTreeNodeIf* lshldxy = phScene->CreateTreeNode(lshldzx, solids[SO_LEFT_SHOULDER_XY], PHTreeNodeDesc());
	PHTreeNodeIf* luarm   = phScene->CreateTreeNode(lshldxy, solids[SO_LEFT_UPPER_ARM], PHTreeNodeDesc());
	PHTreeNodeIf* llarm   = phScene->CreateTreeNode(luarm, solids[SO_LEFT_LOWER_ARM], PHTreeNodeDesc());
	PHTreeNodeIf* lwriyx  = phScene->CreateTreeNode(llarm, solids[SO_LEFT_WRIST_YX], PHTreeNodeDesc());
	PHTreeNodeIf* lwrixz  = phScene->CreateTreeNode(lwriyx, solids[SO_LEFT_WRIST_XZ], PHTreeNodeDesc());
	PHTreeNodeIf* lhand   = phScene->CreateTreeNode(lwrixz, solids[SO_LEFT_HAND], PHTreeNodeDesc());
	*/
}

// --- --- ---
void CRHingeHuman::InitBody(){
	CreateWaist();
	CreateAbdomen();
	CreateChest();
}

void CRHingeHuman::CreateWaist(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;

	// Solid
	solidDesc.mass     = 0.17;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	//solids[SO_WAIST]->SetDynamical(false);
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(0), 'y'));
}

void CRHingeHuman::CreateAbdomen(){
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
	//hingeDesc.lower            = -0.0902;
	//hingeDesc.upper            = 1.7915;
	joints[JO_WAIST_ABDOMEN] = CreateJoint(solids[SO_ABDOMEN], solids[SO_WAIST], hingeDesc);

	phScene->SetContactMode(solids[SO_ABDOMEN], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateChest(){
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
	//hingeDesc.lower            = -0.8552;
	//hingeDesc.upper            = 0.8552;
	joints[JO_ABDOMEN_CHEST] = CreateJoint(solids[SO_CHEST], solids[SO_ABDOMEN], hingeDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_ABDOMEN], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHuman::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRHingeHuman::CreateNeck(){
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
	//hingeDesc.lower            = -1.7628;
	//hingeDesc.upper            = 1.2217;
	joints[JO_CHEST_NECK_X] = CreateJoint(solids[SO_CHEST_NECK_XZ], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springChestNeckZ;
	hingeDesc.damper           = damperChestNeckZ;
	hingeDesc.origin           = Rad(0);
	joints[JO_CHEST_NECK_Z] = CreateJoint(solids[SO_CHEST_NECK_ZY], solids[SO_CHEST_NECK_XZ], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springChestNeckY;
	hingeDesc.damper           = damperChestNeckY;
	hingeDesc.origin           = Rad(0);
	//hingeDesc.lower            = -1.0036;
	//hingeDesc.upper            = 1.0036;
	joints[JO_CHEST_NECK_Y] = CreateJoint(solids[SO_NECK], solids[SO_CHEST_NECK_ZY], hingeDesc);

	phScene->SetContactMode(solids[SO_NECK], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateHead(){
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
	joints[JO_NECK_HEAD_X] = CreateJoint(solids[SO_NECK_HEAD_XZ], solids[SO_NECK], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -headDiameter / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springNeckHeadZ;
	hingeDesc.damper           = damperNeckHeadZ;
	hingeDesc.origin           = Rad(0);
	joints[JO_NECK_HEAD_Z] = CreateJoint(solids[SO_HEAD], solids[SO_NECK_HEAD_XZ], hingeDesc);

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHuman::InitArms(){
	CreateUpperArm(LEFTPART);
	CreateLowerArm(LEFTPART);
	CreateHand(LEFTPART);

	CreateUpperArm(RIGHTPART);
	CreateLowerArm(RIGHTPART);
	CreateHand(RIGHTPART);
}

void CRHingeHuman::CreateUpperArm(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNShoulderZX, soNShoulderXY, soNUpperArm;
	CRHingeHumanDesc::CRHumanJoints joNShoulderZ, joNShoulderX, joNShoulderY;
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
	solidDesc.mass    = 0.04;
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
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springShoulderZ;
	hingeDesc.damper           = damperShoulderZ;
	hingeDesc.origin           = Rad(0);
	joints[joNShoulderZ]       = CreateJoint(solids[soNShoulderZX], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springShoulderX;
	hingeDesc.damper           = damperShoulderX;
	hingeDesc.origin           = Rad(0);
	joints[joNShoulderX]       = CreateJoint(solids[soNShoulderXY], solids[soNShoulderZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, upperArmLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightUpperArm.w, lr*oriRightUpperArm.x, oriRightUpperArm.y, oriRightUpperArm.z).Inv() * Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springShoulderY;
	hingeDesc.damper           = damperShoulderY;
	hingeDesc.origin           = Rad(0);
	joints[joNShoulderY]       = CreateJoint(solids[soNUpperArm], solids[soNShoulderXY], hingeDesc);

	phScene->SetContactMode(solids[soNUpperArm], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateLowerArm(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNUpperArm, soNLowerArm;
	CRHingeHumanDesc::CRHumanJoints joNElbow;
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
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerArmLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightLowerArm.w, lr*oriRightLowerArm.x, oriRightLowerArm.y, oriRightLowerArm.z).Inv() * Quaternionf::Rot(Rad(0), 'y');
	hingeDesc.spring           = springElbow;
	hingeDesc.damper           = damperElbow;
	hingeDesc.origin           = Rad(0);
	joints[joNElbow] = CreateJoint(solids[soNLowerArm], solids[soNUpperArm], hingeDesc);

	phScene->SetContactMode(solids[soNLowerArm], solids[soNUpperArm], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateHand(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNLowerArm, soNWristYX, soNWristXZ, soNHand;
	CRHingeHumanDesc::CRHumanJoints joNWristY, joNWristX, joNWristZ;
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
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNWristYX] = phScene->CreateSolid(solidDesc);
	solids[soNWristXZ] = phScene->CreateSolid(solidDesc);

	// Joint -- Wrist ([p]LowerArm-[c]Hand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -lowerArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springWristY;
	hingeDesc.damper           = damperWristY;
	hingeDesc.origin           = Rad(0);
	joints[joNWristY]          = CreateJoint(solids[soNWristYX], solids[soNLowerArm], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springWristX;
	hingeDesc.damper           = damperWristX;
	hingeDesc.origin           = Rad(0);
	joints[joNWristX]          = CreateJoint(solids[soNWristXZ], solids[soNWristYX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, handLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaterniond(lr*oriRightHand.w, lr*oriRightHand.x, oriRightHand.y, oriRightHand.z).Inv() * Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springWristZ;
	hingeDesc.damper           = damperWristZ;
	hingeDesc.origin           = Rad(0);
	joints[joNWristZ]          = CreateJoint(solids[soNHand], solids[soNWristXZ], hingeDesc);

	phScene->SetContactMode(solids[soNHand], solids[soNLowerArm], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHuman::InitEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRHingeHuman::CreateEye(LREnum lr){
	CDBoxDesc          boxDesc;
	CDSphereDesc       sphereDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNEyeYX, soNEye;
	CRHingeHumanDesc::CRHumanJoints joNEyeY, joNEyeX;
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
	joints[joNEyeY]            = CreateJoint(solids[soNEyeYX], solids[SO_HEAD], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springEyeX;
	hingeDesc.damper           = damperEyeX;
	hingeDesc.origin           = Rad(0);
	joints[joNEyeX] = CreateJoint(solids[soNEye], solids[soNEyeYX], hingeDesc);

	phScene->SetContactMode(solids[soNEye], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeHuman::InitLegs(){
	CreateUpperLeg(LEFTPART);
	CreateLowerLeg(LEFTPART);
	CreateFoot(LEFTPART);

	CreateUpperLeg(RIGHTPART);
	CreateLowerLeg(RIGHTPART);
	CreateFoot(RIGHTPART);

	// 両足は近すぎて足の太さ次第では衝突してしまうため．
	phScene->SetContactMode(solids[SO_LEFT_UPPER_LEG], solids[SO_RIGHT_UPPER_LEG], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateUpperLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNWaistLegZX, soNWaistLegXY, soNUpperLeg;
	CRHingeHumanDesc::CRHumanJoints joNWaistLegZ, joNWaistLegX, joNWaistLegY;
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
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNWaistLegZX] = phScene->CreateSolid(solidDesc);
	solids[soNWaistLegXY] = phScene->CreateSolid(solidDesc);

	// Joint -- WaistLeg ([p]Waist-[c]UpperLeg)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(lr*interLegDistance/2.0, -waistHeight/2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springWaistLegZ;
	hingeDesc.damper           = damperWaistLegZ;
	hingeDesc.origin           = Rad(0);
	joints[joNWaistLegZ]       = CreateJoint(solids[soNWaistLegZX], solids[SO_WAIST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springWaistLegX;
	hingeDesc.damper           = damperWaistLegX;
	hingeDesc.origin           = Rad(0);
	joints[joNWaistLegX]       = CreateJoint(solids[soNWaistLegXY], solids[soNWaistLegZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, upperLegLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springWaistLegY;
	hingeDesc.damper           = damperWaistLegY;
	hingeDesc.origin           = Rad(0);
	joints[joNWaistLegY]       = CreateJoint(solids[soNUpperLeg], solids[soNWaistLegXY], hingeDesc);

	phScene->SetContactMode(solids[soNUpperLeg], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateLowerLeg(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNUpperLeg, soNLowerLeg;
	CRHingeHumanDesc::CRHumanJoints joNKnee;
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
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerLegLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springKnee;
	hingeDesc.damper           = damperKnee;
	hingeDesc.origin           = Rad(0);
	joints[joNKnee] = CreateJoint(solids[soNLowerLeg], solids[soNUpperLeg], hingeDesc);

	phScene->SetContactMode(solids[soNLowerLeg], solids[soNUpperLeg], PHSceneDesc::MODE_NONE);
}

void CRHingeHuman::CreateFoot(LREnum lr){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;

	CRHingeHumanDesc::CRHumanSolids soNLowerLeg, soNAnkleYX, soNAnkleXZ, soNFoot;
	CRHingeHumanDesc::CRHumanJoints joNAnkleY, joNAnkleX, joNAnkleZ;
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
	solidDesc.mass    = 0.04;
	solidDesc.inertia = Matrix3d::Unit() * 1.0;
	solids[soNAnkleYX] = phScene->CreateSolid(solidDesc);
	solids[soNAnkleXZ] = phScene->CreateSolid(solidDesc);

	// Joint -- Ankle ([p]LowerLeg-[c]Foot)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -lowerLegLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = springAnkleY;
	hingeDesc.damper           = damperAnkleY;
	hingeDesc.origin           = Rad(0);
	joints[joNAnkleY]          = CreateJoint(solids[soNAnkleYX], solids[soNLowerLeg], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = springAnkleX;
	hingeDesc.damper           = damperAnkleX;
	hingeDesc.origin           = Rad(0);
	joints[joNAnkleX]          = CreateJoint(solids[soNAnkleXZ], solids[soNAnkleYX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, footThickness / 2.0, (ankleToeDistance - footLength/2.0));
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = springAnkleZ;
	hingeDesc.damper           = damperAnkleZ;
	hingeDesc.origin           = Rad(0);
	joints[joNAnkleZ]          = CreateJoint(solids[soNFoot], solids[soNAnkleXZ], hingeDesc);

	phScene->SetContactMode(solids[soNFoot], solids[soNLowerLeg], PHSceneDesc::MODE_NONE);
}

// --- --- ---
int CRHingeHuman::NSolids(){
	return solids.size();
}

PHSolidIf* CRHingeHuman::GetSolid(int i){
	return solids[i];
}

int CRHingeHuman::NJoints(){
	return joints.size();
}

PHJointIf* CRHingeHuman::GetJoint(int i){
	return joints[i];
}

}
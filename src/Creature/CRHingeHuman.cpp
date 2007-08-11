#include "CRHingeHuman.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRHingeHuman
IF_OBJECT_IMP(CRHingeHuman, SceneObject);

void CRHingeHuman::Init(){
	PHSceneIf* scene = DCAST(PHSceneIf, GetScene());
	PHSdkIf*   sdk   = DCAST(PHSdkIf, scene->GetSdk());

	CDBoxDesc          boxDesc;
	CDSphereDesc       sphereDesc;
	CDCapsuleDesc      capsuleDesc;
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;
	PHBallJointDesc    ballDesc;

	// --- --- --- --- --- --- --- --- --- ---
	// Create Solids

	///// --- 中心パーツ

	// -- Waist
	solidDesc.mass     = 0.17;
	solids[SO_WAIST]   = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(sdk->CreateShape(boxDesc));
	solids[SO_WAIST]->SetDynamical(false);
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(0), 'y'));

	// -- Abdomen
	solidDesc.mass     = 0.028;
	solids[SO_ABDOMEN] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(abdomenBreadth, abdomenHeight, abdomenThickness);
	solids[SO_ABDOMEN]->AddShape(sdk->CreateShape(boxDesc));

	// -- Chest
	solidDesc.mass   = 0.252;
	solids[SO_CHEST] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(sdk->CreateShape(boxDesc));

	// -- Neck
	solidDesc.mass  = 0.028;
	solids[SO_NECK] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize = Vec3f(neckDiameter/1.414, neckLength, neckDiameter/1.414);
	solids[SO_NECK]->AddShape(sdk->CreateShape(boxDesc));

	// -- Head
	solidDesc.mass    = 0.07;
	solids[SO_HEAD]   = scene->CreateSolid(solidDesc);
	sphereDesc.radius = headDiameter / 2.0;
	solids[SO_HEAD]->AddShape(sdk->CreateShape(sphereDesc));

	///// --- 右パーツ

	// -- RightUpperArm
	solidDesc.mass             = 0.04;
	solids[SO_RIGHT_UPPER_ARM] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize            = Vec3f(upperArmDiameter, upperArmLength, upperArmDiameter);
	solids[SO_RIGHT_UPPER_ARM]->AddShape(sdk->CreateShape(boxDesc));

	// -- RightLowerArm
	solidDesc.mass             = 0.02;
	solids[SO_RIGHT_LOWER_ARM] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize            = Vec3f(lowerArmDiameter, lowerArmLength, lowerArmDiameter);
	solids[SO_RIGHT_LOWER_ARM]->AddShape(sdk->CreateShape(boxDesc));

	// -- RightHand
	solidDesc.mass        = 0.01;
	solids[SO_RIGHT_HAND] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize       = Vec3f(handBreadth, handLength, handThickness);
	solids[SO_RIGHT_HAND]->AddShape(sdk->CreateShape(boxDesc));

	// -- RightEye
	solidDesc.mass       = 0.001;
	solidDesc.inertia    = Matrix3d::Unit() * 0.001;
	solids[SO_RIGHT_EYE] = scene->CreateSolid(solidDesc);
	sphereDesc.radius    = eyeDiameter;
	solids[SO_RIGHT_EYE]->AddShape(sdk->CreateShape(sphereDesc));
	boxDesc.boxsize      = Vec3f(0.015,0.015,eyeDiameter+0.029);
	solids[SO_RIGHT_EYE]->AddShape(sdk->CreateShape(boxDesc));

	///// --- 左パーツ

	// -- LeftUpperArm
	solidDesc.mass            = 0.04;
	solids[SO_LEFT_UPPER_ARM] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize           = Vec3f(upperArmDiameter, upperArmLength, upperArmDiameter);
	solids[SO_LEFT_UPPER_ARM]->AddShape(sdk->CreateShape(boxDesc));

	// -- LeftLowerArm
	solidDesc.mass            = 0.02;
	solids[SO_LEFT_LOWER_ARM] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize           = Vec3f(lowerArmDiameter, lowerArmLength, lowerArmDiameter);
	solids[SO_LEFT_LOWER_ARM]->AddShape(sdk->CreateShape(boxDesc));

	// -- LeftHand
	solidDesc.mass       = 0.01;
	solids[SO_LEFT_HAND] = scene->CreateSolid(solidDesc);
	boxDesc.boxsize      = Vec3f(handBreadth, handLength, handThickness);
	solids[SO_LEFT_HAND]->AddShape(sdk->CreateShape(boxDesc));
	
	// -- LeftEye
	solidDesc.mass      = 0.001;
	solidDesc.inertia   = Matrix3d::Unit() * 0.001;
	solids[SO_LEFT_EYE] = scene->CreateSolid(solidDesc);
	sphereDesc.radius   = eyeDiameter;
	solids[SO_LEFT_EYE]->AddShape(sdk->CreateShape(sphereDesc));
	boxDesc.boxsize     = Vec3f(0.015,0.015,eyeDiameter+0.029);
	solids[SO_LEFT_EYE]->AddShape(sdk->CreateShape(boxDesc));

	///// --- 関節の中継ぎ用の形状を有しない剛体

	solidDesc.mass    = 0.001;
	solidDesc.inertia = Matrix3d::Unit() * 0.001;
	solids[SO_CHEST_NECK_XZ] = scene->CreateSolid(solidDesc);
	solids[SO_CHEST_NECK_ZY] = scene->CreateSolid(solidDesc);
	solids[SO_NECK_HEAD_XZ] = scene->CreateSolid(solidDesc);
	solids[SO_RIGHT_SHOULDER_ZX] = scene->CreateSolid(solidDesc);
	solids[SO_RIGHT_SHOULDER_XY] = scene->CreateSolid(solidDesc);
	solids[SO_RIGHT_WRIST_YX] = scene->CreateSolid(solidDesc);
	solids[SO_RIGHT_WRIST_XZ] = scene->CreateSolid(solidDesc);
	solids[SO_RIGHT_EYE_YX] = scene->CreateSolid(solidDesc);
	solids[SO_LEFT_SHOULDER_ZX] = scene->CreateSolid(solidDesc);
	solids[SO_LEFT_SHOULDER_XY] = scene->CreateSolid(solidDesc);
	solids[SO_LEFT_WRIST_YX] = scene->CreateSolid(solidDesc);
	solids[SO_LEFT_WRIST_XZ] = scene->CreateSolid(solidDesc);
	solids[SO_LEFT_EYE_YX] = scene->CreateSolid(solidDesc);

	// --- --- --- --- --- --- --- --- --- ---
	// Create Joints
	
	///// --- 中心パーツ

	// -- [p]Waist-[s]Abdomen
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,  waistHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -abdomenHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 100.0;
	hingeDesc.damper           = 4.0;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = -0.0902;
	hingeDesc.upper            = 1.7915;
	joints[JO_WAIST_ABDOMEN] = scene->CreateJoint(solids[SO_ABDOMEN], solids[SO_WAIST], hingeDesc);

	scene->SetContactMode(solids[SO_ABDOMEN], solids[SO_WAIST], PHSceneDesc::MODE_NONE);

	// -- [p]Abdomen-[s]Chest
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,  abdomenHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -chestHeight / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 100.0;
	hingeDesc.damper           = 4.0;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = -0.8552;
	hingeDesc.upper            = 0.8552;
	joints[JO_ABDOMEN_CHEST] = scene->CreateJoint(solids[SO_CHEST], solids[SO_ABDOMEN], hingeDesc);

	scene->SetContactMode(solids[SO_CHEST], solids[SO_ABDOMEN], PHSceneDesc::MODE_NONE);

	// -- [p]Chest-[s]Neck
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, chestHeight / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 200.0;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = -1.7628;
	hingeDesc.upper            = 1.2217;
	joints[JO_CHEST_NECK_X] = scene->CreateJoint(solids[SO_CHEST_NECK_XZ], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 200.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_CHEST_NECK_Z] = scene->CreateJoint(solids[SO_CHEST_NECK_ZY], solids[SO_CHEST_NECK_XZ], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 60.0;
	hingeDesc.origin           = Rad(0);
	hingeDesc.lower            = -1.0036;
	hingeDesc.upper            = 1.0036;
	joints[JO_CHEST_NECK_Y] = scene->CreateJoint(solids[SO_NECK], solids[SO_CHEST_NECK_ZY], hingeDesc);

	scene->SetContactMode(solids[SO_NECK], solids[SO_CHEST], PHSceneDesc::MODE_NONE);

	// -- [p]Neck-[s]Head
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, neckLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 60.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_NECK_HEAD_X] = scene->CreateJoint(solids[SO_NECK_HEAD_XZ], solids[SO_NECK], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -headDiameter / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 60.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_NECK_HEAD_Z] = scene->CreateJoint(solids[SO_HEAD], solids[SO_NECK_HEAD_XZ], hingeDesc);

	scene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);

	///// --- 右パーツ

	// -- RightShoulder ([p]Chest-[s]RightUpperArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(chestBreadth/2.0 + (upperArmDiameter/2.0*1.414), chestHeight/2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 10.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_SHOULDER_Z] = scene->CreateJoint(solids[SO_RIGHT_SHOULDER_ZX], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 10.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_SHOULDER_X] = scene->CreateJoint(solids[SO_RIGHT_SHOULDER_XY], solids[SO_RIGHT_SHOULDER_ZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, upperArmLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 50.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_SHOULDER_Y] = scene->CreateJoint(solids[SO_RIGHT_UPPER_ARM], solids[SO_RIGHT_SHOULDER_XY], hingeDesc);

	scene->SetContactMode(solids[SO_RIGHT_UPPER_ARM], solids[SO_CHEST], PHSceneDesc::MODE_NONE);

	// -- RightElbow ([p]RightUpperArm-[s]RightLowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerArmLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 4.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_ELBOW] = scene->CreateJoint(solids[SO_RIGHT_LOWER_ARM], solids[SO_RIGHT_UPPER_ARM], hingeDesc);

	scene->SetContactMode(solids[SO_RIGHT_LOWER_ARM], solids[SO_RIGHT_UPPER_ARM], PHSceneDesc::MODE_NONE);

	// -- RightWrist ([p]RightLowerArm-[s]RightHand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -lowerArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 15.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_WRIST_Y] = scene->CreateJoint(solids[SO_RIGHT_WRIST_YX], solids[SO_RIGHT_LOWER_ARM], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 15.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_WRIST_X] = scene->CreateJoint(solids[SO_RIGHT_WRIST_XZ], solids[SO_RIGHT_WRIST_YX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, handLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 15.0;
	hingeDesc.damper           = 10.0;
	joints[JO_RIGHT_WRIST_Z] = scene->CreateJoint(solids[SO_RIGHT_HAND], solids[SO_RIGHT_WRIST_XZ], hingeDesc);

	scene->SetContactMode(solids[SO_RIGHT_HAND], solids[SO_RIGHT_LOWER_ARM], PHSceneDesc::MODE_NONE);

	// -- Right Eye ([p]Head-[s]RightEye)
	hingeDesc                  = PHHingeJointDesc();
	//hingeDesc.posePlug.Pos()   = Vec3d(interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -occiputToEyeDistance+headDiameter/2.0);
	hingeDesc.posePlug.Pos()   = Vec3d(interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -headDiameter/2.0+eyeDiameter/2.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 500.0;
	hingeDesc.damper           = 5.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_EYE_Y] = scene->CreateJoint(solids[SO_RIGHT_EYE_YX], solids[SO_HEAD], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 500.0;
	hingeDesc.damper           = 5.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_RIGHT_EYE_X] = scene->CreateJoint(solids[SO_RIGHT_EYE], solids[SO_RIGHT_EYE_YX], hingeDesc);

	scene->SetContactMode(solids[SO_RIGHT_EYE], solids[SO_HEAD], PHSceneDesc::MODE_NONE);

	///// --- 左パーツ

	// -- LeftShoulder ([p]Chest-[s]LeftUpperArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(-chestBreadth/2.0 - (upperArmDiameter/2.0*1.414), chestHeight/2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 10.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_SHOULDER_Z] = scene->CreateJoint(solids[SO_LEFT_SHOULDER_ZX], solids[SO_CHEST], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 10.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_SHOULDER_X] = scene->CreateJoint(solids[SO_LEFT_SHOULDER_XY], solids[SO_LEFT_SHOULDER_ZX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, upperArmLength / 2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 50.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_SHOULDER_Y] = scene->CreateJoint(solids[SO_LEFT_UPPER_ARM], solids[SO_LEFT_SHOULDER_XY], hingeDesc);

	scene->SetContactMode(solids[SO_LEFT_UPPER_ARM], solids[SO_CHEST], PHSceneDesc::MODE_NONE);

	// -- LeftElbow ([p]LeftUpperArm-[s]LeftLowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -upperArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, lowerArmLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 4.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_ELBOW] = scene->CreateJoint(solids[SO_LEFT_LOWER_ARM], solids[SO_LEFT_UPPER_ARM], hingeDesc);

	scene->SetContactMode(solids[SO_LEFT_LOWER_ARM], solids[SO_LEFT_UPPER_ARM], PHSceneDesc::MODE_NONE);	

	// -- LeftWrist ([p]LeftLowerArm-[s]LeftHand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -lowerArmLength / 2.0, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 15.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_WRIST_Y] = scene->CreateJoint(solids[SO_LEFT_WRIST_YX], solids[SO_LEFT_LOWER_ARM], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 15.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_WRIST_X] = scene->CreateJoint(solids[SO_LEFT_WRIST_XZ], solids[SO_LEFT_WRIST_YX], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0, handLength / 2.0, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 15.0;
	hingeDesc.damper           = 10.0;
	joints[JO_LEFT_WRIST_Z] = scene->CreateJoint(solids[SO_LEFT_HAND], solids[SO_LEFT_WRIST_XZ], hingeDesc);

	scene->SetContactMode(solids[SO_LEFT_HAND], solids[SO_LEFT_LOWER_ARM], PHSceneDesc::MODE_NONE);	

	// -- Left Eye ([p]Head-[s]LeftEye)
	hingeDesc                  = PHHingeJointDesc();
	//hingeDesc.posePlug.Pos()   = Vec3d(-interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -occiputToEyeDistance+headDiameter/2.0);
	hingeDesc.posePlug.Pos()   = Vec3d(-interpupillaryBreadth/2.0, headDiameter/2.0 - vertexToEyeHeight, -headDiameter/2.0+eyeDiameter/2.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(-90), 'x');
	hingeDesc.spring           = 500.0;
	hingeDesc.damper           = 5.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_EYE_Y] = scene->CreateJoint(solids[SO_LEFT_EYE_YX], solids[SO_HEAD], hingeDesc);

	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 500.0;
	hingeDesc.damper           = 5.0;
	hingeDesc.origin           = Rad(0);
	joints[JO_LEFT_EYE_X] = scene->CreateJoint(solids[SO_LEFT_EYE], solids[SO_LEFT_EYE_YX], hingeDesc);

	scene->SetContactMode(solids[SO_LEFT_EYE], solids[SO_HEAD], PHSceneDesc::MODE_NONE);

	for(int i=0; i<(int)SO_NSOLIDS; i++){
		scene->SetContactMode(solids[i], PHSceneDesc::MODE_NONE);
	}
}

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
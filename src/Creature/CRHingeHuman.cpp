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
	PHSolidDesc        solidDesc;
	PHHingeJointDesc   hingeDesc;
	PHBallJointDesc    ballDesc;

	CDShapeIf* shape;

	// --- --- --- --- --- --- --- --- --- ---
	// Create Solids

	PHSolidIf *soWaist, *soAbdomen, *soChest, *soHead;
	PHSolidIf *soRightShoulderZY, *soRightShoulderYX, *soRightUpperArm, *soRightLowerArm;
	PHSolidIf *soRightWristZY, *soRightWristYX, *soRightHand;
	PHSolidIf *soRightEye, *soRightEyeYX;
	PHSolidIf *soLeftShoulderZY, *soLeftShoulderYX, *soLeftUpperArm, *soLeftLowerArm;
	PHSolidIf *soLeftWristZY, *soLeftWristYX, *soLeftHand;
	PHSolidIf *soLeftEye, *soLeftEyeYX;

	// -- Waist
	solidDesc.mass     = 1.0;
	soWaist            = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.19, 0.077, 0.137);
	soWaist->AddShape(sdk->CreateShape(boxDesc));
	//soWaist->SetOrientation(Quaternionf::Rot(Rad(180), 'y'));
	//soWaist->SetFramePosition(Vec3f(3,0,0));
	soWaist->SetDynamical(false);

	// -- Abdomen
	solidDesc.mass     = 1.0;
	soAbdomen          = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.15, 0.08, 0.108);
	soAbdomen->AddShape(sdk->CreateShape(boxDesc));

	// -- Chest
	solidDesc.mass     = 1.0;
	soChest            = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.18, 0.2, 0.1);
	soChest->AddShape(sdk->CreateShape(boxDesc));

	// -- Head
	solidDesc.mass     = 1.0;
	soHead             = scene->CreateSolid(solidDesc);
	sphereDesc.radius  = 0.06;
	soHead->AddShape(sdk->CreateShape(sphereDesc));

	// -- RightShoulder
	solidDesc.mass     = 0.1;
	soRightShoulderZY  = scene->CreateSolid(solidDesc);
	soRightShoulderYX  = scene->CreateSolid(solidDesc);

	// -- RightUpperArm
	solidDesc.mass     = 1.0;
	soRightUpperArm    = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.049, 0.16, 0.048);
	soRightUpperArm->AddShape(sdk->CreateShape(boxDesc));

	// -- RightLowerArm
	solidDesc.mass     = 1.0;
	soRightLowerArm    = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.046, 0.12, 0.046);
	soRightLowerArm->AddShape(sdk->CreateShape(boxDesc));

	// -- RightWrist
	solidDesc.mass     = 0.1;
	soRightWristZY     = scene->CreateSolid(solidDesc);
	soRightWristYX     = scene->CreateSolid(solidDesc);

	// -- RightHand
	solidDesc.mass     = 1.0;
	soRightHand        = scene->CreateSolid(solidDesc);
	sphereDesc.radius  = 0.025;
	soRightHand->AddShape(sdk->CreateShape(sphereDesc));

	// -- LeftShoulder
	solidDesc.mass     = 0.1;
	soLeftShoulderZY   = scene->CreateSolid(solidDesc);
	soLeftShoulderYX   = scene->CreateSolid(solidDesc);

	// -- LeftUpperArm
	solidDesc.mass     = 1.0;
	soLeftUpperArm     = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.049, 0.16, 0.048);
	soLeftUpperArm->AddShape(sdk->CreateShape(boxDesc));

	// -- LeftLowerArm
	solidDesc.mass     = 1.0;
	soLeftLowerArm     = scene->CreateSolid(solidDesc);
	boxDesc.boxsize    = Vec3f(0.046, 0.12, 0.046);
	soLeftLowerArm->AddShape(sdk->CreateShape(boxDesc));

	// -- LeftWrist
	solidDesc.mass     = 0.1;
	soLeftWristZY      = scene->CreateSolid(solidDesc);
	soLeftWristYX      = scene->CreateSolid(solidDesc);

	// -- LeftHand
	solidDesc.mass     = 1.0;
	soLeftHand         = scene->CreateSolid(solidDesc);
	sphereDesc.radius  = 0.025;
	soLeftHand->AddShape(sdk->CreateShape(sphereDesc));

	// -- RightEye
	solidDesc.mass     = 1.0;
	solidDesc.inertia  = Matrix3d::Unit() * 0.001;
	soRightEye         = scene->CreateSolid(solidDesc);
	soRightEyeYX       = scene->CreateSolid(solidDesc);
	sphereDesc.radius  = 0.01;
	soRightEye->AddShape(sdk->CreateShape(sphereDesc));
	
	// -- LeftEye
	solidDesc.mass     = 1.0;
	solidDesc.inertia  = Matrix3d::Unit() * 0.001;
	soLeftEye          = scene->CreateSolid(solidDesc);
	soLeftEyeYX        = scene->CreateSolid(solidDesc);
	sphereDesc.radius  = 0.01;
	soLeftEye->AddShape(sdk->CreateShape(sphereDesc));

	// --- --- --- --- --- --- --- --- --- ---
	// Create Joints
	PHJointIf *joWaistAbdomen, *joAbdomenChest, *joNeck;
	PHJointIf *joRightShoulderZ, *joRightShoulderY, *joRightShoulderX;
	PHJointIf *joRightElbow;
	PHJointIf *joRightWristZ, *joRightWristY, *joRightWristX;
	PHJointIf *joRightEyeY, *joRightEyeX;
	PHJointIf *joLeftShoulderZ, *joLeftShoulderY, *joLeftShoulderX;
	PHJointIf *joLeftElbow;
	PHJointIf *joLeftWristZ, *joLeftWristY, *joLeftWristX;
	PHJointIf *joLeftEyeY, *joLeftEyeX;
	
	// -- Waist-Abdomen
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, 0.04, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -0.04, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 60.0;
	hingeDesc.origin           = Rad(0);
	joWaistAbdomen             = scene->CreateJoint(soAbdomen, soWaist, hingeDesc);

	// -- Abdomen-Chest
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, 0.04, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -0.1, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 160.0;
	hingeDesc.damper           = 60.0;
	hingeDesc.origin           = Rad(0);
	joAbdomenChest             = scene->CreateJoint(soChest, soAbdomen, hingeDesc);

	// -- Neck (Chest-Head)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, 0.1, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, -0.08, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 3.0;
	hingeDesc.damper           = 3.0;
	hingeDesc.origin           = Rad(0);
	joNeck                     = scene->CreateJoint(soHead, soChest, hingeDesc);

	// -- RightShoulder (Chest-RightUpperArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.13, 0.1, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 10.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(-30);
	joRightShoulderZ           = scene->CreateJoint(soRightShoulderZY, soChest, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.spring           = 150.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(0);
	joRightShoulderY           = scene->CreateJoint(soRightShoulderYX, soRightShoulderZY, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, 0.09, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 10.0;
	hingeDesc.origin           = Rad(0);
	joRightShoulderX           = scene->CreateJoint(soRightUpperArm, soRightShoulderYX, hingeDesc);

	// -- RightElbow (RightUpperArm-RightLowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -0.09, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, 0.07, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 4.0;
	hingeDesc.origin           = Rad(-90);
	joRightElbow               = scene->CreateJoint(soRightLowerArm, soRightUpperArm, hingeDesc);

	// -- RightWrist (RightLowerArm-RightHand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -0.02, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 10.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(-30);
	joRightWristZ              = scene->CreateJoint(soRightWristZY, soRightLowerArm, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.spring           = 150.0;
	hingeDesc.damper           = 6.0;
	joRightWristY              = scene->CreateJoint(soRightWristYX, soRightWristZY, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, 0.02, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 10.0;
	joRightWristX              = scene->CreateJoint(soRightHand, soRightWristYX, hingeDesc);

	// -- Right Eye (Head-RightEye)
	/*
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.03, 0.0, 0.06);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.spring           = 0.0;
	hingeDesc.damper           = 0.0;
	hingeDesc.origin           = Rad(0);
	joRightEyeY                = scene->CreateJoint(soRightEyeYX, soHead, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,0,0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 0.0;
	hingeDesc.damper           = 0.0;
	hingeDesc.origin           = Rad(0);
	joRightEyeX                = scene->CreateJoint(soRightEye, soRightEyeYX, hingeDesc);
	*/
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.03, 0.0, 0.06);
	ballDesc.posePlug.Ori()    = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.poseSocket.Pos()  = Vec3d(0,0,0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'x');
	joRightEyeY                = scene->CreateJoint(soRightEye, soHead, ballDesc);

	// -- LeftShoulder (Chest-LeftUpperArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(-0.13, 0.1, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 10.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(-30);
	joLeftShoulderZ            = scene->CreateJoint(soLeftShoulderZY, soChest, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.spring           = 150.0;
	hingeDesc.damper           = 6.0;
	joLeftShoulderY            = scene->CreateJoint(soLeftShoulderYX, soLeftShoulderZY, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, 0.09, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 10.0;
	joLeftShoulderX            = scene->CreateJoint(soLeftUpperArm, soLeftShoulderYX, hingeDesc);

	// -- LeftElbow (LeftUpperArm-LeftLowerArm)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0, -0.09, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, 0.07, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 4.0;
	hingeDesc.origin           = Rad(-90);
	joLeftElbow                = scene->CreateJoint(soLeftLowerArm, soLeftUpperArm, hingeDesc);

	// -- LeftWrist (LeftLowerArm-LeftHand)
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, -0.02, 0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 10.0;
	hingeDesc.damper           = 6.0;
	hingeDesc.origin           = Rad(-30);
	joLeftWristZ               = scene->CreateJoint(soLeftWristZY, soLeftLowerArm, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.spring           = 150.0;
	hingeDesc.damper           = 6.0;
	joLeftWristY               = scene->CreateJoint(soLeftWristYX, soLeftWristZY, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0, 0.02, 0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring           = 6.0;
	hingeDesc.damper           = 10.0;
	joLeftWristX               = scene->CreateJoint(soLeftHand, soLeftWristYX, hingeDesc);

	// -- Left Eye (Head-LeftEye)
	/*
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(-0.03, 0.0, 0.06);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'z');
	hingeDesc.spring           = 0.0;
	hingeDesc.damper           = 0.0;
	hingeDesc.origin           = Rad(0);
	joLeftEyeY                 = scene->CreateJoint(soLeftEyeYX, soHead, hingeDesc);
	hingeDesc                  = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0,0,0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3d(0,0,0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring           = 0.0;
	hingeDesc.damper           = 0.0;
	hingeDesc.origin           = Rad(0);
	joLeftEyeX                 = scene->CreateJoint(soLeftEye, soLeftEyeYX, hingeDesc);
	*/
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(-0.03, 0.0, 0.06);
	ballDesc.posePlug.Ori()    = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.poseSocket.Pos()  = Vec3d(0,0,0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'x');
	joLeftEyeY                 = scene->CreateJoint(soLeftEye, soHead, ballDesc);

	// --- --- --- --- --- --- --- --- --- ---
	// Initialize
	scene->SetContactMode(soWaist, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soAbdomen, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soChest, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soHead, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soRightShoulderZY, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soRightShoulderYX, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soRightHand, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soRightEye, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soRightEyeYX, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soLeftShoulderZY, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soLeftShoulderYX, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soLeftHand, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soLeftEye, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soLeftEyeYX, PHSceneDesc::MODE_NONE);

	solids.push_back(soWaist);
	solids.push_back(soAbdomen);
	solids.push_back(soChest);
	solids.push_back(soHead);
	solids.push_back(soRightShoulderZY);
	solids.push_back(soRightShoulderYX);
	solids.push_back(soRightUpperArm);
	solids.push_back(soRightLowerArm);
	solids.push_back(soRightWristZY);
	solids.push_back(soRightWristYX);
	solids.push_back(soRightHand);
	solids.push_back(soRightEye);
	solids.push_back(soRightEyeYX);
	solids.push_back(soLeftShoulderZY);
	solids.push_back(soLeftShoulderYX);
	solids.push_back(soLeftUpperArm);
	solids.push_back(soLeftLowerArm);
	solids.push_back(soLeftWristZY);
	solids.push_back(soLeftWristYX);
	solids.push_back(soLeftHand);
	solids.push_back(soLeftEye);
	solids.push_back(soLeftEyeYX);

	joints.push_back(joWaistAbdomen);
	joints.push_back(joAbdomenChest);
	joints.push_back(joNeck);
	joints.push_back(joRightShoulderZ);
	joints.push_back(joRightShoulderY);
	joints.push_back(joRightShoulderX);
	joints.push_back(joRightElbow);
	joints.push_back(joRightWristZ);
	joints.push_back(joRightWristY);
	joints.push_back(joRightShoulderX);
	joints.push_back(joRightEyeY);
	joints.push_back(joRightEyeX);
	joints.push_back(joLeftShoulderZ);
	joints.push_back(joLeftShoulderY);
	joints.push_back(joLeftShoulderX);
	joints.push_back(joLeftElbow);
	joints.push_back(joLeftWristZ);
	joints.push_back(joLeftWristY);
	joints.push_back(joLeftShoulderX);
	joints.push_back(joLeftEyeY);
	joints.push_back(joLeftEyeX);
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
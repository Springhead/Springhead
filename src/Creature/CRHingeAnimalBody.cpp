#include "CRHingeAnimalBody.h"

#define GRAVITYMODE 0

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
	solidDesc.mass     = 0.2;
	solids[SO_WAIST]   = phScene->CreateSolid(solidDesc);
	// define the shape of the solid.
	boxDesc.boxsize    = Vec3f(waistBreadth, waistHeight, waistThickness);
	solids[SO_WAIST]->AddShape(phSdk->CreateShape(boxDesc));
	// define the position.
	solids[SO_WAIST]->SetFramePosition(Vec3f(0,0,0));
	solids[SO_WAIST]->SetOrientation(Quaternionf::Rot(Rad(-90), 'x'));

	//d—ÍØ‚é‚©
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
	solidDesc.mass   = 0.3;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Waist-[c]Chest
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0.0, waistHeight / 2.0, 0.0);
	ballDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.poseSocket.Pos() = Vec3d(0.0, -chestHeight / 2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.spring           = springWaistChest;
	ballDesc.damper           = damperWaistChest;
	ballDesc.origin           = Quaterniond::Rot(Rad(0), 'x');										//Quotaniond‚Ìorigin‚Á‚Ä‚Ç‚¤‚â‚Á‚Äw’è‚·‚é‚ÌH
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
	solidDesc.mass	= 0.05;
	solids[SO_TAIL1] = phScene->CreateSolid(solidDesc);
	solidDesc.mass  = 0.05;
	solids[SO_TAIL2] = phScene->CreateSolid(solidDesc);
	solidDesc.mass	= 0.05;
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
	ballDesc.origin            = oriTailWaist;
	//ballDesc.swingUpper        = rangeTailWaist;
	joints[JO_TAIL_WAIST]	   = CreateJoint(solids[SO_TAIL1], solids[SO_WAIST], ballDesc);

	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.origin            = oriTail;
	//ballDesc.swingUpper        = rangeTail;
	joints[JO_TAIL_12]		   = CreateJoint(solids[SO_TAIL2], solids[SO_TAIL1], ballDesc);
	
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.origin            = oriTail;
	//ballDesc.swingUpper        = rangeTail;
	joints[JO_TAIL_23]		   = CreateJoint(solids[SO_TAIL3], solids[SO_TAIL2], ballDesc);

	// define the position
	
	phScene->SetContactMode(solids[SO_TAIL1], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_TAIL2], solids[SO_TAIL1], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_TAIL3], solids[SO_TAIL2], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRHingeAnimalBody::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRHingeAnimalBody::CreateNeck(){
	
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;

	// Solid
	solidDesc.mass = 0.01;
	solids[SO_CHEST_NECK_XY] = phScene->CreateSolid(solidDesc);
	solids[SO_CHEST_NECK_YZ] = phScene->CreateSolid(solidDesc);
	solidDesc.mass = 0.1;
	solids[SO_NECK] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(neckBreath, neckHeight, neckThickness);
	solids[SO_NECK]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Chest - [c]Neck
	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'x');
	hingeDesc.spring		   = springChestNeckXY;
	hingeDesc.damper		   = damperChestNeckXY;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeChestNeckXY[0];
	hingeDesc.upper			   = rangeChestNeckXY[1];
	joints[JO_CHEST_NECK_X]   = CreateJoint(solids[SO_CHEST_NECK_XY], solids[SO_CHEST], hingeDesc);
	phScene->SetContactMode(solids[SO_CHEST_NECK_XY], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
	
	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'x');
	hingeDesc.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'x');
	hingeDesc.spring		   = springChestNeckYZ;
	hingeDesc.damper		   = damperChestNeckYZ;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeChestNeckYZ[0];
	hingeDesc.upper			   = rangeChestNeckYZ[1];
	joints[JO_CHEST_NECK_Y]   = CreateJoint(solids[SO_CHEST_NECK_YZ], solids[SO_CHEST_NECK_XY], hingeDesc);
	phScene->SetContactMode(solids[SO_CHEST_NECK_YZ], solids[SO_CHEST_NECK_XY], PHSceneDesc::MODE_NONE);

	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3d(0.0, chestHeight/2.0, chestThickness/2.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3d(0.0, neckHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springChestNeck;
	hingeDesc.damper		   = damperChestNeck;
	hingeDesc.origin		   = Rad(240);
	hingeDesc.lower			   = rangeChestNeck[0];
	hingeDesc.upper			   = rangeChestNeck[1];
	joints[JO_CHEST_NECK_Z]   = CreateJoint(solids[SO_NECK], solids[SO_CHEST_NECK_YZ], hingeDesc);
	phScene->SetContactMode(solids[SO_NECK], solids[SO_CHEST_NECK_YZ], PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateHead(){
// HingeJoint ver.
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;

	// define the solid
	solidDesc.mass = 0.03;
	solids[SO_HEAD] = phScene->CreateSolid(solidDesc);

	// define the shape
	boxDesc.boxsize = Vec3f(headBreath, headHeight, headThickness);
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(boxDesc));

	// define the connection [p]neck - [c]head
	hingeDesc					= PHHingeJointDesc();
	hingeDesc.posePlug.Pos()	= Vec3d(0.0, -neckHeight/2.0, neckThickness/2.0);
	hingeDesc.posePlug.Ori()	= Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos()	= Vec3d(0.0, -headHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori()	= Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring			= springNeckHead;
	hingeDesc.damper			= damperNeckHead;
	hingeDesc.origin			= Rad(90);
	hingeDesc.lower				= rangeNeckHead[0];
	hingeDesc.upper				= rangeNeckHead[1];
	joints[JO_NECK_HEAD]		= CreateJoint(solids[SO_HEAD], solids[SO_NECK], hingeDesc);
	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
    

// BallJoint ver.
/*
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// define the solid
	solidDesc.mass = 0.05;
	solids[SO_HEAD] = phScene->CreateSolid(solidDesc);
	
	// define the shape
	boxDesc.boxsize = Vec3f(headBreath, headHeight, headThickness);
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(boxDesc));

	// define the connection [p]neck - [c]head
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -neckHeight/2.0, neckThickness/2.0);
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, -headHeight/2.0, 0.0);
	ballDesc.spring            = springNeckHead;
	ballDesc.damper            = damperNeckHead;
	ballDesc.origin            = oriNeckHead;
	ballDesc.swingUpper        = rangeNeckHead;
	joints[JO_NECK_HEAD]	   = CreateJoint(solids[SO_HEAD], solids[SO_NECK], ballDesc);
	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
*/	

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

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;
	int soShoulder_XY, soShoulder_YZ, soBreastbone;
	int joShoulder_X,  joShoulder_Y,  joShoulder_Z;
	

	if(lr == LEFTPART){
		soShoulder_XY = SO_LEFT_SHOULDER_XY;
		soShoulder_YZ = SO_LEFT_SHOULDER_YZ;
		soBreastbone  = SO_LEFT_BREASTBONE;
		joShoulder_X  = JO_LEFT_SHOULDER_X;
		joShoulder_Y  = JO_LEFT_SHOULDER_Y;
		joShoulder_Z  = JO_LEFT_SHOULDER_Z;
	}

	else if(lr == RIGHTPART){
		soShoulder_XY = SO_RIGHT_SHOULDER_XY;
		soShoulder_YZ = SO_RIGHT_SHOULDER_YZ;
		soBreastbone  = SO_RIGHT_BREASTBONE;
		joShoulder_X  = JO_RIGHT_SHOULDER_X;
		joShoulder_Y  = JO_RIGHT_SHOULDER_Y;
		joShoulder_Z  = JO_RIGHT_SHOULDER_Z;
	}
	else {

	}
	// [p]chestË[c]Œp‚¬XY‚Ö
	solidDesc.mass = 0.005;
	solids[soShoulder_XY] = phScene->CreateSolid(solidDesc);

	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springShoulder_X;
	hingeDesc.damper		   = damperShoulder_X;
	hingeDesc.origin		   = Rad(40);
	hingeDesc.lower			   = rangeShoulder_X[0];
	hingeDesc.upper			   = rangeShoulder_X[1];
	
	joints[joShoulder_X]  = phScene->CreateJoint(solids[SO_CHEST], solids[soShoulder_XY], hingeDesc);

	phScene->SetContactMode(solids[SO_CHEST], solids[soShoulder_XY], PHSceneDesc::MODE_NONE);

	// [p]Œp‚¬XYË[c]Œp‚¬YZ‚Ö
	solidDesc.mass = 0.005;
	solids[soShoulder_YZ] = phScene->CreateSolid(solidDesc);

	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.spring		   = springShoulder_Y;
	hingeDesc.damper		   = damperShoulder_Y;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeShoulder_Y[0];
	hingeDesc.upper			   = rangeShoulder_Y[1];
	
	joints[joShoulder_Y] = phScene->CreateJoint(solids[soShoulder_XY], solids[soShoulder_YZ], hingeDesc);
	phScene->SetContactMode(solids[soShoulder_XY], solids[soShoulder_YZ], PHSceneDesc::MODE_NONE);
	
	// [p]Œp‚¬YZË[c]breastbone‚Ö
	solidDesc.mass = 0.02;
	solids[soBreastbone] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(breathtboneBreath, breathtboneHeight, breathtboneThickness);
	solids[soBreastbone]->AddShape(phSdk->CreateShape(boxDesc));
	
	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(lr*chestBreadth/3.0, 0.0, chestThickness);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3f(-lr*breathtboneBreath/2.0, breathtboneHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring		   = springShoulder_Z;
	hingeDesc.damper		   = damperShoulder_Z;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeShoulder_Z[0];
	hingeDesc.upper			   = rangeShoulder_Z[1];

	joints[joShoulder_Z] = phScene->CreateJoint(solids[soShoulder_YZ], solids[soBreastbone], hingeDesc);
	phScene->SetContactMode(solids[soShoulder_YZ], solids[soBreastbone], PHSceneDesc::MODE_NONE);
}

void CRHingeAnimalBody::CreateRadius(LREnum lr){

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;
	int soRadius, soBreastbone;
	int joElbow;

	if(lr == LEFTPART){
		soRadius	 = SO_LEFT_RADIUS;
		soBreastbone = SO_LEFT_BREASTBONE;
		joElbow		 = JO_LEFT_ELBOW; 
	}

	else if(lr == RIGHTPART){
		soRadius	 = SO_RIGHT_RADIUS;
		soBreastbone = SO_RIGHT_BREASTBONE;
		joElbow		 = JO_RIGHT_ELBOW;
	}

	solidDesc.mass = 0.01;
	solids[soRadius] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(radiusBreath, radiusHeight, radiusThickness);
	solids[soRadius]->AddShape(phSdk->CreateShape(boxDesc));
	
	//[p]breastbone-[c]radius
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, breathtboneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, -radiusHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springElbow;
	hingeDesc.damper		   = damperElbow;
	hingeDesc.origin		   = Rad(50);
	hingeDesc.lower			   = rangeElbow[0];
	hingeDesc.upper			   = rangeElbow[1];

	joints[joElbow] = phScene->CreateJoint(solids[soBreastbone], solids[soRadius], hingeDesc);

	phScene->SetContactMode(solids[soBreastbone],solids[soRadius],PHSceneDesc::MODE_NONE);
}	

void CRHingeAnimalBody::CreateFrontCannonBone(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soCannonBone, soRadius;
	int joKnee;

	if(lr == LEFTPART){
		soCannonBone = SO_LEFT_FRONT_CANNON_BONE;
		soRadius	 = SO_LEFT_RADIUS;
		joKnee		 = JO_LEFT_FRONT_KNEE;
	}

	else if(lr == RIGHTPART){
		soCannonBone = SO_RIGHT_FRONT_CANNON_BONE;
		soRadius	 = SO_RIGHT_RADIUS;
		joKnee		 = JO_RIGHT_FRONT_KNEE;
	}

	solidDesc.mass = 0.01;
	solids[soCannonBone] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(frontCannonBoneBreath, frontCannonBoneHeight, frontCannonBoneThickness);
	solids[soCannonBone]->AddShape(phSdk->CreateShape(boxDesc));
	
	// [p]Radius - [c]CannonBone
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, radiusHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, -frontCannonBoneHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springFrontKnee;
	hingeDesc.damper		   = damperFrontKnee;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeFrontKnee[0];
	hingeDesc.upper			   = rangeFrontKnee[1];

	joints[joKnee] = phScene->CreateJoint(solids[soRadius], solids[soCannonBone], hingeDesc);

	phScene->SetContactMode(solids[soRadius],solids[soCannonBone],PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateFrontToeBones(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soAnkle_XY, soAnkle_YZ, soToe, soCannonBone;
	int joAnkle_X, joAnkle_Y, joAnkle_Z;


	if(lr == LEFTPART){
		soAnkle_XY	 = SO_LEFT_FRONT_ANKLE_XY;
		soAnkle_YZ	 = SO_LEFT_FRONT_ANKLE_YZ;
		soToe		 = SO_LEFT_FRONT_TOE;
		soCannonBone = SO_LEFT_FRONT_CANNON_BONE;
		joAnkle_X	 = JO_LEFT_FRONT_ANKLE_X;
		joAnkle_Y	 = JO_LEFT_FRONT_ANKLE_Y;
		joAnkle_Z	 = JO_LEFT_FRONT_ANKLE_Z;
	}

	else if(lr == RIGHTPART){
		soAnkle_XY	 = SO_RIGHT_FRONT_ANKLE_XY;
		soAnkle_YZ	 = SO_RIGHT_FRONT_ANKLE_YZ;
		soToe		 = SO_RIGHT_FRONT_TOE;
		soCannonBone = SO_RIGHT_FRONT_CANNON_BONE;
		joAnkle_X	 = JO_RIGHT_FRONT_ANKLE_X;
		joAnkle_Y	 = JO_RIGHT_FRONT_ANKLE_Y;
		joAnkle_Z	 = JO_RIGHT_FRONT_ANKLE_Z;
	}
	//@[p]frontCannonBoneË[c]Œp‚¬XY‚Ö
	solidDesc.mass = 0.005;
	solids[soAnkle_XY] = phScene->CreateSolid(solidDesc);

	hingeDesc.posePlug.Pos()   = Vec3f(0.0, frontCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springFrontAnkle_X;
	hingeDesc.damper		   = damperFrontAnkle_X;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeFrontAnkle_X[0];
	hingeDesc.upper			   = rangeFrontAnkle_X[1];
	
	joints[joAnkle_X]  = phScene->CreateJoint(solids[soCannonBone], solids[soAnkle_XY], hingeDesc);

	phScene->SetContactMode(solids[soCannonBone], solids[soAnkle_XY], PHSceneDesc::MODE_NONE);

	// [p]Œp‚¬XYË[c]Œp‚¬YZ‚Ö
	solidDesc.mass = 0.005;
	solids[soAnkle_YZ] = phScene->CreateSolid(solidDesc);

	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.spring		   = springFrontAnkle_X;
	hingeDesc.damper		   = damperFrontAnkle_Y;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeFrontAnkle_Y[0];
	hingeDesc.upper			   = rangeFrontAnkle_Y[1];
	
	joints[joAnkle_Y] = phScene->CreateJoint(solids[soAnkle_XY], solids[soAnkle_YZ], hingeDesc);
	phScene->SetContactMode(solids[soAnkle_XY], solids[soAnkle_YZ], PHSceneDesc::MODE_NONE);
	
	// [p]Œp‚¬YZË[c]frontToe‚Ö
	solidDesc.mass = 0.02;
	solids[soToe] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(frontToeBreath, frontToeHeight, frontToeThickness);
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));
	
	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, frontToeHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring		   = springFrontAnkle_Z;
	hingeDesc.damper		   = damperFrontAnkle_Z;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeFrontAnkle_Z[0];
	hingeDesc.upper			   = rangeFrontAnkle_Z[1];

	joints[joAnkle_Z] = phScene->CreateJoint(solids[soAnkle_YZ], solids[soToe], hingeDesc);
	phScene->SetContactMode(solids[soAnkle_YZ], solids[soToe], PHSceneDesc::MODE_NONE);
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

	// —¼‘«‚Í‹ß‚·‚¬‚Ä‘«‚Ì‘¾‚³Ÿ‘æ‚Å‚ÍÕ“Ë‚µ‚Ä‚µ‚Ü‚¤‚½‚ßD
	phScene->SetContactMode(solids[SO_LEFT_FEMUR], solids[SO_RIGHT_FEMUR], PHSceneDesc::MODE_NONE);
}

void CRHingeAnimalBody::CreateFemur(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soHip_XY,soHip_YZ,soFemur;
	int joHip_X, joHip_Y, joHip_Z;
	if(lr == LEFTPART){
		soHip_XY = SO_LEFT_HIP_XY;
		soHip_YZ = SO_LEFT_HIP_YZ;
		soFemur	 = SO_LEFT_FEMUR;
		joHip_X  = JO_LEFT_HIP_X;
		joHip_Y  = JO_LEFT_HIP_Y;
		joHip_Z  = JO_LEFT_HIP_Z;
	}

	else if(lr == RIGHTPART){
		soHip_XY = SO_RIGHT_HIP_XY;
		soHip_YZ = SO_RIGHT_HIP_YZ;
		soFemur	 = SO_RIGHT_FEMUR;
		joHip_X  = JO_RIGHT_HIP_X;
		joHip_Y  = JO_RIGHT_HIP_Y;
		joHip_Z  = JO_RIGHT_HIP_Z;	
			
	}
	
	//Œp‚¬
	solidDesc.mass = 0.005;
	solids[soHip_XY] = phScene->CreateSolid(solidDesc);

	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springHip_X;
	hingeDesc.damper		   = damperHip_X;
	hingeDesc.origin		   = Rad(110);
	hingeDesc.lower			   = rangeHip_X[0];
	hingeDesc.upper			   = rangeHip_X[1];
	
	joints[joHip_X]  = phScene->CreateJoint(solids[SO_WAIST], solids[soHip_XY], hingeDesc);

	phScene->SetContactMode(solids[SO_WAIST], solids[soHip_XY], PHSceneDesc::MODE_NONE);

	// [p]Œp‚¬XYË[c]Œp‚¬YZ‚Ö
	solidDesc.mass = 0.005;
	solids[soHip_YZ] = phScene->CreateSolid(solidDesc);

	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.spring		   = springHip_Y;
	hingeDesc.damper		   = damperHip_Y;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeHip_Y[0];
	hingeDesc.upper			   = rangeHip_Y[1];
	
	joints[joHip_Y] = phScene->CreateJoint(solids[soHip_XY], solids[soHip_YZ], hingeDesc);
	phScene->SetContactMode(solids[soHip_XY], solids[soHip_YZ], PHSceneDesc::MODE_NONE);
	
	// [p]Œp‚¬YZË[c]femur‚Ö
	solidDesc.mass = 0.02;
	solids[soFemur] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(breathtboneBreath, breathtboneHeight, breathtboneThickness);
	solids[soFemur]->AddShape(phSdk->CreateShape(boxDesc));
	
	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(lr*waistBreadth/3.2, -waistHeight/3.0, -waistThickness/2.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3f(-lr*femurBreath/2.0, -femurHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring		   = springHip_Z;
	hingeDesc.damper		   = damperHip_Z;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeHip_Z[0];
	hingeDesc.upper			   = rangeHip_Z[1];

	joints[joHip_Z] = phScene->CreateJoint(solids[soHip_YZ], solids[soFemur], hingeDesc);
	phScene->SetContactMode(solids[soHip_YZ], solids[soFemur], PHSceneDesc::MODE_NONE);
}

void CRHingeAnimalBody::CreateTibia(LREnum lr){
	
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHHingeJointDesc	hingeDesc;
	int soTibia, soFemur;
	int joStifle;

	if(lr == LEFTPART){
		soTibia	 = SO_LEFT_TIBIA;
		soFemur  = SO_LEFT_FEMUR;
		joStifle = JO_LEFT_STIFLE; 
	}

	else if(lr == RIGHTPART){
		soTibia	 = SO_RIGHT_TIBIA;
		soFemur  = SO_RIGHT_FEMUR;
		joStifle = JO_RIGHT_STIFLE;
	}

	solidDesc.mass = 0.01;
	solids[soTibia] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(tibiaBreath, tibiaHeight, tibiaThickness);
	solids[soTibia]->AddShape(phSdk->CreateShape(boxDesc));
	
	//[p]femur-[c]tibia
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, femurHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, -tibiaHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springStifle;
	hingeDesc.damper		   = damperStifle;
	hingeDesc.origin		   = Rad(-30);
	hingeDesc.lower			   = rangeStifle[0];
	hingeDesc.upper			   = rangeStifle[1];

	joints[joStifle] = phScene->CreateJoint(solids[soFemur], solids[soTibia], hingeDesc);

	phScene->SetContactMode(solids[soFemur],solids[soTibia],PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateRearCannonBone(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soCannonBone, soTibia;
	int joKnee;

	if(lr == LEFTPART){
		soCannonBone = SO_LEFT_REAR_CANNON_BONE;
		soTibia		 = SO_LEFT_TIBIA;
		joKnee		 = JO_LEFT_REAR_KNEE;
	}

	else if(lr == RIGHTPART){
		soCannonBone = SO_RIGHT_REAR_CANNON_BONE;
		soTibia		 = SO_RIGHT_TIBIA;
		joKnee		 = JO_RIGHT_REAR_KNEE;
	}

	solidDesc.mass = 0.01;
	solids[soCannonBone] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(rearCannonBoneBreath, rearCannonBoneHeight, rearCannonBoneThickness);
	solids[soCannonBone]->AddShape(phSdk->CreateShape(boxDesc));
	
	// [p]Tibia - [c]CannonBone
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, tibiaHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, -rearCannonBoneHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springRearKnee;
	hingeDesc.damper		   = damperRearKnee;
	hingeDesc.origin		   = Rad(10);
	hingeDesc.lower			   = rangeRearKnee[0];
	hingeDesc.upper			   = rangeRearKnee[1];

	joints[joKnee] = phScene->CreateJoint(solids[soTibia], solids[soCannonBone], hingeDesc);

	phScene->SetContactMode(solids[soTibia],solids[soCannonBone],PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateRearToeBones(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHHingeJointDesc hingeDesc;
	int soAnkle_XY, soAnkle_YZ, soToe, soCannonBone;
	int joAnkle_X, joAnkle_Y, joAnkle_Z;


	if(lr == LEFTPART){
		soAnkle_XY	 = SO_LEFT_REAR_ANKLE_XY;
		soAnkle_YZ	 = SO_LEFT_REAR_ANKLE_YZ;
		soToe		 = SO_LEFT_REAR_TOE;
		soCannonBone = SO_LEFT_REAR_CANNON_BONE;
		joAnkle_X	 = JO_LEFT_REAR_ANKLE_X;
		joAnkle_Y	 = JO_LEFT_REAR_ANKLE_Y;
		joAnkle_Z	 = JO_LEFT_REAR_ANKLE_Z;
	}

	else if(lr == RIGHTPART){
		soAnkle_XY	 = SO_RIGHT_REAR_ANKLE_XY;
		soAnkle_YZ	 = SO_RIGHT_REAR_ANKLE_YZ;
		soToe		 = SO_RIGHT_REAR_TOE;
		soCannonBone = SO_RIGHT_REAR_CANNON_BONE;
		joAnkle_X	 = JO_RIGHT_REAR_ANKLE_X;
		joAnkle_Y	 = JO_RIGHT_REAR_ANKLE_Y;
		joAnkle_Z	 = JO_RIGHT_REAR_ANKLE_Z;
	}
	//@[p]rearCannonBoneË[c]Œp‚¬XY‚Ö
	solidDesc.mass = 0.005;
	solids[soAnkle_XY] = phScene->CreateSolid(solidDesc);

	hingeDesc.posePlug.Pos()   = Vec3f(0.0, rearCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springRearAnkle_X;
	hingeDesc.damper		   = damperRearAnkle_X;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeRearAnkle_X[0];
	hingeDesc.upper			   = rangeRearAnkle_X[1];
	
	joints[joAnkle_X]  = phScene->CreateJoint(solids[soCannonBone], solids[soAnkle_XY], hingeDesc);

	phScene->SetContactMode(solids[soCannonBone], solids[soAnkle_XY], PHSceneDesc::MODE_NONE);

	// [p]Œp‚¬XYË[c]Œp‚¬YZ‚Ö
	solidDesc.mass = 0.005;
	solids[soAnkle_YZ] = phScene->CreateSolid(solidDesc);

	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	hingeDesc.spring		   = springRearAnkle_X;
	hingeDesc.damper		   = damperRearAnkle_Y;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeRearAnkle_Y[0];
	hingeDesc.upper			   = rangeRearAnkle_Y[1];
	
	joints[joAnkle_Y] = phScene->CreateJoint(solids[soAnkle_XY], solids[soAnkle_YZ], hingeDesc);
	phScene->SetContactMode(solids[soAnkle_XY], solids[soAnkle_YZ], PHSceneDesc::MODE_NONE);
	
	// [p]Œp‚¬YZË[c]rearToe‚Ö
	solidDesc.mass = 0.02;
	solids[soToe] = phScene->CreateSolid(solidDesc);

	boxDesc.boxsize = Vec3f(rearToeBreath, rearToeHeight, rearToeThickness);
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));
	
	hingeDesc				   = PHHingeJointDesc();
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, 0.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, rearToeHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	hingeDesc.spring		   = springRearAnkle_Z;
	hingeDesc.damper		   = damperRearAnkle_Z;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeRearAnkle_Z[0];
	hingeDesc.upper			   = rangeRearAnkle_Z[1];

	joints[joAnkle_Z] = phScene->CreateJoint(solids[soAnkle_YZ], solids[soToe], hingeDesc);
	phScene->SetContactMode(solids[soAnkle_YZ], solids[soToe], PHSceneDesc::MODE_NONE);

}
// --- --- ---
void CRHingeAnimalBody::InitEyes(){
	CreateEye(LEFTPART);
	CreateEye(RIGHTPART);
}

void CRHingeAnimalBody::CreateEye(LREnum lr){
	
	if(lr == LEFTPART){
	
	}

	else if(lr == RIGHTPART){
	
	}

}

// --- --- ---
void CRHingeAnimalBody::InitContact(){
	// ©•ª‚É‘®‚·‚é„‘Ì“¯m‚ÌÚG‚ğOffi‚Ü‚¾­‚È‚·‚¬‚é‚©‚àHÅ’áŒÀ‚ÌÚG‚Íc‚µ‚½‚¢i07/09/25, mitakejj
	for (int i=0; i<solids.size(); ++i) {
		for (int j=0; j<solids.size(); ++j) {
			if (i!=j) {
				phScene->SetContactMode(solids[i], solids[j], PHSceneDesc::MODE_NONE);
			}
		}
	}

	// ©•ªˆÈŠO‚É‚·‚Å‚ÉBody‚ª‹‚ê‚Î‚»‚ÌBody‚É‘®‚·‚é„‘Ì‚Æ‚ÌContact‚àØ‚é
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

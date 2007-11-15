#include "CRHingeAnimalBody.h"

#define GRAVITYMODE 1

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRHingeAnimalBodyDesc::CRHingeAnimalBodyDesc(){

	jointOrder = SOCKET_PARENT;

	waistBreadth   = 0.6;
	waistHeight    = 0.8;
	waistThickness = 0.8;

	chestBreadth   = 0.6;
	chestHeight    = 1.5;
	chestThickness = 0.8;

	tailBreadth   = 0.1;
	tailHeight    = 0.2;
	tailThickness = 0.1;

	neckBreadth	  = 0.5;
	neckHeight	  = 1.2;
	neckThickness = 0.4;

	headBreadth   = 0.4;
	headHeight	  = 0.7;
	headThickness = 0.3;

	breastboneBreadth   = 0.2;
	breastboneHeight    = 0.5;
	breastboneThickness = 0.2; 

	radiusBreadth	= 0.2;
	radiusHeight	= 0.7;
	radiusThickness = 0.2;

	frontCannonBoneBreadth	 = 0.15;
	frontCannonBoneHeight	 = 0.6;
	frontCannonBoneThickness = 0.15;

	frontToeBreadth	  = 0.25;
	frontToeHeight	  = 0.1;
	frontToeThickness = 0.25;

	femurBreadth   = 0.25;
	femurHeight	   = 0.65;
	femurThickness = 0.25;

	tibiaBreadth   = 0.2;
	tibiaHeight	   = 0.7;
	tibiaThickness = 0.2;

	rearCannonBoneBreadth   = 0.15;
	rearCannonBoneHeight    = 0.65;
	rearCannonBoneThickness = 0.15;

	rearToeBreadth   = 0.25;
	rearToeHeight    = 0.1;
	rearToeThickness = 0.25;

	// spring and damper of ball joints
	// example:Vec3d(swingDir, swing, twist)
	springWaistChest   = Vec3d(0.0, 1.0, 0.0);    damperWaistChest	= Vec3d(0.0, 20.0, 0.0);
	springWaistTail    = Vec3d(0.0, 0.0, 0.0);    damperWaistTail	= Vec3d(0.0, 1.0, 0.0);
	springTail		   = Vec3d(0.0, 0.0, 0.0);    damperTail		= Vec3d(0.0, 1.0, 0.0);
	springChestNeck    = Vec3d(10.0, 100.0, 0.0); damperChestNeck	= Vec3d(50.0, 20.0, 0.0);
	springNeckHead	   = Vec3d(10.0, 100.0, 0.0); damperNeckHead	= Vec3d(50.0, 20.0, 0.0);
	springShoulder	   = Vec3d(0.0, 50.0, 0.0);   damperShoulder	= Vec3d(0.0, 20.0, 0.0);
	springFrontAnkle   = Vec3d(10.0,100.0, 0.0);  damperFrontAnkle	= Vec3d(5.0, 20.0, 0.0);
	springHip		   = Vec3d(0.0, 50.0, 0.0);   damperHip			= Vec3d(0.0, 20.0, 0.0);
	springRearAnkle	   = Vec3d(10.0,100.0, 0.0);  damperRearAnkle	= Vec3d(5.0, 20.0, 0.0);
	
	// spring and damper of hinge joints
	springElbow		   = 50.0;  damperElbow		= 20.0;
	springFrontKnee	   = 50.0;  damperFrontKnee	= 20.0;
	springStifle	   = 50.0;  damperStifle	= 20.0;
	springRearKnee	   = 50.0;  damperRearKnee	= 20.0;
	
	// origin of ball joints
	goalWaistChest.Swing()    = Rad(0);
	goalChestNeck.SwingDir()  = Rad(180);
	goalChestNeck.Swing()	  = Rad(70);
	goalNeckHead.Swing()	  = Rad(90);
	goalShoulder.SwingDir()   = Rad(0);
	goalShoulder.Swing()	  = Rad(120);
	goalFrontAnkle.Swing()	  = Rad(0);
	goalHip.SwingDir()		  = Rad(180);
	goalHip.Swing()			  = Rad(120);
	goalRearAnkle.Swing()	  = Rad(0);

	// Range of ball joints
	limitWaistChest.upper.Swing()	 = Rad(5);
	limitWaistChest.lower.Twist()	 = Rad(-5);
	limitWaistChest.upper.Twist()	 = Rad(5);
	limitWaistTail.upper.Swing()	 = Rad(10);
	limitWaistTail.lower.Twist()	 = Rad(-5);
	limitWaistTail.upper.Twist()	 = Rad(5);
	limitTail.upper.Swing()			 = Rad(20);
	limitTail.lower.Twist()			 = Rad(-5);
	limitTail.upper.Twist()			 = Rad(5);
	limitChestNeck.upper.Swing()	 = Rad(90);
	limitChestNeck.lower.Twist()	 = Rad(-5);
	limitChestNeck.upper.Twist()	 = Rad(5);
	limitNeckHead.upper.Swing()		 = Rad(100);
	limitNeckHead.lower.Twist()		 = Rad(-10);
	limitNeckHead.upper.Twist()		 = Rad(10);
	limitShoulder.upper.Swing()		 = Rad(180);
	limitShoulder.lower.Twist()		 = Rad(-10);
	limitShoulder.upper.Twist()		 = Rad(10);
	limitFrontAnkle.upper.Swing()	 = Rad(5);
	limitFrontAnkle.lower.Twist()	 = Rad(-1);
	limitFrontAnkle.upper.Twist()	 = Rad(1);
	limitHip.upper.Swing()			 = Rad(180);
	limitHip.lower.Twist()			 = Rad(-10);
	limitHip.upper.Twist()			 = Rad(10);
	limitRearAnkle.upper.Swing()	 = Rad(5);
	limitRearAnkle.lower.Twist()	 = Rad(-1);
	limitRearAnkle.upper.Twist()	 = Rad(1);

	// Range of hinge joints (Vec2d(lower, upper)  lower>upperのとき可動域制限無効)
	rangeElbow		  = Vec2d(Rad(- 90), Rad(+ 90));
	rangeFrontKnee	  = Vec2d(Rad(- 90), Rad(   0));
	rangeStifle		  = Vec2d(Rad(- 90), Rad(+ 90));
	rangeRearKnee	  = Vec2d(Rad(- 90), Rad(+ 90));

	noLegs = false;
}
	
	
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
	solidDesc.mass   = 0.7;
	solids[SO_WAIST] = phScene->CreateSolid(solidDesc);
	solids[SO_WAIST]->SetName("soWaist");
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
	solidDesc.mass   = 0.3;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	solids[SO_CHEST]->SetName("soChest");
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [c]Chest - [p]Waist
	ballDesc                  = PHBallJointDesc();
	ballDesc.posePlug.Pos()   = Vec3d(0.0, chestHeight / 2.0, 0.0);
	ballDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(0), 'z');
	ballDesc.poseSocket.Pos() = Vec3d(0.0, -waistHeight / 2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	ballDesc.spring           = springWaistChest;
	ballDesc.damper           = damperWaistChest;
	ballDesc.goal			  = goalWaistChest;
	ballDesc.limit			  = limitWaistChest;
	joints[JO_WAIST_CHEST]    = CreateJoint(solids[SO_CHEST], solids[SO_WAIST], ballDesc);		//CreateJoint([c],[p],desc)
	joints[JO_WAIST_CHEST]->SetName("joWaistChest");

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
	solids[SO_TAIL1]->SetName("soTail1");
	solidDesc.mass  = 0.05;
	solids[SO_TAIL2] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL2]->SetName("soTail2");
	solidDesc.mass	= 0.05;
	solids[SO_TAIL3] = phScene->CreateSolid(solidDesc);
	solids[SO_TAIL3]->SetName("soTail3");
	
	// define the shape
	boxDesc.boxsize = Vec3f(tailBreadth, tailHeight, tailThickness);
	solids[SO_TAIL1]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL2]->AddShape(phSdk->CreateShape(boxDesc));
	solids[SO_TAIL3]->AddShape(phSdk->CreateShape(boxDesc));

	// define the connection
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.1);
	ballDesc.posePlug.Ori()	   = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, waistHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.spring            = springWaistTail;
	ballDesc.damper            = damperWaistTail;
	ballDesc.goal			   = goalWaistTail;
	ballDesc.limit			   = limitWaistTail;
	joints[JO_WAIST_TAIL]	   = CreateJoint(solids[SO_TAIL1], solids[SO_WAIST], ballDesc);
	joints[JO_WAIST_TAIL]->SetName("joWaistTail");

	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.goal			   = goalTail;
	ballDesc.limit			   = limitTail;
	joints[JO_TAIL_12]		   = CreateJoint(solids[SO_TAIL2], solids[SO_TAIL1], ballDesc);
	joints[JO_TAIL_12]->SetName("joTail12");

	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, -tailHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(0), 'x');
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.goal			   = goalTail;
	ballDesc.limit			   = limitTail;
	joints[JO_TAIL_23]		   = CreateJoint(solids[SO_TAIL3], solids[SO_TAIL2], ballDesc);
	joints[JO_TAIL_23]->SetName("joTail23");
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
	PHBallJointDesc		ballDesc;

	// define the solid
	solidDesc.mass = 0.2;
	solids[SO_NECK] = phScene->CreateSolid(solidDesc);
	solids[SO_NECK]->SetName("soNeck");

	// define the shape
	boxDesc.boxsize = Vec3f(neckBreadth, neckHeight, neckThickness);
	solids[SO_NECK]->AddShape(phSdk->CreateShape(boxDesc));

	// define the joint  [c]neck - [p]chest
	ballDesc.posePlug.Pos()   = Vec3f(0.0, neckHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.poseSocket.Pos() = Vec3f(0.0, -chestHeight/2.0,  0.0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.spring			  = springChestNeck;
	ballDesc.damper			  = damperChestNeck;
	ballDesc.goal			  = goalChestNeck;
	ballDesc.limit			  = limitChestNeck;
	joints[JO_CHEST_NECK] = phScene->CreateJoint(solids[SO_NECK], solids[SO_CHEST], ballDesc);
	joints[JO_CHEST_NECK]->SetName("joChestNeck");

}

void CRHingeAnimalBody::CreateHead(){

// BallJoint ver.

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;

	// define the solid
	solidDesc.mass = 0.05;
	solids[SO_HEAD] = phScene->CreateSolid(solidDesc);
	solids[SO_HEAD]->SetName("soHead");
	// define the shape
	boxDesc.boxsize = Vec3f(headBreadth, headHeight, headThickness);
	solids[SO_HEAD]->AddShape(phSdk->CreateShape(boxDesc));

	// define the connection  [c]head - [p]neck
	ballDesc                   = PHBallJointDesc();
	ballDesc.posePlug.Pos()    = Vec3d(0.0, headHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.poseSocket.Pos()  = Vec3d(0.0, neckHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.spring            = springNeckHead;
	ballDesc.damper            = damperNeckHead;
	ballDesc.goal			   = goalNeckHead;
	ballDesc.limit			   = limitNeckHead;
	joints[JO_NECK_HEAD]	   = CreateJoint(solids[SO_HEAD], solids[SO_NECK], ballDesc);
	joints[JO_NECK_HEAD]->SetName("joNeckHead");

	phScene->SetContactMode(solids[SO_HEAD], solids[SO_NECK], PHSceneDesc::MODE_NONE);
	

}

// --- --- ---
void CRHingeAnimalBody::InitFrontLegs(){

	if(! noLegs){
		CreateBreastBone(LEFTPART);
		CreateRadius(LEFTPART);
		CreateFrontCannonBone(LEFTPART);
		CreateFrontToeBones(LEFTPART);

	}

	if(! noLegs){
		CreateBreastBone(RIGHTPART);
		CreateRadius(RIGHTPART);
		CreateFrontCannonBone(RIGHTPART);
		CreateFrontToeBones(RIGHTPART);

	}
	phScene->SetContactMode(solids[SO_LEFT_BREASTBONE], solids[SO_RIGHT_BREASTBONE], PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateBreastBone(LREnum lr){

	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;
	int soBreastbone;
	int joShoulder;
	

	if(lr == LEFTPART){
		soBreastbone  = SO_LEFT_BREASTBONE;
		joShoulder    = JO_LEFT_SHOULDER;
	}

	else if(lr == RIGHTPART){
		soBreastbone  = SO_RIGHT_BREASTBONE;
		joShoulder    = JO_RIGHT_SHOULDER;
	}
	else {

	}
	// [c]breastboneへ - [p]chest
	solidDesc.mass = 0.02;
	solids[soBreastbone] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soBreastbone]->SetName("soLeftBreastbone");
	else
		solids[soBreastbone]->SetName("soRightBreastbone");

	
	boxDesc.boxsize = Vec3f(breastboneBreadth, breastboneHeight, breastboneThickness);
	solids[soBreastbone]->AddShape(phSdk->CreateShape(boxDesc));
	
	ballDesc.posePlug.Pos()   = Vec3f(lr*breastboneBreadth/2.0, breastboneThickness/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaternionf::Rot(Rad(90), 'y');
	ballDesc.poseSocket.Pos() = Vec3f(-lr*chestBreadth/2.0, -chestThickness/2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	ballDesc.spring			  = springShoulder;
	ballDesc.damper			  = damperShoulder;
	ballDesc.goal			  = goalShoulder;
	ballDesc.limit			  = limitShoulder;
	joints[joShoulder] = phScene->CreateJoint(solids[soBreastbone], solids[SO_CHEST], ballDesc);
	if(lr == LEFTPART)
		joints[joShoulder]->SetName("joLeftShoulder");
	else
		joints[joShoulder]->SetName("joRightShoulder");

	phScene->SetContactMode(solids[soBreastbone], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
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
	if(lr == LEFTPART)
		solids[soRadius]->SetName("soLeftRadius");
	else
		solids[soRadius]->SetName("soRightRadius");

	boxDesc.boxsize = Vec3f(radiusBreadth, radiusHeight, radiusThickness);
	solids[soRadius]->AddShape(phSdk->CreateShape(boxDesc));
	
	//[p]breastbone-[c]radius
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -radiusHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, breastboneHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springElbow;
	hingeDesc.damper		   = damperElbow;
	hingeDesc.origin		   = Rad(-40);
	hingeDesc.lower			   = rangeElbow[0];
	hingeDesc.upper			   = rangeElbow[1];

	joints[joElbow] = phScene->CreateJoint(solids[soRadius], solids[soBreastbone], hingeDesc);
	if(lr == LEFTPART)
		joints[joElbow]->SetName("joLeftElbow");
	else
		joints[joElbow]->SetName("joRightElbow");

	phScene->SetContactMode(solids[soRadius], solids[soBreastbone], PHSceneDesc::MODE_NONE);
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
	if(lr == LEFTPART)
		solids[soCannonBone]->SetName("soLeftFrontCannonBone");
	else
		solids[soCannonBone]->SetName("soRightFrontCannonBone");

	boxDesc.boxsize = Vec3f(frontCannonBoneBreadth, frontCannonBoneHeight, frontCannonBoneThickness);
	solids[soCannonBone]->AddShape(phSdk->CreateShape(boxDesc));
	
	// [p]Radius - [c]CannonBone
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -frontCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, radiusHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springFrontKnee;
	hingeDesc.damper		   = damperFrontKnee;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeFrontKnee[0];
	hingeDesc.upper			   = rangeFrontKnee[1];

	joints[joKnee] = phScene->CreateJoint(solids[soCannonBone], solids[soRadius], hingeDesc);
	if(lr == LEFTPART)
		joints[joKnee]->SetName("joLeftFrontKnee");
	else
		joints[joKnee]->SetName("joRightFrontKnee");

	phScene->SetContactMode(solids[soCannonBone], solids[soRadius], PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateFrontToeBones(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHBallJointDesc  ballDesc;
	int soToe, soCannonBone;
	int joAnkle;


	if(lr == LEFTPART){
		soToe		 = SO_LEFT_FRONT_TOE;
		soCannonBone = SO_LEFT_FRONT_CANNON_BONE;
		joAnkle		 = JO_LEFT_FRONT_ANKLE;
	}

	else if(lr == RIGHTPART){
		soToe		 = SO_RIGHT_FRONT_TOE;
		soCannonBone = SO_RIGHT_FRONT_CANNON_BONE;
		joAnkle		 = JO_RIGHT_FRONT_ANKLE;
	}
	//　[p]frontCannonBone⇒[c]frontToeへ
	solidDesc.mass = 0.2;
	solids[soToe] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soToe]->SetName("soLeftFrontToe");
	else
		solids[soToe]->SetName("soRightFrontToe");

	boxDesc.boxsize = Vec3f(frontToeBreadth, frontToeHeight, frontToeThickness);
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));

	ballDesc.posePlug.Pos()   = Vec3f(0.0, -frontToeHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaternionf::Rot(Rad(0), 'z');
	ballDesc.poseSocket.Pos() = Vec3f(0.0, frontCannonBoneHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(0), 'z');
	ballDesc.spring			  = springFrontAnkle;
	ballDesc.damper			  = damperFrontAnkle;
	ballDesc.goal			  = goalFrontAnkle;
	ballDesc.limit			  = limitFrontAnkle;
	joints[joAnkle] = phScene->CreateJoint(solids[soToe], solids[soCannonBone], ballDesc);
	if(lr == LEFTPART)
		joints[joAnkle]->SetName("joLeftFrontAnkle");
	else
		joints[joAnkle]->SetName("joRightFrontAnkle");

	phScene->SetContactMode(solids[soToe], solids[soCannonBone], PHSceneDesc::MODE_NONE);
}


// --- --- ---
void CRHingeAnimalBody::InitRearLegs(){
	
	if (!noLegs) {
		CreateFemur(LEFTPART);
		CreateTibia(LEFTPART);
		CreateRearCannonBone(LEFTPART);
		CreateRearToeBones(LEFTPART);
	}
	
	if (!noLegs) {
		CreateFemur(RIGHTPART);
		CreateTibia(RIGHTPART);
		CreateRearCannonBone(RIGHTPART);
		CreateRearToeBones(RIGHTPART);
	}
	
	// 両足は近すぎて足の太さ次第では衝突してしまうため．
//	phScene->SetContactMode(solids[SO_LEFT_FEMUR], solids[SO_RIGHT_FEMUR], PHSceneDesc::MODE_NONE);

}


void CRHingeAnimalBody::CreateFemur(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHBallJointDesc ballDesc;
	int soFemur;
	int joHip;
	if(lr == LEFTPART){
		soFemur	 = SO_LEFT_FEMUR;
		joHip    = JO_LEFT_HIP;
	}

	else if(lr == RIGHTPART){
		soFemur	 = SO_RIGHT_FEMUR;
		joHip    = JO_RIGHT_HIP;	
	}
	

	// [p]waist⇒[c]femurへ
	solidDesc.mass = 0.02;
	solids[soFemur] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soFemur]->SetName("soLeftFemur");
	else
		solids[soFemur]->SetName("soRighFemur");
    
	boxDesc.boxsize = Vec3f(femurBreadth, femurHeight, femurThickness);
	solids[soFemur]->AddShape(phSdk->CreateShape(boxDesc));
	
	ballDesc.posePlug.Pos()   = Vec3f(lr*femurBreadth/2.0, femurThickness/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.poseSocket.Pos() = Vec3f(-lr*waistBreadth/2.2, 0.0, -waistThickness/2.0);
	ballDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.spring			  = springHip;
	ballDesc.damper			  = damperHip;
	ballDesc.goal			  = goalHip;
	ballDesc.limit			  = limitHip;
	joints[joHip] = phScene->CreateJoint(solids[soFemur], solids[SO_WAIST], ballDesc);
	if(lr == LEFTPART)
		joints[joHip]->SetName("joLeftHip");
	else
		joints[joHip]->SetName("joRightHip");

	phScene->SetContactMode(solids[soFemur], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
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
	if(lr == LEFTPART)
		solids[soTibia]->SetName("soLeftTibia");
	else
		solids[soTibia]->SetName("soRightTibia");

	boxDesc.boxsize = Vec3f(tibiaBreadth, tibiaHeight, tibiaThickness);
	solids[soTibia]->AddShape(phSdk->CreateShape(boxDesc));
	
	//[p]femur-[c]tibia
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -tibiaHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, femurHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springStifle;
	hingeDesc.damper		   = damperStifle;
	hingeDesc.origin		   = Rad(30);
	hingeDesc.lower			   = rangeStifle[0];
	hingeDesc.upper			   = rangeStifle[1];

	joints[joStifle] = phScene->CreateJoint(solids[soTibia], solids[soFemur], hingeDesc);
	if(lr == LEFTPART)
		joints[joStifle]->SetName("joLeftStifle");
	else
		joints[joStifle]->SetName("joRightStifle");

	phScene->SetContactMode(solids[soTibia], solids[soFemur], PHSceneDesc::MODE_NONE);

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
	if(lr == LEFTPART)
		solids[soCannonBone]->SetName("soLeftRearCannonBone");
	else
		solids[soCannonBone]->SetName("soRightRearCannonBone");
    
	boxDesc.boxsize = Vec3f(rearCannonBoneBreadth, rearCannonBoneHeight, rearCannonBoneThickness);
	solids[soCannonBone]->AddShape(phSdk->CreateShape(boxDesc));
	
	// [p]Tibia - [c]CannonBone
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -rearCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, tibiaHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaternionf::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springRearKnee;
	hingeDesc.damper		   = damperRearKnee;
	hingeDesc.origin		   = Rad(-5);
	hingeDesc.lower			   = rangeRearKnee[0];
	hingeDesc.upper			   = rangeRearKnee[1];

	joints[joKnee] = phScene->CreateJoint(solids[soCannonBone], solids[soTibia], hingeDesc);
	if(lr == LEFTPART)
		joints[joKnee]->SetName("joLeftRearKnee");
	else
		joints[joKnee]->SetName("joRightRearKnee");

	phScene->SetContactMode(solids[soCannonBone], solids[soTibia], PHSceneDesc::MODE_NONE);

}

void CRHingeAnimalBody::CreateRearToeBones(LREnum lr){
	
	CDBoxDesc			boxDesc;
	PHSolidDesc			solidDesc;
	PHBallJointDesc		ballDesc;
	int soToe, soCannonBone;
	int joAnkle;


	if(lr == LEFTPART){
		soToe		 = SO_LEFT_REAR_TOE;
		soCannonBone = SO_LEFT_REAR_CANNON_BONE;
		joAnkle		 = JO_LEFT_REAR_ANKLE;
	}

	else if(lr == RIGHTPART){
		soToe		 = SO_RIGHT_REAR_TOE;
		soCannonBone = SO_RIGHT_REAR_CANNON_BONE;
		joAnkle		 = JO_RIGHT_REAR_ANKLE;
	}
	//　[p]rearCannonBone⇒[c]rearToeへ
	solidDesc.mass = 0.02;
	solids[soToe] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soToe]->SetName("soLeftRearToe");
	else
		solids[soToe]->SetName("soRightRearToe");

	boxDesc.boxsize = Vec3f(rearToeBreadth, rearToeHeight, rearToeThickness);
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));
	
	ballDesc.posePlug.Pos()    = Vec3f(0.0, rearToeHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, rearCannonBoneHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaternionf::Rot(Rad(90), 'x');
	ballDesc.spring		       = springRearAnkle;
	ballDesc.damper			   = damperRearAnkle;
	ballDesc.goal			   = goalRearAnkle;
	ballDesc.limit			   = limitRearAnkle;
	joints[joAnkle] = phScene->CreateJoint(solids[soToe], solids[soCannonBone], ballDesc);
	if(lr == LEFTPART)
		joints[joAnkle]->SetName("joLeftRearAnkle");
	else
		joints[joAnkle]->SetName("joRightRearAnkle");

	phScene->SetContactMode(solids[soToe], solids[soCannonBone], PHSceneDesc::MODE_NONE);

}
// --- --- ---
void CRHingeAnimalBody::InitEyes(){
//	CreateEye(LEFTPART);
//	CreateEye(RIGHTPART);
}

void CRHingeAnimalBody::CreateEye(LREnum lr){
	
	if(lr == LEFTPART){
	
	}

	else if(lr == RIGHTPART){
	
	}

}

// --- --- ---
void CRHingeAnimalBody::InitContact(){
	// 自分に属する剛体同士の接触をOff（まだ少なすぎるかも？最低限の接触は残したい（07/09/25, mitake））
	for (unsigned int i=0; i<solids.size(); ++i) {
		for (unsigned int j=0; j<solids.size(); ++j) {
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
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}
}

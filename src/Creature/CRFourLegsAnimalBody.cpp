#include "CRFourLegsAnimalBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

//コンストラクタ
CRFourLegsAnimalBodyDesc::CRFourLegsAnimalBodyDesc(){

	jointOrder = SOCKET_PARENT;

	//---------------------------------------------------------
	// sizes of solids
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

	//------------------------------------------------------------------
	// spring and damper of ball joints
	springWaistChest   =  100.0; damperWaistChest	= 20.0;
	springWaistTail    =    0.0; damperWaistTail	=  0.0;
	springTail		   =    0.0; damperTail			=  0.0;
	springChestNeck    =  100.0; damperChestNeck	= 20.0;
	springNeckHead	   =  100.0; damperNeckHead		= 20.0;
	springShoulder	   =   50.0; damperShoulder		= 20.0;
	springFrontAnkle   = 1000.0; damperFrontAnkle	= 20.0;
	springHip		   =   50.0; damperHip			= 20.0;
	springRearAnkle	   = 1000.0; damperRearAnkle	= 20.0;
	
	//-------------------------------------------------------------------
	// spring and damper of hinge joints
	springElbow		   =   50.0; damperElbow		= 20.0;
	springFrontKnee	   =   50.0; damperFrontKnee	= 20.0;
	springStifle	   =   50.0; damperStifle		= 20.0;
	springRearKnee	   =   50.0; damperRearKnee		= 20.0;
	
	//-------------------------------------------------------------------
	// ball joints are conrtroled to the direction
	goalChestNeck		  = Quaterniond::Rot(Rad(  60), 'x');
	goalNeckHead		  = Quaterniond::Rot(Rad(- 90), 'x');
	goalShoulder		  = Quaterniond::Rot(Rad(-120), 'x');
	goalHip				  = Quaterniond::Rot(Rad(- 60), 'x');

	//----------------------------------------------------------------------
	// Range of ball joints
	limitSwingWaistChest.upper	= Rad(5);
	limitTwistWaistChest.lower	= Rad(-5);
	limitTwistWaistChest.upper	= Rad(5);
	
	limitSwingWaistTail.upper	= Rad(10);
	limitTwistWaistTail.lower	= Rad(-5);
	limitTwistWaistTail.upper	= Rad(5);

	limitSwingTail.upper		= Rad(20);
	limitTwistTail.lower		= Rad(-5);
	limitTwistTail.upper		= Rad(5);
	
	limitSwingChestNeck.upper	= Rad(180);
	limitTwistChestNeck.lower	= Rad(-5);
	limitTwistChestNeck.upper	= Rad(5);

	limitSwingNeckHead.upper	= Rad(150);
	limitTwistNeckHead.lower	= Rad(-10);
	limitTwistNeckHead.upper	= Rad(10);

	limitSwingShoulder.upper	= Rad(180);
	limitTwistShoulder.lower	= Rad(-10);
	limitTwistShoulder.upper	= Rad(10);

	limitSwingFrontAnkle.upper	= Rad(5);
	limitTwistFrontAnkle.lower	= Rad(-1);
	limitTwistFrontAnkle.upper	= Rad(1);

	limitSwingHip.upper			= Rad(180);
	limitTwistHip.lower			= Rad(-10);
	limitTwistHip.upper			= Rad(10);

	limitSwingRearAnkle.upper	= Rad(5);
	limitTwistRearAnkle.lower	= Rad(-1);
	limitTwistRearAnkle.upper	= Rad(1);

	//-----------------------------------------------------------------------------------
	// Range of hinge joints (Vec2d(lower, upper)  lower>upperのとき可動域制限無効)
	rangeElbow		  = Vec2d(Rad(- 90), Rad(+ 90));
	rangeFrontKnee	  = Vec2d(Rad(   0), Rad(+ 90));
	rangeStifle		  = Vec2d(Rad(- 90), Rad(+ 90));
	rangeRearKnee	  = Vec2d(Rad(- 90), Rad(	0));

	//-----------------------------------------------------------------------------------
	// limit Direction of the ball joints

	//-----------------------------------------------------------------------------------
	// Do you want to make the legs?
	noLegs = false;

	//-----------------------------------------------------------------------------------
	// dynamical 
	dynamicalMode = false;
}
	
	
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// CRFourLegsAnimalBody
IF_OBJECT_IMP(CRFourLegsAnimalBody, CRBody);
	
// --- --- ---
void CRFourLegsAnimalBody::Init(){
	CRBody::Init();
}

// --- --- ---
void CRFourLegsAnimalBody::InitBody(){
	CreateWaist();
	CreateChest();
	CreateTail();
}

void CRFourLegsAnimalBody::CreateWaist(){
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
	solids[SO_WAIST]->SetOrientation(Quaterniond::Rot(Rad(-90), 'x'));

	//重力切るか

	solids[SO_WAIST]->SetFramePosition(Vec3f(0.0, 3, 0.0));
	solids[SO_WAIST]->SetDynamical(dynamicalMode);

}

void CRFourLegsAnimalBody::CreateChest(){
	CDBoxDesc          boxDesc;
	PHSolidDesc        solidDesc;
	PHBallJointDesc    ballDesc;

	// Solid
	solidDesc.mass   = 0.3;
	solids[SO_CHEST] = phScene->CreateSolid(solidDesc);
	solids[SO_CHEST]->SetName("soChest");
	boxDesc.boxsize  = Vec3f(chestBreadth, chestHeight, chestThickness);
	solids[SO_CHEST]->AddShape(phSdk->CreateShape(boxDesc));

	// Joint -- [p]Waist - [c]Chest
	ballDesc                  = PHBallJointDesc();
	ballDesc.poseSocket.Pos() = Vec3f(0.0, waistHeight / 2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.posePlug.Pos()   = Vec3f(0.0, -chestHeight / 2.0, 0.0);
	ballDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.spring           = springWaistChest;
	ballDesc.damper           = damperWaistChest;
	ballDesc.goal			  = goalWaistChest;
	ballDesc.limitSwing		  = limitSwingWaistChest;
	ballDesc.limitTwist		  = limitTwistWaistChest;	

	//----------------------------------------------------------------------------
	// PHScene::CreateJoint([p], [c], desc)で使う。
	// <memo>: [絶滅API]CRBody::CreateJoint([c], [p], desc)と宣言が逆なので注意！
	//----------------------------------------------------------------------------
	joints[JO_WAIST_CHEST]    = phScene->CreateJoint(solids[SO_WAIST], solids[SO_CHEST], ballDesc);		
	joints[JO_WAIST_CHEST]->SetName("joWaistChest");

	phScene->SetContactMode(solids[SO_WAIST], solids[SO_CHEST], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBody::CreateTail(){
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
	// [p]waist - [c]tail1
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, -waistHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, tailHeight/2.0, 0.1);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring            = springWaistTail;
	ballDesc.damper            = damperWaistTail;
	ballDesc.goal			   = goalWaistTail;
	ballDesc.limitSwing		   = limitSwingWaistTail;
	ballDesc.limitTwist		   = limitTwistWaistTail;
	joints[JO_WAIST_TAIL]	   = phScene->CreateJoint(solids[SO_WAIST], solids[SO_TAIL1], ballDesc);
	joints[JO_WAIST_TAIL]->SetName("joWaistTail");

	//[p]tail1 - [c]tail2
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, tailHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.goal			   = goalTail;
	ballDesc.limitSwing		   = limitSwingTail;
	ballDesc.limitTwist		   = limitTwistTail;
	joints[JO_TAIL_12]		   = phScene->CreateJoint(solids[SO_TAIL1], solids[SO_TAIL2], ballDesc);
	joints[JO_TAIL_12]->SetName("joTail12");

	//[p]tail2 - [c]tail3
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, -tailHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, tailHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(0), 'x');
	ballDesc.spring            = springTail;
	ballDesc.damper            = damperTail;
	ballDesc.goal			   = goalTail;
	ballDesc.limitSwing		   = limitSwingTail;
	ballDesc.limitTwist		   = limitTwistTail;
	joints[JO_TAIL_23]		   = phScene->CreateJoint(solids[SO_TAIL2], solids[SO_TAIL3], ballDesc);
	joints[JO_TAIL_23]->SetName("joTail23");
	// define the position
	
	phScene->SetContactMode(solids[SO_TAIL1], solids[SO_WAIST], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_TAIL2], solids[SO_TAIL1], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_TAIL3], solids[SO_TAIL2], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRFourLegsAnimalBody::InitHead(){
	CreateNeck();
	CreateHead();
}

void CRFourLegsAnimalBody::CreateNeck(){
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

	// define the joint  [p]chest - [c]neck
	ballDesc.poseSocket.Pos() = Vec3f(0.0, chestHeight/2.0,  0.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(150), 'x');
	ballDesc.posePlug.Pos()   = Vec3f(0.0, -neckHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(150), 'x');
	ballDesc.spring			  = springChestNeck;
	ballDesc.damper			  = damperChestNeck;
	ballDesc.goal			  = goalChestNeck;
	ballDesc.limitSwing		  = limitSwingChestNeck;
	ballDesc.limitTwist		  = limitTwistChestNeck;

	joints[JO_CHEST_NECK] = phScene->CreateJoint(solids[SO_CHEST], solids[SO_NECK], ballDesc);
	joints[JO_CHEST_NECK]->SetName("joChestNeck");

	phScene->SetContactMode(solids[SO_CHEST], solids[SO_NECK], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBody::CreateHead(){

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

	// define the connection  [p]neck - [c]head
	ballDesc                   = PHBallJointDesc();
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, neckHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, -headHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.spring            = springNeckHead;
	ballDesc.damper            = damperNeckHead;
	ballDesc.goal			   = goalNeckHead;
	ballDesc.limitSwing		   = limitSwingNeckHead;
	ballDesc.limitTwist		   = limitTwistNeckHead;

	joints[JO_NECK_HEAD]	   = phScene->CreateJoint(solids[SO_NECK], solids[SO_HEAD], ballDesc);
	joints[JO_NECK_HEAD]->SetName("joNeckHead");

	phScene->SetContactMode(solids[SO_NECK], solids[SO_HEAD], PHSceneDesc::MODE_NONE);
	

}

// --- --- ---
void CRFourLegsAnimalBody::InitFrontLegs(){

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

void CRFourLegsAnimalBody::CreateBreastBone(LREnum lr){

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
	// [p]chest - [c]breastbone
	solidDesc.mass = 0.02;
	solids[soBreastbone] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soBreastbone]->SetName("soLeftBreastbone");
	else
		solids[soBreastbone]->SetName("soRightBreastbone");

	
	boxDesc.boxsize = Vec3f(breastboneBreadth, breastboneHeight, breastboneThickness);
	solids[soBreastbone]->AddShape(phSdk->CreateShape(boxDesc));
	
	ballDesc.poseSocket.Pos() = Vec3f(lr*chestBreadth/2.0, chestHeight/2.2, -chestThickness/3.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.posePlug.Pos()	  = Vec3f(-lr*breastboneBreadth/2.0, -breastboneHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.spring			  = springShoulder;
	ballDesc.damper			  = damperShoulder;
	ballDesc.goal			  = goalShoulder;
	ballDesc.limitSwing		  = limitSwingShoulder;
	ballDesc.limitTwist		  = limitTwistShoulder;
	ballDesc.limitDir		  = limitDirShoulder;

	//----------------------------------------------------------------------------
	// phScene->CreateJoint([p], [c], desc)で使う。
	// <memo>: [絶滅API]CRBody::CreateJoint([c], [p], desc)と宣言が逆なので注意！
	//----------------------------------------------------------------------------
	joints[joShoulder] = phScene->CreateJoint(solids[SO_CHEST], solids[soBreastbone], ballDesc);
	if(lr == LEFTPART)
		joints[joShoulder]->SetName("joLeftShoulder");
	else
		joints[joShoulder]->SetName("joRightShoulder");

	phScene->SetContactMode(solids[SO_CHEST], solids[soBreastbone], PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solids[SO_NECK], solids[soBreastbone], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBody::CreateRadius(LREnum lr){

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
	
	//[p]breastbone - [c]radius
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, breastboneHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -radiusHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springElbow;
	hingeDesc.damper		   = damperElbow;
	hingeDesc.origin		   = Rad(40);
	hingeDesc.lower			   = rangeElbow[0];
	hingeDesc.upper			   = rangeElbow[1];

	joints[joElbow] = phScene->CreateJoint(solids[soBreastbone], solids[soRadius], hingeDesc);
	if(lr == LEFTPART)
		joints[joElbow]->SetName("joLeftElbow");
	else
		joints[joElbow]->SetName("joRightElbow");

	phScene->SetContactMode(solids[soBreastbone], solids[soRadius], PHSceneDesc::MODE_NONE);
}	

void CRFourLegsAnimalBody::CreateFrontCannonBone(LREnum lr){
	
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
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, radiusHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -frontCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springFrontKnee;
	hingeDesc.damper		   = damperFrontKnee;
	hingeDesc.origin		   = Rad(0);
	hingeDesc.lower			   = rangeFrontKnee[0];
	hingeDesc.upper			   = rangeFrontKnee[1];

	joints[joKnee] = phScene->CreateJoint(solids[soRadius], solids[soCannonBone], hingeDesc);
	if(lr == LEFTPART)
		joints[joKnee]->SetName("joLeftFrontKnee");
	else
		joints[joKnee]->SetName("joRightFrontKnee");

	phScene->SetContactMode(solids[soRadius], solids[soCannonBone], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBody::CreateFrontToeBones(LREnum lr){
	
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
	//　[p]frontCannonBone - [c]frontToe
	solidDesc.mass = 0.2;
	solids[soToe] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soToe]->SetName("soLeftFrontToe");
	else
		solids[soToe]->SetName("soRightFrontToe");

	boxDesc.boxsize = Vec3f(frontToeBreadth, frontToeHeight, frontToeThickness);
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));

	ballDesc.poseSocket.Pos() = Vec3f(0.0, frontCannonBoneHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.posePlug.Pos()   = Vec3f(0.0, -frontToeHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(0), 'z');
	ballDesc.spring			  = springFrontAnkle;
	ballDesc.damper			  = damperFrontAnkle;
	ballDesc.goal			  = goalFrontAnkle;
	ballDesc.limitSwing		  = limitSwingFrontAnkle;
	ballDesc.limitTwist		  = limitTwistFrontAnkle;

	joints[joAnkle] = phScene->CreateJoint(solids[soCannonBone], solids[soToe], ballDesc);
	if(lr == LEFTPART)
		joints[joAnkle]->SetName("joLeftFrontAnkle");
	else
		joints[joAnkle]->SetName("joRightFrontAnkle");

	phScene->SetContactMode(solids[soCannonBone], solids[soToe], PHSceneDesc::MODE_NONE);
}

// --- --- ---
void CRFourLegsAnimalBody::InitRearLegs(){
	
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
	phScene->SetContactMode(solids[SO_LEFT_FEMUR], solids[SO_RIGHT_FEMUR], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBody::CreateFemur(LREnum lr){
	
	CDBoxDesc		 boxDesc;
	PHSolidDesc		 solidDesc;
	PHBallJointDesc  ballDesc;
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
	

	// [p]waist - [c]femur
	solidDesc.mass = 0.02;
	solids[soFemur] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soFemur]->SetName("soLeftFemur");
	else
		solids[soFemur]->SetName("soRighFemur");
    
	boxDesc.boxsize = Vec3f(femurBreadth, femurHeight, femurThickness);
	solids[soFemur]->AddShape(phSdk->CreateShape(boxDesc));
	
	ballDesc.poseSocket.Pos() = Vec3f(lr*waistBreadth/2.0, -waistHeight/2.0, -waistThickness/3.0);
	ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.posePlug.Pos()	  = Vec3f(-lr*femurBreadth/2.0, -femurHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	  = Quaterniond::Rot(Rad(-90), 'x');
	ballDesc.spring			  = springHip;
	ballDesc.damper			  = damperHip;
	ballDesc.goal			  = goalHip;
	ballDesc.limitSwing		  = limitSwingHip;
	ballDesc.limitTwist		  = limitTwistHip;

	joints[joHip] = phScene->CreateJoint(solids[SO_WAIST], solids[soFemur], ballDesc);
	if(lr == LEFTPART)
		joints[joHip]->SetName("joLeftHip");
	else
		joints[joHip]->SetName("joRightHip");

	phScene->SetContactMode(solids[SO_WAIST], solids[soFemur], PHSceneDesc::MODE_NONE);
}

void CRFourLegsAnimalBody::CreateTibia(LREnum lr){
	
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
	
	//[p]femur - [c]tibia
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, femurHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -tibiaHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springStifle;
	hingeDesc.damper		   = damperStifle;
	hingeDesc.origin		   = Rad(-30);
	hingeDesc.lower			   = rangeStifle[0];
	hingeDesc.upper			   = rangeStifle[1];

	joints[joStifle] = phScene->CreateJoint(solids[soFemur], solids[soTibia], hingeDesc);
	if(lr == LEFTPART)
		joints[joStifle]->SetName("joLeftStifle");
	else
		joints[joStifle]->SetName("joRightStifle");

	phScene->SetContactMode(solids[soFemur], solids[soTibia], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBody::CreateRearCannonBone(LREnum lr){
	
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
	hingeDesc.poseSocket.Pos() = Vec3f(0.0, tibiaHeight/2.0, 0.0);
	hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.posePlug.Pos()   = Vec3f(0.0, -rearCannonBoneHeight/2.0, 0.0);
	hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(90), 'y');
	hingeDesc.spring		   = springRearKnee;
	hingeDesc.damper		   = damperRearKnee;
	hingeDesc.origin		   = Rad(-5);
	hingeDesc.lower			   = rangeRearKnee[0];
	hingeDesc.upper			   = rangeRearKnee[1];

	joints[joKnee] = phScene->CreateJoint(solids[soTibia], solids[soCannonBone], hingeDesc);
	if(lr == LEFTPART)
		joints[joKnee]->SetName("joLeftRearKnee");
	else
		joints[joKnee]->SetName("joRightRearKnee");

	phScene->SetContactMode(solids[soTibia], solids[soCannonBone], PHSceneDesc::MODE_NONE);

}

void CRFourLegsAnimalBody::CreateRearToeBones(LREnum lr){
	
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
	//　[p]rearCannonBone - [c]rearToe
	solidDesc.mass = 0.02;
	solids[soToe] = phScene->CreateSolid(solidDesc);
	if(lr == LEFTPART)
		solids[soToe]->SetName("soLeftRearToe");
	else
		solids[soToe]->SetName("soRightRearToe");

	boxDesc.boxsize = Vec3f(rearToeBreadth, rearToeHeight, rearToeThickness);
	solids[soToe]->AddShape(phSdk->CreateShape(boxDesc));
	
	ballDesc.poseSocket.Pos()  = Vec3f(0.0, rearCannonBoneHeight/2.0, 0.0);
	ballDesc.poseSocket.Ori()  = Quaterniond::Rot(Rad(90), 'x');
	ballDesc.posePlug.Pos()    = Vec3f(0.0, rearToeHeight/2.0, 0.0);
	ballDesc.posePlug.Ori()	   = Quaterniond::Rot(Rad(90), 'x');
	ballDesc.spring		       = springRearAnkle;
	ballDesc.damper			   = damperRearAnkle;
	ballDesc.goal			   = goalRearAnkle;
	ballDesc.limitSwing		   = limitSwingRearAnkle;
	ballDesc.limitTwist		   = limitTwistRearAnkle;

	joints[joAnkle] = phScene->CreateJoint(solids[soCannonBone], solids[soToe], ballDesc);
	if(lr == LEFTPART)
		joints[joAnkle]->SetName("joLeftRearAnkle");
	else
		joints[joAnkle]->SetName("joRightRearAnkle");

	phScene->SetContactMode(solids[soCannonBone], solids[soToe], PHSceneDesc::MODE_NONE);

}
// --- --- ---
void CRFourLegsAnimalBody::InitEyes(){
//	CreateEye(LEFTPART);
//	CreateEye(RIGHTPART);
}

void CRFourLegsAnimalBody::CreateEye(LREnum lr){
	
	if(lr == LEFTPART){
	
	}

	else if(lr == RIGHTPART){
	
	}

}

// --- --- ---
void CRFourLegsAnimalBody::InitContact(){
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
		if (DCAST(CRFourLegsAnimalBodyIf,body)!=(this->Cast())) {
			for (int s=0; s<body->NSolids(); ++s) {
				for (unsigned int j=0; j<solids.size(); ++j) {
					phScene->SetContactMode(body->GetSolid(s), solids[j], PHSceneDesc::MODE_NONE);
				}
			}
		}
	}
}

void CRFourLegsAnimalBody::SetUpperBody(){
	upperBody.push_back(solids[CRFourLegsAnimalBodyDesc::SO_WAIST]);
	upperBody.push_back(solids[CRFourLegsAnimalBodyDesc::SO_CHEST]);
	upperBody.push_back(solids[CRFourLegsAnimalBodyDesc::SO_NECK]);
	upperBody.push_back(solids[CRFourLegsAnimalBodyDesc::SO_HEAD]);
}

Vec3d CRFourLegsAnimalBody::GetUpperCenterOfMass(){
	
	SetUpperBody();

	/// ボディ上体の重量小計
	double totalWeightOfUpperPart = 0;
	/// ボディ上体の中心座標
	Vec3d  centerPosOfUpperBlocks = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0; i<(int)upperBody.size(); i++){
		if(upperBody[i]){
			centerPosOfUpperBlocks = centerPosOfUpperBlocks + upperBody[i]->GetCenterPosition() * upperBody[i]->GetMass();
			totalWeightOfUpperPart = totalWeightOfUpperPart + upperBody[i]->GetMass(); 
		}
	}

	return centerPosOfUpperBlocks / totalWeightOfUpperPart;
}


}

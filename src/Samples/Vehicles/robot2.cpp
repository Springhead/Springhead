#include "robot2.h"

void Robot2::Leg::Build(PHSolidIf* body, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk){
	CDBoxDesc bd;
	bd.boxsize = Vec3f(0.1, 0.4, 0.1);
	boxCrank = DCAST(CDBoxIf, sdk->CreateShape(bd));
	bd.boxsize = Vec3f(0.1, 1.0, 0.1);
	boxFoot = DCAST(CDBoxIf, sdk->CreateShape(bd));
	bd.boxsize = Vec3f(0.1, 1.0, 0.1);
	boxGuide = DCAST(CDBoxIf, sdk->CreateShape(bd));

	PHSolidDesc sd;
	sd.mass = 0.1;
	sd.inertia = Matrix3d::Unit() * 0.1;
	soCrank = scene->CreateSolid(sd);
	soCrank->AddShape(boxCrank);
	soFoot[0] = scene->CreateSolid(sd);
	soFoot[0]->AddShape(boxFoot);
	soFoot[1] = scene->CreateSolid(sd);
	soFoot[1]->AddShape(boxFoot);
	soGuide[0] = scene->CreateSolid(sd);
	soGuide[0]->AddShape(boxGuide);
	soGuide[1] = scene->CreateSolid(sd);
	soGuide[1]->AddShape(boxGuide);

	const double K = 100.0, D = 100.0;
	PHHingeJointDesc jd;
	jd.pose[0] = base;
	jd.pose[1].Pos() = Vec3d(0.0, 0.0, 0.0);
	jntCrank = DCAST(PHHingeJointIf, scene->CreateJoint(body, soCrank, jd));
	
	jd.pose[0] = Posed();
	jd.pose[0].Pos() = Vec3d(0.0, 0.2, 0.06);
	jd.pose[1].Pos() = Vec3d(0.0, -0.1, -0.06);
	jntFoot[0] = DCAST(PHHingeJointIf, scene->CreateJoint(soCrank, soFoot[0], jd));
	jd.pose[0].Pos() = Vec3d(0.0, -0.2, -0.06);
	jd.pose[1].Pos() = Vec3d(0.0, -0.1, 0.06);
	jntFoot[1] = DCAST(PHHingeJointIf, scene->CreateJoint(soCrank, soFoot[1], jd));
	
	jntCrank->SetSpring(K);
	jntCrank->SetDamper(D);
	jntCrank->SetSpringOrigin(0.0);
	jntFoot[0]->SetSpring(K);
	jntFoot[0]->SetDamper(D);
	jntFoot[0]->SetSpringOrigin(0.0);
	jntFoot[1]->SetSpring(K);
	jntFoot[1]->SetDamper(D);
	jntFoot[1]->SetSpringOrigin(0.0);

	jd.pose[0].Pos() = Vec3d(0.0, 0.5, 0.0);
	jd.pose[1].Pos() = Vec3d(0.0, -0.5, 0.0);
	jntFootGuide[0] = DCAST(PHHingeJointIf, scene->CreateJoint(soFoot[0], soGuide[0], jd));
	//jd.pose[0].Pos() = Vec3d(0.0, 0.5, 0.0);
	//jd.pose[1].Pos() = Vec3d(0.0, -0.5, 0.0);
	jntFootGuide[1] = DCAST(PHHingeJointIf, scene->CreateJoint(soFoot[1], soGuide[1], jd));
	//jntFootGuide[0]->SetRange(0.0, 0.1);
	//jntFootGuide[1]->SetRange(0.0, 0.1);
	scene->EnableContact(soCrank, soFoot[0], false);
	scene->EnableContact(soCrank, soFoot[1], false);
	scene->EnableContact(soCrank, soGuide[0], false);
	scene->EnableContact(soCrank, soGuide[1], false);
	scene->EnableContact(soGuide[0], soFoot[0], false);
	scene->EnableContact(soGuide[1], soFoot[1], false);
	jntFootGuide[0]->SetSpring(K);
	jntFootGuide[0]->SetDamper(D);
	jntFootGuide[0]->SetSpringOrigin(Rad(-90.0));
	jntFootGuide[1]->SetSpring(K);
	jntFootGuide[1]->SetDamper(D);
	jntFootGuide[1]->SetSpringOrigin(Rad(-90.0));

	//return ;

	double dt = scene->GetTimeStep();
	double T = 2.0;
	for(double t = 0.0; t < T; t+=dt)
		scene->Step();

	Posed pose;
	pose.Pos() = Vec3d(0.9, 0.4, 0.06);
	jd.pose[0] = base * pose;
	jd.pose[1].Pos() = Vec3d(0.0, 0.5, -0.06);
	jntGuideBody[0] = DCAST(PHHingeJointIf, scene->CreateJoint(body, soGuide[0], jd));
	pose.Pos() = Vec3d(0.9, 0.4, -0.06);
	jd.pose[0] = base * pose;
	jd.pose[1].Pos() = Vec3d(0.0, 0.5, 0.06);
	jntGuideBody[1] = DCAST(PHHingeJointIf, scene->CreateJoint(body, soGuide[1], jd));
	
	jntCrank->SetSpring(0.0); jntCrank->SetDamper(0.0);
	jntFoot[0]->SetSpring(0.0); jntFoot[0]->SetDamper(0.0);
	jntFoot[1]->SetSpring(0.0); jntFoot[1]->SetDamper(0.0);
	jntFootGuide[0]->SetSpring(0.0); jntFootGuide[0]->SetDamper(0.0);
	jntFootGuide[1]->SetSpring(0.0); jntFootGuide[1]->SetDamper(0.0);
	
}

void Robot2::Build(PHSceneIf* scene, PHSdkIf* sdk){
	CDBoxDesc bd;
	bd.boxsize = Vec3f(1.5, 0.7, 1.5);
	boxBody = DCAST(CDBoxIf, sdk->CreateShape(bd));
	
	PHSolidDesc sd;
	soBody = scene->CreateSolid(sd);
	soBody->AddShape(boxBody);
	soBody->SetFramePosition(Vec3d(0.0, 2.0, 0.0));
	soBody->SetDynamical(false);

	Posed pose;
	pose.Pos() = Vec3d(-1.5, 0.0, 1.5);
	leg[0].Build(soBody, pose, scene, sdk);
	pose.Pos() = Vec3d(-1.5, 0.0, -1.5);
	leg[2].Build(soBody, pose, scene, sdk);
	
	pose.Ori() = Quaterniond::Rot(Rad(180.0), 'y');
	pose.Pos() = Vec3d(1.5, 0.0, 1.5);
	leg[1].Build(soBody, pose, scene, sdk);
	pose.Pos() = Vec3d(1.5, 0.0, -1.5);
	leg[3].Build(soBody, pose, scene, sdk);

	soBody->SetDynamical(true);
}

void Robot2::Forward(){
	leg[0].jntCrank->SetDesiredVelocity(3.0);
	leg[1].jntCrank->SetDesiredVelocity(-3.0);
	leg[2].jntCrank->SetDesiredVelocity(3.0);
	leg[3].jntCrank->SetDesiredVelocity(-3.0);
}

void Robot2::Backward(){
	leg[0].jntCrank->SetDesiredVelocity(-3.0);
	leg[1].jntCrank->SetDesiredVelocity(3.0);
	leg[2].jntCrank->SetDesiredVelocity(-3.0);
	leg[3].jntCrank->SetDesiredVelocity(3.0);
}

void Robot2::TurnLeft(){
	leg[0].jntCrank->SetDesiredVelocity(3.0);
	leg[1].jntCrank->SetDesiredVelocity(-3.0);
	leg[2].jntCrank->SetDesiredVelocity(-3.0);
	leg[3].jntCrank->SetDesiredVelocity(3.0);
}

void Robot2::TurnRight(){
	leg[0].jntCrank->SetDesiredVelocity(-3.0);
	leg[1].jntCrank->SetDesiredVelocity(3.0);
	leg[2].jntCrank->SetDesiredVelocity(3.0);
	leg[3].jntCrank->SetDesiredVelocity(-3.0);
}

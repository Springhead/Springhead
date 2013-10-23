/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "robot.h"
#include <vector>

using namespace std;

void Robot::Build(const Posed& pose, PHSceneIf* phScene, PHSdkIf* phSdk){
	CDBoxDesc bd;
	bd.boxsize = Vec3f(0.8, 0.6, 1.0);
	boxBody = phSdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.1, 0.4, 0.1);
	boxCrank = phSdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.1, 1.5, 0.1);
	boxFoot = phSdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.1, 0.8, 0.1);
	boxGuide = phSdk->CreateShape(bd)->Cast();
	
	PHSolidDesc sd;
	soBody = phScene->CreateSolid(sd);
	soBody->AddShape(boxBody);
	soBody->SetPose(pose);
	soBody->SetDynamical(false);
	PHRootNodeIf* root = phScene->CreateRootNode(soBody);

	Posed poseLeg[4];
	poseLeg[0].Pos() = Vec3d(-1.3, -0.3, 1.0);
	poseLeg[2].Pos() = Vec3d(-1.3, -0.3, -1.0);
	poseLeg[1].Pos() = Vec3d(1.3, -0.3, 1.0);
	poseLeg[1].Ori() = Quaterniond::Rot(Rad(180.0), 'y');
	poseLeg[3].Pos() = Vec3d(1.3, -0.3, -1.0);
	poseLeg[3].Ori() = Quaterniond::Rot(Rad(180.0), 'y');
	
	for(int i = 0; i < 4; i++){		
		PHSolidDesc sd;
		sd.mass = 0.1;
		sd.inertia = Matrix3d::Unit() * 0.1;
		sd.pose.Pos() = Vec3d(2,1,0);
		leg[i].soCrank = phScene->CreateSolid(sd);
		leg[i].soCrank->AddShape(boxCrank);
		leg[i].soFoot[0] = phScene->CreateSolid(sd);
		leg[i].soFoot[0]->AddShape(boxFoot);
		leg[i].soFoot[1] = phScene->CreateSolid(sd);
		leg[i].soFoot[1]->AddShape(boxFoot);
		leg[i].soGuide[0] = phScene->CreateSolid(sd);
		leg[i].soGuide[0]->AddShape(boxGuide);
		sd.pose.Ori() = Quaterniond::Rot(Rad(160), 'z') * sd.pose.Ori();
		leg[i].soGuide[1] = phScene->CreateSolid(sd);
		leg[i].soGuide[1]->AddShape(boxGuide);

		// クランク
		PHHingeJointDesc jd;
		jd.poseSocket = poseLeg[i];
		jd.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);
		leg[i].jntCrank = phScene->CreateJoint(soBody, leg[i].soCrank, jd)->Cast();
		leg[i].jntCrank->SetDamper(1.0);
		if (i==0) phScene->CreateTreeNode(root, leg[i].soCrank);
	
		const double K = 100.0, D = 0.01;
	
		Posed pose;
//		PHTreeNodeIf* node;				// unused!
		for(int j = 0; j < 2; j++){
			pose.Pos() = Vec3d(0.9, 0.4, (j == 0 ? 0.06 : -0.06));
			jd.poseSocket = poseLeg[i] * pose;
			jd.posePlug.Pos() = Vec3d(0.0, 0.4, (j == 0 ? -0.06 : 0.06));
			leg[i].jntGuideBody[j] = phScene->CreateJoint(soBody, leg[i].soGuide[j], jd)->Cast();
			leg[i].jntGuideBody[j]->SetDamper(D);
			leg[i].jntGuideBody[j]->SetSpring(K);
			leg[i].jntGuideBody[j]->SetTargetPosition(Rad(-90.0));
			//node = phScene->CreateTreeNode(root, leg[i].soGuide[j]);
			
			jd.poseSocket.Ori() = Quaterniond();
			jd.poseSocket.Pos() = Vec3d(0.0, -0.4, 0.0);
			jd.posePlug.Pos() = Vec3d(0.0, 0.7, 0.0);
			leg[i].jntFootGuide[j] = phScene->CreateJoint(leg[i].soGuide[j], leg[i].soFoot[j], jd)->Cast();
			//phScene->CreateTreeNode(node, leg[i].soFoot[j]);
		}
	
		// 閉リンクの構成
		for(int j = 0; j < 2; j++){
			jd.poseSocket = Posed();
			jd.poseSocket.Pos() = Vec3d(0.0, (j == 0 ? 0.1 : -0.1), (j == 0 ? 0.06: -0.06));
			jd.posePlug.Pos() = Vec3d(0.0, -0.1+0.25, (j == 0 ? -0.06 : 0.06));
			leg[i].jntFoot[j] = phScene->CreateJoint(leg[i].soCrank, leg[i].soFoot[j], jd)->Cast();
		}
	}

	// 脚のリンク同士は接触計算しない
	vector<PHSolidIf*> group;
	group.push_back(soBody);
	for(int i = 0; i < 4; i++){
		group.push_back(leg[i].soCrank);
		group.push_back(leg[i].soFoot[0]);
		group.push_back(leg[i].soFoot[1]);
		group.push_back(leg[i].soGuide[0]);
		group.push_back(leg[i].soGuide[1]);
	}
	phScene->SetContactMode(&group[0], group.size(), PHSceneDesc::MODE_NONE);

	//	落ち着くまで待つ
	double dt = phScene->GetTimeStep();
	double T = 5.0;
	for(double t = 0.0; t < T; t+=dt) phScene->Step();

	// バネ解除
	for(int i=0; i<4; ++i){
		leg[i].jntGuideBody[0]->SetSpring(0.0);
		leg[i].jntGuideBody[1]->SetSpring(0.0);
		leg[i].jntFootGuide[0]->SetSpring(0.0);
		leg[i].jntFootGuide[1]->SetSpring(0.0);
	}
	soBody->SetDynamical(true);

}


const double speed = 3;
void Robot::Stop(){
	leg[0].jntCrank->SetOffsetForce(0);
	leg[1].jntCrank->SetOffsetForce(0);
	leg[2].jntCrank->SetOffsetForce(0);
	leg[3].jntCrank->SetOffsetForce(0);
}

void Robot::Forward(){
	leg[0].jntCrank->SetOffsetForce(speed);
	leg[1].jntCrank->SetOffsetForce(-speed);
	leg[2].jntCrank->SetOffsetForce(speed);
	leg[3].jntCrank->SetOffsetForce(-speed);
}

void Robot::Backward(){
	leg[0].jntCrank->SetOffsetForce(-speed);
	leg[1].jntCrank->SetOffsetForce(speed);
	leg[2].jntCrank->SetOffsetForce(-speed);
	leg[3].jntCrank->SetOffsetForce(speed);
}

void Robot::TurnLeft(){
	leg[0].jntCrank->SetOffsetForce(speed);
	leg[1].jntCrank->SetOffsetForce(-speed);
	leg[2].jntCrank->SetOffsetForce(-speed);
	leg[3].jntCrank->SetOffsetForce(speed);
}

void Robot::TurnRight(){
	leg[0].jntCrank->SetOffsetForce(-speed);
	leg[1].jntCrank->SetOffsetForce(speed);
	leg[2].jntCrank->SetOffsetForce(speed);
	leg[3].jntCrank->SetOffsetForce(-speed);
}

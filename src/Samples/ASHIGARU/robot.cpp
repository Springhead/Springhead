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

void Robot::Leg::Build(PHSolidIf* body, PHRootNodeIf* root, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk){

	//部品のサイズを指定する
	CDBoxDesc bd;
	/*bd.boxsize = Vec3f(0.038, 0.12, 0.002);//ASHIGARU足先アルミ板部分
	boxFoot = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.026, 0.032, 0.05);//以下ダイナミクセル
	boxDX1 = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.026, 0.032, 0.05);
	boxDX2 = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.032, 0.026, 0.05);
	boxDX3 = sdk->CreateShape(bd)->Cast();*/


	bd.boxsize = Vec3f(0.1, 0.4, 0.1);
	boxCrank = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.1, 1.5, 0.1);
	boxFoot = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.1, 0.8, 0.1);
	boxGuide = sdk->CreateShape(bd)->Cast();

	//部品のソリッドモデルを作成する
	PHSolidDesc sd;
	sd.mass = 0.1;
	sd.inertia = Matrix3d::Unit() * 0.1;//Unit()は単位行列
	/*soFoot = scene->CreateSolid(sd);
	soFoot->AddShape(boxFoot);
	soDX1 = scene->CreateSolid(sd);
	soDX1->AddShape(boxDX1);
	soDX2 = scene->CreateSolid(sd);
	soDX2->AddShape(boxDX2);
	soDX3 = scene->CreateSolid(sd);
	soDX3->AddShape(boxDX3);*/
	
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

	// クランク
	PHHingeJointDesc jd;
	jd.poseSocket = base;
	jd.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);
	jntCrank = scene->CreateJoint(body, soCrank, jd)->Cast();
	jntCrank->SetDamper(1.0);
	scene->CreateTreeNode(root, soCrank);
	
	const double K = 90.0, D = 10.0;
	
	Posed pose;
	PHTreeNodeIf* node;
	for(int i = 0; i < 2; i++){

		/*jntDX3 = scene->CreateJoint(body, soDX3, jd)->Cast();
		jntDX3->SetDamper(D);
		jntDX3->SetSpring(K);
		jntDX3->SetSpringOrigin(Rad(-90.0));
		node = scene->CreateTreeNode(root, soDX3);*/

		pose.Pos() = Vec3d(0.9, 0.4, (i == 0 ? 0.06 : -0.06));//脚の付け根の位置を設定？ iが0だったら0.06、それ以外は-0.06
		jd.poseSocket = base * pose;//軸の位置を指定？？
		jd.posePlug.Pos() = Vec3d(0.0, 0.4, (i == 0 ? -0.06 : 0.06));//ここの意味が良く分からん。iが0だったら-0.06、それ以外は0.06
		jntGuideBody[i] = scene->CreateJoint(body, soGuide[i], jd)->Cast();//関節を作成
		jntGuideBody[i]->SetDamper(D);
		jntGuideBody[i]->SetSpring(K);
		jntGuideBody[i]->SetSpringOrigin(Rad(-90.0));//ここも良く分からん
		node = scene->CreateTreeNode(root, soGuide[i]);
			
		jd.poseSocket.Ori() = Quaterniond();
		jd.poseSocket.Pos() = Vec3d(0.0, -0.4, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, 0.7, 0.0);
		jntFootGuide[i] = scene->CreateJoint(soGuide[i], soFoot[i], jd)->Cast();
		scene->CreateTreeNode(node, soFoot[i]);
		jntFootGuide[i]->SetSpring(K);
		jntFootGuide[i]->SetDamper(D);
		jntFootGuide[i]->SetSpringOrigin(Rad(90.0));
	}
	
	// しばしお待ちあれ
	double dt = scene->GetTimeStep();
	double T = 0.3;
	for(double t = 0.0; t < T; t+=dt)
		scene->Step();

	// バネ解除
	jntGuideBody[0]->SetSpring(0.0);
	jntGuideBody[1]->SetSpring(0.0);
	jntFootGuide[0]->SetSpring(0.0);
	jntFootGuide[1]->SetSpring(0.0);

	// 閉リンクの構成
	for(int i = 0; i < 2; i++){
		jd.poseSocket = Posed();
		jd.poseSocket.Pos() = Vec3d(0.0, (i == 0 ? 0.1 : -0.1), (i == 0 ? 0.06: -0.06));
		jd.posePlug.Pos() = Vec3d(0.0, -0.1+0.25, (i == 0 ? -0.06 : 0.06));
		jntFoot[i] = scene->CreateJoint(soCrank, soFoot[i], jd)->Cast();
	}
	
	scene->SetContactMode(soCrank, soFoot[0], PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soCrank, soFoot[1], PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soCrank, soGuide[0], PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soCrank, soGuide[1], PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soGuide[0], soFoot[0], PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soGuide[1], soFoot[1], PHSceneDesc::MODE_NONE);
}


void Robot::Build(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk){

	CDConvexMeshDesc md;                                                    // メッシュディスクリプタ(md)
    md.vertices.push_back(Vec3f( 0.0, 0.2, 0.6));							// 六角柱の頂点座標を登録
    md.vertices.push_back(Vec3f( 0.52,0.2, 0.3));	
    md.vertices.push_back(Vec3f( 0.52,0.2,-0.3));	
    md.vertices.push_back(Vec3f( 0.0, 0.2,-0.6));
    md.vertices.push_back(Vec3f(-0.52,0.2,-0.3));	
    md.vertices.push_back(Vec3f(-0.52,0.2, 0.3));
    md.vertices.push_back(Vec3f( 0.0, -0.2, 0.6));
    md.vertices.push_back(Vec3f( 0.52,-0.2, 0.3));	
    md.vertices.push_back(Vec3f( 0.52,-0.2,-0.3));	
    md.vertices.push_back(Vec3f( 0.0, -0.2,-0.6));
    md.vertices.push_back(Vec3f(-0.52,-0.2,-0.3));	
    md.vertices.push_back(Vec3f(-0.52,-0.2, 0.3));

	/*CDConvexMeshIf* mesh1 = DCAST(CDConvexMeshIf, sdk->CreateShape(md));
	PHSolidIf* solid1 = scene->CreateSolid();							// 剛体(solid1)を作成
    solid1->AddShape(mesh1);*/

	boxBody = sdk->CreateShape(md)->Cast();

	/*CDBoxDesc bd;
	bd.boxsize = Vec3f(0.8, 0.6, 1.5);
	boxBody = sdk->CreateShape(bd)->Cast();*/
	
	PHSolidDesc sd;
	soBody = scene->CreateSolid(sd);
	soBody->AddShape(boxBody);
	soBody->SetPose(pose);
	soBody->SetDynamical(false);
	PHRootNodeIf* root = scene->CreateRootNode(soBody);

	Posed poseLeg;
	poseLeg.Ori() = Quaterniond::Rot(Rad(60.0), 'y');
	poseLeg.Pos() = Vec3d(-0.75, -0.4, 1.3);//原点に対する”クランク（アクチュエータ）の”位置
	leg[0].Build(soBody, root, poseLeg, scene, sdk);
	poseLeg.Ori() = Quaterniond::Rot(Rad(-60.0), 'y');
	poseLeg.Pos() = Vec3d(-0.75, -0.4, -1.3);
	leg[2].Build(soBody, root, poseLeg, scene, sdk);
	
	poseLeg.Ori() = Quaterniond::Rot(Rad(180.0), 'y');//脚の生える向きを反転
	poseLeg.Pos() = Vec3d(1.45, -0.4, 0.0);
	leg[1].Build(soBody, root, poseLeg, scene, sdk);
	//poseLeg.Pos() = Vec3d(1.2, -0.3, -1.0);
	//leg[3].Build(soBody, root, poseLeg, scene, sdk);

	vector<PHSolidIf*> group;
	group.push_back(soBody);
	for(int i = 0; i < 3; i++){
		group.push_back(leg[i].soCrank);
		group.push_back(leg[i].soFoot[0]);
		group.push_back(leg[i].soFoot[1]);
		group.push_back(leg[i].soGuide[0]);
		group.push_back(leg[i].soGuide[1]);
	}
	scene->SetContactMode(&group[0], group.size(), PHSceneDesc::MODE_NONE);

	soBody->SetDynamical(true);
}


const double speed = 20.0;
void Robot::Stop(){
	leg[0].jntCrank->SetMotorTorque(0);
	leg[1].jntCrank->SetMotorTorque(0);
	leg[2].jntCrank->SetMotorTorque(0);
	//leg[3].jntCrank->SetMotorTorque(0);
}

void Robot::Forward(){
	leg[0].jntCrank->SetMotorTorque(speed);
	leg[1].jntCrank->SetMotorTorque(-speed);
	leg[2].jntCrank->SetMotorTorque(speed);
	//leg[3].jntCrank->SetMotorTorque(-speed);
}

void Robot::Backward(){
	leg[0].jntCrank->SetMotorTorque(-speed);
	leg[1].jntCrank->SetMotorTorque(speed);
	leg[2].jntCrank->SetMotorTorque(-speed);
	//leg[3].jntCrank->SetMotorTorque(speed);
}

void Robot::TurnLeft(){
	leg[0].jntCrank->SetMotorTorque(speed);
	leg[1].jntCrank->SetMotorTorque(-speed);
	leg[2].jntCrank->SetMotorTorque(-speed);
	//leg[3].jntCrank->SetMotorTorque(speed);
}

void Robot::TurnRight(){
	leg[0].jntCrank->SetMotorTorque(-speed);
	leg[1].jntCrank->SetMotorTorque(speed);
	leg[2].jntCrank->SetMotorTorque(speed);
	//leg[3].jntCrank->SetMotorTorque(-speed);
}

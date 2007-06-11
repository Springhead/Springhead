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

//ロボットの脚を構築
void Robot::Leg::Build(PHSolidIf* body, PHRootNodeIf* root, const Posed& base, PHSceneIf* scene, PHSdkIf* sdk){

	//部品のサイズを指定する
	CDBoxDesc bd;
	bd.boxsize = Vec3f(0.005, 0.12, 0.038);//ASHIGARU足先アルミ板部分
	boxFoot = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.026, 0.067, 0.032);//ダイナミクセル１
	boxDX1 = sdk->CreateShape(bd)->Cast();
	bd.boxsize = Vec3f(0.026, 0.067, 0.032);//ダイナミクセル２
	boxDX2 = sdk->CreateShape(bd)->Cast();

	CDSphereDesc sp;						//足先の球
	sp.radius = 0.008;
	Sphere = sdk->CreateShape(sp)->Cast();

	//部品のソリッドモデルを作成する
	PHSolidDesc sd;
	sd.mass = 0.04;//単位Kg??
	sd.inertia = Matrix3d::Unit() * 0.1;//Unit()は単位行列
	soFoot = scene->CreateSolid(sd);
	soFoot->AddShape(boxFoot);
	sd.mass = 0.09;
	soDX1 = scene->CreateSolid(sd);
	soDX1->AddShape(boxDX1);
	soDX2 = scene->CreateSolid(sd);
	soDX2->AddShape(boxDX2);
	sd.mass = 0.001;
	soSphere = scene->CreateSolid(sd);
	soSphere->AddShape(Sphere);

	PHHingeJointDesc jd;
	jd.poseSocket = base;
	jd.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);//これは何の意味？？変えても何も変化が無い。
	
	const double K = 100.0, D = 10.0;
	
	Posed pose;
	PHTreeNodeIf* node;

	//第一（付け根）関節
		pose.Ori() = Quaterniond::Rot(Rad(90.0), 'x');//ヨー軸関節
		pose.Pos() = Vec3d(0.09, 0.04, 0.0);//脚の付け根の位置を設定
		jd.poseSocket = base * pose;		//軸の位置を指定???
		jd.posePlug.Pos() = Vec3d(0.0, 0.04, 0.0);
		jntDX1 = scene->CreateJoint(body, soDX1, jd)->Cast();//関節を作成
		jntDX1->SetDamper(D);
		jntDX1->SetSpring(K);
		//jntDX1->SetSpringOrigin(Rad(0.0));
		jntDX1->SetSpringOrigin(Rad(-90.0));
		node = scene->CreateTreeNode(root, soDX1);
		
	//第二関節
		jd.poseSocket.Ori() = Quaterniond::Rot(Rad(-90.0), 'y');//ピッチ軸関節
		jd.poseSocket.Pos() = Vec3d(0.0, -0.033, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, 0.033, 0.0);
		jntDX2 = scene->CreateJoint(soDX1, soDX2, jd)->Cast();
		scene->CreateTreeNode(node, soDX2);
		jntDX2->SetSpring(K);
		jntDX2->SetDamper(D);
		jntDX2->SetSpringOrigin(Rad(60.0));
		//PHRootNodeIf* DX2 = scene->CreateRootNode(soDX2);
		//node = scene->CreateTreeNode(root, soDX2);

	//第三関節（足先アルミ板＋球）
		jd.poseSocket.Ori() = Quaterniond::Rot(Rad(0.0), 'y');
		jd.poseSocket.Pos() = Vec3d(0.0, -0.05, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, 0.05, 0.0);
		jntFoot = scene->CreateJoint(soDX2, soFoot, jd)->Cast();
		//scene->CreateTreeNode(node, soFoot);
		jntFoot->SetSpring(K);
		jntFoot->SetDamper(D);
		jntFoot->SetSpringOrigin(Rad(30.0));
		//node = scene->CreateTreeNode(root, soFoot);

	//関節の可動範囲を設定
		//jntDX1->SetRange(Rad(-180.0),Rad(0.0));			//<-何故かこの１行を入れると初期描画がうまくいかない。なんで？？
		jntDX2->SetRange(Rad(-90.0),Rad(90.0));
	
	// しばしお待ちあれ
	double dt = scene->GetTimeStep();
	double T = 0.5;
	for(double t = 0.0; t < T; t+=dt)
		scene->Step();

	// バネ解除
	jntDX1->SetSpring(0.0);
	jntDX2->SetSpring(0.0);
	jntFoot->SetSpring(0.0);

	//soFoot->SetDynamical(true);

	scene->SetContactMode(soDX1, soDX2, PHSceneDesc::MODE_NONE);
	scene->SetContactMode(soDX2, soFoot, PHSceneDesc::MODE_NONE);
											
											/*scene->SetContactMode(soCrank, soFoot[0], PHSceneDesc::MODE_NONE);
											scene->SetContactMode(soCrank, soFoot[1], PHSceneDesc::MODE_NONE);
											scene->SetContactMode(soCrank, soGuide[0], PHSceneDesc::MODE_NONE);
											scene->SetContactMode(soCrank, soGuide[1], PHSceneDesc::MODE_NONE);
											scene->SetContactMode(soGuide[0], soFoot[0], PHSceneDesc::MODE_NONE);
											scene->SetContactMode(soGuide[1], soFoot[1], PHSceneDesc::MODE_NONE);*/
}

//ロボットを構築
void Robot::Build(int module_num, const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk){

	CDConvexMeshDesc md;                                                    // メッシュディスクリプタ(md)
    md.vertices.push_back(Vec3f( 0.0,   0.02, 0.06));							// 六角柱の頂点座標を登録
    md.vertices.push_back(Vec3f( 0.052, 0.02, 0.03));	
    md.vertices.push_back(Vec3f( 0.052, 0.02,-0.03));	
    md.vertices.push_back(Vec3f( 0.0,   0.02,-0.06));
    md.vertices.push_back(Vec3f(-0.052, 0.02,-0.03));	
    md.vertices.push_back(Vec3f(-0.052, 0.02, 0.03));
    md.vertices.push_back(Vec3f( 0.0,  -0.02, 0.06));
    md.vertices.push_back(Vec3f( 0.052,-0.02, 0.03));	
    md.vertices.push_back(Vec3f( 0.052,-0.02,-0.03));	
    md.vertices.push_back(Vec3f( 0.0,  -0.02,-0.06));
    md.vertices.push_back(Vec3f(-0.052,-0.02,-0.03));	
    md.vertices.push_back(Vec3f(-0.052,-0.02, 0.03));

	boxBody = sdk->CreateShape(md)->Cast();
	
	PHSolidDesc sd;
	soBody = scene->CreateSolid(sd);
	soBody->AddShape(boxBody);
	soBody->SetPose(pose);
	soBody->SetDynamical(false);
	PHRootNodeIf* root = scene->CreateRootNode(soBody);

	Posed poseLeg;
	poseLeg.Ori() = Quaterniond::Rot(Rad(60.0), 'y');
	poseLeg.Pos() = Vec3d(-0.075, -0.04, 0.13);//原点に対する”クランク（アクチュエータ）の”位置?
	leg[module_num][0].Build(soBody, root, poseLeg, scene, sdk);
	poseLeg.Ori() = Quaterniond::Rot(Rad(180.0), 'y');//脚の生える向きを反転
	poseLeg.Pos() = Vec3d(0.145, -0.04, 0.0);
	leg[module_num][1].Build(soBody, root, poseLeg, scene, sdk);
	poseLeg.Ori() = Quaterniond::Rot(Rad(-60.0), 'y');
	poseLeg.Pos() = Vec3d(-0.075, -0.04, -0.13);
	leg[module_num][2].Build(soBody, root, poseLeg, scene, sdk);
	
	vector<PHSolidIf*> group;
	group.push_back(soBody);

	for(int i = 0; i < 3; i++){
		group.push_back(leg[module_num][i].soDX1);
		group.push_back(leg[module_num][i].soDX2);
		group.push_back(leg[module_num][i].soFoot);
		group.push_back(leg[module_num][i].soSphere);
	}
	/*for(int i = 0; i < 3; i++){
		group.push_back(leg[i].soCrank);
		group.push_back(leg[i].soFoot[0]);
		group.push_back(leg[i].soFoot[1]);
		group.push_back(leg[i].soGuide[0]);
		group.push_back(leg[i].soGuide[1]);
	}*/
	scene->SetContactMode(&group[0], group.size(), PHSceneDesc::MODE_NONE);
	for(int i = 0; i < 3; i++){
		scene->SetContactMode(leg[module_num][i].soDX1, soBody, PHSceneDesc::MODE_NONE);
		scene->SetContactMode(leg[module_num][i].soDX2, soBody, PHSceneDesc::MODE_NONE);
		scene->SetContactMode(leg[module_num][i].soFoot, soBody, PHSceneDesc::MODE_NONE);
		scene->SetContactMode(leg[module_num][i].soSphere, soBody, PHSceneDesc::MODE_NONE);
	}

	//soBody->SetDynamical(true);
}

const double speed = 40.0;

void Robot::Stop(int module_num){
	leg[module_num][0].jntDX1->SetMotorTorque(0);
	leg[module_num][1].jntDX1->SetMotorTorque(0);
	leg[module_num][2].jntDX1->SetMotorTorque(0);
	leg[module_num][0].jntDX2->SetMotorTorque(0);
	leg[module_num][1].jntDX2->SetMotorTorque(0);
	leg[module_num][2].jntDX2->SetMotorTorque(0);
	leg[module_num][0].jntFoot->SetMotorTorque(0);
	leg[module_num][1].jntFoot->SetMotorTorque(0);
	leg[module_num][2].jntFoot->SetMotorTorque(0);
	//leg[3].jntCrank->SetMotorTorque(0);
}
void Robot::Forward(int module_num){
	leg[module_num][0].jntDX1->SetMotorTorque(-speed);
	leg[module_num][1].jntDX1->SetMotorTorque(-speed);
	leg[module_num][2].jntDX1->SetMotorTorque(-speed);
	leg[module_num][0].jntDX2->SetMotorTorque(speed);
	leg[module_num][1].jntDX2->SetMotorTorque(speed);
	leg[module_num][2].jntDX2->SetMotorTorque(speed);
	leg[module_num][0].jntFoot->SetMotorTorque(speed);
	leg[module_num][1].jntFoot->SetMotorTorque(speed);
	leg[module_num][2].jntFoot->SetMotorTorque(speed);
	//leg[3].jntCrank->SetMotorTorque(-speed);
}

void Robot::Backward(int module_num){
	leg[module_num][0].jntDX1->SetMotorTorque(speed);
	leg[module_num][1].jntDX1->SetMotorTorque(speed);
	leg[module_num][2].jntDX1->SetMotorTorque(speed);
	leg[module_num][0].jntDX2->SetMotorTorque(-speed);
	leg[module_num][1].jntDX2->SetMotorTorque(-speed);
	leg[module_num][2].jntDX2->SetMotorTorque(-speed);
	leg[module_num][0].jntFoot->SetMotorTorque(-speed);
	leg[module_num][1].jntFoot->SetMotorTorque(-speed);
	leg[module_num][2].jntFoot->SetMotorTorque(-speed);
	//leg[3].jntCrank->SetMotorTorque(speed);
}

/*void Robot::TurnLeft(){
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
}*/

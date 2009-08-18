/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageJoints �֐ߋ@�\�̃f��
 Springhead2/src/Samples/Joints/main.cpp

\ref pagePhysics �̊֐ߋ@�\�̃f���v���O�����B

\section secTestPatternJoints �e�X�g�p�^�[��
- �V�[��0�F�q���W�̍�
- �V�[��1�F�`�F�r�V�F�t�����N
- �V�[��2�F�{�[���W���C���g�̍�
- �V�[��3�F�X���C�_�̍�
- �V�[��4�F�p�X�W���C���g
- �V�[��5�F�o�l�_���p�i����ȃo�l�_���p�̊m�F�f���j 
 
\section secSpecJoints �d�l
- ���ʂ̑���F
 - �V�[���ԍ�0, 1, ...���^�C�v����ƑΉ�����V�[���ɐ؂�ւ��i�f�t�H���g�F�V�[��0)
  - 'x'���^�C�v����Ɗ֐߂̗L��/�������؂�ւ��
  - 'z'�ŁA�E��������{�b�N�X���΂��A�Փ˂�����		
  - 'Z'�ŁA��O��������{�b�N�X���΂��A�Փ˂�����
  - 'C'�ŁA�E��������J�v�Z�����΂��A�Փ˂�����
  - 'S'�ŁA�E�������狅���΂��A�Փ˂�����
  - 'P'�ŁA�V�~�����[�V�������~�߂�
- �V�[��0�F
  - '0'�ŁA�q���W�V�[���̐ݒ���s���i�V�[���؊����j
  - ' '���邢��'b'�Ńq���W�p��box�𐶐�
		�@' '�̏ꍇ�͓����A���S���Y����ABA�ƂȂ�
- �V�[��1�F
  - '1'�ŁA�`�F�r�V�F�t�����N�𐶐�����i�V�[���؊����j
  - 'a'�ŁA���[�^�[�g���N��1.0�ɐݒ肷��
  - 's'�ŁA���[�^�[�g���N��0.0�ɐݒ肷��
  - 'd'�ŁA���[�^�[�g���N��-1.0�ɐݒ肷��
  - 'f'�ŁA�ڕW���x��90.0�ɐݒ肷��
  - 'g'�ŁA�ڕW���x��0.0�ɐݒ肷��
  - 'h'�ŁA�ڕW���x��-90.0�ɐݒ肷��
  - 'j'�ŁA�o�l���_(�o�l�̗͂�0�ƂȂ�֐ߕψ�)��1.0�ɐݒ肷��
  - 'k'�ŁA�o�l���_(�o�l�̗͂�0�ƂȂ�֐ߕψ�)��0.0�ɐݒ肷��
  - 'l'�ŁA�o�l���_(�o�l�̗͂�0�ƂȂ�֐ߕψ�)��-1.0�ɐݒ肷��
- �V�[��2�F
  - '2'�ŁA�{�[���W���C���g�V�[���̐ݒ���s���i�V�[���؊����j
  - ' '���邢��'b'�Ńq���W�p��box�𐶐�
		�@' '�̏ꍇ�͓����A���S���Y����ABA�ƂȂ�
- �V�[��3�F
  - '3'�ŁA�X���C�_�V�[���̐ݒ���s���i�V�[���؊����j
  - 'a'�ŁA�d�͕������A(5.0, -5, 0.0)�ɐݒ肷��
  - 'd'�ŁA�d�͕������A(-5.0, -5, 0.0)�ɐݒ肷��
  - �X�y�[�X�L�[�ŃX���C�_�p��box�𐶐�		
- �V�[��4�F
  - '4'�ŁA�p�X�W���C���g�V�[���̐ݒ���s���i�V�[���؊����j
  - 'a'�ŁA�d�͕������A(0.0, -9.8, 0.0)�ɐݒ肷��
  - 'd'�ŁA�d�͕������A(0.0,  9.8, 0.0)�ɐݒ肷��
  - 
- �V�[��5�F
  - '5'�ŁA�o�l�E�_���p�V�[���̐ݒ���s���i�V�[���؊����j
  - 'a'�ŁA���̂̃t���[�����_���A�ʒu(-20.0, 30.0, 0.0)�ɐݒ肷��
  - 's'�ŁA���̂̃t���[�����_���A�ʒu(-10.0, 20.0, 0.0)�ɐݒ肷��
  - 'd'�ŁA���̂̃t���[�����_���A�ʒu( -5.0, 10.0, 0.0)�ɐݒ肷��
  - 'f'�ŁA���̂̃t���[�����_���A�ʒu(  0.0, 10.0, 0.0)�ɐݒ肷��
  - 'g'�ŁA���̂̃t���[�����_���A�ʒu(  5.0, 10.0, 0.0)�ɐݒ肷��
  - 'h'�ŁA���̂̃t���[�����_���A�ʒu( 10.0, 20.0, 0.0)�ɐݒ肷��
  - 'j'�ŁA���̂̃t���[�����_���A�ʒu( 20.0, 30.0, 0.0)�ɐݒ肷��
  - '.'�ŁA�R�[���o�b�N�֐��̌Ăяo�����Ԃ�1/2�{����
  - ','�ŁA�R�[���o�b�N�֐��̌Ăяo�����Ԃ�2�{����
  - 'n'�ŁA�o�l���_��+0.01����
  - 'm'�ŁA�o�l���_��-0.01����
*/

#include <ctime>
#include <string>
#include <sstream>
#include <Springhead.h>		//	Springhead�̃C���^�t�F�[�X
#include <GL/glut.h>	
#include <Foundation/Object.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;
using namespace std;

#define ESC		27

UTRef<PHSdkIf> phSdk;			// SDK�C���^�t�F�[�X
UTRef<GRSdkIf> grSdk;
UTRef<PHSceneIf> scene;		// Scene�C���^�t�F�[�X
UTRef<GRDebugRenderIf> render;
UTRef<GRDeviceGLIf> device;
UTRef<ObjectStatesIf> state;

double simulationPeriod = 50.0;
Vec3d lookAt;
int sceneNo	= 6;							// �V�[���ԍ�
bool bAutoStep = true;	//	�����X�e�b�v
double	CameraRotX = 0.0, CameraRotY = Rad(80.0), CameraZoom = 30.0;
bool bLeftButton = false, bRightButton = false;

PHSolidDesc descFloor;					//�����̂̃f�B�X�N���v�^
PHSolidDesc solidDesc;					//�����̂̃f�B�X�N���v�^
CDShapeIf* shapeBox;
CDShapeIf* shapeSphere;

PHSolidIf* soFloor;						//�����̂̃C���^�t�F�[�X
std::vector<PHSolidIf*> soBox;			//�����̂̃C���^�t�F�[�X
std::vector<PHJointIf*> jntLink;		//�֐߂̃C���^�t�F�[�X
std::vector<PHTreeNodeIf*> nodeTree;
PHHingeJointIf* hingeIf;
PHHingeJointDesc hingeDesc;

void CreateFloor(bool s=true){
	CDConvexMeshDesc desc;
	desc.vertices.push_back(Vec3f( 15, 2, 10));
	desc.vertices.push_back(Vec3f(-15, 2, 10));
	desc.vertices.push_back(Vec3f( 15,-2, 10));
	desc.vertices.push_back(Vec3f( 15, 2,-10));
	desc.vertices.push_back(Vec3f( 15,-2,-10));
	desc.vertices.push_back(Vec3f(-15, 2,-10));
	desc.vertices.push_back(Vec3f(-15,-2, 10));
	desc.vertices.push_back(Vec3f(-15,-2,-10));
	soFloor = scene->CreateSolid(descFloor);
	if (s) soFloor->AddShape(phSdk->CreateShape(desc));
	soFloor->SetFramePosition(Vec3f(0,-2,0));
	soFloor->SetDynamical(false);			// ���͊O�͂ɂ���ē����Ȃ��悤�ɂ���
}

// �V�[��0 : ���̃f���Bspace key�Ŕ���������
void BuildScene0(){	
	CreateFloor();
	//���̍��ɂȂ锠���쐬
	CDConvexMeshDesc bd;
	bd.vertices.push_back(Vec3f(-1.0, -1.0, -1.0));
	bd.vertices.push_back(Vec3f( 1.0, -1.0, -1.0));
	bd.vertices.push_back(Vec3f(-1.0,  1.0, -1.0));
	bd.vertices.push_back(Vec3f(-1.0, -1.0,  1.0));
	bd.vertices.push_back(Vec3f(-1.0,  1.0,  1.0));
	bd.vertices.push_back(Vec3f( 1.0, -1.0,  1.0));
	bd.vertices.push_back(Vec3f( 1.0,  1.0, -1.0));
	bd.vertices.push_back(Vec3f( 1.0,  1.0,  1.0));
	shapeBox = phSdk->CreateShape(bd);
	shapeBox->SetName("shapeBox");
	soBox.push_back(scene->CreateSolid(solidDesc));
	soBox.back()->AddShape(shapeBox);
	//�󒆂ɌŒ肷��
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));
	soBox.back()->SetDynamical(false);
	nodeTree.push_back(scene->CreateRootNode(soBox.back(), PHRootNodeDesc()));

PHSolidIf*	so = scene->CreateSolid(solidDesc);
	so->AddShape(shapeBox);
	so->SetFramePosition(Vec3f(2.0, 20.0, 0.0));
	so->SetDynamical(false);

	so = scene->CreateSolid(solidDesc);
	so->AddShape(shapeBox);
	so->SetFramePosition(Vec3f(0.0, 22.0, 0.0));
	so->SetDynamical(false);

	so = scene->CreateSolid(solidDesc);
	so->AddShape(shapeBox);
	so->SetFramePosition(Vec3f(0.0, 20.0, 2.0));
	so->SetDynamical(false);

	// �d�͂�ݒ�
	scene->SetGravity(Vec3f(0, -9.8, 0));
}

// �V�[��1 : �A�N�`���G�[�^�̃f��
void BuildScene1(){
	CreateFloor();

	CDBoxDesc bd;
	soBox.resize(3);
	bd.boxsize = Vec3f(1.0, 2.0, 1.0);
	soBox[0] = scene->CreateSolid(solidDesc);
	soBox[0]->AddShape(phSdk->CreateShape(bd));
	soBox[0]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	bd.boxsize = Vec3f(1.0, 5.0, 1.0);
	soBox[1] = scene->CreateSolid(solidDesc);
	soBox[1]->AddShape(phSdk->CreateShape(bd));
	soBox[1]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	bd.boxsize = Vec3f(1.0, 10.0, 1.0);
	soBox[2] = scene->CreateSolid(solidDesc);
	soBox[2]->AddShape(phSdk->CreateShape(bd));
	soBox[2]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	PHHingeJointDesc jd;
	jntLink.resize(4);
	jd.poseSocket.Pos() = Vec3d(0.0, 10.0, 0.0);
	jd.posePlug.Pos() = Vec3d(0.0, -1.0, 0.0);
	jntLink[0] = scene->CreateJoint(soFloor, soBox[0], jd);

	jd.poseSocket.Pos() = Vec3d(4.0, 10.0, 0.0);
	jd.posePlug.Pos() = Vec3d(0.0, -2.5, 0.0);
	jntLink[1] = scene->CreateJoint(soFloor, soBox[1], jd);

	jd.poseSocket.Pos() = Vec3d(0.0, 1.0, 0.0);
	jd.posePlug.Pos() = Vec3d(0.0, -5.0, 0.0);
	jntLink[2] = scene->CreateJoint(soBox[0], soBox[2], jd);

	jd.poseSocket.Pos() = Vec3d(0.0, 2.5, 0.0);
	jd.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);
	jntLink[3] = scene->CreateJoint(soBox[1], soBox[2], jd);

	// �ȉ���L���������ABA���@�\���A�����N���\�����邽�߂�1�֐߂̂�LCP�ŉ������
	nodeTree.push_back(scene->CreateRootNode(soFloor, PHRootNodeDesc()));
	nodeTree.push_back(scene->CreateTreeNode(nodeTree[0], soBox[0], PHTreeNodeDesc()));
	nodeTree.push_back(scene->CreateTreeNode(nodeTree[1], soBox[2], PHTreeNodeDesc()));
	nodeTree.push_back(scene->CreateTreeNode(nodeTree[0], soBox[1], PHTreeNodeDesc()));

	scene->SetContactMode(&soBox[0], 3, PHSceneDesc::MODE_NONE);
	scene->SetGravity(Vec3f(0, -9.8, 0));

	scene->Print(DSTR);
}

void BuildScene2(){
	CreateFloor();
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(solidDesc));
	soBox[0]->AddShape(shapeBox);
	soBox[0]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));
	soBox[0]->SetDynamical(false);
	nodeTree.push_back(scene->CreateRootNode(soBox[0], PHRootNodeDesc()));
	scene->SetGravity(Vec3f(0, -9.8, 0));	
}

void BuildScene3(){
	CreateFloor();
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	solidDesc.mass=10.0;
	solidDesc.inertia = 10 * Matrix3d::Unit();
	soBox.push_back(scene->CreateSolid(solidDesc));
	soBox.back()->AddShape(shapeBox);
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));
	soBox.back()->SetDynamical(false);
	scene->SetGravity(Vec3f(0, -9.8, 0));	
}

void BuildScene4(){
	CreateFloor();
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	soBox.push_back(scene->CreateSolid(solidDesc));
	soBox.back()->AddShape(shapeBox);
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	PHPathDesc desc;
	PHPathIf* path = scene->CreatePath(desc);
	double s;
	double radius = 5.0;
	double pitch = 4.0;
	Posed pose;
	for(s = 0.0; s < 4 * (2 * M_PI); s += Rad(1.0)){
		double stmp = s;
		while(stmp > M_PI) stmp -= 2 * M_PI;
		pose.Pos() = Vec3d(radius * cos(stmp), 5.0 + pitch * s / (2 * M_PI), radius * sin(stmp));
		pose.Ori().FromMatrix(Matrix3d::Rot(-stmp, 'y'));
		path->AddPoint(s, pose);
	}
	PHPathJointDesc descJoint;
	jntLink.push_back(scene->CreateJoint(soFloor, soBox[0], descJoint));
	PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[0]);
	joint->AddChildObject(path);
	joint->SetPosition(2 * 2 * M_PI);
	PHTreeNodeIf* node = scene->CreateRootNode(soFloor, PHRootNodeDesc());
	scene->CreateTreeNode(node, soBox[0], PHTreeNodeDesc());
	
	scene->SetGravity(Vec3f(0, -9.8, 0));
}

void BuildScene5(){
	CreateFloor(false);
	
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 6.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	
	soBox.resize(6);

	soBox[0] = scene->CreateSolid(solidDesc);
	soBox[0]->AddShape(shapeBox);

	soBox[1] = scene->CreateSolid(solidDesc);
	soBox[1]->AddShape(shapeBox);

	soBox[2] = scene->CreateSolid(solidDesc);
	soBox[2]->AddShape(shapeBox);

	soBox[3] = scene->CreateSolid(solidDesc);
	soBox[3]->AddShape(shapeBox);

	soBox[4] = scene->CreateSolid(solidDesc);
	soBox[4]->AddShape(shapeBox);

	CDSphereDesc descSphere;
	descSphere.radius = 1.0;
	soBox[5] = scene->CreateSolid(solidDesc);
	soBox[5]->AddShape(phSdk->CreateShape(descSphere));

	jntLink.resize(6);
	PHHingeJointDesc descHinge;
	descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
	jntLink[0] = scene->CreateJoint(soFloor, soBox[0], descHinge);

	descHinge.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
	descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
	jntLink[1] = scene->CreateJoint(soBox[0], soBox[1], descHinge);
	jntLink[2] = scene->CreateJoint(soBox[1], soBox[2], descHinge);
	jntLink[3] = scene->CreateJoint(soBox[2], soBox[3], descHinge);
	jntLink[4] = scene->CreateJoint(soBox[3], soBox[4], descHinge);

	//�ȉ���L��������ƍ���ABA�Ōv�Z����Ċ֐߂̃h���t�g���h�����D
	/*
	PHTreeNodeIf* node = scene->CreateRootNode(soFloor, PHRootNodeDesc());
	node = scene->CreateTreeNode(node, soBox[0], PHTreeNodeDesc());
	node = scene->CreateTreeNode(node, soBox[1], PHTreeNodeDesc());
	node = scene->CreateTreeNode(node, soBox[2], PHTreeNodeDesc());
	node = scene->CreateTreeNode(node, soBox[3], PHTreeNodeDesc());
	node = scene->CreateTreeNode(node, soBox[4], PHTreeNodeDesc());
	*/

	double K = 2000, D = 100;
	//double K = 100000, D = 10000;	
	DCAST(PHHingeJointIf, jntLink[0])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[0])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[1])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[1])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[2])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[2])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[3])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[3])->SetDamper(D);
	DCAST(PHHingeJointIf, jntLink[4])->SetSpring(K);
	DCAST(PHHingeJointIf, jntLink[4])->SetDamper(D);
#ifndef USE_EXPLICIT
	PHSpringDesc descSpring;
	descSpring.spring = 500 * Vec3f(1,1,1);
	descSpring.damper = 100 * Vec3f(1,1,1);
	jntLink[5] = scene->CreateJoint(soBox[4], soBox[5], descSpring);
#endif

	soBox[5]->SetFramePosition(Vec3d(10.0, 5.0, 1.0));
	soBox[5]->SetDynamical(false);
	
	scene->SetContactMode(PHSceneDesc::MODE_NONE);	// �ڐG��؂�
	scene->SetGravity(Vec3f(0, -9.8, 0));	
}

void BuildScene6(){
//PHBallJoint�̃f�o�b�O
// #if->�c�A#else->��
#if 1
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 6.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	
	soBox.resize(2);
	soBox[0] = scene->CreateSolid(solidDesc);
	soBox[0]->AddShape(shapeBox);
	soBox[0]->SetDynamical(false);
	soBox[1] = scene->CreateSolid(solidDesc);
	soBox[1]->AddShape(shapeBox);
	scene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);
	scene->SetGravity(Vec3d());

	jntLink.resize(1);
	PHBallJointDesc desc;
	desc.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
	desc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
	desc.posePlug.Pos()   = Vec3d(0.0, -3.0, 0.0);
	desc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
	desc.spring			  = 100;
	desc.damper			  = 10;
//	desc.limitSwing[0]	  = Rad(  0); // swing lower
//	desc.limitSwing[1]	  = Rad( 20); // swing upper
//	desc.limitTwist[0]	  = Rad(-20); // twist lower
//	desc.limitTwist[1]	  = Rad( 20); // twist upper
	//	desc.goal			  = Quaterniond(0, 0, 1, 1);
	jntLink[0] = scene->CreateJoint(soBox[0], soBox[1], desc);
#else
	CDBoxDesc bd;
	bd.boxsize = Vec3f(6.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	
	soBox.resize(2);
	soBox[0] = scene->CreateSolid(solidDesc);
	soBox[0]->AddShape(shapeBox);
	soBox[0]->SetDynamical(false);
	soBox[1] = scene->CreateSolid(solidDesc);
	soBox[1]->AddShape(shapeBox);
	scene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);

	jntLink.resize(1);
	PHBallJointDesc desc;
	desc.poseSocket.Pos() = Vec3d(3.0, 0.0, 0.0);
	desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
	desc.posePlug.Pos()   = Vec3d(-3.0, 0.0, 0.0);
	desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
//	desc.spring			  = 100;
//	desc.damper			  = 10;
//	desc.limitSwing[0]	  = Rad(  0); // swing lower
//	desc.limitSwing[1]	  = Rad( 20); // swing upper
//	desc.limitTwist[0]	  = Rad(-20); // twist lower
//	desc.limitTwist[1]	  = Rad( 20); // twist upper
	//	desc.goal			  = Quaterniond(0, 0, 1, 1);
	jntLink[0] = scene->CreateJoint(soBox[0], soBox[1], desc);
#endif

}

void BuildScene7(){
//PHHingeJoint�̃f�o�b�O
// #if->�c�A#else->��

#if 1
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2.0, 6.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	
	soBox.resize(2);
	soBox[0] = scene->CreateSolid(solidDesc);
	soBox[0]->AddShape(shapeBox);
	soBox[0]->SetDynamical(false);
	soBox[1] = scene->CreateSolid(solidDesc);
	soBox[1]->AddShape(shapeBox);
	scene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);

	jntLink.resize(1);
	PHHingeJointDesc desc;
	desc.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
	desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
	desc.posePlug.Pos()   = Vec3d(0.0, -3.0, 0.0);
	desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
	desc.spring			  = 100;
	desc.damper			  = 10;
	desc.origin			  = Rad(0);
	desc.lower			  = Rad(0);
	desc.upper			  = Rad(60);
	jntLink[0] = scene->CreateJoint(soBox[0], soBox[1], desc);
#else 
	CDBoxDesc bd;
	bd.boxsize = Vec3f(6.0, 2.0, 2.0);
	shapeBox = phSdk->CreateShape(bd);
	
	soBox.resize(2);
	soBox[0] = scene->CreateSolid(solidDesc);
	soBox[0]->AddShape(shapeBox);
	soBox[0]->SetDynamical(false);
	soBox[1] = scene->CreateSolid(solidDesc);
	soBox[1]->AddShape(shapeBox);
	scene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);

	jntLink.resize(1);
	PHHingeJointDesc desc;
	desc.poseSocket.Pos() = Vec3d(3.0, 0.0, 0.0);
	desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
	desc.posePlug.Pos()   = Vec3d(-3.0, 0.0, 0.0);
	desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
	desc.spring			  = 100;
	desc.damper			  = 10;
	desc.origin			  = Rad(0);
	desc.lower			  = Rad(0);
	desc.upper			  = Rad(60);
	jntLink[0] = scene->CreateJoint(soBox[0], soBox[1], desc);
#endif
}

void BuildScene(){
	switch(sceneNo){
	case 0: BuildScene0(); break;
	case 1: BuildScene1(); break;
	case 2: BuildScene2(); break;
	case 3: BuildScene3(); break;
	case 4: BuildScene4(); break;
	case 5: BuildScene5(); break;
	case 6: BuildScene6(); break;
	case 7: BuildScene7(); break;
	}
}

void OnKey0(char key){
	switch(key){
	case ' ':
	case 'b':
		{
		soBox.push_back(scene->CreateSolid(solidDesc));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10, 10, 0));
		PHHingeJointDesc jdesc;
		jdesc.poseSocket.Pos() = Vec3d( 1.1,  -1.1,  0);
		jdesc.posePlug.Pos() = Vec3d(-1.1, 1.1,  0);
		jdesc.lower = Rad(-30.0);
		jdesc.upper = Rad( 30.0);
		jdesc.damper = 2.0;
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		// �c���[�m�[�h���쐬���CABA�Ōv�Z����悤�Ɏw��
		if(key == ' ')
			nodeTree.push_back(scene->CreateTreeNode(nodeTree.back(), soBox[n-1], PHTreeNodeDesc()));
		// �ȉ��̓M�A�̍쐬�R�[�h
		/*if(jntLink.size() >= 2){
			size_t m = jntLink.size();
			PHGearDesc gdesc;
			gdesc.ratio = 1.0;
			scene->CreateGear(DCAST(PHJoint1DIf, jntLink[m-2]), DCAST(PHJoint1DIf, jntLink[m-1]), gdesc);
		}*/
		}break;
	}
}
void display();
void OnKey1(char key){
	const double K = 30.0;
	const double B = 10.0;
	PHHingeJointIf* hinge = DCAST(PHHingeJointIf, jntLink[0]);
	//PHPathJointIf* path = (jntLink.size() == 5 ? DCAST(PHPathJointIf, jntLink[4]) : NULL); 
	switch(key){
	case 'a': hinge->SetMotorTorque(1.0);	break;
	case 's': hinge->SetMotorTorque(0.0);	break;
	case 'd': hinge->SetMotorTorque(-1.0);	break;
	case 'f':
		hinge->SetDesiredVelocity(Rad(180));
		//if(path)
		//	path->SetDesiredVelocity(Rad(90.0));
		break;
	case 'g':
		hinge->SetDesiredVelocity(Rad(0.0));
		//if(path)
		//	path->SetDesiredVelocity(Rad(0.0));
		break;
	case 'h': hinge->SetDesiredVelocity(Rad(-90.0));	break;
	case 'j':
		hinge->SetSpring(K);
		hinge->SetSpringOrigin(1.0);
		hinge->SetDamper(B);
		break;
	case 'k':
		hinge->SetSpring(K);
		hinge->SetSpringOrigin(0.0);
		hinge->SetDamper(B);
		break;
	case 'l':
		hinge->SetSpring(K);
		hinge->SetSpringOrigin(-1.0);
		hinge->SetDamper(B);
		break;
	/*case 'c':{
		//�`�F�r�V�F�t�����N������̋O�Ղ��L�������ăp�X�W���C���g���쐬
		PHPathDesc descPath;
		descPath.bLoop = true;
		PHPathIf* trajectory = scene->CreatePath(descPath);

		hinge->SetSpring(K);
		hinge->SetDamper(B);
		double theta = -Rad(180.0);
		hinge->SetSpringOrigin(theta);
		for(int i = 0; i < 50; i++)
			scene->Step();
		for(; theta < Rad(180.0); theta += Rad(1.0)){
			hinge->SetSpringOrigin(theta);
			for(int i = 0; i < 5; i++)
				scene->Step();
			Posed pose = soFloor->GetPose().Inv() * soBox[2]->GetPose();
			//pose.Ori() = Quaterniond();
			trajectory->AddPoint(theta, pose);
			display();
		}
	
		soBox.resize(4);
		soBox[3] = scene->CreateSolid(solidDesc);
		soBox[3]->AddShape(soBox[2]->GetShape(0));
		soBox[3]->SetFramePosition(Vec3f(10.0, 20.0, 0.0));

		PHPathJointDesc descJoint;
		descJoint.posePlug.Pos().x = 15.0;
		jntLink.resize(5);
		jntLink[4] = scene->CreateJoint(soFloor, soBox[3], descJoint);
		PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[4]);
		joint->AddChildObject(trajectory);
		joint->SetPosition(0);
	
		}break;*/
	}
}

void OnKey2(char key){
	switch(key){
	case ' ':
	case 'b':{
		soBox.push_back(scene->CreateSolid(solidDesc));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));
		PHBallJointDesc jdesc;
		//jdesc.limit[1].upper =  0.2;	// �ő�X�C���O�p
		//jdesc.limit[2].lower= -0.2;	// �c�C�X�g�p�͈�
		//jdesc.limit[2].upper =  0.2;
		jdesc.poseSocket.Pos() = Vec3d(-1.01, -1.01, -1.01);
		jdesc.posePlug.Pos() = Vec3d(1.01, 1.01, 1.01);
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		if(key == ' ')
			nodeTree.push_back(scene->CreateTreeNode(nodeTree.back(), soBox[n-1], PHTreeNodeDesc()));
		//scene->SetContactMode(PHSceneDesc::MODE_NONE);
		}break;
	}
}

void OnKey3(char key){
	switch(key){
	case ' ':{
		soBox.push_back(scene->CreateSolid(solidDesc));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));
		PHSliderJointDesc jdesc;
		jdesc.poseSocket.Pos() = Vec3d(0, -1.1, 0);
		jdesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
		jdesc.posePlug.Pos() = Vec3d(0,  1.1, 0);
		jdesc.posePlug.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		PHSliderJointIf* slider = DCAST(PHSliderJointIf, jntLink.back());
		slider->SetRange(-0.3, 0.3);
		//slider->SetSpring(1000.0);
		//slider->SetDamper(300);
		//slider->SetSpringOrigin(0.0);
		}break;
	case 'a': scene->SetGravity(Vec3f(5.0, -5, 0.0)); break;
	case 'd': scene->SetGravity(Vec3f(-5.0, -5, 0.0)); break;
	}
}

void OnKey4(char key){
	switch(key){
	case 'a': scene->SetGravity(Vec3f(0.0, -9.8, 0.0)); break;
	case 'd': scene->SetGravity(Vec3f(0.0,  9.8, 0.0)); break;
	}
}

//float goal = 0;
void OnKey5(char key){
	switch(key){
	case 'a': soBox[5]->SetFramePosition(Vec3d(-20.0, 30.0, 0.0)); break;
	case 's': soBox[5]->SetFramePosition(Vec3d(-10.0, 20.0, 0.0)); break;
	case 'd': soBox[5]->SetFramePosition(Vec3d( -5.0, 10.0, 0.0)); break;
	case 'f': soBox[5]->SetFramePosition(Vec3d(  0.0, 10.0, 0.0)); break;
	case 'g': soBox[5]->SetFramePosition(Vec3d(  5.0, 10.0, 0.0)); break;
	case 'h': soBox[5]->SetFramePosition(Vec3d( 10.0, 20.0, 0.0)); break;
	case 'j': soBox[5]->SetFramePosition(Vec3d( 20.0, 30.0, 0.0)); break;
	case '.': 
		simulationPeriod /= 2.0;
		if (simulationPeriod < 1) simulationPeriod = 1.0;
		break;
	case ',': 
		simulationPeriod *= 2.0;
		break;
	/*case ' ':{
		//	���̒ǉ�
		soBox.push_back(scene->CreateSolid(solidDesc));
		soBox.back()->AddShape(shapeBox);
		soBox.back()->SetFramePosition(Vec3f(10.0, 10.0, 0.0));

		//	�W���C���g�쐬
		PHHingeJointDesc descHinge;
		descHinge.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
		size_t n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], descHinge));
		DCAST(PHHingeJointIf, jntLink.back())->SetSpring(K6);
		DCAST(PHHingeJointIf, jntLink.back())->SetDamper(D6);

		scene->SetContactMode(PHSceneDesc::MODE_NONE);	// �ڐG��؂�
		}break;*/
	case 'n':
	//	goal += 0.01;
		for(unsigned i=0; i<jntLink.size(); ++i){
			PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
			//if (j) j->SetSpringOrigin(goal);
		}
		break;
	case 'm':
		//goal -= 0.01;
		for(unsigned i=0; i<jntLink.size(); ++i){
			PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
			//if (j) j->SetSpringOrigin(goal);
		}
		break;
	}

}

void OnKey6(char key){
//PHBallJoint�̃f�o�b�O
//#if->goal��ύX, #else->Velocity��^����
#if 1
	PHBallJointDesc ballDesc;
	switch (key){
		case 'a':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond::Rot(Rad(30), 'x');
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl;
			break;
		case 's':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond(0, -1, 0, 0);
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl;
			break;
		case 'd':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond(0, 0, 1, 0);
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl; 
			break;
		case 'f':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond(0, 0, -1, 0);
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl; 
			break;
		case 'w':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond(0, 0, 1, 1);
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl; 
			break;
		case 'i':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond::Rot(Rad(120), 'x');
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl; 
			break;
		case 'o':
			scene->GetJoint(0)->GetDesc(&ballDesc);
			ballDesc.goal = Quaterniond::Rot(Rad(120), 'x') * Quaterniond::Rot(Rad(20), 'y');
			DCAST(PHBallJointIf, scene->GetJoint(0))->SetDesc(&ballDesc);
			if(DCAST(PHBallJointIf, scene->GetJoint(0)))
				DSTR << "set the value" << endl; 
			break;		
		default:
			break;
	}

#else
	switch (key){
		case 'a': 
			soBox[1]->SetVelocity(Vec3d(2.0, 0.0, 0.0));
			break;
		case 's':
			soBox[1]->SetVelocity(Vec3d(-2.0, 0.0, 0.0));
			break;
		case 'd':
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, -2.0));
			break;
		case 'f':
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			break;
		case 'g':
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
			break;
		case 'h':
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
			break;
		case 'w':
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
			break;
		case 'e':
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
			break;
		case 'r':
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, -2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
			break;
		case 't':
			soBox[1]->SetVelocity(Vec3d(2.0, 0.0, 0.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
			break;
		case 'y':
			soBox[1]->SetVelocity(Vec3d(-2.0, 0.0, 0.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
			break;
		case 'u':
			soBox[1]->SetAngularVelocity(Vec3d(2.0, 0.0, 0.0));
			break;

		default :
			break;
	}
#endif

}

void OnKey7(char key){
	PHHingeJointDesc hingeDesc;
	PHHingeJointIf* hiJoint = DCAST(PHHingeJointIf, jntLink[0]);

	switch(key){
		
		// plus value origins
		{
		case 'w':
			DSTR << hiJoint->GetIfInfo()->ClassName() << endl;
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(10);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'e':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(20);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'r':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(30);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'a':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(40);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 's':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(50);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'd':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(60);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'z':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(70);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'x':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(80);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'c':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(90);
			hiJoint->SetDesc(&hingeDesc);
			break;
		}

		// minus value origin
		{
		case 'i':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-10);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'o':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-20);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'p':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-30);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'j':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-40);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'k':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-50);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'l':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-60);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'b':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-70);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'n':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-80);
			hiJoint->SetDesc(&hingeDesc);
			break;
		case 'm':
			hiJoint->GetDesc(&hingeDesc);
			hingeDesc.origin = Rad(-90);
			hiJoint->SetDesc(&hingeDesc);
			break;
		}

	}

}

void OnKey(char key){
	if (key == 'W'){
		scene->WriteState("state.bin");
	}else if (key == 'R'){
		scene->ReadState("state.bin");
	}else{
		switch(sceneNo){
		case 0: OnKey0(key); break;
		case 1: OnKey1(key); break;
		case 2: OnKey2(key); break;
		case 3: OnKey3(key); break;
		case 4: OnKey4(key); break;
		case 5: OnKey5(key); break;
		case 6: OnKey6(key); break;
		case 7: OnKey7(key); break;
		}
	}
}

void OnTimer0(){}
void OnTimer1(){}
void OnTimer2(){}
void OnTimer3(){}
void OnTimer4(){}

void OnTimer5(){
#ifdef USE_EXPLICIT
	Vec3f dVel = Vec3f() - soBox[4]->GetVelocity();
	Vec3f dPos = soBox[5]->GetPose().Pos() - soBox[4]->GetPose().Pos();
	static float K = 500;
	static float B = 15;
	Vec3f force = K*dPos + B*dVel;
	soBox[4]->AddForce(force, soBox[4]->GetPose()*Vec3f(0,3,0));
#endif
//	DSTR << soBox[4]->GetVelocity() << std::endl;
}
void OnTimer6(){
	//soBox[1]->AddTorque(Vec3f(0,0,200));
		

}
void OnTimer7(){
}
void OnTimer(){
	switch(sceneNo){
	case 0: OnTimer0(); break;
	case 1: OnTimer1(); break;
	case 2: OnTimer2(); break;
	case 3: OnTimer3(); break;
	case 4: OnTimer4(); break;
	case 5: OnTimer5(); break;
	case 6: OnTimer6(); break;
	case 7: OnTimer7(); break;
	}
}	

/**
 brief     	glutDisplayFunc�Ŏw�肵���R�[���o�b�N�֐�
 param	 	�Ȃ�
 return 	�Ȃ�
 */
void display(){
	Affinef view;
	double yoffset = 10.0;
	view.Pos() = CameraZoom * Vec3f(
		cos(CameraRotX) * cos(CameraRotY),
		sin(CameraRotX),
		cos(CameraRotX) * sin(CameraRotY));
	view.PosY() += yoffset;
	view.LookAtGL(Vec3f(0.0, yoffset, 0.0), Vec3f(0.0f, 100.0f, 0.0f));
	render->SetViewMatrix(view.inv());

	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
	glutSwapBuffers();
}

/**
 brief     	�����̐ݒ�
 param	 	�Ȃ�
 return 	�Ȃ�
 */
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 0.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 0.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

/**
 brief     	����������
 param	 	�Ȃ�
 return 	�Ȃ�
 */
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	lookAt.x = 30.0;
	lookAt.y = -20.0;
	lookAt.z = 100.0;
	lookAt.x = 3.0;
	lookAt.y = 10.0;
	lookAt.z = 30.0;
	gluLookAt(lookAt.x, lookAt.y, lookAt.z, 
		      0.0, lookAt.y, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	setLight();
}

/**
 brief		glutReshapeFunc�Ŏw�肵���R�[���o�b�N�֐�
 param		<in/--> w�@�@��
 param		<in/--> h�@�@����
 return		�Ȃ�
 */
void reshape(int w, int h){
	// Viewport�Ǝˉe�s���ݒ�
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/**
 brief 		glutKeyboardFunc�Ŏw�肵���R�[���o�b�N�֐� 
 param		<in/--> key�@�@ ASCII�R�[�h
 param 		<in/--> x�@�@�@ �L�[�������ꂽ���̃}�E�X���W
 param 		<in/--> y�@�@�@ �L�[�������ꂽ���̃}�E�X���W
 return 	�Ȃ�
 */
void keyboard(unsigned char key, int x, int y){
	unsigned int i = 0;
	switch (key) {
		//�I��
		case ESC:		
		case 'q':
			exit(0);
			break;
		//�V�[���؂�ւ�
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
			scene->Clear();
			soFloor = NULL;
			soBox.clear();
			jntLink.clear();
			nodeTree.clear();
			sceneNo = key - '0';
			BuildScene();
			break;
		case ',':
			state->SaveState(scene);
			break;
		case '.':
			state->LoadState(scene);
			break;
		case '/':
			state->ReleaseState(scene);
			break;
		case 'x':
			{
				static bool bEnable = true;
				bEnable = !bEnable;
				for(i = 0; i < jntLink.size(); i++)
					jntLink[i]->Enable(bEnable);
			}break;
		case 'z':{
			soBox.push_back(scene->CreateSolid(solidDesc));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3f(15.0, 15.0, 0.0));
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(1), 'z'));
			soBox.back()->SetVelocity(Vec3d(-5.0, 0.0, 0.0));
			soBox.back()->SetMass(2.0);
//			scene->SetContactMode(PHSceneDesc::MODE_PENALTY);
			}break;	
		case 'Z':{
			soBox.push_back(scene->CreateSolid(solidDesc));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3f(5.0, 13.0, 5.0));
			soBox.back()->SetVelocity(Vec3d(0.0, 0.0, -20.0));
			soBox.back()->SetMass(2.0);
			}break;	
		case 'C':{
			CDCapsuleDesc cd;
			cd.radius = 1.0;
			cd.length = 4.0;
			CDShapeIf* shape = phSdk->CreateShape(cd);

			soBox.push_back(scene->CreateSolid(solidDesc));
			soBox.back()->AddShape(shape);
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(90), 'y'));
			soBox.back()->SetFramePosition(Vec3f(15.0, 15.0, 0.0));
			soBox.back()->SetVelocity(Vec3d(-3.0, 0.0, 0.0));
			soBox.back()->SetAngularVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox.back()->SetMass(2.0);
			}break;	
		case 'S':{
			CDSphereDesc sd;
			sd.radius = 1.0;
			CDShapeIf* shape = phSdk->CreateShape(sd);

			soBox.push_back(scene->CreateSolid(solidDesc));
			soBox.back()->AddShape(shape);
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(90), 'y'));
			soBox.back()->SetFramePosition(Vec3f(15.0, 15.0, 0.0));
			soBox.back()->SetVelocity(Vec3d(-3.0, 0.0, 0.0));
			soBox.back()->SetMass(2.0);
			}break;	
		case 'P':
			bAutoStep = false;
			OnTimer();
			scene->ClearForce();
			scene->GenerateForce();
			scene->Integrate();
			glutPostRedisplay();
			break;
		default:
			OnKey(key);
			break;
	}
}

int xlast, ylast;
void mouse(int button, int state, int x, int y){
	xlast = x, ylast = y;
	if(button == GLUT_LEFT_BUTTON)
		bLeftButton = (state == GLUT_DOWN);
	if(button == GLUT_RIGHT_BUTTON)
		bRightButton = (state == GLUT_DOWN);
}

void motion(int x, int y){
	static bool bFirst = true;
	int xrel = x - xlast, yrel = y - ylast;
	xlast = x;
	ylast = y;
	if(bFirst){
		bFirst = false;
		return;
	}
	// ���{�^��
	if(bLeftButton){
		CameraRotY += xrel * 0.01;
		CameraRotY = Spr::max(Rad(-180.0), Spr::min(CameraRotY, Rad(180.0)));
		CameraRotX += yrel * 0.01;
		CameraRotX = Spr::max(Rad(-80.0), Spr::min(CameraRotX, Rad(80.0)));
	}
	// �E�{�^��
	if(bRightButton){
		CameraZoom *= exp(yrel/10.0);
		CameraZoom = Spr::max(0.1, Spr::min(CameraZoom, 100.0));
	}
}

/**
 brief  	glutTimerFunc�Ŏw�肵���R�[���o�b�N�֐�
 param	 	<in/--> id�@�@ �^�C�}�[�̋�ʂ����邽�߂̏��
 return 	�Ȃ�
 */
void timer(int id){
	glutTimerFunc(simulationPeriod, timer, 0);
	if (bAutoStep){
		OnTimer();
		scene->ClearForce();
		scene->GenerateForce();
		scene->Integrate();
		glutPostRedisplay();
		 
	}
	
}
void idle(){
	scene->ClearForce();
	scene->GenerateForce();
	scene->Integrate();
}

/**
 brief		���C���֐�
 param		<in/--> argc�@�@�R�}���h���C�����͂̌�
 param		<in/--> argv�@�@�R�}���h���C������
 return		0 (����I��)
 */
int main(int argc, char* argv[]){
	// SDK�̍쐬

	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();
	state = ObjectStatesIf::Create();
	// �V�[���I�u�W�F�N�g�̍쐬
	PHSceneDesc dscene;
	dscene.timeStep = 0.05;
	dscene.numIteration = 20;
	scene = phSdk->CreateScene(dscene);				// �V�[���̍쐬
	// �V�[���̍\�z
	BuildScene();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	int window = glutCreateWindow("Joints");

	render = grSdk->CreateDebugRender();
	render->SetRenderMode(true, false);
	//render->EnableRenderAxis();
	render->EnableRenderForce();
	device = grSdk->CreateDeviceGL();

	// �����ݒ�
	device->Init();

	glutTimerFunc(20, timer, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//glutIdleFunc(idle);
	
	render->SetDevice(device);	// �f�o�C�X�̐ݒ�

	initialize();

	glutMainLoop();

}
/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "robot1.h"

void Robot1::InitCylinder(CDConvexMeshDesc& md, float height, float radius, int ndiv){
	md.vertices.resize(2 * ndiv);
	float theta, x, y;
	float halfh = 0.5f * height;
	for(int i = 0; i < ndiv; i++){
		theta = 2 * (float)M_PI * float(i)/float(ndiv);
		x = radius * cos(theta);
		y = radius * sin(theta);
		md.vertices[2 * i + 0] = Vec3f(x, y,  halfh);
		md.vertices[2 * i + 1] = Vec3f(x, y, -halfh);
	}
}

void Robot1::Build(const Posed& pose, PHSceneIf* scene, PHSdkIf* sdk){
	CDBoxDesc bd;
	bd.boxsize = Vec3f(1.0, 1.0, 1.0);
	boxBody = sdk->CreateShape(bd)->Cast();
	
	CDConvexMeshDesc md;
	InitCylinder(md, 0.1f, 0.5f, 12);
	meshWheel = sdk->CreateShape(md)->Cast();

	PHSolidDesc sd;
	soBody = scene->CreateSolid(sd);
	soBody->AddShape(boxBody);
	soBody->SetPose(pose);

	sd.mass = 0.1;
	sd.inertia = 0.1 * Matrix3d::Unit();
	for(int i = 0; i < 4; i++){
		soWheel[i] = scene->CreateSolid(sd);
		soWheel[i]->AddShape(meshWheel);
	}

	PHHingeJointDesc jd;
	jd.poseSocket.Pos() = Vec3d( 1.5, -1.0,  1.5);
	jntWheel[0] = scene->CreateJoint(soBody, soWheel[0], jd)->Cast();
	jd.poseSocket.Pos() = Vec3d(-1.5, -1.0,  1.5);
	jntWheel[1] = scene->CreateJoint(soBody, soWheel[1], jd)->Cast();
	jd.poseSocket.Pos() = Vec3d( 1.5, -1.0, -1.5);
	jntWheel[2] = scene->CreateJoint(soBody, soWheel[2], jd)->Cast();
	jd.poseSocket.Pos() = Vec3d(-1.5, -1.0, -1.5);
	jntWheel[3] = scene->CreateJoint(soBody, soWheel[3], jd)->Cast();

	for(int i = 0; i < 4; i++){
		scene->SetContactMode(soBody, soWheel[i], PHSceneDesc::MODE_NONE);
		for(int j = i + 1; j < 4; j++){
			scene->SetContactMode(soWheel[i], soWheel[j], PHSceneDesc::MODE_NONE);
		}
	}
}

const double speed = 20.0;
void Robot1::Stop(){
	jntWheel[0]->SetDesiredVelocity(0);
	jntWheel[1]->SetDesiredVelocity(0);
	jntWheel[2]->SetDesiredVelocity(0);
	jntWheel[3]->SetDesiredVelocity(0);
}

void Robot1::Forward(){
	jntWheel[0]->SetDesiredVelocity(speed);
	jntWheel[1]->SetDesiredVelocity(speed);
	jntWheel[2]->SetDesiredVelocity(speed);
	jntWheel[3]->SetDesiredVelocity(speed);
}

void Robot1::Backward(){
	jntWheel[0]->SetDesiredVelocity(-speed);
	jntWheel[1]->SetDesiredVelocity(-speed);
	jntWheel[2]->SetDesiredVelocity(-speed);
	jntWheel[3]->SetDesiredVelocity(-speed);
}

void Robot1::TurnLeft(){
	jntWheel[0]->SetDesiredVelocity(speed);
	jntWheel[1]->SetDesiredVelocity(speed);
	jntWheel[2]->SetDesiredVelocity(-speed);
	jntWheel[3]->SetDesiredVelocity(-speed);
}

void Robot1::TurnRight(){
	jntWheel[0]->SetDesiredVelocity(-speed);
	jntWheel[1]->SetDesiredVelocity(-speed);
	jntWheel[2]->SetDesiredVelocity(speed);
	jntWheel[3]->SetDesiredVelocity(speed);
}

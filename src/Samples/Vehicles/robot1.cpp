#include "robot1.h"

void Robot1::InitCylinder(CDConvexMeshDesc& md, float height, float radius, int ndiv){
	md.vertices.resize(2 * ndiv);
	float theta, x, y;
	float halfh = 0.5f * height;
	for(int i = 0; i < ndiv; i++){
		theta = 2 * M_PI * float(i)/float(ndiv);
		x = radius * cos(theta);
		y = radius * sin(theta);
		md.vertices[2 * i + 0] = Vec3f(x, y,  halfh);
		md.vertices[2 * i + 1] = Vec3f(x, y, -halfh);
	}
}

void Robot1::Build(PHSceneIf* scene, PHSdkIf* sdk){
	CDBoxDesc bd;
	bd.boxsize = Vec3f(1.0, 1.0, 1.0);
	boxBody = DCAST(CDBoxIf, sdk->CreateShape(bd));
	
	CDConvexMeshDesc md;
	InitCylinder(md, 0.1, 0.5, 12);
	meshWheel = DCAST(CDConvexMeshIf, sdk->CreateShape(md));

	PHSolidDesc sd;
	soBody = scene->CreateSolid(sd);
	soBody->AddShape(boxBody);
	soBody->SetFramePosition(Vec3d(0.0, 3.0, 0.0));

	sd.mass = 0.1;
	sd.inertia = 0.1 * Matrix3d::Unit();
	for(int i = 0; i < 4; i++){
		soWheel[i] = scene->CreateSolid(sd);
		soWheel[i]->AddShape(meshWheel);
	}

	PHHingeJointDesc jd;
	jd.pose[0].Pos() = Vec3d( 1.5, -1.0,  1.5);
	jntWheel[0] = DCAST(PHHingeJointIf, scene->CreateJoint(soBody, soWheel[0], jd));
	jd.pose[0].Pos() = Vec3d(-1.5, -1.0,  1.5);
	jntWheel[1] = DCAST(PHHingeJointIf, scene->CreateJoint(soBody, soWheel[1], jd));
	jd.pose[0].Pos() = Vec3d( 1.5, -1.0, -1.5);
	jntWheel[2] = DCAST(PHHingeJointIf, scene->CreateJoint(soBody, soWheel[2], jd));
	jd.pose[0].Pos() = Vec3d(-1.5, -1.0, -1.5);
	jntWheel[3] = DCAST(PHHingeJointIf, scene->CreateJoint(soBody, soWheel[3], jd));

	for(int i = 0; i < 4; i++){
		scene->EnableContact(soBody, soWheel[i], false);
		for(int j = i + 1; j < 4; j++){
			scene->EnableContact(soWheel[i], soWheel[j], false);
		}
	}
}

void Robot1::Forward(){
	jntWheel[0]->SetDesiredVelocity(3.0);
	jntWheel[1]->SetDesiredVelocity(3.0);
	jntWheel[2]->SetDesiredVelocity(3.0);
	jntWheel[3]->SetDesiredVelocity(3.0);
}

void Robot1::Backward(){
	jntWheel[0]->SetDesiredVelocity(-3.0);
	jntWheel[1]->SetDesiredVelocity(-3.0);
	jntWheel[2]->SetDesiredVelocity(-3.0);
	jntWheel[3]->SetDesiredVelocity(-3.0);
}

void Robot1::TurnLeft(){
	jntWheel[0]->SetDesiredVelocity(3.0);
	jntWheel[1]->SetDesiredVelocity(3.0);
	jntWheel[2]->SetDesiredVelocity(-3.0);
	jntWheel[3]->SetDesiredVelocity(-3.0);
}

void Robot1::TurnRight(){
	jntWheel[0]->SetDesiredVelocity(-3.0);
	jntWheel[1]->SetDesiredVelocity(-3.0);
	jntWheel[2]->SetDesiredVelocity(3.0);
	jntWheel[3]->SetDesiredVelocity(3.0);
}

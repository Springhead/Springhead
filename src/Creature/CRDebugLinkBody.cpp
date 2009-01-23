/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRDebugLinkBody.h"

#ifndef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

// コンストラクタ
CRDebugLinkBodyDesc::CRDebugLinkBodyDesc(bool enableRange, bool enableFMax){
	soNSolids = 0;
	joNBallJoints = 0;
	joNHingeJoints = 0;
	joNJoints = 0;

	radius  = 0.5;
	length  = 1.2;

	mass = 2.0;

	spring = 5000.0;
	damper = 2 * sqrt(mass * spring);

	fMax = 50;
	shapeMode = MODE_CAPSULE;
	jointMode = MODE_BALL;
}

PHBallJointDesc CRDebugLinkBody::InitBallJointDesc(){
	PHBallJointDesc desc;
	{
		desc.spring = spring;
		desc.damper = damper;
		desc.poseSocket.Pos() = Vec3d(0, 0, length/2);
		desc.posePlug.Pos() = Vec3d(0, 0, -length/2);
		desc.fMax = fMax;
	}
	return desc;
}

PHHingeJointDesc CRDebugLinkBody::InitHingeJointDesc(){
	PHHingeJointDesc desc;
	{
		desc.spring = spring;
		desc.damper = damper;
		desc.poseSocket.Pos() = Vec3d(0, 0, length/2);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(90), 'y');
		desc.posePlug.Pos() = Vec3d(0, 0, -length/2);
		desc.posePlug.Ori() = Quaterniond::Rot(Rad(90), 'y');
		desc.fMax = fMax;
	}
	return desc;
}

void CRDebugLinkBody::SolidFactory(CDShapeMode m){
	PHSolidDesc sDesc;
	{
		sDesc.mass = mass;
	}
	if(m == MODE_SPHERE){
		CDSphereDesc cDesc;
		{
			cDesc.radius = radius;
			cDesc.material.mu  = 0.0;
			cDesc.material.mu0 = 0.0;
			cDesc.material.e   = 0.0;
		}
		for(unsigned int i = 0; i < soNSolids; i++){
			solids.push_back(phScene->CreateSolid(sDesc));
			solids.back()->AddShape(phSdk->CreateShape(cDesc));
		}
	}
	else if(m == MODE_CAPSULE){
		CDCapsuleDesc cDesc;
		{
			cDesc.radius = radius;
			cDesc.length = length;
			cDesc.material.mu  = 0.0;
			cDesc.material.mu0 = 0.0;
			cDesc.material.e   = 0.0;
		}
		for(unsigned int i = 0; i < soNSolids; i++){
			solids.push_back(phScene->CreateSolid(sDesc));
			solids.back()->AddShape(phSdk->CreateShape(cDesc));
		}
	}
	else if(m == MODE_BOX){
		CDBoxDesc cDesc;
		{
			cDesc.boxsize = Vec3f(radius, radius, length);
			cDesc.material.mu  = 0.0;
			cDesc.material.mu0 = 0.0;
			cDesc.material.e   = 0.0;
		}
		for(unsigned int i = 0; i < soNSolids; i++){
			solids.push_back(phScene->CreateSolid(sDesc));
			solids.back()->AddShape(phSdk->CreateShape(cDesc));
		}
	}
	else{
		DSTR << "Undefined CollisionShape" << std::endl;
	}
}
void CRDebugLinkBody::JointFactory(PHJointMode m){
	joNJoints = soNSolids-1;
	if(m == MODE_BALL){
		PHBallJointDesc bDesc = InitBallJointDesc();
		for(unsigned int i = 0; i < joNJoints; i++){
			joints.push_back(phScene->CreateJoint(solids[i], solids[i+1], bDesc));
		}
	}
	else if(m == MODE_HINGE){
		PHHingeJointDesc hDesc = InitHingeJointDesc();
		for(unsigned int i = 0; i < joNJoints; i++){
			joints.push_back(phScene->CreateJoint(solids[i], solids[i+1], hDesc));
		}
	}
	else if(m == MODE_MIXED){
		PHBallJointDesc  bDesc = InitBallJointDesc();
		PHHingeJointDesc hDesc = InitHingeJointDesc();
		for(unsigned int i = 0; i < joNJoints; i++){
			if(i % 2 == 0)	joints.push_back(phScene->CreateJoint(solids[i], solids[i+1], bDesc));
			else			joints.push_back(phScene->CreateJoint(solids[i], solids[i+1], hDesc));
		}
	}
}
void CRDebugLinkBody::CreateBody(){
	if(soNSolids <= 0) return;
	SolidFactory(shapeMode);
	JointFactory(jointMode);
}

void CRDebugLinkBody::InitBody(){}

void CRDebugLinkBody::InitContact(){
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

void CRDebugLinkBody::InitControlMode(PHJointDesc::PHControlMode m){}

void CRDebugLinkBody::Init(){}

}
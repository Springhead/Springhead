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

	breadth = 0.7;
	length  = 2.0;
	thickness = 0.7;

	mass = 2.0;

	spring = 10.0;
	damper = 5.0;

	fMax = 50;
}

void CRDebugLinkBody::CreateBody(){
	PHSolidDesc sDesc;
	{
		sDesc.mass = mass;
	}
	CDBoxDesc bDesc;
	{
		bDesc.boxsize = Vec3f(breadth, length, thickness);
	}
	solids.push_back(phScene->CreateSolid(sDesc));
	solids.back()->AddShape(phSdk->CreateShape(bDesc));
}

void CRDebugLinkBody::InitBody(){}

void CRDebugLinkBody::InitContact(){}

void CRDebugLinkBody::InitControlMode(PHJointDesc::PHControlMode m){}

void CRDebugLinkBody::Init(){}

}
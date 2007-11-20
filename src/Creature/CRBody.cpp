/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRBody, SceneObject);

PHJointIf* CRBody::CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc){
	PHJointIf* joint;
	if (jointOrder == PLUG_PARENT) {
		joint = phScene->CreateJoint(soChild, soParent, desc);
	} else { // SOCKET_PARENT
		Posed pp=desc.posePlug, ps=desc.poseSocket;
		desc.posePlug=ps; desc.poseSocket=pp;
		joint = phScene->CreateJoint(soParent, soChild, desc);
	}
	return joint;
}

PHJointIf* CRBody::CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHBallJointDesc desc){
	PHJointIf* joint;
	if (jointOrder == PLUG_PARENT) {
		joint = phScene->CreateJoint(soChild, soParent, desc);
	} else { // SOCKET_PARENT
		Posed pp=desc.posePlug, ps=desc.poseSocket;
		desc.posePlug=ps; desc.poseSocket=pp;
		joint = phScene->CreateJoint(soParent, soChild, desc);
	}
	return joint;
}

void CRBody::Init(){
}

int CRBody::NSolids(){
	return solids.size();
}

PHSolidIf* CRBody::GetSolid(int i){
	return solids[i];
}

int CRBody::NJoints(){
	return joints.size();
}

PHJointIf* CRBody::GetJoint(int i){
	return joints[i];
}

int CRBody::AddIKControlPoint(CRBodyIf::CRIKCPType type, PHSolidIf* solid, Vec3d pos){
	for(int i=0; i<cpNums.size(); ++i){
		if (cpNums[i].cpType==type && cpNums[i].cpSolid==solid) {
			cpNums[i].cpPos = pos;
			return i;
		}
	}
	cpNums.resize(cpNums.size()+1);
	cpNums.back().cpType  = type;
	cpNums.back().cpSolid = solid;
	cpNums.back().cpPos   = pos;

	jacobian.resize(cpNums.size(), moNums.size());
	movability.resize(cpNums.size(), moNums.size());
	for(int i=0; i<moNums.size(); ++i){
		movability[cpNums.size()-1][i] = false;
	}

	return(cpNums.size()-1);
}

int CRBody::AddIKMovableJoint(int cpnum, PHBallJointIf* ballJoint){
	for(int i=0; i<moNums.size(); ++i){
		if (moNums[i].movableBallJoint == ballJoint) {
			movability[cpnum][i] = true;
			return i;
		}
	}
	moNums.resize(moNums.size()+1);
	moNums.back().movableBallJoint = ballJoint;

	jacobian.resize(cpNums.size(), moNums.size());
	movability.resize(cpNums.size(), moNums.size());
	for(int i=0; i<cpNums.size(); ++i){
		movability[i][moNums.size()-1] = false;
	}
	movability[cpnum][moNums.size()-1] = true;

	return(moNums.size()-1);
}

int CRBody::AddIKMovableJoint(int cpnum, PHHingeJointIf* jo1, PHHingeJointIf* jo2, PHHingeJointIf* jo3){
	for(int i=0; i<moNums.size(); ++i){
		if (
			   (moNums[i].movableHinge1 == jo1 && moNums[i].movableHinge2 == jo2 && moNums[i].movableHinge3 == jo3)
			|| (moNums[i].movableHinge1 == jo1 && moNums[i].movableHinge2 == jo3 && moNums[i].movableHinge3 == jo2)
			|| (moNums[i].movableHinge1 == jo2 && moNums[i].movableHinge2 == jo1 && moNums[i].movableHinge3 == jo3)
			|| (moNums[i].movableHinge1 == jo2 && moNums[i].movableHinge2 == jo3 && moNums[i].movableHinge3 == jo1)
			|| (moNums[i].movableHinge1 == jo3 && moNums[i].movableHinge2 == jo1 && moNums[i].movableHinge3 == jo2)
			|| (moNums[i].movableHinge1 == jo3 && moNums[i].movableHinge2 == jo2 && moNums[i].movableHinge3 == jo1)
			)
		{
			movability[cpnum][i] = true;
			return i;
		}
	}
	moNums.resize(moNums.size()+1);
	moNums.back().movableHinge1 = jo1;
	moNums.back().movableHinge2 = jo2;
	moNums.back().movableHinge3 = jo3;

	jacobian.resize(cpNums.size(), moNums.size());
	movability.resize(cpNums.size(), moNums.size());
	for(int i=0; i<cpNums.size(); ++i){
		movability[i][moNums.size()-1] = false;
	}
	movability[cpnum][moNums.size()-1] = true;

	return(moNums.size()-1);
}

int CRBody::AddIKMovableSolid(int cpnum, PHSolidIf* solid){
	for(int i=0; i<moNums.size(); ++i){
		if (moNums[i].movableSolid == solid) {
			movability[cpnum][i] = true;
			return i;
		}
	}
	moNums.resize(moNums.size()+1);
	moNums.back().movableSolid = solid;

	jacobian.resize(cpNums.size(), moNums.size());
	movability.resize(cpNums.size(), moNums.size());
	for(int i=0; i<cpNums.size(); ++i){
		movability[i][moNums.size()-1] = false;
	}
	movability[cpnum][moNums.size()-1] = true;

	return(moNums.size()-1);
}
}

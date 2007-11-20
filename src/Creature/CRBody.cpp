/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBody.h"

#include "CRBodyIK.h"

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

CRIKControlIf* CRBody::AddIKControl(const IfInfo* ii, const CRIKControlDesc& desc){
	if (ii == CRIKControlPosIf::GetIfInfoStatic()) {
		controlsPosOri.push_back((DBG_NEW CRIKControlPos((const CRIKControlPosDesc&)desc))->Cast());
		ResizeMatrix(0,1);
		return controlsPosOri.back();

	} else if (ii == CRIKControlOriIf::GetIfInfoStatic()) {
		controlsPosOri.push_back((DBG_NEW CRIKControlOri((const CRIKControlOriDesc&)desc))->Cast());
		ResizeMatrix(0,1);
		return controlsPosOri.back();

	} else if (ii == CRIKControlForceIf::GetIfInfoStatic()) {
		return NULL;

	} else {
		assert(0 && "‘z’è‚³‚ê‚Ä‚È‚¢Œ^");
		return NULL;
	}
}

CRIKMovableIf* CRBody::AddIKMovable(const IfInfo* ii, const CRIKMovableDesc& desc){
	if (ii == CRIKMovableSolidPosIf::GetIfInfoStatic()) {
		movablesPosOri.push_back((DBG_NEW CRIKMovableSolidPos((const CRIKMovableSolidPosDesc&)desc))->Cast());
		ResizeMatrix(1,0);
		return movablesPosOri.back();
	
	} else if (ii == CRIKMovableSolidOriIf::GetIfInfoStatic()) {
		movablesPosOri.push_back((DBG_NEW CRIKMovableSolidOri((const CRIKMovableSolidOriDesc&)desc))->Cast());
		ResizeMatrix(1,0);
		return movablesPosOri.back();

	} else if (ii == CRIKMovableBallJointOriIf::GetIfInfoStatic()) {
		movablesPosOri.push_back((DBG_NEW CRIKMovableBallJointOri((const CRIKMovableBallJointOriDesc&)desc))->Cast());
		ResizeMatrix(1,0);
		return movablesPosOri.back();

	} else if (ii == CRIKMovableBallJointTorqueIf::GetIfInfoStatic()) {
		movablesPosOri.push_back((DBG_NEW CRIKMovableBallJointTorque((const CRIKMovableBallJointTorqueDesc&)desc))->Cast());
		ResizeMatrix(1,0);
		return movablesPosOri.back();

	} else if (ii == CRIKMovable3HingeJointOriIf::GetIfInfoStatic()) {
		return NULL;

	} else if (ii == CRIKMovable3HingeJointTorqueIf::GetIfInfoStatic()) {
		return NULL;

	} else {
		assert(0 && "‘z’è‚³‚ê‚Ä‚È‚¢Œ^");
		return NULL;
	}
}

void CRBody::ResizeMatrix(int hInc, int wInc){
	jacobianPosOri.resize(movablesPosOri.size(), controlsPosOri.size());
	movabilityPosOri.resize(movablesPosOri.size(), controlsPosOri.size());
	sqjacobianPosOri.resize(movablesPosOri.size(), movablesPosOri.size());

	for(int i=0; i<hInc; ++i){
		for(int j=0; j<movablesPosOri.size(); ++j){
			movabilityPosOri[controlsPosOri.size()-i-1][j] = false;
		}
	}

	for(int i=0; i<wInc; ++i){
		for(int j=0; j<controlsPosOri.size(); ++j){
			movabilityPosOri[j][movablesPosOri.size()-i-1] = false;
		}
	}
}

}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

#if 0
CRIKControlIf* CRBody::CreateIKControl(const IfInfo* ii, const CRIKControlDesc& desc){
	if (ii == CRIKControlPosIf::GetIfInfoStatic()) {
		postureControls.push_back((DBG_NEW CRIKControlPos((const CRIKControlPosDesc&)desc))->Cast());
		DCAST(CRIKControl,postureControls.back())->SetNumber(postureControls.size()-1);
		return postureControls.back();

	} else if (ii == CRIKControlOriIf::GetIfInfoStatic()) {
		postureControls.push_back((DBG_NEW CRIKControlOri((const CRIKControlOriDesc&)desc))->Cast());
		DCAST(CRIKControl,postureControls.back())->SetNumber(postureControls.size()-1);
		return postureControls.back();

	} else {
		assert(0 && "想定されてない型");
		return NULL;
	}
}

CRIKMovableIf* CRBody::CreateIKMovable(const IfInfo* ii, const CRIKMovableDesc& desc){
	if (ii == CRIKMovableSolidPosIf::GetIfInfoStatic()) {
		postureMovables.push_back((DBG_NEW CRIKMovableSolidPos((const CRIKMovableSolidPosDesc&)desc))->Cast());
		DCAST(CRIKMovable,postureMovables.back())->SetNumber(postureMovables.size()-1);
		return postureMovables.back();

	} else if (ii == CRIKMovableSolidOriIf::GetIfInfoStatic()) {
		postureMovables.push_back((DBG_NEW CRIKMovableSolidOri((const CRIKMovableSolidOriDesc&)desc))->Cast());
		DCAST(CRIKMovable,postureMovables.back())->SetNumber(postureMovables.size()-1);
		return postureMovables.back();

	} else if (ii == CRIKMovableBallJointOriIf::GetIfInfoStatic()) {
		postureMovables.push_back((DBG_NEW CRIKMovableBallJointOri((const CRIKMovableBallJointOriDesc&)desc))->Cast());
		DCAST(CRIKMovable,postureMovables.back())->SetNumber(postureMovables.size()-1);
		return postureMovables.back();

	} else if (ii == CRIKMovable3HingeJointOriIf::GetIfInfoStatic()) {
		postureMovables.push_back((DBG_NEW CRIKMovable3HingeJointOri((const CRIKMovable3HingeJointOriDesc&)desc))->Cast());
		DCAST(CRIKMovable,postureMovables.back())->SetNumber(postureMovables.size()-1);
		return postureMovables.back();

	} else if (ii == CRIKMovableHingeJointOriIf::GetIfInfoStatic()) {
		postureMovables.push_back((DBG_NEW CRIKMovableHingeJointOri((const CRIKMovableHingeJointOriDesc&)desc))->Cast());
		DCAST(CRIKMovable,postureMovables.back())->SetNumber(postureMovables.size()-1);
		return postureMovables.back();

	} else {
		assert(0 && "想定されてない型");
		return NULL;
	}
}

void CRBody::CalcIK(){
	for(unsigned int i=0; i<postureMovables.size(); ++i){
		postureMovables[i]->PrepareSolve();
	}
	int niterIK = 25;
	for(int n=0; n<niterIK; n++){
		for(unsigned int i=0; i<postureMovables.size(); ++i){
			postureMovables[i]->ProceedSolve();
		}
	}
}
#endif

Vec3d CRBody::GetCenterOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;
	/// 重心を求めるときに使うi番目までのブロックの中心座標
	Vec3d  centerPosOfBlocks = Vec3d(0.0, 0.0, 0.0);

	for(int i = 0; i<NSolids(); i++){
		if(solids[i]){
			centerPosOfBlocks = centerPosOfBlocks + solids[i]->GetCenterPosition() * solids[i]->GetMass();
			totalWeight = totalWeight + solids[i]->GetMass(); 
		}
	}

	return centerPosOfBlocks / totalWeight;
}

double CRBody::GetSumOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;

	for(int i = 0; i<NSolids(); i++){
		if(solids[i])
			totalWeight = totalWeight + solids[i]->GetMass(); 
	}

	return totalWeight;
}

Matrix3d CRBody::CalcBoxInertia(Vec3d boxsize, double mass){
	double i_xx = 1.0 / 12.0 * (boxsize[1] * boxsize[1] + boxsize[2] * boxsize[2]) * mass;
	double i_yy = 1.0 / 12.0 * (boxsize[2] * boxsize[2] + boxsize[0] * boxsize[0]) * mass;
	double i_zz = 1.0 / 12.0 * (boxsize[0] * boxsize[0] + boxsize[1] * boxsize[1]) * mass;

	return Matrix3d(i_xx, 0.0,  0.0, 
					0.0,  i_yy, 0.0, 
					0.0,  0.0,  i_zz);
}

}
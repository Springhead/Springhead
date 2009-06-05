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
	return ((size_t)i < solids.size()) ? solids[i] : NULL;
}

int CRBody::NJoints(){
	return joints.size();
}

PHJointIf* CRBody::GetJoint(int i){
	return ((size_t)i < joints.size()) ? joints[i] : NULL;
}

int CRBody::NBallJoints(){
	int counterNBallJoint = 0;
	for(unsigned int i = 0; i < joints.size(); i++){
		if(DCAST(PHBallJointIf, joints[i]))
			counterNBallJoint ++;
	}
	return counterNBallJoint;
}	

int CRBody::NHingeJoints(){
	int counterNHingeJoint = 0;
	for(unsigned int i = 0; i< joints.size(); i++){
		if(DCAST(PHHingeJointIf, joints[i]))
			 counterNHingeJoint ++;
	}
	return counterNHingeJoint;
}

int CRBody::NIKNodes(){
	return ikNodes.size();
}

PHIKNodeIf* CRBody::GetIKNode(int i){
	return ((size_t)i < ikNodes.size()) ? ikNodes[i] : NULL;
}

int CRBody::NControlPoints(){
	return ikControlPoints.size();
}

PHIKControlPointIf* CRBody::GetControlPoint(int i){
	return ((size_t)i < ikControlPoints.size()) ? ikControlPoints[i] : NULL;
}

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
/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBodyGen.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

PHJointIf* CRBodyGen::CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc){
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

PHJointIf* CRBodyGen::CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHBallJointDesc desc){
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

void CRBodyGen::Init(){
	for(size_t i = 0; i < joints.size(); i++){
		if(solids.front() == joints[i]->GetSocketSolid()){
			SetInitPosition(solids.front(), joints[i]);
		}
	}
}

void CRBodyGen::SetInitPosition(PHSolidIf* parentSolid, PHJointIf* childJoint){
	PHSolidIf*	nextParent		= childJoint->GetPlugSolid();
	Posed sp, pp; //< socket, plugのpose
	childJoint->GetSocketPose(sp);
	childJoint->GetPlugPose(pp);
	Posed parentRot;
	parentRot.W() = parentSolid->GetPose().W(); 
	parentRot.X() = parentSolid->GetPose().X();
	parentRot.Y() = parentSolid->GetPose().Y();
	parentRot.Z() = parentSolid->GetPose().Z();
	Vec3d spX = sp.Pos();
	Vec3d ppX = pp.Pos();
	spX = parentRot * spX;
	ppX = parentRot * ppX;
	sp.Px() = spX.X();
	sp.Py() = spX.Y();
	sp.Pz() = spX.Z();
	pp.Px() = ppX.X();
	pp.Py() = ppX.Y();
	pp.Pz() = ppX.Z();
	// 制御目標がない場合
	Posed nextParentPos	= sp * pp.Inv() * parentSolid->GetPose();
	nextParent->SetPose(nextParentPos);
	for(size_t i = 0; i < joints.size(); i++){
		if(nextParent == joints[i]->GetSocketSolid()){
			SetInitPosition(nextParent, joints[i]);
		}
	}
}

int CRBodyGen::NSolids(){
	return solids.size();
}

PHSolidIf* CRBodyGen::GetSolid(int i){
	return ((size_t)i < solids.size()) ? solids[i] : NULL;
}

int CRBodyGen::NJoints(){
	return joints.size();
}

PHJointIf* CRBodyGen::GetJoint(int i){
	return ((size_t)i < joints.size()) ? joints[i] : NULL;
}

int CRBodyGen::NBallJoints(){
	int counterNBallJoint = 0;
	for(unsigned int i = 0; i < joints.size(); i++){
		if(DCAST(PHBallJointIf, joints[i]))
			counterNBallJoint ++;
	}
	return counterNBallJoint;
}	

int CRBodyGen::NHingeJoints(){
	int counterNHingeJoint = 0;
	for(unsigned int i = 0; i< joints.size(); i++){
		if(DCAST(PHHingeJointIf, joints[i]))
			 counterNHingeJoint ++;
	}
	return counterNHingeJoint;
}

int CRBodyGen::NIKNodes(){
	return ikNodes.size();
}

PHIKNodeIf* CRBodyGen::GetIKNode(int i){
	return ((size_t)i < ikNodes.size()) ? ikNodes[i] : NULL;
}

int CRBodyGen::NControlPoints(){
	return ikControlPoints.size();
}

PHIKControlPointIf* CRBodyGen::GetControlPoint(int i){
	return ((size_t)i < ikControlPoints.size()) ? ikControlPoints[i] : NULL;
}

Vec3d CRBodyGen::GetCenterOfMass(){
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

double CRBodyGen::GetSumOfMass(){
	/// 重心を求める時に使うi番目までの重心の小計
	double totalWeight = 0;

	for(int i = 0; i<NSolids(); i++){
		if(solids[i])
			totalWeight = totalWeight + solids[i]->GetMass(); 
	}

	return totalWeight;
}

Matrix3d CRBodyGen::CalcBoxInertia(Vec3d boxsize, double mass){
	double i_xx = 1.0 / 12.0 * (boxsize[1] * boxsize[1] + boxsize[2] * boxsize[2]) * mass;
	double i_yy = 1.0 / 12.0 * (boxsize[2] * boxsize[2] + boxsize[0] * boxsize[0]) * mass;
	double i_zz = 1.0 / 12.0 * (boxsize[0] * boxsize[0] + boxsize[1] * boxsize[1]) * mass;

	return Matrix3d(i_xx, 0.0,  0.0, 
					0.0,  i_yy, 0.0, 
					0.0,  0.0,  i_zz);
}

double CRBodyGen::GetTargetMechanicalEnergy(PHSolidIf* rootSolid){
	return GetTargetKineticEnergy() + GetTargetPotentialEnergy(rootSolid);
}

double CRBodyGen::GetTargetKineticEnergy(){
	double ans = DBL_MAX;
	if(solids.size() > 0){
		ans = 0;
		for(size_t i = 0; i < solids.size(); i++){
			double m = solids[i]->GetMass();
			Vec3d  v = solids[i]->GetVelocity();
			ans += 0.5 * m * pow(v[0], 2);
			ans += 0.5 * m * pow(v[1], 2);
			ans += 0.5 * m * pow(v[2], 2);
			Matrix3d I = solids[i]->GetInertia();
			Vec3d	 o = solids[i]->GetAngularVelocity();
			ans += 0.5 * I[0][0] * pow(o[0], 2);
			ans += 0.5 * I[1][1] * pow(o[1], 2);
			ans += 0.5 * I[2][2] * pow(o[2], 2);
		}
	}
	return ans;
}

double CRBodyGen::GetTargetPotentialEnergy(PHSolidIf* rootSolid){
	double ans = DBL_MAX;
	
	return ans;
}

double CRBodyGen::GetMechanicalEnergy(PHSolidIf* rootSolid){
	return GetKineticEnergy() + GetPotentialEnergy(rootSolid);
}

double CRBodyGen::GetKineticEnergy(){
	double ans = DBL_MAX;
	if(solids.size() > 0){
		ans = 0;
		for(size_t i = 0; i < solids.size(); i++){
			double m = solids[i]->GetMass();
			Vec3d  v = solids[i]->GetVelocity();
			ans += 0.5 * m * pow(v[0], 2);
			ans += 0.5 * m * pow(v[1], 2);
			ans += 0.5 * m * pow(v[2], 2);
			Matrix3d I = solids[i]->GetInertia();
			Vec3d	 o = solids[i]->GetAngularVelocity();
			ans += 0.5 * I[0][0] * pow(o[0], 2);
			ans += 0.5 * I[1][1] * pow(o[1], 2);
			ans += 0.5 * I[2][2] * pow(o[2], 2);
		}
	}
	return ans;
}

double CRBodyGen::GetPotentialEnergy(PHSolidIf* rootSolid){
	double ans = DBL_MAX;
	
	if(rootSolid) ans = 0;
	for(size_t i = 0; i < joints.size(); i++){
		if(rootSolid == joints[i]->GetSocketSolid()){
			ans += CalcPotential(rootSolid->GetPose(), rootSolid, joints[i]);
		}
	}
	return ans;
}

double CRBodyGen::CalcPotential(Posed parentPos, PHSolidIf* parentSolid, PHJointIf* childJoint){
	double ans = DBL_MAX;

	
	return ans;
}

}//< end of namespace Spr
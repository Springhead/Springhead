/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRBodyGenerator/CRBodyGen.h>
#include <Creature/SprCRBody.h>
#include <Physics/SprPHScene.h>
#include <algorithm>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

CRBodyGen::CRBodyGen(const CRBodyGenDesc& desc):CRBodyGenDesc(desc){
}

CRBoneIf* CRBodyGen::CreateJoint(CRBoneIf* soChild, CRBoneIf* soParent, const PHHingeJointDesc& desc){
	PHSceneIf* phScene = crCreature->GetPHScene();

	PHHingeJointDesc d = desc;
	d.posePlug = desc.poseSocket;
	d.poseSocket = desc.posePlug;
	PHJointIf* phJoint = phScene->CreateJoint(soParent->GetPHSolid(), soChild->GetPHSolid(), d);

	PHIKHingeActuatorDesc descIKAct; descIKAct.bEnabled = true;
	PHIKActuatorIf*	phIKAct = phScene->CreateIKActuator(descIKAct);
	phIKAct->AddChildObject(phJoint);

	CRBoneDesc descIKJoint;
	CRBoneIf* crJoint = crBody->CreateObject(CRBoneIf::GetIfInfoStatic(), &descIKJoint)->Cast();

	crJoint->AddChildObject(phJoint);
	crJoint->AddChildObject(phIKAct);

	crBody->AddChildObject(crJoint);

	return crJoint;
}

CRBoneIf* CRBodyGen::CreateJoint(CRBoneIf* soChild, CRBoneIf* soParent, const PHBallJointDesc& desc){
	PHSceneIf* phScene = crCreature->GetPHScene();

	PHBallJointDesc d = desc;
	d.posePlug = desc.poseSocket;
	d.poseSocket = desc.posePlug;
	PHJointIf* phJoint = phScene->CreateJoint(soParent->GetPHSolid(), soChild->GetPHSolid(), d);

	PHIKBallActuatorDesc descIKAct; descIKAct.bEnabled = true;
	PHIKActuatorIf*	phIKAct = phScene->CreateIKActuator(descIKAct);
	phIKAct->AddChildObject(phJoint);

	CRBoneDesc descIKJoint;
	CRBoneIf* crJoint = crBody->CreateObject(CRBoneIf::GetIfInfoStatic(), &descIKJoint)->Cast();

	crJoint->AddChildObject(phJoint);
	crJoint->AddChildObject(phIKAct);

	crBody->AddChildObject(crJoint);

	return crJoint;
}

CRBoneIf* CRBodyGen::CreateSolid(const PHSolidDesc& desc) {
	PHSceneIf* phScene = crCreature->GetPHScene();

	PHSolidIf* phSolid = phScene->CreateSolid(desc);

	PHIKEndEffectorDesc descIKEE;
	descIKEE.bEnabled = false;
	// descIKEE.targetLocalPosition = Vec3d(0,0,0.1);
	PHIKEndEffectorIf* phIKEE = phScene->CreateIKEndEffector(descIKEE);
	phIKEE->AddChildObject(phSolid);

	CRBoneDesc descIKSolid;
	CRBoneIf* crSolid = crBody->CreateObject(CRBoneIf::GetIfInfoStatic(), &descIKSolid)->Cast();

	crSolid->AddChildObject(phSolid);
	crSolid->AddChildObject(phIKEE);

	crBody->AddChildObject(crSolid);

	return crSolid;
}

CRBodyIf* CRBodyGen::Generate(CRCreatureIf* crCreature){
	this->crCreature = crCreature;
	
	CRBodyDesc descBody;
	crBody = crCreature->CreateBody(descBody);

	return NULL;
}

// これ要るのか？
void CRBodyGen::SetInitPosition(PHSolidIf* parentSolid, PHJointIf* childJoint){
	/*
	PHSolidIf*	nextParent		= childJoint->GetPlugSolid();
	Posed sp, pp; //< socket, plugのpose
	childJoint->GetSocketPose(sp);
	childJoint->GetPlugPose(pp);
	Quaterniond target;
	if(PHHingeJointIf* hj = DCAST(PHHingeJointIf, childJoint)){
		target = Quaterniond::Rot(hj->GetTargetPosition(), 'z');
	}else if(PHBallJointIf* bj = DCAST(PHBallJointIf, childJoint)){
		target = bj->GetTargetPosition();
	}
	Posed targetRot;
	targetRot.Ori() = target;
	targetRot.Pos() = Vec3d();
	Posed nextParentPos = parentSolid->GetPose() * sp * targetRot * pp.Inv();
	nextParent->SetPose(nextParentPos);

	for(size_t i = 0; i < joints.size(); i++){
		if(nextParent == joints[i]->GetSocketSolid()){
			SetInitPosition(nextParent, joints[i]);
		}
	}
	*/
}


// これらの関数はCRBodyに移動予定（mitake）
#if 0
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

	if(rootSolid){
		ans = 0;
		ans += rootSolid->GetMass() * phScene->GetGravity().Y() * rootSolid->GetPose().PosY();
		for(size_t i = 0; i < joints.size(); i++){
			if(rootSolid == joints[i]->GetSocketSolid()){
				// Solidのposeを書き換えてはいけないため，引数が3つ
				ans += CalcTargetPotential(rootSolid->GetPose(), rootSolid, joints[i]);
			}
		}
	}

	return ans;
}

double CRBodyGen::CalcTargetPotential(Posed parentPos, PHSolidIf* parentSolid, PHJointIf* childJoint){
	double ans = DBL_MAX;
	
	PHSolidIf*	nextParent = childJoint->GetPlugSolid();
	Posed sp, pp; //< socket, plugのpose
	childJoint->GetSocketPose(sp);
	childJoint->GetPlugPose(pp);
	PHHingeJointIf* hj = DCAST(PHHingeJointIf, childJoint);
	Quaterniond target = Quaterniond::Rot(hj->GetPosition(), 'z');
	Posed targetRot;
	targetRot.Ori() = target;
	targetRot.Pos() = Vec3d();
	Posed nextParentPos = parentSolid->GetPose() * sp * targetRot * pp.Inv();
	
	ans += parentSolid->GetMass() * phScene->GetGravity().Y() * nextParentPos.PosY();
	for(size_t i = 0; i < joints.size(); i++){
		if(nextParent == joints[i]->GetSocketSolid()){
			ans += CalcTargetPotential(nextParentPos, nextParent, joints[i]);
		}
	}
	
	return ans;
}

double CRBodyGen::GetMechanicalEnergy(){
	return GetKineticEnergy() + GetPotentialEnergy();
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

double CRBodyGen::GetPotentialEnergy(){
	double ans = 0;

	for(size_t i = 0; i < solids.size(); i++){
		ans += solids[i]->GetMass() * solids[i]->GetPose().PosY();
	}
	ans *= phScene->GetGravity().Y();

	return ans;
}
#endif

}//< end of namespace Spr
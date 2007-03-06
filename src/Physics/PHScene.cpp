/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <sstream>

namespace Spr{;

//----------------------------------------------------------------------------
//	PHScene
IF_OBJECT_IMP(PHScene, Scene);

PHScene::PHScene(const PHSceneDesc& desc):PHSceneDesc(desc){
	Init();
}
void PHScene::Init(){
	engines.scene = this;
	Scene::Clear();

	solids = DBG_NEW PHSolidContainer;
	engines.Add(solids);
	PHSolidInitializer* si = DBG_NEW PHSolidInitializer;
	si->container = solids;
	engines.Add(si);
	
	gravityEngine = DBG_NEW PHGravityEngine;
	engines.Add(gravityEngine);
	gravityEngine->accel = gravity;

	penaltyEngine = DBG_NEW PHPenaltyEngine;
	engines.Add(penaltyEngine);
	
	constraintEngine = DBG_NEW PHConstraintEngine;
	constraintEngine->numIter = numIteration;
	engines.Add(constraintEngine);

	count = 0;
}

PHSdkIf* PHScene::GetSdk(){
	NameManagerIf* nm = GetNameManager();
	PHSdkIf* sdk = DCAST(PHSdkIf, nm);
	return sdk;
}

PHSolidIf* PHScene::CreateSolid(const PHSolidDesc& desc){
	UTRef<PHSolid> s = DBG_NEW PHSolid(desc, Cast());
	AddChildObject(s->Cast());
	return s->Cast();
}
int PHScene::NSolids()const{
	return solids->solids.size();
}
PHSolidIf** PHScene::GetSolids(){
	return (PHSolidIf**)&*solids->solids.begin();
}

CDShapeIf* PHScene::CreateShape(const CDShapeDesc& desc){
	return GetSdk()->CreateShape(desc);
}

PHJointIf* PHScene::CreateJoint(const PHJointDesc& desc){
	return constraintEngine->CreateJoint(desc)->Cast();
}
PHJointIf* PHScene::CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const PHJointDesc& desc){
	return constraintEngine->CreateJoint(desc, lhs->Cast(), rhs->Cast())->Cast();	
}
int PHScene::NJoints()const{
	return constraintEngine->joints.size();
}
PHJointIf* PHScene::GetJoint(int i){
	return DCAST(PHJointIf, constraintEngine->joints[i]);
}

PHPathIf* PHScene::CreatePath(const PHPathDesc& desc){
	PHPath* rv = DBG_NEW PHPath(desc);
	return rv->Cast();
}

PHRootNodeIf* PHScene::CreateRootNode(const PHRootNodeDesc& desc){
	return constraintEngine->CreateRootNode(desc)->Cast();
}
PHRootNodeIf* PHScene::CreateRootNode(PHSolidIf* root, const PHRootNodeDesc& desc){
	return constraintEngine->CreateRootNode(desc, root->Cast())->Cast();
}
int PHScene::NRootNodes()const{
	return constraintEngine->trees.size();
}
PHRootNodeIf* PHScene::GetRootNode(int i){
	return constraintEngine->trees[i]->Cast();
}
PHTreeNodeIf* PHScene::CreateTreeNode(const PHTreeNodeDesc& desc){
	return constraintEngine->CreateTreeNode(desc)->Cast();
}
PHTreeNodeIf* PHScene::CreateTreeNode(PHTreeNodeIf* parent, PHSolidIf* child, const PHTreeNodeDesc& desc){
	return constraintEngine->CreateTreeNode(desc, parent->Cast(), child->Cast())->Cast();
}
PHGearIf* PHScene::CreateGear(const PHGearDesc& desc){
	return constraintEngine->CreateGear(desc)->Cast();
}
PHGearIf* PHScene::CreateGear(PHJoint1DIf* lhs, PHJoint1DIf* rhs, const PHGearDesc& desc){
	return constraintEngine->CreateGear(desc, lhs->Cast(), rhs->Cast())->Cast();
}
int PHScene::NGears()const{
	return constraintEngine->gears.size();
}
PHGearIf* PHScene::GetGear(int i){
	return constraintEngine->gears[i]->Cast();
}
void PHScene::Clear(){
	engines.Clear();
	Init();
}

void PHScene::SetTimeStep(double dt){
	timeStep = dt;
}

void PHScene::Step(){
	ClearForce();
	GenerateForce();
	Integrate();
}
void PHScene::ClearForce(){
	engines.ClearForce();
}
	
#if defined _MSC_VER	
#define FP_ERROR_MASK	(_EM_INEXACT|_EM_UNDERFLOW)
void PHScene::GenerateForce(){
	_clearfp();
	_controlfp(FP_ERROR_MASK, _MCW_EM);
	engines.GenerateForce();
	_controlfp(_MCW_EM, _MCW_EM);
}
void PHScene::Integrate(){
	_clearfp();
	_controlfp(FP_ERROR_MASK, _MCW_EM);
	engines.Integrate();
	_controlfp(_MCW_EM, _MCW_EM);
	count++;
}
#else
void PHScene::GenerateForce(){
	engines.GenerateForce();
}
void PHScene::Integrate(){
	engines.Integrate();
	count++;
}	
#endif
	
void PHScene::SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, PHSceneDesc::ContactMode mode){
	penaltyEngine->EnableContact(lhs, rhs, mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(lhs, rhs, mode == PHSceneDesc::MODE_LCP);
}

void PHScene::SetContactMode(PHSolidIf** group, size_t length, PHSceneDesc::ContactMode mode){
	penaltyEngine->EnableContact(group, length, mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(group, length, mode == PHSceneDesc::MODE_LCP);
}

void PHScene::SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode mode){
	penaltyEngine->EnableContact(solid, mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(solid, mode == PHSceneDesc::MODE_LCP);
}

void PHScene::SetContactMode(PHSceneDesc::ContactMode mode){
	penaltyEngine->EnableContact(mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(mode == PHSceneDesc::MODE_LCP);
}
int PHScene::GetNumIteration(){
	return constraintEngine->numIter;
}
void PHScene::SetNumIteration(int n){
	constraintEngine->numIter = n;
}

void PHScene::SetGravity(const Vec3d& accel){
	PHGravityEngine* ge;
	engines.Find(ge);
	assert(ge);
	ge->accel = accel;
}
Vec3f PHScene::GetGravity(){
	PHGravityEngine* ge;
	engines.Find(ge);
	assert(ge);
	return ge->accel;
}
ObjectIf* PHScene::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Scene::CreateObject(info, desc);
	if (!rv){
		if (info->Inherit(CDShapeIf::GetIfInfoStatic())){
			rv = CreateShape(*(const CDShapeDesc*)desc);
		}
	}
	return rv;
}
size_t PHScene::NChildObject() const{
	//return engines.size();
	return NSolids() + NJoints() + NRootNodes() + NGears();
}
ObjectIf* PHScene::GetChildObject(size_t pos){
	//return engines[pos]->Cast();
	if(pos < NSolids()) return GetSolids()[pos];
	pos -= NSolids();
	if(pos < NJoints()) return GetJoint(pos);
	pos -= NJoints();
	if(pos < NRootNodes()) return GetRootNode(pos);
	pos -= NRootNodes();
	if(pos < NGears()) return GetGear(pos);
	return NULL;
}
bool PHScene::AddChildObject(ObjectIf* o){
	bool ok = false;
	PHSolidIf* solid = DCAST(PHSolidIf, o);
	if(solid){
		if(	solids->AddChildObject(o) &&
			gravityEngine->AddChildObject(o) &&
			penaltyEngine->AddChildObject(o) &&
			constraintEngine->AddChildObject(o))
		{
            SetContactMode(solid, PHSceneDesc::MODE_LCP);	//デフォルトでLCP
			ok = true;
		}
	}
	PHJointIf* con = DCAST(PHJointIf, o);
	if(con && constraintEngine->AddChildObject(con))
		ok = true;
	PHTreeNodeIf* node = DCAST(PHTreeNodeIf, o);
	if(node && constraintEngine->AddChildObject(o))
		ok = true;
	PHGearIf* gear = DCAST(PHGearIf, o);
	if(gear && constraintEngine->AddChildObject(o))
		ok = true;
	// MergeSceneなどで他のSceneから移動してくる場合もあるので所有権を更新する
	if(ok)
		DCAST(SceneObject, o)->SetScene(Cast());
	return ok;
}
bool PHScene::DelChildObject(ObjectIf* o){
	PHSolidIf* solid = DCAST(PHSolidIf, o);
	if(solid){
		solids->DelChildObject(o);
		penaltyEngine->DelChildObject(o);
		constraintEngine->DelChildObject(o);
		gravityEngine->DelChildObject(o);
		return true;
	}
	PHJointIf* con = DCAST(PHJointIf, o);
	if(con)
		return constraintEngine->DelChildObject(o);

	PHTreeNodeIf* node = DCAST(PHTreeNodeIf, o);
	if(node)
		return constraintEngine->DelChildObject(o);

	PHGearIf* gear = DCAST(PHGearIf, o);
	if(gear)
		return constraintEngine->DelChildObject(o);
	
	return false;
}

PHConstraintEngine* PHScene::GetConstraintEngine(){
	return constraintEngine;
}

PHGravityEngine* PHScene::GetGravityEngine(){
	return gravityEngine;
}
}

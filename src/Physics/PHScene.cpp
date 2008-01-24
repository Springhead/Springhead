/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
void PHScene::UpdateDesc(){
	if (gravityEngine)
		gravity = gravityEngine->accel;
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
	return solids->solids.empty() ? NULL : (PHSolidIf**)&*solids->solids.begin();
}

CDShapeIf* PHScene::CreateShape(const IfInfo* ii, const CDShapeDesc& desc){
	return GetSdk()->CreateShape(ii, desc);
}

PHJointIf* PHScene::CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const PHJointDesc& desc){
	PHJoint* joint = constraintEngine->CreateJoint(ii, desc, lhs->Cast(), rhs->Cast());
	joint->SetScene(Cast());
	return joint->Cast();	
}
int PHScene::NJoints()const{
	return constraintEngine->joints.size();
}
PHJointIf* PHScene::GetJoint(int i){
	return DCAST(PHJointIf, constraintEngine->joints[i]);
}
int PHScene::NContacts()const{
	return constraintEngine->points.size();
}
PHContactPointIf* PHScene::GetContact(int i){
	return DCAST(PHContactPointIf, constraintEngine->points[i]);
}

PHRootNodeIf* PHScene::CreateRootNode(PHSolidIf* root, const PHRootNodeDesc& desc){
	PHRootNode* node = constraintEngine->CreateRootNode(desc, root->Cast());
	node->SetScene(Cast());
	return node->Cast();
}
int PHScene::NRootNodes()const{
	return constraintEngine->trees.size();
}
PHRootNodeIf* PHScene::GetRootNode(int i){
	return constraintEngine->trees[i]->Cast();
}

PHTreeNodeIf* PHScene::CreateTreeNode(PHTreeNodeIf* parent, PHSolidIf* child, const PHTreeNodeDesc& desc){
	PHTreeNode* node = constraintEngine->CreateTreeNode(desc, parent->Cast(), child->Cast());
	node->SetScene(Cast());
	return node->Cast();
}

PHGearIf* PHScene::CreateGear(PHJoint1DIf* lhs, PHJoint1DIf* rhs, const PHGearDesc& desc){
	PHGear* gear = constraintEngine->CreateGear(desc, lhs->Cast(), rhs->Cast());
	gear->SetScene(Cast());
	return gear->Cast();
}
int PHScene::NGears()const{
	return constraintEngine->gears.size();
}
PHGearIf* PHScene::GetGear(int i){
	return constraintEngine->gears[i]->Cast();
}
PHPathIf* PHScene::CreatePath(const PHPathDesc& desc){
	PHPath* path = constraintEngine->CreatePath(desc)->Cast();
	path->SetScene(Cast());
	return path->Cast();
}
int PHScene::NPaths()const{
	return constraintEngine->paths.size();
}
PHPathIf* PHScene::GetPath(int i){
	return constraintEngine->paths[i]->Cast();
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
			rv = CreateShape(info, *(const CDShapeDesc*)desc);
		}
	}
	return rv;
}
size_t PHScene::NChildObject() const{
	//return engines.size();
	return NSolids() + NJoints() + NRootNodes() + NGears() + NPaths();
}
ObjectIf* PHScene::GetChildObject(size_t pos){
	//return engines[pos]->Cast();
	if(pos < (size_t)NSolids()) return GetSolids()[pos];
	pos -= NSolids();
	if(pos < (size_t)NJoints()) return GetJoint(pos);
	pos -= NJoints();
	if(pos < (size_t)NRootNodes()) return GetRootNode(pos);
	pos -= NRootNodes();
	if(pos < (size_t)NGears()) return GetGear(pos);
	pos -= NGears();
	if(pos < (size_t)NPaths()) return GetPath(pos);
	return NULL;
}
bool PHScene::AddChildObject(ObjectIf* o){
	bool ok = false;
	PHSolid* solid = DCAST(PHSolid, o);
	if(solid){
		if(	solids->AddChildObject(o) &&
			gravityEngine->AddChildObject(o) &&
			penaltyEngine->AddChildObject(o) &&
			constraintEngine->AddChildObject(o))
		{
            SetContactMode(solid->Cast(), PHSceneDesc::MODE_LCP);	//デフォルトでLCP
			solid->scene = this;
			solid->engine = constraintEngine;
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
	PHPathIf* path = DCAST(PHPathIf, o);
	if(path && constraintEngine->AddChildObject(o))
		ok = true;
	// MergeSceneなどで他のSceneから移動してくる場合もあるので所有権を更新する
	if(ok){
		SceneObject* so = DCAST(SceneObject, o);
		so->SetScene(Cast());
		// 名前が付いていない場合にデフォルト名を与える
		if(strcmp(so->GetName(), "") == 0){
			char name[256];
			if(solid)
				sprintf(name, "solid%d", NSolids()-1);
			if(con)
				sprintf(name, "joint%d", NJoints()-1);
			if(gear)
				sprintf(name, "gear%d", NGears()-1);
			if(path)
				sprintf(name, "path%d", NPaths()-1);
			so->SetName(name);
		}
	}
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
	PHPathIf* path = DCAST(PHPathIf, o);
	if(path)
		return constraintEngine->DelChildObject(o);
	
	return false;
}

PHEngines* PHScene::GetEngines(){
	return &engines;
}

int PHScene::NEngines(){
	
	return engines.size();
}

PHEngineIf* PHScene::GetEngine(int i){
	return XCAST(engines[i]);
}

PHConstraintEngineIf* PHScene::GetConstraintEngine(){
	return XCAST(constraintEngine);
}

PHGravityEngineIf* PHScene::GetGravityEngine(){
	return XCAST(gravityEngine);
}

PHPenaltyEngineIf* PHScene::GetPenaltyEngine(){
	return XCAST(penaltyEngine);
}


}

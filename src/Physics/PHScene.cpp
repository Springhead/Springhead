#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <sstream>

namespace Spr{;

//----------------------------------------------------------------------------
//	PHScene
IF_OBJECT_IMP(PHScene, Scene);

PHScene::PHScene(const PHSceneDesc& desc, PHSdkIf* s):PHSceneDesc(desc){
	Init();
	sdk = s;
}
PHScene::PHScene(){
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
	
	PHGravityEngine* ge = DBG_NEW PHGravityEngine;
	engines.Add(ge);
	ge->accel = gravity;

	switch(contactSolver){
	case SOLVER_PENALTY:{
		PHPenaltyEngine* pe = DBG_NEW PHPenaltyEngine;
		engines.Add(pe);
		}break;
	case SOLVER_CONSTRAINT:{
		PHConstraintEngine* ce = DBG_NEW PHConstraintEngine;
		engines.Add(ce);
		}break;
	default: assert(false);
	}

	count = 0;
}

PHSdkIf* PHScene::GetSdk(){
	return sdk;	
}

PHSolidIf* PHScene::CreateSolid(const PHSolidDesc& desc){
	UTRef<PHSolid> s = DBG_NEW PHSolid(desc, (PHSceneIf*)this);
	AddChildObject(s->GetIf());
	return s;
}
PHSolidIf* PHScene::CreateSolid(){
	PHSolidDesc def;
	return CreateSolid(def);
}
CDShapeIf* PHScene::CreateShape(const CDShapeDesc& desc){
	CDShapeIf* rv = sdk->CreateShape(desc);
	return rv;
}

int PHScene::NSolids(){
	return solids->solids.size();
}
PHSolidIf** PHScene::GetSolids(){
	return (PHSolidIf**)&*solids->solids.begin();
}

PHJointIf* PHScene::CreateJoint(const PHJointDesc& desc){
	PHConstraintEngine* ce;
	engines.Find(ce);
	assert(ce);
	return ce->AddJoint(desc);
}

PHPathIf* PHScene::CreatePath(const PHPathDesc& desc){
	return DBG_NEW PHPath(desc);
}

PHJointIf* PHScene::CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const PHJointDesc& desc){
	PHConstraintEngine* ce;
	engines.Find(ce);
	assert(ce);
	return ce->AddJoint((PHSolid*)lhs, (PHSolid*)rhs, desc);	
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
#define FP_ERROR_MASK	(_EM_INEXACT|_EM_UNDERFLOW)
void PHScene::GenerateForce(){
//	_controlfp(FP_ERROR_MASK, _MCW_EM);	//	コメントをはずすと例外が起きる．要調査
	engines.GenerateForce();
//	_controlfp(_MCW_EM, _MCW_EM);
}
void PHScene::Integrate(){
//	_controlfp(FP_ERROR_MASK, _MCW_EM);
	engines.Integrate();
//	_controlfp(_MCW_EM, _MCW_EM);
//	time += timeStep;
	count++;
}

void PHScene::EnableContact(PHSolidIf* lhs, PHSolidIf* rhs, bool bEnable){
	PHConstraintEngine* ce;
	engines.Find(ce);
	assert(ce);
	ce->EnableContact((PHSolid*)lhs, (PHSolid*)rhs, bEnable);
}

void PHScene::SetGravity(Vec3f accel){
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
			rv = CreateShape(*(CDShapeDesc*)desc);
		}else if (info->Inherit(PHJointIf::GetIfInfoStatic())){
			//この段階でPHSolidへの参照はNULL．AddChildObjectで解決される
			rv = CreateJoint(*(PHJointDesc*)desc);
		}else if(info == PHSolidIf::GetIfInfoStatic()){
			rv = CreateSolid(*(const PHSolidDesc*)desc);
		}
	}
	return rv;
}
size_t PHScene::NChildObject() const{
	return engines.size();
}
ObjectIf* PHScene::GetChildObject(size_t pos){
	return (PHEngineIf*)engines[pos];
}
bool PHScene::AddChildObject(ObjectIf* o){
	bool rv = solids->AddChildObject(o);
	PHSolid* s = DCAST(PHSolid, o);
	if (rv){
		switch(contactSolver){
		case SOLVER_PENALTY:{
			PHPenaltyEngine* pe;
			engines.Find(pe);
			assert(pe);
			pe->Add(s);
			}break;
		case SOLVER_CONSTRAINT:{
			PHConstraintEngine* ce;
			engines.Find(ce);
			assert(ce);
			ce->Add(s);
			}break;
		default: assert(false);
		}
	}
	return rv;
}


}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHScene.h>
#include <Physics/PHSdk.h>
#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHPathJoint.h>
#include <Physics/PHForceField.h>
#include <Physics/PHPenaltyEngine.h>
#include <Physics/PHConstraintEngine.h>
#include <Physics/PHHapticEngine.h>
#include <Physics/PHFemEngine.h>
#include <Physics/PHOpEngine.h>
#include <sstream>

#include <Foundation/UTPreciseTimer.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	PHSceneDesc
void PHSceneDesc::Init(){
	PHSceneState::Init();
	gravity				     = Vec3d(0.0, -9.8, 0.0);
	airResistanceRate	     = 1.0;
	contactTolerance         = 0.002;
	impactThreshold          = 10.0;
	frictionThreshold        = 0.01;
	maxDeltaPosition         = FLT_MAX;
	maxDeltaOrientation      = FLT_MAX;
	maxVelocity              = FLT_MAX;
	maxAngularVelocity       = 100.0;
	maxForce                 = FLT_MAX;
	maxMoment                = FLT_MAX;
	numIteration		     = 15;
	method                   = SOLVER_GS;
	bContactDetectionEnabled = true;
	bCCDEnabled              = true;
	broadPhaseMode           = MODE_SORT_AND_SWEEP_X;
	blendMode                = BLEND_AVE_ADD;
}
//----------------------------------------------------------------------------
//	PHScene
PHScene::PHScene(const PHSceneDesc& desc):PHSceneDesc(desc){
	Init();
}
void PHScene::Init(){
	//コンストラクタで設定した値を上書きしてしまうため、コメントアウトしました
	//PHSceneDesc::Init();

	engines.scene = this;
	Scene::Clear();

	// エンジン作成
	solids = DBG_NEW PHSolidContainer;
	engines.Add(solids);
	PHSolidInitializer* si = DBG_NEW PHSolidInitializer;
	si->container = solids;
	engines.Add(si);
	
	gravityEngine = DBG_NEW PHGravityEngine;
	engines.Add(gravityEngine);
	
	penaltyEngine = DBG_NEW PHPenaltyEngine;
	engines.Add(penaltyEngine);
	
	constraintEngine = DBG_NEW PHConstraintEngine;
	engines.Add(constraintEngine);

	ikEngine = DBG_NEW PHIKEngine;
	engines.Add(ikEngine);

	femEngine = DBG_NEW PHFemEngine;
	engines.Add(femEngine);

	hapticEngine = DBG_NEW PHHapticEngine;
	engines.Add(hapticEngine);

	opEngine = DBG_NEW PHOpEngine;
	engines.Add(opEngine);

	opSpAgent = DBG_NEW PHOpSpHashColliAgent;
	opSpAgent->SetScene(this->Cast());
	opSpAgent->spTable = DBG_NEW CDSpatialHashTable();
	
	PHHapticEngineCallStep2* hpe = DBG_NEW PHHapticEngineCallStep2;
	hpe->engine = hapticEngine;
	engines.Add(hpe);

	AfterSetDesc();
}
void PHScene::AfterSetDesc(){
	gravityEngine->accel = gravity;
	constraintEngine->numIter = numIteration;
	constraintEngine->method  = method;
	timeStepInv = 1.0/timeStep;

}
void PHScene::BeforeGetDesc() const{
	// EngineのAPIを介して変更される可能性もあるので
	// BeforeGetDescがconst仕様なのでcastが必要
	(Vec3d&)gravity    = gravityEngine->accel;
	(int&)numIteration = constraintEngine->numIter;
	(int&)method       = constraintEngine->method;
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
	return (int)solids->solids.size();
}
PHSolidIf** PHScene::GetSolids(){
	return solids->solids.empty() ? NULL : (PHSolidIf**)&*solids->solids.begin();
}

CDShapeIf* PHScene::CreateShape(const IfInfo* ii, const CDShapeDesc& desc){
	return GetSdk()->CreateShape(ii, desc);
}

PHJointIf* PHScene::CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const PHJointDesc& desc){
	PHJoint* joint = constraintEngine->CreateJoint(ii, desc, lhs->Cast(), rhs->Cast());
	AddChildObject(joint->Cast());
	return joint->Cast();
}
int PHScene::NJoints()const{
	return (int)constraintEngine->joints.size();
}
PHJointIf* PHScene::GetJoint(int i){
	return DCAST(PHJointIf, constraintEngine->joints[i]);
}
int PHScene::NContacts()const{
	return (int)constraintEngine->points.size();
}
PHContactPointIf* PHScene::GetContact(int i){
	return DCAST(PHContactPointIf, constraintEngine->points[i]);
}

int	PHScene::NSolidPairs()const{
	return (int)constraintEngine->solidPairs.size();
}

PHSolidPairForLCPIf* PHScene::GetSolidPair(int i, int j){
	return constraintEngine->solidPairs.item(i, j)->Cast();
}

PHSolidPairForLCPIf* PHScene::GetSolidPair(PHSolidIf* lhs, PHSolidIf* rhs, bool& bSwap){
	bSwap = false;
	int l = -1, r = -1;
	PHSolidIf** solid = GetSolids();
	for(int i = 0; i < NSolids(); i++){
		if(solid[i] == lhs) l = i;
		if(solid[i] == rhs) r = i;
	}
	if(l == -1 || r == -1) return NULL;
	if(l > r){
		std::swap(l, r);
		bSwap = true;
	}
	return GetSolidPair(l, r);
}

PHRootNodeIf* PHScene::CreateRootNode(PHSolidIf* root, const PHRootNodeDesc& desc){
	PHRootNode* node = constraintEngine->CreateRootNode(desc, root->Cast());
	if(!node)
		return NULL;
	AddChildObject(node->Cast());
	return node->Cast();
}
int PHScene::NRootNodes()const{
	return (int)constraintEngine->trees.size();
}
PHRootNodeIf* PHScene::GetRootNode(int i){
	return constraintEngine->trees[i]->Cast();
}

PHTreeNodeIf* PHScene::CreateTreeNode(PHTreeNodeIf* parent, PHSolidIf* child, const PHTreeNodeDesc& desc){
	PHTreeNode* node = constraintEngine->CreateTreeNode(desc, parent->Cast(), child->Cast());
	if(!node)
		return NULL;
	node->SetScene(Cast());
	return node->Cast();
}

void PHScene::CreateTreeNodesRecurs(PHTreeNodeIf* node, PHSolidIf* solid){
	PHSolidIf* sockSolid;
	PHSolidIf* plugSolid;
	for(int i = 0; i < NJoints(); i++){
		PHJointIf* jnt = GetJoint(i);
		sockSolid = jnt->GetSocketSolid();
		plugSolid = jnt->GetPlugSolid  ();
		if(sockSolid == solid && !DCAST(PHSolid, plugSolid)->IsArticulated()){
			PHTreeNodeIf* childNode = CreateTreeNode(node, plugSolid);
			CreateTreeNodesRecurs(childNode, plugSolid);
		}
	}
}

PHRootNodeIf* PHScene::CreateTreeNodes(PHSolidIf* solid){
	if(DCAST(PHSolid, solid)->IsArticulated())
		return 0;

	PHRootNodeIf* rootNode = CreateRootNode(solid);
	CreateTreeNodesRecurs(rootNode, solid);

	rootNode->Print(DSTR);

	return rootNode;
}

PHGearIf* PHScene::CreateGear(PH1DJointIf* lhs, PH1DJointIf* rhs, const PHGearDesc& desc){
	PHGear* gear = constraintEngine->CreateGear(desc, lhs->Cast(), rhs->Cast());
	if(!gear)
		return NULL;
	AddChildObject(gear->Cast());
	return gear->Cast();
}
int PHScene::NGears()const{
	return (int)constraintEngine->gears.size();
}
PHGearIf* PHScene::GetGear(int i){
	return constraintEngine->gears[i]->Cast();
}
PHPathIf* PHScene::CreatePath(const PHPathDesc& desc){
	PHPath* path = constraintEngine->CreatePath(desc)->Cast();
	AddChildObject(path->Cast());
	return path->Cast();
}
int PHScene::NPaths()const{
	return (int)constraintEngine->paths.size();
}
PHPathIf* PHScene::GetPath(int i){
	return constraintEngine->paths[i]->Cast();
}
PHRayIf* PHScene::CreateRay(const PHRayDesc& desc){
	PHRay* ray = DBG_NEW PHRay();
	ray->SetDesc(&desc);
	AddChildObject(ray->Cast());
	return ray->Cast();
}
int PHScene::NRays()const{
	return (int)rays.size();
}
PHRayIf* PHScene::GetRay(int i){
	return rays[i]->Cast();
}

PHIKActuatorIf* PHScene::CreateIKActuator(const IfInfo* ii, const PHIKActuatorDesc& desc){
	PHIKActuator* actuator = ikEngine->CreateIKActuator(ii, desc)->Cast();
	AddChildObject(actuator->Cast());
	return actuator->Cast();
}
int PHScene::NIKActuators() const{
	return (int)ikEngine->actuators.size();
}
PHIKActuatorIf* PHScene::GetIKActuator(int i){
	return ikEngine->actuators[i]->Cast();
}
PHIKEndEffectorIf* PHScene::CreateIKEndEffector(const PHIKEndEffectorDesc& desc){
	PHIKEndEffector* endeffector = ikEngine->CreateIKEndEffector(desc)->Cast();
	AddChildObject(endeffector->Cast());
	return endeffector->Cast();
}
int PHScene::NIKEndEffectors() const{
	return (int)ikEngine->endeffectors.size();
}
PHIKEndEffectorIf* PHScene::GetIKEndEffector(int i){
	return ikEngine->endeffectors[i]->Cast();
}
int PHScene::NFemMeshes() const {
	return (int)femEngine->meshes.size();
}
PHFemMeshIf* PHScene::GetFemMesh(int i){
	return femEngine->meshes[i]->Cast();
}
int PHScene::NFemMeshesNew() const{
	return (int)femEngine->meshes_n.size();
}
PHFemMeshNewIf*	PHScene::GetFemMeshNew(int i){
	return femEngine->meshes_n[i]->Cast();
}
int PHScene::NOpObjs() const {
	return (int)opEngine->opObjs.size();
}
PHOpObj* PHScene::GetOpObj(int i){
	return opEngine->opObjs[i]->Cast();
}

void PHScene::Clear(){
	engines.Clear();
	Init();
}

void PHScene::SetTimeStep(double dt){
	timeStep = dt;
	timeStepInv = 1.0/dt;
}

static UTPreciseTimer ptimer;

void PHScene::Step(){
	int t0, t1, t2;
	ptimer.CountUS();
	ClearForce();
	t0 = ptimer.CountUS();
	ptimer.CountUS();
	GenerateForce();
	t1 = ptimer.CountUS();
	ptimer.CountUS();
	Integrate();
	t2 = ptimer.CountUS();
	//DSTR << "clear: " << t0 << " gen: " << t1 << " int: " << t2 << std::endl;
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
void PHScene::IntegratePart1(){
	for(PHEngines::iterator it = engines.dynamicalSystem; *it!=constraintEngine && it!=engines.end2(); ++it){
		(*it)->Step();		
	}
	constraintEngine->StepPart1();
}
void PHScene::IntegratePart2(){
	constraintEngine->StepPart2();
	PHEngines::iterator it;
	for(it = engines.dynamicalSystem; it!=engines.end2(); ++it){
		if (*it==constraintEngine){
			++it;
			break;
		}
	}
	for(; it!=engines.end2(); ++it){
		(*it)->Step();
	}
}
	
void PHScene::SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, PHSceneDesc::ContactMode mode){
	penaltyEngine   ->EnableContact(lhs, rhs, mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(lhs, rhs, mode == PHSceneDesc::MODE_LCP);
	hapticEngine->EnableContact(lhs, rhs, (mode == PHSceneDesc::MODE_PENALTY) || (mode == PHSceneDesc::MODE_LCP));
}

void PHScene::SetContactMode(PHSolidIf** group, size_t length, PHSceneDesc::ContactMode mode){
	penaltyEngine   ->EnableContact(group, length, mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(group, length, mode == PHSceneDesc::MODE_LCP);
	hapticEngine    ->EnableContact(group, length, (mode == PHSceneDesc::MODE_PENALTY) || (mode == PHSceneDesc::MODE_LCP));
}

void PHScene::SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode mode){
	penaltyEngine   ->EnableContact(solid, mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(solid, mode == PHSceneDesc::MODE_LCP);
	if (!DCAST(PHHapticPointer, solid)) {
		// 対象が通常剛体の場合は、Pointerとの接触を切る必要があるためHapticEngineにも伝える。
		hapticEngine->EnableContact(solid, (mode == PHSceneDesc::MODE_PENALTY) || (mode == PHSceneDesc::MODE_LCP));

		// <!!>
		// PHHapticEngineにより、毎ステップ、
		// PHHapticPointer p に対して SetContactMode(p, NONE) が行われる。
		// そのため対象は通常剛体に限らないと何にも接触しなくなる。
	}
}

void PHScene::SetContactMode(PHSceneDesc::ContactMode mode){
	penaltyEngine   ->EnableContact(mode == PHSceneDesc::MODE_PENALTY);
	constraintEngine->EnableContact(mode == PHSceneDesc::MODE_LCP);
	hapticEngine    ->EnableContact((mode == PHSceneDesc::MODE_PENALTY) || (mode == PHSceneDesc::MODE_LCP));
}
int  PHScene::GetLCPSolver(){
	return constraintEngine->method;
}
void PHScene::SetLCPSolver(int method){
	constraintEngine->method = method;
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
Vec3d PHScene::GetGravity(){
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
	return NSolids() + NJoints() + NRootNodes() + NGears() + NPaths()
		+ NContacts() + NRays() + NIKActuators() + NIKEndEffectors()
		+ 1; // for PHIKEngine
}
ObjectIf* PHScene::GetChildObject(size_t pos){
	//return engines[pos]->Cast();
	if(pos < (size_t)NSolids()) return GetSolids()[pos];
	pos -= NSolids();
	if(pos < (size_t)NJoints()) return GetJoint((int)pos);
	pos -= NJoints();
	if(pos < (size_t)NRootNodes()) return GetRootNode((int)pos);
	pos -= NRootNodes();
	if(pos < (size_t)NGears()) return GetGear((int)pos);
	pos -= NGears();
	if(pos < (size_t)NPaths()) return GetPath((int)pos);
	pos -= NPaths();
	if(pos < (size_t)NContacts()) return GetContact((int)pos);
	pos -= NContacts();
	if(pos < (size_t)NRays()) return GetRay((int)pos);
	pos -= NRays();
	if(pos < (size_t)NIKActuators()) return GetIKActuator((int)pos);
	pos -= NIKActuators();
	if(pos < (size_t)NIKEndEffectors()) return GetIKEndEffector((int)pos);
	pos -= NIKEndEffectors();
	if (pos < 1) return GetIKEngine();
	pos -= 1;
	return NULL;
}
bool PHScene::AddChildObject(ObjectIf* o){
	bool ok = false;
	PHSolid* solid = DCAST(PHSolid, o);
	if(solid){
		solids          ->AddChildObject(o);
		gravityEngine   ->AddChildObject(o);
		penaltyEngine   ->AddChildObject(o);
		constraintEngine->AddChildObject(o);
		hapticEngine    ->AddChildObject(o);
		
		SetContactMode(solid->Cast(), PHSceneDesc::MODE_LCP);	//デフォルトでLCP
		solid->engine = constraintEngine;
		ok = true;
	}
	PHJointIf* con = DCAST(PHJointIf, o);
	if(con && constraintEngine->AddChildObject(con)){
		ok = true;
	}
	PHTreeNodeIf* node = DCAST(PHTreeNodeIf, o);
	if(node && constraintEngine->AddChildObject(o))
		ok = true;
	PHGearIf* gear = DCAST(PHGearIf, o);
	if(gear && constraintEngine->AddChildObject(o))
		ok = true;
	PHPathIf* path = DCAST(PHPathIf, o);
	if(path && constraintEngine->AddChildObject(o))
		ok = true;
	PHRayIf* ray = DCAST(PHRayIf, o);
	if(ray){
		rays.push_back(ray->Cast());
		ok = true;
	}
	PHIKActuatorIf* ikNode = DCAST(PHIKActuatorIf, o);
	if(ikNode && ikEngine->AddChildObject(o))
		ok = true;
	PHIKEndEffectorIf* ikPoint = DCAST(PHIKEndEffectorIf, o);
	if(ikPoint && ikEngine->AddChildObject(o))
		ok = true;
	PHFemMeshIf* fem = o->Cast();
	if (fem && femEngine->AddChildObject(o))
		ok = true;
	PHFemMeshNewIf* fem_n = o->Cast();
	if(fem_n && femEngine->AddChildObject(o))
		ok = true;
	PHOpObj* opObj = o->Cast();
	if (opObj && opEngine->AddChildObject(o))
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
			else if(con)
				sprintf(name, "joint%d", NJoints()-1);
			else if(node)
				sprintf(name, "node%d", NRootNodes()-1);
			else if(gear)
				sprintf(name, "gear%d", NGears()-1);
			else if(path)
				sprintf(name, "path%d", NPaths()-1);
			else if(ray)
				sprintf(name, "ray%d", NRays() - 1);
			else if(ikNode)
				sprintf(name, "ikact%d", NIKActuators()-1);
			else if(ikPoint)
				sprintf(name, "ikeef%d", NIKEndEffectors()-1);
			else if(fem)
				sprintf(name, "fem%d", NFemMeshes()-1);
			else if(fem_n)
				sprintf(name, "fem%d", NFemMeshes()-1);
			so->SetName(name);
		}
	}

	// Engine is not a SceneObject? <!!>
	PHIKEngine* ikEngine_ = o->Cast();
	if (ikEngine_) {
		PHIKEngineDesc d;
		ikEngine_->GetDesc(&d);
		ikEngine->SetDesc(&d);
		ok = true;
	}

	return ok;
}
bool PHScene::DelChildObject(ObjectIf* o){
	if(DCAST(PHSolidIf, o)){
		gravityEngine->DelChildObject(o);
		penaltyEngine->DelChildObject(o);
		constraintEngine->DelChildObject(o);
		hapticEngine->DelChildObject(o);
		// UTRefで参照しているSolidContainerから最後に削除する
		solids->DelChildObject(o);
		return true;
	}
	
	if(DCAST(PHJointIf, o))
		return constraintEngine->DelChildObject(o);
	
	if(DCAST(PHTreeNodeIf, o))
		return constraintEngine->DelChildObject(o);
	
	if(DCAST(PHGearIf, o))
		return constraintEngine->DelChildObject(o);

	if(DCAST(PHPathIf, o))
		return constraintEngine->DelChildObject(o);

	PHRay* ray = DCAST(PHRay, o);
	if(ray){
		PHRays::iterator it = find(rays.begin(), rays.end(), ray);
		if(it != rays.end()){
			rays.erase(it);
			return true;
		}
		return false;
	}

	if(DCAST(PHIKActuatorIf, o))
		return ikEngine->DelChildObject(o);

	if(DCAST(PHIKEndEffectorIf, o))
		return ikEngine->DelChildObject(o);
		
	if(DCAST(PHFemMeshIf, o))
		return femEngine->DelChildObject(o);

	if (DCAST(PHOpObj, o))
		return opEngine->DelChildObject(o);

	return false;
}

PHEngines* PHScene::GetEngines(){
	return &engines;
}

int PHScene::NEngines(){
	
	return (int)engines.size();
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

PHIKEngineIf* PHScene::GetIKEngine(){
	return XCAST(ikEngine);
}

PHFemEngineIf* PHScene::GetFemEngine(){
	return XCAST(femEngine);
}

PHOpEngineIf* PHScene::GetOpEngine(){
	return XCAST(opEngine);
}

PHOpSpHashColliAgentIf* PHScene:: GetOpColliAgent(){
	return XCAST(opSpAgent);
}

PHHapticEngineIf* PHScene::GetHapticEngine(){
	return XCAST(hapticEngine);
}

PHHapticPointerIf* PHScene::CreateHapticPointer(){
	UTRef< PHHapticPointer > p = DBG_NEW PHHapticPointer();
	AddChildObject(p->Cast());
	return p->Cast();	
}

void PHScene::StepHapticLoop(){
	if(hapticEngine) hapticEngine->StepHapticLoop();
}

size_t PHScene::GetStateSize() const{
	return sizeof(PHSceneState) + (constraintEngine ? constraintEngine->GetStateSize() : 0);
}
void PHScene::ConstructState(void* m) const{
	new (m) PHSceneState();
	char* p = (char*)m;
	p += sizeof(PHSceneState);
	if (constraintEngine){
		constraintEngine->ConstructState(p);
	}
}
void PHScene::DestructState(void* m) const{
	char* p = (char*)m;
	((PHSceneState*)p)->~PHSceneState();
	p += sizeof(PHSceneState);
	if (constraintEngine){
		constraintEngine->DestructState(p);
	}
}
bool PHScene::GetState(void* s) const{
	char* p = (char*) s;
	*(PHSceneState*)p = *this;
	p += sizeof(PHSceneState);
	if (constraintEngine){
		constraintEngine->GetState(p);
	}
	return true;
}
void PHScene::SetState(const void* s){
	const char* p = (char*) s;
	*(PHSceneState*)this = *(const PHSceneState*)p;
	p += sizeof(PHSceneState);
	if (constraintEngine){
		constraintEngine->SetState(p);
	}
}

void PHScene::GetStateR(char*& s){
	bool rv = GetState(s);
	size_t sz = GetStateSize();
	s += sz;
	assert(rv || sz==0);
	size_t n = NChildObject();
	for(size_t i=0; i<n; ++i){
		// childとしてConstraintは除外
		/*→Constraintをセーブしたい時は
		PHConstrainEngine::SetBSaveConstraintsをtrueに
		*/
		if(! DCAST(PHConstraintIf, GetChildObject(i))){
			((PHSolid*)GetChildObject(i))->GetStateR(s);
		}
	}
}
void PHScene::SetStateR(const char*& s){
	SetState(s);
	s += GetStateSize();
	size_t n = NChildObject();
	for(size_t i=0; i<n; ++i){
		// childとしてConstraintは除外
		/*→Constraintをセーブしたい時は
		PHConstrainEngine::SetBSaveConstraintsをtrueに
		*/
		if(! DCAST(PHConstraintIf, GetChildObject(i))){
			((PHSolid*)GetChildObject(i))->SetStateR(s);
		}
	}
}
bool PHScene::WriteStateR(std::ostream& fout){
	fout << '\n';
	fout.write(GetTypeInfo()->ClassName(), strlen(GetTypeInfo()->ClassName()));
	fout << '\t';
	size_t ss = GetStateSize();
	char* state = new char[ss];
	ConstructState(state);
	GetState(state);
	fout.write((char*)&ss, sizeof(ss));
	fout.write(state, ss);
	if (constraintEngine->bSaveConstraints){
		int off = (int)(ss - sizeof(PHConstraintsSt));
		PHConstraintsSt* cst = (PHConstraintsSt*)(state + off);
		if (cst->gears.size()) fout.write((char*)&*cst->gears.begin(), sizeof(PHConstraintState)*cst->gears.size());
		if (cst->joints.size()) fout.write((char*)&*cst->joints.begin(), sizeof(PHConstraintState)*cst->joints.size());
	}
	DestructState(state);
	delete[] state;
	size_t n = NSolids();
	for(size_t i=0; i<n; ++i) GetSolids()[i]->WriteStateR(fout);
	return true;
}
bool PHScene::ReadStateR(std::istream& fin){
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (fin.get() != '\n') assert(0);
	fin.read(buf, strlen(GetTypeInfo()->ClassName()));
	assert(strcmp(buf, GetTypeInfo()->ClassName()) == 0);
	if (fin.get() != '\t') assert(0);
	size_t ss;
	fin.read((char*)&ss, sizeof(ss));
	char* state = new char[ss];
	fin.read(state, ss);

	PHConstraintsSt* cst = NULL;
	if (constraintEngine->bSaveConstraints){
		int off = int(ss - sizeof(PHConstraintsSt));
		cst = (PHConstraintsSt*)(state + off);
		size_t gsz = cst->gears.size();
		size_t jsz = cst->joints.size();
		new (cst) PHConstraintsSt;
		cst->gears.resize(gsz);
		if (gsz) fin.read((char*)&*cst->gears.begin(), sizeof(PHConstraintState)*gsz);
		cst->joints.resize(jsz);
		if (jsz) fin.read((char*)&*cst->joints.begin(), sizeof(PHConstraintState)*jsz);
	}
	SetState(state);
	if (cst) cst->~PHConstraintsSt();
	delete[] state;
	size_t n = NSolids();
	for(size_t i=0; i<n; ++i) GetSolids()[i]->ReadStateR(fin);
	return true;
}

void PHScene::SetStateMode(bool bConstraints){
	constraintEngine->bSaveConstraints = bConstraints;
}

void PHScene::SetContactDetectionRange(Vec3f center, Vec3f extent, int nx, int ny, int nz){
	constraintEngine->SetDetectionRange(center, extent, nx, ny, nz);
	penaltyEngine   ->SetDetectionRange(center, extent, nx, ny, nz);
	hapticEngine    ->SetDetectionRange(center, extent, nx, ny, nz);
}

void PHScene::SetMaterialBlending(int mode){
	blendMode = mode;
}

void PHScene::DumpObjectR(std::ostream& os, int level) const{
	Object::DumpObjectR(os, level);
	size_t n = engines.size();
	os << std::endl;
	os << level << " PHScene::engines" << std::endl;
	for(size_t i=0; i<n; ++i){
		engines[(int)i]->DumpObjectR(os, level+1);
	}
}

//----------------------------------------------------------------------------
//	PHRay
void PHRay::Apply(){
	Vec3f ori, dir;
	Vec3f point[2];
	float offset[2];
	Posed p, pinv;

	hits.clear();

	PHScene* phScene = DCAST(PHScene, GetScene());
	for(int i = 0; i < phScene->NSolids(); i++){
		PHSolid* solid = DCAST(PHSolid, phScene->GetSolids()[i]);
		std::string solidName = solid->GetName();
		if(!(solidName =="PHCursor")){
			for(int j = 0; j < (int)solid->NShape(); j++){
				p = (solid->GetPose() * solid->GetShapePose(j));
				pinv = p.Inv();
				ori = pinv * origin;
				dir = pinv.Ori() * direction;
				CDShape* shape = solid->GetShape(j)->Cast();
				int num = shape->LineIntersect(ori, dir, point, offset);
				for(int k = 0; k < num; k++){
					hits.push_back(PHRaycastHit());
					hits.back().solid = solid->Cast();
					hits.back().shape = shape->Cast();
					hits.back().point = p * point[k];
					hits.back().distance = offset[k];
				}
			}
		}
	}

}

PHRaycastHit* PHRay::GetNearest(){
	PHRaycastHit* nearest = NULL;
	float distMin = FLT_MAX;
	for(int i = 0; i < (int)hits.size(); i++){
		if(hits[i].distance < distMin){
			nearest = &hits[i];
			distMin = hits[i].distance;
		}
	}
	return nearest;
}

PHRaycastHit* PHRay::GetDynamicalNearest(){
	PHRaycastHit* nearest = NULL;
	float distMin = FLT_MAX;
	for(int i = 0; i < (int)hits.size(); i++){
		if(hits[i].distance < distMin){
			PHRaycastHit* n = &hits[i];
			if(n->solid){
				n->solid->IsDynamical();
				if(n->solid->IsDynamical()){
					nearest = n;
					distMin = n->distance;
				}
			}
		}
	}
	return nearest;
}

}

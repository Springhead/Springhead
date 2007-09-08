/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRInternalScene.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRInternalSceneObject, SceneObject);

PHSolidIf* CRInternalSceneObject::GetSolid(){
	return solid;
}

Vec3f CRInternalSceneObject::GetPos(){
	return position;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRISAttractiveObject, CRInternalSceneObject);

float CRISAttractiveObject::GetAttractiveness(){
	return attractiveness;
}

void CRISAttractiveObject::SetAttractiveness(float attractiveness){
	this->attractiveness = attractiveness;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
IF_OBJECT_IMP(CRISTravelPotentialObject, CRInternalSceneObject);

Vec2f CRISTravelPotentialObject::GetStrengthCoeff(){
	return strength;
}

void CRISTravelPotentialObject::SetStrengthCoeff(Vec2f strength){
	this->strength = strength;
}

Vec2f CRISTravelPotentialObject::GetDecayCoeff(){
	return decay;
}

void CRISTravelPotentialObject::SetDecayCoeff(Vec2f decay){
	this->decay = decay;
}

Vec2f CRISTravelPotentialObject::GetPotential(Vec2f currPos){
	Vec3f objPos3 = solid->GetPose() * position;
	Vec2f objPos = Vec2f(objPos3.X(), objPos3.Z());
	float dist = (currPos - objPos).norm();
	Vec2f dir  = (currPos - objPos).unit();
	float U = -strength.X()/pow(dist, decay.X()) + strength.Y()/pow(dist, decay.Y());
	return(U * dir);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// 
bool CRInternalScene::LargerAttention(const CRInternalSceneObject* &a, const CRInternalSceneObject* &b){
	// Attentionの降順に並べるのが目的
	// ただしAttractiveObjectでないISObjはAttractiveObjectよりさらに後

	CRISAttractiveObjectIf* attA = DCAST(CRISAttractiveObjectIf, a);
	CRISAttractiveObjectIf* attB = DCAST(CRISAttractiveObjectIf, b);

	if (a==NULL && b==NULL) {
		return true;
	} else if (a==NULL && b!=NULL) {
		return false;
	} else if (a!=NULL && b==NULL) {
		return true;
	} else {
		return(attA->GetAttractiveness() > attB->GetAttractiveness());
	}
}

IF_OBJECT_IMP(CRInternalScene, SceneObject);

void CRInternalScene::SortByAttractiveness(){
	std::sort(sceneObjects.begin(), sceneObjects.end(), CRInternalScene::LargerAttention);
}

CRInternalSceneObjectIf* CRInternalScene::FindObject(PHSolidIf* solid, Vec3f pos){
	for (int i=0; i<sceneObjects.size(); i++) {
		if (sceneObjects[i]->GetSolid() == solid && (sceneObjects[i]->GetPos() - pos).norm() < 0.001) { // 数値は変更可能にすること(mitake)
			return sceneObjects[i]->Cast();
		}
	}
	return NULL;
}

CRInternalSceneObjectIf* CRInternalScene::GetObject(int i){
	return sceneObjects[i]->Cast();
}

int CRInternalScene::NObjects(){
	return sceneObjects.size();
}

CRInternalSceneObjectIf* CRInternalScene::CreateInternalSceneObject(const IfInfo* ii, const CRInternalSceneObjectDesc& desc){
	if (ii == CRISAttractiveObjectIf::GetIfInfoStatic()) {
		sceneObjects.push_back(DBG_NEW CRISAttractiveObject((const CRISAttractiveObjectDesc&)desc, this->Cast()));

	} else if (ii == CRISTravelPotentialObjectIf::GetIfInfoStatic()) {
		sceneObjects.push_back(DBG_NEW CRISTravelPotentialObject((const CRISTravelPotentialObjectDesc&)desc, this->Cast()));

	} else {
		assert(0 && "想定されてない型");

	}

	return sceneObjects.back()->Cast();
}
}

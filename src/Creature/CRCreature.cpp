/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Creature/CRCreature.h>
#include <Creature/CREngine.h>
#include <Creature/CRBody.h>
#include <Creature/CRScene.h>
#include <Physics/SprPHScene.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

// -------------------------------------------------------------------------

void CRCreature::Step() {
	for (size_t i=0; i<engines.size(); ++i) {
		if (engines[i]->IsEnabled()) {
			engines[i]->Step();
		}
	}
}

CRBodyIf* CRCreature::CreateBody(const IfInfo* ii, const CRBodyDesc& desc) {
	CRBodyIf* crBody = CreateObject(ii, &desc)->Cast();
	AddChildObject(crBody);
	return crBody;
}

CREngineIf* CRCreature::CreateEngine(const IfInfo* ii, const CREngineDesc& desc) {
	CREngineIf* crEngine = CreateObject(ii, &desc)->Cast();
	AddChildObject(crEngine);
	return crEngine;
}

CRSceneIf* CRCreature::CreateScene(const IfInfo* ii, const CRSceneDesc& desc) {
	CRSceneIf* crScene = CreateObject(ii, &desc)->Cast();
	AddChildObject(crScene);
	return crScene;
}

ObjectIf* CRCreature::GetChildObject(size_t i) {
	if (i < bodies.size()) {
		return bodies[i];
	} else if (i < bodies.size() + engines.size()) {
		return engines[i - bodies.size()];
	} else if (i < bodies.size() + engines.size() + scenes.size()) {
		return scenes[i - bodies.size() - engines.size()];
	} else {
		return NULL;
	}
}

bool CRCreature::AddChildObject(ObjectIf* o){
	DSTR << o->GetIfInfo()->ClassName() << std::endl;

	CRBodyIf* b = DCAST(CRBodyIf, o);
	if (b){
		if (std::find(bodies.begin(), bodies.end(), b) == bodies.end()){
			bodies.push_back(b);
			DCAST(SceneObject, b)->SetScene(this->Cast());
			return true;
		}
	}

	CREngineIf* e = DCAST(CREngineIf, o);
	if (e){
		if (std::find(engines.begin(), engines.end(), e) == engines.end()){
			engines.push_back(e);
			// エンジンをプライオリティに従ってソート
			UTContentsLess< UTRef<CREngine> > less;
			std::sort(engines.begin(), engines.end(), less);
			DCAST(SceneObject, e)->SetScene(this->Cast());
			return true;
		}
	}

	CRSceneIf* s = DCAST(CRSceneIf, o);
	if (s){
		if (std::find(scenes.begin(), scenes.end(), s) == scenes.end()){
			scenes.push_back(s);
			DCAST(SceneObject, s)->SetScene(this->Cast());
			return true;
		}
	}

	PHSceneIf* p = DCAST(PHSceneIf, o);
	if (p){
		phScene = p;
		return true;
	}

	return false;
}

bool CRCreature::DelChildObject(ObjectIf* o){
	CRBodyIf* b = DCAST(CRBodyIf, o);
	if (b){
		CRBodies::iterator it = std::find(bodies.begin(), bodies.end(), b);
		if(it != bodies.end()){
			bodies.erase(it);
			return true;
		}
	}

	CREngineIf* e = DCAST(CREngineIf, o);
	if (e){
		CREngines::iterator it = std::find(engines.begin(), engines.end(), e);
		if(it != engines.end()){
			engines.erase(it);
			return true;
		}
	}

	CRSceneIf* s = DCAST(CRSceneIf, o);
	if (s){
		CRScenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if(it != scenes.end()){
			scenes.erase(it);
			return true;
		}
	}
	return false;
}

}

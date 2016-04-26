/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHEngine.h>
#include <Physics/PHScene.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	PHEngine

PHEngine::PHEngine(){
	bEnabled = true;
}

//----------------------------------------------------------------------------
//	PHEngines

void PHEngines::Add(PHEngine* e){
	e->SetScene(scene->Cast());
	push_back(e);
	Sort();
}
PHEngine* PHEngines::FindInherit(const UTTypeInfo& ti) const {
	for(const_iterator it = base::begin(); it != base::end(); ++it){
//		DSTR << (*it)->GetTypeInfo()->className << std::endl;
		if ((*it)->GetTypeInfo()->Inherit(&ti)){
			return *it;
		}
	}
	return NULL;
}
PHEngine* PHEngines::FindInherit(const UTTypeInfo& ti, UTString name) const {
	for(const_iterator it = base::begin(); it != base::end(); ++it){
		if ((*it)->GetTypeInfo()->Inherit(&ti) && name.compare((*it)->GetName())==0){
			return *it;
		}
	}
	return NULL;
}

PHEngines::PHEngines(){
}

void PHEngines::Clear(){
	for(iterator it = base::begin(); it != base::end(); ++it) (*it)->Clear();
	clear();
	initializer = forceGenerator = dynamicalSystem = motion = listener = end2();
}

void PHEngines::ClearForce(){
	for(iterator it = begin2(); it != forceGenerator && it!=end2(); ++it){
		PHEngine* e = *it;
		if(e->bEnabled)
			e->Step();
	}
}

void PHEngines::GenerateForce(){
	for(iterator it = forceGenerator; it != dynamicalSystem && it!=end2(); ++it){
		PHEngine* e = *it;
		if(e->bEnabled)
			e->Step();
	}
}

void PHEngines::Integrate(){
	for(iterator it = dynamicalSystem; it != motion && it!=end2(); ++it){	//本当はmotionまでにすべきだけど，
		PHEngine* e = *it;
		if(e->bEnabled)
			e->Step();
	}
}


void PHEngines::Sort(){
	//エンジンをプライオリティに従ってソート
	UTContentsLess< UTRef<PHEngine> > less;
	std::sort(begin2(), end2(), less);

	//各カテゴリの先頭へのイテレータを記憶
	initializer = forceGenerator = dynamicalSystem = motion = listener = end2();
	for(iterator it = begin2(); it != end2(); ++it){
		if(initializer == end2() && (*it)->GetPriority() >= SGBP_INITIALIZER)
			initializer = it;

		if(forceGenerator == end2() && (*it)->GetPriority() >= SGBP_FORCEGENERATOR)
			forceGenerator = it;

		if (dynamicalSystem == end2() && (*it)->GetPriority() >= SGBP_DYNAMICALSYSTEM)
			dynamicalSystem = it;

		if (motion == end2() && (*it)->GetPriority() >= SGBP_MOTION)
			motion = it;

		if (listener == end2() && (*it)->GetPriority() >= SGBP_LISTENER)
			listener = it;
	}
}

bool PHEngines::Del(PHEngine* e){
	PHEngines::iterator it = std::find(begin2(), end2(), UTRef<PHEngine>(e));
	if (it != end()){
		erase(it);
		Sort();
		return true;
	}
	return false;
}


}

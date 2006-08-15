/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Object.h"
#include "Scene.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <sstream>
#include <iomanip>

namespace Spr {;
int IfInfo::maxId;
IfInfo::IfInfo(const char* cn, IfInfo** b): className(cn), base(b){
	id = ++maxId;
}
bool IfInfo::Inherit(const char* key) const {
	if(strcmp(ClassName(),key)==0) return true;
	IfInfo** pb = base;
	while(*pb){
		if ((*pb)->Inherit(key)) return true;
		++pb;
	}
	return false;
}
bool IfInfo::Inherit(const IfInfo* key) const {
#ifdef __BORLANDC__
	if(strcmp(ClassName(),key->ClassName())==0) return true;
#else
	if(this == key) return true;
#endif
	IfInfo** pb = base;
	while(*pb){
		if ((*pb)->Inherit(key)) return true;
		++pb;
	}
	return false;
}
void IfInfo::RegisterFactory(FactoryBase* f) const {
	IfInfo* info = (IfInfo*) this;
	info->factories.push_back(f);
}
FactoryBase* IfInfo::FindFactory(const IfInfo* info) const {
	for (Factories::const_iterator it = factories.begin(); it != factories.end(); ++it){
		if(info == (*it)->GetIfInfo()){
			return (FactoryBase*)*it;
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------
//	Object
IF_IMP_BASE(Object);
OBJECT_IMP_BASE(Object);

void Object::PrintHeader(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << ">" << std::endl;
	os.width(w);
}
void Object::PrintChildren(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os.width(w);
	os.widen(w+2);
	for(size_t i=0; i<NChildObject(); ++i){
		GetChildObject(i)->Print(os);
	}
}
void Object::PrintFooter(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "</" << GetTypeInfo()->ClassName() << ">" << std::endl;
	os.width(w);
}
void Object::PrintShort(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << "/>" << std::endl;
	os.width(w);
}
void Object::Print(std::ostream& os) const {
	int w = os.width();
	if (NChildObject()){
		PrintHeader(os);
		PrintChildren(os);
		PrintFooter(os);
	}else{
		os.width(0);
		os << UTPadding(w);
		os << "<" << GetTypeInfo()->ClassName() << "/>" << std::endl;
	}
	os.width(w);
}
ObjectIf* Object::CreateObject(const IfInfo* keyInfo, const void* desc){
	const IfInfo* info = GetIf()->GetIfInfo();
	FactoryBase* factory = info->FindFactory(keyInfo);
	if (factory){
		return factory->Create(desc, GetIf());
	}
	return NULL;
}

//----------------------------------------------------------------------------
//	NamedObject
IF_OBJECT_IMP(NamedObject, Object);

void NamedObject::SetNameManager(NameManager* s){
	assert(!nameManager);
	nameManager = s;
}
void NamedObject::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	if (NChildObject()){
		os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << ">" << std::endl;
		os.width(w+2);
		for(size_t i=0; i<NChildObject(); ++i){
			GetChildObject(i)->Print(os);
		}
		os.width(0);
		os << UTPadding(w);
		os << "</" << GetTypeInfo()->ClassName() << ">" << std::endl;
	}else{
		os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << "/>" << std::endl;
	}
	os.width(w);
}
void NamedObject::PrintShort(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << "/>" << std::endl;
	os.width(w);
}
void NamedObject::SetName(const char* n){
	if (name.compare(n) == 0) return;
	assert(nameManager);
	if (name.length()) nameManager->names.Del(this);
	name = n;
	nameManager->names.Add(this);
}

//----------------------------------------------------------------------------
//	SceneObject
IF_OBJECT_IMP(SceneObject, NamedObject);

void SceneObject::SetScene(SceneIf* s){
	SetNameManager(DCAST(NameManager, s));
	nameManager->GetNameMap();
}
SceneIf* SceneObject::GetScene(){
	NameManager* nm = GetNameManager();
	return DCAST(Scene, nm);
}


//----------------------------------------------------------------------------
//	ObjectStates

IF_OBJECT_IMP(ObjectStates, Object);

size_t ObjectStates::CalcStateSize(ObjectIf* o){
	size_t sz = o->GetStateSize();
	size_t n = o->NChildObject();
	for(size_t i=0; i<n; ++i){
		sz += CalcStateSize(o->GetChildObject(i));
	}
	return sz;
}
void ObjectStates::ReleaseState(ObjectIf* o){
	char* s = state;
	DestructState(o, s);
	delete state;
	state = NULL;
	size=0;
}
void ObjectStates::DestructState(ObjectIf* o, char*& s){
	o->DestructState(s);
	s += o->GetStateSize();
	size_t n = o->NChildObject();
	for(size_t i=0; i<n; ++i){
		DestructState(o->GetChildObject(i), s);
	}
}
void ObjectStates::ConstructState(ObjectIf* o, char*& s){
	o->ConstructState(s);
	s += o->GetStateSize();
	size_t n = o->NChildObject();
	for(size_t i=0; i<n; ++i){
		ConstructState(o->GetChildObject(i), s);
	}	
}
void ObjectStates::AllocateState(ObjectIf* o){
	if (state) ReleaseState(o);
	size = CalcStateSize(o);
	state = new char[size];
	char* s = state;
	ConstructState(o, s);
}
void ObjectStates::SaveState(ObjectIf* o){
	if (!state) AllocateState(o);
	char* s = state;
//	DSTR << "Save:" << std::endl;
	SaveState(o, s);
}

void ObjectStates::SaveState(ObjectIf* o, char*& s){
//	DSTR << std::setbase(16) <<  (unsigned)s << " " << o->GetStateSize() << "  ";
//	DSTR << o->GetIfInfo()->ClassName() << std::endl;

	bool rv = o->GetState(s);
	size_t sz = o->GetStateSize();
	s += sz;
	assert(rv || sz==0);
	size_t n = o->NChildObject();
	for(size_t i=0; i<n; ++i){
		SaveState(o->GetChildObject(i), s);
	}
}
void ObjectStates::LoadState(ObjectIf* o){
	char* s = state;
//	DSTR << "Load:" << std::endl;
	LoadState(o, s);
}
void ObjectStates::LoadState(ObjectIf* o, char*& s){
//	DSTR << std::setbase(16) <<  (unsigned)s << " " << o->GetStateSize() << "  ";
//	DSTR << o->GetIfInfo()->ClassName() << std::endl;

	o->SetState(s);
	s += o->GetStateSize();
	size_t n = o->NChildObject();
	for(size_t i=0; i<n; ++i){
		LoadState(o->GetChildObject(i), s);
	}
}

ObjectStatesIf* SPR_CDECL CreateObjectStates(){
	return new ObjectStates;
}



//----------------------------------------------------------------------------
//	Sdk  Ç±Ç±Ç…ì¸ÇÍÇÈÇÃÇ™ê≥ÇµÇ¢Ç∆Ç¢Ç§ãCÇÕÇµÇ»Ç¢ÇÃÇ≈Ç¢Ç∏ÇÍèCê≥ (06.08.15 mitake)

static std::vector< UTRef<SdkFactoryBase> > sdkFactories;
void SPR_CDECL RegisterSdk(SdkFactoryBase* sdkFactory){
	sdkFactories.push_back(sdkFactory);
}

ObjectIf* SPR_CDECL CreateSdk(const IfInfo* info, const void* desc){
	for (size_t i=0; i<sdkFactories.size(); i++) {
		if (info == sdkFactories[i]->GetIfInfo()) {
			return sdkFactories[i]->Create(desc);
		}
	}
	return NULL;
}



}	//	namespace Spr

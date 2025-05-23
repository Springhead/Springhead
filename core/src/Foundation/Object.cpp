﻿/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/Object.h>
#include <Foundation/Scene.h>
#include <Foundation/UTTypeDesc.h>
#include <sstream>
#include <iomanip>

#include <string.h>  // strcmp

namespace Spr {;

struct MemCheck{
	MemCheck(){
		#if defined _DEBUG && _MSC_VER			
		// メモリリークチェッカ
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		#endif
	}
	void Start(){}
};
static MemCheck memCheck;


//-------------------------------------------------------------------------
//	IfInfo
//
bool IfInfo::Inherit(const IfInfo* info) const{
	if (info == this) return true;
	const IfInfo** base = baseList;
	for(;*base; ++base){
		if ((*base)->Inherit(info)) return true;
	}
	return false;
}
void IfInfo::SetState(UTTypeDescIf* t) {
	state = t->Cast();
}
void IfInfo::SetDesc(UTTypeDescIf* t) {
	desc = t->Cast();
}

//-------------------------------------------------------------------------
//	TypeInfoManager
//
TypeInfoManager* TypeInfoManager::typeInfoManager;
TypeInfoManager* SPR_CDECL TypeInfoManager::Get(){
	static UTRef<TypeInfoManager> tim;
	if (!typeInfoManager){
		memCheck.Start();
		tim = typeInfoManager = DBG_NEW TypeInfoManager;
	}
	return typeInfoManager;
}
TypeInfoManager::TypeInfoManager(){
}
TypeInfoManager::~TypeInfoManager(){
}
void TypeInfoManager::RegisterIf(IfInfo* i){
	ifs.push_back(i); 
}
void TypeInfoManager::RegisterObject(UTTypeInfo* t){ 
	objects.push_back(t); 
}
IfInfo* TypeInfoManager::FindIfInfo(const char* cn){
	for(unsigned i=0; i != ifs.size(); ++i){
		if (strcmp(ifs[i]->ClassName(), cn) == 0){
			return ifs[i];
		}
	}
	UTString cn2(cn);
	cn2.append("If");
	for(unsigned i=0; i != ifs.size(); ++i){
		if (strcmp(ifs[i]->ClassName(), cn2.c_str()) == 0){
			return ifs[i];
		}
	}

	for(unsigned i=0; i != ifs.size(); ++i){
		DSTR << ifs[i]->ClassName() << std::endl;
	}

	return NULL;
}
UTTypeInfo* TypeInfoManager::FindTypeInfo(const char* cn){
	for(int i=0; objects.size(); ++i){
		if (strcmp(objects[i]->ClassName(), cn) == 0){
			return objects[i];
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------
//	IfInfo
//

int IfInfo::maxId;
IfInfo* IfInfo::Find(const char* cname){
	TypeInfoManager* tim = TypeInfoManager::Get();
	return tim->FindIfInfo(cname);
}
void IfInfo::RegisterFactory(FactoryBase* f) const {
	IfInfo* info = (IfInfo*) this;
	info->factories.push_back(f);
	const_cast<IfInfo*>(f->GetIfInfo())->creator = info;
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
ObjectIf::~ObjectIf(){
	assert(0);	//	An interface struct should not be deleted.
}
int ObjectIf::DelRef() const {
	int rv = ((Object*)this)->DelRef();
	if (rv) return rv;
	delete (Object*)this;
	return 1;
}
void Object::PrintHeader(std::ostream& os, bool bClose) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName();
	if (bClose){
		os << "/>" << std::endl;
	}else{
		os << ">" << std::endl;
	}
	os.width(w);
}
void Object::PrintChildren(std::ostream& os) const {
	int w = os.width();
	os.width(w+2);
	for(size_t i=0; i<NChildObject(); ++i){
		GetChildObject(i)->Print(os);
	}
	os.width(w);
}
void Object::PrintFooter(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "</" << GetTypeInfo()->ClassName() << ">" << std::endl;
	os.width(w);
}
void Object::PrintShort(std::ostream& os) const {
	PrintHeader(os, true);
}
void Object::Print(std::ostream& os) const {
	if (NChildObject()){
		PrintHeader(os, false);
		PrintChildren(os);
		PrintFooter(os);
	}else{
		PrintHeader(os, true);
	}
}
ObjectIf* Object::CreateObject(const IfInfo* keyInfo, const void* desc){
	ObjectIf* selfIf = Cast();
	const IfInfo* info = selfIf->GetIfInfo();
	FactoryBase* factory = info->FindFactory(keyInfo);
	if (factory){
		return factory->Create(desc, selfIf);
	}
	return NULL;
}


size_t Object::GetStateSizeR() const {
	size_t rv = GetStateSize();
	size_t n = NChildObjectForState();
	for (size_t i = 0; i < n; ++i) {
		rv += GetChildObjectForState(i)->GetStateSizeR();
	}
	return rv;
}
void Object::ConstructStateR(char*& s) const {
#ifdef _DEBUG
	int ConAdr = (size_t)s & 0xFFFF;
	NamedObjectIf* no = DCAST(NamedObjectIf, this);
	DSTR << "ConstrucctStateR(C=" << std::setbase(16) << ConAdr;
	DSTR << ") of " << (no ? no->GetName() : "") << ": " << GetIfInfo()->ClassName() << std::endl;

	stateConstruct = s;
#endif
	ConstructState(s);
	s += GetStateSize();
	size_t n = NChildObjectForState();
	for (size_t i = 0; i < n; ++i) {
		GetChildObjectForState(i)->ConstructStateR(s);
	}
}
void Object::DestructStateR(char*& s) const {
#ifdef _DEBUG
	assert(stateConstruct == s);
#endif
	DestructState(s);
	s += GetStateSize();
	size_t n = NChildObjectForState();
	for (size_t i = 0; i < n; ++i) {
		GetChildObjectForState(i)->DestructStateR(s);
	}
}
void Object::GetStateR(char*& s) const {
#ifdef _DEBUG
	int GetAdr = (size_t)s & 0xFFFF;
	int ConAdr = (size_t)stateConstruct & 0xFFFF;
	NamedObjectIf* no = DCAST(NamedObjectIf, this);
	DSTR << "GetStateR(G=" << std::setbase(16) << GetAdr << " C=" << ConAdr;
	DSTR << ") of " << (no ? no->GetName() : "") << ": " << GetIfInfo()->ClassName() << std::endl;
	if(stateConstruct != s) _CrtDbgBreak();
#endif
	bool rv = GetState(s);
	size_t sz = GetStateSize();
	s += sz;
	assert(rv || sz == 0);
	size_t n = NChildObjectForState();
	for (size_t i = 0; i < n; ++i) {
		GetChildObjectForState(i)->GetStateR(s);
	}
}
void Object::SetStateR(const char*& s) {
	//	DSTR << std::setbase(16) <<  (unsigned)s << " " << o->GetStateSize() << "  ";
	//	DSTR << o->GetIfInfo()->ClassName() << std::endl;
	SetState(s);
	s += GetStateSize();
	size_t n = NChildObjectForState();
	for (size_t i = 0; i < n; ++i) {
		GetChildObjectForState(i)->SetStateR(s);
	}
}


bool Object::WriteState(std::string fileName){
	std::ofstream fout(fileName.c_str(), std::ios::binary | std::ios::out);
	if(!fout) return false;
	WriteStateR(fout);
	fout.close();
	return true;
}
bool Object::WriteStateR(std::ostream& fout){
	UTTypeDesc* sd = GetIfInfo()->state;
	if (sd){
		fout << '\n' << sd->typeName << '\t';
		size_t ss = GetStateSize();
		char* state = DBG_NEW char [ss];
		ConstructState(state);
		GetState(state);
		sd->Write(fout, state);	
		DestructState(state);
		delete[] state;
	}
	size_t n = NChildObject();
	for(size_t i=0; i<n; ++i){
		(Object*)GetChildObject(i)->WriteStateR(fout);
	}
	return true;
}
bool Object::ReadState(std::string fileName){
	std::ifstream fin(fileName.c_str(), std::ios::binary | std::ios::in);
	if(!fin) return false;
	ReadStateR(fin);
	fin.close();
	return true;
}
bool Object::ReadStateR(std::istream& fin){
	UTTypeDesc* sd = GetIfInfo()->state;
	if (sd){
		if (fin.get() != '\n') assert(0);
		std::string tn;
		fin >> tn;
		assert(tn.compare(sd->typeName)==0);
		if (fin.get() != '\t') assert(0);
		size_t ss = GetStateSize();
		char* state = DBG_NEW char [ss];
		ConstructState(state);
		sd->Read(fin, state);	
		SetState(state);
		DestructState(state);
		delete[] state;
	}
	size_t n = NChildObject();
	for(size_t i=0; i<n; ++i){
		(Object*)GetChildObject(i)->ReadStateR(fin);
	}
	return true;
}

void Object::DumpObjectR(std::ostream & os, ObjectIf::object_set_t& dumped, int level) const {
	os << level << " " << GetTypeInfo()->ClassName() << std::endl;
	DumpObject(os);
	size_t n = NChildObject();
	os << std::endl;
	dumped.insert((ObjectIf*)this);
	for(size_t i=0; i<n; ++i){
		const ObjectIf* child = GetChildObject(i);
		if (dumped.find(child) == dumped.end()) {
			child->DumpObjectR(os, dumped, level + 1);
		}
	}
}

//----------------------------------------------------------------------------
//	NamedObject
NameManagerIf* NamedObject::GetNameManager() const{
	return nameManager->Cast();
}
void NamedObject::SetNameManager(NameManagerIf* s){
	assert(!s || s->RefCount() >= 0);
	if (nameManager){
		nameManager->names.Del(this);
	}
	nameManager = DCAST(NameManager, s);
	if (name.length()){
		if (nameManager) nameManager->names.Add(this);
	}
}
NamedObject::NamedObject(const NamedObject& n):nameManager(n.nameManager){
}
NamedObject& NamedObject::operator=(const NamedObject& n){
	SetName("");
	SetNameManager(n.nameManager->Cast());
	return *this;
}

NamedObject::~NamedObject(){
	if (nameManager && name.length()){
		nameManager->names.Del(this);
		nameManager = NULL;
	}
}

void NamedObject::PrintHeader(std::ostream& os, bool bClose) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << 
		(bClose ? "/>" : ">") << std::endl;
	os.width(w);
}
void NamedObject::SetName(const char* n){
	if (name.compare(n) == 0) return;
	if (name.length()){
		assert(nameManager);
		nameManager->names.Del(this);
	}
	name = n;
	if (name.length()) {
		assert(nameManager);
		nameManager->names.Add(this);
	}
}

//----------------------------------------------------------------------------
//	SceneObject
void SceneObject::SetScene(SceneIf* s){
	SetNameManager(s);
	nameManager->GetNameMap();
}
SceneIf* SceneObject::GetScene() const{
	return DCAST(SceneIf, GetNameManager());
}

SceneObjectIf* SceneObject::CloneObject(){
	return NULL;
}

//----------------------------------------------------------------------------
//	ObjectStates

size_t ObjectStates::CalcStateSize(const ObjectIf* o){
	size_t sz = o->GetStateSizeR();
	return sz;
}
void ObjectStates::ReleaseState(const ObjectIf* o){
	if (!state) return;
	char* s = state;
	o->DestructStateR(s);
	delete state;
	state = NULL;
	size=0;
}

void ObjectStates::AllocateState(const ObjectIf* o){
	if (state) ReleaseState(o);
	size = CalcStateSize(o);
	state = new char[size];
	char* s = state;
	o->ConstructStateR(s);
}
void ObjectStates::SaveState(const ObjectIf* o){
	if (!state) AllocateState(o);
	char* s = state;
//	DSTR << "Save:" << std::endl;
	o->GetStateR(s);
}

void ObjectStates::SingleSave(const ObjectIf* o){
	if (!state) ReleaseState(o);
	size_t size = o->GetStateSize();
	state = new char[size];
	char* s = state;
	o->ConstructState(s);
	((Object*)o)->GetState(s);
}

void ObjectStates::SingleLoad(ObjectIf* o){
	if (!state) return;
	const char* s = state;
	((Object*)o)->SetState(s);
}

void ObjectStates::LoadState(ObjectIf* o){
	if (!state) return;
	const char* s = state;
//	DSTR << "Load:" << std::endl;
	((Object*)o)->SetStateR(s);
}
ObjectStatesIf* ObjectStatesIf::Create(){
	ObjectStates* o = new ObjectStates;
	return o->Cast();
}

}	//	namespace Spr

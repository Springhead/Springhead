#include "Object.h"
#include "Scene.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <sstream>
#include <iomanip>

namespace Spr {;

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

//----------------------------------------------------------------------------
//	Object
IF_IMP_BASE(Object);
OBJECTIMPBASE(Object);

void Object::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	if (NChildObject()){
		os << "<" << GetTypeInfo()->ClassName() << ">" << std::endl;
		os.widen(w+2);
		for(size_t i=0; i<NChildObject(); ++i){
			GetChildObject(i)->Print(os);
		}
		os.width(0);
		os << UTPadding(w);
		os << "</" << GetTypeInfo()->ClassName() << ">" << std::endl;
	}else{
		os << "<" << GetTypeInfo()->ClassName() << "/>" << std::endl;
	}
	os.width(w);
}

//----------------------------------------------------------------------------
//	NamedObject
IF_IMP(NamedObject, Object);
OBJECTIMP(NamedObject, Object);

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
void NamedObject::SetName(const char* n){
	if (name.compare(n) == 0) return;
	assert(nameManager);
	if (name.length()) nameManager->names.Del(this);
	name = n;
	nameManager->names.Add(this);
}

//----------------------------------------------------------------------------
//	SceneObject
IF_IMP(SceneObject, NamedObject);
OBJECTIMP(SceneObject, NamedObject);

void SceneObject::SetScene(SceneIf* s){
	SetNameManager(OCAST(NameManager, s));
	nameManager->GetNameMap();
}
SceneIf* SceneObject::GetScene(){
	NameManager* nm = GetNameManager();
	return DCAST(Scene, nm);
}


//----------------------------------------------------------------------------
//	ObjectStates

IF_IMP(ObjectStates, Object);
OBJECTIMP(ObjectStates, Object);

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

ObjectStatesIf* CreateObjectStates(){
	return new ObjectStates;
}



}	//	namespace Spr

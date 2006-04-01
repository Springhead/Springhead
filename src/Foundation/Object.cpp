//#include "springhead.h"
#include "Object.h"
#include "Scene.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <sstream>

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

}

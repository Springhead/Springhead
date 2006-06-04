#include <Springhead.h>
#include <Foundation/Object.h>
#include "Scene.h"
#include <sstream>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif


namespace Spr{;

UTTypeInfoImp<ObjectNames::ObjectKey> ObjectNames::ObjectKey::typeInfo("", NULL);
ObjectNames::ObjectKey ObjectNames::key;
ObjectNames::ObjectKey::ObjectKey(){
	AddRef();
}
ObjectNames::ObjectKey::~ObjectKey(){
	DelRef();
}
bool ObjectNames::Add(NamedObject* obj){
	if (!obj->GetName() || !strlen(obj->GetName())) return false;
	std::pair<iterator, bool> rv = insert(obj);
	if (rv.second){
		return true;
	}else if (*rv.first == obj){
		return false;
	}
	UTString base = obj->name;
	int i=1;
	do{
		std::ostringstream ss;
		ss << "_" << i << '\0';
		obj->name = base + ss.str();
		rv = insert(obj);
		i++;
	} while(!rv.second);
	nameMap[base] = obj->name;
	DSTR << "name change: " << base << " -> " << obj->name << std::endl;
	return true;
}
void ObjectNames::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	for(const_iterator it = begin(); it != end(); ++it){
		os << UTPadding(w);
		os << (*it)->GetName() << " : " << (*it)->GetTypeInfo()->ClassName();
		os << std::endl;
	}
	os.width(w);
}
bool ObjectNamesLess::operator () (const NamedObject* o1, const NamedObject* o2) const {
	int name = strcmp(o1->GetName(), o2->GetName());
	if (name < 0) return true;
/*	if (name == 0){
		int cls = 0;
		if (GETCLASSNAME(o1)[0] && GETCLASSNAME(o2)[0]){
			cls = strcmp(GETCLASSNAME(o1), GETCLASSNAME(o2));
			if (cls < 0) return true;
		}
	}
*/
	return false;
}
//----------------------------------------------------------------------------
//	NameManager
IF_OBJECT_IMP(NameManager, NamedObject);
void NameManager::ClearName(){
	names.clear();
}
void NameManager::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << ">" << std::endl;
	os.width(w+2);
	names.Print(os); 
	for(size_t i=0; i<NChildObject(); ++i){
		GetChildObject(i)->Print(os);
	}
	os.width(0);
	os << UTPadding(w);
	os << "</" << GetTypeInfo()->ClassName() << ">" << std::endl;
	os.width(w);
}


//----------------------------------------------------------------------------
//	Scene
IF_OBJECT_IMP(Scene, NameManager);

Scene::Scene(){
	Clear();
}
void Scene::Clear(){
	ClearName();
}

}

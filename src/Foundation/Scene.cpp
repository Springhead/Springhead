#include <Springhead.h>
#include <Foundation/Object.h>
#include "Scene.h"
#include <sstream>
#include <FLOAT.H>

#pragma hdrstop


namespace Spr{;

UTTypeInfoImp<ObjectNames::ObjectKey> ObjectNames::ObjectKey::typeInfo("", NULL);
ObjectNames::ObjectKey ObjectNames::key;
ObjectNames::ObjectKey::ObjectKey(){
	AddRef();
}
ObjectNames::ObjectKey::~ObjectKey(){
	DelRef();
}
bool ObjectNames::Add(Object* obj){
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
	for(const_iterator it = begin(); it != end(); ++it){
		os << (*it)->GetName() << " : " << (*it)->GetTypeInfo()->ClassName();
		os << std::endl;
	}
}
bool ObjectNamesLess::operator () (Object* o1, Object* o2) const {
	int name = strcmp(o1->GetName(), o2->GetName());
	if (name < 0) return true;
	if (name == 0){
		int cls = 0;
		if (GETCLASSNAME(o1)[0] && GETCLASSNAME(o2)[0]){
			cls = strcmp(GETCLASSNAME(o1), GETCLASSNAME(o2));
			if (cls < 0) return true;
		}
		if (cls == 0){
			if (o1->GetNameSpace()[0] && o2->GetNameSpace()[0]){
				if (strcmp(o1->GetNameSpace(), o2->GetNameSpace()) < 0) return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------------
//	Scene
OBJECTIMP(Scene, Object);

Scene::Scene(){
	Clear();
}
void Scene::Clear(){
	ClearName();
	Print(DSTR);
}

void Scene::ClearName(){
	names.clear();
}

}

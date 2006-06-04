#ifndef SPR_SCENE_H
#define SPR_SCENE_H
#include <Foundation/SprObject.h>
#include <iosfwd>

namespace Spr{;

struct NameManagerIf: public NamedObjectIf{
	IF_DEF(NameManager);
	template <class T> void FindObject(UTRef<T>& t, UTString name){
		T* p;
		FindObject(p, name);
		t = p;
	}
	template <class T> void FindObject(T*& t, UTString name){
		NamedObjectIf* p = FindObject(name,T::GetIfInfoStatic()->className);
		t = DCAST(T, p);
	}
	virtual NamedObjectIf* FindObject(UTString name, const char* cls=NULL)=0;
};

struct SceneIf: public NameManagerIf{
	IF_DEF(Scene);
};

}

#endif

#ifndef SPR_SCENE_H
#define SPR_SCENE_H
#include <Foundation/SprObject.h>
#include <iosfwd>

namespace Spr{;

struct NameManagerIf: public NamedObjectIf{
	IF_DEF(NameManager);
	template <class T> void FindObject(UTRef<T>& t, UTString name, UTString ns=""){
		T* p;
		FindObject(p, name, ns);
		t = p;
	}
	template <class T> void FindObject(T*& t, UTString name, UTString ns=""){
		NamedObjectIf* p = FindObject(name,T::GetIfInfoStatic()->className, ns);
		t = ICAST(T, p);
	}
	virtual NamedObjectIf* FindObject(UTString name, const char* cls, UTString ns)=0;
};

struct SceneIf: public NameManagerIf{
	IF_DEF(Scene);
};

}

#endif

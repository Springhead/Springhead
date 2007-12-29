/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** @file SprScene.h
	名前を持つオブジェクト
 */
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
	virtual NamedObjectIf* FindObject(UTString name, UTString cls="")=0;
};

struct SceneIf: public NameManagerIf{
	IF_DEF(Scene);
};

struct SdkIf: public NameManagerIf{
	IF_DEF(Sdk);
	static SdkIf* SPR_CDECL CreateSdk(const IfInfo* info, const void* desc);
};

}

#endif

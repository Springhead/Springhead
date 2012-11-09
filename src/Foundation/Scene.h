/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SCENE_H
#define SCENE_H
#include <SprFoundation.h>
#include "Object.h"
#include <set>
#include <map>

namespace Spr {;


class ObjectNamesLess{
public:
	bool operator () (const NamedObject* o1, const NamedObject* o2) const;
};
/**	名前とシーングラフのオブジェクトの対応表
	名前をキーにしたセットを用意し，名前の一意性の保証とオブジェクトの
	検索を実現する．	*/
class ObjectNames:public std::set<NamedObject*, ObjectNamesLess >{
public:
	typedef std::map<UTString, UTString> TNameMap;
	class ObjectKey:public NamedObject{
	public:
		SPR_OBJECTDEF_NOIF(ObjectNames::ObjectKey);
		ObjectKey();
		~ObjectKey();
	};
	TNameMap nameMap;
	static ObjectKey key;
	void Print(std::ostream& os) const;

	///	名前からオブジェクトを取得
	typedef std::pair<iterator, iterator> range_type;
	NamedObject* Find(UTString name, UTString cn="") const {
		key.name = name;
		iterator it = ((ObjectNames*)this)->find(&key);
		if (it == end()) return NULL;
		NamedObject* obj = *it;
		if (!cn.length()) return obj;
		if (obj->GetTypeInfo()->Inherit(cn.c_str())) return obj;
		return NULL;
	}
	/**	オブジェクトの追加，
		名前のないオブジェクトは追加できない．この場合 false を返す．
		追加に成功すると true． すでに登録されていた場合は false を返す．
		名前がぶつかる場合は，追加するオブジェクトの名前が変更される．	*/
	bool Add(NamedObject* obj);
	///	オブジェクトの削除
	bool Del(NamedObject* key){
		iterator it = find(key);
		if (it==end()) return false;
		erase(it);
		return true;
	}
	///	オブジェクトの削除
	bool Del(UTString name){
		NamedObject key;
		key.name = name;
		key.AddRef();
		iterator it = find(&key);
		key.DelRef();
		if (it==end()) return false;
		erase(it);
		return true;
	}
	///	すべて削除
	void Clear();
};
inline std::ostream& operator << (std::ostream& os, const ObjectNames& ns){
	ns.Print(os); return os;
}

class SPR_DLL NameManager:public NamedObject{
public:
	SPR_OBJECTDEF(NameManager);
protected:
	/*	名前とオブジェクトの対応表  */
	ObjectNames names;
	
	///@name	名前空間を構成するためのツリー
	//@{
	///
	typedef std::vector<NameManager*> NameManagers;
	///	子の名前空間
	NameManagers childManagers;
	//@}

public:
	NameManager();
	virtual ~NameManager();
	virtual void SetNameManager(NameManager* s);
	void AddChildManager(NameManager* c);
	void DelChildManager(NameManager* c);
	///	型と名前からオブジェクトを取得
	template <class T> void FindObject(UTRef<T>& t, UTString name){
		T* p;
		FindObject(p, name);
		t = p;
	}
	template <class T> void FindObject(T*& t, UTString name){
		NamedObject* p = FindObject(name, GETCLASSNAMES(T));
		t = DCAST(T, p);
	}
	virtual NamedObjectIf* FindObject(UTString name, UTString cls="");
	typedef ObjectNames::iterator SetIt;
//	typedef std::pair<SetIt, SetIt> SetRange;
//	SetRange RangeObject(UTString n){ return names.Range(n); }
	
	ObjectNames::TNameMap& GetNameMap(){ return names.nameMap; }

	void Clear();

	///	デバッグ用
	void Print(std::ostream& os) const;
	friend class NamedObject;

	//	普通は呼び出さない，名前検索の内部関数．
	NamedObjectIf* FindObjectExact(UTString name, UTString cls="");
	NamedObjectIf* FindObjectFromDescendant(UTString name, UTString cls="");
	NamedObjectIf* FindObjectFromAncestor(UTString name, UTString cls="");	
};

/**	シーンの基本クラス	*/
class SPR_DLL Scene:public NameManager{
	SPR_OBJECTDEF(Scene);
public:
	///	コンストラクタ
};

class SPR_DLL Sdk:public NameManager{
	UTRef<TypeInfoManager> typeInfoManager;	//	typeInfo/IfInfoがsdkが消える前に消えることを避ける。
protected:
	friend struct SdkIf;
	static std::vector< UTRef<FactoryBase> > sdkFactories;
public:
	SPR_OBJECTDEF(Sdk);
	static void SPR_CDECL RegisterFactory(FactoryBase* sdkFactory);
public:
	Sdk();
	virtual ~Sdk(){}
};

}
#endif

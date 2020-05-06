/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef UTLOADHANDLER_H
#define UTLOADHANDLER_H

#include <Foundation/Object.h>
#include <Foundation/UTTypeDesc.h>
#include <Foundation/UTLoadContext.h>
#include <string>

namespace Spr{;
class UTLoadContext;
/**	自動ローダ・セーバで対応できないノードのロード・セーブをするクラス．
*/
class UTLoadHandler:public UTRefCount{
public:
	UTString type;
	
	/// DOMノードのロード開始時に呼ばれる
	virtual void BeforeLoadData(UTLoadedData* d, UTLoadContext* fc){}
	/// DOMノードのロード終了時に呼ばれる
	virtual void AfterLoadData(UTLoadedData* d, UTLoadContext* fc){}

	/// CreateObjectの直前に呼ばれる
	virtual void BeforeCreateObject(UTLoadedData* d, UTLoadContext* fc){}
	/// CreateObjectの直後に呼ばれる
	virtual void AfterCreateObject(UTLoadedData* d, UTLoadContext* fc){}
	/** 子オブジェクトの作成後に呼ばれる
		objは作成された子オブジェクト
	 */
	virtual void AfterCreateChild(UTLoadedData* d, ObjectIf* child, UTLoadContext* fc){}
	/// 子オブジェクトが全ての作成された後に呼ばれる
	virtual void AfterCreateChildren(UTLoadedData* d, UTLoadContext* fc){}
	
	virtual void Save(UTLoadContext* fc){};
	struct Less{
		bool operator()(const UTLoadHandler* h1, const UTLoadHandler* h2) const{
			return h1->type.compare(h2->type) < 0;
		}
	};

	virtual ~UTLoadHandler(){}
};
///	UTLoadHandlerの実装テンプレート
template <class T>
class UTLoadHandlerImp: public UTLoadHandler{
public:
	typedef T Desc;
	UTLoadHandlerImp(const char* t){
		type = t;
	}
	template <class S>
	void Get(S*& s, UTLoadContext* ctx){
		s = NULL;
		for(int i=(int)ctx->objects.size()-1; i>=0 && !s; --i){
			s = DCAST(S, ctx->objects[i]);
		}
		assert(s);
	}
	void BeforeCreateObject(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		BeforeCreateObject(*desc, ld, ctx);
	}
	void AfterCreateObject(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		AfterCreateObject(*desc, ld, ctx);
	}
	void AfterCreateChild(UTLoadedData* ld, ObjectIf* child, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		AfterCreateChild(*desc, ld, child, ctx);
	}
	void AfterCreateChildren(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		AfterCreateChildren(*desc, ld, ctx);
	}
	void BeforeLoadData(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		BeforeLoadData(*desc, ld, ctx);
	}
	void AfterLoadData(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		AfterLoadData(*desc, ld, ctx);
	}
	virtual void BeforeCreateObject(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
	virtual void AfterCreateObject(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
	virtual void AfterCreateChild(T& t, UTLoadedData* ld, ObjectIf* child, UTLoadContext* ctx){}
	virtual void AfterCreateChildren(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
	virtual void BeforeLoadData(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
	virtual void AfterLoadData(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
};

///	型情報(UTTypeDescDb)，ハンドラ(UTLoadHandlerDb) を切り替えるハンドラ
template <class T>
class UTLoadHandlerSetDb: public UTLoadHandlerImp<T>{
public:
	UTLoadHandlerSetDb(const char* t):UTLoadHandlerImp<T>(t){}
	UTRef<UTTypeDescDb> typeDb;
	UTRef<UTLoadHandlerDb> handlerDb;
	virtual void BeforeLoadData(T& t, UTLoadedData* ld, UTLoadContext* ctx){
		if (typeDb) ctx->typeDbs.Push(typeDb);
		if (handlerDb) ctx->handlerDbs.Push(handlerDb);
	}
	virtual void AfterLoadData(T& t, UTLoadedData* ld, UTLoadContext* ctx){
		if (handlerDb) ctx->handlerDbs.Pop();
		if (typeDb) ctx->typeDbs.Pop();
	}
};

/**	
	
*/
class UTLoadHandlerDb:public std::multiset< UTRef<UTLoadHandler>, UTLoadHandler::Less >, public UTRefCount{
	std::set<UTString, UTStringLess> addedGroups;
public:
	UTString group;
	UTLoadHandlerDb& operator += (const UTLoadHandlerDb& b){
		if (b.group != group && addedGroups.find(b.group) == addedGroups.end()){
			insert(b.begin(), b.end());
			addedGroups.insert(b.group);
		}
		return *this;
	}
	void Print(std::ostream& os) const {
		os << group << ":" << std::endl;
		for(const_iterator it = begin(); it != end(); ++it){
			os << "  " << (*it)->type << std::endl;
		}
	}
};
inline bool operator < (const UTLoadHandlerDb& a, const UTLoadHandlerDb& b){
	return a.group.compare(b.group) < 0;
}

class UTLoadHandlerDbPool: public std::set< UTRef<UTLoadHandlerDb>, UTContentsLess< UTRef<UTLoadHandlerDb> > >, public UTRefCount{
	static UTRef<UTLoadHandlerDbPool> pool;
	static UTLoadHandlerDbPool* SPR_CDECL GetPool();
public:
	static UTLoadHandlerDb* SPR_CDECL Get(const char* gp);
};

}

#endif	// UTLOADHANDLER_H

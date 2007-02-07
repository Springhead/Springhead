/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
	virtual void BeforeCreateObject(UTLoadedData* d, UTLoadContext* fc){}
	virtual void AfterCreateObject(UTLoadedData* d, UTLoadContext* fc){}
	virtual void AfterCreateChildren(UTLoadedData* d, UTLoadContext* fc){}
	virtual void BeforeLoadData(UTLoadedData* d, UTLoadContext* fc){}
	virtual void AfterLoadData(UTLoadedData* d, UTLoadContext* fc){}

	virtual void Save(UTLoadContext* fc){};
	struct Less{
		bool operator()(const UTLoadHandler* h1, const UTLoadHandler* h2) const{
			return h1->type.compare(h2->type) < 0;
		}
	};
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
		for(int i=ctx->objects.size()-1; i>=0 && !s; --i){
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
	virtual void AfterCreateChildren(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
	virtual void BeforeLoadData(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
	virtual void AfterLoadData(T& t, UTLoadedData* ld, UTLoadContext* ctx){}
};

/**	
	
*/
class UTLoadHandlers:public std::multiset< UTRef<UTLoadHandler>, UTLoadHandler::Less >, public UTRefCount{
public:
	UTString group;
	UTLoadHandlers& operator += (const UTLoadHandlers& b){
		insert(b.begin(), b.end());
		return *this;
	}
};
inline bool operator < (const UTLoadHandlers& a, const UTLoadHandlers& b){
	return a.group < b.group;
}

class UTLoadHandlerDb: public std::set< UTRef<UTLoadHandlers>, UTContentsLess< UTRef<UTLoadHandlers> > >, public UTRefCount{
	static UTRef<UTLoadHandlerDb> handlerDb;
public:
	static UTLoadHandlers* SPR_CDECL GetHandlers(const char* gp);
	virtual UTLoadHandlers* GetHandlersImp(const char* gp);
};

}

#endif	// UTLOADHANDLER_H

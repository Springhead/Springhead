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
	virtual void Load(UTLoadedData* d, UTLoadContext* fc){};
	virtual void Loaded(UTLoadedData* d, UTLoadContext* fc){};
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
	void Load(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		Load(*desc, ctx);
	}
	void Loaded(UTLoadedData* ld, UTLoadContext* ctx){
		T* desc = (T*)ld->data;
		Loaded(*desc, ctx);
	}
	virtual void Load(T& t, UTLoadContext* ctx)=0;
	virtual void Loaded(T& t, UTLoadContext* ctx){}
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

#ifndef FINODEHANDLER_H
#define FINODEHANDLER_H

#include <Foundation/Object.h>
#include <Foundation/UTTypeDesc.h>
#include <FileIO/FILoadContext.h>
#include <string>

namespace Spr{;

#define REGISTER_NODE_HANDLER(x)	\
	FINodeHandlers::theNodeHandlers->insert(DBG_NEW x);

class FILoadContext;

/**	自動ローダ・セーバで対応できないノードのロード・セーブをするクラス．
*/
class FINodeHandler:public UTRefCount{
public:
	UTString type;
	virtual void Load(FILoadContext* ctx){};
	virtual void Loaded(FILoadContext* ctx){};
	virtual void Save(FILoadContext* ctx){};
};
struct FINodeHandlerLess{
	bool operator()(const FINodeHandler* h1, const FINodeHandler* h2) const{
		return h1->type.compare(h2->type) < 0;
	}
};
///	FINodeHandlerの実装テンプレート
template <class T>
class FINodeHandlerImp: public FINodeHandler{
public:
	typedef T Desc;
	FINodeHandlerImp(const char* t){
		type = t;
	}
	template <class S>
	void Get(S*& s, FILoadContext* ctx){
		s = NULL;
		for(int i=ctx->objects.size()-1; i>=0 && !s; --i){
			s = DCAST(S, ctx->objects[i]);
		}
		assert(s);
	}
	void Load(FILoadContext* ctx){
		T* desc = (T*)ctx->datas.Top()->data;
		Load(*desc, ctx);
	}
	void Loaded(FILoadContext* ctx){
		T* desc = (T*)ctx->datas.Top()->data;
		Loaded(*desc, ctx);
	}
	virtual void Load(T& t, FILoadContext* ctx)=0;
	virtual void Loaded(T& t, FILoadContext* ctx){}
};

/**	
	
*/
class FINodeHandlers:public std::multiset< UTRef<FINodeHandler>, FINodeHandlerLess >, public UTRefCount{
public:
	static UTRef<FINodeHandlers> theNodeHandlers;	
};

void RegisterNodeHandlers();

}

#endif	// FINODEHANDLER_H

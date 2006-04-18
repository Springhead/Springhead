#ifndef FINODEHANDLER_H
#define FINODEHANDLER_H

#include <Foundation/Object.h>
#include <FileIO/FITypeDesc.h>
#include <string>

namespace Spr{;

class FILoadContext;

/**	自動ローダ・セーバで対応できないノードのロード・セーブをするクラス．
*/
class FINodeHandler:public UTRefCount{
public:
	UTString type;
	virtual void Load(FILoadContext* ctx){};
	virtual void Save(FILoadContext* ctx){};
};
struct FINodeHandlerLess{
	bool operator()(const FINodeHandler* h1, const FINodeHandler* h2) const{
		return h1->type.compare(h2->type) < 0;
	}
};

/**	
	
*/
class FINodeHandlers:public std::multiset< UTRef<FINodeHandler>, FINodeHandlerLess >, public UTRefCount{
public:
	static UTRef<FINodeHandlers> theNodeHandlers;	
};

void RegisterNodeHandler();

}

#endif	// FINODEHANDLER_H

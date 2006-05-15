#ifndef SPR_CRSimpleIf_H
#define SPR_CRSimpleIf_H
#include <Foundation/SprObject.h>

//@{
namespace Spr{;
struct CRSimpleState{
};

struct CRSimpleDesc : CRSimpleState{
	CRSimpleDesc(){}
};

struct CRSimpleIf : public SceneObjectIf{
	IF_DEF(CRSimple);
	virtual void Hoge() =0;
	virtual void Step() =0;
};

//@}

}	//	namespace Spr
#endif

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
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

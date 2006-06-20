/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRSIMPLE_H
#define CRSIMPLE_H

#include <SprCreature.h>
#include <Foundation/Object.h>

namespace Spr{;
class CRSimple : public InheritSceneObject<CRSimpleIf, SceneObject>, public CRSimpleDesc{
	OBJECT_DEF(CRSimple);
	CRSimple(const CRSimpleDesc& desc=CRSimpleDesc(), SceneIf* s=NULL){}
	void Hoge();
	void Step();
};
}	//	namespace Spr

#endif

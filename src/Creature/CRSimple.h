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
#include <Physics/PHEngine.h>

namespace Spr{;

class CRSimple : public InheritPHEngine<CRSimpleIf, PHEngine>, public CRSimpleDesc{
public:
	OBJECT_DEF(CRSimple);
	ACCESS_DESC(CRSimple);

	CRSimpleDesc desc;
	PHSolidIf*   solid;
	CRSimple(const CRSimpleDesc& d=CRSimpleDesc());

	int GetPriority() const { return SGBP_CREATURE; }
	void Step();
};

}	//	namespace Spr

#endif

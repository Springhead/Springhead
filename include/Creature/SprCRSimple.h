/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRSimpleIf_H
#define SPR_CRSimpleIf_H
#include <SprPhysics.h>

namespace Spr{;

//@{

struct CRSimpleDesc{
	std::string solidname;
	int         torque;
};

struct CRSimpleIf : public PHEngineIf{
	IF_DEF(CRSimple);
	virtual void Step() =0;
};

//@}

}	//	namespace Spr
#endif

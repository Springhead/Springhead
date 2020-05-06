/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHINovint.h
 *	@brief Novintのインタフェース
*/

#ifndef SPR_HINovint_H
#define SPR_HINovint_H

#include <HumanInterface/SprHIBase.h>

namespace Spr{;

/**	\addtogroup	gpHumanInterface	*/
//@{

/**	@brief	NovintFalconのインタフェース	*/
struct HINovintFalconIf: public HIHapticIf{
	SPR_VIFDEF(HINovintFalcon);
};
struct HINovintFalconDesc{
	SPR_DESCDEF(HINovintFalcon);
};


//@}
}
#endif

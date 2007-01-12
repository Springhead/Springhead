/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprDRDevice.h
 *	@brief ヒューマンインタフェースを作るための材料になるデバイスのクラス．たとえば，D/A,A/D,PIOなど．
*/

#ifndef SPR_HIMouse6D_H
#define SPR_HIMouse6D_H
#include <SprFoundation.h>

namespace Spr{;
/**	\addtogroup	gpHumanInterface	*/
//@{

struct DVKeyMouseIf;
///	実デバイス．インタフェースカードとか，USBデバイスとか，
struct HIMouse6DIf: public HIPoseIf{
	IF_DEF(HIMouse6D);
	virtual DVKeyMouseIf* GetKeyMouse()=0;
};


//@}
}
#endif

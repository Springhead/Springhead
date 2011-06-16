/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_HI_MOUSE6D_H
#define SPR_HI_MOUSE6D_H

#include <HumanInterface/SprHIDevice.h>
#include <HumanInterface/SprHIKeyMouse.h>

namespace Spr{;
/**	\addtogroup	gpHumanInterface	*/
//@{

/**
	マウスとキーボードによるカメラ姿勢などの6自由度入力
 **/
struct HIMouse6DIf: public HIPoseIf{
	SPR_VIFDEF(HIMouse6D);

	DVKeyMouseIf*	GetKeyMouse();
	void			SetKeyMouse(DVKeyMouseIf* dv);
};
struct HIMouse6DDesc{
	SPR_DESCDEF(HIMouse6D);
};


//@}
}
#endif

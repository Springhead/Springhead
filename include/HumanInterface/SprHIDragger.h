/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_HI_DRAGGER_H
#define SPR_HI_DRAGGER_H

#include <HumanInterface/SprHIDevice.h>
#include <HumanInterface/SprHIKeyMouse.h>

namespace Spr {;

/** @brief ドラッガーインタフェース
	マウスでシーン中の物体をつかんで動かすためのインタフェース．

 */

struct HIDraggerDesc{
};

struct HIDraggerCallback{

};

struct HIDraggerIf : HIBaseIf{
	SPR_VIFDEF(HIDragger);

	void			SetCallback(HIDraggerCallback* callback);

	DVKeyMouseIf*	GetKeyMouse();
	void			SetKeyMouse(DVKeyMouseIf* dv);
};

}	//	namespace Spr
#endif

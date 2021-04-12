/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FW_JOYSTICK_H
#define SPR_FW_JOYSTICK_H

#include <HumanInterface/SprHIJoyStick.h>

namespace Spr {;


/**
	GLUTジョイスティック
 **/

struct FWJoyStickGLUTIf: public HIRealDeviceIf{
	SPR_IFDEF(FWJoyStickGLUT);
};

struct FWJoyStickGLUTDesc {
	SPR_DESCDEF(FWJoyStickGLUT);
};

}	//	namespace Spr
#endif

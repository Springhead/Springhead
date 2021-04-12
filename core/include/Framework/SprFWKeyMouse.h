/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWKEYMOUSE_H
#define SPR_FWKEYMOUSE_H

#include <HumanInterface/SprHIKeyMouse.h>

namespace Spr {;

/**
	GLUTマウス
 **/
struct FWKeyMouseGLUTIf: public HIRealDeviceIf{
	SPR_IFDEF(FWKeyMouseGLUT);
};
struct FWKeyMouseGLUTDesc{
	SPR_DESCDEF(FWKeyMouseGLUT);
};

}	//	namespace Spr
#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

///	マウスとキーボードによるカメラ姿勢などの6自由度入力
struct HIMouse6DIf: public HIPoseIf{
	SPR_VIFDEF(HIMouse6D);
	DVKeyMouseIf* GetKeyMouse();
};

//@}
}
#endif

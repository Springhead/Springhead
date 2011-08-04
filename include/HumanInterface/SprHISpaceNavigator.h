/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_HI_SPACENAVIGATOR_H
#define SPR_HI_SPACENAVIGATOR_H

#include <HumanInterface/SprHIDevice.h>
#include <HumanInterface/SprHIKeyMouse.h>

namespace Spr{;
/**	\addtogroup	gpHumanInterface	*/
//@{

/**
	3Dconnexion Space Navigator
 **/
struct HISpaceNavigatorIf: public HIPoseIf{
	SPR_VIFDEF(HISpaceNavigator);

	///	現在の姿勢を設定
	bool SetPose(Posef pose);

	/// RAW Input Windowsメッセージを解釈する
	bool PreviewMessage(void *m);
};
struct HISpaceNavigatorDesc{
	SPR_DESCDEF(HISpaceNavigator);

	void* hWnd;
};


//@}
}
#endif

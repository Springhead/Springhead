/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHISpaceNavigator.h
 *	@brief 3Dconnexion SpaceNavigator 6自由度入力デバイス
*/

#ifndef SPR_HI_SPACENAVIGATOR_H
#define SPR_HI_SPACENAVIGATOR_H

#include <HumanInterface/SprHIDevice.h>
#include <HumanInterface/SprHIKeyMouse.h>

namespace Spr{;
/**	\addtogroup	gpHumanInterface	*/
//@{

/**	@brief SpaceNavigatorのインタフェース */
struct HISpaceNavigatorIf: public HIPoseIf{
	SPR_VIFDEF(HISpaceNavigator);

	///	現在の姿勢を設定
	bool SetPose(Posef pose);

	///	限界まで押し込んだ時の移動速度を設定する
	void SetMaxVelocity(float mV);

	///	限界までひねった時の回転速度を設定する
	void SetMaxAngularVelocity(float mAV);

	/// RAW Input Windowsメッセージを解釈する
	bool PreviewMessage(void *m);

	/// カーソル移動を視点変換するためのView行列をセットする
	void SetViewMatrix(Affinef view);

	/// カーソル移動を視点変換するためのPerspective行列をセットする
	void SetPersMatrix(Affinef pers);
};
struct HISpaceNavigatorDesc{
	SPR_DESCDEF(HISpaceNavigator);

	void* hWnd;
	float maxVelocity;
	float maxAngularVelocity;

	HISpaceNavigatorDesc() {
		hWnd = NULL;
		maxVelocity = 1;
		maxAngularVelocity = 1; 
	}
};


//@}
}
#endif

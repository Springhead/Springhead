/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef SPR_FWHAPTICPOINTERIF_H
#define SPR_FWHAPTICPOINTERIF_H


#include <Physics/SprPHHaptic.h>
#include <HumanInterface/SprHIBase.h>
#include <Foundation/SprObject.h>

namespace Spr{;
/** \addtogroup gpFramework */
//@{

struct FWHapticPointerIf : SceneObjectIf {
	SPR_IFDEF(FWHapticPointer);

	/** @brief PHHapticPointerIfを設定する
	*/
	void SetPHHapticPointer(PHHapticPointerIf* hpGlobal);

	/** @brief PHHapticPointerIfを取得
	*/	
	PHHapticPointerIf*	GetPHHapticPointer();

	/** @brief HumanInterfaceIfを設定する
	*/
	void SetHumanInterface(HIBaseIf* hi);

	/** @brief HumanInterfaceIfを設定する
	*/
	HIBaseIf* GetHumanInterface();

	///** @brief 計算提示力を取得する
	//*/
	//Vec3f GetHapticForce();

	///** @breif 計算提示トルクを取得する
	//*/
	//Vec3f GetHapticTorque();
};

//@}
}

#endif

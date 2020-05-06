/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_HI_JOYSTICK_H
#define SPR_HI_JOYSTICK_H

#include <HumanInterface/SprHIBase.h>

namespace Spr {;

struct DVJoyStickMask{
	enum {
		BUTTON_A = 0x01,
		BUTTON_B = 0x02,
		BUTTON_C = 0x04,
		BUTTON_D = 0x08,
	};
};

/** 仮想ジョイスティック
	・使用方法は仮想マウスと同様
 */
struct DVJoyStickCallback{
	/** @brief マウスが動いたときの処理
		@param buttonMask 押されているボタンの状態
		@param x		ジョイスティックx座標（左右）
		@param y		ジョイスティックy座標（前後）
		@param z		ジョイスティックひねりor上下
		
		zdeltaはマウスホイールに対応するデバイスを使用する場合のみ有効．
	*/
	virtual bool OnUpdate(int buttonMask, int x, int y, int z){ return false; }
};


/**
	ジョイスティック仮想デバイス
 **/
struct DVJoyStickIf: public HIVirtualDeviceIf{
	SPR_IFDEF(DVJoyStick);
	
	///	ハンドラの追加
	void AddCallback(DVJoyStickCallback* cb);
	/// ハンドラの削除
	void RemoveCallback(DVJoyStickCallback* cb);

	// ポーリング周期の設定
	void SetPollInterval(int ms);
	// ポーリング周期の取得
	int  GetPollInterval();

};

/**
	GLUTジョイスティック
 **/
struct DRJoyStickGLUTIf: public HIRealDeviceIf{
	SPR_IFDEF(DRJoyStickGLUT);
};

}	//	namespace Spr
#endif

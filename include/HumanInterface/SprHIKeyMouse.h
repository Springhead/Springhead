/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_HIKEYMOUSE_H
#define SPR_HIKEYMOUSE_H

#include <HumanInterface/SprHIBase.h>
namespace Spr {;

struct DVKeyMouseHandler{
	/// 押されているキー，ボタンの状態を示すフラグ
	enum ButtonState{
		NONE,
		LBUTTON = 0x01,
		RBUTTON = 0x02,
		SHIFT   = 0x04,
		CONTROL = 0x08,
		MBUTTON = 0x10,
	};
	/** マウスが動いたときの処理
		@param keyState 押されているキー，ボタンの状態
	*/
	bool OnMouseMove(int keyState, int x, int y, int zdelta);
	///
	bool OnClick(int keyState, int x, int y);
	///
	bool OnDoubleClick(int keyState, int x, int y);
	///	キー処理
	bool OnKey(bool isDown, int vkey, int keyState, int x, int y);
};


///	DVKeyMouse
struct DVKeyMouseIf:public HIVirtualDeviceIf{
	///	マウスの位置を返すための構造体
	struct DVMousePosition{
		int x;
		int y;
		unsigned time;
	};
	///	キーの状態
	enum DVKeyState{
		NONE	= 0,
		PRESS	= 1,
		TOGGLE_ON = 2,
		BOTH	= PRESS|TOGGLE_ON,
	};
	SPR_IFDEF(DVKeyMouse);
	///	デバイスの名前
	const char* Name() const ;
	///	ハンドラの設定
	void SetHandler(DVKeyMouseHandler* h);
	///	マウスボタン・キーボード状態取得
	int GetKeyState(int key);
	///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
	DVKeyMouseIf::DVMousePosition GetMousePosition(int count=0);
};

///	DRKeyMouseWin32
struct DRKeyMouseWin32If:public HIRealDeviceIf{
	SPR_IFDEF(DRKeyMouseWin32);
};

///	DRKeyMouseGLUT
struct DRKeyMouseGLUTIf:public HIRealDeviceIf{
	SPR_IFDEF(DRKeyMouseGLUT);
};

}	//	namespace Spr
#endif

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

/** 仮想キーボード・マウス
	・キー・マウスイベントを処理するクラスはDVKeyMouseCallbackを継承しイベントハンドラを実装する
	・その上でDVKeyMouseBaseIf::AddCallbackで自身を登録する
	・ハンドラは複数登録可能．
 */

/// 押されているキー，ボタンの状態を示すフラグ
struct DVButtonMask{
	enum{
		LBUTTON		= 0x01,
		RBUTTON		= 0x02,
		MBUTTON		= 0x04,
		SHIFT		= 0x08,
		CONTROL		= 0x10,
		ALT			= 0x20,
	};
};
struct DVButtonSt{
	enum{
		DOWN = 0,
		UP	 = 1,
	};
};
struct DVKeySt{
	enum{
		PRESSED		= 0x01,		///< 押されている
		TOGGLE_ON	= 0x02,		///< トグルされている(caps lockなど)
	};
};
/** ascii以外の特殊キーコード．値はGLUTの特殊キーコードに256を加算したもの．
 **/
struct DVKeyCode{
	enum{
		BACKSPACE				= 8,
		TAB						= 9,
		CR						= 13,
		ENTER					= CR,
		ESC						= 27,
		DEL						= 127,
		OFFSET					= 256,
		F1, 
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		LEFT					= 100 + OFFSET,
		UP,
		RIGHT,
		DOWN,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		INSERT,
	};
};

struct DVKeyMouseCallback{
	/** @brief マウスクリック時の処理
		@param button	DVButtonMask列挙子の組み合わせ
		@param state	DVButtonSt列挙子の値
		@param x		カーソルのx座標
		@param y		カーソルのy座標
	 */
	virtual bool OnMouse(int button, int state, int x, int y){ return false; }
	/** @brief ダブルクリック時の処理
		@param button	ButtonMask列挙子の組み合わせ
		@param x		カーソルのx座標
		@param y		カーソルのy座標
	 */
	virtual bool OnDoubleClick(int button, int x, int y){ return false; }
	/** @brief マウスが動いたときの処理
		@param button	ButtonMask列挙子の組み合わせ
		@param x		カーソルのx座標
		@param y		カーソルのy座標
		@param zdelta	マウスホイールの変化量
		
		zdeltaはマウスホイールに対応するデバイスを使用する場合のみ有効．
	*/
	virtual bool OnMouseMove(int button, int x, int y, int zdelta){ return false; }
	/** @brief キー入力処理
		@param state	DVKeySt列挙子の値
		@param key		asciiコードかDVKeyCode列挙子の値
		@param x		カーソルのx座標
		@param y		カーソルのy座標
	 */
	virtual bool OnKey(int state, int key, int x, int y){ return false; }
};

/**
	キーボード・マウス仮想デバイス
 **/
struct DVKeyMouseIf: public HIVirtualDeviceIf{
	SPR_IFDEF(DVKeyMouse);
	
	///	ハンドラの追加
	void AddCallback(DVKeyMouseCallback* cb);
	/// ハンドラの削除
	void RemoveCallback(DVKeyMouseCallback* cb);

	/**	@brief マウスボタン・キーボード状態取得
		@param	key		DVKeyCodeの値
		@return			DVKeyStの値
		win32マウス限定．
	 */
	int GetKeyState(int key);

	/**	@brief 現在および過去のマウス位置を取得
		@param x		x座標
		@param y		y座標
		@param time		タイムスタンプ
		@param count	いくつ前の情報を取得するか
		countは[0,63]の範囲が有効．0ならば最新，1以上は過去の履歴を取得する．
		Win32とGLUTで挙動が異なるので注意．GLUTではtime値は無効．
	 */
	void GetMousePosition(int& x, int& y, int& time, int count=0);
};

/**
	Win32マウス
 **/
struct DRKeyMouseWin32If: public HIRealDeviceIf{
	SPR_IFDEF(DRKeyMouseWin32);
	//	Windowsメッセージを渡し、マウスとキーボードのイベントの処理をさせる。
	bool PreviewMessage(void* m);
};
struct DRKeyMouseWin32Desc{
	SPR_DESCDEF(DRKeyMouseWin32);
};

}	//	namespace Spr
#endif

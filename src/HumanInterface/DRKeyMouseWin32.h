/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DRKEYMOUSEWIN32_H
#define DRKEYMOUSEWIN32_H

#include <HumanInterface/HIDevice.h>

namespace Spr {;

///	DVKeyMouse
class DRKeyMouseWin32: public HIRealDevice{
public:
	SPR_OBJECTDEF(DRKeyMouseWin32);
	///	仮想デバイス(KeyMouse)
	class DV: public DVKeyMouse{
	public:
		DV(DRKeyMouseWin32* dr, int ch):DVKeyMouse(dr, ch){}
		DRKeyMouseWin32* GetRealDevice(){ return realDevice->Cast(); }

		///	マウスボタン・キーボード状態取得
		virtual int GetKeyState(int key){ return GetRealDevice()->GetKeyState(key); }
		///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
		virtual void GetMousePosition(int& x, int& y, int& time, int count=0){
			return GetRealDevice()->GetMousePosition(x, y, time, count);
		}
	};
public:
	DRKeyMouseWin32(const DRKeyMouseWin32Desc& desc = DRKeyMouseWin32Desc());
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	virtual HIVirtualDeviceIf*	Rent(const IfInfo* ii, const char* name, int portNo);

	/// windows仮想キーコードからSpringhead共通キーコードへの変換
	int	ConvertKeyCode(int key, bool spr_to_win32);
	
	int		GetKeyState(int key);
	void	GetMousePosition(int& x, int& y, int& time, int count);

	///	Win32メッセージを見て，マウスボタンのイベント処理をする．
	bool PreviewMessage(void* m);
	void UpdateMousePos(int x, int y);
	
	struct MousePos{
		int x, y;
		unsigned time;
	};
	enum { NHISTORY = 10 };
	MousePos history[NHISTORY];
};

}	//	namespace Spr
#endif

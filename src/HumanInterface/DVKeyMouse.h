/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DVKEYMOUSE_H
#define DVKEYMOUSE_H

#include <HumanInterface/HIVirtualDevice.h>
namespace Spr {;

///	DVKeyMouse
class SPR_DLL DVKeyMouse:public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVKeyMouse);
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "KeyMouse"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
	///	ハンドラの設定
	virtual void SetHandler(DVKeyMouseHandler* h)=0;
	///	マウスボタン・キーボード状態取得
	virtual int GetKeyState(int key)=0;
	///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
	virtual DVKeyMouseIf::DVMousePosition GetMousePosition(int count=0){
		return DVKeyMouseIf::DVMousePosition();
	}
};

}	//	namespace Spr
#endif

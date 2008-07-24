/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DRKeyMouseGLUT_H
#define DRKeyMouseGLUT_H

#include <HumanInterface/HIVirtualDevice.h>
#include <HumanInterface/DVKeyMouse.h>
#include <GL/glut.h>

#ifndef GLUTCALLBACK
#define GLUTCALLBACK
#endif

namespace Spr {;

///	DVKeyMouse
class DRKeyMouseGLUT:public HIRealDevice{
public:
	SPR_OBJECTDEF(DRKeyMouseGLUT);
	///	仮想デバイス(KeyMouse)
	class KeyMouse:public DVKeyMouse{
	protected:
		char name[100];
		DRKeyMouseGLUT* glut;
	public:
		KeyMouse(DRKeyMouseGLUT* w);
		///	デバイスの名前
		virtual const char* Name() const{ return name; }
		///	ハンドラの設定
		virtual void SetHandler(DVKeyMouseHandler* h);
		///	マウスボタン・キーボード状態取得
		virtual int GetKeyState(int key);
		///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
		virtual DVKeyMouseIf::DVMousePosition GetMousePosition(int count=0);
		///
		virtual HIRealDeviceIf* RealDevice(){ return glut->Cast(); }
	};
protected:
	char name[100];
	DVKeyMouseHandler* handler;
	static DRKeyMouseGLUT* This;
public:
	DRKeyMouseGLUT();
	///	初期化
	virtual bool Init();
	///	仮想デバイスの登録
	void Register(HISdkIf* intf);
	///	デバイスの名前
	virtual const char* Name() const { return name; }
	///	デバイスの種類
	static const char* TypeS() { return "KeyMouse"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
	///	ハンドラの設定
	virtual void SetHandler(DVKeyMouseHandler* h){
		handler = h;
	}
	static void GLUTCALLBACK OnMouse(int button, int state, int x, int y);
	static void GLUTCALLBACK OnMotion(int x, int y);
	static void GLUTCALLBACK OnPassiveMotion(int x, int y);
};

}	//	namespace Spr
#endif

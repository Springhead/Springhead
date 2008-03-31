/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPPGLUT_H
#define SPR_FWAPPGLUT_H

#include <Framework/SprFWAppGL.h>

namespace Spr{;

class FWWinGLUT : public FWWin{
	UTString title;
	bool	fullScreen;
public:
	FWWinGLUT(int wid, GRRenderIf* r):FWWin(wid, r), fullScreen(false){}

	virtual void Position(int left, int top);
	virtual void Reshape(int width, int height);
	virtual void SetTitle(UTString t);
	virtual UTString GetTitle(){ return title; }
	virtual void FullScreen();
	virtual bool IsFullScreen(){ return fullScreen; }
};

/** @brief GLUTを用いるアプリケーションクラス
 */
class FWAppGLUT : public FWAppGL{
public:

protected:
	static FWAppGLUT* instance;
	static void SPR_CDECL GlutDisplayFunc();
	static void SPR_CDECL GlutReshapeFunc(int w, int h);
	static void SPR_CDECL GlutTimerFunc(int id);
	static void SPR_CDECL GlutIdleFunc();
	static void SPR_CDECL GlutKeyboardFunc(unsigned char key, int x, int y);
	static void SPR_CDECL GlutMouseFunc(int button, int state, int x, int y);
	static void SPR_CDECL GlutMotionFunc(int x, int y);
	static void SPR_CDECL GlutJoystickFunc(unsigned int buttonMask, int x, int y, int z);
	static void SPR_CDECL AtExit();
public:
	~FWAppGLUT();
	
	///	描画コールバック
	virtual void Display();

	///	GLUTの初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc, char* argv[]);
	///	ウィンドウを作成し、ウィンドウ IDを返す
	virtual FWWin* CreateWin(const FWWinDesc& d=FWWinDesc());
	///	ウィンドウを破棄する
	virtual void DestroyWin(FWWin* w);
	///	カレントウィンドウを設定する。設定後のIDを返す。
	virtual void SetCurrentWin(FWWin* w);
	///	カレントウィンドウを返す。
	virtual FWWin* GetCurrentWin();

	///	フレームワークをスタートする。
	virtual void Start();
};


}

#endif

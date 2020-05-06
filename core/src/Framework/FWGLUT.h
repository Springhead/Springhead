/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWGLUT_H
#define FWGLUT_H

#include <SprDefs.h>
#include <Foundation/UTTimer.h>
#include <Framework/FWGraphicsHandler.h>
#include <Framework/FWWin.h>

namespace Spr{;

class DRKeyMouseGLUT;
class DRJoyStickGLUT;

/**
	FWGraphicsHandlerのGLUTによる実装
 */
class FWGLUT : public FWGraphicsHandler, public UTTimerProvider{
public:
	SPR_OBJECTDEF_NOIF(FWGLUT);
protected:
	bool	idleFuncFlag;	///< IdleFuncの呼び出しに関するFlag	
	
	DRKeyMouseGLUT*	keyMouse;
	DRJoyStickGLUT*	joyStick;

	static FWGLUT*	GetInstance();

	/** コールバック関数 */
	static void SPR_CDECL GlutDisplayFunc();
	static void SPR_CDECL GlutReshapeFunc(int w, int h);
	static void SPR_CDECL GlutTimerFunc(int id);
	static void SPR_CDECL GlutIdleFunc();
	static void SPR_CDECL GlutMouseFunc(int button, int state, int x, int y);
	static void SPR_CDECL GlutMotionFunc(int x, int y);
	static void SPR_CDECL GlutPassiveMotionFunc(int x, int y);
	static void SPR_CDECL GlutKeyFunc(unsigned char ch, int x, int y);
	static void SPR_CDECL GlutSpecialKeyFunc(int ch, int x, int y);
#ifdef USE_FREEGLUT
	static void SPR_CDECL GlutMouseWheelFunc(int, int, int, int);
#endif
	static void SPR_CDECL AtExit();

	virtual void RegisterCallbacks();
public:	
	///	GLUTの初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc, char** argv);

	/// UTTimerProviderの実装
	virtual bool StartTimer(UTTimer* timer);
	virtual bool StopTimer(UTTimer* timer);
	
	virtual void StartMainLoop();
	virtual void EnableIdleFunc(bool on = true);
	virtual void EnterGameMode();
	virtual void LeaveGameMode();

	/** ウィンドウ */
	///	ウィンドウを作成し、ウィンドウ IDを返す
	virtual FWWinIf* CreateWin(const FWWinDesc& d, FWWinIf* parent);
	///	ウィンドウを破棄する
	virtual void DestroyWin(FWWinIf* w);
	///	カレントウィンドウを設定する
	virtual void SetCurrentWin(FWWinIf* w);
	///	カレントウィンドウを返す。
	virtual int GetCurrentWin();	
	///カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	virtual void PostRedisplay();
	/// Shift,Ctrl,Altのステートを返す
	virtual int GetModifiers();
	/// ウィンドウ属性操作
	virtual Vec2i	GetPosition		(FWWinBase* win);
	virtual void	SetPosition		(FWWinBase* win, int left, int top);
	virtual Vec2i	GetSize			(FWWinBase* win);
	virtual void	SetSize			(FWWinBase* win, int width, int height);
	virtual void	SetTitle		(FWWinBase* win, UTString title);
	virtual void	SetFullScreen	(FWWin* win);

	FWGLUT();
	~FWGLUT();

};

}





#endif
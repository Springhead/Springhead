#ifndef FW_GRAPHICSADAPTEE_H
#define FW_GRAPHICSADAPTEE_H

#include <Framework/SprFWGraphicsAdaptee.h>
#include <Foundation/Object.h>

namespace Spr{;

class FWApp;

class FWGraphicsAdaptee : public Object, public FWGraphicsAdapteeDesc{
public:
	SPR_OBJECTDEF_ABST(FWGraphicsAdaptee);
	FWApp*			fwApp;			///< アプリケーションクラスへの参照
	GRDeviceIf*		grDevice;		///< グラフィクスデバイスへの参照
public:
	///	初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc = 0, char** argv = NULL){}
	
	/// mainloopを呼び，タイマーをスタートする
	virtual void StartMainLoop()=0;

	/// Idle callbackの有効化/無効化
	virtual void EnableIdleFunc(bool on = true)=0;

	/// フルスクリーンモードへの切り替え．API名は暫定的にGLUTに準拠
	virtual void EnterGameMode()=0;
	virtual void LeaveGameMode()=0;

	/** ウィンドウ */
	///	ウィンドウを作成し、ウィンドウ IDを返す
	virtual FWWinIf* CreateWin(const FWWinDesc& d){return NULL;};
	///	ウィンドウを破棄する
	virtual void DestroyWin(FWWinIf* w){};
	///	カレントウィンドウを設定する
	virtual void SetCurrentWin(FWWinIf* w){};
	///	カレントウィンドウを返す。
	virtual FWWinIf* GetCurrentWin(){return NULL;};
	///	カレントウィンドウを返す。
	virtual int GetWinFromId(){return NULL;};
	/// カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	virtual void PostRedisplay(){};
	/// Shift,Ctrl,Altのステートを返す
	virtual int GetModifiers(){return NULL;};

	GRDeviceIf* GetGRDevice(){ return grDevice; }

	FWGraphicsAdaptee(FWApp* a){ fwApp = a; }
};

}

#endif
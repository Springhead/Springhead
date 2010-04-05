#ifndef FW_GRAPHICSADAPTEE_H
#define FW_GRAPHICSADAPTEE_H

#include <Springhead.h>

using namespace std;

namespace Spr{;

class FWApp;
class FWWin;
class FWWinDesc;
typedef void SPR_CDECL FWTimerFunc(int id);

class FWGraphicsAdapteeDesc{
public:
	FWGraphicsAdapteeDesc(){};
};

class FWGraphicsAdaptee :public FWGraphicsAdapteeDesc, public UTRefCount{
protected:
	FWApp*	 fwApp;
public:	
	FWGraphicsAdaptee(){};
	void SetAdapter(FWApp* a){ fwApp = a; }	// FWAppの設定
	FWApp* GetFWApp(){ return fwApp; }		// FWAppの取得
	
	///	初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc = 0, char* argv[] = NULL){}
	
	/** タイマ */
	/// mainloopを呼び，タイマーをスタートする
	virtual void StartMainLoop()=0;

	/// Idle callbackの有効化/無効化
	virtual void EnableIdleFunc(bool on = true)=0;

	/// フルスクリーンモードへの切り替え．API名は暫定的にGLUTに準拠
	virtual void EnterGameMode()=0;
	virtual void LeaveGameMode()=0;

	/** ウィンドウ */
	///	ウィンドウを作成し、ウィンドウ IDを返す
	virtual FWWin* CreateWin(const FWWinDesc& d){return NULL;};
	///	ウィンドウを破棄する
	virtual void DestroyWin(FWWin* w){};
	///	カレントウィンドウを設定する
	virtual void SetCurrentWin(FWWin* w){};
	///	カレントウィンドウを返す。
	virtual FWWin* GetCurrentWin(){return NULL;};
	///	カレントウィンドウを返す。
	virtual int GetWinFromId(){return NULL;};
	/// カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	virtual void PostRedisplay(){};
	/// Shift,Ctrl,Altのステートを返す
	virtual int GetModifiers(){return NULL;};

};

}

#endif
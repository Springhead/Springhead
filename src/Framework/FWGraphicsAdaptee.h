#ifndef FW_GRAPHICSADAPTEE_H
#define FW_GRAPHICSADAPTEE_H

#include<sstream>
#include<string>
#include<vector>
#include<Springhead.h>
#include<Framework/FWAppInfo.h>
#include <GL/glut.h>

using namespace std;

namespace Spr{;

class FWApp;
class FWWin;
class FWWinDesc;
typedef void SPR_CDECL GTimerFunc(int id);

class FWGraphicsAdapteeDesc{
public:
	FWGraphicsAdapteeDesc();
};

class FWGraphicsAdaptee :public FWGraphicsAdapteeDesc, public UTRefCount{
protected:
	FWApp*	 fwApp;
public:	
	FWGraphicsAdaptee();
	void SetAdapter(FWApp* a){ fwApp = a; };	// FWAppの設定
	FWApp* GetFWApp(){ return fwApp; };			// FWAppの取得
	
	///	初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc = 0, char* argv[] = NULL){};
	
	/** タイマ */
	/// タイマーを設定する
	virtual void Timer(int i){};
	/// タイマーを作成する
	virtual void AddTimer(){};
	/// 最後に作成したタイマーの番号を返す
	virtual int GetTimerNo(){return NULL;};
	/// i番目のタイマーにTimerfuncを設定する
	virtual void SetTimerFunc(GTimerFunc* f ,int i){};
	/// Timerfuncで呼ぶことにより，ループを作成する
	virtual void Loop(int i,double timeSteo){};
	/// mainloopを呼び，タイマーをスタートする
	virtual void TimerStart(){};

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
	virtual int Modifiers(){return NULL;};

};

}

#endif
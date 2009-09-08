/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <Springhead.h>
#include <Framework/Framework.h>
#include <Framework/FWInteractAdaptee.h>
#include <Framework/FWGraphicsAdaptee.h>
#include <map>
#include "Foundation/UTMMTimer.h"
#include <vector>


namespace Spr{;

/** @brief アプリケーションクラス
	Springheadのクラスは基本的に継承せずに使用するように設計されているが，
	FWAppおよびその派生クラスは例外であり，ユーザはFWAppあるいはその派生クラスを継承し，
	仮想関数をオーバライドすることによって独自機能を実装する．
 */
class FWApp : public UTRefCount{

private:
	UTRef<FWSdkIf>						fwSdk;
	UTRef<HISdkIf>						hiSdk;	
public:
	virtual ~FWApp();
	Wins								wins;		//ウィンドウ情報
	UTRef<FWVFuncBridge>				vfBridge;

protected:
	MouseInfo							mouseInfo;	//マウス情報
	CameraInfo							cameraInfo;	//カメラ情報
	std::map<FWSceneIf*, DragInfo>		dragInfo;	//剛体ドラッグ情報

public:
// 派生クラスで定義する必要がある仮想関数 -----------------------------

	/** @brief 初期化
		FWAppオブジェクトの初期化を行う．最初に必ず呼ぶ．
	 */
	virtual void Init(int argc = 0, char* argv[] = NULL)=0;
	/** @brief タイマー処理
		繰り返し実行を行う．
	 */
	virtual void Timer(){}
	/** @brief シーンの描画
		シーンが表示されるときに呼ばれる．
		描画処理をカスタマイズしたい場合にオーバライドする．
		デフォルトではFWSdk::Drawが呼ばれる．
	 */
	virtual void Display()=0;

// 派生クラスで定義することのできる仮想関数 -----------------------------

	/** @brief アイドル処理
		イベントが何もない場合にバックグラウンド処理を行う．
	 */
	virtual void IdleFunc(){};
	/** @brief メインループの実行
		glutの場合，glutIdleFunc,glutmainLoopの実行
	 */
	virtual void TimerStart();
	/** @brief 描画領域のサイズ変更
		@param w 描画領域の横幅
		@param h 描画領域の縦幅
		ユーザによってウィンドウサイズが変更されたときなどに呼ばれる．
		デフォルトではFWSdk::Reshapeが呼ばれる．
	 */
	virtual void Reshape(int w, int h);
	/** @brief キーボードイベントのハンドラ
	 */
	virtual void Keyboard(int key, int x, int y){}
	/** @brief キーボードイベントのハンドラ
		Keyboard関数のはじめに呼ぶ
	 */
	void BeginKeyboard();
	/** @brief キーボードイベントのハンドラ
		Keyboard関数の終わりに呼ぶ
	 */
	void EndKeyboard();

	/** @brief マウスイベントのハンドラ
	 */
	virtual void MouseButton(int button, int state, int x, int y);

	/** @brief マウスイベントのハンドラ
		デフォルトでは左ドラッグで視点変更，右ドラッグでズームイン/アウト
	 */
	virtual void MouseMove(int x, int y);

	/** @brif ジョイスティックのハンドラ
	*/
	virtual void Joystick(unsigned int buttonMask, int x, int y, int z){}


//　FWAppのインタフェース -----------------------------------------

	/** @brief SDKを取得する
	*/
	FWSdkIf*	GetSdk(){ return fwSdk; }

	/** @brief SDKを作成する
	*/
	void		CreateSdk();

	/** @brief windowにシーンを与える
	*/
	void		AssignScene(FWWin* win);

	/** @brief ウィンドウに対応するコンテキストを作る
		@param desc	ディスクリプタ
		ウィンドウを作成する．対応するレンダラは新しく作成され，
		シーンはアクティブなシーンが関連つけられる．
	 */
	FWWin*		CreateWin(const FWWinDesc& desc = FWWinDesc());
	
	/** @brief ウィンドウの初期化
	 */
	void		InitWindow();

	/** @brief ウィンドウの数 
	*/
	int			NWin(){ return (int)wins.size(); }

	/**	@brief ウィンドウをIDから探す
	*/
	FWWin*		GetWinFromId(int wid);

	/** @brief ウィンドウを取得する
		indexで指定されたウィンドウを取得する．
	 */
	FWWin*		GetWin(int index);

	/** @brief 現在のウィンドウを取得する
	*/
	FWWin*		GetCurrentWin();

	/** @brief ウィンドウを削除する
	 */
	virtual void DestroyWin(FWWin* win);

	/** @brief 現在のウィンドウを設定する
	 */
	virtual void SetCurrentWin(FWWin* win);

	/** @brief カメラ情報を返す
		@return camInfo
	*/
	CameraInfo	GetCameraInfo(){return cameraInfo;}

	/** @brief Ctrl, Shift, Altの状態を返す
		個々の派生クラスで実装される
	 */

	virtual int	GetModifier();

// ------------------------------------------------------------------------
	
/** 描画パート */
private:
	UTRef<FWGraphicsAdaptee>	grAdaptee;	//グラフィクスシステムのアダプティ
public:
	FWGraphicsAdaptee* GetGRAdaptee(){return grAdaptee;};

	enum grAdapteeType{
		TypeGLUT,
		TypeGLUI,
	};
	void SetGRAdaptee(grAdapteeType type);

/**コールバック関数*/
public:
	static FWApp* instance;
	void CallDisplay();
	void CallReshape(int w, int h);
	void CallIdleFunc();
	void CallKeyboard(int key, int x, int y);
	void CallMouseButton(int button, int state, int x, int y);
	void CallMouseMove(int x, int y);
	void CallJoystick(unsigned int buttonMask, int x, int y, int z);

/** FWInteraction */
///////////////////////////////////////////////////////////////////////////////////
public:
protected:
	FWInteractSceneIf*					curINScene;
	FWInteractScenes					inScenes;
	UTRef<FWInteractAdaptee>			interactAdaptee;
	std::vector<UTRef < HIBaseIf > >	humanInterfaces;
public:
	void							CreateHISdk();
	HISdkIf*						GetHISdk();
	void							AddHI(HIBaseIf* hi);
	HIBaseIf*						GetHI(int i);
	FWInteractSceneIf*				CreateINScene(const FWInteractSceneDesc& desc);
	FWInteractSceneIf*				GetINScene(int i = -1);
	int								NINScenes();
	void							INClear();
///////////////////////////////////////////////////////////////////////////////////


/**マルチメディアタイマ */
protected:
	vector<UTMMTimer*> mmtimer;				/// マルチメディアタイマの宣言
public:
	UTMMTimer* CreateMMTimerFunc();
	UTMMTimer* GetMMTimerFunc(int n);
	void MTimerRelease();
	void MTimerStart();
/**タイマ　*/
protected:
	typedef UTRef<GTimer> UTRef_GTimer;
	vector<UTRef_GTimer> gTimer;
public:
	GTimer* CreateTimerFunc();
	GTimer* GetTimerFunc(int n);
};

}
#endif

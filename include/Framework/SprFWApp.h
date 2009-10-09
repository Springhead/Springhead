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
#include <Framework/SprFWInteractScene.h>
#include <Framework/SprFWGraphicsAdaptee.h>
#include <Framework/SprFWAppInfo.h>
#include <map>
#include <vector>
#include <GL/glut.h>


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
	Wins								wins;		///< ウィンドウ情報
	UTRef<FWVFuncBridge>				vfBridge;	///< 多言語(Rubyなど)へポートする際に仮想関数が適切に呼ばれるようにするためのブリッジ

protected:
	MouseInfo							mouseInfo;	///< マウス情報
	CameraInfo							cameraInfo;	///< カメラ情報
	std::map<FWSceneIf*, DragInfo>		dragInfo;	///< 剛体ドラッグ情報

public:
// 派生クラスで定義する必要がある仮想関数 -----------------------------

	/** @brief 初期化
		FWAppオブジェクトの初期化を行う．最初に必ず呼ぶ．
	 */
	virtual void Init(int argc = 0, char* argv[] = NULL)=0;

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
	virtual void IdleFunc(){}

	/** @brief タイマー処理
		繰り返し実行を行う．
	 */
	virtual void TimerFunc(int id){}
	
	/** @brief メインループの実行
		glutの場合，glutIdleFunc,glutmainLoopの実行
	 */
	virtual void StartMainLoop();

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
	void DestroyWin(FWWin* win);

	/** @brief 現在のウィンドウを設定する
	 */
	void SetCurrentWin(FWWin* win);

	/** @brief カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	 */
	void PostRedisplay();

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
	enum grAdapteeType{
		TypeGLUT,
		TypeGLUI,
	};
	/** @brief 描画の設定
		FWGraphicsAdapteeを設定する．最初に必ず呼ぶ．
	 */
	void SetGRAdaptee(grAdapteeType type);
	/** @brief 描画の設定を取得
		FWGraphicsAdapteeを取得する．　
	 */
	FWGraphicsAdaptee* GetGRAdaptee(){return grAdaptee;}; //将来的には削除したい

	/** @brief FWGraphicsAdapteeの初期化
		FWGraphicsAdapteeの初期化を行う．最初に必ず呼ぶ．
	 */
	void GRInit(int argc = 0, char* argv[] = NULL);

/** コールバック関数*/
public:
	static FWApp* instance;
	void CallDisplay();
	void CallReshape(int w, int h);
	void CallTimerFunc(int id);
	void CallIdleFunc();
	void CallKeyboard(int key, int x, int y);
	void CallMouseButton(int button, int state, int x, int y);
	void CallMouseMove(int x, int y);
	void CallJoystick(unsigned int buttonMask, int x, int y, int z);

/** FWInteraction */
///////////////////////////////////////////////////////////////////////////////////
public:
protected:
	FWInteractSceneIf*					curIAScene;
	FWInteractScenes					iaScenes;
	std::vector<UTRef < HIBaseIf > >	humanInterfaces;
public:
	void							CreateHISdk();
	HISdkIf*						GetHISdk();
	void							AddHI(HIBaseIf* hi);
	HIBaseIf*						GetHI(int i);
	FWInteractSceneIf*				CreateIAScene(const FWInteractSceneDesc& desc);
	FWInteractSceneIf*				GetIAScene(int i = -1);
	int								NIAScenes();
	void							ClearIAScenes();
///////////////////////////////////////////////////////////////////////////////////


/** タイマ　*/
protected:
	vector<UTRef<FWTimer> > fwTimers;
public:

	/** @brief タイマーを作成する
		最初に作成されたタイマのIDは0．その後は1ずつ増加する．ReleaseTimerによって既存のタイマのIDが変化することは無い．
		タイマ周期の初期値は0．周期が0の場合はアクティブなシーンのtime stepがタイマ周期となる．
		異なる周期を設定する場合はFWTimer::SetIntervalを使用する．
	 */
	int CreateTimer(FWTimer::TimerType t = FWTimer::GLUT);

	/** @brief タイマーの時間間隔の設定
	*/
	void SetInterval(int id, unsigned ms);	
	/** @brief タイマーの時間間隔の取得
	*/
	unsigned GetInterval(int id);
	/** @brief タイマーの分解能の設定
	*/
	void SetResolution(int id, unsigned r);	
	/** @brief タイマーの分解能の取得
	*/
	unsigned GetResolution(int id);
	/** @brief タイマーを解放する
	*/
	void ReleaseTimer(int id);
	/** @brief タイマーを再構成する
	*/
	void RecreateTimer(int id);
	/** @brief 全てのタイマーを解放する
	*/
	void ReleaseAllTimer();
	/** @brief 全てのタイマーを再構成する
	*/
	void CreateAllTimer();
	/** @brief 全てのタイマーを削除する
	*/
	void ClearAllTimer();

	/** @brief タイマーを取得する */
	FWTimer* GetTimer(int id);
};

}
#endif

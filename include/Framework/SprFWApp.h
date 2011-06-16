/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <HumanInterface/SprHIKeyMouse.h>
#include <Framework/SprFWWin.h>

namespace Spr{;

struct FWSdkIf;
struct FWGraphicsAdapteeIf;

/** @brief アプリケーションクラス
	Springheadのクラスは基本的に継承せずに使用するように設計されているが，
	FWAppおよびその派生クラスは例外であり，ユーザはFWAppあるいはその派生クラスを継承し，
	仮想関数をオーバライドすることによって独自機能を実装する．

	FWApp -
		1 FWSdk -
			n FWScene
			n GRRender
		1 FWGraphicsAdaptee
		n FWWindow
			1 [FWScene]
			1 [GRRender]
		1 hiSdk

 */
class FWApp : public UTRefCount, public DVKeyMouseHandler{
protected:
	UTRef<FWSdkIf>				fwSdk;		///< Framework SDK	
	
	UTRef<FWGraphicsAdapteeIf>	grAdaptee;	///< グラフィクスシステムのアダプティ
		
	// ウィンドウ
	typedef std::vector< UTRef<FWWinIf> > Wins;
	Wins		wins;
	
	/** @brief ウィンドウにシーンを与える
		@param win シーンを割り当てるウィンドウ
		winに，既存のウィンドウが割り当てられていないシーンを割り当てる．
		該当するシーンが見つからない場合，あるいはwinに既にシーンが割り当てられている場合は何もしない．
	*/
	void	AssignScene(FWWinIf* win);

public:
	FWApp();
	virtual ~FWApp();

// 派生クラスで定義する必要がある仮想関数 -----------------------------

	/** @brief 初期化
		FWAppオブジェクトの初期化を行う．最初に必ず呼ぶ．
	 */
	virtual void Init(int argc = 0, char* argv[] = NULL);

	/** @brief シーンの描画
		シーンが表示されるときに呼ばれる．
		描画処理をカスタマイズしたい場合にオーバライドする．
		デフォルトではFWSdk::Drawが呼ばれる．
	 */
	virtual void Display();

	/** @brief IdleFuncの呼び出しを有効化/無効化する
		glutの場合，glutIdleFuncに対応する．

		＊一般性を持たせ、かつ他とあわせるためにEnableIdleFuncを追加しました。
		DisableIdleFuncも残していますがobsoleteとしたいと思います。
	 */
	void DisableIdleFunc(){ EnableIdleFunc(false); }
	void EnableIdleFunc(bool on = true);
	
	/** @brief メインループの実行
		glutの場合，glutmainLoopの実行
	 */
	void StartMainLoop();

// 派生クラスで定義することのできる仮想関数 -----------------------------

	/** @brief アイドル処理
		イベントが何もない場合にバックグラウンド処理を行う．
	 */
	virtual void IdleFunc(){}

	/** @brief タイマー処理
		繰り返し実行を行う．
	 */
	virtual void TimerFunc(int id);

	/** @brief 描画領域のサイズ変更
		@param w 描画領域の横幅
		@param h 描画領域の縦幅
		ユーザによってウィンドウサイズが変更されたときなどに呼ばれる．
		デフォルトではFWSdk::Reshapeが呼ばれる．
	 */
	virtual void Reshape(int w, int h);

	/** @brief キーボードイベントのハンドラ
		@param key キーコード
		@param x カーソルｘ座標
		@param y カーソルｙ座標
		特殊キーのキーコードは使用するグラフィクスライブラリに依存する．
		
		＊GLUTの場合，glutKeyboardFuncコールバックとglutSpecialFuncコールバックで
		キーコードが重複している．そこでglutSpecialFuncコールバックに渡されたキーコードには
		256が加算されてKeyboardに渡される．
	 */
	virtual void Keyboard(int key, int x, int y){}

	/** @brief マウスイベントのハンドラ
	 */
	virtual void MouseButton(int button, int state, int x, int y);

	/** @brief マウスイベントのハンドラ
		デフォルトでは左ドラッグで視点変更，右ドラッグでズームイン/アウト
	 */
	virtual void MouseMove(int x, int y);

	/** @brief ジョイスティックのハンドラ
	*/
	virtual void Joystick(unsigned int buttonMask, int x, int y, int z){}

	/** @brief 終了処理ハンドラ
	 */
	virtual void AtExit(){}

	/** DVKeyMouseHandlerの仮想関数
		デフォルトではFWAppの従来のイベントハンドラを呼びだすのみ．
		こちらを直接オーバライドしてもよい．
	 **/
	virtual bool OnMouse(int button, int state, int x, int y){
		MouseButton(button, state, x, y);
		return true;
	}
	virtual bool OnMouseMove(int state, int x, int y, int zdelta){
		MouseMove(x, y);
		return true;
	}
	virtual bool OnDoubleClick(int button, int x, int y){
		return false;
	}
	virtual bool OnKey(int state, int key, int x, int y){
		Keyboard(key, x, y);
		return true;
	}

	//　FWAppのインタフェース -----------------------------------------

	/** @brief SDKを取得する
	*/
	FWSdkIf*	GetSdk(){ return fwSdk; }

	/** @brief SDKを作成する
	 */
	void		CreateSdk();

	/** @brief ウィンドウに対応するコンテキストを作る
		@param desc	ディスクリプタ
		ウィンドウを作成する．対応するレンダラは新しく作成され，
		既存のウィンドウが割り当てられていないシーンが関連づけられる．
	 */
	FWWinIf*	CreateWin(const FWWinDesc& desc = FWWinDesc());
	/** @brief ウィンドウの数 */
	int			NWin(){ return (int)wins.size(); }
	
	/**	@brief ウィンドウをIDから探す
		@param wid ウィンドウID
		glutの場合，widはglutGetWindowが返す識別子．
	*/
	FWWinIf*	GetWinFromId(int wid);

	/** @brief ウィンドウを取得する
		@param index 何番目のウィンドウを取得するか
		indexで指定されたウィンドウを取得する．
		DestroyWinを呼ぶとインデックスは変化するので注意が必要．
	 */
	FWWinIf*	GetWin(int index);

	/** @brief 現在のウィンドウを取得する
	*/
	FWWinIf*	GetCurrentWin();

	/** @brief ウィンドウを削除する
	 */
	void DestroyWin(FWWinIf* win);

	/** @brief 現在のウィンドウを設定する
	 */
	void SetCurrentWin(FWWinIf* win);

	/** @brief カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	 */
	void PostRedisplay();

	
	/** @brief カメラ情報を返す
		@return camInfo
	*/
	//FWUICamera*	GetCameraInfo(){return &cameraInfo;}

	/** @brief Ctrl, Shift, Altの状態を返す
		個々の派生クラスで実装される
	 */
	int	GetModifier();
	
	enum grAdapteeType{
		TypeNone,	///< アダプタ無し
		TypeGLUT,	///< GLUT
		TypeGLUI,	///< GLUI
	};
	/** @brief 描画の設定
		FWGraphicsAdapteeを設定する．最初に必ず呼ぶ．
	 */
	void SetGRAdaptee(grAdapteeType type);

	/** @brief 描画の設定を取得
		FWGraphicsAdapteeを取得する．　
	 */
	//FWGraphicsAdaptee* GetGRAdaptee(){return grAdaptee;}; //将来的には削除したい

	/** @brief FWGraphicsAdapteeの初期化
		FWGraphicsAdapteeの初期化を行う．最初に必ず呼ぶ．
	 */
	void GRInit(int argc = 0, char* argv[] = NULL);

/** コールバック関数*/
public:
	void CallDisplay();
	void CallReshape(int w, int h);
	void CallIdleFunc();
	void CallKeyboard(int key, int x, int y);
	void CallMouseButton(int button, int state, int x, int y);
	void CallMouseMove(int x, int y);
	void CallJoystick(unsigned int buttonMask, int x, int y, int z);

public:

	/** @brief タイマーを作成する
		@param	mode	タイマの種類
		@return			タイマオブジェクト
	 */
	UTTimerIf*	CreateTimer(UTTimerIf::Mode mode = UTTimerIf::FRAMEWORK);

public:
	/**  削除候補API  **/
	/// ウィンドウを1つだけ作成
	void	InitWindow(){ if(!NWin()) CreateWin(); }
	/// 初期状態にする
	void	Clear(){}
	/// リセットする
	void	Reset(){}

};

}
#endif

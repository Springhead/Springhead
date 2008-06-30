/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <Framework/SprFWSdk.h>
#include <Framework/SprFWScene.h>
#include <map>

namespace Spr{;

/** Rubyのプログラム上でFWAppを継承していくつかの関数をオーバライドする際に，
　　フレームワークがうまくオーバライドされた関数を認識し実行するためのメカニズム
 */
class FWVFuncBridge : public UTRefCount{
public:
	virtual void Link(void* pObj) = 0;
	virtual bool Display() = 0;
	virtual bool Reshape(int w, int h) = 0;
	virtual bool Keyboard(int key, int x, int y) = 0;
	virtual bool MouseButton(int button, int state, int x, int y) = 0;
	virtual bool MouseMove(int x, int y) = 0;
	virtual bool Step() = 0;
	virtual bool Idle() = 0;
	virtual bool Joystick(unsigned int buttonMask, int x, int y, int z) = 0;
	virtual void AtExit() = 0;
	virtual ~FWVFuncBridge(){}
};

//	hase	TypeDescができないようにクラスにしてある。TypeDesc側での対応が望ましい。
class FWWinDesc{
public:
	int width;			///<	幅
	int height;			///<	高さ
	int left;			///<	左端の座標
	int top;			///<	上端の座標
	int parentWindow;	///<	子ウィンドウを作る場合は、親のID、そうでなければ0
	UTString title;		///<	ウィンドウのタイトル文字列(トップレベルウィンドウのみ)
	bool fullscreen;	///<	フルスクリーンにするかどうか

	FWWinDesc(int w=640, int h=480, int l=-1, int t=-1, int p=0, bool f=false):
		width(w), height(h), left(l), top(t), parentWindow(p), fullscreen(f){
	}
};

/**	@brief ウィンドウ
	window ID, scene, renderを持つ．
	実装されるのはFWWinGlutなので，FWAppGlutでの使用が前提．
 */
class FWWin : public UTRefCount, public FWWinDesc{
protected:
	int id;
public:	// protectedでよいが互換性のため一時的に
	UTRef<GRRenderIf>	render;
	UTRef<FWSceneIf>	scene;

	FWWin(int wid, GRRenderIf* r):id(wid), render(r){}
public:
	virtual void Position(int left, int top){}
	virtual void Reshape(int width, int height){}
	virtual void SetTitle(UTString title){}
	virtual UTString GetTitle(){ return UTString(); }
	virtual void SetFullScreen(bool full = true){}
	virtual bool GetFullScreen(){ return false; }
	virtual int  GetWidth(){ return 0; }
	virtual int  GetHeight(){ return 0; }

	int			GetID() const{ return id; }
	GRRenderIf*	GetRender(){ return render; }
	FWSceneIf*  GetScene(){ return scene; }
	void		SetScene(FWSceneIf* s){ scene = s; }

};
typedef FWWinDesc FWAppGLUTDesc;

/** @brief アプリケーションクラス
	Springheadのクラスは基本的に継承せずに使用するように設計されているが，
	FWAppおよびその派生クラスは例外であり，ユーザはFWAppあるいはその派生クラスを継承し，
	仮想関数をオーバライドすることによって独自機能を実装する．

	FWAppクラスはグラフィックスの初期化機能を持たないので，
	通常は派生クラスであるFWAppGLやFWAppGLUTを使用する．
 */
class FWApp{
protected:
	/// マウス情報
	struct MouseInfo{
		TVec2<int> lastPos;			/// 前回のカーソル座標
		bool left, middle, right;	/// ボタン押し下げ
		bool shift, ctrl, alt;		/// コントロールキー状態
		bool first;
		MouseInfo():left(false), middle(false), right(false), first(false){}
	} mouseInfo;
	/// カメラ情報
	/// 本来はscene毎に個別に保持すべき。要修正
	struct CameraInfo{
		Vec3f target;		/// 中心点
		Vec2f rot;			/// 経度角，緯度角
		float zoom;			/// 拡大率（対象からの距離）
		Vec2f rotRangeX, rotRangeY;
		Vec2f zoomRange;
		Affinef view;
		CameraInfo():
			rot(Rad(0.0), Rad(20.0)), zoom(1.0f),
			rotRangeY(Rad(-180.0), Rad(180.0)), rotRangeX(Rad(-80.0), Rad(80.0)), zoomRange(0.01f, 100.0f){}
	} cameraInfo;

	/// 剛体ドラッグ機能
	struct DragInfo{
		PHRayIf* ray;			/// カーソル下の剛体を特定するためのPHRay
		PHSolidIf* cursor;		/// カーソル剛体
		PHSpringIf* spring;
		float	depth;
		DragInfo():ray(NULL), cursor(NULL), spring(NULL), depth(0.0f){}
	};
	std::map<FWSceneIf*, DragInfo>	dragInfo;
	
	UTRef<FWSdkIf> fwSdk;
	typedef std::vector< UTRef<FWWin> > Wins;
	Wins wins;

	void AssignScene(FWWin* win);
	
	void CallDisplay(){
		if(!vfBridge || !vfBridge->Display())
			Display();
	}
	void CallReshape(int w, int h){
		if(!vfBridge || !vfBridge->Reshape(w, h))
			Reshape(w, h);
	}
	void CallKeyboard(int key, int x, int y){
		if(!vfBridge || !vfBridge->Keyboard(key, x, y))
			Keyboard(key, x, y);
	}
	void CallMouseButton(int button, int state, int x, int y){
		if(!vfBridge || !vfBridge->MouseButton(button, state, x, y))
			MouseButton(button, state, x, y);
	}
	void CallMouseMove(int x, int y){
		if(!vfBridge || !vfBridge->MouseMove(x, y))
			MouseMove(x, y);
	}
	void CallStep(){
		if(!vfBridge || !vfBridge->Step())
			Step();
	}
	void CallIdle(){
		if(!vfBridge || !vfBridge->Idle())
			Idle();
	}
	void CallJoystick(unsigned int buttonMask, int x, int y, int z){
		if(!vfBridge || !vfBridge->Joystick(buttonMask, x, y, z))
			Joystick(buttonMask, x, y, z);
	}

	/** @brief Ctrl, Shift, Altの状態を返す
		個々の派生クラスで実装される
	 */
	virtual int GetModifier(){ return 0; }

public:
	// 以下の定数はGLUTに合わせてある
	enum MouseButtonType{
		LEFT_BUTTON = 0,
		MIDDLE_BUTTON = 1,
		RIGHT_BUTTON = 2,
	};
	enum MouseButtonState{
		BUTTON_DOWN = 0,
		BUTTON_UP = 1,
	};
	enum ModifierMask{
		ACTIVE_SHIFT = 1,
		ACTIVE_CTRL = 2,
		ACTIVE_ALT = 4,
	};

	UTRef<FWVFuncBridge>	vfBridge;

	/** @brief SDKを取得する
	 */
	FWSdkIf*	GetSdk(){ return fwSdk; }

	/** @brief 初期化
		FWAppオブジェクトの初期化を行う．最初に必ず呼ぶ．
	 */
	virtual void Init(int argc = 0, char* argv[] = NULL);

	/** @brief コマンドライン引数の処理
		アプリケーションに渡されたコマンドライン引数を処理したい場合にオーバライドする
	 */
	virtual void ProcessArguments(int argc, char* argv[]){}

	/** @brief ウィンドウに対応するコンテキストを作る
		@param desc	ディスクリプタ
		ウィンドウを作成する．対応するレンダラは新しく作成され，
		シーンはアクティブなシーンが関連つけられる．
	 */
	virtual FWWin* CreateWin(const FWWinDesc& desc){ return NULL; }
	
	/** @brief ウィンドウを削除する
	 */
	virtual void DestroyWin(FWWin* win){}

	/** @brief ウィンドウの数
	 */
	virtual int NWin(){ return (int)wins.size(); }
	
	/**	@brief ウィンドウをIDから探す
	 */
	virtual FWWin* GetWinFromId(int wid);
 	
	/** @brief ウィンドウを取得する
		indexで指定されたウィンドウを取得する．
	 */
	virtual FWWin* GetWin(int index);

	/** @brief 現在のウィンドウを取得する
	 */
	virtual FWWin* GetCurrentWin(){ return NULL;}

	/** @brief 現在のウィンドウを設定する
	 */
	virtual void SetCurrentWin(FWWin* win){}

	/** @brief シミュレーションの実行
		デフォルトではFWSdk::Stepが呼ばれる．
	 */
	virtual void Step();

	/** @brief idle関数
	 */
	virtual void Idle(){}

	/** @brief シーンの描画
		シーンが表示されるときに呼ばれる．
		描画処理をカスタマイズしたい場合にオーバライドする．
		デフォルトではFWSdk::Drawが呼ばれる．
	 */
	virtual void Display();

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

	virtual ~FWApp();

};

}
#endif

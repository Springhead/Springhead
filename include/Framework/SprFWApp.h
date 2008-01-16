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

namespace Spr{;

/** Rubyのプログラム上でFWAppを継承していくつかの関数をオーバライドする際に，
　　フレームワークがうまくオーバライドされた関数を認識し実行するためのメカニズム
 */
class FWVFuncBridge : public UTRefCount{
public:
	virtual void Link(void* pObj) = 0;
	virtual bool Display() = 0;
	virtual bool Reshape(int w, int h) = 0;
	virtual bool Keyboard(unsigned char key, int x, int y) = 0;
	virtual bool MouseButton(int button, int state, int x, int y) = 0;
	virtual bool MouseMove(int x, int y) = 0;
	virtual bool Step() = 0;
	virtual bool Idle() = 0;
	virtual void AtExit() = 0;
	virtual ~FWVFuncBridge(){}
};

/** @brief アプリケーションクラス
	Springheadのクラスは基本的に継承せずに使用するように設計されているが，
	FWAppおよびその派生クラスは例外であり，ユーザはFWAppあるいはその派生クラスを継承し，
	仮想関数をオーバライドすることによって独自機能を実装する．

	FWAppクラスはグラフィックスの初期化機能を持たないので，
	通常は派生クラスであるFWAppGLやFWAppGLUTを使用する．
 */
class FWApp{
protected:
	UTRef<FWSdkIf> fwSdk;
	
	void CallDisplay(){
		if(!vfBridge || !vfBridge->Display())
			Display();
	}
	void CallReshape(int w, int h){
		if(!vfBridge || !vfBridge->Reshape(w, h))
			Reshape(w, h);
	}
	void CallKeyboard(unsigned char key, int x, int y){
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
public:
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
	virtual void Keyboard(unsigned char key, int x, int y){}

	/** @brief マウスイベントのハンドラ
	 */
	virtual void MouseButton(int button, int state, int x, int y){}

	/** @brief マウスイベントのハンドラ
	 */
	virtual void MouseMove(int x, int y){}

	virtual ~FWApp();

	//	現在のWindowを返す
	virtual FWWin* GetWin(){ return NULL; }
};

}
#endif

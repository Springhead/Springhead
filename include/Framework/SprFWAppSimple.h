/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWAPP_SIMPLE_H
#define SPR_FWAPP_SIMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
using namespace Spr;								// Springheadの名前空間

/** @brief アプリケーションクラス
	FWAppSimpleはFWAppの派生クラスであり，ユーザーによる実装部分を実装したクラスである．
	BuildObject()をオーバーライドすることでシミュレーションを作成することができる．
	独自の機能は仮想関数をオーバライドすることによって実装する．
 */

class FWAppSimple;
class FWAppSimple : public FWApp{
public:
	FWAppSimple();									// コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	virtual void Init(int argc, char* argv[]);		// GLUTの初期化
	virtual void Timer();							// タイマーの作成
	virtual void IdleFunc(){};						// IdleFuncの作成
	virtual void InitCameraView();					// カメラ座標		
	virtual void BuildObject()=0;					// 物体を作成
	virtual void Step();							// シミュレーションを1ステップの処理
	virtual void Display();							// 描画
	virtual void Reset();							// シーンのリセット
	virtual void Keyboard(int key, int x, int y){};	// キーボード関数
	void				CallStep();
	static void SPR_CDECL	TimerFunc(int id);			// ループで呼ばれる関数

	bool bDrawInfo;
}; 

#endif

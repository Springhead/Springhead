/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWHAPTIC_SAMPLE_H
#define FWHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWHapticSample : public FWApp{
public:
	enum EngineType{
		SINGLE = 0,				// シングルすれっど
		MULTI,					// マルチスレッド
		LD,						// マルチ+局所シミュレーション
	} engineType;
	PHSceneIf* phscene;			// PHSceneへのポインタ

	PHHapticPointerIf* pointer; // 力覚ポインタへのポインタ
	float pdt;					// 物理スレッドの刻み
	float hdt;					// 力覚スレッドの刻み
	int physicsTimerID;			// 物理スレッドのタイマ
	int hapticTimerID;			// 力覚スレッドのタイマ
	UTRef<HIBaseIf> device;		// 力覚インタフェースへのポインタ
	HIHapticDummyIf* dummyDevice;	// キーボードで操作するダミーデバイス
	bool bPause;				//	シミュレーションの一時停止
	
	FWHapticSample();
	void InitInterface();						// 力覚インタフェースの初期化
	void Init(int argc = 0, char* argv[] = 0);	// アプリケーションの初期化
	virtual void BuildScene();						// オブジェクトの作成
	virtual void Keyboard(int key, int x, int y);	// キーボード関数
	void TimerFunc(int id);							// 各タイマが呼ぶコールバック関数
};


#endif
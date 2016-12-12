/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef APP_H
#define APP_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class App : public FWApp{
public:
	float hdt;								// 力覚スレッドの刻み
	int physicsTimerID;						// 物理スレッドのタイマ
	int hapticTimerID;						// 力覚スレッドのタイマ
	UTRef<HIBaseIf> spg;					// 力覚インタフェースへのポインタ
	PHSceneIf* phScene;
	PHSolidIf* floor;
	PHSolidIf* pointer;

	App();
	void InitInterface();					// 力覚インタフェースの初期化
	void Init(int argc = 0, char* argv[] = 0);		// アプリケーションの初期化
	void BuildScene();
	virtual void Keyboard(int key, int x, int y);	// キーボード関数
	void TimerFunc(int id);							// 各タイマが呼ぶコールバック関数
	void RenderHaptic();	//	1kHzで呼ばれる力覚計算関数
};

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWHAPTIC_APP_BASE_H
#define FWHAPTIC_APP_BASE_H

#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>
#include <Framework/FWExpandedPHSolid.h>
#include <vector>

namespace Spr{;

typedef 	std::vector<PHSolidIf*> HapticPointers;

class FWHapticAppBase : public FWAppGLUT, public UTRefCount{
protected:
	// 変数
	HapticPointers hapticPointers;
	FWExpandedPHSolids expandedPHSolids;
	double localRange;
	// フラグ
	bool bDebug;
public:
	// 
	FWHapticAppBase();
	virtual void Init(int argc, char* argv[]);							///< 初期化，はじめにこれを呼ぶ
	virtual void InitCameraView();										///< 初期カメラ位置
	virtual void ResetScene() = 0;										///< シーンをリセット
	virtual void BuildScene() = 0;										///< シーンをビルド
	virtual void CallBack() = 0;											///< MultiMediaTimerが呼ぶコールバック関数
	virtual void Start();														///< アプリケーション開始
	virtual void Idle() = 0;													///< glutIdelFuncが呼ぶ関数
	virtual void Step() = 0;												///< シミュレーションステップ
	virtual void Display();													///< 描画関数(glutPostRedisplay()で呼ばれる
	virtual void DebugDisplay(GRDebugRenderIf* render);		///< デバック表示にしたときに呼ばれる関数
	virtual FWExpandedPHSolid** ExpandPHSolidInfo();			///< シーンが持つPHSolidに力覚提示に必要な情報を付加する
	virtual void FindNearestObjectFromHapticPointer(PHSolidIf * hPointer);			///< 力覚ポインタ近傍の物体を見つける
	virtual void UpdateHapticPointer(int i, PHSolid hapticInterface);						///< 力覚インタフェースの状態を力覚ポインタに設定
	virtual void SyncHapticProcess() = 0;								///< HapticProcessと同期する関数
	virtual void Keyboard(int key, int x, int y) = 0;					///< glutKeyboarcFunc()が呼ぶ関数

	// protected変数へのアクセス
	void AddHapticPointer(PHSolidIf* ps);
	PHSolidIf* GetHapticPointer(int i);
	FWExpandedPHSolid** GetFWExpandedPHSolids();
	int GetNExpandedPHSolids();

	// フラグを切り替えるための関数
	void SetDebugMode(bool bD);
	bool GetDebugMode();
};

}
#endif
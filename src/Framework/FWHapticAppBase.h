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

class FWHapticAppBase : public FWAppGLUT, public UTRefCount{
protected:
	// 変数
	int nIter;																	///< iteration数
	double hapticTimeStep;												///< シミュレーションループ時間
	double physicTimeStep;												///< ハプティックループ時間
	PHSolid hapticInterface;												///< hapticInterface
	PHSolidIf* hapticPointer;												///< hatpticPointer
	FWExpandedPHSolids expandedPHSolids;						///< ExpandedPHSolidを格納vector
	double localRange;
	// フラグ
	bool bStep;																///< 物理シミュレーションするかどうか
	bool bOneStep;															///< シミュレーションのステップ実行
	bool bDebug;																///< デバックモード切替
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
	virtual void FindNearestObjectFromHapticPointer(PHSolidIf* hPointer);			///< 力覚ポインタ近傍の物体を見つける
	virtual void UpdateHapticPointer(PHSolidIf* hPointer, PHSolid hInterface);		///< 力覚インタフェースの状態を力覚ポインタに設定
	virtual void SyncHapticProcess() = 0;								///< HapticProcessと同期する関数
	virtual void Keyboard(int key, int x, int y) = 0;					///< glutKeyboarcFunc()が呼ぶ関数

	// protected変数へのアクセス
	void SetNIter();
	void SetHapticTimeSetp();
	double GetHapticTimeStep();
	void SetPhysicTimeSetp();
	double GetPhysicTimeStep();
	PHSolid GetHapticInterface();
	void SetHapticPointer(PHSolidIf* hpointer);
	PHSolidIf* GetHapticPointer();										///< HapticPointerを返す
	FWExpandedPHSolid** GetFWExpandedPHSolids();			///< ExpandedPHSolidsを返す
	int GetNExpandedPHSolids();											///< ExpandedPHSolidsの数を返す

	// フラグを切り替えるための関数
	void SetDebugMode(bool bD);										///< デバック表示モード切替
	bool GetDebugMode();													///< デバック状態を返す
};

}
#endif
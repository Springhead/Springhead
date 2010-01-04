/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWLDHAPTIC_SAMPLE_H
#define FWLDHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWLDHapticSample : public FWApp{
public:
	FWLDHapticSample();
	virtual void Init(int argc, char* argv[]);			///< アプリケーションの初期化
	virtual void InitCameraView();						///< カメラ行列の初期化
	virtual void InitHumanInterface();					///< HapticInterfaceの初期化
	virtual void Reset();
	virtual void TimerFunc(int id);
	virtual void IdleFunc();							
	virtual void Display();								///< 描画関数	
	virtual void BuildScene();							///< 剛体の作成
	virtual void BuildPointer();						///< 剛体の作成
								
	virtual void Keyboard(int key, int x, int y);		///< キーボード関数

	/// オプション
	bool bStep;									///< シミュレーションを進めるかどうか
	bool bOneStep;								///< ステップ実行
	bool bDrawInfo;								///< デバック表示するかどうか
	double dt;									///< 更新ステップ[s]
}; 

#endif
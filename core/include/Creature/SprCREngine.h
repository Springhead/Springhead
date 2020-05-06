/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRENGINEIF_H
#define SPR_CRENGINEIF_H

#include <Foundation/SprObject.h>

namespace Spr{;

//@{

struct CRCreatureIf;

///	クリーチャ・エンジンの基本クラス
struct CREngineIf : public SceneObjectIf{
	SPR_IFDEF(CREngine);

	/** @brief 実行順序を決めるプライオリティ値．小さいほど早い
	 */
	int GetPriority() const;

	/** @brief 初期化する
	 */
	void Init();

	/** @brief 処理を１ステップ分実行する
	 */
	void Step();

	/** @breif 有効・無効を切り替える
	*/
	void Enable(bool enable);

	/** @brief 有効・無効かを返す
	*/
	bool IsEnabled();
};

struct CREngineDesc{
	SPR_DESCDEF(CREngine);

	/// クリーチャのモジュールの分類と優先順位
	///	100の倍数は分類を表し、以下の位はその中での実行順序を規定する
	enum CREnginePriority {
		// 感覚系
		CREP_SENSOR			= 100,
		// 行動決定
		CREP_DECISION		= 200,
		// 運動制御
		CREP_CONTROLLER		= 300,
	};

	CREngineDesc(){
	}
};

//@}

}

#endif//SPR_CRENGINEIF_H

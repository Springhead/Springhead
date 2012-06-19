/*
 *  Copyright (c) 2003-2009, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRSDKIF_H
#define SPR_CRSDKIF_H

#include <Foundation/SprScene.h>

namespace Spr{;

//@{

struct CRCreatureIf;
struct CRCreatureDesc;

/// クリーチャSdkのインタフェース
struct CRSdkIf : SdkIf {
	SPR_IFDEF(CRSdk);

	/** @brief SDKオブジェクトを作成する
	 */
	static CRSdkIf* SPR_CDECL CreateSdk();

	/** @brief クリーチャを一体作成する
	 */
	CRCreatureIf* CreateCreature(const IfInfo* ii, const CRCreatureDesc& desc);
	template <class T> CRCreatureIf* CreateCreature(const T& desc){
		return CreateCreature(T::GetIfInfo(), desc);
	}

	/** @brief クリーチャの個数（人数？）を取得する
	 */
	int NCreatures() const;

	/** @brief クリーチャ一体を取得する
	 */
	CRCreatureIf* GetCreature(int index);

	/** @brief 全クリーチャについて１ステップ処理を実行する
	 */
	void Step();

	static void SPR_CDECL RegisterSdk();
};

/// クリーチャSdkのデスクリプタ
struct CRSdkDesc {
	SPR_DESCDEF(CRSdk);

	CRSdkDesc(){
	}
};

//@}

}

#endif // SPR_CRSDKIF_H
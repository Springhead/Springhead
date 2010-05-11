/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRCREATUREIF_H
#define SPR_CRCREATUREIF_H

#include <SprFoundation.h>

namespace Spr{;

//@{

struct CRBodyIf;
struct CRBodyDesc;
struct CREngineIf;
struct CREngineDesc;
struct CRSceneIf;
struct CRSceneDesc;

// ------------------------------------------------------------------------------
/// 型情報登録

void SPR_CDECL CRRegisterTypeDescs();

// ------------------------------------------------------------------------------
/// クリーチャのインタフェース
struct CRCreatureIf : SceneIf{
	SPR_IFDEF(CRCreature);

	/** @brief 感覚→情報処理→運動 の１ステップを実行する
	*/
	void Step();

	/** @brief ボディをつくる
	*/
	CRBodyIf* CreateBody(const IfInfo* ii, const CRBodyDesc& desc);
	template <class T> CRBodyIf* CreateBody(const T& desc){
		return CreateBody(T::GetIfInfo(), desc);
	}

	/** @brief ボディを取得する
	*/
	CRBodyIf* GetBody(int i);

	/** @brief ボディの数を取得する
	*/
	int NBodies();

	/** @brief CREngineを作成する
	*/
	CREngineIf* CreateEngine(const IfInfo* ii, const CREngineDesc& desc);
	template <class T> CREngineIf* CreateEngine(const T& desc){
		return CreateEngine(T::GetIfInfo(), desc);
	}

	/** @brief CREngineを取得する
	*/
	CREngineIf* GetEngine(int i);

	/** @brief CREngineの数を取得する
	*/
	int NEngines();

	/** @brief CRSceneを作成する
	*/
	CRSceneIf* CreateScene(const IfInfo* ii, const CRSceneDesc& desc);
	template <class T> CRSceneIf* CreateScene(const T& desc){
		return CreateScene(T::GetIfInfo(), desc);
	}
	
	/** @brief CRSceneを取得する
	*/
	CRSceneIf* GetScene(int i);

	/** @brief CRSceneの数を取得する
	*/
	int NScenes();

	/** @brief 関連するPHSceneを取得する
	*/
	PHSceneIf* GetPHScene();
};

/// クリーチャのデスクリプタ
struct CRCreatureDesc{
	SPR_DESCDEF(CRCreature);

	CRCreatureDesc(){
	}
};

//@}

}

#endif // SPR_CRCREATUREIF_H

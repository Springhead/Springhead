/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRAttentionListIf_H
#define SPR_CRAttentionListIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct PHSolidIf;

// ------------------------------------------------------------------------------
/// 注意対象を保持する構造体
struct CRAttentionItem{
	/// 注意対象のとなる剛体
	PHSolidIf* solid;

	/// 剛体ローカル座標系における注意対象位置
	Vec3f      pos;

	/// 注意を引き付ける度合
	float      attractiveness;

	CRAttentionItem(){
		solid          = NULL;
		pos            = Vec3f(0,0,0);
		attractiveness = 0.0f;
	}
	CRAttentionItem(PHSolidIf* s, Vec3f p, float a){
		solid          = s;
		pos            = p;
		attractiveness = a;
	}
};

// ------------------------------------------------------------------------------
/// 注意物体リストのデスクリプタ
struct CRAttentionListDesc{

	CRAttentionListDesc(){
	}
};

/// 注意物体リストのインターフェース
struct CRAttentionListIf : SceneObjectIf{
	IF_DEF(CRAttentionList);

	/** @brief リストをクリアする
	*/
	virtual void Clear()= 0;

	/** @brief 注意対象リストをソートする
	*/
	virtual void SortAttentionList();

	/** @brief 剛体solidの座標posに設定された注意を取得する
		@param solid 剛体（剛体ではなく場所のみに設定された注意の場合NULL）
		@param pos 剛体のローカル座標系における座標
	*/
	virtual CRAttentionItem* GetAttentionItem(PHSolidIf* solid, Vec3f pos)= 0;

	/** @brief i番目にAttractivenessの大きい注意を取得する
	*/
	virtual CRAttentionItem* GetAttentionItem(int i)= 0;

	/** @brief AttractiveListに追加された対象の数
	*/
	virtual int NAttentionItems()= 0;
};

//@}

}

#endif//SPR_CRReachingMovementIf_H

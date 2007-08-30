/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRATTENTIONLIST_H
#define CRATTENTIONLIST_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"

#include <vector>
#include <algorithm>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意物体リスト
*/
class CRAttentionList : public SceneObject, public CRAttentionListIfInit, public CRAttentionListDesc {
private:
	/** @brief 注意対象のリスト
	*/
	std::vector<CRAttentionItem*> attentionList;

	/** @brief ソートに使う
	*/
	static bool Small(const CRAttentionItem* &a, const CRAttentionItem* &b);

	/** @brief 初期化処理
	*/
	void Init();

public:
	OBJECTDEF(CRAttentionList, SceneObject);
	ACCESS_DESC(CRAttentionList);

	CRAttentionList(){ Init(); }
	CRAttentionList(const CRAttentionListDesc& desc, SceneIf* s=NULL) : CRAttentionListDesc(desc) {
		if(s){SetScene(s);}
		Init();
	}

	/** @brief リストをクリアする
	*/
	virtual void Clear();

	/** @brief 注意対象リストをソートする
	*/
	virtual void SortAttentionList();

	/** @brief 剛体solidの座標posに設定された注意を取得する
		@param solid 剛体（剛体ではなく場所のみに設定された注意の場合NULL）
		@param pos 剛体のローカル座標系における座標
	*/
	virtual CRAttentionItem* GetAttentionItem(PHSolidIf* solid, Vec3f pos);

	/** @brief i番目の中意を取得する（普通Sort後に使う）
	*/
	virtual CRAttentionItem* GetAttentionItem(int i);

	/** @brief AttractiveListに追加された対象の数
	*/
	virtual int NAttentionItems();
};
}
//@}

#endif//CRATTENTIONLIST_H

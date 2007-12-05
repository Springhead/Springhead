/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRINTERNALSCENE_H
#define CRINTERNALSCENE_H

#include <Springhead.h>

#include <Foundation/Object.h>
#include "IfStubCreature.h"
#include "AccessDescDumpCreature.h"

#include <vector>
#include <algorithm>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 内部シーンを構成するオブジェクト
*/
class CRInternalSceneObject : public SceneObject, public CRInternalSceneObjectIfInit {

protected:
	/// 所属する内部シーン
	CRInternalSceneIf* internalScene;

public:
	OBJECTDEF(CRInternalSceneObject, SceneObject);
	ACCESS_DESC_CRInternalSceneObjectDesc_NOBASE;

	CRInternalSceneObject(){}
	CRInternalSceneObject(const CRInternalSceneObjectDesc& desc, CRInternalSceneIf* is) {
		SetDesc(&desc);
		internalScene = is;
	}

	/** @brief 指定したtypeか判定する
	*/
	virtual bool IsA(const char* typestr);

	/** @brief 種類を返す
	*/
	virtual const char* GetISObjType();

	/** @brief 剛体を返す
	*/
	virtual PHSolidIf* GetSolid();

	/** @brief 位置を返す
	*/
	virtual Vec3f GetPos();

	/** @brief 位置を設定する
	*/
	virtual void SetPos(Vec3d pos);
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 注意をひきつける物体
*/
class CRISAttractiveObject : public CRInternalSceneObject, public CRISAttractiveObjectIfInit {

private:

public:
	OBJECTDEF(CRISAttractiveObject, CRInternalSceneObject);
	ACCESS_DESC_CRISAttractiveObjectDesc(CRInternalSceneObject);

	CRISAttractiveObject(){}
	CRISAttractiveObject(const CRISAttractiveObjectDesc& desc, CRInternalSceneIf* is) 
		: CRInternalSceneObject((const CRInternalSceneObjectDesc&)desc, is)
	{
		SetDesc(&desc);
	}

	/** @brief 総合的にこの物体が注意をひきつける度合を得る
	*/
	virtual float GetTotalAttractiveness();

	/** @brief ボトムアップ注意の度合を加算する
	*/
	virtual void AddBottomupAttr(float attr);

	/** @brief ボトムアップ注意の度合をリセットする
	*/
	virtual void ClearBottomupAttr();

	/** @brief トップダウン注意の度合を設定する
	*/
	virtual void SetTopdownAttr(float attr);

	/** @brief 不確実性を増す
	*/
	virtual void IncUncertainty();

	/** @brief 不確実性を減らす
	*/
	virtual void DecUncertainty();

	/** @brief 不確実性を取得する
	*/
	virtual float GetUncertainty();
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 歩行のポテンシャルにかかわる物体
*/
class CRISTravelPotentialObject : public CRInternalSceneObject, public CRISTravelPotentialObjectIfInit {

private:

public:
	OBJECTDEF(CRISTravelPotentialObject, CRInternalSceneObject);
	ACCESS_DESC_CRISTravelPotentialObjectDesc(CRInternalSceneObject);

	CRISTravelPotentialObject(){}
	CRISTravelPotentialObject(const CRISTravelPotentialObjectDesc& desc, CRInternalSceneIf* is) 
		: CRInternalSceneObject((const CRInternalSceneObjectDesc&)desc, is)
	{
		SetDesc(&desc);
	}

	/*
	/// デスクリプタ関連
protected:
	Vec2f  strength;
	Vec2f  decay;
public:
	virtual void SetDesc(const void* desc){
		CRInternalSceneObject::SetDesc(desc);
		strength = ((CRISTravelPotentialObjectDesc*)desc)->strength;
		decay    = ((CRISTravelPotentialObjectDesc*)desc)->decay;
	}
	virtual bool GetDesc(void* desc){
		CRInternalSceneObject::GetDesc(desc);
		((CRISTravelPotentialObjectDesc*)desc)->strength = strength;
		((CRISTravelPotentialObjectDesc*)desc)->decay    = decay;
		return true;
	}
	*/

	/** @brief ポテンシャルの強さの係数（A, B）を得る
	*/
	virtual Vec2f GetStrengthCoeff();

	/** @brief ポテンシャルの強さの係数（A, B）を設定する
	*/
	virtual void SetStrengthCoeff(Vec2f strength);

	/** @brief ポテンシャルの減衰の係数（n, m）を得る
	*/
	virtual Vec2f GetDecayCoeff();

	/** @brief ポテンシャルの減衰の係数（n, m）を設定する
	*/
	virtual void SetDecayCoeff(Vec2f decay);

	/** @brief ある位置に対応するポテンシャルを計算する
	*/
	virtual Vec2f GetPotential(Vec2f currPos);
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 内部シーン
*/
class CRInternalScene : public SceneObject, public CRInternalSceneIfInit, public CRInternalSceneDesc {

private:
	/// 持ち主たるCreature
	CRCreatureIf* creature;

	/** @brief 注意対象のリスト
	*/
	std::vector<CRInternalSceneObject*> sceneObjects;

	/** @brief ソートに使う
	*/
	static bool LargerAttention(const CRInternalSceneObject* a, const CRInternalSceneObject* b);

public:
	OBJECTDEF(CRInternalScene, SceneObject);
	ACCESS_DESC(CRInternalScene);

	CRInternalScene(){}
	CRInternalScene(const CRInternalSceneDesc& desc, CRCreatureIf* c=NULL) : CRInternalSceneDesc(desc) {
		if(c!=NULL && c->GetScene()){SetScene(c->GetScene());}
		creature = c;
	}

	/** @brief ステップごとの処理を実行する
	*/
	virtual void Step();

	/** @brief Attractivenessをクリアする（本当はBottomupだけ）
	*/
	virtual void ClearAttractiveness();

	/** @brief Attractivenessの大きい順にソートする
	*/
	virtual void SortByAttractiveness();

	/** @brief 剛体solidの座標posに設定された注意を取得する
		@param solid 剛体（剛体ではなく場所のみに設定された注意の場合NULL）
		@param pos 剛体のローカル座標系における座標
	*/
	virtual CRInternalSceneObjectIf* FindObject(PHSolidIf* solid, Vec3f pos);

	/** @brief i番目の物体を取得する（Sort後に用いるとSortされた順序で取得できる）
	*/
	virtual CRInternalSceneObjectIf* GetISObject(int i);

	/** @brief 物体の数
	*/
	virtual int NObjects();

	/** @brief オブジェクトを作成する
	*/
	virtual CRInternalSceneObjectIf* CreateInternalSceneObject(const IfInfo* ii, const CRInternalSceneObjectDesc& desc);
};
}
//@}

#endif//CRINTERNALSCENE_H

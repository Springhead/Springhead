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
private:
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

	/*
	/// デスクリプタ関連
protected:
	PHSolidIf* solid;
	Vec3f position;
public:
	virtual void SetDesc(const void* desc){
		solid    = ((CRInternalSceneObjectDesc*)desc)->solid;
		position = ((CRInternalSceneObjectDesc*)desc)->position;
	}
	virtual bool GetDesc(void* desc){
		((CRInternalSceneObjectDesc*)desc)->solid    = solid;
		((CRInternalSceneObjectDesc*)desc)->position = position;
		return true;
	}
	*/

	/** @brief 剛体を返す
	*/
	virtual PHSolidIf* GetSolid();

	/** @brief 位置を返す
	*/
	virtual Vec3f GetPos();
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

	/*
	/// デスクリプタ関連
protected:
	float  attractiveness;
public:
	virtual void SetDesc(const void* desc){
		CRInternalSceneObject::SetDesc(desc);
		attractiveness = ((CRISAttractiveObjectDesc*)desc)->attractiveness;
	}
	virtual bool GetDesc(void* desc){
		CRInternalSceneObject::GetDesc(desc);
		((CRISAttractiveObjectDesc*)desc)->attractiveness = attractiveness;
		return true;
	}
	*/

	/** @brief 注意をひきつける度合を得る
	*/
	virtual float GetAttractiveness();

	/** @brief 注意をひきつける度合を設定する
	*/
	virtual void SetAttractiveness(float attractiveness);
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
	static bool LargerAttention(const CRInternalSceneObject* &a, const CRInternalSceneObject* &b);

public:
	OBJECTDEF(CRInternalScene, SceneObject);
	ACCESS_DESC(CRInternalScene);

	CRInternalScene(){}
	CRInternalScene(const CRInternalSceneDesc& desc, CRCreatureIf* c=NULL) : CRInternalSceneDesc(desc) {
		if(c!=NULL && c->GetScene()){SetScene(c->GetScene());}
		creature = c;
	}

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
	virtual CRInternalSceneObjectIf* GetObject(int i);

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

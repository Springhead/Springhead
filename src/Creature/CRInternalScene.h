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

#include <vector>
#include <algorithm>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief 内部シーンを構成するオブジェクト
*/
class CRInternalSceneObject : public SceneObject, public CRInternalSceneObjectIfInit, public CRInternalSceneObjectDesc {
private:
	/// 所属する内部シーン
	CRInternalSceneIf* internalScene;

public:
	OBJECTDEF(CRInternalSceneObject, SceneObject);
	// ACCESS_DESC(CRInternalSceneObject);

	CRInternalSceneObject(){}
	CRInternalSceneObject(const CRInternalSceneObjectDesc& desc, CRInternalSceneIf* is) : CRInternalSceneObjectDesc(desc) {
		internalScene = is;
	}

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
class CRISAttractiveObject : public CRInternalSceneObject, public CRISAttractiveObjectIfInit, public CRISAttractiveObjectDesc {
private:

public:
	OBJECTDEF(CRISAttractiveObject, CRInternalSceneObject);
	// ACCESS_DESC(CRISAttractiveObject);

	CRISAttractiveObject(){}
	CRISAttractiveObject(const CRISAttractiveObjectDesc& desc, CRInternalSceneIf* is) 
		: CRISAttractiveObjectDesc(desc)
		, CRInternalSceneObject((const CRInternalSceneObjectDesc&)desc, is)
	{
	}

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
class CRISTravelPotentialObject : public CRInternalSceneObject, public CRISTravelPotentialObjectIfInit, public CRISTravelPotentialObjectDesc {
private:

public:
	OBJECTDEF(CRISTravelPotentialObject, CRInternalSceneObject);
	// ACCESS_DESC(CRISTravelPotentialObject);

	CRISTravelPotentialObject(){}
	CRISTravelPotentialObject(const CRISTravelPotentialObjectDesc& desc, CRInternalSceneIf* is) 
		: CRISTravelPotentialObjectDesc(desc) 
		, CRInternalSceneObject((const CRInternalSceneObjectDesc&)desc, is)
	{
	}

	/** @brief ポテンシャル係数を設定する
	*/
	virtual Vec2f GetPotential();

	/** @brief ポテンシャル係数を設定する
	*/
	virtual void SetPotential(Vec2f potential);
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

/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRInternalSceneIf_H
#define SPR_CRInternalSceneIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct PHSolidIf;

// ------------------------------------------------------------------------------
/// 内部シーンを構成する物体のインタフェース
struct CRInternalSceneObjectIf : SceneObjectIf{
	IF_DEF(CRInternalSceneObject);

	/** @brief 剛体を返す
	*/
	virtual PHSolidIf* GetSolid()= 0;

	/** @brief 位置を返す
	*/
	virtual Vec3f GetPos()= 0;
};

/// 内部シーンを構成する物体のデスクリプタ
struct CRInternalSceneObjectDesc{
	DESC_DEF_FOR_OBJECT(CRInternalSceneObject);

	/// 位置の基準となる剛体
	PHSolidIf* solid;

	/// 剛体ローカル座標系における位置
	Vec3f pos;

	CRInternalSceneObjectDesc(){
		solid          = NULL;
		pos            = Vec3f(0,0,0);
	}
};

// ------------------------------------------------------------------------------
/// 注意をひきつける物体のインタフェース
struct CRISAttractiveObjectIf : CRInternalSceneObjectIf{
	IF_DEF(CRISAttractiveObject);

	/** @brief 注意をひきつける度合を得る
	*/
	virtual float GetAttractiveness()= 0;

	/** @brief 注意をひきつける度合を設定する
	*/
	virtual void SetAttractiveness(float attractiveness)= 0;
};

/// 注意をひきつける物体のデスクリプタ
struct CRISAttractiveObjectDesc : CRInternalSceneObjectDesc{
	DESC_DEF_FOR_OBJECT(CRISAttractiveObject);

	/// 注意を引き付ける度合
	float  attractiveness;

	CRISAttractiveObjectDesc(){
		attractiveness = 0.0f;
	}
};

// ------------------------------------------------------------------------------
/// 歩行のポテンシャル制御にかかわる物体のインタフェース
struct CRISTravelPotentialObjectIf : CRInternalSceneObjectIf{
	IF_DEF(CRISTravelPotentialObject);

	/** @brief ポテンシャル係数を設定する
	*/
	virtual Vec2f GetPotential()= 0;

	/** @brief ポテンシャル係数を設定する
	*/
	virtual void SetPotential(Vec2f potential)= 0;
};

/// 歩行のポテンシャル制御にかかわる物体のデスクリプタ
struct CRISTravelPotentialObjectDesc : CRInternalSceneObjectDesc{
	DESC_DEF_FOR_OBJECT(CRISTravelPotentialObject);

	/// ポテンシャル係数のベクトル
	Vec2f  potential;

	CRISTravelPotentialObjectDesc(){
		potential = Vec2f(0.0f, 0.0f);
	}
};

// ------------------------------------------------------------------------------
/// 内部シーンのインターフェース
struct CRInternalSceneIf : SceneObjectIf{
	IF_DEF(CRInternalScene);

	/** @brief Attractivenessの大きい順にソートする
	*/
	virtual void SortByAttractiveness()= 0;

	/** @brief 剛体solidの座標posに設定された注意を取得する
		@param solid 剛体（剛体ではなく場所のみに設定された注意の場合NULL）
		@param pos 剛体のローカル座標系における座標
	*/
	virtual CRInternalSceneObjectIf* FindObject(PHSolidIf* solid, Vec3f pos)= 0;

	/** @brief i番目の物体を取得する（Sort後に用いるとSortされた順序で取得できる）
	*/
	virtual CRInternalSceneObjectIf* GetObject(int i)= 0;

	/** @brief 物体の数
	*/
	virtual int NObjects()= 0;

	/** @brief オブジェクトを作成する
	*/
	virtual CRInternalSceneObjectIf* CreateInternalSceneObject(const IfInfo* ii, const CRInternalSceneObjectDesc& desc)= 0;
	template <class T> CRInternalSceneObjectIf* CreateInternalSceneObject(const T& desc){
		return CreateInternalSceneObject(T::GetIfInfo(), desc);
	}
};

/// 内部シーンのデスクリプタ
struct CRInternalSceneDesc{
	DESC_DEF_FOR_OBJECT(CRInternalScene);

	CRInternalSceneDesc(){
	}
};

//@}

}

#endif//SPR_CRReachingMovementIf_H

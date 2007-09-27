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

	/** @brief 指定したtypeか判定する
	*/
	virtual bool IsA(const char* typestr)= 0;

	/** @brief 種類を返す
	*/
	virtual const char* GetISObjType()= 0;

	/** @brief 剛体を返す
	*/
	virtual PHSolidIf* GetSolid()= 0;

	/** @brief 位置を返す
	*/
	virtual Vec3f GetPos()= 0;

	/** @brief 位置を設定する
	*/
	virtual void SetPos(Vec3d pos)= 0;
};

/// 内部シーンを構成する物体のデスクリプタ
struct CRInternalSceneObjectDesc{
	DESC_DEF_FOR_OBJECT(CRInternalSceneObject);

	/// 種類
	char* type;

	/// 位置の基準となる剛体
	PHSolidIf* solid;

	/// 剛体ローカル座標系における位置
	Vec3f position;

	CRInternalSceneObjectDesc(){
		type           = NULL;
		solid          = NULL;
		position       = Vec3f(0,0,0);
	}
};

// ------------------------------------------------------------------------------
/// 注意をひきつける物体のインタフェース
struct CRISAttractiveObjectIf : CRInternalSceneObjectIf{
	IF_DEF(CRISAttractiveObject);

	/** @brief 総合的にこの物体が注意をひきつける度合を得る
	*/
	virtual float GetTotalAttractiveness()= 0;

	/** @brief ボトムアップ注意の度合を加算する
	*/
	virtual void AddBottomupAttr(float attr)= 0;

	/** @brief ボトムアップ注意の度合をリセットする
	*/
	virtual void ClearBottomupAttr()= 0;

	/** @brief トップダウン注意の度合を設定する
	*/
	virtual void SetTopdownAttr(float attr)= 0;

	/** @brief 不確実性を増す
	*/
	virtual void IncUncertainty()= 0;

	/** @brief 不確実性を減らす
	*/
	virtual void DecUncertainty()= 0;

	/** @brief 不確実性を取得する
	*/
	virtual float GetUncertainty()= 0;
};

/// 注意をひきつける物体のデスクリプタ
struct CRISAttractiveObjectDesc : CRInternalSceneObjectDesc{
	DESC_DEF_FOR_OBJECT(CRISAttractiveObject);

	/// ボトムアップ性の注意の度合
	float bottomupAttr;

	/// トップダウン性の注意として割り当てられた値
	float topdownAttr;

	/// 不確実性レベル（物体が見えると減少し，見えないと増加する，0.0～1.0の値）
	float uncertainty;

	/// 不確実性レベルの増加率
	float uncertaintyIncRate;

	/// 不確実性レベルの減少率
	float uncertaintyDecRate;

	CRISAttractiveObjectDesc(){
		bottomupAttr = 0.0f;
		topdownAttr  = 0.0f;
		uncertainty  = 1.0f;
		uncertaintyIncRate = 0.99f;
		uncertaintyDecRate = 0.9f;
	}
};

// ------------------------------------------------------------------------------
/// 歩行のポテンシャル制御にかかわる物体のインタフェース
struct CRISTravelPotentialObjectIf : CRInternalSceneObjectIf{
	IF_DEF(CRISTravelPotentialObject);

	/*
	** 距離rにおけるレナード・ジョーンズポテンシャル
	**     U = -A / (r^n) + B / (r^m)
	*/

	/** @brief ポテンシャルの強さの係数（A, B）を得る
	*/
	virtual Vec2f GetStrengthCoeff()= 0;

	/** @brief ポテンシャルの強さの係数（A, B）を設定する
	*/
	virtual void SetStrengthCoeff(Vec2f strength)= 0;

	/** @brief ポテンシャルの減衰の係数（n, m）を得る
	*/
	virtual Vec2f GetDecayCoeff()= 0;

	/** @brief ポテンシャルの減衰の係数（n, m）を設定する
	*/
	virtual void SetDecayCoeff(Vec2f decay)= 0;

	/** @brief ある位置に対応するポテンシャルを計算する
	*/
	virtual Vec2f GetPotential(Vec2f currPos)= 0;
};

/// 歩行のポテンシャル制御にかかわる物体のデスクリプタ
struct CRISTravelPotentialObjectDesc : CRInternalSceneObjectDesc{
	DESC_DEF_FOR_OBJECT(CRISTravelPotentialObject);

	/// 強さの計数
	Vec2f  strength;
	/// 減衰の計数
	Vec2f  decay;

	CRISTravelPotentialObjectDesc(){
		strength = Vec2f(0.0f, 0.0f);
		decay    = Vec2f(0.0f, 0.0f);
	}
};

// ------------------------------------------------------------------------------
/// 内部シーンのインターフェース
struct CRInternalSceneIf : SceneObjectIf{
	IF_DEF(CRInternalScene);

	/** @brief ステップごとの処理を実行する
	*/
	virtual void Step()= 0;

	/** @brief Attractivenessをクリアする（本当はBottomupだけ）
	*/
	virtual void ClearAttractiveness()= 0;

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
	virtual CRInternalSceneObjectIf* GetISObject(int i)= 0;

	/** @brief 物体の数
	*/
	virtual int NObjects()= 0;

	/** @brief オブジェクトを作成する
	*/
	virtual CRInternalSceneObjectIf* CreateInternalSceneObject(const IfInfo* ii, const CRInternalSceneObjectDesc& desc)= 0;
	template <class T> CRInternalSceneObjectIf* CreateInternalSceneObject(const T& desc){
		return CreateInternalSceneObject(T::GetIfInfo(), desc);
	}

	/** @brief 正体のわからない物体がデフォルトで持つトップダウン注意の量を返す
	*/
	virtual float GetDefaultTopdownAttr()= 0;
};

/// 内部シーンのデスクリプタ
struct CRInternalSceneDesc{
	DESC_DEF_FOR_OBJECT(CRInternalScene);

	/// Uncertaintyが1の物体が持つTopDownAttentionの量
	float defaultTopdownAttr;

	CRInternalSceneDesc(){
		defaultTopdownAttr = 0.0f;
	}
};

//@}

}

#endif//SPR_CRReachingMovementIf_H

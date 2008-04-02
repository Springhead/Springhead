/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	SPR_IFDEF(CRInternalSceneObject);

	/** @brief 指定したtypeか判定する
	*/
	bool IsA(const char* typestr);

	/** @brief 種類を返す
	*/
	const char* GetISObjType();

	/** @brief 剛体を返す
	*/
	PHSolidIf* GetSolid();

	/** @brief 位置を返す
	*/
	Vec3f GetPos();

	/** @brief 位置を設定する
	*/
	void SetPos(Vec3d pos);
};

/// 内部シーンを構成する物体のデスクリプタ
struct CRInternalSceneObjectDesc{
	SPR_DESCDEF(CRInternalSceneObject);

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
	SPR_IFDEF(CRISAttractiveObject);

	/** @brief 総合的にこの物体が注意をひきつける度合を得る
	*/
	float GetTotalAttractiveness();

	/** @brief ボトムアップ注意の度合を加算する
	*/
	void AddBottomupAttr(float attr);

	/** @brief ボトムアップ注意の度合をリセットする
	*/
	void ClearBottomupAttr();

	/** @brief トップダウン注意の度合を設定する
	*/
	void SetTopdownAttr(float attr);

	/** @brief 不確実性を増す
	*/
	void IncUncertainty();

	/** @brief 不確実性を減らす
	*/
	void DecUncertainty();

	/** @brief 不確実性を取得する
	*/
	float GetUncertainty();
};

/// 注意をひきつける物体のデスクリプタ
struct CRISAttractiveObjectDesc : CRInternalSceneObjectDesc{
	SPR_DESCDEF(CRISAttractiveObject);

	/// ボトムアップ性の注意の度合
	float bottomupAttr;

	/// トップダウン性の注意として割り当てられた値
	float topdownAttr;

	/// この物体が不確実な時に発するAttention
	float unknownAttr;

	/// 不確実性レベル（物体が見えると減少し，見えないと増加する，0.0～1.0の値）
	float uncertainty;

	/// 不確実性レベルの増加率
	float uncertaintyIncRate;

	/// 不確実性レベルの減少率
	float uncertaintyDecRate;

	CRISAttractiveObjectDesc(){
		bottomupAttr = 0.0f;
		topdownAttr  = 0.0f;
		unknownAttr  = 1.0f;
		uncertainty  = 1.0f;
		uncertaintyIncRate = 0.99f;
		uncertaintyDecRate = 0.2f;
	}
};

// ------------------------------------------------------------------------------
/// 歩行のポテンシャル制御にかかわる物体のインタフェース
struct CRISTravelPotentialObjectIf : CRInternalSceneObjectIf{
	SPR_IFDEF(CRISTravelPotentialObject);

	/*
	** 距離rにおけるレナード・ジョーンズポテンシャル
	**     U = -A / (r^n) + B / (r^m)
	*/

	/** @brief ポテンシャルの強さの係数（A, B）を得る
	*/
	Vec2f GetStrengthCoeff();

	/** @brief ポテンシャルの強さの係数（A, B）を設定する
	*/
	void SetStrengthCoeff(Vec2f strength);

	/** @brief ポテンシャルの減衰の係数（n, m）を得る
	*/
	Vec2f GetDecayCoeff();

	/** @brief ポテンシャルの減衰の係数（n, m）を設定する
	*/
	void SetDecayCoeff(Vec2f decay);

	/** @brief ある位置に対応するポテンシャルを計算する
	*/
	Vec2f GetPotential(Vec2f currPos);
};

/// 歩行のポテンシャル制御にかかわる物体のデスクリプタ
struct CRISTravelPotentialObjectDesc : CRInternalSceneObjectDesc{
	SPR_DESCDEF(CRISTravelPotentialObject);

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
	SPR_IFDEF(CRInternalScene);

	/** @brief ステップごとの処理を実行する
	*/
	void Step();

	/** @brief Attractivenessをクリアする（本当はBottomupだけ）
	*/
	void ClearAttractiveness();

	/** @brief Attractivenessの大きい順にソートする
	*/
	void SortByAttractiveness();

	/** @brief 剛体solidの座標posに設定された注意を取得する
		@param solid 剛体（剛体ではなく場所のみに設定された注意の場合NULL）
		@param pos 剛体のローカル座標系における座標
	*/
	CRInternalSceneObjectIf* FindObject(PHSolidIf* solid, Vec3f pos);

	/** @brief i番目の物体を取得する（Sort後に用いるとSortされた順序で取得できる）
	*/
	CRInternalSceneObjectIf* GetISObject(int i);

	/** @brief 物体の数
	*/
	int NObjects();

	/** @brief オブジェクトを作成する
	*/
	CRInternalSceneObjectIf* CreateInternalSceneObject(const IfInfo* ii, const CRInternalSceneObjectDesc& desc);
	template <class T> CRInternalSceneObjectIf* CreateInternalSceneObject(const T& desc){
		return CreateInternalSceneObject(T::GetIfInfo(), desc);
	}
};

/// 内部シーンのデスクリプタ
struct CRInternalSceneDesc{
	SPR_DESCDEF(CRInternalScene);

	CRInternalSceneDesc(){
	}
};

//@}

}

#endif//SPR_CRReachingMovementIf_H

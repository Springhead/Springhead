/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRFrame.h
 *	@brief シーングラフのノード
*/

/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRFrame_H
#define SPR_GRFrame_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct GRRenderIf;

/** @brief ビジュアルのディスクリプタ */
struct GRVisualDesc{
};

struct GRMaterialIf;
/**	@brief	グラフィックスで表示に影響を与えるもの	*/
struct GRVisualIf: public SceneObjectIf{
	SPR_IFDEF(GRVisual);
	///	レンダリング．子ノード，弟ノードのレンダリングより前に呼ばれる．
	void Render(GRRenderIf* r);
	///	レンダリング終了処理．子ノード，弟ノードのレンダリングが終わってから呼ばれる．
	void Rendered(GRRenderIf* r);

	/// レンダリングの有効化/無効化
	void Enable(bool on = true);
	/// 有効/無効状態の取得
	bool IsEnabled();

	///
	GRMaterialIf* GetMaterial();
	///
	void SetMaterial(GRMaterialIf* mat);
};
		
///	DirectX の Frame の変換を表す FrameTransformMatrix ノードを読むためのDesc
struct GRFrameTransformMatrix{
	Affinef transform;
};

	
/**	@brief	グラフィックスシーングラフのツリーのノード．座標系を持つ．*/
struct GRFrameIf: public GRVisualIf{
	SPR_IFDEF(GRFrame);

	/** @brief 親フレームを取得する */
	GRFrameIf* GetParent();

	/** @brief 親フレームを設定する */
	void SetParent(GRFrameIf* fr);

	/** @brief 子ノードの数を取得 */
	int NChildren();

	/** @brief 子ノードの配列を取得 */
	GRVisualIf** GetChildren();

	/** @brief 親ノードとの相対変換を取得 */
	Affinef GetTransform();

	/** @brief ワールドフレームとの相対変換を取得 */
	Affinef GetWorldTransform();

	/** @brief 親ノードとの相対変換を設定 */
	void SetTransform(const Affinef& af);

	/** bounding boxを計算
		@param bbmin
		@param bbmax
		@param aff		AABBを定義する座標系との変換
		@return 子ビジュアルにGRMeshがなくてbboxが求まらない場合false

		このフレーム以下にあるGRMeshをもとにbounding boxを計算する
	 */
	bool CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Affinef& aff = Affinef());
};

///	@brief GRFrame のDesc．座標系を指定する
struct GRFrameDesc:public GRVisualDesc{
	SPR_DESCDEF(GRFrame);
	Affinef transform;
};


/**	@brief	グラフィクスのシーングラフのダミーノード．
	後でプログラムから使うために，Visualを入れておくためのコンテナ．
	描画などをしないので安心してしまっておける．	*/
struct GRDummyFrameIf: public GRVisualIf{
	SPR_IFDEF(GRDummyFrame);
};
/**	@brief GRDummyFrame のDesc．ダミーフレーム．
	Meshなどを表示したくはないが，とりあえずロードだけしておき，
	後でプログラムで参照したい場合，ダミーフレームに入れておけば
	無駄な描画がされない．	*/
struct GRDummyFrameDesc:public GRVisualDesc{
	SPR_DESCDEF(GRDummyFrame);
};
///	@brief アニメーションのデスクリプタ XのAnimationに対応
struct GRKey{
	unsigned time;				//	時刻
	std::vector<float> values;	//	変換の値
};
///	時刻と変換の対応表 XのAnimationKeyに対応
struct GRAnimationKey{
	int keyType;				//	変換の種類
	std::vector<GRKey> keys;	//	時刻と変換のセット
};
/**	@brief	アニメーション(GRFrameのツリーを動かす)	*/
struct GRAnimationIf: public SceneObjectIf{
	SPR_IFDEF(GRAnimation);
	///	アニメーション名と時刻で定まるボーンの変更を，重みをつけて現在のボーンの変換行列に適用する．
	void BlendPose(float time, float weight);
	///	ボーンの変換行列を初期値に戻す．
	void ResetPose();
	///	フレームの変換行列を初期値に戻す．
	void LoadInitialPose();
	///	AnimationKeyを取得する．
	GRAnimationKey GetAnimationKey(int n);
	///	AnimationKeyの数を取得する．
	int NAnimationKey();
	/// 時刻tのGRKeyを削除する
	void DeletePose(float t);
};

struct GRAnimationDesc{
	SPR_DESCDEF(GRAnimation);
	enum KeyType { ROTATION, SCALE, POSITION, MATRIX };
	///	複数のAnimationKeyでひとつの変換を表す
	std::vector<GRAnimationKey> keys;	
};

/**	@brief	アニメーション(GRFrameのツリーを動かす)	*/
struct GRAnimationSetIf: public SceneObjectIf{
	SPR_IFDEF(GRAnimationSet);
	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	void BlendPose(float time, float weight);
	///	フレームの変換行列を初期値に戻す．
	void ResetPose();
	///	フレームの変換行列を初期値に戻す．
	void LoadInitialPose();

	///	子オブジェクト(animations)を返す
	ObjectIf* GetChildObject(size_t p);
	/// 現在のGRFrameからポーズを作成する tは時刻を指定
	void SetCurrentAnimationPose(float t);
	/// 時刻tのGRKeyを削除する
	void DeleteAnimationPose(float t);
	/// アニメーションの最終時間を取得する
	float GetLastKeyTime();
};
///	@brief アニメーションセットのデスクリプタ
struct GRAnimationSetDesc{
	SPR_DESCDEF(GRAnimationSet);
};

/**	@brief	アニメーション(GRFrameのツリーを動かす)	*/
struct GRAnimationControllerIf: public SceneObjectIf{
	SPR_IFDEF(GRAnimationController);
	///	指定の時刻の変換に重みをかけて、ボーンをあらわすターゲットのフレームに適用する。
	void BlendPose(UTString name, float time, float weight);
	///	フレームの変換行列を初期値に戻す．
	void ResetPose();
	///	フレームの変換行列を初期値に戻す．
	void LoadInitialPose();

	///	GRAnimationの追加
	bool AddChildObject(ObjectIf* o);
	///	GRAnimationの削除
	bool DelChildObject(ObjectIf* o);
	///	GRAnimationの数
	size_t NChildObject();
	///	GRAnimationの取得
	ObjectIf* GetChildObject(size_t p);
	///	GRAnimationSetの取得
	GRAnimationSetIf* GetAnimationSet(size_t p);
};
///	@brief アニメーションコントローラのデスクリプタ
struct GRAnimationControllerDesc{
	SPR_DESCDEF(GRAnimationController);
};
//@}
}
#endif

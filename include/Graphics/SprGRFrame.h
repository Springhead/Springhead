/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
#include <SprFoundation.h>

namespace Spr{;

struct GRRenderIf;

/** @brief ビジュアルのディスクリプタ */
struct GRVisualDesc{
};

/**	@brief	グラフィックスで表示に影響を与えるもの	*/
struct GRVisualIf: public NamedObjectIf{
	IF_DEF(GRVisual);
	///	レンダリング．子ノード，弟ノードのレンダリングより前に呼ばれる．
	virtual void Render(GRRenderIf* r)=0;
	///	レンダリング終了処理．子ノード，弟ノードのレンダリングが終わってから呼ばれる．
	virtual void Rendered(GRRenderIf* r)=0;
};
		
///	DirectX の Frame の変換を表す FrameTransformMatrix ノードを読むためのDesc
struct GRFrameTransformMatrix{
	Affinef transform;
};

	
/**	@brief	グラフィックスシーングラフのツリーのノード．座標系を持つ．*/
struct GRFrameIf: public GRVisualIf{
	IF_DEF(GRFrame);

	/** @brief 親フレームを取得する */
	virtual GRFrameIf* GetParent()=0;

	/** @brief 親フレームを設定する */
	virtual void SetParent(GRFrameIf* fr)=0;

	/** @brief 子ノードの数を取得 */
	virtual int NChildren() = 0;

	/** @brief 子ノードの配列を取得 */
	virtual GRVisualIf** GetChildren() = 0;

	/** @brief 親ノードとの相対変換を取得 */
	virtual Affinef GetTransform()=0;

	/** @brief ワールドフレームとの相対変換を取得 */
	virtual Affinef GetWorldTransform()=0;

	/** @brief 親ノードとの相対変換を設定 */
	virtual void SetTransform(const Affinef& af)=0;

	virtual void Print(std::ostream& os) const =0;
};
///	@brief GRFrame のDesc．座標系を指定する
struct GRFrameDesc:public GRVisualDesc{
	DESC_DEF_FOR_OBJECT(GRFrame);
	Affinef transform;
};


/**	@brief	アニメーション(GRFrameのツリーを動かす)	*/
struct GRAnimationIf: public SceneObjectIf{
	IF_DEF(GRAnimation);
};
///	@brief アニメーションのデスクリプタ XのAnimationSetに対応
struct GRAnimationDesc{
	DESC_DEF_FOR_OBJECT(GRAnimation);
	enum KeyType { ROTATION, SCALE, POSITION, MATRIX };
	struct Key{
		unsigned time;				//	時刻
		std::vector<float> values;	//	変換の値
	};
	///	キーフレーム XのAnimationKeyに対応
	struct AnimationKey{
		KeyType keyType;			//	変換の種類
		std::vector<Key> keys;		//	キーフレーム(時刻と変換のセット)たち
	};
	///	フレーム１つ分のアニメーション XのAnimationに対応
	struct Animation{
		std::vector<AnimationKey> keys;		//	複数のキーフレーム
		std::vector<GRFrameIf*> targets;	//	対象のフレーム
	};
	///	アニメーション＝ひとつの行動に対応するアニメーション＝複数のフレームのアニメーション
	std::vector<Animation> animations;
};

/**	@brief	アニメーション(GRFrameのツリーを動かす)	*/
struct GRAnimationSetIf: public SceneObjectIf{
	IF_DEF(GRAnimationSet);
};
///	@brief アニメーションセットのデスクリプタ
struct GRAnimationSetDesc{
	DESC_DEF_FOR_OBJECT(GRAnimationSet);
};


//@}
}
#endif

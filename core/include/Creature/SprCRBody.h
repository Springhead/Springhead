/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CRBODYIF_H
#define SPR_CRBODYIF_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct CRBoneIf;
struct PHIKActuatorIf;

/// クリーチャのボディモデルのインターフェイス
struct CRBodyIf : SceneObjectIf{
	SPR_IFDEF(CRBody);

	/** @brief あるIKActuatorを持つ構成要素を探す
	*/
	CRBoneIf* FindByIKActuator(PHIKActuatorIf* actuator);

	/** @brief ラベルから構成要素を探す
	*/
	CRBoneIf* FindByLabel(UTString label);

	/** @brief 構成ボーンの数を得る
	*/
	int NBones();

	/** @brief i番目の構成ボーンを得る
	*/
	CRBoneIf* GetBone(int i);

	/** @brief ボディの重心座標を得る
	*/
	Vec3d GetCenterOfMass();

	/** @brief ボディの総質量を得る
	*/
	double GetSumOfMass();

	/** @brief １ステップ
	*/
	void Step();
};

/// クリーチャのボディモデルのデスクリプタ
struct CRBodyDesc{
	SPR_DESCDEF(CRBody);

	CRBodyDesc(){
	}
};

}

#endif//SPR_CRBODYIF_H

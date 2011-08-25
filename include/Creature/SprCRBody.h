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

//@{

struct CRBodyPartIf;
struct CRSolidIf;
struct CRJointIf;

/// クリーチャのボディモデルのインターフェイス
struct CRBodyIf : SceneObjectIf{
	SPR_IFDEF(CRBody);

	/** @brief ラベルから構成要素を探す
	*/
	CRBodyPartIf* FindByLabel(UTString label);

	/** @brief ボディに関するステップ処理を行う
	*/
	void Step();

	/** @brief 構成剛体の数を得る
	*/
	int NSolids();

	/** @brief i番目の構成剛体を得る
	*/
	CRSolidIf* GetSolid(int i);

	/** @brief 構成関節の数を得る
	*/
	int NJoints();

	/** @brief i番目の構成関節を得る
	*/
	CRJointIf* GetJoint(int i);

	/** @brief ボディの重心座標を得る
	*/
	Vec3d GetCenterOfMass();

	/** @brief ボディの総質量を得る
	*/
	double GetSumOfMass();
};

/// クリーチャのボディモデルのデスクリプタ
struct CRBodyDesc{
	SPR_DESCDEF(CRBody);

	CRBodyDesc(){
	}
};

}

#endif//SPR_CRBODY_H

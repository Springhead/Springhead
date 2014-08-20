/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHISkeletonSensor.h
 *	@brief スケルトン（ボーン構造）を認識するセンサ。Kinect, Leapmotionなど。
*/

#ifndef FW_SKELETONSENSORIF_H
#define FW_SKELETONSENSORIF_H

#include <Foundation/SprObject.h>
#include <HumanInterface/SprHISkeletonSensor.h>

namespace Spr{;
/**	\addtogroup	gpHumanInterface	*/
//@{

/**	@brief SkeletonSensorのデスクリプタ */
struct FWSkeletonSensorDesc {
	// あらかじめ登録されたPHSkeletonが無い（足りない）場合に自動でPHSkeletonを作る
	bool bCreatePHSkeleton;
	// PHSkeletonが剛体を持っていなかったら（足りなかったら）自動でPHSolidを作る
	bool bCreatePHSolid;
	// 自動でPHSolidを作る際に自動でCDShapeも作成する
	bool bCreateCDShape;
	// 自動でPHSolidを作る際に自動で２つのPHSolidを作り間をPHSpringでつなぐ
	bool bCreatePHSpring;
	// 自動で作るRoundConeの直径（＝指の太さ）
	Vec2d radius;

	FWSkeletonSensorDesc() {
		bCreatePHSkeleton = true;
		bCreatePHSolid    = true;
		bCreateCDShape    = true;
		bCreatePHSpring   = false;
		radius            = Vec2d(0.7, 0.7);
	}
};

struct PHSkeletonIf;
/**	@brief SkeletonSensorのインタフェース */
struct FWSkeletonSensorIf: public SceneObjectIf {
	SPR_IFDEF(FWSkeletonSensor);

	/// センサからの読み込みとPHSceneへの反映を行う
	void Update();

	/// 直径をセット
	void SetRadius(Vec2d r);

	/// 関連付けられたHISkeletonSensorを返す
	HISkeletonSensorIf* GetSensor();

	/// スケルトン情報を返す
	int NSkeleton();
	PHSkeletonIf* GetSkeleton(int i);

};

//@}
}
#endif

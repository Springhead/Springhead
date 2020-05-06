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

#ifndef HI_SKELETONSENSORIF_H
#define HI_SKELETONSENSORIF_H

#include <HumanInterface/SprHIBase.h>

namespace Spr{;
/**	\addtogroup	gpHumanInterface	*/
//@{

/**	@brief SkeletonSensorが返すスケルトンに含まれるボーン*/
struct HIBoneIf: public ObjectIf{
	SPR_IFDEF(HIBone);

	/// 親を返す
	HIBoneIf* GetParent();

	/// 位置を返す
	Vec3d GetPosition();
	/// 方向ベクトル（長さ1）を返す
	Vec3d GetDirection();
	/// 長さを返す
	double GetLength();
};

/**	@brief SkeletonSensorが返すスケルトン*/
struct HISkeletonIf: public ObjectIf{
	SPR_IFDEF(HISkeleton);

	/// 全体の姿勢を返す
	Posed GetPose();
	/// ボーンの数を返す
	int NBones();
	/// ボーンを返す
	HIBoneIf* GetBone(int i);
	/// ルートボーンを返す
	HIBoneIf* GetRoot();
	/// 掴んでる度を返す
	float GetGrabStrength();

	/// トラッキング状況
	bool IsTracked();

	/// トラッキングID
	int GetTrackID();
};

/**	@brief SkeletonSensorのインタフェース */
struct HISkeletonSensorIf: public HIBaseIf{
	SPR_IFDEF(HISkeletonSensor);

	/// スケルトンの数を返す
	int NSkeletons();
	/// スケルトンを返す
	HISkeletonIf* GetSkeleton(int i);

	// スケール係数をセットする
	void SetScale(double s);
	// 中心位置をセットする
	void SetCenter(Vec3d c);
	// 座標軸の向きを決める回転をセットする
	void SetRotation(Quaterniond q);

};

//@}
}
#endif

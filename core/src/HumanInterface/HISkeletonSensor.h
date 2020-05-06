/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef HI_SKELETONSENSOR_H
#define HI_SKELETONSENSOR_H

#include <HumanInterface/HIBase.h>
#include <HumanInterface/SprHISkeletonSensor.h>
#include <Foundation/SprObject.h>

#include <vector>

namespace Spr{;

// SkeletonSensorが返すスケルトンに含まれるボーン
class HIBone: public Object {
public:
	SPR_OBJECTDEF(HIBone);

	HIBoneIf* parent;

	Vec3d     position;
	Vec3d     direction;
	double    length;

	// ----- ----- ----- ----- -----
	// API関数

	/// 親を返す
	HIBoneIf* GetParent() { return parent; }

	/// 位置を返す
	Vec3d GetPosition() { return position; }
	/// 方向ベクトル（長さ1）を返す
	Vec3d GetDirection() { return direction; }
	/// 長さを返す
	double GetLength() { return length; }
};

// SkeletonSensorが返すスケルトン
class HISkeleton: public Object {
public:
	SPR_OBJECTDEF(HISkeleton);

	std::vector< UTRef<HIBoneIf> > bones;

	Posed pose;

	float grabStrength;

	int trackID;

	bool bTracked;

	// ----- ----- ----- ----- -----
	// API関数

	/// 全体の姿勢を返す
	Posed GetPose() { return pose; }
	/// ボーンの数を返す
	int NBones() { return (int)(bones.size()); }
	/// ボーンを返す
	HIBoneIf* GetBone(int i) { return bones[i]; }
	/// ルートボーンを返す
	HIBoneIf* GetRoot() {
		for (int i=0; i<(int)bones.size(); ++i) {
			if (bones[i]->GetParent()==NULL) { return bones[i]; }
		}
		return NULL;
	}
	/// 掴んでる度を返す
	virtual float GetGrabStrength() { return grabStrength; }
	/// トラッキング状況
	bool IsTracked() { return bTracked; }
	/// トラッキングID
	int GetTrackID() { return trackID; }

	// ----- ----- ----- ----- -----
	// 非API関数

	/// 個数に足りない分のボーンを準備
	void PrepareBone(int n) {
		for (int i=(int)bones.size(); i<n; ++i) {
			bones.push_back( (DBG_NEW HIBone())->Cast() );
		}
	}
};

// スケルトンを取得するセンサー（Kinect, Leapmotionなど）
class HISkeletonSensor: public HIBase {
public:
	SPR_OBJECTDEF(HISkeletonSensor);

	std::vector< UTRef<HISkeletonIf> > skeletons;

	double      scale;
	Vec3d       center;
	Quaterniond rotation;

	HISkeletonSensor() {
		scale    = 1.0;
		center   = Vec3d();
		rotation = Quaterniond();
	}

	// ----- ----- ----- ----- -----
	// API関数

	/// スケルトンの数を返す
	int NSkeletons() { return (int)(skeletons.size()); }
	/// スケルトンを返す
	HISkeletonIf* GetSkeleton(int i) { return skeletons[i]; }

	// スケール係数をセットする
	void SetScale(double s) { scale = s; }
	// 中心位置をセットする
	void SetCenter(Vec3d c) { center = c; }
	// 座標軸の向きを決める回転をセットする
	void SetRotation(Quaterniond q) { rotation = q; }

	// ----- ----- ----- ----- -----
	// 非API関数

	/// 個数に足りない分のスケルトンを準備
	void PrepareSkeleton(int n) {
		for (int i=(int)skeletons.size(); i<n; ++i) {
			skeletons.push_back( (DBG_NEW HISkeleton())->Cast() );
		}
	}
};

}

#endif//HI_LEAP_H

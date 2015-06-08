/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef FW_SKELETONSENSOR_H
#define FW_SKELETONSENSOR_H

#include <Foundation/Object.h>

#include <Framework/SprFWSkeletonSensor.h>
#include <Framework/SprFWScene.h>
#include <Physics/SprPHSkeleton.h>
#include <Physics/SprPHScene.h>
#include <Physics/SprPHSdk.h>
#include <HumanInterface/SprHISkeletonSensor.h>

#include <vector>

namespace Spr{;
struct FWSkeletonInfo {
	int                 id, invisibleCnt;
	UTRef<PHSkeletonIf> phSkeleton;
	FWSkeletonInfo() : id(-1), invisibleCnt(0), phSkeleton(NULL) {}
};

// HISkeletonSensorとPHSkeletonをつなぐひと
class FWSkeletonSensor: public FWSkeletonSensorDesc, public SceneObject {
public:
	SPR_OBJECTDEF(FWSkeletonSensor);
	ACCESS_DESC(FWSkeletonSensor);

	HISkeletonSensorIf*        sensor;
	std::vector<FWSkeletonInfo> skeletons;

	// ----- ----- ----- ----- -----

	FWSkeletonSensor(const FWSkeletonSensorDesc& desc = FWSkeletonSensorDesc()) : FWSkeletonSensorDesc(desc) {
		sensor = NULL;
	}

	// ----- ----- ----- ----- -----
	// API関数

	void Update();

	/// 直径をセット
	void SetRadius(Vec2d r) { this->radius = r; }

	/// 関連付けられたHISkeletonSensorを返す
	HISkeletonSensorIf* GetSensor() { return sensor; }

	/// スケルトン情報を返す
	int NSkeleton() const {
		int cnt = 0;
		for (size_t i = 0; i < skeletons.size(); ++i) {
			if (skeletons[i].id >= 0) { cnt++; }
		}
		return cnt;
	}
	PHSkeletonIf* GetSkeleton(int i) {
		int cnt = 0;
		for (size_t i = 0; i < skeletons.size(); ++i) {
			if (skeletons[i].id >= 0) { cnt++; }
			if (cnt == i) { return skeletons[i].phSkeleton; }
		}
		return NULL;
	}

	// ----- ----- ----- ----- -----
	// ベースクラスのAPI関数
	
	virtual bool        AddChildObject(ObjectIf* o) {
		if (DCAST(HISkeletonSensorIf, o)) { sensor = o->Cast();             return true; }
		if (DCAST(PHSkeletonIf, o))       { FWSkeletonInfo si; si.phSkeleton = o->Cast();  skeletons.push_back(si); return true; }
		return false;
	}
	virtual ObjectIf*   GetChildObject(size_t pos)  {
		if (sensor==NULL) {
			return GetSkeleton(pos);
		} else {
			if (pos==0) {
				return sensor;
			} else {
				return GetSkeleton(pos-1);
			}
		}
	}
	virtual size_t NChildObject() const {
		return( ((sensor==NULL) ? 0 : 1) + (size_t)(NSkeleton()) );
	}

	// ----- ----- ----- ----- -----
	// 非API関数

	void ProcessSkeleton(HISkeletonIf* hiSkel, int i);
	void AddSkeleton();
	void FreezeSkeleton(int i);
	void UnfreezeSkeleton(int i);
	void PrepareBone(PHSkeletonIf* phSkel, int n);

};

}

#endif//FW_SKELETONSENSOR_H

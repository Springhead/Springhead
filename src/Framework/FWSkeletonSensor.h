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

// HISkeletonSensorとPHSkeletonをつなぐひと
class FWSkeletonSensor: public FWSkeletonSensorDesc, public SceneObject {
public:
	SPR_OBJECTDEF(FWSkeletonSensor);

	HISkeletonSensorIf*        sensor;
	std::vector<PHSkeletonIf*> phSkeletons;

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

	// ----- ----- ----- ----- -----
	// ベースクラスのAPI関数
	
	virtual bool        AddChildObject(ObjectIf* o) {
		if (DCAST(HISkeletonSensorIf,o)) { sensor = o->Cast();             return true; }
		if (DCAST(PHSkeletonIf,o))       { phSkeletons.push_back(o->Cast()); return true; }
		return false;
	}
	virtual ObjectIf*   GetChildObject(size_t pos)  {
		if (sensor==NULL) {
			return phSkeletons[pos];
		} else {
			if (pos==0) {
				return sensor;
			} else {
				return phSkeletons[pos-1];
			}
		}
	}
	virtual size_t NChildObject() const {
		return( ((sensor==NULL) ? 0 : 1)+phSkeletons.size() );
	}

	// ----- ----- ----- ----- -----
	// 非API関数

	void PrepareSkeleton(int n);
	void PrepareBone(PHSkeletonIf* phSkel, int n);


};

}

#endif//FW_SKELETONSENSOR_H

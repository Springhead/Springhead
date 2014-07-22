/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef HI_LEAP_H
#define HI_LEAP_H

// Libraries/LeapSdkをチェックアウトして以下のdefineのコメントアウトを外すとLeapmotionが使えるようになる。
#define USE_LEAP

#include <HumanInterface/HISkeletonSensor.h>
#include <HumanInterface/SprHILeap.h>

#ifdef USE_LEAP
#include "../../Libraries/LeapSDK/include/Leap.h"
#pragma comment(lib, "Leap.lib")
#endif

namespace Spr{;

// Leapmotion
class HILeap: public HILeapDesc, public HISkeletonSensor {

	#ifdef USE_LEAP
	Leap::Controller* leap;
	#endif

public:
	SPR_OBJECTDEF(HILeap);

	HILeap(const HILeapDesc& desc = HILeapDesc()) {
	}

	~HILeap() {
		#ifdef USE_LEAP
		if (leap) { delete leap; }; leap = NULL;
		#endif
	}

	// ----- ----- ----- ----- -----
	// HIBaseのAPI

	bool Init(const void* desc);
	void Update(float dt);

	// ----- ----- ----- ----- -----
	// 非API関数

	/// Leap座標系から（scaleを考慮した上での）Springhead座標系へ
	#ifdef USE_LEAP
	inline Vec3d ToSpr(Leap::Vector lv) {
		return rotation * Vec3d(lv.x, lv.y, lv.z) * scale;
	}
	inline Quaterniond ToSpr(Leap::Matrix lm) {
		Matrix3d m;
		for (int i=0; i<3; ++i) {
			m[0][i] = lm.xBasis[i]; m[1][i] = lm.yBasis[i]; m[2][i] = lm.zBasis[i];
		}
		Quaterniond q; q.FromMatrix(m);
		return q;
	}
	#endif

};

}

#endif//HI_LEAP_H

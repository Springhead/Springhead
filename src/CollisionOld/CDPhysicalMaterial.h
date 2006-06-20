/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDPhysicalMaterial_H
#define CDPhysicalMaterial_H

#include <SceneGraph/SGObject.h>

namespace Spr {;

/// 物理マテリアルのデータ
class CDPhysicalMaterialData{
public:
	CDPhysicalMaterialData(){ InitData(); }
	///
	void InitData();
	///	抗力のばね係数
	float reflexSpring;
	///	抗力のダンパ係数
	float reflexDamper;
	///	摩擦力のばね係数
	float frictionSpring;
	///	摩擦力のダンパ係数
	float frictionDamper;
	///	最大静止摩擦係数
	float staticFriction;
	///	動摩擦係数
	float dynamicFriction;
};

/// 物理マテリアル
class CDPhysicalMaterial:public SGObject{
public:
	SGOBJECT_DEF(CDPhysicalMaterial);
	/// 物理マテリアルのデータ
	CDPhysicalMaterialData pMatData;
	CDPhysicalMaterial(){
	};	
};

}
#endif

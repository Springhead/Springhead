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
	SGOBJECTDEF(CDPhysicalMaterial);
	/// 物理マテリアルのデータ
	CDPhysicalMaterialData pMatData;
	CDPhysicalMaterial(){
	};	
};

}
#endif
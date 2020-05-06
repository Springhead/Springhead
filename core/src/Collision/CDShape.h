/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSHAPE_H
#define PHSHAPE_H

#include "../Foundation/Object.h"
#include <Collision/SprCDShape.h>
#include "../Collision/CollisionDecl.hpp"

namespace Spr{;

/// 形状
/// 形状自体は位置情報を持たない．位置情報は形状を参照する各剛体が持つ．
class CDShape : public NamedObject{
public:

	SPR_OBJECTDEF_ABST(CDShape);
	SPR_DECLMEMBEROF_CDShapeDesc;

	void	SetStaticFriction(float mu0){
		material.mu0 = mu0;
		DSTR << "friction: " << material.mu0 << std::endl;
	}
	float	GetStaticFriction(){ return material.mu0; }
	void	SetDynamicFriction(float mu){ material.mu = mu; }
	float	GetDynamicFriction(){ return material.mu; }
	void	SetElasticity(float e){ material.e = e; }
	float	GetElasticity(){ return material.e; }
	void	SetDensity(float d){ material.density = d; }
	float	GetDensity(){ return material.density; }
	void    SetContactSpring(float K){ material.spring = K; }
	float   GetContactSpring(){ return material.spring; }
	void    SetContactDamper(float D){ material.damper = D; }
	float   GetContactDamper(){ return material.damper; }

	void	SetReflexSpring(float K){ material.reflexSpring = K; }
	float	GetReflexSpring(){return material.reflexSpring;}
	void	SetReflexDamper(float D){ material.reflexDamper = D; }
	float	GetReflexDamper(){return material.reflexDamper;}
	void	SetFrictionSpring(float K){ material.frictionSpring = K; }
	float	GetFrictionSpring(){return material.frictionSpring;}
	void	SetFrictionDamper(float D){ material.frictionDamper = D; }
	float	GetFrictionDamper(){return material.frictionDamper;}
	
	void SetVibration(float vibA, float vibB, float vibW){
		material.vibA = vibA;
		material.vibB = vibB;
		material.vibW = vibW;
		material.vibT = 0;
		material.vibContact = false;
	}
	void SetVibA(float vibA){material.vibA = vibA;}
	float GetVibA(){return material.vibA;}
	void SetVibB(float vibB){material.vibB = vibB;}
	float GetVibB(){return material.vibB;}
	void SetVibW(float vibW){material.vibW = vibW;}
	float GetVibW(){return material.vibW;}
	void SetVibT(float vibT){material.vibT = vibT;}
	float GetVibT(){return material.vibT;}
	void SetVibContact(bool vibContact){material.vibContact = vibContact;}
	bool GetVibContact(){return material.vibContact;}

	const PHMaterial&	GetMaterial(){return material;}
	void				SetMaterial(const PHMaterial& mat){material = mat;}

	virtual float    CalcVolume(){ return FLT_MAX; }
	virtual Vec3f    CalcCenterOfMass(){ return Vec3f(); }
	virtual Matrix3f CalcMomentOfInertia(){ return Matrix3f(); }
	virtual void     CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose)=0;
	virtual bool     IsInside(const Vec3f& p){ return false; }
	virtual int	     LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset){ return 0; }

	// 幾何計算用のstatic関数
	static float    CalcCylinderVolume    (float r, float l);
	static Matrix3f CalcCylinderInertia   (float r, float l);
	static float    CalcHemisphereCoM     (float r);
	static float    CalcHemisphereVolume  (float r);
	static Matrix3f CalcHemisphereInertia (float r);
	static float    CalcConeCoM           (float l);
	static float    CalcConeVolume        (float r, float l);
	static Matrix3f	CalcConeInertia       (float r, float l);
	static float    CalcTetrahedronVolume (const Vec3f& a, const Vec3f& b, const Vec3f& c);
	static Vec3f    CalcTetrahedronCoM    (const Vec3f& a, const Vec3f& b, const Vec3f& c);
	static Matrix3f CalcTetrahedronInertia(const Vec3f& a, const Vec3f& b, const Vec3f& c);
	
};

}	//	namespace Spr
#endif

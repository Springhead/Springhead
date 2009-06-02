/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSHAPE_H
#define PHSHAPE_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CollisionDecl.hpp"

namespace Spr{;

/// 形状
/// 形状自体は位置情報を持たない．位置情報は形状を参照する各剛体が持つ．
class CDShape : public NamedObject{
public:

	SPR_OBJECTDEF_ABST(CDShape);
	SPR_DECLMEMBEROF_CDShapeDesc;

	void	SetStaticFriction(float mu0){ material.mu0 = mu0; }
	float	GetStaticFriction(){ return material.mu0; }
	void	SetDynamicFriction(float mu){ material.mu = mu; }
	float	GetDynamicFriction(){ return material.mu; }
	void	SetElasticity(float e){ material.e = e; }
	float	GetElasticity(){ return material.e; }
	void	SetDensity(float d){ material.density = d; }
	float	GetDensity(){ return material.density; }

	void SetSpringDamper(float spring, float damper){
		material.springK = spring;
		material.damperD = damper;
	}
	float GetSpringK(){return material.springK;}
	float GetDamperD(){return material.damperD;}
	void SetVibration(float vibA, float vibB, float vibW){
		material.vibA = vibA;
		material.vibB = vibB;
		material.vibW = vibW;
	}
	void SetVibA(float vibA){material.vibA = vibA;}
	float GetVibA(){return material.vibA;}
	void SetVibB(float vibB){material.vibB = vibB;}
	float GetVibB(){return material.vibB;}
	void SetVibW(float vibW){material.vibW = vibW;}
	float GetVibW(){return material.vibW;}

	PHMaterial	GetMaterial(){return material;}
	void		SetMaterial(PHMaterial mat){material = mat;}

	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose)=0;
	virtual bool IsInside(const Vec3f& p){ return false; }
	virtual float CalcVolume(){ return FLT_MAX; }
	virtual Vec3f	CalcCenterOfMass(){ return Vec3f(); }
	virtual Matrix3f CalcMomentOfInertia(){ return Matrix3f(); }

	virtual int	LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset){ return 0; }
};

}	//	namespace Spr
#endif

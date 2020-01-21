/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDShape.h>

namespace Spr{;

PHMaterial::PHMaterial(){
	e = 0.4f;
	density = 1.0f;
	spring = 0.0f;
	damper = 0.0f;

	reflexSpring = 5.0f;
	reflexDamper = 0.1f;
	frictionSpring = 5.0;
	frictionDamper = 0.1f;
	vibA = -20;
	vibB = 90;
	vibW = 200;
	vibT = 0;
	vibContact = false;

	mu = 0.35f;
	mu0 = 0.5f;


	/**	時変摩擦係数のための係数	ms = mu+ A log(1+Bt),  md = mu+ A log (1+B C/v)  [Dieterich 1979]
		Cは動摩擦時のスリップ距離を意味する C/v < dt(時間刻み) のときは、v=C/dt を使う */
	timeVaryFrictionA = 0.1f;
	timeVaryFrictionB = 1800;
	timeVaryFrictionC = 0.0f;
	///	粘性摩擦のための係数	f_t = frictionViscocity * vel * f_N
	frictionViscosity = 0.0f;
	stribeckVelocity = 0.1f;
	stribeckmu = 0.1f;


//GMS用
	timeVaryFrictionAs = { 0.1f,0.1f,0.1f };
	timeVaryFrictionBs = { 1800,1800,1800 };
	timeVaryFrictionCs = { 0.0f,0.0f,0.0f };
	timeVaryFrictionDs = { 0.0f,0.0f,0.0f };
	mus = { 0.35f,0.35f,0.35f };
	mu0s = { 0.5f,0.5f,0.50f };
	stribeckmus = { 0.1f,0.1f,0.1f };
	stribeckVelocitys = { 0.1f,0.1f,0.1f };
	c = { 0.01f,0.01f,0.01f };
	bristleK = { 1000.0f,700.0f,800.0f };

	

	velocityFieldMode      = VelocityField::NONE;
	velocityFieldAxis      = Vec3d();
	velocityFieldMagnitude = 0.0;
}

Vec3d PHMaterial::CalcVelocity(const Vec3d& pos, const Vec3d& normal) const{
	if(velocityFieldMode == VelocityField::NONE){
		return Vec3d();
	}
	if(velocityFieldMode == VelocityField::LINEAR){
		return velocityFieldAxis * velocityFieldMagnitude;
	}
	if(velocityFieldMode == VelocityField::CYLINDER){
		Vec3d  v  = velocityFieldAxis % pos;
		double vn = v * normal;
		Vec3d  vt = v - vn * normal;
		const double eps = 1.0e-10;
		double vtnorm = vt.norm();
		if(vtnorm < eps)
			return Vec3d();
		vt *= (velocityFieldMagnitude/vtnorm);
		return vt;
	}
	return Vec3d();
}

//-------------------------------------------------------------------------------------------------

float CDShape::CalcCylinderVolume(float r, float l){
	return (float)M_PI * r * r * l;
}

Matrix3f CDShape::CalcCylinderInertia(float r, float l){
	float Ix = (1.0f/4.0f) * r*r + (1.0f/12.0f) * l*l;
	float Iz = (1.0f/2.0f) * r*r;
	return CalcCylinderVolume(r, l) * Matrix3f::Diag(Ix, Ix, Iz);
}

float CDShape::CalcHemisphereCoM(float r){
	return (3.0f/8.0f) * r;
}

float CDShape::CalcHemisphereVolume(float r){
	return (4.0f/3.0f) * (float)M_PI * r*r*r;
}

Matrix3f CDShape::CalcHemisphereInertia(float r){
	float I = (2.0f/5.0f) * r*r;
	return CalcHemisphereVolume(r) * Matrix3f::Diag(I, I, I);
}

float CDShape::CalcConeCoM(float l){
	return (1.0f/4.0f) * l;
}

float CDShape::CalcConeVolume(float r, float l){
	return (1.0f/3.0f) * (float)M_PI * r*r * l;
}

Matrix3f CDShape::CalcConeInertia(float r, float l){
	float Ix = (3.0f/20.0f) * r*r + (1.0f/10.0f) * l*l;
	float Iz = (3.0f/10.0f) * r*r;
	return CalcConeVolume(r, l) * Matrix3f::Diag(Ix, Ix, Iz);
}

float CDShape::CalcTetrahedronVolume(const Vec3f& a, const Vec3f& b, const Vec3f& c){
	return (1.0f/6.0f) * std::abs(a * (b % c));
}
Vec3f CDShape::CalcTetrahedronCoM(const Vec3f& a, const Vec3f& b, const Vec3f& c){
	return (1.0f/4.0f) * (a + b + c);
}
Matrix3f CDShape::CalcTetrahedronInertia(const Vec3f& a, const Vec3f& b, const Vec3f& c){
	// 正準四面体からの変換行列
	Matrix3f A;
	A.col(0) = a;
	A.col(1) = b;
	A.col(2) = c;
	// 正準四面体のC行列
	float c0 = (1.0f/60.0f);
	float c1 = (1.0f/120.0f);
	Matrix3f C(
		c0, c1, c1,
		c1, c0, c1,
		c1, c1, c0);
	// C行列の変換
	Matrix3f Cd = std::abs(A.det()) * (A * C * A.trans());
	Matrix3f Id = (Cd[0][0] + Cd[1][1] + Cd[2][2]) * Matrix3f::Unit() - Cd;
	return Id;
}
	
}

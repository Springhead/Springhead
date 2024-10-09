/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDSphere.h>

namespace Spr{;
	
//----------------------------------------------------------------------------
//	CDSphere
	
CDSphere::CDSphere() {
}

CDSphere::CDSphere(const CDSphereDesc& d){
	radius = d.radius;
	material = d.material;
	CalcMetrics();
}

// サポートポイントを求める
int CDSphere::Support(Vec3f&w, const Vec3f& v) const{
	float n = v.norm();
	if(n < 1.0e-10f){
		w = Vec3f();
		return -1;
	}
	w = (radius / n) * v;
	return -1;
}

// 切り口を求める. 接触解析を行う.
IntersectionType CDSphere::FindCutRing(CDCutRing& r, const Posed& toW) {
	return SEC_POINT;
}

bool CDSphere::IsInside(const Vec3f& p){
	return p.square() < radius * radius;
}
float CDSphere::CalcVolume(){
	return float(4.0/3.0*M_PI*radius*radius*radius);
}
Matrix3f CDSphere::CalcMomentOfInertia(){
	return (CalcVolume() * (0.4f * radius * radius)) * Matrix3f::Unit();
}
int CDSphere::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset){
	const float eps = 1.0e-10f;
	Vec3f p;
	int num = 0;

	const Vec3f n = origin;	//面の法線 = カメラとshapeの原点を結ぶベクトル
	float tmp = n * dir;	//面の法線とポインタのベクトルとの内積
	if(abs(tmp) < eps)	//内積が小さい場合は判定しない
		return num;

	float s = ((Vec3f(0.0,0.0,0.0) - origin) * n) / tmp; //カメラと面の距離 
	if(s < 0.0)
		return num;
	p = origin + dir * s;	//直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)

	// 円の内部にあるか
	if(p.norm()<GetRadius()){
		//result[num] = p;
		result[num] = Vec3f(0.0,0.0,0.0); //Sphereは中心位置にジョイントを接続した方が使いやすそう
		offset[num] = s;
		num++;
	}
	return num;
}

}	//	namespace Spr

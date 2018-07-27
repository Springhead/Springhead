/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDEllipsoid.h>

namespace Spr{;
CDEllipsoid::CDEllipsoid() {
}
CDEllipsoid::CDEllipsoid(const CDEllipsoidDesc& d) {
	radius = d.radius;
	material = d.material;
}

// サポートポイントを求める
/*
	f(x,y,z) =  x^2/a^2 + y^2/b^2 + z^2 / c^2 = 1
	k n = (fx = 2x/a^2, fy = 2y/b^2, fz = 2z/c^2)

	x = n.x a^2 / 2, y = n.y b^2 / 2, z = 
*/
int CDEllipsoid::Support(Vec3f&w, const Vec3f& v) const {
	float n = v.norm();
	if (n < 1.0e-10f) {
		w = Vec3f();
		return -1;
	}
	Vec3d vn = v / n;
	w.x = vn.x * Square(radius.x);
	w.y = vn.y * Square(radius.y);
	w.z = vn.z * Square(radius.z);
	double scale = Square(w.x) / Square(radius.x) + Square(w.y) / Square(radius.y) + Square(w.z) / Square(radius.z);
	w /= sqrt(scale);
	return -1;
}

// 切り口を求める. 接触解析を行う.
bool CDEllipsoid::FindCutRing(CDCutRing& r, const Posed& toW) {
	return false;
}

bool CDEllipsoid::IsInside(const Vec3f& p) {
	return p.square() < radius * radius;
}
float CDEllipsoid::CalcVolume() {
	return float(4.0 / 3.0*M_PI*radius.x*radius.y*radius.z);
}
Matrix3f CDEllipsoid::CalcMomentOfInertia() {
	Matrix3f rv;
	rv.clear();
	rv[0][0] = CalcVolume() * 0.2f * (Square(radius.y) + Square(radius.z));
	rv[1][1] = CalcVolume() * 0.2f * (Square(radius.z) + Square(radius.x));
	rv[2][2] = CalcVolume() * 0.2f * (Square(radius.x) + Square(radius.y));
	return rv;
}

int CDEllipsoid::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset) {
/*	const float eps = 1.0e-10f;
	Vec3f p;
	int num = 0;

	const Vec3f n = origin;	//面の法線 = カメラとshapeの原点を結ぶベクトル
	float tmp = n * dir;	//面の法線とポインタのベクトルとの内積
	if (abs(tmp) < eps)	//内積が小さい場合は判定しない
		return num;

	float s = ((Vec3f(0.0, 0.0, 0.0) - origin) * n) / tmp; //カメラと面の距離 
	if (s < 0.0)
		return num;
	p = origin + dir * s;	//直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)

							// 円の内部にあるか
	if (p.norm()<GetRadius()) {
		//result[num] = p;
		result[num] = Vec3f(0.0, 0.0, 0.0); //Sphereは中心位置にジョイントを接続した方が使いやすそう
		offset[num] = s;
		num++;
	}
	return num;
	*/
	return 0;	//	TBI
}

}

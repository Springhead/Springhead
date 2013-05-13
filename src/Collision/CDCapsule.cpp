/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDCapsule.h>

namespace Spr{;

//----------------------------------------------------------------------------
//	CDCapsule
CDCapsule::CDCapsule(const CDCapsuleDesc& desc){
	material	= desc.material;
	radius		= desc.radius;
	length		= desc.length;
}

bool CDCapsule::IsInside(const Vec3f& p){
	return (p - Vec3f(0.0f, 0.0f, 0.5f * -length)).square() < radius * radius ||
		   (p - Vec3f(0.0f, 0.0f, 0.5f * +length)).square() < radius * radius ||
		   (p.x*p.x + p.y*p.y < radius * radius && -0.5f*length < p.z && p.z < 0.5f*length);
}
float CDCapsule::CalcVolume(){
	return  2.0f * CDShape::CalcHemisphereVolume(radius) + CDShape::CalcCylinderVolume(radius, length);
}

Matrix3f CDCapsule::CalcMomentOfInertia(){
	/*Matrix3f ans;
	// 下記いずれもリンク切れ
	// http://www12.plala.or.jp/ksp/mechanics/inertiaTable1/
	// http://www.dynamictouch.matrix.jp/tensormodel.php
	ans[0][0] = ((radius * radius)/4.0f + (length*length)/12.0f + 2.0f * 83.0f/320.0f * radius * radius) + length * length / 2.0f;
	ans[0][1] = 0.0f;
	ans[0][2] = 0.0f;
	ans[1][0] = 0.0f;
	ans[1][1] = ((radius * radius)/4.0f + (length*length)/12.0f+ 2.0f * 83.0f/320.0f * radius * radius) + length * length / 2.0f;
	ans[1][2] = 0.0f;
	ans[2][0] = 0.0f;
	ans[2][1] = 0.0f;
	ans[2][2] = (9.0f/5.0f * radius * radius);
	return ans;*/
	Matrix3f I0 = CDShape::CalcCylinderInertia  (radius, length);
	float    V1 = CDShape::CalcHemisphereVolume (radius);
	Matrix3f I1 = CDShape::CalcHemisphereInertia(radius);
	float    c1 = CDShape::CalcHemisphereCoM    (radius);
	float    halfl  = length / 2.0f;
	float    offset = halfl*(halfl + 2.0f*c1);

	return I0 + 2.0f * (I1 + V1 * Matrix3f::Diag(offset, offset, 0.0f));
}
	
// サポートポイントを求める
int CDCapsule::Support(Vec3f& w, const Vec3f& v) const {
	float n = v.norm();
	if(n < 1.0e-10f){
		w = Vec3f();
	}else{
		w = (radius / n) * v;
	}
	if (v.Z() >= 0){
		w.Z() += length*0.5f;
	}else{
		w.Z() -= length*0.5f;
	}
	return -1;
}


// 切り口を求める. 接触解析を行う.
bool CDCapsule::FindCutRing(CDCutRing& ring, const Posed& toW) {
	//	切り口(ring.local)系での カプセルの向き
	Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0,0,1);
	Vec3f center = ring.localInv * toW.Pos();
	float sign = center.X() > 0.0f ? 1.0f : -1.0f;
	if (dir.X()*sign < 0) dir = -dir;
	center -= sign*dir * length/2;
	
	if (dir.X()*sign < 0.1f){		//	カプセルが接触面に大体平行な場合
		float shrink = sqrt(1-dir.X()*dir.X());	//	傾いているために距離が縮む割合
		float start = -0.0f*length*shrink;
		float end = 1.0f*length*shrink;
		if (dir.X()*sign > 1e-4){	//	完全に平行でない場合
			float is = (radius/shrink-sign*center.X()) / (sign*dir.X()) * shrink;	//	接触面と中心線を半径ずらした線との交点
			if (start<= is && is <= end) end = is;
			else return false;
		}
		//	ringに線分を追加
		float lenInv = 1/sqrt(dir.Y()*dir.Y() + dir.Z()*dir.Z());
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Y(), -dir.Z())*lenInv, -start));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Y(), dir.Z())*lenInv, end));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Z(), -dir.Y())*lenInv, 0));
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Z(), dir.Y())*lenInv, 0));
		return true;
	}else{
		return false;
	}
}

int CDCapsule::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset){
	const float eps = 1.0e-10f;
	Vec3f p;
	int num = 0;
	Vec3f sCenter[2];	//球の中心位置
	sCenter[0] = Vec3f(0.0f,0.0f,length);
	sCenter[1] = Vec3f(0.0f,0.0f,-length);

	//球部分の判定
	for(int i=0; i<2; i++){
		const Vec3f n = sCenter[i] - origin;		 //面の法線 = カメラと球の原点を結ぶベクトル
		float tmp = n * dir;						 //面の法線とポインタのベクトルとの内積
		if(abs(tmp) < eps)							 //内積が小さい場合は判定しない
			continue;
		float s = ((sCenter[i] - origin) * n) / tmp; //カメラと面の距離 
		if(s < 0.0)
			continue;
		p = origin + dir * s;						 //直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)
		Vec3f po = p-sCenter[i];					 //球の中心を原点とした時の交点の位置
		// 円の内部にあるか
		if(po.norm()<GetRadius()){
			result[num] = p;
			offset[num] = s;
			num++;
		}
	}
	//円柱部分の判定
	const Vec3f n =  origin;						 //カメラ方向への垂直な断面の法線 = カメラとshapeの原点を結ぶベクトル
	float tmp = n * dir;							 //面の法線とポインタのベクトルとの内積

	for(int i=0; i<1; i++){
		if(abs(tmp) < eps)							 //内積が小さい場合は判定しない
			continue;
		float s = ((- origin) * n) / tmp;			 //カメラと面の距離 
		if(s < 0.0)
			continue;
		p = origin + dir * s;						 //直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)
		
		Vec2f pr = Vec2f(p.x,p.y);					 //xy平面のp
		// 円柱の内部にあるか
		if(pr.norm()<GetRadius() && abs(p.z)<=GetLength()*0.5){
			result[num] = p;
			offset[num] = s;
			num++;
		}
	}
	return num;
}

}	//	namespace Spr

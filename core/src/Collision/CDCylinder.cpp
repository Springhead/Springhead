/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

/* made by KIM SAE YOUNG */
/* 円筒の実現のために、円筒のディスクリプタとなります */

#include <Collision/CDCylinder.h>

namespace Spr {
	const double weak_epsilon = 1.0e-4;
	//----------------------------------------------------------------------------
	//	CDCylinder
	CDCylinder::CDCylinder(const CDCylinderDesc& desc) {
		material = desc.material;
		radius = desc.radius;
		length = desc.length;
	}

	// CDCylinderのVolume計算
	float CDCylinder::CalcVolume() {
		return  CDShape::CalcCylinderVolume(radius, length);
	}

	Matrix3f CDCylinder::CalcMomentOfInertia() {

		Matrix3f I0 = CDShape::CalcCylinderInertia(radius, length);

		return I0;
	}

	// サポートポイントを求める
	int CDCylinder::Support(Vec3f& w, const Vec3f& v) const {
		// 中心から座標系が出てくるとして計算
		// CylinderのCenterから見たときに座標系（z軸はLengthに沿って）
		Vec3f m = Vec3f(0.0f, 0.0f, 0.0f);

		//サポート探すときにはx,y座標だけで考える
		//zは後で、阿知の範囲を見て足す
		m.X() = v.X();
		m.Y() = v.Y();
		float p = v.Z();
		float n = sqrt(Square(v.X()) + Square(v.Y()));
		float sign = 0.0f;
		sign = v.Z() > 0.0f ? 1.0f : -1.0f;

		if (n < 1.0e-10f) {
			if (p <= -1.0e-10f) {
				w = Vec3f(0.0f, 0.0f, 0.0f);
				w.Z() = -length * 0.5f;
			}
			else if (p < 1.0e-10f){
				w = Vec3f(0.0f, 0.0f, 0.0f);
			}
			else{
				w = Vec3f(0.0f, 0.0f, 0.0f);
				w.Z() = length * 0.5f;
			}
		}
		else {
			if (p <= -1.0e-10f) {
				w = (radius / n) * m;
				w.Z() = (-1) * 0.5f * length;
			}
			else if (p < 1.0e-10f) {
				w = (radius / n) * m;
			}
			else {
				w = (radius / n) * m;
				w.Z() = 0.5f * length;
			}
		}

		return -1;

	}

	// サポートポイントがどこにあるかを求める
	int CDCylinder::SupportTag(const Vec3f& v, std::vector<Vec3d>& c_vec) const {
		// 中心から座標系が出てくるとして計算
		// CylinderのCenterから見たときに座標系（z軸はLengthに沿って）
		Vec3f m = Vec3f(0.0f, 0.0f, 0.0f);
		Vec3d w = Vec3d();

		//サポート探すときにはx,y座標だけで考える
		//zは後で、阿知の範囲を見て足す
		m.X() = v.X();
		m.Y() = v.Y();
		float p = v.Z();
		float n = m.norm();
		float sign = 0.0f;
		sign = v.Z() > 0.0f ? 1.0f : -1.0f;

		if (n < 1.0e-2f) {
			if (p <= -1.0e-10f) {
				w = Vec3d(0.0f, 0.0f, 0.0f);
				w.Z() = -length * 0.5f;
				c_vec.push_back(w);
				return 3;
			}
			else if (p < 1.0e-10f) {
				w = Vec3d(0.0f, 0.0f, 0.0f);
				c_vec.push_back(w);
				return 0; // NONE
			}
			else {
				w = Vec3d(0.0f, 0.0f, 0.0f);
				w.Z() = length * 0.5f;
				c_vec.push_back(w);
				return 3; // CIRCLE_FACE
			}
		}
		else {
			if (p <= -1.0e-10f) {
				w = (radius / n) * m;
				w.Z() = (-1) * 0.5f * length;
				c_vec.push_back(w);
				return 1; // EDGE
			}
			else if (p < 1.0e-10f) {
				w = Vec3f(0.0f, 0.0f, 0.0f);
				c_vec.push_back(w);
				return 2; // Z-EDGE
			}
			else {
				w = (radius / n) * m;
				w.Z() = 0.5f * length;
				c_vec.push_back(w);
				return 1;
			}
		}
	}

	// 切り口を求める. 接触解析を行う.
	IntersectionType CDCylinder::FindCutRing(CDCutRing& ring, const Posed& toW) {
		//	切り口(ring.local)系での円筒の向き
		Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0, 0, 1); // 円盤がz軸として定義されているため
		Vec3f center = ring.localInv * toW.Pos();
		float sign = center.X() > 0.0f ? 1.0f : -1.0f;
		if (dir.X()*sign < 0) dir = -dir;
		center -= sign*dir * length / 2;

		if (dir.X()*sign < 0.1f) {		//	円筒が接触面に大体平行な場合
			float shrink = sqrt(1 - dir.X()*dir.X());	//	傾いているために距離が縮む割合
			float start = -0.0f*length*shrink;
			float end = 1.0f*length*shrink;
			if (dir.X()*sign > 1e-4) {	//	完全に平行でない場合
				float is = (radius / shrink - sign*center.X()) / (sign*dir.X()) * shrink;	//	接触面と中心線を半径ずらした線との交点
				if (start <= is && is <= end) end = is;
				else return SEC_POINT;
			}
			//	ringに線分を追加
			float lenInv = 1 / sqrt(dir.Y()*dir.Y() + dir.Z()*dir.Z());
			ring.lines.push_back(CDCutLine(Vec2f(-dir.Y(), -dir.Z())*lenInv, -start));
			ring.lines.push_back(CDCutLine(Vec2f(dir.Y(), dir.Z())*lenInv, end));
			ring.lines.push_back(CDCutLine(Vec2f(dir.Z(), -dir.Y())*lenInv, 0));
			ring.lines.push_back(CDCutLine(Vec2f(-dir.Z(), dir.Y())*lenInv, 0));
			return SEC_POLYGON;
		}
		else if (dir.X() * sign > 0.8f) {		//	円筒が接触面に大体垂直な場合
			return SEC_CURVETURE;
		}
		else {
			return SEC_POINT;
		}
	}

	int CDCylinder::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset) {
		const float eps = 1.0e-10f;
		Vec3f p;
		int num = 0;
		Vec3f sCenter[2];	//球の中心位置
		sCenter[0] = Vec3f(0.0f, 0.0f, length);
		sCenter[1] = Vec3f(0.0f, 0.0f, -length);

		//球部分の判定
		for (int i = 0; i<2; i++) {
			const Vec3f n = sCenter[i] - origin;		 //面の法線 = カメラと球の原点を結ぶベクトル
			float tmp = n * dir;						 //面の法線とポインタのベクトルとの内積
			if (abs(tmp) < eps)							 //内積が小さい場合は判定しない
				continue;
			float s = ((sCenter[i] - origin) * n) / tmp; //カメラと面の距離 
			if (s < 0.0)
				continue;
			p = origin + dir * s;						 //直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)
			Vec3f po = p - sCenter[i];					 //球の中心を原点とした時の交点の位置
														 // 円の内部にあるか
			if (po.norm()<GetRadius()) {
				result[num] = p;
				offset[num] = s;
				num++;
			}
		}
		//円柱部分の判定
		const Vec3f n = origin;						 //カメラ方向への垂直な断面の法線 = カメラとshapeの原点を結ぶベクトル
		float tmp = n * dir;							 //面の法線とポインタのベクトルとの内積

		for (int i = 0; i<1; i++) {
			if (abs(tmp) < eps)							 //内積が小さい場合は判定しない
				continue;
			float s = ((-origin) * n) / tmp;			 //カメラと面の距離 
			if (s < 0.0)
				continue;
			p = origin + dir * s;						 //直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)

			Vec2f pr = Vec2f(p.x, p.y);					 //xy平面のp
														 // 円柱の内部にあるか
			if (pr.norm()<GetRadius() && abs(p.z) <= GetLength()*0.5) {
				result[num] = p;
				offset[num] = s;
				num++;
			}
		}
		return num;
	}

	Vec3d CDCylinder::GetPointA()
	{
		Vec3d w = Vec3d();
		w.Z() = -length * 0.5;
		return w;
	}

	Vec3d CDCylinder::GetPointB()
	{
		Vec3d w = Vec3d();
		w.Z() = length * 0.5;
		return w;
	}

	bool CDCylinder::IsInside(const Vec3f& p) {
		if (-0.5 * (length + weak_epsilon) <= p.z && p.z <= 0.5 * (length + weak_epsilon))
			if (p.x * p.x + p.y * p.y <= (radius + weak_epsilon) * (radius + weak_epsilon))
				return true;

		return false;
	}

}	//	namespace Spr

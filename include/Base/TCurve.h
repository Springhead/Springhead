/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef TCURVE_H
#define TCURVE_H

#include <algorithm>
#include <tuple>
#include <vector>
#ifndef	_WIN32
  #include <float.h>
#endif

namespace Spr{;

struct Interpolate{
	enum{
		Constant,		//< 定数補間				区間始点の値を区間中で保持．微分値は0
		LinearDiff,		//< 線形補間(微分)		区間始点と終点の位置を線形に補間
		LinearInt,		//< 線形補間(積分)		区間始点の位置と速度で線形に補間
		Quadratic,		//< 2次補間				区間始点の値を微分値，区間終点の値をもとに係数を決定
		Cubic,			//< 3次補間				区間始点と終点の値と微分値をもとに係数を決定
		SlerpDiff,		//< 球面線形補間(微分)	区間始点と終点の向きを線形補間
		SlerpInt,		//< 球面線形補間(積分)	区間始点の向きと角速度で線形補間
		Squad,			//< 球面3次補間
	};
};

/**
	補間関数
 **/
template<class P, class V, class T>
inline P InterpolatePos(T t, T t0, P p0, V v0, T t1, P p1, V v1, int type){
	if(type == Interpolate::Constant)
		return p0;
		
	T h = t1 - t0;
	const T eps = 1.0e-12;
	if(h < eps)
		return p0;
		
	if(type == Interpolate::LinearInt)
		return p0 + (t - t0) * v0;

	T s = (t - t0)/h;
	if(type == Interpolate::LinearDiff)
		return (1.0 - s)*p0 + s*p1;
		
	T s2 = s*s;
	if(type == Interpolate::Quadratic)
		return (1.0 - s2) * p0 + s2 * p1 + ((s - s2) * h) * v0;

	T s3 = s2*s;
	if(type == Interpolate::Cubic)
		return (1.0 - 3.0*s2 + 2.0*s3) * p0 + ((s - 2.0*s2 + s3)*h) * v0 + (3.0*s2 - 2.0*s3) * p1 + ((-s2 + s3)*h) * v1;
		
	return P();
}

template<class P, class V, class T>
inline V InterpolateVel(T t, T t0, P p0, V v0, T t1, P p1, V v1, int type){
	if(type == Interpolate::Constant)
		return  V();

	if(type == Interpolate::LinearInt)
		return v0;
				
	T h = t1 - t0;
	const T eps = 1.0e-12;
	if(h < eps)
		return V();

	if(type == Interpolate::LinearDiff)
		return (p1 - p0)/h;

	T s = (t - t0)/h;

	if(type == Interpolate::Quadratic)
		return (2*s/h) * (p1 - p0) + (1 - 2*s) * v0;

	T s2 = s*s;
	if(type == Interpolate::Cubic)
		return (6.0*(s - s2)/h) * (p1 - p0) + (1.0 - 4*s + 3*s2) * v0 + (-2*s + 3*s2) * v1;

	return V();
}

template<class P, class V, class T>
inline V InterpolateAcc(T t, T t0, P p0, V v0, T t1, P p1, V v1, int type){
	if(type == Interpolate::Constant)
		return V();
	if(type == Interpolate::LinearInt)
		return V();
	if(type == Interpolate::LinearDiff)
		return V();
				
	T h = t1 - t0;
	T h2 = h*h;
	const T eps = 1.0e-12;
	if(h < eps)
		return V();

	T s = (t - t0)/h;

	if(type == Interpolate::Quadratic)
		return (2/h2) * (p1 - p0) - (2/h) * v0;

	T s2 = s*s;
	if(type == Interpolate::Cubic)
		return (6.0*(1 - 2*s)/h2) * (p1 - p0) + ((-4 + 6*s)/h) * v0 + ((-2 + 6*s)/h) * v1;

	return V();
}

template<class P, class V, class T>
inline P InterpolateOri(T t, T t0, P p0, V v0, T t1, P p1, V v1, int type){
	if(type == Interpolate::Constant)
		return p0;
		
	T h = t1 - t0;
	const T eps = 1.0e-12;
	if(h < eps)
		return p0;
		
	// 角速度のノルムの軸を求め，軸まわりに(ノルム*経過時間)回転する
	if(type == Interpolate::SlerpInt){
		V w = v0;
		T wnorm = w.norm();
		if(wnorm < eps)
			return p0;
		V axis = w/wnorm;
		return P::Rot(wnorm * (t - t0), axis) * p0;	//< 角速度がグローバル座標なので左からかけるのに注意
	}

	T s = (t - t0)/h;
	// 相対quaternionを求め，その軸まわりに定速回転
	if(type == Interpolate::SlerpDiff){
		P qrel = p0.Conjugated() * p1;
		V axis = qrel.Axis();
		T angle = qrel.Theta();
		return p0 * P::Rot(s * angle, axis);			//< こちらは右からかける
	}
		
	// SQUADは未実装

	return P();
}

template<class P, class V, class T>
inline V InterpolateAngvel(T t, T t0, P p0, V v0, T t1, P p1, V v1, int type){
	if(type == Interpolate::Constant)
		return V();

	if(type == Interpolate::SlerpInt)
		return v0;
				
	T h = t1 - t0;
	const T eps = 1.0e-12;
	if(h < eps)
		return V();

	if(type == Interpolate::SlerpDiff){
		// 相対quaternionから角速度を求める
		P qrel = p0.Conjugated() * p1;
		V w = (qrel.Theta() / h) * qrel.Axis();
		// グローバル座標に変換
		return p0 * w;
	}

	// SQUADは未実装
		
	return V();
}

template<class P, class V, class T>
inline V InterpolateAngacc(T t, T t0, P p0, V v0, T t1, P p1, V v1, int type){
	if(type == Interpolate::Constant)
		return V();
	if(type == Interpolate::SlerpInt)
		return V();
	if(type == Interpolate::SlerpDiff)
		return V();
	
	// SQUADは未実装	
	return V();
}

/**
	曲線クラス
 **/

template<class P, class V, class T>
class TCurve{
public:
	typedef P	pos_t;
	typedef V	vel_t;
	typedef T	real_t;

protected:
	struct Point{
		int		id;		//
		real_t	t;
		pos_t	pos;
		vel_t	vel;

		Point(int _id, real_t _t):id(_id), t(_t){}
	};

	int	type;

	// 点の配列．常にtの昇順に並ぶ．
	std::vector<struct Point>	points;

	struct CompByTime{
		bool operator()(const struct Point& lhs, const struct Point& rhs){
			return lhs.t < rhs.t;
		}
	};
	struct CompByID{
		bool operator()(const struct Point& lhs, const struct Point& rhs){
			return lhs.id < rhs.id;
		}
	};
	struct CheckID{
		int id;
		bool operator()(const struct Point& p){
			return p.id == id;
		}
		CheckID(int _id):id(_id){}
	};

	// t順にソート
	void SortPoints(){
		std::sort(points.begin(), points.end(), CompByTime());
	}
	
public:
	void SetType(int t){ type = t; }
	int  GetType()const{ return type; }
	
	/** @brief 点を追加する
		@param	t	時刻
		@return	ID	追加された点のID
		点の順序は点の追加・削除や時刻の変更により変化するが，IDは不変．
	 **/
	int AddPoint(real_t t){
		int id;
		if(points.empty())
			 id = 0;
		else id = std::max_element(points.begin(), points.end(), CompByID())->id + 1;
		points.push_back(Point(id, t));
		SortPoints();
		return id;
	}

	/// IDからインデックスを取得
	int	IndexFromID(int id){
		typename std::vector<struct Point>::iterator it = find_if(points.begin(), points.end(), CheckID(id));
		if(it == points.end())
			return -1;
		return it - points.begin();
	}

	void RemovePoint(int idx){
		if(idx < points.size())
		points.erase(points.begin() + idx);
	}

	size_t	NPoints()const{ return points.size(); }

	real_t GetTime(int idx)const{
		return points[idx].t;
	}
	void SetTime(int idx, real_t t){
		points[idx].t = t;
		SortPoints();
	}

	pos_t GetPos(int idx)const{
		return points[idx].pos;
	}
	void SetPos(int idx, pos_t p){
		points[idx].pos = p;
	}

	vel_t GetVel(int idx)const{
		return points[idx].vel;
	}
	void SetVel(int idx, vel_t v){
		points[idx].vel = v;
	}

	std::pair<int, int>	GetSegment(real_t t)const{
		if(points.size() < 2)
			return std::make_pair(0, 0);
		int idx = 0;
		while(idx < (int)points.size()-1 && points[idx+1].t < t)
			idx++;
		if(idx == points.size()-1)
			return std::make_pair(idx, idx);
		return std::make_pair(idx, idx+1);
	}

	int	GetNearest(real_t t)const{
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		return (t - points[i0].t < points[i1].t - t) ? i0 : i1;
	}

};

/**
	R^n上の曲線
 **/
template<class V, class T>
class TCurveEuclid : public TCurve<V, V, T>{
public:
	typedef V	pos_t;
	typedef V	vel_t;
	typedef T	real_t;

	pos_t	CalcPos(real_t t){
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		struct Point& p0 = TCurve<V,V,T>::points[i0];
		struct Point& p1 = TCurve<V,V,T>::points[i1];
		return InterpolatePos(t, p0.t, p0.pos, p0.vel, p1.t, p1.pos, p1.vel, TCurve<V,V,T>::type);
	}

	vel_t	CalcVel(real_t t){
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		struct Point& p0 = TCurve<V,V,T>::points[i0];
		struct Point& p1 = TCurve<V,V,T>::points[i1];
		return InterpolateVel(t, p0.t, p0.pos, p0.vel, p1.t, p1.pos, p1.vel, TCurve<V,V,T>::type);
	}

	vel_t	CalcAcc(real_t t){
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		struct Point& p0 = TCurve<V,V,T>::points[i0];
		struct Point& p1 = TCurve<V,V,T>::points[i1];
		return InterpolateAcc(t, p0.t, p0.pos, p0.vel, p1.t, p1.pos, p1.vel, TCurve<V,V,T>::type);
	}

	TCurveEuclid(){
		TCurve<V,V,T>::type = Interpolate::LinearDiff;
	}
};

/**
	Quaternionの補間
	- 角速度はグローバル座標で指定する
 **/
template<class T>
class TCurveQuat : public TCurve<TQuaternion<T>, TVec3<T>, T>{
public:
	typedef TQuaternion<T>	pos_t;
	typedef TVec3<T>	vel_t;
	typedef T	real_t;

	pos_t	CalcPos(real_t t){
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		struct Point& p0 = TCurve<TQuaternion<T>,TVec3<T>,T>::points[i0];
		struct Point& p1 = TCurve<TQuaternion<T>,TVec3<T>,T>::points[i1];
		return InterpolateOri(t, p0.t, p0.pos, p0.vel, p1.t, p1.pos, p1.vel, TCurve<TQuaternion<T>,TVec3<T>,T>::type);
	}

	vel_t	CalcVel(real_t t){
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		struct Point& p0 = TCurve<TQuaternion<T>,TVec3<T>,T>::points[i0];
		struct Point& p1 = TCurve<TQuaternion<T>,TVec3<T>,T>::points[i1];
		return InterpolateAngvel(t, p0.t, p0.pos, p0.vel, p1.t, p1.pos, p1.vel, TCurve<TQuaternion<T>,TVec3<T>,T>::type);
	}

	vel_t	CalcAcc(real_t t){
		int i0, i1;
		std::tie(i0, i1) = GetSegment(t);
		struct Point& p0 = TCurve<TQuaternion<T>,TVec3<T>,T>::points[i0];
		struct Point& p1 = TCurve<TQuaternion<T>,TVec3<T>,T>::points[i1];
		return InterpolateAngacc(t, p0.t, p0.pos, p0.vel, p1.t, p1.pos, p1.vel, TCurve<TQuaternion<T>,TVec3<T>,T>::type);
	}

	TCurveQuat(){
		TCurve<TQuaternion<T>,TVec3<T>,T>::type = SLERP_DIFF;
	}
};

typedef TCurveEuclid<float, float>		Curvef;
typedef TCurveEuclid<double, double>	Curved;
typedef TCurveEuclid<Vec2f, float>		Curve2f;
typedef TCurveEuclid<Vec2d, double>		Curve2d;
typedef TCurveEuclid<Vec3f, float>		Curve3f;
typedef TCurveEuclid<Vec3d, double>		Curve3d;
typedef TCurveEuclid<Vec4f, float>		Curve4f;
typedef TCurveEuclid<Vec4d, double>		Curve4d;
typedef TCurveQuat<float>				QuatCurvef;
typedef TCurveQuat<double>				QuatCurved;

}

#endif

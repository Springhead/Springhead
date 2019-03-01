/*
*  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef CRMINJERKTRAJECTORY_H
#define CRMINJERKTRAJECTORY_H

#include <Foundation/SprObject.h>
#include <Base/Spatial.h>

namespace Spr { ;

// 躍度最小軌道の基本クラス
// TP : 位置の型
// TV : 速度、加速度の型
template<class TP, class TV>
class CRMinimumJerkTrajectory {
public:
	// 開始時間
	float startTime;
	// 到達目標時間
	float goalTime;
	
	// 開始点の条件設定
	virtual void SetStart(TP sp, TV sv, TV sa, float st) {
		SetStartPosition(sp);
		SetStartVelocity(sv);
		SetStartAcceralation(sa);
		SetStartTime(st);
	}
	virtual void SetStartPosition(TP sp) {}
	virtual void SetStartVelocity(TV sv) {}
	virtual void SetStartAcceralation(TV sa) {}
	virtual void SetStartTime(float st) {}

	// 到達目標点の条件設定
	virtual void SetGoal(TP gp, TV gv, TV ga, float gt) {
		SetGoalPosition(gp);
		SetGoalVelocity(gv);
		SetGoalAcceralation(ga);
		SetGoalTime(gt);
	}
	virtual void SetGoalPosition(TP gp) {}
	virtual void SetGoalVelocity(TV gv) {}
	virtual void SetGoalAcceralation(TV ga) {}
	virtual void SetGoalTime(float gt) {}

	// 位置の取得
	virtual TP GetPosition(float t) { return TP(); }
	// 速度の取得
	virtual TV GetVelocity(float t) { return TV(); }
	// 開始点との位置差分の取得
	virtual TP GetDeltaPosition(float t) { return TP(); }
};

// 位置と速度の型が同じ場合の躍度最小軌道
template<class T>
class CRNDMinimumJerkTrajectory : public CRMinimumJerkTrajectory<T, T>{
protected:
public:
	// 開始点の位置、速度、加速度
	T startPosition; 
	T startVelocity;
	T startAcceralation;

	// 到達目標点の位置、速度、加速度
	T goalPosition;
	T goalVelocity;
	T goalAcceralation;

	// 位置速度計算の係数
	PTM::TVector<6, T> coefficient;

	// 現在の条件に基づく係数の生成
	void CompCoefficient();

	CRNDMinimumJerkTrajectory() {};

	virtual void SetStartPosition(T sp) { this->startPosition = sp; CompCoefficient(); }
	virtual void SetStartVelocity(T sv) { this->startVelocity = sv; CompCoefficient(); }
	virtual void SetStartAcceralation(T sa) { this->startAcceralation = sa; CompCoefficient(); }
	virtual void SetStartTime(float st) { this->startTime = st; CompCoefficient(); }

	virtual void SetGoalPosition(T gp) { this->goalPosition = gp; CompCoefficient(); }
	virtual void SetGoalVelocity(T gv) { this->goalVelocity = gv; CompCoefficient(); }
	virtual void SetGoalAcceralation(T ga) { this->goalAcceralation = ga; CompCoefficient(); }
	virtual void SetGoalTime(float gt) { this->goalTime = gt; CompCoefficient(); }

	virtual T GetPosition(float t);
	virtual T GetVelocity(float t);
	virtual T GetDeltaPosition(float t);
};

typedef CRNDMinimumJerkTrajectory<double> CRAngleMinimumJerkTrajectory;

typedef CRNDMinimumJerkTrajectory<Vec3d> CRPositionMinimumJerkTrajectory;
typedef CRNDMinimumJerkTrajectory<Vec3d> CREulerAngleMinimumJerkTrajectory;

// クオータニオン用の躍度最小軌道
class CRQuaternionMinimumJerkTrajectory : public CRMinimumJerkTrajectory<Quaterniond, Vec3d>{
protected:
	// Quaternion成分用の係数
	CRNDMinimumJerkTrajectory<double> positionCoefficient;
	// オイラー角での角速度成分
	CREulerAngleMinimumJerkTrajectory velocityTrajectory;
public:
	// 開始姿勢、到達目標姿勢
	Quaterniond startPosition, goalPosition;

	CRQuaternionMinimumJerkTrajectory() {
		positionCoefficient = CRNDMinimumJerkTrajectory<double>();
		positionCoefficient.SetStart(0.0, 0.0, 0.0, 0.0);
		positionCoefficient.SetGoal(1.0, 0.0, 0.0, 1.0);
	}

	virtual void SetStartPosition(Quaterniond sp) { this->startPosition = sp; }
	virtual void SetStartVelocity(Vec3d sv) { this->velocityTrajectory.SetStartVelocity(sv); }
	virtual void SetStartAcceralation(Vec3d sa) { this->velocityTrajectory.SetStartAcceralation(sa); }
	virtual void SetStartTime(float st) { this->startTime = st; this->velocityTrajectory.SetStartTime(st); }

	virtual void SetGoalPosition(Quaterniond gp) { this->goalPosition = gp; }
	virtual void SetGoalVelocity(Vec3d gv) { this->velocityTrajectory.SetGoalVelocity(gv); }
	virtual void SetGoalAcceralation(Vec3d ga) { this->velocityTrajectory.SetGoalAcceralation(ga); }
	virtual void SetGoalTime(float gt) { this->goalTime = gt; this->velocityTrajectory.SetGoalTime(gt); }

	virtual Quaterniond GetPosition(float t);
	virtual Vec3d GetVelocity(float t);
	virtual Quaterniond GetDeltaPosition(float t);
};

// Posed用の躍度最小軌道
class CRPoseMinimumJerkTrajectory : public CRMinimumJerkTrajectory<Posed, SpatialVector>{
protected:
	// Posedの位置成分の軌道
	CRPositionMinimumJerkTrajectory positionTrajectory;
	// Posedの姿勢成分の軌道
	CRQuaternionMinimumJerkTrajectory orientationTrajectory;
public:

	CRPoseMinimumJerkTrajectory() {};

	virtual void SetStartPosition(Posed sp) { this->positionTrajectory.SetStartPosition(sp.Pos()); this->orientationTrajectory.SetStartPosition(sp.Ori()); }
	virtual void SetStartVelocity(SpatialVector sv) { this->positionTrajectory.SetStartVelocity(sv.v()); this->orientationTrajectory.SetStartVelocity(sv.w()); }
	virtual void SetStartAcceralation(SpatialVector sa) { this->positionTrajectory.SetStartAcceralation(sa.v()); this->orientationTrajectory.SetStartAcceralation(sa.w()); }
	virtual void SetStartTime(float st) { this->positionTrajectory.SetStartTime(st); this->orientationTrajectory.SetStartTime(st); }

	virtual void SetGoalPosition(Posed gp) { this->positionTrajectory.SetGoalPosition(gp.Pos()); this->orientationTrajectory.SetGoalPosition(gp.Ori()); }
	virtual void SetGoalVelocity(SpatialVector gv) { this->positionTrajectory.SetGoalVelocity(gv.v()); this->orientationTrajectory.SetGoalVelocity(gv.w()); }
	virtual void SetGoalAcceralation(SpatialVector ga) { this->positionTrajectory.SetGoalAcceralation(ga.v()); this->orientationTrajectory.SetGoalAcceralation(ga.w()); }
	virtual void SetGoalTime(float gt) { this->positionTrajectory.SetGoalTime(gt); this->orientationTrajectory.SetGoalTime(gt); }

	virtual Posed GetPosition(float t);
	virtual SpatialVector GetVelocity(float t);
	virtual Posed GetDeltaPosition(float t);
};

class CRPoseViaPointsMinimumJerkTrajectory{
protected:
	Posed startPosition;
	SpatialVector startVelocity;
	SpatialVector startAcceralation;
	float startTime;

	std::vector<CRPoseMinimumJerkTrajectory> subs;
public:
	CRPoseViaPointsMinimumJerkTrajectory() {};
	~CRPoseViaPointsMinimumJerkTrajectory()
	{
		for (int i = 0; i < subs.size(); i++) {
			//
		}
		subs.resize((size_t)0);
		std::vector<CRPoseMinimumJerkTrajectory>().swap(subs);
	}

	virtual void SetStartPosition(Posed sp) { subs[0].SetStartPosition(sp); }
	virtual void SetStartVelocity(SpatialVector sv) { subs[0].SetStartVelocity(sv); }
	virtual void SetStartAcceralation(SpatialVector sa) { subs[0].SetStartAcceralation(sa); }
	virtual void SetStartTime(float st) { subs[0].SetStartTime(st); }
	
	virtual void Add(Posed vp, SpatialVector vv, SpatialVector va, float vt) {
		CRPoseMinimumJerkTrajectory sub = CRPoseMinimumJerkTrajectory();
		subs.push_back(sub);
	}

	virtual void Recalc();
};

}

#endif
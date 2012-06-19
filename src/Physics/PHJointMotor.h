/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_JOINT_MOTOR_H
#define PH_JOINT_MOTOR_H

#include <Physics/SprPHJoint.h>
#include <Physics/PhysicsDecl.hpp>
#include <Physics/PHSpatial.h>

namespace Spr{;

template<int NDOF> class PHNDJoint;

class PHJoint;
class PH1DJoint;
class PHBallJoint;
class PHSpring;

///	パラメータ一覧（1D/3D/6Dを共通で扱えるようにするためのデスクリプタもどき）
template<int NDOF>
struct PHNDJointMotorParam {
	// State
	PTM::TVector<NDOF,double> fAvg;
	SpatialVector             xs;
	bool                      bYielded;
	// Desc
	PTM::TVector<NDOF,double> spring;
	PTM::TVector<NDOF,double> damper;
	PTM::TVector<NDOF,double> secondDamper;
	PTM::TVector<NDOF,double> targetVelocity;
	PTM::TVector<NDOF,double> offsetForce;
	double yieldStress;
	double hardnessRate;
};

///	N自由度関節の関節コントローラ
template<int NDOF>
class PHNDJointMotor {
public:
	typedef PTM::TVector<NDOF,double> VecNd;

	/// コントロールの対象となる関節
	PHJoint* joint;

	///< 現在のばね部の距離（三要素モデル用）
	SpatialVector newXs;

	/// コンストラクタ
	PHNDJointMotor() {}

	// ----- Motorの機能

	/// 拘束軸を決定する
	virtual void SetupAxisIndex();

	/// dA, dbを計算する
	virtual void CompBias();

	// ----- このクラスの機能

	/// 弾性変形用のCompBias
	void CompBiasElastic();

	/// 塑性変形用のCompBias
	void CompBiasPlastic();

	/// 降伏したかどうか
	void CheckYielded();

	// ----- 派生クラスで実装する機能
	/// propVを計算する
	virtual PTM::TVector<NDOF,double> GetPropV() { return PTM::TVector<NDOF,double>(); }

	/// パラメータを取得する
	virtual void GetParams(PHNDJointMotorParam<NDOF>& p) {}

	/// パラメータを反映する
	virtual void SetParams(PHNDJointMotorParam<NDOF>& p) {}
};

///	1自由度関節の関節コントローラ
class PH1DJointMotor : public PHNDJointMotor<1> {
public:
	/// コンストラクタ
	PH1DJointMotor() {}

	// ----- PHNDJointMotorの派生クラスで実装する機能
	/// propVを計算する
	virtual PTM::TVector<1,double> GetPropV();

	/// パラメータを取得する
	virtual void GetParams(PHNDJointMotorParam<1>& p);

	/// パラメータを反映する
	virtual void SetParams(PHNDJointMotorParam<1>& p);
};

///	球関節の関節コントローラ
class PHBallJointMotor : public PHNDJointMotor<3> {
public:
	/// コンストラクタ
	PHBallJointMotor() {}

	// ----- PHNDJointMotorの派生クラスで実装する機能
	/// propVを計算する
	virtual PTM::TVector<3,double> GetPropV();

	/// パラメータを取得する
	virtual void GetParams(PHNDJointMotorParam<3>& p);

	/// パラメータを反映する
	virtual void SetParams(PHNDJointMotorParam<3>& p);
};

///	バネダンパのコントローラ
class PHSpringMotor : public PHNDJointMotor<6> {
public:
	/// コンストラクタ
	PHSpringMotor() {}

	// ----- PHNDJointMotorの派生クラスで実装する機能
	/// propVを計算する
	virtual PTM::TVector<6,double> GetPropV();

	/// パラメータを取得する
	virtual void GetParams(PHNDJointMotorParam<6>& p);

	/// パラメータを反映する
	virtual void SetParams(PHNDJointMotorParam<6>& p);
};

}

#endif//PH_JOINT_MOTOR_H

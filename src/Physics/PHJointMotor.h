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
#include <Physics/SprPHJointMotor.h>
#include <Physics/PhysicsDecl.hpp>
#include <Physics/PHConstraint.h>

namespace Spr{;

template<int NDOF> class PHNDJoint;

class PHJoint;
class PH1DJoint;
class PHBallJoint;
class PHSpring;

///	パラメータ一覧（1D/3D/6Dを共通で扱えるようにするためのデスクリプタもどき）
template<int NDOF>
class PHNDJointMotorParam {
public:
	typedef PTM::TVector<NDOF,double> VecNd;

	// State
	VecNd			fAvg;
	SpatialVector   xs;
	bool            bYielded;
	// Desc
	VecNd           spring;
	VecNd           damper;
	VecNd           secondDamper;
	VecNd           targetVelocity;
	VecNd           offsetForce;
	double          yieldStress;
	double          hardnessRate;
};

///	N自由度関節の関節コントローラ
template<int NDOF>
class PHNDJointMotor : public PHConstraintBase{
public:
	typedef PTM::TVector<NDOF,double> VecNd;

	/// コントロールの対象となる関節
	PHJoint* joint;

	VecNd	fMinDt;
	VecNd	fMaxDt;

	///< 現在のばね部の距離（三要素モデル用）
	SpatialVector newXs;

	/// コンストラクタ
	PHNDJointMotor() {}

	// ----- PHConstraintBaseの仮想関数
	virtual void SetupAxisIndex();
	virtual void Setup         ();
	virtual void Iterate       ();
	virtual void CompResponse      (double df, int i);
	virtual void CompResponseDirect(double df, int i);

	/// 拘束軸を決定する
	//virtual void SetupAxisIndex();

	/// dA, dbを計算する
	//virtual void CompBias();

	// ----- このクラスの機能

	/// 弾性変形用のCompBias
	void CompBiasElastic();

	/// 塑性変形用のCompBias
	void CompBiasPlastic();

	/// 降伏したかどうか
	void CheckYielded();

	// ----- 派生クラスで実装する機能
	/// propVを計算する
	virtual VecNd GetPropV() { return VecNd(); }

	/// パラメータを取得する
	virtual void GetParams(PHNDJointMotorParam<NDOF>& p) {}

	/// パラメータを反映する
	virtual void SetParams(PHNDJointMotorParam<NDOF>& p) {}
};

///	1自由度関節の関節コントローラ
class PH1DJointMotor : public SceneObject, public PHNDJointMotor<1> {
public:
	SPR_OBJECTDEF(PH1DJointMotor);
	SPR_DECLMEMBEROF_PH1DJointMotorDesc;

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
class PHBallJointMotor : public SceneObject, public PHNDJointMotor<3> {
public:
	SPR_OBJECTDEF(PHBallJointMotor);
	SPR_DECLMEMBEROF_PHBallJointMotorDesc;

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
class PHSpringMotor : public SceneObject, public PHNDJointMotor<6> {
public:
	SPR_OBJECTDEF(PHSpringMotor);
	SPR_DECLMEMBEROF_PHSpringMotorDesc;

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

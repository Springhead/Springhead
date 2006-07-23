/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONSTRAINT_H
#define PHCONSTRAINT_H

#include <SprPhysics.h>

#define SUBMAT(r, c, h, w) sub_matrix(PTM::TSubMatrixDim<r, c, h, w>())
#define SUBVEC(o, l) sub_vector(PTM::TSubVectorDim<o, l>())

namespace Spr{;

typedef PTM::TMatrixRow<6, 6, double> Matrix6d;
typedef PTM::TMatrixRow<6, 3, double> Matrix63d;
typedef PTM::TVector<6, double> Vec6d;

/// 剛体の情報
struct PHSolidInfoForLCP : public PHSolidInfo{
	double		minv;		/// 質量の逆数
	Matrix3d	Iinv;		/// 慣性行列の逆行列
	Vec3d		f, t;		/// ローカル座標での外力
	Vec3d		v, w;		/// ローカル座標での現在の速度
	Vec3d		dv0, dw0;	/// 拘束力以外の外力による速度変化量
	Vec3d		dv, dw;		/// 拘束力による速度変化量
	//Vec3d		dV, dW;		/// Correctionによる移動量，回転量
	void SetupDynamics(double dt);
	void SetupCorrection();
	PHSolidInfoForLCP(PHSolid* s):PHSolidInfo(s){}
};

/// PHConstraint派生クラスのためのルータ
template<class inf, class base>
class InheritConstraint : public InheritSceneObject<inf, base>{
public:
	void Enable(bool bEnable = true){base::Enable(bEnable);}
	bool IsEnabled(){return base::IsEnabled();}
};

///
class PHConstraint : public InheritSceneObject<PHConstraintIf, SceneObject>{
public:
	OBJECT_DEF_ABST(PHConstraint);
	enum PHControlMode{
		MODE_TORQUE,
		MODE_POSITION,
		MODE_VELOCITY
	} mode;
	
	bool		bEnabled;			/// 有効化されている場合にtrue
	bool		bFeasible;			/// 両方の剛体がundynamicalな場合true

	PHSolidInfoForLCP* solid[2];
	Matrix3d	Rj[2];				/// 各剛体に張り付いた関節フレーム
	Vec3d		rj[2];
	Matrix3d	Rjrel;				/// 関節フレーム間の位置関係
	Quaterniond	qjrel;
	Vec3d		rjrel;
	Vec3d		vjrel, wjrel;		/// 関節フレーム間の相対速度
	/// Jvv, Jvw : 各剛体の速度，角速度から相対速度へのヤコビ行列
	/// Jwv, Jww : 各剛体の速度，角速度から相対角速度へのヤコビ行列
	/// Jqv, Jqw : 各剛体の速度，角速度から相対quaternionの時間微分へのヤコビ行列
	/**
				|	  v[0]	  w[0]	  v[1]	  w[1]
	  ----------+---------------------------------
		vjrel	|	Jvv[0]	Jvw[0]	Jvv[1]	Jvw[1]
		wjrel	|	Jwv[0]	Jww[0]	Jwv[1]	Jww[1]
		qjrel	|	Jqv[0]	Jqw[0]	Jqv[1]	Jqw[1]
	*/
	Matrix3d	Jvv[2], Jvw[2], Jwv[2], Jww[2];	//, Jqv[2], Jqw[2];
	Matrix3d	Tvv[2], Tvw[2], Twv[2], Tww[2];
	
	//int		dim_d, dim_c;	// 拘束の次元
	bool		constr[6];	// 拘束する自由度．trueならば拘束する．

	//Matrix63d	Jdv[2], Jdw[2];	// Dynamics用の拘束ヤコビ行列
	//Matrix63d	Tdv[2], Tdw[2];
	
	//Matrix63d	Jcv[2], Jcw[2];	// Correction用の拘束ヤコビ行列
	//Matrix63d	Tcv[2], Tcw[2];

	Vec3d		fv, fw;	/// dynamicsにおける関節力の力積
	//Vec3d		Fv, Fq; /// correctionにおける関節力

	//Vec6d		b, B;	// bベクトル．dynamics用とcorrection用
	Vec3d		bv, bw;	/// dynamicsにおける拘束速度
	//Vec3d		bv_bias, bw_bias;	/// 速度制御を実現するためのbv, bwのオフセット量
	//Vec3d		Bv, Bq; /// correctionにおける拘束誤差
	//Vec6d		Ad, Ac;	// A行列の対角成分．dynamics用とcorrection用
	Vec3d		Av, Aw; //, Aq;
	Vec3d		dbv, dbw;	//バネダンパ実装時などにおけるbの補正値
	Vec3d		dAv, dAw;	//同Aの対角成分の補正値
	
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual bool AddChildObject(ObjectIf* o);
	virtual void Enable(bool bEnable = true){bEnabled = bEnable;}
	virtual bool IsEnabled(){return bEnabled;}
	void Init();
	virtual void CompJacobian(bool bCompAngular);
	void SetupDynamics(double dt, double correction_rate, double shrink_rate);
	void IterateDynamics();
	//void SetupCorrection(double dt, double max_error);
	//void IterateCorrection();
	//virtual void CompDof(){}					/// dim_v, dim_w, dim_qを設定する
	//virtual void CompMotorForce(){}				/// fv, fwにモータによる影響を設定する
	//virtual void CompConstraintJacobian()=0;
	virtual void CompBias(double dt, double correction_rate){}	/// 
	//virtual void CompError(double dt)=0;			/// Bv, Bqを設定する
	virtual void Projection(double& f, int k){}
	//virtual void ProjectionCorrection(double& F, int k){}
	PHConstraint();
};
class PHConstraints : public std::vector< UTRef<PHConstraint> >{
public:
	void SetupDynamics(double dt, double correction_rate, double shrink_rate){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupDynamics(dt, correction_rate, shrink_rate);
	}
	/*void SetupCorrection(double dt, double max_error){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupCorrection(dt, max_error);
	}*/
	void IterateDynamics(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateDynamics();
	}
	/*void IterateCorrection(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateCorrection();
	}*/
};

}

#endif

#ifndef PHCONSTRAINT_H
#define PHCONSTRAINT_H

#include <SprPhysics.h>

namespace Spr{;

/// 剛体の情報
struct PHSolidAux : public Object{
	PHSolid*	solid;		/// PHSolidへの参照
	double		minv;		/// 質量の逆数
	Matrix3d	Iinv;		/// 慣性行列の逆行列
	Vec3d		f, t;		/// ローカル座標での外力
	Vec3d		v, w;		/// ローカル座標での現在の速度
	Vec3d		dv0, dw0;	/// 拘束力以外の外力による速度変化量
	Vec3d		dv, dw;		/// 拘束力による速度変化量
	Vec3d		dV, dW;		/// Correctionによる移動量，回転量
	void SetupDynamics(double dt);
	void SetupCorrection();
	PHSolidAux::PHSolidAux(PHSolid* s):solid(s){}
};
class PHSolidAuxs : public std::vector< UTRef<PHSolidAux> >{
public:
	iterator Find(PHSolid* s){
		iterator is;
		for(is = begin(); is != end(); is++)
			if((*is)->solid == s)
				break;
		return is;
	};
	void SetupDynamics(double dt){
		for(iterator is = begin(); is != end(); is++)
			(*is)->SetupDynamics(dt);
	}
	void SetupCorrection(){
		for(iterator is = begin(); is != end(); is++)
			(*is)->SetupCorrection();
	}
};

/// PHConstraint派生クラスのためのルータ
template<class inf, class base>
class InheritConstraint : public InheritSceneObject<inf, base>{
public:
	void Enable(bool bEnable = true){base::Enable(bEnable);}
	bool IsEnabled(){return base::IsEnabled();}
};
template<class inf, class base>
class InheritJoint1D : public InheritConstraint<inf, base>{
public:
	void	SetRange(double lower, double upper){base::SetRange(lower, upper);}
	void	GetRange(double& lower, double& upper){base::GetRange(lower, upper);}
	void	SetMotorTorque(double t){base::SetMotorTorque(t);}
	double	GetMotorTorque(){return base::GetMotorTorque();}
	// void	SetDesiredPosition(double p, double t) = 0;	/// 目標変位を設定する
	// double	GetDesiredPosition() = 0;				/// 目標変位を取得する
	void	SetDesiredVelocity(double v){base::SetDesiredVelocity(v);}
	double	GetDesiredVelocity(){return base::GetDesiredVelocity();}
	void	SetSpring(double K){base::SetSpring(K);}
	double	GetSpring(){return base::GetSpring();}
	void	SetSpringOrigin(double org){base::SetSpringOrigin(org);}
	double	GetSpringOrigin(){return base::GetSpringOrigin();}
	void	SetDamper(double D){base::SetDamper(D);}
	double	GetDamper(){return base::GetDamper();}
};

///
class PHConstraint : public InheritSceneObject<PHConstraintIf, SceneObject>{
public:
	//OBJECTDEF(PHConstraint);
	enum PHControlMode{
		MODE_TORQUE,
		MODE_POSITION,
		MODE_VELOCITY
	} mode;
	int			dim_v, dim_w, dim_q;
	//int			idx_v[3], idx_w[3], idx_q[3];
	bool		bEnabled;			/// 有効化されている場合にtrue
	bool		bFeasible;			/// 両方の剛体がundynamicalな場合true

	PHSolidAux* solid[2];
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
		vrel	|	Jvv[0]	Jvw[0]	Jvv[1]	Jvw[1]
		wrel	|	Jwv[0]	Jww[0]	Jwv[1]	Jww[1]
		qdrel	|	Jqv[0]	Jqw[0]	Jqv[1]	Jqw[1]
	*/
	Matrix3d	Jvv[2], Jvw[2], Jwv[2], Jww[2], Jqv[2], Jqw[2];
	Matrix3d	Tvv[2], Tvw[2], Twv[2], Tww[2], Tqv[2], Tqw[2];
	Vec3d		fv, fw;	/// dynamicsにおける関節力
	Vec3d		Fv, Fq; /// correctionにおける関節力
	Vec3d		bv, bw;	/// dynamicsにおける拘束速度
	//Vec3d		bv_bias, bw_bias;	/// 速度制御を実現するためのbv, bwのオフセット量
	Vec3d		Bv, Bq; /// correctionにおける拘束誤差
	Vec3d		Av, Aw, Aq;
	
	virtual void Enable(bool bEnable = true){bEnabled = bEnable;}
	virtual bool IsEnabled(){return bEnabled;}
	virtual void Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc);
	void CompJacobian(bool bCompAngular);
	void SetupDynamics(double dt);
	void SetupCorrection(double dt, double max_error);
	void IterateDynamics();
	void IterateCorrection();
	virtual void CompDof(){}					/// dim_v, dim_w, dim_qを設定する
	virtual void CompMotorForce(){}				/// fv, fwにモータによる影響を設定する
	virtual void CompBias(double dt){}	/// 
	virtual void CompError();			/// Bv, Bqを設定する
	virtual void Projectionfv(double& f, int k){}
	virtual void Projectionfw(double& f, int k){}
	virtual void ProjectionFv(double& F, int k){}
	virtual void ProjectionFq(double& F, int k){}
	PHConstraint();
};
class PHConstraints : public std::vector< UTRef<PHConstraint> >{
public:
	void SetupDynamics(double dt){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupDynamics(dt);
	}
	void SetupCorrection(double dt, double max_error){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupCorrection(dt, max_error);
	}
	void IterateDynamics(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateDynamics();
	}
	void IterateCorrection(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateCorrection();
	}
};

}

#endif
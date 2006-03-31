#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Collision/CDDetectorImp.h>
#include <vector>

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
	void Init();
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

class PHContactPoint : public InheritConstraint<PHContactPointIf, PHConstraint>{
public:
	//OBJECTDEF(PHContactPoint);
	CDShapePair* shapePair;
	Vec3d pos;
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_CONTACT;}
	virtual void CompError();
	virtual void Projectionfv(double& f, int k);
	virtual void ProjectionFv(double& F, int k);
	PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
	PHContactPoint(const Matrix3d& local, CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
};

class PHJoint : public InheritConstraint<PHJointIf, PHConstraint>{
public:

};
class PHJoint1D : public InheritConstraint<PHJoint1DIf, PHJoint>{
public:
	//OBJECTDEF(PHJoint1D);
	bool on_lower, on_upper;	/// 可動範囲の下限、上限に達している場合にtrue
	double lower, upper;		/// 可動範囲の下限、上限
	double torque;
	double pos_d, vel_d;		/// 目標変位、目標速度
	double spring, origin, damper;	/// バネ係数、バネ原点、ダンパ係数
	
	virtual void Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc){
		PHConstraint::Init(lhs, rhs, desc);
		const PHJoint1DDesc& desc1D = (const PHJoint1DDesc&)desc;
		lower = desc1D.lower;
		upper = desc1D.upper;
		spring = desc1D.spring;
		origin = desc1D.origin;
		damper = desc1D.damper;
		torque = desc1D.torque;
	}
	virtual void	SetRange(double l, double u){lower = l, upper = u;}
	virtual void	GetRange(double& l, double& u){l = lower, u = upper;}
	virtual void	SetMotorTorque(double t){mode = MODE_TORQUE; torque = t;}
	virtual double	GetMotorTorque(){return torque;}
	//virtual void SetDesiredPosition(double p){mode = MODE_POSITION; pos_d = p;}
	//virtual double GetDesiredPosition(){return pos_d;}
	virtual void	SetDesiredVelocity(double v){mode = MODE_VELOCITY; vel_d = v;}
	virtual double	GetDesiredVelocity(){return vel_d;}
	virtual void	SetSpring(double K){spring = K;}
	virtual double	GetSpring(){return spring;}
	virtual void	SetSpringOrigin(double org){origin = org;}
	virtual double	GetSpringOrigin(){return origin;}
	virtual void	SetDamper(double D){damper = D;}
	virtual double	GetDamper(){return damper;}
	
};

class PHHingeJoint : public InheritJoint1D<PHHingeJointIf, PHJoint1D>{
public:
	//OBJECTDEF(PHHingeJoint);
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_HINGE;}
	virtual double GetPosition();
	virtual double GetVelocity();
	virtual void CompDof();
	virtual void CompMotorForce();
	virtual void CompBias(double dt);
	virtual void Projectionfw(double& f, int k);
	virtual void ProjectionFq(double& F, int k);
	PHHingeJoint(){}
};
class PHSliderJoint : public InheritJoint1D<PHSliderJointIf, PHJoint1D>{
public:
	//OBJECTDEF(PHSliderJoint);
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_SLIDER;}
	virtual void CompDof();
	PHSliderJoint(){}
};
class PHBallJoint : public InheritConstraint<PHBallJointIf, PHConstraint>{
public:
	//OBJECTDEF(PHBallJoint);
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_BALL;}
	virtual void CompDof();
	PHBallJoint(){}
};

class PHConstraintEngine;
///	Shapeの組合わせ
class PHShapePair: public CDShapePair{
public:
	///	
	PHShapePair(){}
	PHShapePair(CDShape* s0, CDShape* s1):CDShapePair(s0, s1){}
	///	接触判定．接触の中心(center 最侵入点の中点)と法線ベクトルまで求める
	bool Detect(unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1);
	///	接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．
	void EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1);
};
class PHConstraintEngine: public PHEngine{
	friend class PHSolidPair;

	OBJECTDEF(PHConstraintEngine);

	/// Solidの組み合わせの配列
	class PHSolidPair{
	public:
		bool bEnabled;
		UTRef<PHSolidAux> solid[2];
		/// Shapeの組み合わせの配列
		typedef UTCombination<PHShapePair> PHShapePairs;
		PHShapePairs	shapePairs;

		void Init(PHSolidAux* s0, PHSolidAux* s1);	/// 初期化
		bool Detect(PHConstraintEngine* engine);	

	};
	typedef UTCombination<PHSolidPair> PHSolidPairs;
	
protected:
	bool			ready;			/// 
	PHSolidAuxs		solids;			/// 剛体の配列
	PHSolidPairs	solidPairs;	
	PHConstraints	points;			///	接触点の配列
	PHConstraints	joints;			///	関節の配列
	int max_iter_dynamics;			/// Dynamics()の反復回数
	int max_iter_correction;		/// Correction()の反復回数
	//double step_size;				/// LCPのステップ幅
	//double converge_criteria;		/// 収束判定の閾値
	double max_error;

	bool DetectPenetration();		/// 全体の交差の検知
	void PrintContacts();
	void SetupDynamics(double dt);	/// LCPの準備
	void SetupCorrection(double dt);/// 誤差の修正
	void IterateDynamics();			/// Correction()での一度の反復
	void IterateCorrection();		/// Correction()での一度の反復
	void UpdateSolids(double dt);	/// 結果をSolidに反映する
	//void SetInitialValue();		/// LCPの決定変数の初期値を設定
	
public:
	void Add(PHSolid* s);			/// Solid を登録する
	void Remove(PHSolid* s);		/// 登録されているSolidを削除する
	PHJoint* AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc);	/// 関節の追加する
	void EnableContact(PHSolid* lhs, PHSolid* rhs, bool bEnable);
	void Invalidate(){ready = false;}	/// readyフラグをリセット
	void Init();						/// 初期化し，readyフラグをセット
	///
	int GetPriority() const {return SGBP_CONSTRAINTENGINE;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s);

	PHConstraintEngine();
	~PHConstraintEngine();

	friend class PHShapePair;
};


}	//	namespace Spr
#endif

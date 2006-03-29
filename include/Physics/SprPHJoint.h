#ifndef SPR_PHJOINTIf_H
#define SPR_PHJOINTIf_H
#include <Foundation/SprObject.h>

namespace Spr{;

struct PHJointDesc{
	enum JointType{
		JOINT_CONTACT,
		JOINT_HINGE,
		JOINT_SLIDER,
		JOINT_BALL,
	} type;					/// 関節の種類
	bool bEnabled;
	Posed poseJoint[2];		/// 剛体から見た関節の位置と傾き
	PHJointDesc():bEnabled(true){}
};
struct PHJoint1DDesc : public PHJointDesc{
	double	lower, upper;	/// 可動範囲. lower < upperのときに有効となる
	PHJoint1DDesc():lower(0.0), upper(0.0){}
};
struct PHHingeJointDesc : public PHJoint1DDesc{
	PHHingeJointDesc(){
		type = JOINT_HINGE;
	}
};
struct PHSliderJointDesc : public PHJoint1DDesc{
	PHSliderJointDesc(){
		type = JOINT_SLIDER;
	}
};
struct PHBallJointDesc : public PHJointDesc{
	double	max_angle;		/// 円錐状の可動範囲
	PHBallJointDesc(){
		type = JOINT_BALL;
		max_angle = 0.0;
	}
};

struct PHConstraintIf : public SceneObjectIf{
	IF_DEF(PHConstraint);
	virtual void Enable(bool bEnable = true) = 0;		/// 関節を有効/無効化する
	virtual bool IsEnabled() = 0;
	virtual PHJointDesc::JointType GetJointType() = 0;	/// 関節の種類
};
struct PHContactPointIf : public PHConstraintIf{
	IF_DEF(PHContactPoint);
};
struct PHJointIf : public PHConstraintIf{
	IF_DEF(PHJoint);
};
struct PHJoint1DIf : public PHJointIf{
	IF_DEF(PHJoint1D);
	virtual void	SetRange(double lower, double upper) = 0;	/// 可動範囲を設定する
	virtual void	SetTorque(double t) = 0;					/// 関節トルクを設定する
	virtual double	GetTorque() = 0;							/// 関節トルクを取得する
	//virtual void	SetDesiredPosition(double p, double t) = 0;	/// 目標変位を設定する
	//virtual double	GetDesiredPosition() = 0;				/// 目標変位を取得する
	virtual void	SetDesiredVelocity(double v) = 0;			/// 目標速度を設定する
	virtual double	GetDesiredVelocity() = 0;					/// 目標速度を取得する
	virtual double	GetPosition() = 0;							/// 変位を取得する
	virtual double	GetVelocity() = 0;							/// 速度を取得する
};
struct PHHingeJointIf : public PHJoint1DIf{
	IF_DEF(PHHingeJoint);
};
struct PHSliderJointIf : public PHJoint1DIf{
	IF_DEF(PHSliderJoint);
};
struct PHBallJointIf : public PHConstraintIf{
	IF_DEF(PHBallJoint);
};

}

#endif

/**
 *	@file SprPHJoint.h
 *	@brief 接触や関節など
*/
#ifndef SPR_PHJOINTIf_H
#define SPR_PHJOINTIf_H
#include <Foundation/SprObject.h>

namespace Spr{;

/** \addtogroup gpJoint 関節クラス	*/
//@{

/// 拘束のディスクリプタ
struct PHConstraintDesc{
	/// 拘束の種類
	enum ConstraintType{
		CONTACT,
		HINGEJOINT,
		SLIDERJOINT,
		BALLJOINT,
		PARAMETRICJOINT
	} type;
	/// 有効/無効フラグ
	bool bEnabled;
	/// 剛体から見た関節の位置と傾き
	Posed pose[2];
	PHConstraintDesc():bEnabled(true){}
};

/// 関節のディスクリプタ	<	何もメンバを追加しない場合は，typedefと別名定義を FIDesc.cppに追加で．
typedef PHConstraintDesc	PHJointDesc;

/// 1軸関節のディスクリプタ
struct PHJoint1DDesc : public PHJointDesc{
	double	lower, upper;	/// 可動範囲. lower < upperのときに有効となる
	double	spring;		/// バネ係数
	double  origin;		/// バネ原点
	double  damper;		/// ダンパ係数
	double	torque;		/// モータトルク
	PHJoint1DDesc():lower(0.0), upper(0.0), spring(0.0), origin(0.0), damper(0.0), torque(0.0){}
};

/// ヒンジのディスクリプタ
struct PHHingeJointDesc : public PHJoint1DDesc{
	PHHingeJointDesc(){
		type = HINGEJOINT;
	}
};

/// スライダのディスクリプタ
struct PHSliderJointDesc : public PHJoint1DDesc{
	PHSliderJointDesc(){
		type = SLIDERJOINT;
	}
};

/// ボールジョイントのディスクリプタ
struct PHBallJointDesc : public PHJointDesc{
	double	max_angle;		///< 円錐状の可動範囲
	Vec3d	torque;			///< モータトルク
	PHBallJointDesc(){
		type = BALLJOINT;
		max_angle = 0.0;
	}
};

/// 拘束のインタフェース
struct PHConstraintIf : public SceneObjectIf{
	IF_DEF(PHConstraint);

	/** @brief 拘束を有効/無効化する
		@param bEnable trueならば有効化，falseならば無効化する
	 */
	virtual void Enable(bool bEnable = true) = 0;

	/** @brief 有効か無効かを取得する
		@return 有効ならばtrue, 無効ならばfalse
	 */
	virtual bool IsEnabled() = 0;

	/** @brief 拘束の種類を取得する
		@return 拘束の種類
	 */
	virtual PHConstraintDesc::ConstraintType GetConstraintType() = 0;
};

/// 接触点拘束のインタフェース
struct PHContactPointIf : public PHConstraintIf{
	IF_DEF(PHContactPoint);
};

/// 関節のインタフェース
struct PHJointIf : public PHConstraintIf{
	IF_DEF(PHJoint);
};

/// 1軸関節のインタフェース
struct PHJoint1DIf : public PHJointIf{
	IF_DEF(PHJoint1D);

	/** @brief 可動範囲を設定する
		@param lower 可動範囲の下限
		@param upper 可動範囲の上限
	 */
	virtual void	SetRange(double lower, double upper) = 0;
	
	/** @brief 可動範囲を取得する
		@param lower 可動範囲の下限を取得する変数への参照
		@param upper 可動範囲の上限を取得する変数への参照
	 */
	virtual void	GetRange(double& lower, double& upper) = 0;

	/** @brief モータトルクを設定する
		@param torque モータトルク
	 */
	virtual void	SetMotorTorque(double t) = 0;

	/** @brief モータトルクを取得する
		@return モータトルク
	 */
	virtual double	GetMotorTorque() = 0;

	//virtual void	SetDesiredPosition(double p, double t) = 0;	/// 目標変位を設定する
	//virtual double	GetDesiredPosition() = 0;				/// 目標変位を取得する

	/** @brief 目標速度を設定する
		@param vel 目標速度
	 */
	virtual void	SetDesiredVelocity(double v) = 0;

	/** @brief 目標速度を取得する
		@return 目標速度
	 */
	virtual double	GetDesiredVelocity() = 0;

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	virtual void	SetSpring(double spring) = 0;

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	virtual double	GetSpring() = 0;

	/** @brief バネの原点を設定する
		@param origin バネ原点

		バネ原点とはバネの力が0となる関節変位のこと．
	 */
	virtual void	SetSpringOrigin(double origin) = 0;

	/** @brief バネの原点を取得する
		@return バネ原点
	 */
	virtual double	GetSpringOrigin() = 0;

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	virtual void	SetDamper(double damper) = 0;

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	virtual double	GetDamper() = 0;

	/** @brief 関節変位を取得する
		@return 関節変位
	 */
	virtual double	GetPosition() = 0;

	/** @brief 関節速度を取得する
		@return 関節速度
	 */
	virtual double	GetVelocity() = 0;
};

/// ヒンジのインタフェース
struct PHHingeJointIf : public PHJoint1DIf{
	IF_DEF(PHHingeJoint);
};

/// スライダのインタフェース
struct PHSliderJointIf : public PHJoint1DIf{
	IF_DEF(PHSliderJoint);
};

/// パラメトリックジョイントのインタフェース
struct PHParametricJointIf : public PHJoint1DIf{
	IF_DEF(PHParametricJoint);
};

/// ボールジョイントのインタフェース
struct PHBallJointIf : public PHConstraintIf{
	IF_DEF(PHBallJoint);

	/** @brief 最大角度を設定する
		@param angle 最大角度
	 */
	virtual void SetMaxAngle(double angle) = 0;

	/** @brief 最大角度を取得する
		@return 最大角度
	 */
	virtual double GetMaxAngle() = 0;

	/** @brief モータトルクを設定する
		@param torque モータトルク
	 */
	virtual void SetMotorTorque(const Vec3d& torque)=0;

	/** @brief モータトルクを取得する
		@return モータトルク
	 */
	virtual Vec3d GetMotorTorque()=0;

	/** @brief 関節変位を取得する
		@return 関節変位
	 */
	virtual Quaterniond	GetPosition() = 0;

	/** @brief 関節速度を取得する
		@return 関節速度
	 */
	virtual Vec3d GetVelocity() = 0;

};

//@}

}

#endif

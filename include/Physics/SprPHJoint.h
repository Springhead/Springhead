/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHJoint.h
 *	@brief 接触や関節など
*/
#ifndef SPR_PHJOINTIf_H
#define SPR_PHJOINTIf_H
#include <Foundation/SprObject.h>

namespace Spr{;

/** \addtogroup gpPhysics */
//@{
/** \defgroup gpJoint ジョイント*/
//@{

/// 拘束のディスクリプタ
struct PHConstraintDesc{
	/// 拘束の種類
	enum ConstraintType{
		INVALID_CONSTRAINT,
		CONTACT,
		HINGEJOINT,
		SLIDERJOINT,
		BALLJOINT,
		PATHJOINT,
		SPRING
	} type;
	/// 有効/無効フラグ
	bool bEnabled;
	/// 剛体から見た関節の位置と傾き
	Posed poseSocket;
	Posed posePlug;
	PHConstraintDesc():bEnabled(true){}
};

/// 関節のディスクリプタ	<	何もメンバを追加しない場合は，typedefと別名定義を FIDesc.cppに追加で．
typedef PHConstraintDesc	PHJointDesc;

/// 1軸関節のディスクリプタ
struct PHJoint1DDesc : public PHJointDesc{
	double	lower, upper;	///< 可動範囲. lower < upperのときに有効となる
	double	spring;			///< バネ係数
	double  origin;			///< バネ原点
	double  damper;			///< ダンパ係数
	double	torque;			///< モータトルク

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

/// パス上の1つの点
struct PHPathPoint{
	double	s;			///< スカラーパラメータ
	Posed	pose;		///< 位置と傾き
};
/// パスのディスクリプタ
struct PHPathDesc{
	std::vector<PHPathPoint> points;	///< パス上の点列
	bool bLoop;							///< trueならばループパス，falseならばオープンパス．デフォルトはfalse．
	PHPathDesc():bLoop(false){}
};

/// パスジョイントのディスクリプタ
struct PHPathJointDesc : public PHJoint1DDesc{
	PHPathJointDesc(){
		type = PATHJOINT;
	}
};

/// ボールジョイントのディスクリプタ
struct PHBallJointDesc : public PHJointDesc{
	double	swingUpper;		///< スイング角度の上限
	double  twistLower;		///< ツイスト角度の下限
	double  twistUpper;		///< ツイスト角度の上限
	double	swingSpring;	///< スイング角のバネ係数
	double  swingDamper;	///< スイング角のダンパ係数
	double	twistSpring;	///< ツイスト角のバネ係数
	double  twistDamper;	///< ツイスト角のダンパ係数
	Vec3d	torque;			///< モータトルク
	PHBallJointDesc(){
		type = BALLJOINT;
		swingUpper = -1.0;
		swingSpring = swingDamper = 0.0;
		twistLower =  1.0;
		twistUpper = -1.0;
		twistSpring = twistDamper = 0.0;
	}
};

/// バネダンパのディスクリプタ
struct PHSpringDesc : public PHConstraintDesc{
	Vec3d spring;		///< バネ係数
	Vec3d damper;		///< ダンパ係数
	PHSpringDesc(){
		type = SPRING;
	}
};

/** @brief ツリーノードのディスクリプタ
 */
struct PHTreeNodeDesc{
};
struct PHRootNodeDesc : public PHTreeNodeDesc{
};
struct PHHingeJointNodeDesc : public PHTreeNodeDesc{
};
struct PHSliderJointNodeDesc : public PHTreeNodeDesc{
};
struct PHBallJointNodeDesc : public PHTreeNodeDesc{
};
struct PHPathJointNodeDesc : public PHTreeNodeDesc{
};

/// ギアのディスクリプタ
struct PHGearDesc{
	double ratio;		///< ギア比
	PHGearDesc(){
		ratio = 1.0;
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

	/** @brief 拘束する剛体間の相対位置・姿勢を取得
		@param p ソケットに対するプラグの位置と向き
	 */
	virtual void GetRelativePose(Posed& p) = 0;

	/** @brief 拘束する剛体間の相対速度
		@param v ソケットに対するプラグの速度
		@param w ソケットに対するプラグの角速度
	 */
	virtual void GetRelativeVelocity(Vec3d& v, Vec3d& w) = 0;

	/** @brief 拘束力を取得
		@param f 並進力
		@param t モーメント
	 */
	virtual void GetConstraintForce(Vec3d& f, Vec3d& t) = 0;
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

/// パスのインタフェース
struct PHPathIf : public SceneObjectIf{
	IF_DEF(PHPath);
	/** @brief パスに点を追加する
		@param s 関節座標
		@param pose ソケットに対するプラグの姿勢
		パスに制御点を追加する．
		すなわち，関節座標がsであるときのソケット-プラグ間の姿勢をposeに設定する．
	 */
	virtual void AddPoint(double s, const Posed& pose) = 0;

	/** @brief ループパスかオープンパスかを設定する
		@param bOnOff trueならばループパス，falseならばオープンパス
		ループパスの場合，関節座標が最小の制御点と最大の制御点をシームレスにつなぐ．
		オープンパスの場合，関節座標が最小，最大の制御点がそれぞれ可動範囲の下限，上限となる．
	 */
	virtual void SetLoop(bool bOnOff = true) = 0;
	/** @brief ループパスかオープンパスかを取得する
		@return trueならばループパス，falseならばオープンパス
	 */
	virtual bool IsLoop() = 0;
};

/// パスジョイントのインタフェース
struct PHPathJointIf : public PHJoint1DIf{
	IF_DEF(PHPathJoint);
	virtual void SetPosition(double q)=0;
};

/// ボールジョイントのインタフェース
struct PHBallJointIf : public PHConstraintIf{
	IF_DEF(PHBallJoint);

	/** @brief スイング角の可動範囲を設定する
		@param upper 最大スイング角度
		可動範囲制限を無効化するにはupperに負の値を設定する
	 */
	virtual void SetSwingRange(double upper) = 0;

	/** @brief スイング角の可動範囲を取得する
		@return 最大スイング角度
	 */
	virtual double GetSwingRange() = 0;

	/** @brief ツイスト角の可動範囲を設定する
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
		可動範囲制限を無効化するにはlower > upperな値を設定する
	 */
	virtual void SetTwistRange(double lower, double upper) = 0;

	/** @brief ツイスト角の可動範囲を取得する
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
	 */
	virtual void GetTwistRange(double& lower, double& upper) = 0;

	/** @brief モータトルクを設定する
		@param torque モータトルク
	 */
	virtual void SetMotorTorque(const Vec3d& torque)=0;

	/** @brief モータトルクを取得する
		@return モータトルク
	 */
	virtual Vec3d GetMotorTorque()=0;

	/** @brief 関節変位を取得する
		@return スイング方位角，スイング角，ツイスト角からなるベクトル
	 */
	virtual Vec3d GetAngle() = 0;

	/** @brief 関節速度を取得する
		@return スイング方位角，スイング角，ツイスト角の時間変化率からなるベクトル
	 */
	virtual Vec3d GetVelocity() = 0;

};

/// バネダンパのインタフェース
struct PHSpringIf : public PHConstraintIf{
	IF_DEF(PHSpring);
	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	virtual void SetSpring(const Vec3d& spring)=0;

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	virtual Vec3d GetSpring()=0;

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	virtual void SetDamper(const Vec3d& damper)=0;

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	virtual Vec3d GetDamper()=0;

};

/// ツリーノードのインタフェース
struct PHTreeNodeIf : public SceneObjectIf{
	IF_DEF(PHTreeNode);
};
/// ルートノードのインタフェース
struct PHRootNodeIf : public PHTreeNodeIf{
	IF_DEF(PHRootNode);
};
struct PHHingeJointNodeIf : public PHTreeNodeIf{
	IF_DEF(PHHingeJointNode);
};
struct PHSliderJointNodeIf : public PHTreeNodeIf{
	IF_DEF(PHSliderJointNode);
};
struct PHBallJointNodeIf : public PHTreeNodeIf{
	IF_DEF(PHBallJointNode);
};
struct PHPathJointNodeIf : public PHTreeNodeIf{
	IF_DEF(PHPathJointNode);
};

/// ギアのインタフェース
struct PHGearIf : public SceneObjectIf{
	IF_DEF(PHGear);
};
	
//@}
//@}

}

#endif

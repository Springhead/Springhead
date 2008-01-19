/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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
	/*enum ConstraintType{
		INVALID_CONSTRAINT,
		CONTACT,
		HINGEJOINT,
		SLIDERJOINT,
		BALLJOINT,
		PATHJOINT,
		SPRING
	} type;*/
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
	double	desiredVelocity; ///< 目標速度
	double	torque;			///< モータトルク
	double	fMax;			///< 関節にかけられる最大の力
	double	fMin;			///< 関節にかけられる最小の力

	PHJoint1DDesc():lower(0.0), upper(0.0), spring(0.0), origin(0.0), damper(0.0), desiredVelocity(0.0), torque(0.0), fMax(FLT_MAX), fMin(-FLT_MAX){}
};


/** @brief ツリーノードのディスクリプタ
 */
struct PHTreeNodeDesc{
	bool bEnabled;
	PHTreeNodeDesc(){
		bEnabled = true;
	}
};
struct PHRootNodeDesc : public PHTreeNodeDesc{
	PHRootNodeDesc(){}
};
struct PHTreeNode1DDesc : public PHTreeNodeDesc{
};
struct PHHingeJointNodeDesc : public PHTreeNode1DDesc{
};
struct PHSliderJointNodeDesc : public PHTreeNode1DDesc{
};
struct PHPathJointNodeDesc : public PHTreeNode1DDesc{
};
struct PHBallJointNodeDesc : public PHTreeNodeDesc{
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

	/** @brief ソケットの位置・向きを取得・設定する
	 */
	virtual void GetSocketPose(Posed& pose) = 0;
	virtual void SetSocketPose(const Posed& pose) = 0;

	/** @brief プラグの位置・向きを取得・設定する
	 */
	virtual void GetPlugPose(Posed& pose) = 0;
	virtual void SetPlugPose(const Posed& pose) = 0;
	
	/** @brief 拘束の種類を取得する
		@return 拘束の種類
	 */
	//virtual PHConstraintDesc::ConstraintType GetConstraintType() = 0;

	/** @brief 拘束する剛体間の相対位置・姿勢を取得
		@param p ソケットに対するプラグの位置と向き
	 */
	virtual void GetRelativePose(Posed& p) = 0;
	
	/** @brief 拘束する剛体間の相対位置を取得
	*/
	virtual Vec3d GetRelativePoseR() = 0;

	/** @brief 拘束する剛体間の相対姿勢を取得
	*/
	virtual Quaterniond GetRelativePoseQ() = 0;
	
	/** @brief 拘束する剛体間の相対速度
		@param v ソケットに対するプラグの速度
		@param w ソケットに対するプラグの角速度
	 */
	virtual void GetRelativeVelocity(Vec3d& v, Vec3d& w) = 0;

	/** @brief 拘束力を取得
		@param f 並進力
		@param t モーメント
		拘束力と拘束トルクをソケットのローカル座標系で返す
	 */
	virtual void GetConstraintForce(Vec3d& f, Vec3d& t) = 0;
};

/// 拘束の集合のインタフェース
struct PHConstraintsIf : public SceneObjectIf{
	IF_DEF(PHConstraints);

	/** @brief 指定された剛体の組に作用している拘束を返す
		@param lhs 剛体その１
		@param rhs 剛体その２
		@return 剛体間に作用している拘束
	*/
	virtual PHConstraintIf* FindBySolidPair(PHSolidIf* lhs, PHSolidIf* rhs) = 0;
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
	
	/** @brief 関節の出すことができる最大トルク(N)を設定する
	*/
	virtual void SetTorqueMax(double max) = 0;

	/** @brief 関節の出すことができる最大トルク(N)を取得する
	*/
	virtual double GetTorqueMax() = 0;

		/** @brief 関節の出すことができる最小トルク(N)を設定する
	*/
	virtual void SetTorqueMin(double min) = 0;

	/** @brief 関節の出すことができる最小トルク(N)を取得する
	*/
	virtual double GetTorqueMin() = 0;

};

/// ヒンジのインタフェース
struct PHHingeJointIf : public PHJoint1DIf{
	IF_DEF(PHHingeJoint);
};
/// ヒンジのディスクリプタ
struct PHHingeJointDesc : public PHJoint1DDesc{
	DESC_DEF_FOR_OBJECT(PHHingeJoint);
	PHHingeJointDesc(){}
};

/// スライダのインタフェース
struct PHSliderJointIf : public PHJoint1DIf{
	IF_DEF(PHSliderJoint);
};
/// スライダのディスクリプタ
struct PHSliderJointDesc : public PHJoint1DDesc{
	DESC_DEF_FOR_OBJECT(PHSliderJoint);
	PHSliderJointDesc(){}
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
/// パスジョイントのディスクリプタ
struct PHPathJointDesc : public PHJoint1DDesc{
	DESC_DEF_FOR_OBJECT(PHPathJoint);
	PHPathJointDesc(){}
};

/// ボールジョイントのインタフェース
struct PHBallJointIf : public PHConstraintIf{
	IF_DEF(PHBallJoint);

	
	/** @brief スイング角の可動範囲を設定する
		@param lower 最小スイング角度
		@param upper 最大スイング角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	virtual void SetSwingRange(Vec2d range) = 0;
	/** @brief スイング角の可動範囲を取得する
		@param lower 最大スイング角度
		@param upper 最大スイング角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	virtual void GetSwingRange(Vec2d& range) = 0;

	/** @brief ツイスト角の可動範囲を設定する
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	virtual void SetTwistRange(Vec2d range) = 0;
	/** @brief ツイスト角の可動範囲を取得する
		@param lower 最大ツイスト角度
		@param upper 最大ツイスト角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	virtual void GetTwistRange(Vec2d& range) = 0;

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

	/** @brief 関節の出せる最大トルクを設定する
		@param max 最大トルク
	*/
	virtual void SetTorqueMax(double max) = 0;

	/** @brief 関節の出せる最大トルクを取得する
	*/
	virtual double GetTorqueMax() = 0;

	/** @brief 関節の出せる最小トルクを設定する
		@param min 最小トルク
	*/
	virtual void SetTorqueMin(double min) = 0;

	/** @brief 関節の出せる最小トルクを取得する
	*/
	virtual double GetTorqueMin() = 0;


};
/// ボールジョイントのディスクリプタ
struct PHBallJointDesc : public PHJointDesc{
	DESC_DEF_FOR_OBJECT(PHBallJoint);
	double			spring;		///< バネ係数
	double			damper;		///< ダンパ係数
	Vec2d			limitSwing; ///< swing角の可動域（[0] or .lower, [1] or .upper）
	Vec2d			limitTwist;	///< twist角の可動域（[0] or .lower, [1] or .upper）
	Vec3d			limitDir;	///< 可動域の中心ベクトル
	Quaterniond		goal;		///< バネダンパの制御目標
	Vec3d			torque;		///< モータトルク
	double			fMax;		///< 関節にかけられる最大の力
	double			fMin;		///< 関節にかけられる最小の力
	
	PHBallJointDesc();		///< ディスクリプタのコンストラクタ
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

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	virtual void SetSpringOri(const double spring)=0;

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	virtual double GetSpringOri()=0;

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	virtual void SetDamperOri(const double damper)=0;

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	virtual double GetDamperOri()=0;
};
/// バネダンパのディスクリプタ
struct PHSpringDesc : public PHConstraintDesc{
	DESC_DEF_FOR_OBJECT(PHSpring);
	Vec3d		spring;		///< バネ係数
	Vec3d		damper;		///< ダンパ係数
	double		springOri;
	double		damperOri;
	PHSpringDesc();
};

/// ツリーノードのインタフェース
struct PHTreeNodeIf : public SceneObjectIf{
	IF_DEF(PHTreeNode);
	virtual void Enable(bool bEnable = true)=0;
};
/// ルートノードのインタフェース
struct PHRootNodeIf : public PHTreeNodeIf{
	IF_DEF(PHRootNode);
};
/// １軸関節ノードのインタフェース
struct PHTreeNode1DIf : public PHTreeNodeIf{
	IF_DEF(PHTreeNode1D);
};
struct PHHingeJointNodeIf : public PHTreeNode1DIf{
	IF_DEF(PHHingeJointNode);
};
struct PHSliderJointNodeIf : public PHTreeNode1DIf{
	IF_DEF(PHSliderJointNode);
};
struct PHPathJointNodeIf : public PHTreeNode1DIf{
	IF_DEF(PHPathJointNode);
};
struct PHBallJointNodeIf : public PHTreeNodeIf{
	IF_DEF(PHBallJointNode);
};

/// ギアのインタフェース
struct PHGearIf : public SceneObjectIf{
	IF_DEF(PHGear);
};
	
//@}
//@}

}

#endif

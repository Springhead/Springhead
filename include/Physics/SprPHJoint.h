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
	SPR_IFDEF(PHConstraint);

	/** @brief 拘束を有効/無効化する
		@param bEnable trueならば有効化，falseならば無効化する
	 */
	void Enable(bool bEnable = true);

	/** @brief 有効か無効かを取得する
		@return 有効ならばtrue, 無効ならばfalse
	 */
	bool IsEnabled();

	/** @brief ソケットの位置・向きを取得・設定する
	 */
	void GetSocketPose(Posed& pose);
	void SetSocketPose(const Posed& pose);

	/** @brief プラグの位置・向きを取得・設定する
	 */
	void GetPlugPose(Posed& pose);
	void SetPlugPose(const Posed& pose);
	
	/** @brief 拘束の種類を取得する
		@return 拘束の種類
	 */
	//PHConstraintDesc::ConstraintType GetConstraintType();

	/** @brief 拘束する剛体間の相対位置・姿勢を取得
		@param p ソケットに対するプラグの位置と向き
	 */
	void GetRelativePose(Posed& p);
	
	/** @brief 拘束する剛体間の相対位置を取得
	*/
	Vec3d GetRelativePoseR();

	/** @brief 拘束する剛体間の相対姿勢を取得
	*/
	Quaterniond GetRelativePoseQ();
	
	/** @brief 拘束する剛体間の相対速度
		@param v ソケットに対するプラグの速度
		@param w ソケットに対するプラグの角速度
	 */
	void GetRelativeVelocity(Vec3d& v, Vec3d& w);

	/** @brief 拘束力を取得
		@param f 並進力		: PHConstraint::f.v()
		@param t モーメント : PHConstraint::f.w()
		拘束力と拘束トルクをソケットのローカル座標系で返す
	 */
	void GetConstraintForce(Vec3d& f, Vec3d& t);
};

/// 拘束の集合のインタフェース
struct PHConstraintsIf : public SceneObjectIf{
	SPR_IFDEF(PHConstraints);

	/** @brief 指定された剛体の組に作用している拘束を返す
		@param lhs 剛体その１
		@param rhs 剛体その２
		@return 剛体間に作用している拘束
	*/
	PHConstraintIf* FindBySolidPair(PHSolidIf* lhs, PHSolidIf* rhs);
};

/// 接触点拘束のインタフェース
struct PHContactPointIf : public PHConstraintIf{
	SPR_IFDEF(PHContactPoint);
};

/// 関節のインタフェース
struct PHJointIf : public PHConstraintIf{
	SPR_IFDEF(PHJoint);
};

/// 1軸関節のインタフェース
struct PHJoint1DIf : public PHJointIf{
	SPR_IFDEF(PHJoint1D);

	/** @brief 可動範囲を設定する
		@param lower 可動範囲の下限
		@param upper 可動範囲の上限
	 */
	void	SetRange(double lower, double upper);
	
	/** @brief 可動範囲を取得する
		@param lower 可動範囲の下限を取得する変数への参照
		@param upper 可動範囲の上限を取得する変数への参照
	 */
	void	GetRange(double& lower, double& upper);

	/** @brief モータトルクを設定する
		@param torque モータトルク
	 */
	void	SetMotorTorque(double t);

	/** @brief モータトルクを取得する
		@return モータトルク
	 */
	double	GetMotorTorque();

	//void	SetDesiredPosition(double p, double t);	/// 目標変位を設定する
	//double	GetDesiredPosition();				/// 目標変位を取得する

	/** @brief 目標速度を設定する
		@param vel 目標速度
	 */
	void	SetDesiredVelocity(double v);

	/** @brief 目標速度を取得する
		@return 目標速度
	 */
	double	GetDesiredVelocity();

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void	SetSpring(double spring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	double	GetSpring();

	/** @brief バネの原点を設定する
		@param origin バネ原点

		バネ原点とはバネの力が0となる関節変位のこと．
	 */
	void	SetSpringOrigin(double origin);

	/** @brief バネの原点を取得する
		@return バネ原点
	 */
	double	GetSpringOrigin();

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void	SetDamper(double damper);

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	double	GetDamper();

	/** @brief 関節変位を取得する
		@return 関節変位
	 */
	double	GetPosition();

	/** @brief 関節速度を取得する
		@return 関節速度
	 */
	double	GetVelocity();
	
	/** @brief 関節の出すことができる最大トルク(N)を設定する
	*/
	void SetTorqueMax(double max);

	/** @brief 関節の出すことができる最大トルク(N)を取得する
	*/
	double GetTorqueMax();

		/** @brief 関節の出すことができる最小トルク(N)を設定する
	*/
	void SetTorqueMin(double min);

	/** @brief 関節の出すことができる最小トルク(N)を取得する
	*/
	double GetTorqueMin();

};

/// ヒンジのインタフェース
struct PHHingeJointIf : public PHJoint1DIf{
	SPR_IFDEF(PHHingeJoint);
};
/// ヒンジのディスクリプタ
struct PHHingeJointDesc : public PHJoint1DDesc{
	SPR_DESCDEF(PHHingeJoint);
	PHHingeJointDesc(){}
};

/// スライダのインタフェース
struct PHSliderJointIf : public PHJoint1DIf{
	SPR_IFDEF(PHSliderJoint);
};
/// スライダのディスクリプタ
struct PHSliderJointDesc : public PHJoint1DDesc{
	SPR_DESCDEF(PHSliderJoint);
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
	SPR_IFDEF(PHPath);
	/** @brief パスに点を追加する
		@param s 関節座標
		@param pose ソケットに対するプラグの姿勢
		パスに制御点を追加する．
		すなわち，関節座標がsであるときのソケット-プラグ間の姿勢をposeに設定する．
	 */
	void AddPoint(double s, const Posed& pose);

	/** @brief ループパスかオープンパスかを設定する
		@param bOnOff trueならばループパス，falseならばオープンパス
		ループパスの場合，関節座標が最小の制御点と最大の制御点をシームレスにつなぐ．
		オープンパスの場合，関節座標が最小，最大の制御点がそれぞれ可動範囲の下限，上限となる．
	 */
	void SetLoop(bool bOnOff = true);
	/** @brief ループパスかオープンパスかを取得する
		@return trueならばループパス，falseならばオープンパス
	 */
	bool IsLoop();
};

/// パスジョイントのインタフェース
struct PHPathJointIf : public PHJoint1DIf{
	SPR_IFDEF(PHPathJoint);
	void SetPosition(double q);
};
/// パスジョイントのディスクリプタ
struct PHPathJointDesc : public PHJoint1DDesc{
	SPR_DESCDEF(PHPathJoint);
	PHPathJointDesc(){}
};

/// ボールジョイントのインタフェース
struct PHBallJointIf : public PHConstraintIf{
	SPR_IFDEF(PHBallJoint);

	
	/** @brief スイング角の可動範囲を設定する
		@param lower 最小スイング角度
		@param upper 最大スイング角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	void SetSwingRange(Vec2d range);
	/** @brief スイング角の可動範囲を取得する
		@param lower 最大スイング角度
		@param upper 最大スイング角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	void GetSwingRange(Vec2d& range);

	/** @brief ツイスト角の可動範囲を設定する
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	void SetTwistRange(Vec2d range);
	/** @brief ツイスト角の可動範囲を取得する
		@param lower 最大ツイスト角度
		@param upper 最大ツイスト角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	void GetTwistRange(Vec2d& range);

	/** @brief モータトルクを設定する
		@param torque モータトルク
	 */
	void SetMotorTorque(const Vec3d& torque);

	/** @brief モータトルクを取得する
		@return モータトルク
	 */
	Vec3d GetMotorTorque();

	/** @brief 関節変位を取得する
		@return スイング方位角，スイング角，ツイスト角からなるベクトル
	 */
	Vec3d GetAngle();

	/** @brief 関節速度を取得する
		@return スイング方位角，スイング角，ツイスト角の時間変化率からなるベクトル
	 */
	Vec3d GetVelocity();

	/** @brief 関節の出せる最大トルクを設定する
		@param max 最大トルク
	*/
	void SetTorqueMax(double max);

	/** @brief 関節の出せる最大トルクを取得する
	*/
	double GetTorqueMax();

	/** @brief 関節の出せる最小トルクを設定する
		@param min 最小トルク
	*/
	void SetTorqueMin(double min);

	/** @brief 関節の出せる最小トルクを取得する
	*/
	double GetTorqueMin();


};
/// ボールジョイントのディスクリプタ
struct PHBallJointDesc : public PHJointDesc{
	SPR_DESCDEF(PHBallJoint);
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
	SPR_IFDEF(PHSpring);
	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void SetSpring(const Vec3d& spring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	Vec3d GetSpring();

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void SetDamper(const Vec3d& damper);

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	Vec3d GetDamper();

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void SetSpringOri(const double spring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	double GetSpringOri();

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void SetDamperOri(const double damper);

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	double GetDamperOri();
};
/// バネダンパのディスクリプタ
struct PHSpringDesc : public PHConstraintDesc{
	SPR_DESCDEF(PHSpring);
	Vec3d		spring;		///< バネ係数
	Vec3d		damper;		///< ダンパ係数
	double		springOri;
	double		damperOri;
	PHSpringDesc();
};

/// ツリーノードのインタフェース
struct PHTreeNodeIf : public SceneObjectIf{
	SPR_IFDEF(PHTreeNode);
	void Enable(bool bEnable = true);
};
/// ルートノードのインタフェース
struct PHRootNodeIf : public PHTreeNodeIf{
	SPR_IFDEF(PHRootNode);
};
/// １軸関節ノードのインタフェース
struct PHTreeNode1DIf : public PHTreeNodeIf{
	SPR_IFDEF(PHTreeNode1D);
};
struct PHHingeJointNodeIf : public PHTreeNode1DIf{
	SPR_IFDEF(PHHingeJointNode);
};
struct PHSliderJointNodeIf : public PHTreeNode1DIf{
	SPR_IFDEF(PHSliderJointNode);
};
struct PHPathJointNodeIf : public PHTreeNode1DIf{
	SPR_IFDEF(PHPathJointNode);
};
struct PHBallJointNodeIf : public PHTreeNodeIf{
	SPR_IFDEF(PHBallJointNode);
};

/// ギアのインタフェース
struct PHGearIf : public SceneObjectIf{
	SPR_IFDEF(PHGear);
};
	
//@}
//@}

}

#endif

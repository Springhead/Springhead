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

struct PHConstraintEngineDesc{
	int		numIter;					///< 速度更新LCPの反復回数
	int		numIterCorrection;			///< 誤差修正LCPの反復回数
	int		numIterContactCorrection;	///< 接触点の誤差修正LCPの反復回数
	double	velCorrectionRate;			///< 速度のLCPで関節拘束の誤差を修正する場合の誤差修正比率
	double	posCorrectionRate;			///< 位置のLCPで，関節拘束の誤差を修正する場合の誤差修正比率
	double  contactCorrectionRate;		///< 接触の侵入解消のための，速度のLCPでの補正比率．
	double	shrinkRate;					///< LCP初期値を前回の解に対して縮小させる比率
	double	shrinkRateCorrection;
	double	freezeThreshold;			///< 剛体がフリーズする閾値
	double	accelSOR;					///< SOR法の加速係数
	bool	bGearNodeReady;				///< ギアノードがうまく構成されているかのフラグ．ノードやギアを追加・削除するたびにfalseになる
	bool	bSaveConstraints;			///< SaveState, LoadStateに， constraints を含めるかどうか．本来不要だが，f, Fが変化する．
	bool	bUpdateAllState;			///< 剛体の速度，位置の全ての状態を更新する．
	bool	bUseContactSurface;			///< 面接触を使う
	PHConstraintEngineDesc();
};

/** \defgroup gpJoint ジョイント*/
//@{

/// 拘束のディスクリプタ
struct PHConstraintDesc{
	/// 有効/無効フラグ
	bool bEnabled;
	/// 剛体から見た関節の位置と傾き
	Posed poseSocket;
	Posed posePlug;
	PHConstraintDesc():bEnabled(true){}
};

/// 関節のディスクリプタ	<	何もメンバを追加しない場合は，typedefと別名定義を FIDesc.cppに追加で．
//typedef PHConstraintDesc	PHJointDesc;
struct PHJointDesc : public PHConstraintDesc{
	/*enum PHControlMode{
		MODE_TORQUE=0,
		MODE_POSITION,
		MODE_VELOCITY,
		MODE_TRAJ,
	} mode;*/
	enum PHDeformationType{
		ELASTIC =0,
		PLASTIC =1,
		ELASTIC_PLASTIC =2
	}type;

		PHJointDesc();	
};
/// 1軸関節のディスクリプタ
struct PHJoint1DDesc : public PHJointDesc{
	double	lower, upper;	///< 可動範囲. lower < upperのときに有効となる
	double	spring;			///< バネ係数
	double  targetPosition;	///< バネの制御目標
	double  damper;			///< ダンパ係数
	double	targetVelocity; ///< 目標速度
	double	offsetForce;	///< 慣性項を計算して入れる場合に使用．
	//double	torque;		///< モータトルク
	double	rangeSpring;	///< 可動範囲バネ
	double	rangeDamper;	///< 可動範囲ダンパ
	double	fMax;			///< 関節にかけられる最大の力
	double	secondDamper;	///< 二個目のダンパ係数
	double  yieldStress;	///< 降伏応力
	double  hardnessRate;	///< 降伏応力以下の場合に二個目のダンパ係数に掛ける比率
	PHJoint1DDesc();
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

struct PHSceneIf;
/// 拘束のインタフェース
struct PHConstraintIf : public SceneObjectIf{
	SPR_IFDEF(PHConstraint);

	/** @brief ソケット側の剛体を取得する
	 */
	PHSolidIf* GetSocketSolid();

	/** @brief プラグ側の剛体を取得する
	 */
	PHSolidIf* GetPlugSolid();

	/** @brief 所属するシーンの取得	 */
	PHSceneIf* GetScene() const;

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
	
	/** @biref 拘束する剛体間のワールド座標系で見た向きを取得
	*/
	Quaternionf GetAbsolutePoseQ();
	
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

	/** @brief PHMotorの拘束力を取得
		@return 拘束モーメント
	*/
	Vec3d GetMotorf();

	/** @brief PHJointLimitの拘束力を取得
		@return 拘束モーメント
	*/
	Vec3d GetLimitf();

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

	/** @brief 指定された剛体の組に作用している総合的な力を返す
		@param lhs 剛体その１
		@param rhs 剛体その２
		@return 剛体間に作用している総合的な力
	*/
	Vec3d GetTotalForce(PHSolidIf* lhs, PHSolidIf* rhs);
};

/// 接触点拘束のインタフェース
struct PHContactPointIf : public PHConstraintIf{
	SPR_IFDEF(PHContactPoint);
};

/// 関節のインタフェース
struct PHJointIf : public PHConstraintIf{
	SPR_IFDEF(PHJoint);

	/**関節のControlModeの取得,設定する*/
	//PHJointDesc::PHControlMode	GetMode();
	//void	SetMode(PHJointDesc::PHControlMode mode);

	/** @brief 変形のタイプを取得する
		@return 0:ELASTIC,1:PLASTIC,2:ELASTIC_PLASTIC
	 */
	int		GetDefomationType();	
	/** @brief 変形のタイプを設定する
		@param input 0:ELASTIC,1:PLASTIC,2:ELASTIC_PLASTIC
	 */
	void	SetDefomationType(int t);	


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
	void	SetTargetVelocity(double v);

	/** @brief 目標速度を取得する
		@return 目標速度
	 */
	double	GetTargetVelocity();
	
	/** @brief 目標軌道の速度を設定する
		@param v 目標軌道の速度
	*/
	void SetTrajectoryVelocity(double v);

	/** @brief 目標軌道の速度を取得する
		@return 目標軌道の速度
	*/
	double GetTrajectoryVelocity();

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void	SetSpring(double spring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	double	GetSpring();

	/** @brief バネの制御目標を設定する
		@param targetPosition バネの制御目標

		バネ原点とはバネの力が0となる関節変位のこと．
	 */
	void	SetTargetPosition(double targetPosition);

	/** @brief バネの原点を取得する
		@return バネ原点
	 */
	double	GetTargetPosition();

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
	
	/** @brief 定数項を代入する
		@param 代入する値
	*/
	void	SetOffsetForce(double dat);

	/** @brief 補正力を得る
		@return 補正値
	*/
	double	GetOffsetForce();

	/** @brief 関節の出すことができる最大トルク(N)の絶対値を設定する
		@param max ある関節の最大トルク(0～DBL_MAX)
	*/
	void SetTorqueMax(double max);

	/** @brief 関節の出すことができる最大トルク(N)の絶対値を取得する
	*/
	double GetTorqueMax();

	/** @brief 可動域にかかっているかどうかを取得する
		@return かかっていればtrue
	*/
	bool IsLimit();

		/** @brief 二個目のダンパ係数を取得する
		@return 二個目のダンパ係数
	 */
	double  GetSecondDamper();

	/** @brief 二個目のダンパ係数を設定する
		@param input 二個目のダンパ係数
	 */
	void	SetSecondDamper(double input);
	/** @brief 降伏応力を設定する
		@return 降伏応力
	 */
	double GetYieldStress();
	/** @brief 降伏応力を取得する
		@param input 降伏応力
	 */
    void SetYieldStress(const double yS);
	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を設定する
		@return 硬くする倍率
	 */
	double GetHardnessRate();
	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を取得する
		@param input 硬くする倍率
	 */
	void SetHardnessRate(const double hR);
	
	/** @brief 変形のタイプを表示する
		@return 変形のタイプ
	 */
	PHJointDesc::PHDeformationType 	GetDeformationMode();


	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void	SetRangeSpring(double rSpring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	double	GetRangeSpring();

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void	SetRangeDamper(double rDamper);

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	double	GetRangeDamper();
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
	
	bool bConstraintY;
	bool bConstraintRollX;
	bool bConstraintRollZ;

	PHSliderJointDesc();
};

/// パス上の1つの点
struct PHPathPoint{
	double	s;			///< スカラーパラメータ
	Posed	pose;		///< 位置と傾き
	PHPathPoint(){}
	PHPathPoint(double _s, Posed _pose):s(_s), pose(_pose){}
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
struct PHBallJointIf : public PHJointIf{
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
	/** @brief 極値でのツイスト角の可動範囲を設定する。ConstLineとともに使用
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
		可動範囲制限を無効化するにはディスクリプタで書き換えなければ良い
		（デフォルトで可動域制限は無効になっている）
	 */
	void SetTwistPole(Vec2d range);
	/**@brief 拘束位置を設定する
	   @param 拘束位置のファイルを読み込む
    */
	bool SetConstLine(char* fileName, bool i);
	/**@brief 拘束位置を設定する
	   @param [way][0]スイング方位角　[1]スイング角　[2]ツイスト角min　[3]ツイスト角max　[4]傾き
    */
	void SetConstPoint(int Num, int way, double a);
	/**@brief 拘束位置を取得する
	   @param [way][0]スイング方位角　[1]スイング角　[2]ツイスト角min　[3]ツイスト角max　[4]傾き
    */
	double GetConstLine(int Num, int way);

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

	/**  @brief 関節変位を取得する
		 @return Xjrel.q(親剛体から見た子剛体の相対変位をQuaternionで表現したもの)
	*/
	Quaterniond GetPosition();

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

	/** @brief 制御の目標向きを設定する
	*/
	void SetTargetPosition(Quaterniond p);
	
	/** @brief 制御の目標向きを取得する
	*/
	Quaterniond GetTargetPosition();

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void	SetSpring(double spring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	double	GetSpring();

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void	SetDamper(double damper);

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	double	GetDamper();

	/** @brief 速度制御に切り替え，速度を設定する
	*/
	void  SetTargetVelocity(Vec3d q);

	/** @brief 速度制御の目標速度を取得する
	*/
	Vec3d GetTargetVelocity();

	/** @brief 軌道追従制御に切り替え，目標速度を設定する
	*/
	void SetTrajectoryVelocity(Vec3d q);
	
	/** @brief 軌道追従制御の目標速度項を取得する
	*/
	Vec3d GetTrajectoryVelocity();

	/** @brief 定数項を設定する	
	*/
	void SetOffsetForce(Vec3d ofst);

	/**	@brief 定数項を取得する
	*/
	Vec3d GetOffsetForce();

	/** @brief 可動域にかかっているかどうかを取得する
		@return かかっていればtrue
	*/
	bool IsLimit();

	/** @brief 二個目のダンパ係数を取得する
		@return 二個目のダンパ係数
	 */
	double  GetSecondDamper();

	/** @brief 二個目のダンパ係数を設定する
		@param input 二個目のダンパ係数
	 */
	void	SetSecondDamper(double input);
	/** @brief 降伏応力を設定する
		@return 降伏応力
	 */
	double GetYieldStress();
	/** @brief 降伏応力を取得する
		@param input 降伏応力
	 */
    void SetYieldStress(const double yS);
	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を設定する
		@return 硬くする倍率
	 */
	double GetHardnessRate();
	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を取得する
		@param input 硬くする倍率
	 */
	void SetHardnessRate(const double hR);
	
	/** @brief 断面2次モーメントを設定する
		@return 断面2次モーメントVec3d(x,y,z)
	 */
	Vec3d	GetInertia();	
	/** @brief 断面2次モーメントを設定する
		@param input 断面2次モーメントVec3d(x,y,z)
	 */
	void	SetInertia(const Vec3d i);
	
	/** @brief 変形のタイプを表示する
		@return 変形のタイプ
	 */
	PHJointDesc::PHDeformationType 	GetDeformationMode();

	/** @brief Jointに加わっている力の過去5ステップ分を平均したノルムを取得する
		@return Jointに加わっている力の過去5ステップ分を平均したノルム
	 */
	double GetmotorfNorm();	

	void SetConstraintMode(int t);

};

/// ボールジョイントのディスクリプタ
struct PHBallJointDesc : public PHJointDesc{
	SPR_DESCDEF(PHBallJoint);
	double			spring;			 ///< バネ係数
	double			damper;			 ///< ダンパ係数
	Vec2d			limitSwing;		 ///< swing角の可動域（[0] or .lower, [1] or .upper）
	Vec2d			limitSwingDir;	 ///< swing方位角の可動域（[0] or .lower, [1] or .upper）
	Vec2d			limitTwist;		 ///< twist角の可動域（[0] or .lower, [1] or .upper）
	Vec3d			limitDir;		 ///< 可動域の中心ベクトル
	Quaterniond		targetPosition;  ///< バネダンパの制御目標
	Vec3d			targetVelocity;  ///< 目標となる回転ベクトル
	Vec3d			offsetForce;	 ///< 定数項（軌道追従制御の加速度の項を入れるのに使ったりする）
	//Vec3d			torque;			 ///< モータトルク
	double			fMax;			 ///< 関節にかけられる最大の力(絶対値)
	Vec2d			poleTwist;		 ///< spline拘束でSwing角0のときのTwist角可動域（[0] or .lower, [1] or .upper）
	
	double secondDamper;			///< 第２ダンパ係数
	double yieldStress;				///< 降伏応力
	double hardnessRate;			///< 降伏応力以下の場合に二個目のダンパ係数に掛ける比率
	Vec3d  Inertia;					///< 断面２次モーメント
	
	enum PHConstraintType{
		non = 0,
		SwingTwist =1,
		Spline =2,
		Jacobian =3
	}ConstMode;

	PHBallJointDesc();		///< ディスクリプタのコンストラクタ
};


/// バネダンパのインタフェース
struct PHSpringIf : public PHJointIf{
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
struct PHSpringDesc : public PHJointDesc{
	SPR_DESCDEF(PHSpring);
	Vec3d		spring;		///< バネ係数
	Vec3d		damper;		///< ダンパ係数
	double		springOri;
	double		damperOri;
	double		fMax;			///< 関節にかけられる最大の力
	Vec3d		secondDamper;		// 二個目のダンパ係数
	double		yieldStress;		// 降伏応力
	double		hardnessRate;		// 降伏応力以下の場合に二個目のダンパ係数に掛ける比率
	
	PHSpringDesc();
};

/// ツリーノードのインタフェース
struct PHTreeNodeIf : public SceneObjectIf{
	SPR_IFDEF(PHTreeNode);

	/** @brief 有効・無効を切り替える
	 */
	void Enable(bool bEnable = true);

	/** @brief 有効・無効を返す
	 */
	bool IsEnabled();

	/** @brief 子ノードの数を取得する
	 */
	int NChildren();

	/** @brief 子ノードを取得する
	 */
	PHTreeNodeIf* GetChild(int i);

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

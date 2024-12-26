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
#include <Physics/SprPHJointLimit.h>
#include <Physics/SprPHJointMotor.h>

#include <float.h>  // FLT_MAX

#include <Base/Spatial.h>

namespace Spr{;

struct PHSolidIf;

/** \addtogroup gpPhysics */
//@{

/** \defgroup gpJoint ジョイント*/
//@{

/// 拘束のデスクリプタ
struct PHConstraintDesc{
	bool bEnabled;      ///< 有効/無効フラグ
	Posed poseSocket;   ///< 剛体から見た関節の位置と傾き
	Posed posePlug;

	PHConstraintDesc():bEnabled(true){}
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

	/** @brief ツリーを構成しているかを取得する
	 */
	bool IsArticulated();

	/** @brief ソケットの位置・向きを取得・設定する
	 */
	void GetSocketPose(Posed& pose);
	void SetSocketPose(const Posed& pose);

	/** @brief プラグの位置・向きを取得・設定する
	 */
	void GetPlugPose(Posed& pose);
	void SetPlugPose(const Posed& pose);
	
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

	// <!!>
	/** @brief 拘束力を取得
	 */
	//Vec3d GetForce();

	/** @brief 拘束トルクを取得
	 */
	//Vec3d GetTorque();
	/**
	 */
	bool IsYielded();

	/** @biref 状態の更新
	*/
	void UpdateState();
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

// -----  -----  -----  -----  -----

/// 接触点拘束のインタフェース
struct PHContactPointIf : public PHConstraintIf{
	SPR_IFDEF(PHContactPoint);
	Posed GetPose();
	bool IsStaticFriction();
};

// -----  -----  -----  -----  -----

/// 関節のデスクリプタ
struct PHJointDesc : public PHConstraintDesc {
	double	fMax;			///< 関節の出すことができる最大の力
	PHJointDesc() {
		fMax = FLT_MAX;
	}
};

/// 関節のインタフェース
struct PHJointIf : public PHConstraintIf{
	SPR_IFDEF(PHJoint);

	/** @brief 関節の出すことができる最大拘束力(N)の絶対値を設定する
		@param max ある関節の最大拘束力(0～DBL_MAX)
	*/
	void SetMaxForce(double max);

	/** @brief 関節の出すことができる最大拘束力(N)の絶対値を取得する
	*/
	double GetMaxForce();
};

// -----  -----  -----  -----  -----

/// 1軸関節のデスクリプタ
struct PH1DJointDesc : public PHJointDesc {
	bool   cyclic;
	double spring;
	double damper;
	double secondDamper;
	double targetPosition;
	double targetVelocity;
	double offsetForce;
	double yieldStress;
	double hardnessRate;
//	double secondMoment;

	PH1DJointDesc() {
		cyclic          = false;
		spring          = 0;
		damper          = 0;
		secondDamper    = FLT_MAX;
		targetPosition  = 0;
		targetVelocity  = 0;
		offsetForce     = 0;
		yieldStress     = FLT_MAX;
		hardnessRate    = 1.0;
	}
};

// struct PH1DJointLimitIf;
// struct PH1DJointLimitDesc;
/// 1軸関節のインタフェース
struct PH1DJointIf : public PHJointIf{
	SPR_IFDEF(PH1DJoint);

	/** @brief 可動域制限を作成する
	 */
	PH1DJointLimitIf* CreateLimit(const PH1DJointLimitDesc& desc = PH1DJointLimitDesc());

	/** @brief 駆動モータを追加・削除する
	 */
	PH1DJointMotorIf* CreateMotor(const IfInfo* ii, const PH1DJointMotorDesc& desc = PH1DJointMotorDesc());
	template <class T> PH1DJointMotorIf* CreateMotor(const T& desc){
		return CreateMotor(T::GetIfInfo(), desc);
	}
	bool              AddMotor(PH1DJointMotorIf* m);
	bool              RemoveMotor(int n);

	/** @brief cyclicかどうかを取得
		trueの場合，positionが[-pi, pi]の範囲で循環する．
	 */
	bool	IsCyclic();

	/** @brief cyclicかどうかを設定
	 */
	void	SetCyclic(bool on);

	/** @brief 関節変位を取得する
		@return 関節変位
	 */
	double	GetPosition();

	/** @brief 関節速度を取得する
		@return 関節速度
	 */
	double	GetVelocity();

	/** @brief 関節可動域拘束を取得する
	@return 関節可動域拘束
	*/
	bool HasLimit();

	/** @brief 関節可動域拘束を取得する
		@return 関節可動域拘束
	 */
	PH1DJointLimitIf* GetLimit();

	/** @brief バネ係数を設定する
		@param spring バネ係数
	 */
	void SetSpring(double spring);

	/** @brief バネ係数を取得する
		@return バネ係数
	 */
	double GetSpring();

	/** @brief ダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void	SetDamper(double damper);

	/** @brief ダンパ係数を取得する
		@return ダンパ係数
	 */
	double	GetDamper();

	/** @brief 二個目のダンパ係数を取得する
		@return 二個目のダンパ係数
	 */
	double  GetSecondDamper();

	/** @brief 二個目のダンパ係数を設定する
		@param input 二個目のダンパ係数
	 */
	void	SetSecondDamper(double input);

	/** @brief バネの制御目標を設定する
		@param targetPosition バネの制御目標

		バネ原点とはバネの力が0となる関節変位のこと．
	 */
	void	SetTargetPosition(double targetPosition);

	/** @brief バネの原点を取得する
		@return バネ原点
	 */
	double	GetTargetPosition();

	/** @brief 目標速度を設定する
		@param vel 目標速度
	 */
	void	SetTargetVelocity(double v);

	/** @brief 目標速度を取得する
		@return 目標速度
	 */
	double GetTargetVelocity();

	/** @brief 定数項を代入する
		@param 代入する値
	*/
	void SetOffsetForce(double dat);

	/** @brief 補正力を得る
		@return 補正値
	*/
	double GetOffsetForce();

	/** @brief N番モータの定数項を代入する
	@param N番モータに代入する値
	*/
	void SetOffsetForceN(int n, double dat);

	/** @brief N番モータの補正力を得る
	@return N番モータの補正値
	*/
	double GetOffsetForceN(int n);

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

	/** @brief 断面二次モーメントを設定する
		@param secondMoment 断面二次モーメント
	 */
	//void SetSecondMoment(const double& sM);

	/** @brief 断面二次モーメントを取得する
		@return 断面二次モーメント
	 */
	//double GetSecondMoment();

	/** @brief モータ数を返す
	@return モータ数
	*/
	int NMotors();

	/** @brief モータ群を返す
	    @return motors
	*/
	PH1DJointMotorIf** GetMotors();

	/** @brief Motorの出力した力を返す
		@return 力
	*/
	double GetMotorForce();

	/** @brief N番Motorの出力した力を返す
	@ return 力
	*/
	double GetMotorForceN(int n);

	/** @brief Limitの出力した力を返す
	@ return 力
	*/
	double GetLimitForce();
};

/// ヒンジのインタフェース
struct PHHingeJointIf : public PH1DJointIf{
	SPR_IFDEF(PHHingeJoint);
};

/// ヒンジのデスクリプタ
struct PHHingeJointDesc : public PH1DJointDesc{
	SPR_DESCDEF(PHHingeJoint);
};

/// スライダのインタフェース
struct PHSliderJointIf : public PH1DJointIf{
	SPR_IFDEF(PHSliderJoint);
};

/// スライダのデスクリプタ
struct PHSliderJointDesc : public PH1DJointDesc{
	SPR_DESCDEF(PHSliderJoint);
	// 以下のオプションは，現状ではABAを有効にするとうまく機能しないので注意．<!!>
	bool bConstraintY;      ///< 並進Y軸を拘束するか デフォルトtrue
	bool bConstraintRollX;  ///< 回転X軸を拘束するか デフォルトtrue
	bool bConstraintRollZ;  ///< 回転Z軸を拘束するか デフォルトtrue

	PHSliderJointDesc() {
		bConstraintY		= true;
		bConstraintRollX	= true;
		bConstraintRollZ	= true;
	}
};

// -----  -----  -----  -----  -----

/// パス上の1つの点
struct PHPathPoint{
	double	s;			///< スカラーパラメータ
	Posed	pose;		///< 位置と傾き
	PHPathPoint(){}
	PHPathPoint(double _s, Posed _pose):s(_s), pose(_pose){}
};

/// パスのデスクリプタ
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
struct PHPathJointIf : public PH1DJointIf{
	SPR_IFDEF(PHPathJoint);
	void SetPosition(double q);
};
/// パスジョイントのデスクリプタ
struct PHPathJointDesc : public PH1DJointDesc{
	SPR_DESCDEF(PHPathJoint);
	PHPathJointDesc(){}
};

// -----  -----  -----  -----  -----
/** ユーザサイドで完全カスタマイズ可能な関節
 */
struct PHGenericJointIf;

class PHGenericJointCallback{
public:
	virtual void SetParam(PHGenericJointIf* jnt, const std::string& name, double value){}
	virtual bool IsCyclic(PHGenericJointIf* jnt){ return false; }
	// LCP用関数
	virtual void GetMovableAxes        (PHGenericJointIf* jnt, int& n, int* indices){}
	virtual void CompBias              (PHGenericJointIf* jnt, Vec3d&  dbv, Vec3d&  dbw, const Vec3d& prel, const Quaterniond& qrel, const Vec3d& vrel, const Vec3d& wrel){}
	virtual void CompError             (PHGenericJointIf* jnt, Vec3d&  Bv , Vec3d&  Bw , const Vec3d& prel, const Quaterniond& qrel                                      ){}
	virtual void UpdateJointState      (PHGenericJointIf* jnt, double& pos, double& vel, const Vec3d& prel, const Quaterniond& qrel, const Vec3d& vrel, const Vec3d& wrel){}
	// ABA用関数
	virtual void CompJointJacobian     (PHGenericJointIf* jnt, Vec3d& Jv  , Vec3d&       Jw  , double pos            ){}
	virtual void CompJointCoriolisAccel(PHGenericJointIf* jnt, Vec3d& cv  , Vec3d&       cw  , double pos, double vel){}
	virtual void CompRelativePosition  (PHGenericJointIf* jnt, Vec3d& prel, Quaterniond& qrel, double pos            ){}
	virtual void CompRelativeVelocity  (PHGenericJointIf* jnt, Vec3d& vrel, Vec3d&       wrel, double pos, double vel){}
};
struct PHGenericJointIf : public PH1DJointIf{
	SPR_IFDEF(PHGenericJoint);
	void SetCallback(PHGenericJointCallback* cb);
	void SetParam   (const std::string& name, double value);
};
struct PHGenericJointDesc : public PH1DJointDesc{
	SPR_DESCDEF(PHGenericJoint);
	PHGenericJointDesc(){}
};

// -----  -----  -----  -----  -----

// struct PHBallJointLimitIf;
// struct PHBallJointLimitDesc;
struct PHBallJointMotorIf;
/// ボールジョイントのインタフェース
struct PHBallJointIf : public PHJointIf{
	SPR_IFDEF(PHBallJoint);

	/** @brief 可動域制限を作成する
	 */
	PHBallJointLimitIf* CreateLimit(const IfInfo* ii, const PHBallJointLimitDesc& desc = PHBallJointLimitDesc());
	template <class T> PHBallJointLimitIf* CreateLimit(const T& desc){
		return CreateLimit(T::GetIfInfo(), desc);
	}

	/** @brief 駆動モータを追加・削除する
	*/
	PHBallJointMotorIf* CreateMotor(const IfInfo* ii, const PHBallJointMotorDesc& desc = PHBallJointMotorDesc());
	template <class T> PHBallJointMotorIf* CreateMotor(const T& desc){
		return CreateMotor(T::GetIfInfo(), desc);
	}
	bool              AddMotor(PHBallJointMotorIf* m);
	bool              RemoveMotor(int n);

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

	/** @brief 関節可動域拘束を取得する
	@return 関節可動域拘束
	*/
	bool HasLimit();

	/** @brief 関節可動域拘束を取得する
		@return 関節可動域拘束
	 */
	PHBallJointLimitIf* GetLimit();

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

	/** @brief 二個目のダンパ係数を取得する
		@return 二個目のダンパ係数
	 */
	Vec3d  GetSecondDamper();

	/** @brief 二個目のダンパ係数を設定する
		@param input 二個目のダンパ係数
	 */
	void	SetSecondDamper(Vec3d damper2);

	/** @brief 制御の目標向きを設定する
	*/
	void SetTargetPosition(Quaterniond p);
	
	/** @brief 制御の目標向きを取得する
	*/
	Quaterniond GetTargetPosition();

	/** @brief 速度制御の目標速度を設定する
	*/
	void  SetTargetVelocity(Vec3d q);

	/** @brief 速度制御の目標速度を取得する
	*/
	Vec3d GetTargetVelocity();

	/** @brief 力のオフセットを設定する	
	*/
	void SetOffsetForce(Vec3d ofst);

	/**	@brief 力のオフセットを取得する
	*/
	Vec3d GetOffsetForce();

	/** @brief N番モータの定数項を代入する
	@param N番モータに代入する値
	*/
	void SetOffsetForceN(int n, Vec3d ofst);

	/** @brief N番モータの補正力を得る
	@return N番モータの補正値
	*/
	Vec3d GetOffsetForceN(int n);

	/** @brief 降伏応力を取得する
		@param input 降伏応力
	 */
    void SetYieldStress(const double yS);

	/** @brief 降伏応力を設定する
		@return 降伏応力
	 */
	double GetYieldStress();

	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を取得する
		@param input 硬くする倍率
	 */
	void SetHardnessRate(const double hR);
	
	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を設定する
		@return 硬くする倍率
	 */
	double GetHardnessRate();

	/** @brief 断面2次モーメントを設定する
		@param input 断面2次モーメントVec3d(x,y,z)
	 */
	//void	SetSecondMoment(const Vec3d m);

	/** @brief 断面2次モーメントを設定する
		@return 断面2次モーメントVec3d(x,y,z)
	 */
	//Vec3d	GetSecondMoment();	

	/** @brief モータ数を返す
	    @return モータ数
	*/
	int NMotors();

	/** @brief モータ群を返す
	@return motors
	*/
	PHBallJointMotorIf** GetMotors();

	/** @brief Motorの出力した力を返す
		@return 力
	*/
	Vec3d GetMotorForce();

	/** @brief N番Motorの出力した力を返す
	    @ return 力
	*/
	Vec3d GetMotorForceN(int n);

	/** @brief Limitの出力した力を返す
	@ return 力
	*/
	Vec3d GetLimitForce();
};

/// ボールジョイントのデスクリプタ
struct PHBallJointDesc : public PHJointDesc {
	SPR_DESCDEF(PHBallJoint);
	double spring;
	double damper;
	Vec3d  secondDamper;
	Quaterniond targetPosition;
	Vec3d  targetVelocity;
	Vec3d  offsetForce;
	double yieldStress;
	double hardnessRate;
//	Vec3d  secondMoment;

	PHBallJointDesc() {
		spring          = 0;
		damper          = 0;
		secondDamper    = Vec3d(FLT_MAX, FLT_MAX, FLT_MAX);
		targetPosition  = Quaterniond();
		targetVelocity  = Vec3d();
		offsetForce     = Vec3d();
		yieldStress     = FLT_MAX;
		hardnessRate    = 1.0;
	}
};

// -----  -----  -----  -----  -----
/** 固定関節
	自由度を持たない関節．剛体同士の固定に用いる
 */
struct PHFixJointIf : public PHJointIf{
	SPR_IFDEF(PHFixJoint);
};
struct PHFixJointDesc : public PHJointDesc{
	SPR_DESCDEF(PHFixJoint);
};

// -----  -----  -----  -----  -----

struct PHSpringMotorIf;
/// バネダンパのインタフェース
struct PHSpringIf : public PHJointIf{
	SPR_IFDEF(PHSpring);

	/** @brief 目標位置を設定する
		@param targetPosition 目標位置
	*/
	void SetTargetPosition(const Vec3d& targetPosition);

	/** @brief 目標位置を取得する
		@return 目標位置
	*/
	Vec3d GetTargetPosition();

	/** @brief 目標姿勢を設定する
		@param targetOrientation 目標姿勢
	*/
	void SetTargetOrientation(const Quaterniond& targetOrientation);

	/** @brief 目標姿勢を取得する
		@return 目標姿勢
	*/
	Quaterniond GetTargetOrientation();

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

	/** @brief 並進の第二ダンパ係数を設定する
		@param secondDamper 並進の第二ダンパ係数
	 */
	void SetSecondDamper(const Vec3d& secondDamper);

	/** @brief 並進の第二ダンパ係数を取得する
		@return 並進の第二ダンパ係数
	 */
	Vec3d GetSecondDamper();

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

	/** @brief 回転の第二ダンパ係数を設定する
		@param secondDamperOri 回転の第二ダンパ係数
	 */
	void SetSecondDamperOri(const double& secondDamperOri);

	/** @brief 回転の第二ダンパ係数を取得する
		@return 回転の第二ダンパ係数
	 */
	double GetSecondDamperOri();

	/** @brief 降伏応力を設定する
		@param yieldStress 降伏応力
	 */
	void SetYieldStress(const double& yieldStress);

	/** @brief 降伏応力を取得する
		@return 降伏応力
	 */
	double GetYieldStress();

	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を設定する
		@return 硬くする倍率
	 */
	void SetHardnessRate(const double& hardnessRate);

	/** @brief 降伏応力以下の場合にダンパを硬くする倍率を取得する
		@param input 硬くする倍率
	 */
	double GetHardnessRate();

	/** @brief 断面二次モーメントを設定する
		@param secondMoment 断面二次モーメント
	 */
	//void SetSecondMoment(const Vec3d& secondMoment);

	/** @brief 断面二次モーメントを取得する
		@return 断面二次モーメント
	 */
	//Vec3d GetSecondMoment();

	/** @brief バネの発揮している力を取得する
	 */
	Vec6d GetMotorForce();

	/** @brief バネの発揮している力を設定する
	 */
	void SetOffsetForce(const Vec6d& offsetForce);

	/** @brief 目標速度を設定する
	 */
	void SetTargetVelocity(const Vec6d& targetVelocity);

	/** @brief 目標速度を取得する
	 */
	Vec6d GetTargetVelocity();
};

/// バネダンパのデスクリプタ
struct PHSpringDesc : public PHJointDesc {
	SPR_DESCDEF(PHSpring);
	Vec3d targetPosition;
	Quaterniond targetOrientation;
	Vec3d  spring;
	Vec3d  damper;
	Vec3d  secondDamper;
	double springOri;
	double damperOri;
	double secondDamperOri;
	double yieldStress;
	double hardnessRate;
//	Vec3d  secondMoment;
	Vec6d  targetVelocity;
	Vec6d  offsetForce;

	PHSpringDesc() {
		targetPosition    = Vec3d();
		targetOrientation = Quaterniond();
		spring            = Vec3d();
		damper            = Vec3d();
		secondDamper      = Vec3d(FLT_MAX, FLT_MAX, FLT_MAX);
		springOri         = 0;
		damperOri         = 0;
		secondDamperOri   = FLT_MAX;
		yieldStress       = FLT_MAX;
		hardnessRate      = 1.0;
		targetVelocity    = Vec6d();
		offsetForce       = Vec6d();
	}
};

// -----  -----  -----  -----  -----
/** 合致拘束
	多様な幾何拘束を課すのに利用
	3DCADのアセンブリ拘束のようなイメージ
	PHJointとの違いは
	・PHJointLimitがない
	・PHJointMotorがない
	・PHTreeNodeでABAが利用できない
 */
struct PHMateIf : PHJointIf{
	SPR_IFDEF(PHMate);
};
struct PHMateDesc : PHJointDesc{
	SPR_DESCDEF(PHMate);
};

/** 点-点対偶
	ソケットとプラグの原点を一致させる
	ボールジョイントと似ているが低機能な分高速
 */
struct PHPointToPointMateIf : PHMateIf{
	SPR_IFDEF(PHPointToPointMate);
};
struct PHPointToPointMateDesc : PHMateDesc{
	SPR_DESCDEF(PHPointToPointMate);
};
/** 点-線分対偶
	プラグの原点をソケットのz軸上に拘束する
	軸上の範囲も指定可
 */
struct PHPointToLineMateIf : PHMateIf{
	SPR_IFDEF(PHPointToLineMate);
};
struct PHPointToLineMateDesc : PHMateDesc{
	SPR_DESCDEF(PHPointToLineMate);
};
/** 点-面対偶
	プラグの原点をソケットのxy平面上に拘束する
	面上(z方向)の範囲も指定可
 */
struct PHPointToPlaneMateIf : PHMateIf{
	SPR_IFDEF(PHPointToPlaneMate);
	void	SetRange(Vec2d  range);
	void	GetRange(Vec2d& range);
};
struct PHPointToPlaneMateDesc : PHMateDesc{
	SPR_DESCDEF(PHPointToPlaneMate);
	Vec2d	range;
};
/** 線-線対偶
	プラグのz軸とソケットのz軸を一致させる
 */
struct PHLineToLineMateIf : PHMateIf{
	SPR_IFDEF(PHLineToLineMate);
};
struct PHLineToLineMateDesc : PHMateDesc{
	SPR_DESCDEF(PHLineToLineMate);
};
/** 面-面対偶
	プラグのxy平面とソケットのxy平面を一致させる
 */
struct PHPlaneToPlaneMateIf : PHMateIf{
	SPR_IFDEF(PHPlaneToPlaneMate);
};
struct PHPlaneToPlaneMateDesc : PHMateDesc{
	SPR_DESCDEF(PHPlaneToPlaneMate);
};

// -----  -----  -----  -----  -----

/// ツリーノードのデスクリプタ
struct PHTreeNodeDesc{
	bool bEnabled;
	PHTreeNodeDesc(){
		bEnabled = true;
	}
};

struct PHRootNodeState {
	Posed nextPose;
	bool useNextPose;

	PHRootNodeState() {
		nextPose = Posed();
		useNextPose = false;
	}
};


struct PHRootNodeDesc : public PHTreeNodeDesc, PHRootNodeState{
	PHRootNodeDesc(){}
};
struct PHTreeNode1DDesc      : public PHTreeNodeDesc{};
struct PHHingeJointNodeDesc  : public PHTreeNode1DDesc{};
struct PHSliderJointNodeDesc : public PHTreeNode1DDesc{};
struct PHPathJointNodeDesc   : public PHTreeNode1DDesc{};
struct PHGenericJointNodeDesc: public PHTreeNode1DDesc{};
struct PHBallJointNodeDesc   : public PHTreeNodeDesc{};
struct PHFixJointNodeDesc    : public PHTreeNodeDesc{};

struct PHRootNodeIf;

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

	/** @brief 親ノードを取得する
	 */
	PHTreeNodeIf* GetParentNode();

	/** @brief 子ノードを取得する
	 */
	PHTreeNodeIf* GetChildNode(int i);

	/** @brief 根ノードを取得する
	 */
	PHRootNodeIf* GetRootNode();

	/** @brief 関連づけられている剛体を取得する
	 */
	PHSolidIf* GetSolid();

	/** @brief Articulated Inertiaを取得する
	 */
	Spr::SpatialMatrix GetI();

	/** @brief コリオリ力を取得する
	 */
	Spr::SpatialVector GetIc();

	/** @brief コリオリ加速度を取得する
	 */
	Spr::SpatialVector GetC();

	/** @brief Articulated Bias Forceを取得する
	 */
	Spr::SpatialVector GetZ();

	/** @brief 子ノードから親ノードへの座標変換Xcpを取得する
	 */
	Spr::SpatialMatrix GetXcp_mat();

	/** @brief Setupを実行する
	 */
	void Setup();

	/** @brief ABA用の慣性テンソルを初期化
	 */
	void InitArticulatedInertia();

	/** @brief ABA用の慣性テンソルを計算
	 */
	void CompArticulatedInertia();

};
/// ルートノードのインタフェース
struct PHRootNodeIf : public PHTreeNodeIf{
	SPR_IFDEF(PHRootNode);

	/** @brief コールバック関数の型
	 */
	typedef void (* CompControlForce)(PHRootNodeIf*, void*);

	/** @brief 剛体の位置と向きを上書きするか取得する
		@return trueならば剛体の位置と向きは上書きされる
	 */
	bool IsUseNextPose();

	/** @brief 剛体の位置と向きを上書きするか設定する
		@param bOn trueならば剛体の位置と向きは上書きされる
	 */
	void SetUseNextPose(bool bOn);

	/** @brief 剛体の位置と向きを上書きするための設定する
		@param p シーンに対する剛体の位置と向き
	 */
	void SetNextPose(const Posed& p);

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
struct PHGenericJointNodeIf : public PHTreeNode1DIf{
	SPR_IFDEF(PHGenericJointNode);
};
struct PHBallJointNodeIf : public PHTreeNodeIf{
	SPR_IFDEF(PHBallJointNode);
};
struct PHFixJointNodeIf : public PHTreeNodeIf{
	SPR_IFDEF(PHFixJointNode);
};

// -----  -----  -----  -----  -----

/// ギアのデスクリプタ
struct PHGearDesc{
	enum{
		MODE_VEL,		///< ratio * v0 = v1
		MODE_POS,		///< ratio * p0 + offset = p1
	};
	bool   bEnabled;
	double ratio;		///< ギア比
	double offset;
	int    mode;

	PHGearDesc(){
		bEnabled = true;
		ratio    = 1.0;
		offset   = 0.0;
		mode     = MODE_VEL;
	}
};

/// ギアのインタフェース
struct PHGearIf : public SceneObjectIf{
	SPR_IFDEF(PHGear);

	void   Enable(bool bEnable = true);
	bool   IsEnabled();

	void   SetRatio(double ratio);
	double GetRatio();

	void   SetOffset(double offset);
	double GetOffset();

	void   SetMode(int mode);
	int    GetMode();
};
	
//@}
//@}

}

#endif

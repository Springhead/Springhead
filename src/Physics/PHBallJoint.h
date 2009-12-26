/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 *  PHBallJointはswingによる倒しこみ角度の制限と、twistによるねじりの制限ができます。
 *
 */
#ifndef PHBALLJOINT_H
#define PHBALLJOINT_H

#include <SprPhysics.h>
#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>
#include <Physics/PHJointLimit.h>
#include <Physics/PHMotor.h>

namespace Spr{

class PHBallJoint;
///	ボールジョイントに対応するツリーノード
class PHBallJointNode : public PHTreeNodeND<3>{
public:
	SPR_OBJECTDEF1(PHBallJointNode, PHTreeNode);
	SPR_DECLMEMBEROF_PHBallJointNodeDesc;

	/// スイング・ツイスト角の時間変化率から角速度へのヤコビアン
	Matrix3d	Jst;

	PHBallJoint*		GetJoint(){return PHTreeNodeND<3>::GetJoint()->Cast();}
	virtual void		CompJointJacobian();
	virtual void		CompJointCoriolisAccel();
	virtual void		UpdateJointPosition(double dt);
	virtual void		CompRelativePosition();
	virtual void		CompRelativeVelocity();
	//virtual void		ModifyJacobian();
	//virtual void		CompBias();
	//virtual void		Projection(double& f, int i);
	PHBallJointNode(const PHBallJointNodeDesc& desc = PHBallJointNodeDesc()){}
	
};

enum PHDeformationType;
class PHBallJoint : public PHJointND<3>{
public:
	SPR_OBJECTDEF1(PHBallJoint, PHJoint);
	SPR_DECLMEMBEROF_PHBallJointDesc;
protected:
	friend class PHBallJointLimit;
	friend class PHBallJointMotor;
	PHBallJointLimit	limit;		///< 可動範囲拘束
	PHBallJointMotor	motor;		///< 関節コントローラ

	Matrix3d		Jc;						///< 拘束座標系の速度・加速度　＝　Jc * Socket座標系から見たPlug座標系の速度、加速度
	Matrix3d		Jcinv;					///< Socket座標系から見たPlug座標系の速度、加速度  ＝　Jcinv * 拘束座標系の速度・加速度
	
	// 軌道追従制御用の変数，消さないで by Toki Aug. 2008
	Quaterniond qd,	 preQd;		///< ある時刻の目標位置への回転軸ベクトルと１時刻前の目標位置への回転軸ベクトル
	Vec3d qdDot, preQdDot;	///< ある時刻の目標位置への回転軸ベクトルの一階微分と１時刻前の目標位置への回転軸ベクトルの一階微分
	Vec3d qdWDot;			///< ある時刻の目標位置への回転軸ベクトルの二階微分

	//virtual void	ControlCheck(double dtinv);	//< 制御をかけるかのチェック
	//virtual void	MovableCheck(double dtinv);	//< 可動域をかけるかのチェック
	//virtual void	AfterSetDesc();
	//virtual coord_t GetTorqueND(){ return (coord_t&)torque; }
	friend class PHBallJointNode;
public:
	/// インタフェースの実装
	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	
	virtual void		SetSwingRange(Vec2d  range)	{ limitSwing = range; }			///< スイング角の範囲を設定する関数
	virtual void		GetSwingRange(Vec2d& range) { range = limitSwing; }			///< スイング角の範囲を得る関数
	virtual void		SetTwistRange(Vec2d  range)	{ limitTwist = range; }			///< ツイスト角の範囲を設定する関数
	virtual void		GetTwistRange(Vec2d& range) { range = limitTwist; }			///< ツイスト角の範囲を得る関数

	virtual void		SetMotorTorque(const Vec3d& t){offsetForce = t;}			///< モータのトルクを設定する関数
	virtual Vec3d		GetMotorTorque(){return offsetForce;}						///< モータのトルクを返す関数
	virtual Vec3d		GetAngle(){return position;}								///< 角度を返す関数
	virtual Quaterniond GetPosition(){return Xjrel.q;}								///< 角度のクォータニオンを返す関数
	virtual Vec3d		GetVelocity(){return velocity;}								///< 速度を返す関数
	virtual void		SetTorqueMax(double max){fMax = fabs(max); }
	virtual double		GetTorqueMax(){return fMax;}
	virtual void		SetTargetVelocity(Vec3d v){ targetVelocity = v;}
	virtual Vec3d		GetTargetVelocity() const {return targetVelocity;}
	virtual void		SetTrajectoryVelocity(Vec3d v) { targetVelocity = v;}
	virtual Vec3d		GetTrajectoryVelocity() const {return targetVelocity;}
	virtual void		SetTargetPosition(Quaterniond p){ targetPosition = p;}
	virtual Quaterniond GetTargetPosition(){return targetPosition;}

	virtual void		SetSpring(double spring){ this->spring = spring; }
	virtual double		GetSpring(){ return spring; }
	virtual void		SetDamper(double damper){ this->damper = damper; }
	virtual double		GetDamper(){ return damper; }

	virtual void		SetOffsetForce(Vec3d ofst){offsetForce = ofst;}
	virtual Vec3d		GetOffsetForce(){ return offsetForce;
							// API名と処理内容に食い違いがあると思われる
							/*Vec3d ans, tmp;
							if(anyLimit)
								ans = Vec3d();
							else
								GetConstraintForce(tmp, ans);
							return ans;*/
						}
	virtual bool		IsLimit(){return limit.anyLimit;}
	/// 拘束点の入力
	virtual bool		SetConstLine(char* fileName , int i){ return limit.SetConstLine(fileName, i); }
	/// 拘束点の座標の取得
	virtual double		GetConstLine(int num, int way){ return limit.GetConstLine(num, way); }
	/// ConstLineと使うTwist角の範囲制限
	virtual void		SetTwistPole(Vec2d range){ poleTwist = range; }

	/// インタフェースの実装
	double  GetSecondDamper()				{return secondDamper;}
	void	SetSecondDamper(double input)	{secondDamper = input;}
	double  GetYieldStress()				{return yieldStress;}
	void	SetYieldStress(double input)	{yieldStress = input;}
	double  GetHardnessRate()				{return hardnessRate;}
	void	SetHardnessRate(double input)	{hardnessRate = input;}
	void	SetInertia(const Vec3d i)		{Inertia = i;}
	Vec3d	GetInertia()					{return Inertia;}
	void	SetDefomationType(int t)		{type = (PHJointDesc::PHDeformationType)t;}
	int 	GetDefomationType()				{return (int)type;}
	double 	GetmotorfNorm()					{return motor.fNorm;}
	PHJointDesc::PHDeformationType 	GetDeformationMode();

	/// 仮想関数のオーバライド
	virtual void	SetupLCP();
	virtual	void	IterateLCP();
	virtual void	CompBias();							///< 侵入量の判定
	virtual void	UpdateJointState();					///< joint情報（Socketに対するPlugの向きベクトル）の更新
	virtual void	CompError();						///< エラー判定
	
	virtual PHTreeNode*		CreateTreeNode(){
		return DBG_NEW PHBallJointNode();
	}
	PHBallJoint(const PHBallJointDesc& desc = PHBallJointDesc());				/// - コンストラクタ
};

}

#endif

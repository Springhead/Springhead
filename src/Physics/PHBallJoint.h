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

namespace Spr{

struct OnLimit{
	bool onLower;
	bool onUpper;
	bool& operator [] (int i){
		return i==0 ? onLower : onUpper;
	}
};
struct limitLine{
	double SwingLow[15][5];
	double SwingUp[15][5];
};

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
	virtual void		ModifyJacobian();
	virtual void		CompBias();
	virtual void		Projection(double& f, int i);
	PHBallJointNode(const PHBallJointNodeDesc& desc = PHBallJointNodeDesc()){}
	
};

class PHBallJoint : public PHJointND<3>{
public:
	SPR_OBJECTDEF1(PHBallJoint, PHJoint);
	SPR_DECLMEMBEROF_PHBallJointDesc;
protected:
	
	/*********************************************************
	Socket座標系：親剛体についている関節の座標系
	Plug座標系：子剛体についている関節の座標系
	拘束座標系：毎ステップ更新する，拘束の条件に使う座標系
	(x軸：拘束円の接線方向，
	 y軸：limitDirの延長線上の一点と交わる直線，
	 z軸：Socket座標系から見たPlug座標系のz軸の方向)
	**********************************************************/

	Vec3d			nowTheta;				///< 現在SocketからPlugに伸びているベクトル(Jc.ez())と稼動域制限の中心ベクトルとのなす角度(.x:swing, .y:twist, .z:swingDir)
	bool			anyLimit;				///< どこかのリミットにかかっているかどうかを調べるフラグ == (onLimit.onUpper || onLimit.onLower)
	Matrix3d		Jc;						///< 拘束座標系の速度・加速度　＝　Jc * Socket座標系から見たPlug座標系の速度、加速度
	Matrix3d		Jcinv;					///< Socket座標系から見たPlug座標系の速度、加速度  ＝　Jcinv * 拘束座標系の速度・加速度
	OnLimit			onLimit[2];				///< 可動域制限にかかっているとtrue ([0]:swing, [1]:twist)	
	double			fMinDt, fMaxDt;
	limitLine		LimitLine;				///< 拘束範囲の指定
	int				limitCount[2];				///< 上の配列のいくつまで入っているのか数える
	double			Irrupt;					///< 侵入量
	Vec3d			tanLine;
	int				FunNum;
	Matrix3d		limDir;					///< 初期の拘束座標系 (x軸,y軸,z軸( = limitDir))
	Vec3d			BefPos;					///< 前回の位置

	// 軌道追従制御用の変数，消さないで by Toki Aug. 2008
	Quaterniond qd,	 preQd;		///< ある時刻の目標位置への回転軸ベクトルと１時刻前の目標位置への回転軸ベクトル
	Vec3d qdDot, preQdDot;	///< ある時刻の目標位置への回転軸ベクトルの一階微分と１時刻前の目標位置への回転軸ベクトルの一階微分
	Vec3d qdWDot;			///< ある時刻の目標位置への回転軸ベクトルの二階微分

	virtual void	ControlCheck(double dtinv);	//< 制御をかけるかのチェック
	virtual void	MovableCheck(double dtinv);	//< 可動域をかけるかのチェック
	virtual void	AfterSetDesc();
	virtual coord_t GetTorqueND(){ return (coord_t&)torque; }
	friend class PHBallJointNode;
public:
	/// インタフェースの実装
	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	
	virtual void		SetSwingRange(Vec2d  range)	{ limitSwing = range; }			///< スイング角の範囲を設定する関数
	virtual void		GetSwingRange(Vec2d& range) { range = limitSwing; }			///< スイング角の範囲を得る関数
	virtual void		SetTwistRange(Vec2d  range)	{ limitTwist = range; }			///< ツイスト角の範囲を設定する関数
	virtual void		GetTwistRange(Vec2d& range) { range = limitTwist; }			///< ツイスト角の範囲を得る関数

	virtual void		SetMotorTorque(const Vec3d& t){/*mode = MODE_TORQUE;*/ torque = t;}					///< モータのトルクを設定する関数
	virtual Vec3d		GetMotorTorque(){return torque;}							///< モータのトルクを返す関数
	virtual Vec3d		GetAngle(){return position;}								///< 角度を返す関数
	virtual Quaterniond GetPosition(){return Xjrel.q;}								///< 角度のクォータニオンを返す関数
	virtual Vec3d		GetVelocity(){return velocity;}								///< 速度を返す関数
	virtual void		SetTorqueMax(double max){fMax = max; fMaxDt = fMax*GetScene()->GetTimeStep(); }
	virtual double		GetTorqueMax(){return fMax;}
	virtual void		SetTorqueMin(double min){fMin = min; fMinDt = fMin*GetScene()->GetTimeStep(); }
	virtual double		GetTorqueMin(){return fMin;}
	virtual void		SetDesiredVelocity(Vec3d v){/*mode = MODE_VELOCITY;*/ desiredVelocity = v;}
	virtual Vec3d		GetDesiredVelocity() const {return desiredVelocity;}
	virtual void		SetTrajectoryVelocity(Vec3d v) {/*mode = MODE_POSITION;*/ desiredVelocity = v;}
	virtual Vec3d		GetTrajectoryVelocity() const {return desiredVelocity;}
	virtual void		SetGoal(Quaterniond g){/*mode = MODE_POSITION;*/ goal = g;}
	virtual Quaterniond GetGoal(){return goal;}

	virtual void		SetSpring(double spring){ this->spring = spring; }
	virtual double		GetSpring(){ return spring; }
	virtual void		SetDamper(double damper){ this->damper = damper; }
	virtual double		GetDamper(){ return damper; }

	virtual void		SetOffsetForce(Vec3d ofst){offset = ofst;}
	virtual Vec3d		GetOffsetForce(){
							Vec3d ans;
							if(anyLimit)
								ans = Vec3d();
							else
								GetConstraintForce(Vec3d(), ans);
							return ans;
						}
	virtual bool		IsLimit(){return anyLimit;}
	virtual bool		SetConstLine(char* fileName , int i);///< 拘束点の入力
	virtual double		GetConstLine(int Num, int way){return LimitLine.SwingUp[Num][way];}///拘束点の座標の取得
	virtual void		SetTwistPole(Vec2d range){ PoleTwist = range; }		///< ConstLineと使うTwist角の範囲制限

	/// 仮想関数のオーバライド
	virtual void	AddMotorTorque(){f.w() = torque * GetScene()->GetTimeStep();}	///< トルクを加える関数
	virtual void	SetConstrainedIndex(bool* con);		///< 拘束をあたえるかどうかの判定
	virtual void	ModifyJacobian();
	virtual void	CompBias();							///< 侵入量の判定
	virtual void	Projection(double& f, int k);		///< LCPで力を拘束するのかどうかの判定をする関数（onLimit[0]がtrueになっている時に働く） 
	virtual void	UpdateJointState();					///< joint情報（Socketに対するPlugの向きベクトル）の更新
	virtual void	CompError();						///< エラー判定
	
	virtual PHTreeNode*		CreateTreeNode(){
		return DBG_NEW PHBallJointNode();
	}
	PHBallJoint(const PHBallJointDesc& desc = PHBallJointDesc());				/// - コンストラクタ
};

}

#endif

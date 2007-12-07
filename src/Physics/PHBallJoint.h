/*
 *  Copyright (c) 2003-2007, Shoichi Hasegawa and Springhead development team 
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
#include <fstream>

namespace Spr{

struct OnLimit{
	bool onLower;
	bool onUpper;
	bool& operator [] (int i){
		return i==0 ? onLower : onUpper;
	}
};

///	ボールジョイントに対応するツリーノード
class PHBallJointNode : public PHTreeNodeND<3>{
public:
	OBJECTDEF(PHBallJointNode, PHTreeNode);
	/// スイング・ツイスト角の時間変化率から角速度へのヤコビアン
	Matrix3d	Jst;

	PHBallJoint* GetJoint(){return DCAST(PHBallJoint, PHTreeNodeND<3>::GetJoint());}
	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void UpdateJointPosition(double dt);
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void ModifyJacobian();
	virtual void CompBias();
	virtual void Projection(double& f, int i);
	PHBallJointNode(const PHBallJointNodeDesc& desc = PHBallJointNodeDesc()){}
	
};

class PHBallJoint : public PHJointND<3>, public PHBallJointIfInit{
public:
	OBJECTDEF(PHBallJoint, PHJoint);

	double			spring, damper;			///< バネ、ダンパ係数
	Vec3d			limitDir;				///< 可動域の中心ベクトル（初期方向Z軸）
	Vec2d			nowTheta;				///< 現在SocketからPlugに伸びているベクトル(Jc.ez())と稼動域制限の中心ベクトルとのなす角度(.x:swing, .y:swingDir, .z:twist)
	Range<double>	limitSwing;				///< swing角の範囲
	Range<double>	limitTwist;				///< twist角の範囲
	bool			anyLimit;				///< どこかのリミットにかかっているかどうかを調べるフラグ == (onLimit.onUpper || onLimit.onLower)
	Matrix3d		Jc;						///< Socket座標系の速度・加速度　＝　Jc * 拘束座標系の速度、加速度
	Matrix3d		Jcinv;					///< 拘束座標系の速度、加速度    ＝　Jcinv * Socket座標系の速度・加速度
	OnLimit			onLimit[2];				///< 可動域制限にかかっているとtrue ([0]:swing, [1]:twist)	
	Quaterniond		goal;					///< 制御目標

	/// インタフェースの実装
	//virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	
	virtual void	SetSwingRange(Range<double>  range)	{ limitSwing = range; }		///< スイング角の範囲を設定する関数
	virtual void	GetSwingRange(Range<double>& range) { range = limitSwing; }		///< スイング角の範囲を得る関数
	virtual void    SetTwistRange(Range<double>  range)	{ limitTwist = range; }		///< ツイスト角の範囲を設定する関数
	virtual void	GetTwistRange(Range<double>& range) { range = limitTwist; }		///< ツイスト角の範囲を得る関数

	virtual void	SetMotorTorque(const Vec3d& t){torque = t;}										///< モータのトルクを設定する関数
	virtual Vec3d	GetMotorTorque(){return torque;}												///< モータのトルクを返す関数
	virtual Vec3d	GetAngle(){return position;}													///< 角度を返す関数
	virtual Vec3d	GetVelocity(){return velocity;}													///< 速度を返す関数

	/// 仮想関数のオーバライド
	virtual bool GetDesc(void* desc);																///< デスクリプタの情報を得るための関数
	virtual void SetDesc(const void* desc);															///< デスクリプタを設定する関数
	virtual void AddMotorTorque(){f.w() = torque * scene->GetTimeStep();}							///< トルクを加える関数
	virtual void SetConstrainedIndex(bool* con);													///< 拘束をあたえるかどうかの判定
	virtual void ModifyJacobian();
	virtual void CompBias();																		///< 侵入量の判定
	virtual void Projection(double& f, int k);														///< LCPで力を拘束するのかどうかの判定をする関数（onLimit[0]がtrueになっている時に働く） 
	virtual void UpdateJointState();																///< joint情報（Socketに対するPlugの向きベクトル）の更新
	virtual void CompError();																		///< エラー判定
	
	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHBallJointNode();
	}
	PHBallJoint(const PHBallJointDesc& desc = PHBallJointDesc());									/// - コンストラクタ
};

}

#endif

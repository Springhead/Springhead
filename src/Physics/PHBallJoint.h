/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHBALLJOINT_H
#define PHBALLJOINT_H

#include <SprPhysics.h>
#include <Physics/PHJoint.h>
#include <Physics/PHTreeNode.h>

namespace Spr{;

/** 回転のスイング・ツイスト角表現 */
struct SwingTwist : public Vec3d{
	double SwingDir(){return item(0);}
	double Swing(){return item(1);}
	double Twist(){return item(2);}
	void ToQuaternion(Quaterniond& q);
	void FromQuaternion(const Quaterniond& q);
	void Jacobian(Matrix3d& J, const Quaterniond& q);
};

///	ボールジョイントに対応するツリーノード
class PHBallJointNode : public PHTreeNodeND<3>{
public:
	virtual void CompJointJacobian();
	virtual void CompJointCoriolisAccel();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompBias();
	virtual void Projection(double& f, int i);
	//PHBallJointNode(PHBallJoint* j):PHTreeNodeND<3>(j){}
};

class PHBallJoint : public PHJointND<3>, PHBallJointIfInit{
protected:
	bool		swingOnUpper, twistOnLower, twistOnUpper;
	double		swingUpper, swingDamper, swingSpring;
	double		twistLower, twistUpper, twistDamper, twistSpring;
	SwingTwist	angle, velocity;	///< スイング・ツイスト角表現の角度と角速度
	Vec3d		torque;
	Matrix3d	Jst;
public:
	OBJECT_DEF(PHBallJoint);
	
	/// インタフェースの実装
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	virtual void	SetSwingRange(double u){swingUpper = u;}
	virtual double	GetSwingRange(){return swingUpper;}
	virtual void	SetTwistRange(double l, double u){twistLower = l; twistUpper = u;}
	virtual void	GetTwistRange(double& l, double& u){l = twistLower; u = twistUpper;}
	virtual void	SetMotorTorque(const Vec3d& t){torque = t;}
	virtual Vec3d	GetMotorTorque(){return torque;}
	virtual Vec3d	GetAngle(){return angle;}
	virtual Vec3d	GetVelocity(){return velocity;}
	
	/// 仮想関数のオーバライド
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual void AddMotorTorque(){f.w = torque * scene->GetTimeStep();}
	virtual void SetConstrainedIndex(bool* con);
	virtual void ModifyJacobian();
	virtual void CompBias();
	virtual void Projection(double& f, int k);
	virtual void UpdateJointState();
	//virtual void CompError(double dt);
	//virtual void ProjectionCorrection(double& F, int k);
	
	virtual PHTreeNode* CreateTreeNode(){
		return DBG_NEW PHBallJointNode();
	}
	PHBallJoint();
};

}

#endif

#ifndef PHBALLJOINT_H
#define PHBALLJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

class PHBallJoint : public InheritConstraint<PHBallJointIf, PHJoint>{
	double	max_angle;
	Vec3d	torque;
public:
	//OBJECTDEF(PHBallJoint);
	virtual void SetDesc(const PHConstraintDesc& desc);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHJointDesc::BALLJOINT;}
	virtual void SetMaxAngle(double angle){max_angle = angle;}
	virtual double GetMaxAngle(){return max_angle;}
	virtual void SetMotorTorque(const Vec3d& t){torque = t;}
	virtual Vec3d GetMotorTorque(){return torque;}
	virtual Quaterniond GetPosition();
	virtual Vec3d GetVelocity();
	virtual void CompConstraintJacobian();
	virtual void CompBias(double dt);
	virtual void ProjectionDynamics(double& f, int k);
	virtual void ProjectionCorrection(double& F, int k);
	PHBallJoint(){}
};

}

#endif

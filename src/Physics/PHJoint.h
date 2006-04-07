#ifndef PHJOINT_H
#define PHJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

class PHJoint : public InheritConstraint<PHJointIf, PHConstraint>{
public:

};
class PHJoint1D : public InheritConstraint<PHJoint1DIf, PHJoint>{
public:
	//OBJECTDEF(PHJoint1D);
	bool on_lower, on_upper;	/// 可動範囲の下限、上限に達している場合にtrue
	double lower, upper;		/// 可動範囲の下限、上限
	double torque;
	double pos_d, vel_d;		/// 目標変位、目標速度
	double spring, origin, damper;	/// バネ係数、バネ原点、ダンパ係数
	
	virtual void Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc);
	virtual void	SetRange(double l, double u){lower = l, upper = u;}
	virtual void	GetRange(double& l, double& u){l = lower, u = upper;}
	virtual void	SetMotorTorque(double t){mode = MODE_TORQUE; torque = t;}
	virtual double	GetMotorTorque(){return torque;}
	//virtual void SetDesiredPosition(double p){mode = MODE_POSITION; pos_d = p;}
	//virtual double GetDesiredPosition(){return pos_d;}
	virtual void	SetDesiredVelocity(double v){mode = MODE_VELOCITY; vel_d = v;}
	virtual double	GetDesiredVelocity(){return vel_d;}
	virtual void	SetSpring(double K){spring = K;}
	virtual double	GetSpring(){return spring;}
	virtual void	SetSpringOrigin(double org){origin = org;}
	virtual double	GetSpringOrigin(){return origin;}
	virtual void	SetDamper(double D){damper = D;}
	virtual double	GetDamper(){return damper;}
	
};

}

#endif

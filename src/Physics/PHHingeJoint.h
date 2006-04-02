#ifndef PHHINGEJOINT_H
#define PHHINGEJOINT_H

#include <SprPhysics.h>
#include <Foundation/Object.h>

namespace Spr{;

class PHHingeJoint : public InheritJoint1D<PHHingeJointIf, PHJoint1D>{
public:
	//OBJECTDEF(PHHingeJoint);
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_HINGE;}
	virtual double GetPosition();
	virtual double GetVelocity();
	virtual void CompDof();
	virtual void CompMotorForce();
	virtual void CompBias(double dt);
	virtual void Projectionfw(double& f, int k);
	virtual void ProjectionFq(double& F, int k);
	PHHingeJoint(){}
};

}

#endif
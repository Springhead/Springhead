#ifndef PHSLIDERJOINT_H
#define PHSLIDERJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

class PHSliderJoint : public InheritJoint1D<PHSliderJointIf, PHJoint1D>{
public:
	//OBJECTDEF(PHSliderJoint);
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::SLIDERJOINT;}
	virtual double GetPosition();
	virtual double GetVelocity();
	virtual void CompConstraintJacobian();
	virtual void CompBias(double dt);
	virtual void CompError();
	virtual void ProjectionDynamics(double& f, int k);
	virtual void ProjectionCorrection(double& F, int k);
};

}

#endif

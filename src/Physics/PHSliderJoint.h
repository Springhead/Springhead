#ifndef PHSLIDERJOINT_H
#define PHSLIDERJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

class PHSliderJoint : public InheritJoint1D<PHSliderJointIf, PHJoint1D>{
public:
	//OBJECTDEF(PHSliderJoint);
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_SLIDER;}
	virtual void CompDof();
	PHSliderJoint(){}
};

}

#endif
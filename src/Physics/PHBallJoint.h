#ifndef PHBALLJOINT_H
#define PHBALLJOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

namespace Spr{;

class PHBallJoint : public InheritConstraint<PHBallJointIf, PHConstraint>{
public:
	//OBJECTDEF(PHBallJoint);
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_BALL;}
	PHBallJoint(){}
};

}

#endif
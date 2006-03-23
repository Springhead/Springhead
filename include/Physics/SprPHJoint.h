#ifndef SPR_PHJOINTIf_H
#define SPR_PHJOINTIf_H
#include <Foundation/SprObject.h>

namespace Spr{;

struct PHJointDesc{
	enum JointType{
		JOINT_CONTACT,
		JOINT_HINGE,
		JOINT_SLIDER,
		JOINT_BALL,
	} type;
	Posed poseJoint[2];
};
struct PHHingeJointDesc : public PHJointDesc{
	PHHingeJointDesc(){
		type = JOINT_HINGE;
	}
};

struct PHConstraintIf{
	//virtual void Enable(bool bEnable = true) = 0;
};

}

#endif

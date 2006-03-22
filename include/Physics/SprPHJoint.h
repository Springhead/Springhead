#ifndef SPR_PHJOINTIf_H
#define SPR_PHJOINTIf_H
#include <Foundation/SprObject.h>

namespace Spr{;

struct PHJointDesc{
	enum {
		JOINT_HINGE
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

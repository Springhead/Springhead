#ifndef PHCONTACTPOINT_H
#define PHCONTACTPOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

class CDShapePair;

namespace Spr{;

class PHContactPoint : public InheritConstraint<PHContactPointIf, PHConstraint>{
public:
	//OBJECTDEF(PHContactPoint);
	CDShapePair* shapePair;
	Vec3d pos;
	virtual PHJointDesc::JointType GetJointType(){return PHJointDesc::JOINT_CONTACT;}
	virtual void CompConstraintJacobian();
	virtual void CompError();
	virtual void ProjectionDynamics(double& f, int k);
	virtual void ProjectionCorrection(double& F, int k);
	PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
	PHContactPoint(const Matrix3d& local, CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
};

}

#endif
#ifndef PHCONTACTPOINT_H
#define PHCONTACTPOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

class PHShapePairForLCP;

namespace Spr{;

class PHContactPoint : public InheritConstraint<PHContactPointIf, PHConstraint>{
public:
	OBJECT_DEF_ABST(PHContactPoint);
	PHShapePairForLCP* shapePair;
	Vec3d pos;
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::CONTACT;}
	virtual void CompConstraintJacobian();
	virtual void CompError(double dt);
	virtual void ProjectionDynamics(double& f, int k);
	virtual void ProjectionCorrection(double& F, int k);
	PHContactPoint(PHShapePairForLCP* sp, Vec3d p, PHSolidInfoForLCP* s0, PHSolidInfoForLCP* s1);
	PHContactPoint(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolidInfoForLCP* s0, PHSolidInfoForLCP* s1);
};

}

#endif

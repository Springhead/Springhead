#ifndef PHCONTACTPOINT_H
#define PHCONTACTPOINT_H

#include <SprPhysics.h>
#include "PHConstraint.h"

class CDShapePair;

namespace Spr{;

class PHContactPoint : public InheritConstraint<PHContactPointIf, PHConstraint>{
public:
	OBJECT_DEF_ABST(PHContactPoint);
	CDShapePair* shapePair;
	Vec3d pos;
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){return PHConstraintDesc::CONTACT;}
	virtual void CompConstraintJacobian();
	virtual void CompError(double dt);
	virtual void ProjectionDynamics(double& f, int k);
	virtual void ProjectionCorrection(double& F, int k);
	PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
	PHContactPoint(const Matrix3d& local, CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
};

}

#endif

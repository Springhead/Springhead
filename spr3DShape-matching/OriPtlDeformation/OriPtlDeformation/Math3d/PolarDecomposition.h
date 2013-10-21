#ifndef POLAR_DECOMPOSITION
#define POLAR_DECOMPOSITION

#include <Springhead.h>
#include "math.h"
#include <iostream>
#include "m2Real.h"

using namespace Spr;

class polarDecomposition {
public:
//----------------------------------------------------------------------------
void polarDecom(Matrix3f &R, Matrix3f &S, Matrix3f &res);

float diffOneNorm(const Matrix3f &y,const Matrix3f &elt);

//----------------------------------------------------------------------------
float lInfNorm(Matrix3f &elt);

//----------------------------------------------------------------------------
float l1Norm(Matrix3f &elt);

void qDUDecomposition(Matrix3f& kQ,
									Matrix3f& elt) ;// Vec3f& kD, Vec3f& kU,

m2Real NewPolarDecomposition( Matrix3f& M, Matrix3f& Q );
};
#endif
#include "PolarDecomposition.h"
#include <Springhead.h>
#include "math.h"
#include <iostream>
#include <stdio.h>
#include "m2Real.h"
//#define G3D_DEBUG

using namespace Spr;



m2Real polarDecomposition::NewPolarDecomposition( Matrix3f& M, Matrix3f& Q )
{
  Matrix3f Mk;
  Matrix3f Ek;
  m2Real det, M_oneNorm, M_infNorm, E_oneNorm;

  // Mk = M^T
  //Mk.trans( M );
  Mk = M.trans();

  M_oneNorm = l1Norm(Mk);
  M_infNorm = lInfNorm(Mk);

  do
  {
    Matrix3f MadjTk;

    // row 2 x row 3
    MadjTk[0] = cross( Mk[1], Mk[2] );
    // row 3 x row 1
    MadjTk[1] = cross( Mk[2], Mk[0] );
    // row 1 x row 2
    MadjTk[2] = cross( Mk[0], Mk[1] );

    det = Mk[0][0] * MadjTk[0][0] + Mk[0][1] * MadjTk[0][1] + Mk[0][2] * MadjTk[0][2];
    if (det == 0.0)
    {
//      printf("Warning (polarDecomposition) : zero determinant encountered.\n");
      break;
    }

    m2Real MadjT_one = l1Norm(MadjTk);
    m2Real MadjT_inf = lInfNorm(MadjTk);

    m2Real gamma = sqrt(sqrt((MadjT_one * MadjT_inf) / (M_oneNorm * M_infNorm)) / fabs(det));
    m2Real g1 = gamma * static_cast<m2Real>(0.5);
    m2Real g2 = static_cast<m2Real>(0.5) / (gamma * det);

    for(int i=0; i<3; i++)
    {
		for(int j=0; j<3;j++)
		{
			Ek[i][j] = Mk[i][j];
			Mk[i][j] = g1 * Mk[i][j] + g2 * MadjTk[i][j];
			Ek[i][j] -= Mk[i][j];
		}
     /* Ek.ptr()[i] = Mk.ptr()[i];
      Mk.ptr()[i] = g1 * Mk.ptr()[i] + g2 * MadjTk.ptr()[i];
      Ek.ptr()[i] -= Mk.ptr()[i];*///ptr() is cast to a line stored array
    }

    E_oneNorm = l1Norm(Ek);
    M_oneNorm = l1Norm(Mk);
    M_infNorm = lInfNorm(Mk);
  }
  while ( E_oneNorm > M_oneNorm * 1e-6);//zeroTolerance() );

  // Q = Mk^T
  Q = Mk.trans();

  return det;
}



void polarDecomposition::qDUDecomposition (Matrix3f& kQ, //Vec3f& kD, Vec3f& kU, 
									Matrix3f& elt)  {
	Vec3f kD,kU;
    // Factor M = QR = QDU where Q is orthogonal, D is diagonal,
    // and U is upper triangular with ones on its diagonal.  Algorithm uses
    // Gram-Schmidt orthogonalization (the QR algorithm).
    //
    // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.  The matrix R has entries
    //
    //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
    //   r10 = 0      r11 = q1*m1  r12 = q1*m2
    //   r20 = 0      r21 = 0      r22 = q2*m2
    //
    // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
    // u02 = r02/r00, and u12 = r12/r11.

    // Q = rotation
    // D = scaling
    // U = shear

    // D stores the three diagonal entries r00, r11, r22
    // U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

    // build orthogonal matrix Q
    float fInvLength = 1.0 / sqrt(elt[0][0] * elt[0][0]
                                       + elt[1][0] * elt[1][0] +
                                       elt[2][0] * elt[2][0]);
    kQ[0][0] = elt[0][0] * fInvLength;
    kQ[1][0] = elt[1][0] * fInvLength;
    kQ[2][0] = elt[2][0] * fInvLength;

    float fDot = kQ[0][0] * elt[0][1] + kQ[1][0] * elt[1][1] +
                kQ[2][0] * elt[2][1];
    kQ[0][1] = elt[0][1] - fDot * kQ[0][0];
    kQ[1][1] = elt[1][1] - fDot * kQ[1][0];
    kQ[2][1] = elt[2][1] - fDot * kQ[2][0];
    fInvLength = 1.0 / sqrt(kQ[0][1] * kQ[0][1] + kQ[1][1] * kQ[1][1] +
                                  kQ[2][1] * kQ[2][1]);
    kQ[0][1] *= fInvLength;
    kQ[1][1] *= fInvLength;
    kQ[2][1] *= fInvLength;

    fDot = kQ[0][0] * elt[0][2] + kQ[1][0] * elt[1][2] +
           kQ[2][0] * elt[2][2];
    kQ[0][2] = elt[0][2] - fDot * kQ[0][0];
    kQ[1][2] = elt[1][2] - fDot * kQ[1][0];
    kQ[2][2] = elt[2][2] - fDot * kQ[2][0];
    fDot = kQ[0][1] * elt[0][2] + kQ[1][1] * elt[1][2] +
           kQ[2][1] * elt[2][2];
    kQ[0][2] -= fDot * kQ[0][1];
    kQ[1][2] -= fDot * kQ[1][1];
    kQ[2][2] -= fDot * kQ[2][1];
    fInvLength = 1.0 / sqrt(kQ[0][2] * kQ[0][2] + kQ[1][2] * kQ[1][2] +
                                  kQ[2][2] * kQ[2][2]);
    kQ[0][2] *= fInvLength;
    kQ[1][2] *= fInvLength;
    kQ[2][2] *= fInvLength;

    // guarantee that orthogonal matrix has determinant 1 (no reflections)
    float fDet = kQ[0][0] * kQ[1][1] * kQ[2][2] + kQ[0][1] * kQ[1][2] * kQ[2][0] +
                kQ[0][2] * kQ[1][0] * kQ[2][1] - kQ[0][2] * kQ[1][1] * kQ[2][0] -
                kQ[0][1] * kQ[1][0] * kQ[2][2] - kQ[0][0] * kQ[1][2] * kQ[2][1];

    if ( fDet < 0.0 ) {
        for (int iRow = 0; iRow < 3; iRow++)
            for (int iCol = 0; iCol < 3; iCol++)
                kQ[iRow][iCol] = -kQ[iRow][iCol];
    }

    // build "right" matrix R
    Matrix3f kR;

    kR[0][0] = kQ[0][0] * elt[0][0] + kQ[1][0] * elt[1][0] +
               kQ[2][0] * elt[2][0];

    kR[0][1] = kQ[0][0] * elt[0][1] + kQ[1][0] * elt[1][1] +
               kQ[2][0] * elt[2][1];

    kR[1][1] = kQ[0][1] * elt[0][1] + kQ[1][1] * elt[1][1] +
               kQ[2][1] * elt[2][1];

    kR[0][2] = kQ[0][0] * elt[0][2] + kQ[1][0] * elt[1][2] +
               kQ[2][0] * elt[2][2];

    kR[1][2] = kQ[0][1] * elt[0][2] + kQ[1][1] * elt[1][2] +
               kQ[2][1] * elt[2][2];

    kR[2][2] = kQ[0][2] * elt[0][2] + kQ[1][2] * elt[1][2] +
               kQ[2][2] * elt[2][2];

    // the scaling component
    kD[0] = kR[0][0];

    kD[1] = kR[1][1];

    kD[2] = kR[2][2];

    // the shear component
    float fInvD0 = 1.0 / kD[0];

    kU[0] = kR[0][1] * fInvD0;

    kU[1] = kR[0][2] * fInvD0;

    kU[2] = kR[1][2] / kD[1];
}


//----------------------------------------------------------------------------
void polarDecomposition::polarDecom(Matrix3f &R, Matrix3f &S, Matrix3f &res) {
    /*
      Polar decomposition of a matrix. Based on pseudocode from
      Nicholas J Higham, "Computing the Polar Decomposition -- with
      Applications Siam Journal of Science and Statistical Computing, Vol 7, No. 4,
      October 1986.

      Decomposes A into R*S, where R is orthogonal and S is symmetric.

      Ken Shoemake's "Matrix animation and polar decomposition"
      in Proceedings of the conference on Graphics interface '92
      seems to be better known in the world of graphics, but Higham's version
      uses a scaling constant that can lead to faster convergence than
      Shoemake's when the initial matrix is far from orthogonal.
    */

    Matrix3f X = res;
    Matrix3f tmp = X.inv();
	Matrix3f Xit = tmp.trans();
    int iter = 0;
    
    const int MAX_ITERS = 100;

    const double eps = 50 * std::numeric_limits<float>::epsilon();
    const float BigEps = 50 * eps;

	//std::cout<<"what is epsilon() :"<<eps<<std::endl;

    /* Higham suggests using OneNorm(Xit-X) < eps * OneNorm(X)
     * as the convergence criterion, but OneNorm(X) should quickly
     * settle down to something between 1 and 1.7, so just comparing
     * with eps seems sufficient.
     *--------------------------------------------------------------- */

    //double resid = X.diffOneNorm(Xit);
	double resid = diffOneNorm(Xit,X); 
    while (resid > eps && iter < MAX_ITERS) {

      tmp = X.inv();
      Xit = tmp.trans();
      
      if (resid < BigEps) {
	// close enough use simple iteration
	X += Xit;
	X *= 0.5f;
      }
      else {
	// not close to convergence, compute acceleration factor
        float gamma = sqrt( sqrt(
			 //(Xit.l1Norm()* Xit.lInfNorm())/(X.l1Norm()*X.lInfNorm()) ) );
                  (l1Norm(Xit)* lInfNorm(Xit))/(l1Norm(X)*lInfNorm(X)) ) );

	X *= 0.5f * gamma;
	tmp = Xit;
	tmp *= 0.5f / gamma;
	X += tmp;
      }
      
      //resid = X.diffOneNorm(Xit);
	  resid = diffOneNorm(Xit,X);
      iter++;
    }

    R = X;
    tmp = R.trans();

    S = tmp * (res);

    // S := (S + S^t)/2 one more time to make sure it is symmetric
    tmp = S.trans();

    S += tmp;
    S *= 0.5f;

#ifdef G3D_DEBUG
    // Check iter limit
    if(iter >= MAX_ITERS)
	std::cout<<"error ="<<"iter >= MAX_ITERS"<<std::endl;
    // Check A = R*S
    tmp = R*S;
    resid = diffOneNorm(tmp,res);
    if(resid >= eps)
	std::cout<<"error ="<<"resid >= eps R*S 1"<<std::endl;
    // Check R is orthogonal
    tmp = R*R.trans();
	resid = diffOneNorm(tmp,Matrix3f::Unit());
    if(resid >= eps)
	std::cout<<"error ="<<"resid >= eps R 2"<<std::endl;
    // Check that S is symmetric
    tmp = S.trans();
    resid = diffOneNorm(tmp,S);
    if(resid >= eps)
	std::cout<<"error ="<<"resid >= eps S 3"<<std::endl;
#endif
}

float polarDecomposition::diffOneNorm(const Matrix3f &y,const Matrix3f &elt){
    float oneNorm = 0;
    
    for (int c = 0; c < 3; ++c){
    
      float f = fabs(elt[0][c] - y[0][c]) + fabs(elt[1][c] - y[1][c])
	+ fabs(elt[2][c] - y[2][c]);
      
      if (f > oneNorm) {
	oneNorm = f;
      }
    }
    return oneNorm;
}

//----------------------------------------------------------------------------
float polarDecomposition::lInfNorm(Matrix3f &elt){
    // The infinity norm of a matrix is the max row sum in absolute value.
    float infNorm = 0;

    for (int r = 0; r < 3; ++r) {
      
      float f = fabs(elt[r][0]) + fabs(elt[r][1])+ fabs(elt[r][2]);
      
      if (f > infNorm) {
	infNorm = f;
      }
    }
    return infNorm;
}

//----------------------------------------------------------------------------
float polarDecomposition::l1Norm(Matrix3f &elt){
    // The one norm of a matrix is the max column sum in absolute value.
    float oneNorm = 0;
    for (int c = 0; c < 3; ++c) {
      
      float f = fabs(elt[0][c])+ fabs(elt[1][c]) + fabs(elt[2][c]);
      
      if (f > oneNorm) {
	oneNorm = f;
      }
    }
    return oneNorm;
}


////New decomposition
//template<class Real>
//Real Decompose<Real>::polarDecomposition( const defaulttype::Mat<3,3,Real>& M, defaulttype::Mat<3,3,Real>& Q )
//{
//  defaulttype::Mat<3,3,Real> Mk;
//  defaulttype::Mat<3,3,Real> Ek;
//  Real det, M_oneNorm, M_infNorm, E_oneNorm;
//
//  // Mk = M^T
//  Mk.transpose( M );;
//
//  M_oneNorm = oneNorm(Mk);
//  M_infNorm = infNorm(Mk);
//
//  do
//  {
//    defaulttype::Mat<3,3,Real> MadjTk;
//
//    // row 2 x row 3
//    MadjTk[0] = cross( Mk[1], Mk[2] );
//    // row 3 x row 1
//    MadjTk[1] = cross( Mk[2], Mk[0] );
//    // row 1 x row 2
//    MadjTk[2] = cross( Mk[0], Mk[1] );
//
//    det = Mk(0,0) * MadjTk(0,0) + Mk(0,1) * MadjTk(0,1) + Mk(0,2) * MadjTk(0,2);
//    if (det == 0.0)
//    {
////      printf("Warning (polarDecomposition) : zero determinant encountered.\n");
//      break;
//    }
//
//    Real MadjT_one = oneNorm(MadjTk);
//    Real MadjT_inf = infNorm(MadjTk);
//
//    Real gamma = sqrt(sqrt((MadjT_one * MadjT_inf) / (M_oneNorm * M_infNorm)) / fabs(det));
//    Real g1 = gamma * static_cast<Real>(0.5);
//    Real g2 = static_cast<Real>(0.5) / (gamma * det);
//
//    for(int i=0; i<9; i++)
//    {
//      Ek.ptr()[i] = Mk.ptr()[i];//ptr is cast to a line stored array
//      Mk.ptr()[i] = g1 * Mk.ptr()[i] + g2 * MadjTk.ptr()[i];
//      Ek.ptr()[i] -= Mk.ptr()[i];
//    }
//
//    E_oneNorm = oneNorm(Ek);
//    M_oneNorm = oneNorm(Mk);
//    M_infNorm = infNorm(Mk);
//  }
//  while ( E_oneNorm > M_oneNorm * zeroTolerance() );
//
//  // Q = Mk^T
//  Q.transpose( Mk );
//
//  return det;
//}


//QR
//void Matrix3::qDUDecomposition (Matrix3& kQ,
//                                Vector3& kD, Vector3& kU) const {
//    // Factor M = QR = QDU where Q is orthogonal, D is diagonal,
//    // and U is upper triangular with ones on its diagonal.  Algorithm uses
//    // Gram-Schmidt orthogonalization (the QR algorithm).
//    //
//    // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
//    //
//    //   q0 = m0/|m0|
//    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
//    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
//    //
//    // where |V| indicates length of vector V and A*B indicates dot
//    // product of vectors A and B.  The matrix R has entries
//    //
//    //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
//    //   r10 = 0      r11 = q1*m1  r12 = q1*m2
//    //   r20 = 0      r21 = 0      r22 = q2*m2
//    //
//    // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
//    // u02 = r02/r00, and u12 = r12/r11.
//
//    // Q = rotation
//    // D = scaling
//    // U = shear
//
//    // D stores the three diagonal entries r00, r11, r22
//    // U stores the entries U[0] = u01, U[1] = u02, U[2] = u12
//
//    // build orthogonal matrix Q
//    float fInvLength = 1.0 / sqrt(elt[0][0] * elt[0][0]
//                                       + elt[1][0] * elt[1][0] +
//                                       elt[2][0] * elt[2][0]);
//    kQ[0][0] = elt[0][0] * fInvLength;
//    kQ[1][0] = elt[1][0] * fInvLength;
//    kQ[2][0] = elt[2][0] * fInvLength;
//
//    float fDot = kQ[0][0] * elt[0][1] + kQ[1][0] * elt[1][1] +
//                kQ[2][0] * elt[2][1];
//    kQ[0][1] = elt[0][1] - fDot * kQ[0][0];
//    kQ[1][1] = elt[1][1] - fDot * kQ[1][0];
//    kQ[2][1] = elt[2][1] - fDot * kQ[2][0];
//    fInvLength = 1.0 / sqrt(kQ[0][1] * kQ[0][1] + kQ[1][1] * kQ[1][1] +
//                                  kQ[2][1] * kQ[2][1]);
//    kQ[0][1] *= fInvLength;
//    kQ[1][1] *= fInvLength;
//    kQ[2][1] *= fInvLength;
//
//    fDot = kQ[0][0] * elt[0][2] + kQ[1][0] * elt[1][2] +
//           kQ[2][0] * elt[2][2];
//    kQ[0][2] = elt[0][2] - fDot * kQ[0][0];
//    kQ[1][2] = elt[1][2] - fDot * kQ[1][0];
//    kQ[2][2] = elt[2][2] - fDot * kQ[2][0];
//    fDot = kQ[0][1] * elt[0][2] + kQ[1][1] * elt[1][2] +
//           kQ[2][1] * elt[2][2];
//    kQ[0][2] -= fDot * kQ[0][1];
//    kQ[1][2] -= fDot * kQ[1][1];
//    kQ[2][2] -= fDot * kQ[2][1];
//    fInvLength = 1.0 / sqrt(kQ[0][2] * kQ[0][2] + kQ[1][2] * kQ[1][2] +
//                                  kQ[2][2] * kQ[2][2]);
//    kQ[0][2] *= fInvLength;
//    kQ[1][2] *= fInvLength;
//    kQ[2][2] *= fInvLength;
//
//    // guarantee that orthogonal matrix has determinant 1 (no reflections)
//    float fDet = kQ[0][0] * kQ[1][1] * kQ[2][2] + kQ[0][1] * kQ[1][2] * kQ[2][0] +
//                kQ[0][2] * kQ[1][0] * kQ[2][1] - kQ[0][2] * kQ[1][1] * kQ[2][0] -
//                kQ[0][1] * kQ[1][0] * kQ[2][2] - kQ[0][0] * kQ[1][2] * kQ[2][1];
//
//    if ( fDet < 0.0 ) {
//        for (int iRow = 0; iRow < 3; iRow++)
//            for (int iCol = 0; iCol < 3; iCol++)
//                kQ[iRow][iCol] = -kQ[iRow][iCol];
//    }
//
//    // build "right" matrix R
//    Matrix3 kR;
//
//    kR[0][0] = kQ[0][0] * elt[0][0] + kQ[1][0] * elt[1][0] +
//               kQ[2][0] * elt[2][0];
//
//    kR[0][1] = kQ[0][0] * elt[0][1] + kQ[1][0] * elt[1][1] +
//               kQ[2][0] * elt[2][1];
//
//    kR[1][1] = kQ[0][1] * elt[0][1] + kQ[1][1] * elt[1][1] +
//               kQ[2][1] * elt[2][1];
//
//    kR[0][2] = kQ[0][0] * elt[0][2] + kQ[1][0] * elt[1][2] +
//               kQ[2][0] * elt[2][2];
//
//    kR[1][2] = kQ[0][1] * elt[0][2] + kQ[1][1] * elt[1][2] +
//               kQ[2][1] * elt[2][2];
//
//    kR[2][2] = kQ[0][2] * elt[0][2] + kQ[1][2] * elt[1][2] +
//               kQ[2][2] * elt[2][2];
//
//    // the scaling component
//    kD[0] = kR[0][0];
//
//    kD[1] = kR[1][1];
//
//    kD[2] = kR[2][2];
//
//    // the shear component
//    float fInvD0 = 1.0 / kD[0];
//
//    kU[0] = kR[0][1] * fInvD0;
//
//    kU[1] = kR[0][2] * fInvD0;
//
//    kU[2] = kR[1][2] / kD[1];
//}
//





//----------------------------------------------------------------------------
//void Matrix3::polarDecomposition(Matrix3 &R, Matrix3 &S) const{
//    /*
//      Polar decomposition of a matrix. Based on pseudocode from
//      Nicholas J Higham, "Computing the Polar Decomposition -- with
//      Applications Siam Journal of Science and Statistical Computing, Vol 7, No. 4,
//      October 1986.
//
//      Decomposes A into R*S, where R is orthogonal and S is symmetric.
//
//      Ken Shoemake's "Matrix animation and polar decomposition"
//      in Proceedings of the conference on Graphics interface '92
//      seems to be better known in the world of graphics, but Higham's version
//      uses a scaling constant that can lead to faster convergence than
//      Shoemake's when the initial matrix is far from orthogonal.
//    */
//
//    Matrix3 X = *this;
//    Matrix3 tmp = X.inverse();
//    Matrix3 Xit = tmp.transpose();
//    int iter = 0;
//    
//    const int MAX_ITERS = 100;
//
//    const double eps = 50 * std::numeric_limits<float>::epsilon();
//    const float BigEps = 50 * eps;
//
//    /* Higham suggests using OneNorm(Xit-X) < eps * OneNorm(X)
//     * as the convergence criterion, but OneNorm(X) should quickly
//     * settle down to something between 1 and 1.7, so just comparing
//     * with eps seems sufficient.
//     *--------------------------------------------------------------- */
//
//    double resid = X.diffOneNorm(Xit);
//    while (resid > eps && iter < MAX_ITERS) {
//
//      tmp = X.inverse();
//      Xit = tmp.transpose();
//      
//      if (resid < BigEps) {
//	// close enough use simple iteration
//	X += Xit;
//	X *= 0.5f;
//      }
//      else {
//	// not close to convergence, compute acceleration factor
//        float gamma = sqrt( sqrt(
//                  (Xit.l1Norm()* Xit.lInfNorm())/(X.l1Norm()*X.lInfNorm()) ) );
//
//	X *= 0.5f * gamma;
//	tmp = Xit;
//	tmp *= 0.5f / gamma;
//	X += tmp;
//      }
//      
//      resid = X.diffOneNorm(Xit);
//      iter++;
//    }
//
//    R = X;
//    tmp = R.transpose();
//
//    S = tmp * (*this);
//
//    // S := (S + S^t)/2 one more time to make sure it is symmetric
//    tmp = S.transpose();
//
//    S += tmp;
//    S *= 0.5f;
//
//#ifdef G3D_DEBUG
//    // Check iter limit
//    assert(iter < MAX_ITERS);
//
//    // Check A = R*S
//    tmp = R*S;
//    resid = tmp.diffOneNorm(*this);
//    assert(resid < eps);
//
//    // Check R is orthogonal
//    tmp = R*R.transpose();
//    resid = tmp.diffOneNorm(Matrix3::identity());
//   


//polarDecomposition 2D
//void polarDecomposition(const Matrix3ff &A, Matrix3ff &R, Matrix3ff &S)
//{
//	// A = RS, where S is symmetric and R is orthonormal
//	// -> S = (A^T A)^(1/2)
//
//	//R.id();	// default answer
//	id(R);
//
//
//
//	Matrix3ff ATA;
//	multiplyTransposedLeft(A, A, ATA);
//
//	Matrix3ff U;
//	id(R);
//	eigenDecomposition(ATA, U);
//
//	float l0 = ATA.xx; if (l0 <= 0.0f) l0 = 0.0f; else l0 = 1.0f / sqrt(l0);//??????????????-1
//	float l1 = ATA.yy; if (l1 <= 0.0f) l1 = 0.0f; else l1 = 1.0f / sqrt(l1);
//
//	Matrix3ff S1;
//	S1.xx = l0*U.xx*U.xx + l1*U.xy*U.xy;
//	S1.xy = l0*U.xx*U.yx + l1*U.xy*U.yy;
//	S1.yx = S1.xy;
//	S1.yy = l0*U.yx*U.yx + l1*U.yy*U.yy;
//	R.multiply(A, S1);
//	S.multiplyTransposedLeft(R, A);
//}
//
//void eigenDecomposition(Matrix3ff &A, Matrix3ff &R)
//{
//	// only for symmetric matrices!
//	// A = R A' R^T, where A' is diagonal and R orthonormal
//
//	id(R);	// unit matrix
//	jacobiRotate(A, R);
//}
//
//void jacobiRotate(Matrix3ff &A, Matrix3ff &R)
//{
//	// rotates A through phi in 01-plane to set A(0,1) = 0
//	// rotation stored in R whose columns are eigenvectors of A
//	float d = (A(0,0) - A(1,1))/(2.0f*A(0,1));
//	float t = 1.0f / (fabs(d) + sqrt(d*d + 1.0f));
//	if (d < 0.0f) t = -t;
//	float c = 1.0f/sqrt(t*t + 1);
//	float s = t*c;
//	A(0,0) += t*A(0,1);
//	A(1,1) -= t*A(0,1);
//	A(0,1) = A(1,0) = 0.0f;
//	// store rotation in R
//	for (int k = 0; k < 2; k++) {
//		float Rkp = c*R(k,0) + s*R(k,1);
//		float Rkq =-s*R(k,0) + c*R(k,1);
//		R(k,0) = Rkp;
//		R(k,1) = Rkq;
//	}
//}
//
//void multiplyTransposedLeft(const Matrix3ff &left, const Matrix3ff &right, Matrix3ff &resultMatrix) {//?×ó³Ë¾ØÕó£¿
//		Matrix3ff res;
//		res.xx = left.xx*right.xx + left.yx*right.yx;
//		res.xy = left.xx*right.xy + left.yx*right.yy;
//		res.yx = left.xy*right.xx + left.yy*right.yx;
//		res.yy = left.xy*right.xy + left.yy*right.yy;
//		resultMatrix = res;
//
//	}
//
//void id(Matrix3ff &A)//make identity matrix@’PˆÊs—ñŽ®‚ðì‚é
//{
//	A.xx = 1;A.yy = 1;A.zz = 1;
//	A.xy = 0;A.xz = 0;A.yz = 0;A.yx = 0;A.zx = 0;A.zy = 0;
//}


#ifndef PHOPDECOMPOSITIONMETHODS_H
#define PHOPDECOMPOSITIONMETHODS_H

#include <math.h>
#include <stdlib.h>
#include "Physics/SprPHOpObj.h"

namespace Spr{;



class Jacobi
{
private: Matrix3f	tmp;
public:
	std::vector<float> a;//a = new float[9];
	std::vector<float> v;//float v = new float[9];
	std::vector<float> d;//float d = new float[3];
	void init()
	{

		a.reserve(9);
		v.reserve(9);
		d.reserve(3);
		for (int k = 0; k<9; k++)
		{
			a.push_back(0);
			v.push_back(0);
		}
		for (int k = 0; k<3; k++)
		{
			d.push_back(0);
		}
	}
	Matrix3f	matrix;
	Vec3f		eigenValues;
	Matrix3f 	eigenVectors;

	int		maxSweeps;
	float	tol;

	Jacobi()
	{
		maxSweeps = 5;
		tol = 1e-10f;
	}

	void Do() {
		float onorm, dnorm;
		float b, dma, q, t, c, s;
		float atemp, vtemp, dtemp;
		int i, j, k, l;




		tmp = matrix.trans();
		inputValueFromMatrix(tmp, a);
		tmp = eigenVectors.trans();
		inputValueFromMatrix(tmp, v);
		/*for (int m = 0; m < 9; m++) {
		a[m] = tmp[m];
		}
		tmp = eigenVectors.trans();
		for (int m = 0; m < 9; m++) {
		v[m] = tmp[m];
		}*/

		d[0] = a[0];
		d[1] = a[4];
		d[2] = a[8];

		for (l = 1; l <= maxSweeps; l++) {
			dnorm = fabs(d[0]) + fabs(d[1]) + fabs(d[2]);
			onorm = fabs(a[1]) + fabs(a[2]) + fabs(a[5]);
			// Normal end point of this algorithm.
			if ((onorm / dnorm) <= tol) {
				Update(d, v);
				return;
			}

			for (j = 1; j < 3; j++) {
				for (i = 0; i <= j - 1; i++) {
					b = a[3 * i + j];
					if (fabs(b) > 0.0f) {
						dma = d[j] - d[i];
						if ((fabs(dma) + fabs(b)) <= fabs(dma)) {
							t = b / dma;
						}
						else {
							q = 0.5f * dma / b;
							t = 1.0f / (fabs(q) + sqrt(1.0f + q*q));
							if (q < 0.0)
								t = -t;
						}

						c = 1.0f / sqrt(t*t + 1.0f);
						s = t * c;
						a[3 * i + j] = 0.0f;

						for (k = 0; k <= i - 1; k++) {
							atemp = c * a[3 * k + i] - s * a[3 * k + j];
							a[3 * k + j] = s * a[3 * k + i] + c * a[3 * k + j];
							a[3 * k + i] = atemp;
						}

						for (k = i + 1; k <= j - 1; k++) {
							atemp = c * a[3 * i + k] - s * a[3 * k + j];
							a[3 * k + j] = s * a[3 * i + k] + c * a[3 * k + j];
							a[3 * i + k] = atemp;
						}

						for (k = j + 1; k < 3; k++) {
							atemp = c * a[3 * i + k] - s * a[3 * j + k];
							a[3 * j + k] = s * a[3 * i + k] + c * a[3 * j + k];
							a[3 * i + k] = atemp;
						}

						for (k = 0; k < 3; k++) {
							vtemp = c * v[3 * k + i] - s * v[3 * k + j];
							v[3 * k + j] = s * v[3 * k + i] + c * v[3 * k + j];
							v[3 * k + i] = vtemp;
						}

						dtemp = c*c*d[i] + s*s*d[j] - 2.0f*c*s*b;
						d[j] = s*s*d[i] + c*c*d[j] + 2.0f*c*s*b;
						d[i] = dtemp;
					}
				}
			}
		}
		Update(d, v);
		/*delete  a;
		delete  v;
		delete  d;*/

		return;
	}
	
	void Update(std::vector<float> &d, std::vector<float> &v) {
		eigenValues.x = d[0];
		eigenValues.y = d[1];
		eigenValues.z = d[2];

		tmp = Matrix3f();
		inputValueFromBuffer(v, tmp);
		/*for (int i = 0; i < 9; ++ i) {
		tmp[i] = v[i];
		}*/

		eigenVectors = tmp.trans();
	}

	void inputValueFromMatrix(Matrix3f &tmp, std::vector<float> &a)
	{
		a[0] = tmp.xx;
		a[1] = tmp.xy;
		a[2] = tmp.xz;
		a[3] = tmp.yx;
		a[4] = tmp.yy;
		a[5] = tmp.yz;
		a[6] = tmp.zx;
		a[7] = tmp.zy;
		a[8] = tmp.zz;
	}

	void inputValueFromBuffer(std::vector<float> &a, Matrix3f &tmp)
	{
		tmp.xx = a[0];
		tmp.xy = a[1];
		tmp.xz = a[2];
		tmp.yx = a[3];
		tmp.yy = a[4];
		tmp.yz = a[5];
		tmp.zx = a[6];
		tmp.zy = a[7];
		tmp.zz = a[8];
	}
};

class FloatErrorTest
{
public:
	static bool   CheckBadFloatValue(Vec3f &f)
	{
		if (!(f.x == f.x) || !(f.y == f.y) || !(f.z == f.z))
		{
			DSTR << "f is not a valid Number!" ;
			return false;
		}
		if (isnan(f.norm()) != 0)
		{
			DSTR << "f is not a valid Number!" ;
			return false;
		}
		if (!finite(f.x) || !finite(f.y) || !finite(f.z))
		{
			DSTR << "f is not a valid Number!";
			return false;
		}
		return true;
	}
	static bool   CheckBadFloatValue(float f)
	{
		if (!(f == f))
		{
			DSTR << "f is not a valid Number!" ;

			return false;
		}
		if (isnan(f) != 0)
		{
			DSTR <<"f is not a valid Number!" ;
			return false;
		}
		if (!finite(f))
		{
			DSTR << "f is not a valid Number!";
			return false;
		}
		return true;
	}
	static bool    CheckBadFloatValue(float f1, float f2, float f3)
	{
		if (!(f1 == f1) || !(f2 == f2) || !(f3 == f3))
		{
			DSTR << "f is not a valid Number!" ;
			return false;
		}
		if (isnan(f1) != 0 || isnan(f2) != 0 || isnan(f3) != 0)
		{
			DSTR << "f is not a valid Number!" ;
			return false;
		}
		if (!finite(f1) || !finite(f2) || !finite(f3))
		{
			DSTR << "f is not a valid Number!" ;
			return false;
		}
		return true;
	}

};
class polarDecomposition {
public:
//----------------------------------------------------------------------------
void polarDecom(Matrix3f &R, Matrix3f &S, Matrix3f &res)
{
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
    
    const int MAX_ITERS = 1000;

    const double eps = 10 * std::numeric_limits<float>::epsilon();
    const double BigEps = 10 * eps;

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

float diffOneNorm(const Matrix3f &y,const Matrix3f &elt){
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
float lInfNorm(Matrix3f &elt){
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
float l1Norm(Matrix3f &elt){
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

//void qDUDecomposition(Matrix3f& kQ,
//									Matrix3f& elt) ;// Vec3f& kD, Vec3f& kU,
//
//m2Real NewPolarDecomposition( Matrix3f& M, Matrix3f& Q );
};

class MatrixExtension
{
public:
	Matrix3f MatrixesMultiply3f(const Matrix3f &res, const Matrix3f &other)
	{
		Matrix3f result;
		result.xx = res.xx * other.xx + res.xy * other.yx + res.xz * other.zx;
		result.xy = res.xx * other.xy + res.xy * other.yy + res.xz * other.zy;
		result.xz = res.xx * other.xz + res.xy * other.yz + res.xz * other.zz;

		result.yx = res.yx * other.xx + res.yy * other.yx + res.yz * other.zx;
		result.yy = res.yx * other.xy + res.yy * other.yy + res.yz * other.zy;
		result.yz = res.yx * other.xz + res.yy * other.yz + res.yz * other.zz;

		result.zx = res.zx * other.xx + res.zy * other.yx + res.zz * other.zx;
		result.zy = res.zx * other.xy + res.zy * other.yy + res.zz * other.zy;
		result.zz = res.zx * other.xz + res.zy * other.yz + res.zz * other.zz;
	
		return result;
	}

	Vec3f MatrixMultiVec3fRight(const Matrix3f &res, const Vec3f &other)
	{
		
		Vec3f result;
		result.x = res.xx * other.x + res.xy * other.y + res.xz * other.z;
		result.y = res.yx * other.x + res.yy * other.y + res.yz * other.z;
		result.z = res.zx * other.x + res.zy * other.y + res.zz * other.z;
	
	
		return result;
	}

	Vec3f MatrixMultiVec3fLeft(const Vec3f &other,const Matrix3f &res)
	{
		
		Vec3f result;
		result.x = res.xx * other.x + res.yx * other.y + res.zx * other.z;
		result.y = res.xy * other.x + res.yy * other.y + res.zy * other.z;
		result.z = res.xz * other.x + res.yz * other.y + res.zz * other.z;
	
	
		return result;

	}
	Vec3f VectorCross(Vec3f v1,Vec3f v2)
	{
		
		return Vec3f(((v1.y*v2.z) - (v1.z*v2.y)),((v1.z*v2.x) - (v1.x*v2.z)),((v1.x*v2.y - v1.y*v2.x)));
	}
};

class JacobiDecomposition 
{
public:
		// Computes the eigenvectors and eigenvalues of the symmetric matrix A using
	// the classic Jacobi method of iteratively updating A as A = J^T * A * J,
	// where J = J(p, q, theta) is the Jacobi rotation matrix.
	//
	// On exit, v will contain the eigenvectors, and the diagonal elements
	// of a are the corresponding eigenvalues.
	//
	// See Golub, Van Loan, Matrix Computations, 3rd ed, p428
	void Jacobi(Matrix3f &a, Matrix3f &v)
	{
		int i, j, n, p, q;
		float prevoff, c, s;
		Matrix3f J, b, t;

		// Initialize v to identity matrix
		for (i = 0; i < 3; i++) {
			v[i][0] = v[i][1] = v[i][2] = 0.0f;
			v[i][i] = 1.0f;
		}

		// Repeat for some maximum number of iterations
		const int MAX_ITERATIONS = 50;
		for (n = 0; n < MAX_ITERATIONS; n++) {
			// Find largest off-diagonal absolute element a[p][q]
			p = 0; q = 1;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (i == j) continue;
					if (abs(a[i][j]) > abs(a[p][q])) {
						p = i;
						q = j;
					}
				}
			}

			// Compute the Jacobi rotation matrix J(p, q, theta)
			// (This code can be optimized for the three different cases of rotation)
			SymSchur2(a, p, q, c, s);
			for (i = 0; i < 3; i++) {
				J[i][0] = J[i][1] = J[i][2] = 0.0f;
				J[i][i] = 1.0f;
			}
			J[p][p] =  c; J[p][q] = s;
			J[q][p] = -s; J[q][q] = c;

			// Cumulate rotations into what will contain the eigenvectors
			v = v * J;

			// Make 'a' more diagonal, until just eigenvalues remain on diagonal
			a = (J.trans() * a) * J;
    
			// Compute "norm" of off-diagonal elements
			float off = 0.0f;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (i == j) continue;
					off += a[i][j] * a[i][j];
				}
			}
			/* off = sqrt(off); not needed for norm comparison */

			// Stop when norm no longer decreasing
			if (n > 2 && off >= prevoff)
				return;
        
			prevoff = off;
		}
	}

	// 2-by-2 Symmetric Schur decomposition. Given an n-by-n symmetric matrix
	// and indicies p, q such that 1 <= p < q <= n, computes a sine-cosine pair
	// (s, c) that will serve to form a Jacobi rotation matrix.
	//
	// See Golub, Van Loan, Matrix Computations, 3rd ed, p428
	void SymSchur2(Matrix3f &a, int p, int q, float &c, float &s)
	{
		if (abs(a[p][q]) > 0.0001f) {
			float r = (a[q][q] - a[p][p]) / (2.0f * a[p][q]);
			float t;
			if (r >= 0.0f)
				t = 1.0f / (r + sqrt(1.0f + r*r));
			else
				t = -1.0f / (-r + sqrt(1.0f + r*r));
			c = 1.0f / sqrt(1.0f + t*t);
			s = t * c;
		} else {
			c = 1.0f;
			s = 0.0f;
		}
	}
};


//svd function
#define SIGN(u, v)     ( (v)>=0.0 ? fabs(u) : -fabs(u) )
#define MAX(x, y)     ( (x) >= (y) ? (x) : (y) )  

class SVDDecomposition
{
public:

	float radius(float u, float v)
	{
		float          w;
		u = fabs(u);
		v = fabs(v);
		if (u > v) {
			w = v / u;
			return (float)(u * sqrt(1. + w * w));
		} else {
			if (v) {
				 w = u / v;
				 return (float)(v * sqrt(1. + w * w));
			} else
				return 0.0;
		}
	}

	/*
	 Given matrix a[m][n], m>=n, using svd decomposition a = p d q' to get
	 p[m][n], diag d[n] and q[n][n].
	*/
	//void SVDDecomposition::svd(int m, int n, double **a, double **p, double *d, double **q)
		void svd(int m, int n, Spr::Matrix3f a, Spr::Matrix3f &p, Spr::Vec3f &d, Spr::Matrix3f &q)
	{
			int             flag, i, its, j, jj, k, l, nm, nm1 = n - 1, mm1 = m - 1;
			float          c, f, h, s, x, y, z;
			float          anorm = 0, g = 0, scale = 0;
			//double         *r = tvector_alloc(0, n, double);
			float			*r = (float*)malloc(sizeof(float)*n);

			for (i = 0; i < m; i++)
					for (j = 0; j < n; j++)
							p[i][j] = a[i][j];
			//for (i = m; i < n; i++)
			//                p[i][j] = 0;

			/* Householder reduction to bidigonal form */
			for (i = 0; i < n; i++)
			{
					l = i + 1;
					r[i] = scale * g;
					g = s = scale = 0.0;
					if (i < m)
					{
							for (k = i; k < m; k++)
									scale += fabs(p[k][i]);
							if (scale)
							{
									for (k = i; k < m; k++)
									{
											p[k][i] /= scale;
											s += p[k][i] * p[k][i];
									}
									f = p[i][i];
									g = -SIGN(sqrt(s), f);
									h = f * g - s;
									p[i][i] = f - g;
									if (i != nm1)
									{
											for (j = l; j < n; j++)
											{
													for (s = 0.0, k = i; k < m; k++)
															s += p[k][i] * p[k][j];
													f = s / h;
													for (k = i; k < m; k++)
															p[k][j] += f * p[k][i];
											}
									}
									for (k = i; k < m; k++)
											p[k][i] *= scale;
							}
					}
					d[i] = scale * g;
					g = s = scale = 0.0;
					if (i < m && i != nm1)
					{
							for (k = l; k < n; k++)
									scale += fabs(p[i][k]);
							if (scale)
							{
									for (k = l; k < n; k++)
									{
											p[i][k] /= scale;
											s += p[i][k] * p[i][k];
									}
									f = p[i][l];
									g = -SIGN(sqrt(s), f);
									h = f * g - s;
									p[i][l] = f - g;
									for (k = l; k < n; k++)
											r[k] = p[i][k] / h;
									if (i != mm1)
									{
											for (j = l; j < m; j++)
											{
													for (s = 0.0, k = l; k < n; k++)
															s += p[j][k] * p[i][k];
													for (k = l; k < n; k++)
															p[j][k] += s * r[k];
											}
									}
									for (k = l; k < n; k++)
											p[i][k] *= scale;
							}
					}
					anorm = MAX(anorm, fabs(d[i]) + fabs(r[i]));
			}

			/* Accumulation of right-hand transformations */
			for (i = n - 1; i >= 0; i--)
			{
					if (i < nm1)
					{
							if (g)
							{
									for (j = l; j < n; j++)
											q[j][i] = (p[i][j] / p[i][l]) / g;
									for (j = l; j < n; j++)
									{
											for (s = 0.0, k = l; k < n; k++)
													s += p[i][k] * q[k][j];
											for (k = l; k < n; k++)
													q[k][j] += s * q[k][i];
									}
							}
							for (j = l; j < n; j++)
									q[i][j] = q[j][i] = 0.0;
					}
					q[i][i] = 1.0f;
					g = r[i];
					l = i;
			}
			/* Accumulation of left-hand transformations */
			for (i = n - 1; i >= 0; i--)
			{
					l = i + 1;
					g = d[i];
					if (i < nm1)
							for (j = l; j < n; j++)
									p[i][j] = 0.0;
					if (g)
					{
							g = 1.0f / g;
							if (i != nm1)
							{
									for (j = l; j < n; j++)
									{
											for (s = 0.0, k = l; k < m; k++)
													s += p[k][i] * p[k][j];
											f = (s / p[i][i]) * g;
											for (k = i; k < m; k++)
													p[k][j] += f * p[k][i];
									}
							}
							for (j = i; j < m; j++)
									p[j][i] *= g;
					} else
							for (j = i; j < m; j++)
									p[j][i] = 0.0;
					++p[i][i];
			}
			/* diagonalization of the bidigonal form */
			for (k = n - 1; k >= 0; k--)
			{                       /* loop over singlar values */
					for (its = 0; its < 30; its++)
					{               /* loop over allowed iterations */
							flag = 1;
							for (l = k; l >= 0; l--)
							{       /* test for splitting */
									nm = l - 1;     /* note that r[l] is always
													 * zero */
									if (fabs(r[l]) + anorm == anorm)
									{
											flag = 0;
											break;
									}
									if (fabs(d[nm]) + anorm == anorm)
											break;
							}
							if (flag)
							{
									c = 0.0;        /* cancellation of r[l], if
													 * l>1 */
									s = 1.0f;
									for (i = l; i <= k; i++)
									{
											f = s * r[i];
											if (fabs(f) + anorm != anorm)
											{
													g = d[i];
													h = radius(f, g);
													d[i] = h;
													h = 1.0f / h;
													c = g * h;
													s = (-f * h);
													for (j = 0; j < m; j++)
													{
															y = p[j][nm];
															z = p[j][i];
															p[j][nm] = y * c + z * s;
															p[j][i] = z * c - y * s;
													}
											}
									}
							}
							z = d[k];
							if (l == k)
							{       /* convergence */
									if (z < 0.0)
									{
											d[k] = -z;
											for (j = 0; j < n; j++)
													q[j][k] = (-q[j][k]);
									}
									break;
							}
							if (its == 30)
							{
									//error("svd: No convergence in 30 svd iterations", non_fatal);
									return;
							}
							x = d[l];       /* shift from bottom 2-by-2 minor */
							nm = k - 1;
							y = d[nm];
							g = r[nm];
							h = r[k];
							f = (float) (((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y));
							g = radius(f, 1.0f);
							/* next QR transformation */
							f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
							c = s = 1.0f;
							for (j = l; j <= nm; j++)
							{
									i = j + 1;
									g = r[i];
									y = d[i];
									h = s * g;
									g = c * g;
									z = radius(f, h);
									r[j] = z;
									c = f / z;
									s = h / z;
									f = x * c + g * s;
									g = g * c - x * s;
									h = y * s;
									y = y * c;
									for (jj = 0; jj < n; jj++)
									{
											x = q[jj][j];
											z = q[jj][i];
											q[jj][j] = x * c + z * s;
											q[jj][i] = z * c - x * s;
									}
									z = radius(f, h);
									d[j] = z;       /* rotation can be arbitrary
													 * id z=0 */
									if (z)
									{
											z = 1.0f / z;
											c = f * z;
											s = h * z;
									}
									f = (c * g) + (s * y);
									x = (c * y) - (s * g);
									for (jj = 0; jj < m; jj++)
									{
											y = p[jj][j];
											z = p[jj][i];
											p[jj][j] = y * c + z * s;
											p[jj][i] = z * c - y * s;
									}
							}
							r[l] = 0.0;
							r[k] = f;
							d[k] = x;
					}
			}
			free(r);

			// dhli add: the original code does not sort the eigen value
			// should do that and change the eigen vector accordingly

	}
};//SVD decomposition

}//namespace
#endif

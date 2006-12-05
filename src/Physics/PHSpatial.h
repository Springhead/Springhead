/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSPATIAL_H
#define PHSPATIAL_H

#include <SprPhysics.h>

/**
 SpatialVector	  V(v, w) = [v; w];
 SpatialTransform X(R, r) = [R^T, -R^T * r%; O, R^T];
 ^TÇÕì]íu, %ÇÕäOêœçsóÒÅCOÇÕóÎçsóÒÅD
 X(R, r) * V(v, w) = [R^T * (v + w % r); R^T * w];

 çáê¨: X(R1, r1) * X(R2, r2) = X(R2 * R1, r2 + R2 * r1);
 ãt:   X(R, r)^-1 = X(R^T, -R^T * r);

 ì]íu: X(R, r)^T = [R, O; r% * R, R];
	X(R, r)^T * V(f, t) = V(R * f, r % (R * f) + R * t);
 ì]íuÇÃçáê¨: X(R1, r1)^T * X(R2, r2)^T = (X(R2, r2) * X(R1, r1))^T = X(R1 * R2, r1 + R1 * r2)^T
 ì]íuÇÃãt: (X(R, r)^T)^-1 = (X(R, r)^-1)^T

 */

namespace Spr{;

#define SUBMAT(r, c, h, w) sub_matrix(PTM::TSubMatrixDim<r, c, h, w>())
#define SUBVEC(o, l) sub_vector(PTM::TSubVectorDim<o, l>())
typedef PTM::TMatrixCol<6, 6, double> Matrix6d;
typedef PTM::TVector<6, double> Vec6d;

///	v1 * v2^T Ç≈ìæÇÁÇÍÇÈçsóÒ
inline Matrix3d VVtr(const Vec3d& v1, const Vec3d& v2){
	return Matrix3d(
		v1[0] * v2[0], v1[0] * v2[1], v1[0] * v2[2],
		v1[1] * v2[0], v1[1] * v2[1], v1[1] * v2[2],
		v1[2] * v2[0], v1[2] * v2[1], v1[2] * v2[2]);
}

/// SpatialTransform
struct SpatialTransformTranspose;
struct SpatialTransform{
	Matrix3d	R;
	Vec3d		r;

	SpatialTransform(){}
	SpatialTransform(const Matrix3d& _R, const Vec3d& _r):R(_R), r(_r){}
	SpatialTransform(const SpatialTransform& X):R(X.R), r(X.r){}

	SpatialTransform inv()const{
		return SpatialTransform(R.trans(), -(R.trans() * r));
	}
	const SpatialTransformTranspose& trans()const{
		return (SpatialTransformTranspose&)*this;
	}
};

inline SpatialTransform operator*(const SpatialTransform& lhs, const SpatialTransform& rhs){
	return SpatialTransform(rhs.R * lhs.R, rhs.r + rhs.R * lhs.r);
}

/// SpatialTransformTranspose
struct SpatialTransformTranspose{
	Matrix3d	R;
	Vec3d		r;

	SpatialTransformTranspose(){}
	SpatialTransformTranspose(const Matrix3d& _R, const Vec3d& _r):R(_R), r(_r){}
	SpatialTransformTranspose(const SpatialTransform& X):R(X.R), r(X.r){}

	SpatialTransformTranspose inv()const{
		return SpatialTransformTranspose(R.trans(), -(R.trans() * r));
	}
	const SpatialTransform& trans()const{
		return (SpatialTransform&)*this;
	}

};

inline SpatialTransformTranspose operator*(const SpatialTransformTranspose& lhs, const SpatialTransformTranspose& rhs){
	return SpatialTransformTranspose(lhs.R * rhs.R, lhs.r + lhs.R * rhs.r);
}

/// SpatialVector
struct SpatialVector{
	Vec3d v;
	Vec3d w;

	void clear(){v.clear(); w.clear();}
	SpatialVector(){}
	SpatialVector(const Vec3d& _v, const Vec3d& _w):v(_v), w(_w){}
	SpatialVector(const SpatialVector& V):v(V.v), w(V.w){}
	double& operator[](int i){
		return ((double*)this)[i];
	}
	double operator[](int i)const{
		return ((const double*)this)[i];
	}
	SpatialVector& operator+=(const SpatialVector& V){
		v += V.v;
		w += V.w;
		return *this;
	}
	SpatialVector& operator-=(const SpatialVector& V){
		v -= V.v;
		w -= V.w;
		return *this;
	}
	SpatialVector& operator*=(double k){
		v *= k;
		w *= k;
		return *this;
	}
};

inline SpatialVector operator+ (const SpatialVector& lhs, const SpatialVector& rhs){
	return SpatialVector(lhs.v + rhs.v, lhs.w + rhs.w);
}
inline SpatialVector operator- (const SpatialVector& lhs, const SpatialVector& rhs){
	return SpatialVector(lhs.v - rhs.v, lhs.w - rhs.w);
}
inline SpatialVector operator* (double k, const SpatialVector& V){
	return SpatialVector(k * V.v, k * V.w);
}
inline SpatialVector operator* (const SpatialVector& V, double k){
	return SpatialVector(k * V.v, k * V.w);
}
inline double operator* (const SpatialVector& lhs, const SpatialVector& rhs){
	return lhs.v * rhs.v + lhs.w * rhs.w;
}
inline SpatialVector operator*(const SpatialTransform& X, const SpatialVector& V){
	return SpatialVector(X.R.trans() * (V.v + V.w % X.r), X.R.trans() * V.w);
}
inline SpatialVector operator*(const SpatialTransformTranspose& X, const SpatialVector& V){
	return SpatialVector(X.R * V.v, X.r % (X.R * V.v) + X.R * V.w);
}

/// SpatialMatrix
struct SpatialMatrix{
	Matrix3d vv, vw, wv, ww;
	void clear(){vv.clear(); vw.clear(); wv.clear(); ww.clear();}
	SpatialMatrix inv(){
		Matrix3d vvinv = vv.inv();
		Matrix3d S = ww - wv * vvinv * vw;
		Matrix3d Sinv = S.inv();
		SpatialMatrix Y;
		Y.vw = -1.0 * vvinv * vw * Sinv;
		Y.vv = (Matrix3d::Unit() - Y.vw * wv) * vvinv;
		Y.wv = -1.0 * Sinv * wv * vvinv;
		Y.ww = Sinv;
		return Y;
	}
	SpatialMatrix(){}
	SpatialMatrix(const Matrix3d& _vv, const Matrix3d& _vw, const Matrix3d& _wv, const Matrix3d& _ww):
		vv(_vv), vw(_vw), wv(_wv), ww(_ww){}
	SpatialMatrix& operator=(const SpatialTransform& X){
		vv = X.R.trans();
		vw = -1.0 * X.R.trans() * Matrix3d::Cross(X.r);
		wv.clear();
		ww = X.R.trans();
		return *this;
	}
	SpatialMatrix& operator=(const SpatialTransformTranspose& Xtr){
		vv = Xtr.R;
		vw.clear();
		wv = Matrix3d::Cross(Xtr.r) * Xtr.R;
		ww = Xtr.R;
		return *this;
	}
	SpatialMatrix& operator+= (const SpatialMatrix& M){
		vv += M.vv; vw += M.vw; wv += M.wv; ww += M.ww;
		return *this;
	}
	SpatialMatrix& operator*= (double k){
		vv *= k; vw *= k; wv *= k; ww *= k;
		return *this;
	}
};
inline SpatialMatrix operator+(const SpatialMatrix& lhs, const SpatialMatrix& rhs){
	return SpatialMatrix(lhs.vv + rhs.vv, lhs.vw + rhs.vw, lhs.wv + rhs.wv, lhs.ww + rhs.ww);
}
inline SpatialMatrix operator-(const SpatialMatrix& lhs, const SpatialMatrix& rhs){
	return SpatialMatrix(lhs.vv - rhs.vv, lhs.vw - rhs.vw, lhs.wv - rhs.wv, lhs.ww - rhs.ww);
}
inline SpatialMatrix operator*(const SpatialMatrix& lhs, const SpatialMatrix& rhs){
	return SpatialMatrix(
		lhs.vv * rhs.vv + lhs.vw * rhs.wv, lhs.vv * rhs.vw + lhs.vw * rhs.ww,
		lhs.wv * rhs.vv + lhs.ww * rhs.wv, lhs.wv * rhs.vw + lhs.ww * rhs.ww);
}
inline SpatialVector operator*(const SpatialMatrix& M, const SpatialVector& V){
	SpatialVector Y;
	Y.v = M.vv * V.v + M.vw * V.w;
	Y.w = M.wv * V.v + M.ww * V.w;
	return Y;
}
inline SpatialMatrix VVtr(const SpatialVector& lhs, const SpatialVector& rhs){
	SpatialMatrix Y;
	Y.vv = VVtr(lhs.v, rhs.v);
	Y.vw = VVtr(lhs.v, rhs.w);
	Y.wv = VVtr(lhs.w, rhs.v);
	Y.ww = VVtr(lhs.w, rhs.w);
	return Y;
}

inline SpatialMatrix Xtr_Mat_X(const SpatialTransform& X, const SpatialMatrix& A){
	/*SpatialMatrix Y;
	Matrix3d tmp = Matrix3d::Cross(X.R.trans() * X.r);
	Matrix3d tmp2 = A.vw - A.vv * tmp;
	Y.vv = X.R * A.vv * X.R.trans();
	Y.vw = X.R * tmp2 * X.R.trans();
	Y.wv = X.R * (tmp * A.vv + A.wv) * X.R.trans();
	Y.ww = X.R * (tmp * tmp2 - A.wv * tmp + A.ww) * X.R.trans();*/
	SpatialMatrix Xtrm, Xm;
	Xtrm = X.trans();
	Xm = X;
	return Xtrm * A * Xm;
}

}

#endif

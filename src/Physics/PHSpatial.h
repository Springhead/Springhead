/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSPATIAL_H
#define PHSPATIAL_H

/**
 SpatialVector	  V(v, w) = [v; w];
 SpatialTransform X(R, r) = [R^T, -R^T * r%; O, R^T];
 ^Tは転置, %は外積行列，Oは零行列．
 X(R, r) * V(v, w) = [R^T * (v + w % r); R^T * w];

 合成: X(R1, r1) * X(R2, r2) = X(R2 * R1, r2 + R2 * r1);
 逆:   X(R, r)^-1 = X(R^T, -R^T * r);

 転置: X(R, r)^T = [R, O; r% * R, R];
	X(R, r)^T * V(f, t) = V(R * f, r % (R * f) + R * t);
 転置の合成: X(R1, r1)^T * X(R2, r2)^T = (X(R2, r2) * X(R1, r1))^T = X(R1 * R2, r1 + R1 * r2)^T
 転置の逆: (X(R, r)^T)^-1 = (X(R, r)^-1)^T

 */

namespace Spr{;

#define SUBMAT(r, c, h, w) sub_matrix(PTM::TSubMatrixDim<r, c, h, w>())
#define SUBVEC(o, l) sub_vector(PTM::TSubVectorDim<o, l>())

///	v1 * v2^T で得られる行列
inline Matrix3d VVtr(const Vec3d& v1, const Vec3d& v2){
	return Matrix3d(
		v1[0] * v2[0], v1[0] * v2[1], v1[0] * v2[2],
		v1[1] * v2[0], v1[1] * v2[1], v1[1] * v2[2],
		v1[2] * v2[0], v1[2] * v2[1], v1[2] * v2[2]);
}

/// SpatialTransform
struct SpatialTransformTranspose;
struct SpatialTransform{
	Vec3d		r;
	Quaterniond	q;

	SpatialTransform(){}
	SpatialTransform(const Vec3d& _r, const Quaterniond& _q):r(_r), q(_q){}

	SpatialTransform inv()const{
		return SpatialTransform(-(q.Conjugated() * r), q.Conjugated());
	}
	const SpatialTransformTranspose& trans()const{
		return (SpatialTransformTranspose&)*this;
	}
};

inline SpatialTransform operator*(const SpatialTransform& lhs, const SpatialTransform& rhs){
	return SpatialTransform(rhs.r + rhs.q * lhs.r, rhs.q * lhs.q);
}

/// SpatialTransformTranspose
struct SpatialTransformTranspose{
	Vec3d		r;
	Quaterniond	q;

	SpatialTransformTranspose(){}
	SpatialTransformTranspose(const Vec3d& _r, const Quaterniond& _q):r(_r), q(_q){}

	SpatialTransformTranspose inv()const{
		return SpatialTransformTranspose(-(q.Conjugated() * r), q.Conjugated());
	}
	const SpatialTransform& trans()const{
		return (SpatialTransform&)*this;
	}

};

inline SpatialTransformTranspose operator*(const SpatialTransformTranspose& lhs, const SpatialTransformTranspose& rhs){
	return SpatialTransformTranspose(lhs.r + lhs.q * rhs.r, lhs.q * rhs.q);
}

typedef PTM::TMatrixRow<6, 6, double> Matrix6d;

/// SpatialVector
struct SpatialVector : public Vec6d{
	Vec3d& v(){return *(Vec3d*)this;}
	Vec3d& w(){return *((Vec3d*)this + 1);}
	const Vec3d& v()const{return *(const Vec3d*)this;}
	const Vec3d& w()const{return *((const Vec3d*)this + 1);}

	SpatialVector(){
		clear();
	}
	SpatialVector(const Vec3d& _v, const Vec3d& _w){
		v() = _v;
		w() = _w;
	}
};

/** SpatialMatrix
	行ベクトルをポインタでとるためにTMatrixRowを継承
 */
struct SpatialMatrix : public PTM::TMatrixRow<6, 6, double>{
	typedef PTM::TSubMatrixRow<3, 3, PTM::TMatrixRow<6, 6, double>::desc> SubMatrix;
	SubMatrix&	vv(){return SUBMAT(0, 0, 3, 3);}
	SubMatrix&	vw(){return SUBMAT(0, 3, 3, 3);}
	SubMatrix&	wv(){return SUBMAT(3, 0, 3, 3);}
	SubMatrix&	ww(){return SUBMAT(3, 3, 3, 3);}

	//SpatialMatrix inv(){
	//	Matrix3d vvinv = vv.inv();
	//	Matrix3d S = ww - wv * vvinv * vw;
	//	Matrix3d Sinv = S.inv();
	//	SpatialMatrix Y;
	//	Y.vw = -1.0 * vvinv * vw * Sinv;
	//	Y.vv = (Matrix3d::Unit() - Y.vw * wv) * vvinv;
	//	Y.wv = -1.0 * Sinv * wv * vvinv;
	//	Y.ww = Sinv;
	//	return Y;
	//}
	SpatialMatrix& operator=(const SpatialTransform& X){
		X.q.Conjugated().ToMatrix(vv());
		vw() = -1.0 * vv() * Matrix3d::Cross(X.r);
		wv().clear();
		ww() = vv();
		return *this;
	}
	SpatialMatrix& operator=(const SpatialTransformTranspose& Xtr){
		Xtr.q.ToMatrix(vv());
		vw().clear();
		wv() = Matrix3d::Cross(Xtr.r) * vv();
		ww() = vv();
		return *this;
	}
	SpatialMatrix(){}
	SpatialMatrix(const SpatialMatrix& X){
		*this = X;
	}
	SpatialMatrix(const SpatialTransform& X){
		*this = X;
	}
};

inline double QuadForm(const SpatialVector& v1, const SpatialMatrix& M, const SpatialVector& v2){
	double y = 0.0;
	for(int i = 0; i < 6; i++)for(int j = 0; j < 6; j++)
		y += v1[i] * M[i][j] * v2[j];
	return y;
}

inline SpatialVector operator+ (const SpatialVector& lhs, const SpatialVector& rhs){
	return SpatialVector(lhs.v() + rhs.v(), lhs.w() + rhs.w());
}
inline SpatialVector operator- (const SpatialVector& lhs, const SpatialVector& rhs){
	return SpatialVector(lhs.v() - rhs.v(), lhs.w() - rhs.w());
}
inline SpatialVector operator* (double k, const SpatialVector& V){
	return SpatialVector(k * V.v(), k * V.w());
}
inline SpatialVector operator* (const SpatialVector& V, double k){
	return SpatialVector(k * V.v(), k * V.w());
}
inline double operator* (const SpatialVector& lhs, const SpatialVector& rhs){
	return lhs.v() * rhs.v() + lhs.w() * rhs.w();
}
inline SpatialVector operator*(const SpatialTransform& X, const SpatialVector& V){
	return SpatialVector(X.q.Conjugated() * (V.v() + V.w() % X.r), X.q.Conjugated() * V.w());
}
inline SpatialVector operator*(const SpatialTransformTranspose& X, const SpatialVector& V){
	Vec3d tmp = X.q * V.v();
	return SpatialVector(tmp, X.r % tmp + X.q * V.w());
}
inline SpatialVector operator/(const SpatialVector& lhs, double k){
	return SpatialVector(lhs.v() / k, lhs.w() / k);
}

inline SpatialMatrix operator+(const SpatialMatrix& lhs, const SpatialMatrix& rhs){
	SpatialMatrix Y;
	(Matrix6d&)Y = (const Matrix6d&)lhs + (const Matrix6d&)rhs;
	return Y;
}
inline SpatialMatrix operator-(const SpatialMatrix& lhs, const SpatialMatrix& rhs){
	SpatialMatrix Y;
	(Matrix6d&)Y = (const Matrix6d&)lhs - (const Matrix6d&)rhs;
	return Y;
}
inline SpatialMatrix operator*(const SpatialMatrix& lhs, const SpatialMatrix& rhs){
	SpatialMatrix Y;
	(Matrix6d&)Y = (const Matrix6d&)lhs * (const Matrix6d&)rhs;
	return Y;
}
inline SpatialVector operator*(const SpatialMatrix& M, const SpatialVector& V){
	SpatialVector Y;
	(Vec6d&)Y = (const Matrix6d&)M * (const Vec6d&)V;
	return Y;
}
inline SpatialMatrix VVtr(const SpatialVector& lhs, const SpatialVector& rhs){
	SpatialMatrix Y;
	Y.vv() = VVtr(lhs.v(), rhs.v());
	Y.vw() = VVtr(lhs.v(), rhs.w());
	Y.wv() = VVtr(lhs.w(), rhs.v());
	Y.ww() = VVtr(lhs.w(), rhs.w());
	return Y;
}

inline void Xtr_Mat_X(SpatialMatrix& Y, const SpatialTransform& X, const SpatialMatrix& A){
	//SpatialMatrix Y;
	//Matrix3d tmp = Matrix3d::Cross(X.R.trans() * X.r);
	//Matrix3d tmp2 = A.vw() - A.vv() * tmp;
	//Y.vv() = X.R * A.vv() * X.R.trans();
	//Y.vw() = X.R * tmp2 * X.R.trans();
	//Y.wv() = X.R * (tmp * A.vv() + A.wv()) * X.R.trans();
	//Y.ww() = X.R * (tmp * tmp2 - A.wv() * tmp + A.ww()) * X.R.trans();
	//return Y;
	SpatialMatrix Xtrm, Xm;
	Xtrm = X.trans();
	Xm = X;
	(Matrix6d&)Y = Xtrm * A * Xm;
}

}

#endif

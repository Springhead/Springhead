%module PTM

%include "workaround.i"

%{
#include <SprBase.h>
using namespace PTM;
using namespace Spr;
%}

%define VEC_MEMBER(type)
void	clear();
double	norm();
double	square();
void	resize(size_t sz);
size_t	size();
type	unit();
void	unitize();
%enddef
	
%define MAT_MEMBER(mat, vec)
void	clear();
vec		col(size_t i);
vec		row(size_t i);
double	det();
size_t	height();
size_t	width();
void	resize(size_t nrow, size_t ncol);
mat		trans();
mat		inv();
%enddef

%define VEC_EXTEND(type)
double __getitem__(size_t index){
	return $self->operator[](index);
}
void __setitem__(size_t index, double val){
	$self->operator[](index) = val;
}
type __add__(type v){
	return *$self + v;
}
type __sub__(type v){
	return *$self - v;
}
double __mul__(type v){
	return *$self * v;
}
%enddef

%define MAT_EXTEND(mat, vec)
double __getitem__(size_t r, size_t c){
	return (*$self)[r][c];
}
void __setitem__(size_t r, size_t c, double val){
	(*$self)[r][c] = val;
}
mat __add__(mat m){
	return *$self + m;
}
mat __sub__(mat m){
	return *$self - m;
}
mat __mul__(mat m){
	return *$self * m;
}
vec __mul__(vec v){
	return *$self * v;
}
%enddef

class Vec2d{
public:
	double x, y;
	VEC_MEMBER(Vec2d)
	static Vec2d Zero();
	Vec2d();
	Vec2d(double xi, double yi);
};
%extend Vec2d{
	VEC_EXTEND(Vec2d)
}

class Vec3d{
public:
	double x, y, z;
	VEC_MEMBER(Vec3d)
	static Vec3d Zero();
	Vec3d();
	Vec3d(double xi, double yi, double zi);
};
%extend Vec3d{
	VEC_EXTEND(Vec3d)
}

class Vec4d{
public:
	double x, y, z, w;
	VEC_MEMBER(Vec4d)
	static Vec4d Zero();
	Vec4d();
	Vec4d(double xi, double yi, double zi, double wi);
};
%extend Vec4d{
	VEC_EXTEND(Vec4d)
}

class Matrix2d{
public:
	double xx, xy, yx, yy;
	MAT_MEMBER(Matrix2d, Vec2d)	
	Matrix2d();
	Matrix2d(const Vec2d& exi, const Vec2d& eyi);
	Matrix2d(double m11, double m12, double m21, double m22);
	static Matrix2d Zero();
	static Matrix2d Unit();
	static Matrix2d Diag(double x, double y);
	static Matrix2d Rot(double rad);
	static Matrix2d Rot(const Vec2d& a, char axis);
	double angle() const;
};

%extend Matrix2d{
	MAT_EXTEND(Matrix2d, Vec2d)
}

class Matrix3d{
public:
	double xx, xy, xz;
	double yx, yy, yz;
	double zx, zy, zz;
	MAT_MEMBER(Matrix3d, Vec3d)
	Matrix3d();
	Matrix3d(const Vec3d& exi, const Vec3d& eyi, const Vec3d& ezi);
	Matrix3d(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33);

	static Matrix3d Zero();
	static Matrix3d Unit();
	static Matrix3d Diag(double x, double y, double z);
	static Matrix3d Rot(const Vec3d& a, const Vec3d& b, char axis = 'x');
	static Matrix3d Rot(double th, char axis);
	static Matrix3d Rot(double th, const Vec3d& axis);
	static Matrix3d Rot(const Vec4d& q);
	static Matrix3d Cross(const Vec3d& v);
};

%extend Matrix3d{
	MAT_EXTEND(Matrix3d, Vec3d)
}

bool IsUnitary(Matrix3d r);

%rename("trn")  Affine2d::getTrn;
%rename("trn=") Affine2d::setTrn;
%rename("pos")  Affine2d::getPos;
%rename("pos=") Affine2d::setPos;
%rename("rot")  Affine2d::getRot;
%rename("rot=") Affine2d::setRot;
%extend Affine2d{
	MAT_EXTEND(Affine2d, Vec2d)
	void setTrn(const Vec2d& v){
		$self->Trn() = v;
	}
	Vec2d getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec2d& v){
		$self->Pos() = v;
	}
	Vec2d getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix2d& m){
		$self->Rot() = m;
	}
	Matrix2d getRot(){
		return $self->Rot();
	}
}	

class Affine2d{
public:
	double xx, xy, xz;
	double yx, yy, yz;
	double px, py, pz;
	MAT_MEMBER(Affine2d, Vec3d)
	static Affine2d Unit();
	static Affine2d Trn(double px, double py);
	static Affine2d Rot(double th);
	static Affine2d Scale(double sx, double sy);
	Affine2d();
};

%rename("trn")  Affined::getTrn;
%rename("trn=") Affined::setTrn;
%rename("pos")  Affined::getPos;
%rename("pos=") Affined::setPos;
%rename("rot")  Affined::getRot;
%rename("rot=") Affined::setRot;
%extend Affined{
	MAT_EXTEND(Affined, Vec3d)
	void setTrn(const Vec3d& v){
		$self->Trn() = v;
	}
	Vec3d getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec3d& v){
		$self->Pos() = v;
	}
	Vec3d getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix3d& m){
		$self->Rot() = m;
	}
	Matrix3d getRot(){
		return $self->Rot();
	}
}	
class Affined{
public:
	double xx, xy, xz, xw;
	double yx, yy, yz, yw;
	double zx, zy, zz, zw;
	double px, py, pz, pw;
	MAT_MEMBER(Affined, Vec4d)
	static Affined Unit();
	static Affined Trn(double px, double py, double pz);
	static Affined Rot(double th, char axis);
	static Affined Rot(double th, const Vec3d& axis);
	static Affined Scale(double sx, double sy, double sz);
	static Affined ProjectionGL(
		const Vec3d& screen,
		const Vec2d& size,
		double front=1.0f, double back=10000.0f);
	static Affined ProjectionD3D(
		const Vec3d& screen,
		const Vec2d& size,
		double front=1.0f, double back=10000.0f);
		
	void LookAt(const Vec3d& posi);
	void LookAt(const Vec3d& posz, const Vec3d& posy);
	void LookAtGL(const Vec3d& posi);
	void LookAtGL(const Vec3d& posz, const Vec3d& posy);
	
	Affined();
};

%rename("v") Quaterniond::getV;
%rename("v=") Quaterniond::setV;
%extend Quaterniond{
	double __getitem__(size_t index){
		return $self->operator[](index);
	}
	void __setitem__(size_t index, double val){
		$self->operator[](index) = val;
	}
	Quaterniond __add__(Quaterniond q){
		return *$self + q;
	}
	Quaterniond __sub__(Quaterniond q){
		return *$self - q;
	}
	Quaterniond __mul__(Quaterniond q){
		return *$self * q;	
	}
	Vec3d transform(Vec3d v){
		return *$self * v;
	}
	
	void setV(const Vec3d& v){
		$self->V() = v;
	}
	Vec3d getV(){
		return $self->V();
	}
}
class Quaterniond{
public:
	double w,x,y,z;
	VEC_MEMBER(Quaterniond)
	Vec3d RotationHalf();
	Vec3d Rotation();
	Vec3d Axis();
	double Theta();
	
	Quaterniond();
	Quaterniond(double wi, double xi, double yi, double zi);
	
	static Quaterniond Rot(double angle, const Vec3d& axis);
	static Quaterniond Rot(double angle, char axis);
	static Quaterniond Rot(const Vec3d& rot);

	void Conjugate();
	Quaterniond Conjugated() const;
	Quaterniond Inv() const;

	void FromMatrix(const Matrix3d& m);
	void ToMatrix(Matrix3d& mat) const;
	void ToEular(Vec3d& v);
	void FromEular(const Vec3d& v);
	void RotationArc(const Vec3d& lhs, const Vec3d& rhs);
	void Euler(double yaw, double pitch, double roll);
	Quaterniond Derivative(const Vec3d& w);
	Vec3d AngularVelocity(const Quaterniond& qd);
};
double dot(const Quaterniond& q1, const Quaterniond& q2);
Quaterniond interpolate(double t, const Quaterniond& q1, const Quaterniond& q2);

%rename("pos") Posed::getPos;
%rename("pos=") Posed::setPos;
%rename("ori") Posed::getOri;
%rename("ori=") Posed::setOri;
%extend Posed{
	Vec3d transform(Vec3d v){
		return *$self * v;
	}
	Posed __mul__(Posed p){
		return *$self * p;
	}
	void setPos(const Vec3d& v){
		$self->Pos() = v;
	}
	Vec3d getPos(){
		return $self->Pos();
	}
	void setOri(const Quaterniond& q){
		$self->Ori() = q;
	}
	Quaterniond getOri(){
		return $self->Ori();
	}
}
class Posed{
public:
	double w,x,y,z;
	double px, py, pz;
	VEC_MEMBER(Posed)
	Posed Inv() const;
	
	static Posed Unit();
	static Posed Unit(const Vec3d& v);
	static Posed Unit(const Quaterniond &q);
	static Posed Trn(double px, double py, double pz);
	static Posed Trn(const Vec3d &v);
	static Posed Rot(double wi, double xi, double yi, double zi);
	static Posed Rot(double angle, const Vec3d& axis);
	static Posed Rot(double angle, char axis);
	static Posed Rot(const Vec3d& rot);

	void FromAffine(const Affined& f);
	void ToAffine(Affined& af) const;

	Posed();	
};
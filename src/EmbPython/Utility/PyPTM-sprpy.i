%define VEC_MEMBER(type)
void	clear();
double	norm();
double	square();
void	resize(size_t sz);
size_t	size();
type	unit();
void	unitize();
%enddef
	
%define VEC_CONSTRUCTOR(vec, elem, argname)
vec();
vec(vec&);
vec(elem* argname);
%enddef

%define VEC_EXTEND(vec, elem)
elem __getitem__(size_t index){
	return $self->operator[](index);
}
void __setitem__(size_t index, elem val){
	$self->operator[](index) = val;
}
vec __add__(vec v){
	return *$self + v;
}
vec __sub__(vec v){
	return *$self - v;
}
vec __mul__(elem k){
	return *$self * k;
}
elem __mul__(vec v){
	return *$self * v;
}
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

%define MAT_CONSTRUCTOR(mat)
mat();
mat(const mat&);
%enddef

%define MAT_EXTEND(mat, vec, elem)
elem __getitem__(size_t r, size_t c){
	return (*$self)[r][c];
}
void __setitem__(size_t r, size_t c, elem val){
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
mat __mul__(elem k){
	return *$self * k;
}
%enddef



namespace Spr{

double Deg(double rad);
double Rad(double deg);

class Vec2f{
public:
	float x, y;
	VEC_MEMBER(Vec2f)
	VEC_CONSTRUCTOR(Vec2f, float, _2f)
	Vec2f(float xi, float yi);
};
%extend Vec2f{
	VEC_EXTEND(Vec2f, float)
}

class Vec2d{
public:
	double x, y;
	VEC_MEMBER(Vec2d)
	VEC_CONSTRUCTOR(Vec2d, double, _2d)
	Vec2d(double xi, double yi);
};
%extend Vec2d{
	VEC_EXTEND(Vec2d, double)
}

class Vec3f{
public:
	float x, y, z;
	VEC_MEMBER(Vec3f)
	VEC_CONSTRUCTOR(Vec3f, float, _3f)
	Vec3f(float xi, float yi, float zi);
};
%extend Vec3f{
	VEC_EXTEND(Vec3f, float)
}
class Vec3d{
public:
	double x, y, z;
	VEC_MEMBER(Vec3d)
	VEC_CONSTRUCTOR(Vec3d, double, _3d)
	Vec3d(double xi, double yi, double zi);
};
%extend Vec3d{
	VEC_EXTEND(Vec3d, double)
}

class Vec4f{
public:
	float x, y, z, w;
	VEC_MEMBER(Vec4f)
	VEC_CONSTRUCTOR(Vec4f, float, _4f)
	Vec4f(float xi, float yi, float zi, float wi);
};
%extend Vec4f{
	VEC_EXTEND(Vec4f, float)
}
class Vec4d{
public:
	double x, y, z, w;
	VEC_MEMBER(Vec4d)
	VEC_CONSTRUCTOR(Vec4d, double, _4d)
	Vec4d(double xi, double yi, double zi, double wi);
};
%extend Vec4d{
	VEC_EXTEND(Vec4d, double)
}

class Matrix2f{
public:
	float xx, xy, yx, yy;
	MAT_MEMBER(Matrix2f, Vec2f)
	Matrix2f();
	Matrix2f(const Vec2f& exi, const Vec2f& eyi);
	Matrix2f(float m11, float m12, float m21, float m22);
	static Matrix2f Zero();
	static Matrix2f Unit();
	static Matrix2f Diag(float x, float y);
	static Matrix2f Rot(float rad);
	static Matrix2f Rot(const Vec2f& a, char axis);
	float angle() const;
};
%extend Matrix2f{
	MAT_EXTEND(Matrix2f, Vec2f, float)
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
	MAT_EXTEND(Matrix2d, Vec2d, double)
}

class Matrix3f{
public:
	float xx, xy, xz;
	float yx, yy, yz;
	float zx, zy, zz;
	MAT_MEMBER(Matrix3f, Vec3f)
	Matrix3f();
	Matrix3f(const Vec3f& exi, const Vec3f& eyi, const Vec3f& ezi);
	Matrix3f(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);

	static Matrix3f Zero();
	static Matrix3f Unit();
	static Matrix3f Diag(float x, float y, float z);
	static Matrix3f Rot(const Vec3f& a, const Vec3f& b, char axis = 'x');
	static Matrix3f Rot(float th, char axis);
	static Matrix3f Rot(float th, const Vec3f& axis);
	static Matrix3f Rot(const Vec4f& q);
	static Matrix3f Cross(const Vec3f& v);
};
%extend Matrix3f{
	MAT_EXTEND(Matrix3f, Vec3f, float)
}
bool IsUnitary(Matrix3f r);
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
	MAT_EXTEND(Matrix3d, Vec3d, double)
}
bool IsUnitary(Matrix3d r);

// TAffineだけはグラフィクス系との親和性を考慮してfloat, double両方の具現化をポートする
%extend Affine2f{
	MAT_EXTEND(Affine2f, Vec2f, float)
	void setTrn(const Vec2f& v){
		$self->Trn() = v;
	}
	Vec2f getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec2f& v){
		$self->Pos() = v;
	}
	Vec2f getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix2f& m){
		$self->Rot() = m;
	}
	Matrix2f getRot(){
		return $self->Rot();
	}
}	
class Affine2f{
public:
	float xx, xy, xz;
	float yx, yy, yz;
	float px, py, pz;
	MAT_MEMBER(Affine2f, Vec3d)
	static Affine2f Unit();
	static Affine2f Trn(float px, float py);
	static Affine2f Rot(float th);
	static Affine2f Scale(float sx, float sy);
	Affine2f();
};

%extend Affinef{
	MAT_EXTEND(Affinef, Vec3f, float)
	void setTrn(const Vec3f& v){
		$self->Trn() = v;
	}
	Vec3f getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec3f& v){
		$self->Pos() = v;
	}
	Vec3f getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix3f& m){
		$self->Rot() = m;
	}
	Matrix3f getRot(){
		return $self->Rot();
	}
}	
class Affinef{
public:
	float xx, xy, xz, xw;
	float yx, yy, yz, yw;
	float zx, zy, zz, zw;
	float px, py, pz, pw;
	MAT_MEMBER(Affinef, Vec4f)
	static Affinef Unit();
	static Affinef Trn(float px, float py, float pz);
	static Affinef Rot(float th, char axis);
	static Affinef Rot(float th, const Vec3d& axis);
	static Affinef Scale(float sx, float sy, float sz);
	static Affinef ProjectionGL(
		const Vec3f& screen,
		const Vec2f& size,
		float front=1.0f, float back=10000.0f);
	static Affinef ProjectionD3D(
		const Vec3f& screen,
		const Vec2f& size,
		float front=1.0f, float back=10000.0f);
		
	void LookAt(const Vec3f& posi);
	void LookAt(const Vec3f& posz, const Vec3f& posy);
	void LookAtGL(const Vec3f& posi);
	void LookAtGL(const Vec3f& posz, const Vec3f& posy);
	
	Affinef();
};

%extend Affine2d{
	MAT_EXTEND(Affine2d, Vec2d, double)
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

%extend Affined{
	MAT_EXTEND(Affined, Vec3d, double)
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
	
	VEC_CONSTRUCTOR(Quaterniond, double, _4d)
	Quaterniond(double wi, double xi, double yi, double zi);
	
	static Quaterniond Rot(double angle, const Vec3d& axis);
	static Quaterniond Rot(double angle, char axis);
	static Quaterniond Rot(const Vec3d& rot);

	void Conjugate();
	Quaterniond Conjugated() const;
	Quaterniond Inv() const;

	void FromMatrix(const Matrix3d& m);
	void ToMatrix(Matrix3d& mat) const;
	void ToEuler(Vec3d& v);
	void FromEuler(const Vec3d& v);
	void RotationArc(const Vec3d& lhs, const Vec3d& rhs);
	void Euler(double yaw, double pitch, double roll);
	Quaterniond Derivative(const Vec3d& w);
	Vec3d AngularVelocity(const Quaterniond& qd);
};
double dot(const Quaterniond& q1, const Quaterniond& q2);
Quaterniond interpolate(double t, const Quaterniond& q1, const Quaterniond& q2);



%extend Quaternionf{
	float __getitem__(size_t index){
		return $self->operator[](index);
	}
	void __setitem__(size_t index, float val){
		$self->operator[](index) = val;
	}
	Quaternionf __add__(Quaternionf q){
		return *$self + q;
	}
	Quaternionf __sub__(Quaternionf q){
		return *$self - q;
	}
	Quaternionf __mul__(Quaternionf q){
		return *$self * q;	
	}
	Vec3f transform(Vec3f v){
		return *$self * v;
	}
	
	void setV(const Vec3f& v){
		$self->V() = v;
	}
	Vec3f getV(){
		return $self->V();
	}
}
class Quaternionf{
public:
	float w,x,y,z;
	VEC_MEMBER(Quaternionf)
	Vec3f RotationHalf();
	Vec3f Rotation();
	Vec3f Axis();
	float Theta();
	
	VEC_CONSTRUCTOR(Quaternionf, float, _4d)
	Quaternionf(float wi, float xi, float yi, float zi);
	
	static Quaternionf Rot(float angle, const Vec3f& axis);
	static Quaternionf Rot(float angle, char axis);
	static Quaternionf Rot(const Vec3f& rot);

	void Conjugate();
	Quaternionf Conjugated() const;
	Quaternionf Inv() const;

	void FromMatrix(const Matrix3d& m);
	void ToMatrix(Matrix3d& mat) const;
	void ToEuler(Vec3f& v);
	void FromEuler(const Vec3f& v);
	void RotationArc(const Vec3f& lhs, const Vec3f& rhs);
	void Euler(float yaw, float pitch, float roll);
	Quaternionf Derivative(const Vec3f& w);
	Vec3f AngularVelocity(const Quaternionf& qd);
};
float dot(const Quaternionf& q1, const Quaternionf& q2);
Quaternionf interpolate(float t, const Quaternionf& q1, const Quaternionf& q2);


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
	//static Posed Unit(const Vec3d& v);
	//static Posed Unit(const Quaterniond &q);
	static Posed Trn(double px, double py, double pz);
	static Posed Trn(const Vec3d &v);
	static Posed Rot(double wi, double xi, double yi, double zi);
	static Posed Rot(double angle, const Vec3d& axis);
	static Posed Rot(double angle, char axis);
	static Posed Rot(const Vec3d& rot);
	static Posed Rot(const Quaterniond& q);

	void FromAffine(const Affined& f);
	void ToAffine(Affined& af) const;

	VEC_CONSTRUCTOR(Posed, double, _7d)
};

//class string{};

//クリティカルセクション
UTCriticalSection EPCriticalSection;
}	//	namespace Spr

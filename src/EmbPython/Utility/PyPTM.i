%include "EPObject.i"

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
//vec(elem* argname);
%enddef

%define VEC_EXTEND(vec, elem)
elem __getitem__(size_t $var1){
	return $self->operator[]($var1);
}
void __setitem__(size_t $var1, elem $var2){
	$self->operator[]($var1) = $var2;
}
vec __add__(vec $var1){
	return *$self + $var1;
}
vec __sub__(vec $var1){
	return *$self - $var1;
}
vec __mul__(elem $var1){
	return *$self * $var1;
}
elem __mul__(vec $var1){
	return *$self * $var1;
}
PyObject* tuple(){
	int size = $self->SIZE;
	PyObject* buf;
	PyObject* tuple =  PyTuple_New(size);

	for( int i=0 ; i < size ; i++){
		buf = Py_BuildValue("d",$self->data[i]);
		PyTuple_SetItem( tuple , i, buf);
	}
	return tuple;
}

//vector< vector<int> > testa(){
//	vector<int> v;
//	v.push_back(1);
//	v.push_back(2);
//	v.push_back(3);
//	
//	vector<int> v1;
//	v1.push_back(5);
//	v1.push_back(6);
//	v1.push_back(7);
//	
//	vector< vector<int> > vv;
//	vv.push_back(v);
//	vv.push_back(v1);
//	return vv;
//}

PyObject* __repr__(){
	int size = $self->SIZE;
	PyObject* buf = PyUnicode_FromString("(");
	PyObject* end = PyUnicode_FromString(")");

	char data_buf[32];
	for( int i=0 ; i < size ; i++){
		sprintf_s(data_buf,"%s%.20lf", (i==0?"":","),$self->data[i] );
		PyUnicode_AppendAndDel(&buf,PyUnicode_FromFormat(data_buf));
	}
	PyUnicode_AppendAndDel(&buf,end);
	
	return buf;
}

PyObject* __str__(){
	PyObject* repr = EP##vec##___repr__(self);
	PyObject* prefix = PyUnicode_FromString( #vec );
	PyUnicode_AppendAndDel( &prefix, repr);
	return prefix;
}

%enddef

%define VEC2_EXTEND(vec,elem)
elem CrossR(vec $var1){
	return PTM::cross(*$self,$var1);
}
elem CrossL(vec $var1){
	return PTM::cross(*$self,$var1);
}
%enddef

%define VEC3_EXTEND(vec,elem)
vec CrossR(vec $var1){
	return PTM::cross(*$self,$var1);
}
vec CrossL(vec $var1){
	return PTM::cross(*$self,$var1);
}
%enddef


%define VEC_EXTEND_EXPRESSION(vec,elem)
PyObject* tuple(){
	int size = $self->SIZE;
	PyObject* buf;
	PyObject* tuple =  PyTuple_New(size);

	for( int i=0 ; i < size ; i++){
		buf = Py_BuildValue("d",$self->data[i]);
		PyTuple_SetItem( tuple , i, buf);
	}
	return tuple;
}

PyObject* __repr__(){
	int size = $self->SIZE;
	PyObject* buf = PyUnicode_FromString("(");
	PyObject* end = PyUnicode_FromString(")");

	char data_buf[32];
	for( int i=0 ; i < size ; i++){
		sprintf_s(data_buf,"%s%.3lf", (i==0?"":","),$self->data[i] );
		PyUnicode_AppendAndDel(&buf,PyUnicode_FromFormat(data_buf));
	}
	PyUnicode_AppendAndDel(&buf,end);
	
	return buf;
}

PyObject* __str__(){
	PyObject* repr = EP##vec##___repr__(self);
	PyObject* prefix = PyUnicode_FromString( #vec );
	PyUnicode_AppendAndDel( &prefix, repr);
	return prefix;
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
elem __getitem__(size_t $var1, size_t $var2){
	return (*$self)[$var1][$var2];
}
void __setitem__(size_t $var1, size_t $var2, elem $var3){
	(*$self)[$var1][$var2] = $var3;
}
mat __add__(mat $var1){
	return *$self + $var1;
}
mat __sub__(mat $var1){
	return *$self - $var1;
}
mat __mul__(mat $var1){
	return *$self * $var1;
}
vec __mul__(vec $var1){
	return *$self * $var1;
}
mat __mul__(elem $var1){
	return *$self * $var1;
}
PyObject* tuple(){
	int w = $self->WIDTH;
	int h = $self->HEIGHT;
	PyObject* buf;
	PyObject* row = PyTuple_New(w);
	
	for( int i=0 ; i < h ; i++){
		PyObject* col = PyTuple_New(h);
		for ( int j=0 ; j < w ; j++){
			buf = Py_BuildValue("d",$self->data[j][i]);
			PyTuple_SetItem( col , j, buf);
		}
		PyTuple_SetItem(row, i , col);
	}
	return row;
}

PyObject* __repr__(){
	int w = $self->WIDTH;
	int h = $self->HEIGHT;
	
	PyObject* buf = PyUnicode_FromString("(");
	PyObject* end = PyUnicode_FromString(")");

	char data_buf[32];
	for( int i=0 ; i < h ; i++){
		PyUnicode_AppendAndDel(&buf,PyUnicode_FromString("("));
		for ( int j=0 ; j < w ; j++){
			sprintf_s(data_buf,"%s%.1lf", (j==0?"":","),$self->data[j][i] );
			PyUnicode_AppendAndDel(&buf,PyUnicode_FromFormat(data_buf));
		}
		PyUnicode_AppendAndDel(&buf,PyUnicode_FromString(")"));
	}
	PyUnicode_AppendAndDel(&buf,end);
	
	return buf;
}

PyObject* __str__(){
	PyObject* repr = EP##mat##___repr__(self);
	PyObject* prefix = PyUnicode_FromString( #mat );
	PyUnicode_AppendAndDel( &prefix, repr);
	return prefix;
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
	VEC2_EXTEND(Vec2f,float)
}
EXTEND_NEW(Vec2f)

class Vec2d{
public:
	double x, y;
	VEC_MEMBER(Vec2d)
	VEC_CONSTRUCTOR(Vec2d, double, _2d)
	Vec2d(double xi, double yi);
};
%extend Vec2d{
	VEC_EXTEND(Vec2d, double)
	VEC2_EXTEND(Vec2d, double)
}
EXTEND_NEW(Vec2d)

class Vec2i{
public:
	int x, y;
	VEC_MEMBER(Vec2i)
	VEC_CONSTRUCTOR(Vec2i, int, _2i)
	Vec2i(int xi, int yi);
};
%extend Vec2i{
	VEC_EXTEND(Vec2i, int)
	VEC2_EXTEND(Vec2i, int)
}
EXTEND_NEW(Vec2i)

class Vec3f{
public:
	float x, y, z;
	VEC_MEMBER(Vec3f)
	VEC_CONSTRUCTOR(Vec3f, float, _3f)
	Vec3f(float xi, float yi, float zi);
};
%extend Vec3f{
	VEC_EXTEND(Vec3f, float)
	VEC3_EXTEND(Vec3f, float)
}
EXTEND_NEW(Vec3f)

class Vec3d{
public:
	double x, y, z;
	VEC_MEMBER(Vec3d)
	VEC_CONSTRUCTOR(Vec3d, double, _3d)
	Vec3d(double xi, double yi, double zi);
};
%extend Vec3d{
	VEC_EXTEND(Vec3d, double)
	VEC3_EXTEND(Vec3d, double)
}
EXTEND_NEW(Vec3d)

class Vec3i{
public:
	int x, y, z;
	VEC_MEMBER(Vec3i)
	VEC_CONSTRUCTOR(Vec3i, int, _3i)
	Vec3i(int xi, int yi, int zi);
};
%extend Vec3i{
	VEC_EXTEND(Vec3i, int)
}
EXTEND_NEW(Vec3i)

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
EXTEND_NEW(Vec4f)

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
EXTEND_NEW(Vec4d)

class Vec4i{
public:
	int x, y, z, w;
	VEC_MEMBER(Vec4i)
	VEC_CONSTRUCTOR(Vec4i, int, _4i)
	Vec4i(int xi, int yi, int zi, int wi);
};
%extend Vec4i{
	VEC_EXTEND(Vec4i, int)
}
EXTEND_NEW(Vec4i)


class Vec6f{
public:
	float vx, vy, vz, wx, wy, wz;
	VEC_MEMBER(Vec6f)
	VEC_CONSTRUCTOR(Vec6f, float, _6f)
	Vec6f(float vxi, float vyi, float vzi, float wxi, float wyi, float wzi);
};
%extend Vec6f{
	VEC_EXTEND(Vec6f, float)
}
EXTEND_NEW(Vec6f)

class Vec6d{
public:
	double vx, vy, vz, wx, wy, wz;
	VEC_MEMBER(Vec6d)
	VEC_CONSTRUCTOR(Vec6d, double, _6d)
	Vec6d(double vxi, double vyi, double vzi, double wxi, double wyi, double wzi);
};
%extend Vec6d{
	VEC_EXTEND(Vec6d, double)
}
EXTEND_NEW(Vec6d)

class Vec6i{
public:
	int vx, vy, vz, wx, wy, wz;
	VEC_MEMBER(Vec6i)
	VEC_CONSTRUCTOR(Vec6i, int, _6i)
	Vec6i(int vxi, int vyi, int vzi, int wxi, int wyi, int wzi);
};
%extend Vec6i{
	VEC_EXTEND(Vec6i, int)
}
EXTEND_NEW(Vec6i)


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
EXTEND_NEW(Matrix2f)

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
EXTEND_NEW(Matrix2d)

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
EXTEND_NEW(Matrix3f)

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
EXTEND_NEW(Matrix3d)

// TAffineだけはグラフィクス系との親和性を考慮してfloat, double両方の具現化をポートする
%extend Affine2f{
	MAT_EXTEND(Affine2f, Vec2f, float)
	void setTrn(const Vec2f& $var1){
		$self->Trn() = $var1;
	}
	Vec2f getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec2f& $var1){
		$self->Pos() = $var1;
	}
	Vec2f getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix2f& $var1){
		$self->Rot() = $var1;
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
EXTEND_NEW(Affine2f)

%extend Affinef{
	MAT_EXTEND(Affinef, Vec3f, float)
	void setTrn(const Vec3f& $var1){
		$self->Trn() = $var1;
	}
	Vec3f getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec3f& $var1){
		$self->Pos() = $var1;
	}
	Vec3f getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix3f& $var1){
		$self->Rot() = $var1;
	}
	Matrix3f getRot(){
		return $self->Rot();
	}
	float get(int $var1, int $var2){
		return $self->data[$var1][$var2];
	}
	float set(int $var1, int $var2, float $var3){
		return $self->data[$var1][$var2] = $var3;
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
EXTEND_NEW(Affinef)

%extend Affine2d{
	MAT_EXTEND(Affine2d, Vec2d, double)
	void setTrn(const Vec2d& $var1){
		$self->Trn() = $var1;
	}
	Vec2d getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec2d& $var1){
		$self->Pos() = $var1;
	}
	Vec2d getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix2d& $var1){
		$self->Rot() = $var1;
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
EXTEND_NEW(Affine2d)

%extend Affined{
	MAT_EXTEND(Affined, Vec3d, double)
	void setTrn(const Vec3d& $var1){
		$self->Trn() = $var1;
	}
	Vec3d getTrn(){
		return $self->Trn();
	}
	void setPos(const Vec3d& $var1){
		$self->Pos() = $var1;
	}
	Vec3d getPos(){
		return $self->Pos();
	}
	void setRot(const Matrix3d& $var1){
		$self->Rot() = $var1;
	}
	Matrix3d getRot(){
		return $self->Rot();
	}
	double get(int $var1, int $var2){
		return $self->data[$var1][$var2];
	}
	double set(int $var1, int $var2, double $var3){
		return $self->data[$var1][$var2] = $var3;
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
EXTEND_NEW(Affined)

%extend Quaterniond{
	VEC_EXTEND_EXPRESSION(Vec4d,double)

	double __getitem__(size_t $var1){
		return $self->operator[]($var1);
	}
	void __setitem__(size_t $var1, double $var2){
		$self->operator[]($var1) = $var2;
	}
	Quaterniond __add__(Quaterniond $var1){
		return *$self + $var1;
	}
	Quaterniond __sub__(Quaterniond $var1){
		return *$self - $var1;
	}
	Quaterniond __mul__(Quaterniond $var1){
		return *$self * $var1;	
	}
	Vec3d __mul__(Vec3d $var1){
		return *$self * $var1;
	}
	
	void setV(const Vec3d& $var1){
		$self->V() = $var1;
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
EXTEND_NEW(Quaterniond)



%extend Quaternionf{
	VEC_EXTEND_EXPRESSION(Vec4f,float)
	float __getitem__(size_t $var1){
		return $self->operator[]($var1);
	}
	void __setitem__(size_t $var1, float $var2){
		$self->operator[]($var1) = $var2;
	}
	Quaternionf __add__(Quaternionf $var1){
		return *$self + $var1;
	}
	Quaternionf __sub__(Quaternionf $var1){
		return *$self - $var1;
	}
	Quaternionf __mul__(Quaternionf $var1){
		return *$self * $var1;	
	}
	Vec3f __mul__(Vec3f $var1){
		return *$self * $var1;
	}
	
	void setV(const Vec3f& $var1){
		$self->V() = $var1;
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
EXTEND_NEW(Quaternionf)


%extend Posed{
	Vec3d transform(Vec3d $var1){
		return *$self * $var1;
	}
	Posed __mul__(Posed $var1){
		return *$self * $var1;
	}
	void setPos(const Vec3d& $var1){
		$self->Pos() = $var1;
	}
	Vec3d getPos(){
		return $self->Pos();
	}
	void setOri(const Quaterniond& $var1){
		$self->Ori() = $var1;
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
	Posed(const Vec3d& p,const Quaterniond& q); 
	Posed(double,double,double,double,double,double,double);
};
EXTEND_NEW(Posed)


%extend Posef{
	Vec3f transform(Vec3f $var1){
		return *$self * $var1;
	}
	Posef __mul__(Posef $var1){
		return *$self * $var1;
	}
	void setPos(const Vec3f& $var1){
		$self->Pos() = $var1;
	}
	Vec3f getPos(){
		return $self->Pos();
	}
	void setOri(const Quaternionf& $var1){
		$self->Ori() = $var1;
	}
	Quaternionf getOri(){
		return $self->Ori();
	}
}
class Posef{
public:
	float w,x,y,z;
	float px, py, pz;
	VEC_MEMBER(Posef)
	Posef Inv() const;
	
	static Posef Unit();
	static Posef Trn(float px, float py, float pz);
	static Posef Trn(const Vec3f &v);
	static Posef Rot(float wi, float xi, float yi, float zi);
	static Posef Rot(float angle, const Vec3f& axis);
	static Posef Rot(float angle, char axis);
	static Posef Rot(const Vec3f& rot);
	static Posef Rot(const Quaternionf& q);

	void FromAffine(const Affinef& f);
	void ToAffine(Affinef& af) const;

	VEC_CONSTRUCTOR(Posef, float, _7f)
	Posef(const Vec3f& p,const Quaternionf& q); 
	Posef(float,float,float,float,float,float,float);
};
EXTEND_NEW(Posef)

}	//	namespace Spr

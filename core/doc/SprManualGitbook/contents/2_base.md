Baseモジュールは基本機能の集合体です．以下では項目別にそれらについて解説します．



## 行列・ベクトル演算

次表によく使われる行列・ベクトルクラスを示します．末尾の整数はベクトルや行列のサイズを表し，*f*，*d*はそれぞれ*float*型，*double*型に対応します．



### ベクトル
ベクトル型は物体の位置や速度，力などの物理量を表現するために頻繁に使われます．例えば*double*型の要素からなる3次元ベクトル*x*を定義するには
```c++
Vec3d x;
```
とします．要素アクセスは*[]*演算子を用います．
```c++
x[0];    // 0-th element
```
この他，*Vec\[2|3]\[f|d]*については*.x*, *.y*, *.z*でも要素アクセスできます．任意の固定サイズのベクトルも使えます．*float*型の10次元ベクトルは
```c++
TVector<10, float> x;    // 10-dimensional float vector
```
とします．可変長ベクトルは
```c++
VVector<float> x;
x.resize(10);            // can be resized at any time
```
で使えます.**ただしVVector::resize**によりサイズ変更を行うと既存の内容は破棄されますので注意して下さい．



基本的な演算は一通りサポートされています．

```c++
Vec3d a, b, c;
double k;

c = a + b;               // addition
a += b;

c = a - b;               // subtraction
a -= b;

b = k * a;               // multiply vector by scalar
a *= k;

k = x * y;               // scalar product

x % y;                   // vector product (3D vector only)
```
すべてのベクトル型について以下のメンバ関数が使えます．
```c++
a.size();                // number of elements
a.norm();                // norm
a.square();              // square of norm
a.unitize();             // normalize
b = a.unit();            // normalized vector
```



### 行列

行列は平行移動や回転などの変換や，剛体の慣性モーメントを表現するために使われます．例えば，*double*型の要素からなる3x3行列 `A` は次のように定義します．
```c++
Matrix3d A;
```
要素アクセスは*[]*演算子を用います．
```c++
x[0][1];    // element at 0-th row, 1-th column
```
任意の固定サイズの行列も使えます．メモリ上に列方向に要素が整列した行列は
```c++
TMatrixCol<2, 3, float> M;    // column-oriented 2x3 matrix
```
要素が行方向に整列した行列は
```c++
TMatrixRow<2, 3, float> M;    // row-oriented 2x3 matrix
```
となります．ちなみにさきほどの*Matrix3d*は*TMatrixCol<3,3,double>*と等価です．可変サイズ行列は
```c++
VMatrixCol<float> M;
M.resize(10, 13);             // column-oriented variable matrix
```
で使えます．*VMatrixCol*では要素はメモリ上で列方向に並びます．一方*VMatrixRow*では行方向に要素が並びます．行列型についても，ベクトル型と同様の四則演算がサポートされています．行列とベクトル間の演算は次のようになります．
```c++
Matrix3d M;
Vec3d a, b;

b = M * a;               // multiplication
```
すべての行列型について以下のメンバ関数で行数および列数が取得できます．
```c++
M.height();              // number of rows
M.width();               // number of columns
```
2x2, 3x3行列については以下の静的メンバ関数が用意されています．
```c++
Matrix2d N;
Matrix3d M;
double theta;
Vec3d axis;

// methods common to Matrix2[f|d] and Matrix3[f|d]
M = Matrix3d::Zero();        // zero matrix; same as M.clear()
M = Matrix3d::Unit();        // identity matrix
M = Matrix3d::Diag(x,y,z);   // diagonal matrix

N = Matrix2d::Rot(theta);    // rotation in 2D

M = Matrix3d::Rot(theta, 'x');    // rotation w.r.t. x-axis
                                  // one can specify 'y' and 'z' too
M = Matrix3d::Rot(theta, axis);   // rotation along arbitrary vector
```

### アフィン変換
アフィン変換は主にグラフィクスにおける変換を指定するために使用します．アフィン変換型*Affine[f|d]*は4x4行列としての機能を備えています．加えて以下のメンバ関数が使えます．
```c++
Affinef A;
Matrix3f R;
Vec3f p;

R = A.Rot();           // rotation part
p = A.Trn();           // translation part
```
また，よく使用するアフィン変換を生成する静的メンバが用意されています．
```c++
A = Affinef::Unit();            // identity transformation
A = Affinef::Trn(x, y, z);      // translation
A = Affinef::Rot(theta, 'x');   // rotation w.r.t. x-axis
                                // one can specify 'y' and 'z' too
A = Affinef::Rot(theta, axis);  // rotation w.r.t. arbitrary axis
A = Affinef::Scale(x, y, z);    // scaling
```

### クォータニオン
クォータニオンは主に物理計算における剛体の向きや回転を表現するために使います．クォータニオンは4次元ベクトルの基本機能を備えています．要素アクセスは*[]*演算子に加えて以下の方法が使えます．
```c++
Quaterniond q;
q.w;                   // same as q[0]
q.x;                   // same as q[1]
q.y;                   // same as q[2]
q.z;                   // same as q[3]
q.V();                 // vector composed of x,y,z elements
```
演算は以下のように行います．まず，クォータニオン同士の積は回転の合成を表します．
```c++
Quaterniond q, q0, q1;
q0 = Quaterniond::Rot(Rad(30.0), 'x');   // 30deg rotation along x-axis
q1 = Quaterniond::Rot(Rad(-90.0), 'y');  // -90deg rotationt along y-axis

q = q1 * q0;
```
つぎに，クォータニオンと3次元ベクトルとの積は，ベクトルの回転を表します．
```c++
Vec3d a(1, 0, 0);
Vec3d b = q0 * a;
```
このように，クォータニオンは基本的に回転行列を同じような感覚で使えます．*Quaterniond[f|d]*には以下のメンバ関数があります．まず回転軸と回転角度を取得するには
```c++
Vec3d axis = q.Axis();        // rotation axis
double angle = q.Theta();     // rotation angle
```
とします．また，逆回転を表す共役クォータニオンを得るには
```c++
q.Conjugate();         // conjugate (reverse rotation)

Quaterniond y;
y = q.Conjugated();    // return conjugated quaternion
y = q.Inv();           // return inverse (normalized conjugate)
```
とします．*Conjugate*はそのクォータニオン自体を共役クォータニオンに変換するのに対し，*Conjugated*は単位共役クォータニオンを返します．*Inv*は*Conjugated*とほぼ等価ですが，戻り値のノルムが$1$となるように正規化を行います．回転を表すクォータニオンは理論上は必ずノルムが$1$なので正規化は不要ですが，実際は数値計算における誤差で次第にノルムがずれてくることがあります．このような誤差を補正するために適宜正規化を行う必要があります．回転行列と相互変換するには以下のようにします．
```c++
Matrix3d R = Matrix3d::Rot(Rad(60.0), 'z');
q.FromMatrix(R);       // conversion from rotation matrix
q.ToMatrix(R);         // conversion to rotation matrix
```
*FromMatrix*は渡された回転行列*R*と等価なクォータニオンとして*q*を設定します．一方*ToMatrix*は，参照渡しされた*R*を*q*と等価な回転行列として設定します．同様に，以下はオイラー角との相互変換を行います．

```c++
Vec3d angle;
q.ToEuler(angle);      // to Euler angle
q.FromEuler(angle);    // from Euler angle
```
最後に，以下の関数は2つのベクトルに対し，片方をもう片方に一致されるような回転を表すクォータニオンを求めます．一般に2つのベクトルを一致させる回転は一意ではありませんが，*RotationArc*は両方のベクトルに直交する軸に関する回転，いわば最短距離の回転を求めます．
```c++
Vec3d r0(1, 0, 0), r1(0, 1, 0);
q.RotationArc(r0, r1);    // rotation that maps r0 to r1 
```

### ポーズ
ポーズは位置と向きの複合型です．役割としてはアフィン変換に似ていますが，全部で7つの成分で表現できるためアフィン変換よりもコンパクトです．ポーズは物理計算での剛体の位置と向きを表現するためなどに用います．*Pose[f|d]*型のメンバ変数は*Pos*と*Ori*の2つのみで，それぞれポーズの並進成分(*Vec3[f|d]*)と回転成分(*Quaternion[f|d]*)への参照を返します．
```c++
Posed P;
P.Pos() = Vec3d(1, 2, 3);
P.Ori() = Quaterniond::Rot(Rad(45.0), 'x');
Vec3d p = P.Pos();
Quaterniond q = P.Ori();
```

### 初期値について
*Vec\[2|3|4]\[f|d]*型はゼロベクトルに初期化されます．*Matrix\[2|3]\[f|d]*型および*Affine\[f|d]*型は単位行列に初期化されます．また，*Quaternion\[f|d]*は恒等写像を表すクォータニオンに初期化されます．



## スマートポインタ
参照カウントにもとづくスマートポインタです．参照カウントが$0$になったオブジェクトのメモリを自動的に解放するためにユーザが手動で*delete*を実行する手間が省け，メモリリークの危険が低減できます．参照カウントクラスは*UTRefCount*です．Springheadのほとんどのクラスは*UTRefCount*を継承しています．スマートポインタはテンプレートクラス*UTRef*です．以下に例を示します．
```c++
class A : public UTRefCount{};
UTRef<A> a = new A();
// no need to delete a
```



## その他の機能

### UTString
文字列型です．現状ではstd::stringと等価です．
### UTTypeDesc
Springheadのクラスが持つ実行時型情報です．
### UTTreeNode
ツリー構造の基本クラスです．


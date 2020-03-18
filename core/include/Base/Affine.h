/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef AFFINE_H
#define AFFINE_H

/** \addtogroup gpLinearAlgebra */
//@{
/** \defgroup gpAffine @!JAAffine行列クラス@!ENA Affine Matrix@!*
	\section secAffineIntro @!JAはじめに@!ENIntroduction@!*
		@!JA
		このページはAffine行列クラス(Spr::TAffine, Spr::TAffine2)の説明です．
		Affine行列クラスライブラリは，3Dシミュレーションに必須な
		Affine行列をC++のクラスにしたものです．
		@!EN
		This is a document for Affine Matrix Library.
		Affine Matrix Library is a set of classes for affine matrix,
		which are necessary for 3D simulation.
		@!*
	\section secAffineUsage @!JA使い方@!ENUsage@!*
	@!JA
	Affine 行列クラスライブラリは，ヘッダファイルだけからなる
	クラスライブラリなので, TAffine.h, TinyVec.h, TinyMat.h, TMatrix.h, TMatrixUtility.h, TVector.h
	を同じフォルダに入れておき，.cppファイルからヘッダをインクルードするだけで
	使用できます．
	@!EN
	Affine Matrix Library consist only from some header files.
	You can use this library just coping some header files
	(TAffine.h, TinyVec.h, TinyMat.h, TMatrix.h, TMatrixUtility.h, TVector.h)
	and including them from your .cpp files.
	@!*
	\subsection secAffineSample @!JA サンプル @!EN sample@!*
	\verbatim
#include "Affine.h"                         //  TAffine行列ライブラリのインクルードする．
#include <iostream>

using namespace Spr;	//  @!JA Affine行列クラスはSpr名前空間の中で宣言されている．@!EN Affine matrix class is declared in the name space of Spr.@!*

void main(){
    Affinef af=Affinef::Rad(Rad(30), 'z');  //  @!JA要素がfloatなTAffine行列を宣言. 
                                                @!ENDeclare an Affine matrix of float element.
                                                @!*
                                                @!JAz軸回り30度回転行列に初期化
                                                @!ENInitializing as a matrix of rotation of 30 degrees around z axis.
                                                @!*
                                                
    Vec3f vec(1,0,0);                       //  @!JA要素がfloatな3次元のベクトルを宣言
                                                @!ENDeclare a vector of float element.
                                                @!*
    std::cout << af;
    std::cout << vec << std::endl;
    std::cout << af * vec << std::endl;
}\endverbatim
	\subsection secAffineVecFunc @!JA Affine行列・ベクトルのメンバと演算  @!EN Functions of Affine matrixs and vectors. @!*
	@!JA
	普通に演算ができます．
	<ul>
	<li> +:和, -:差, *:積/内積, /:定数分の1
	<li> ==:比較, =:代入
	<li> <<:出力, >>:入力
	<li> %:ベクトルの外積
	</ul>
	@!EN Normal calculations are supported.
	<ul>
	<li> +:addition, -:subtraction, *:multiplication/inner product, /:division
	<li> ==:comparison, =:assign
	<li> <<:output, >>:input
	<li> %:cross product
	</ul>
	@!*
	@!JA
	Affine変換は，
	\verbatim
	TAffine<float> af; TVec3<float> v, af_v;
	af_v = af * v;\endverbatim
	とすればできます．
	@!EN
	Affine transformation is executed by:
	\verbatim
	TAffine<float> af; TVec3<float> v, af_v;
	af_v = af * v;\endverbatim
	@!*
	@!JA
	また，次のようにTAffine行列の部分を取り出すことができます．
	<ul>
		<li> af.Ex():	X軸基底ベクトル．(3次元ベクトル)．
		<li> af.Ey():	Y軸基底ベクトル．(3次元ベクトル)．
		<li> af.Ez():	Z軸基底ベクトル．(3次元ベクトル)．
		<li> af.Trn():	平行移動部分．(3次元ベクトル)．
		<li> af.Rot():	回転変換部分．(3×３行列)．
	</ul>
	@!EN
	You can extract some portion of the Affine matrix as:
	<ul>
		<li> af.Ex():	base vector for x axis (3 dimensional vector)
		<li> af.Ey():	base vector for y axis (3 dimensional vector)
		<li> af.Ez():	base vector for z axis (3 dimensional vector)
		<li> af.Trn():	portion of translation．(3 dimensional vector)
		<li> af.Rot():	portion of rotation．(3x3 matrix)
	</ul>
	@!*
	@!JA 部分への代入などもできます．
	@!EN You can assign portion of affine matrx.
	@!*
	\verbatim
	TAffine<float> af;
	af.Pos() = Vec3f(10,0,0);
	af.Rot() = TAffine<float>::Rot(Rad(30), 'x').Rot() * af.Rot();\endverbatim
	@!JA ベクトルは次のようなメンバ関数を持ちます．
	@!EN a vector has following members.
	@!*
	<ul>
	<li> unit(): @!JA 向きが等しい単位ベクトルを返す．
				 @!EN return a unit vector.
				 @!*
	<li> norm(): @!JA ベクトルの大きさ(ノルム)を返す．
				 @!EN return the size (norm) of a vector.
				 @!*
	</ul>
	\subsection secAffineConstruct Affine行列の初期化 
	TAffine行列(Spr::TAffine)には便利なコンストラクタや初期化関数を用意しました．
	いくつかをここで紹介します．
	<ul>
		<li> Spr::TAffine::Trn (T px, T py, T pz):	
				平行移動する行列に初期化．TはTAffine<T>のT．floatやdoubleなどで良い．
		<li> Spr::TAffine::Rot(element_type th, char axis):
				回転行列を返す．thはラジアン．axisは，'x', 'y', 'z'．element_typeはTのこと．
		<li> Spr::TAffine::ProjectionD3D (TVec3 screen, TVec2 size, T front=1.0f, T back=10000.0f):
				D3D用射影行列として初期化．
		<li> Spr::TAffine::ProjectionGL (TVec3 screen, TVec2 size, T front=1.0f, T back=10000.0f):
		<br>
			OpenGL用射影行列として初期化(-Zが前)．
			<ul>
				<li> screen  カメラから見たビューポートの中心の位置
				<li> size    ビューポートのサイズ
				<li> front   手前のクリッピング平面とカメラの距離
				<li> back    奥のクリッピング平面とカメラの距離
			</ul>
		<li> Spr::TAffine::LookAtGL (TVec3 pos, TVec3 diry)
			位置はそのままで，posに-Ez(), diry に Ey()が向くようなAffine行列．
			OpenGLのgluLookAtと等価．
	</ul>
	
	\section thanks 謝辞
	LU分解，逆行列，ガウス消去法などの行列計算アルゴリズムは，<br>
    「『Ｃ言語による最新アルゴリズム事典』全ソースコード」<br>
    ftp://ftp.matsusaka-u.ac.jp/pub/algorithms<br>
	奥村 晴彦 Haruhiko Okumura<br>
	を改変して流用させていただきました．
	自由にコードを使えるよう公開してくださってありがとうございます．

	@section secRefLA リファレンス
		@ref gpLinearAlgebra
*/
//@{


#include "TMatrix.h"
#include "TMatrixUtility.h"
#include "TinyVec.h"
#include "TinyMat.h"
#include "VectorDebug.h"

/**	@file Affine.h 2/3次元アフィン行列*/

#ifndef PTM_PACK	//	単体で使用する場合は，namespace に入れない
namespace Spr {
#endif

#undef M_PI
#ifdef __BORLANDC__
#define M_PI 3.14159265358979323846
#else
///	円周率π
const double M_PI = 3.14159265358979323846;
#endif

#undef abs
#ifdef __BORLANDC__
/*	絶対値．BCB6が違う関数をリンクしてしまう(たぶんバグ)ので，template は使用していない
	std::abs とぶつかるためだと思われる．	*/
#define DEF_ABS_FUNC(T)		inline T abs(T t){ return t > T()  ?  t  :  -t; }
DEF_ABS_FUNC(float)
DEF_ABS_FUNC(double)
DEF_ABS_FUNC(char)
DEF_ABS_FUNC(int)
DEF_ABS_FUNC(long)
#else
template <class T> T abs(T t){ return t > T()  ?  t  :  -t; }
#endif

#undef sign
///	符号(正なら1, 負なら -1 を返す)
template <class T> T sign(T t){
	return t > T()  ?  T(1)  :  T(-1);
}

#if _MSC_VER < 12

#undef min
/// 小さい方を返す
template <class T> T min(T a, T b){return a < b ? a : b;}

#undef max
/// 大きい方を返す
template <class T> T max(T a, T b){return a > b ? a : b;}

#endif

/// 平均をとる
template <class T> T ave(T a, T b){return T(0.5 * (a + b));}

///	度をラジアンに変換
inline double Rad(double deg){
	return ((double)deg/360*2*M_PI);
}
inline float Radf(double deg){
	return (float)((double)deg/360*2*M_PI);
}
///	ラジアンを度に変換
inline double Deg(double rad){
	return (rad/(2*M_PI)) * 360;
}
inline float Degf(double rad){
	return (float)(rad/(2*M_PI)) * 360;
}
///	2乗
template <class SC>
inline SC Square(SC x){
	return x*x;
}
///	2x2行列の行列式
template <class SC>
inline SC Det2(SC a, SC b, SC c, SC d){
	return ((a)*(d) - (b)*(c));
}

//-----------------------------------------------------------------------------
//	TAffine2

/**	TAffine2行列(回転,拡大,平行移動を表す)行列.
	概要は，\ref pgAffine 参照．
*/
template <class T>
class TAffine2:public PTM::TMatrixBase<3,3,
	PTM::TMatrixDescCol< TAffine2<T>, PTM::TMatrixRow<3,3,T>, 3,3,3,T> >{
public:
	typedef PTM::TMatrixDescCol< TAffine2<T>, PTM::TMatrixRow<3,3,T>, 3,3,3,T> desc;
	typedef PTM::TMatrixBase<3,3,desc> base_type;
	///	基本的なメンバの定義 @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TAffine2);
	union{
		struct{
			T xx, xy, xz;
			T yx, yy, yz;
			T px, py, pz;
		};
		T data[3][3];
	};
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[j][i]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[j][i]; }
	
	/**@name	基底ベクトルへのアクセス	*/
	//@{
	/// 
	TVec2<element_type>& Ex(){
		return *(TVec2<element_type>*) &this->item(0,0);
	}
	/// 
	const TVec2<element_type>& Ex() const{
		return *(TVec2<element_type>*) &this->item(0,0);
	}
	/// 
	TVec2<element_type>& Ey(){
		return *(TVec2<element_type>*) &this->item(0,1);
	}
	/// 
	const TVec2<element_type>& Ey() const{
		return *(TVec2<element_type>*) &this->item(0,1);
	}
	/// 
	TVec2<element_type>& Trn(){
		return *(TVec2<element_type>*) &this->item(0,2);
	}
	/// 
	const TVec2<element_type>& Trn() const{
		return *(TVec2<element_type>*) &this->item(0,2);
	}
	/// 
	TVec2<element_type>& Pos(){ return Trn(); }
	/// 
	const TVec2<element_type>& Pos() const { return Trn(); }
	//@}

	/**@name	要素へのアクセス	*/
	//@{
	/// 
	element_type& ExX() {return Ex().X();}
	/// 
	element_type& ExY() {return Ex().Y();}
	/// 
	element_type& EyX() {return Ey().X();}
	/// 
	element_type& EyY() {return Ey().Y();}
	/// 
	element_type& TrnX() {return Trn().X();}
	/// 
	element_type& TrnY() {return Trn().Y();}
	///	TrnX()の別名
	element_type& PosX() {return Trn().X();}
	///	TrnY()の別名
	element_type& PosY() {return Trn().Y();}
	//@}

	///	回転拡大変換部を取り出す.
#ifdef _WIN32
	PTM::TSubMatrixCol<2,2, desc>& Rot() { return sub_matrix(0,0,PTM::TSubMatrixCol<2,2, desc>()); }
#else
	PTM::TSubMatrixCol<2,2, desc>& Rot() { return this->sub_matrix(0,0,PTM::TSubMatrixCol<2,2, desc>()); }
#endif
	///	回転拡大変換部を取り出す (const版).
#ifdef _WIN32
	const PTM::TSubMatrixCol<2,2, desc>& Rot() const { return sub_matrix(0,0,PTM::TSubMatrixCol<2,2, desc>()); }
#else
	const PTM::TSubMatrixCol<2,2, desc>& Rot() const { return this->sub_matrix(0,0,PTM::TSubMatrixCol<2,2, desc>()); }
#endif
	
	/**@name	初期化と構築	*/
	//@{
	///	単位行列
	static TAffine2<T> Unit(){
		TAffine2<T> y;
		PTM::init_unitize(y);
		return y;
	}
	///	平行移動
	static TAffine2<T> Trn(element_type px, element_type py){
		TAffine2<T> y;
		y.Trn().X() = px;
		y.Trn().Y() = py;
		return y;
	}
	///	回転，なぜか引数がひとつだとVC.netでエラーになる．
	static TAffine2<T> Rot(element_type th, int d=0){
		TAffine2 y;
		PTM::init_rot(y.Rot(), th);
		return y;
	}
	/// 拡大
	static TAffine2<T> Scale(element_type sx, element_type sy){
		TAffine2<T> y;
		y.item(0, 0) = sx; y.item(1, 1) = sy;
		return y;
	}
	///コンストラクタ
	void set_default(){PTM::init_unitize(*this);}
	//@}
};

///	TAffine2とベクトルの掛け算
template <class TD, class TV>
TVec2<TV> operator * (const PTM::TMatrixBase<3,3, TD>& a, const TVec2<TV>& b){
	TVec2<TV> r;
	r[0] = a[0][0]*b[0] + a[0][1]*b[1] + a[0][2];
	r[1] = a[1][0]*b[0] + a[1][1]*b[1] + a[1][2];
	return r;
}


//-----------------------------------------------------------------------------
//	TAffine
/**	TAffine行列(回転,拡大,平行移動を表す)行列.
	概要は，\ref pgAffine 参照．	*/

template <class T>
class TAffine:public PTM::TMatrixBase<4,4,
	PTM::TMatrixDescCol< TAffine<T>, PTM::TMatrixRow<4,4,T>, 4,4,4,T> >{
public:
	typedef PTM::TMatrixDescCol< TAffine<T>, PTM::TMatrixRow<4,4,T>, 4,4,4,T> desc;
	typedef PTM::TMatrixBase<4,4,desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_MATRIX_BASIC_MEMBER	*/
	DEF_MATRIX_BASIC_MEMBER(TAffine);
	union{
		struct{
			T xx, xy, xz, xw;
			T yx, yy, yz, yw;
			T zx, zy, zz, zw;
			T px, py, pz, pw;
		};
		T data[4][4];
	};
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[j][i]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[j][i]; }

	/**@name	基底ベクトルへのアクセス	*/
	//@{
	/// 
	TVec3<element_type>& Ex() { return (TVec3<element_type>&)this->col(0); }
	/// 
	const TVec3<element_type>& Ex() const { return (TVec3<element_type>&)this->col(0); }
	/// 
	TVec3<element_type>& Ey() { return (TVec3<element_type>&)this->col(1); }
	/// 
	const TVec3<element_type>& Ey() const { return (TVec3<element_type>&)this->col(1); }
	/// 
	TVec3<element_type>& Ez() { return (TVec3<element_type>&)this->col(2); }
	/// 
	const TVec3<element_type>& Ez() const { return (TVec3<element_type>&)this->col(2); }
	/// 
	TVec3<element_type>& Trn() { return (TVec3<element_type>&)this->col(3); }
	/// 
	const TVec3<element_type>& Trn() const { return (TVec3<element_type>&)this->col(3); }
	///	平行移動成分(Trn()の別名)
	TVec3<element_type>& Pos() {return Trn();}
	///	平行移動成分(Trn()の別名,const 版)
	const TVec3<element_type>& Pos() const {return Trn();}
	//@}

	/**@name	要素へのアクセス	*/
	//@{
	/// 
	element_type& ExX() {return Ex().X();}
	const element_type& ExX() const {return Ex().X();}
	/// 
	element_type& ExY() {return Ex().Y();}
	const element_type& ExY() const {return Ex().Y();}
	/// 
	element_type& ExZ() {return Ex().Z();}
	const element_type& ExZ() const {return Ex().Z();}
	/// 
	element_type& EyX() {return Ey().X();}
	const element_type& EyX() const {return Ey().X();}
	/// 
	element_type& EyY() {return Ey().Y();}
	const element_type& EyY() const {return Ey().Y();}
	/// 
	element_type& EyZ() {return Ey().Z();}
	const element_type& EyZ() const {return Ey().Z();}
	/// 
	element_type& EzX() {return Ez().X();}
	const element_type& EzX() const {return Ez().X();}
	/// 
	element_type& EzY() {return Ez().Y();}
	const element_type& EzY() const {return Ez().Y();}
	/// 
	element_type& EzZ() {return Ez().Z();}
	const element_type& EzZ() const {return Ez().Z();}
	/// 
	element_type& TrnX() {return Trn().X();}
	const element_type& TrnX() const {return Trn().X();}
	/// 
	element_type& TrnY() {return Trn().Y();}
	const element_type& TrnY() const {return Trn().Y();}
	/// 
	element_type& TrnZ() {return Trn().Z();}
	const element_type& TrnZ() const {return Trn().Z();}
	///	TrnX()の別名
	element_type& PosX() {return TrnX();}
	const element_type& PosX() const {return TrnX();}
	///	TrnY()の別名
	element_type& PosY() {return TrnY();}
	const element_type& PosY() const {return TrnY();}
	///	TrnZ()の別名
	element_type& PosZ() {return TrnZ();}
	const element_type& PosZ() const {return TrnZ();}
	///
	element_type& ExW() {return this->item(3,0);}
	const element_type& ExW() const {return this->item(3,0);}
	///
	element_type& EyW() {return this->item(3,1);}
	const element_type& EyW() const {return this->item(3,1);}
	///
	element_type& EzW() {return this->item(3,2);}
	const element_type& EzW() const {return this->item(3,2);}
	///
	element_type& TrnW() {return this->item(3,3);}
	const element_type& TrnW() const {return this->item(3,3);}
	///
	element_type& PosW() {return this->item(3,3);}
	const element_type& PosW() const {return this->item(3,3);}
	//@}

	///	回転拡大変換部への参照を返す.
	PTM::TSubMatrixCol<3,3, desc>& Rot(){ return this->sub_matrix(PTM::TSubMatrixDim<0,0,3,3>()); }
	///	回転拡大変換部への参照を返す (const版).
	const PTM::TSubMatrixCol<3,3, desc>& Rot() const { return this->sub_matrix(PTM::TSubMatrixDim<0,0,3,3>()); }

	/**@name	初期化と構築	*/
	///	単位行列
	static TAffine<T> Unit(){
		TAffine<T> y;
		PTM::init_unitize(y);
		return y;
	}
	///	平行移動
	static TAffine<T> Trn(element_type px, element_type py, element_type pz){
		TAffine<T> y;
		y.Trn().X() = px;
		y.Trn().Y() = py;
		y.Trn().Z() = pz;
		return y;
	}
	///	x/y/z軸まわり回転
	static TAffine<T> Rot(element_type th, char axis)
	{
		TAffine<T> y;
#ifdef __BORLANDC__
        TMatrix3<T> r = y.Rot();
		PTM::init_rot(r, th, axis);
#else
		PTM::init_rot(y.Rot(), th, axis);
#endif
		return y;
	}
	/**	任意軸まわり回転
\verbatim
		+																	   +
		|u^2+(1-u^2)cos(th)      uv(1-cos(th))-wsin(th)  wu(1-cos(th))+vsin(th)|
	R =	|uv(1-cos(th))+wsin(th)  v^2+(1-v^2)cos(th)      vw(1-cos(th))-usin(th)|
		|wu(1-cos(th))-vsin(th)  vw(1-cos(th))+usin(th)  w^2+(1-w^2)cos(th)    |
		+																	   +
\endverbatim
*/
	template <class BUF>
	static TAffine<T> Rot(element_type th, const PTM::TVectorBase<3, BUF>& axis)
	{
		TAffine<T> y;
		Matrix3f r;
		PTM::init_rot(r, th, axis);
		y.Rot() = r;
		return y;
	}
	/// 拡大
	static TAffine<T> Scale(element_type sx, element_type sy, element_type sz){
		TAffine<T> y;
		y.item(0, 0) = sx; y.item(1, 1) = sy; y.item(2, 2) = sz;
		return y;
	}
	/**	OpenGLの射影行列として初期化
		@param screen	カメラから見たビューポートの中心の位置
		@param size		ビューポートのサイズ
		@param front	手前のクリッピング平面とカメラの距離
		@param back		奥のクリッピング平面とカメラの距離	*/
	template <class BUFS, class BUFZ>
	static TAffine<T> ProjectionGL(
		const PTM::TVectorBase<3, BUFS>& screen,
		const PTM::TVectorBase<2, BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f)
	{
		TAffine<T> y;
		PTM::init_projection_gl(y, screen, size, front, back);
		return y;
	}
	/**	Direct3Dの射影行列として初期化
		@param screen	カメラから見たビューポートの中心の位置
		@param size		ビューポートのサイズ
		@param front	手前のクリッピング平面とカメラの距離
		@param back		奥のクリッピング平面とカメラの距離	*/
	template <class BUFS, class BUFZ>
	static TAffine<T> ProjectionD3D(
		const PTM::TVectorBase<3, BUFS>& screen,
	    const PTM::TVectorBase<2, BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f)
	{
		TAffine<T> y;
		PTM::init_projection_d3d(y, screen, size, front, back);
		return y;
	}

	/** OpenGLの直交射影変換
		@param vpSize	ビューポートのサイズ
	 */
	template <class BUFS, class BUFZ>
	static TAffine<T> OrthoGL(
		const PTM::TVectorBase<3,BUFS>& screen,
		const PTM::TVectorBase<2,BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f)
	{
		TAffine<T> y;
		PTM::init_ortho_gl(y, screen, size, front, back);
		return y;
	}

	/* obsolete. デフォルト引数に変えました tazz
	template <class BUF>
	void LookAt(const PTM::TVectorBase<3, BUF>& targetPos)
	{
		PTM::init_look_at(*this, targetPos);
	}
	*/
	/**	注視行列として初期化
		@param targetPos 注視点の位置
		@param upDir ｙ向きベクトル
		Affine行列の位置はそのままに，Ez()がtargetPosへ向き，Ey()がupDirへ向くように傾きを設定する．
		位置はTrn() (Pos())で予め設定する．
		// NOTE: 今までの実装ではupDirが位置ベクトルとして実装されていましたが，OpenGLの仕様に合わせるため
		// 方向ベクトルとしました．既存のコードは影響を受ける可能性があります． tazz 09/05/29
	*/
	template <class BUF>
	void LookAt(const PTM::TVectorBase<3, BUF>& targetPos,
		const TVec3<typename BUF::element_type>& upDir = TVec3<typename BUF::element_type>(0,1,0))
	{
		PTM::init_look_at(*this, targetPos, upDir);
	}
	/* obsolete
	template <class BUF>
	void LookAtGL(const PTM::TVectorBase<3, BUF>& targetPos)
	{
		PTM::init_look_at_gl(*this, targetPos);
	}
	*/
	/** 注視行列として初期化
		@param targetPos 注視点の位置
		@param upDir ｙ向きベクトル
		Affine行列の位置はそのままに，-Ez()がtargetPosへ向き，Ey()がupDirへ向くように傾きを設定する．
		位置はTrn() (Pos())で予め設定する．
		// NOTE: 上と同様の注意．
	 */
	template <class BUF>
	void LookAtGL(const PTM::TVectorBase<3, BUF>& targetPos,
		const TVec3<typename BUF::element_type>& upDir = TVec3<typename BUF::element_type>(0,1,0))
	{
		PTM::init_look_at_gl(*this, targetPos, upDir);
	}
	/// 正規直交化
	void Orthonormalization(){
		this->Ex() /= this->Ex().norm();
		this->Ey() /= this->Ey().norm();
		this->Ez() /= this->Ez().norm();
		for (int i=0; i<10; ++i) {
			Vec3f u = PTM::cross(this->Ey(), this->Ez());
			Vec3f v = PTM::cross(this->Ez(), this->Ex());
			Vec3f w = PTM::cross(this->Ex(), this->Ey());
			this->Ex() = (this->Ex()+u)/2.0f;
			this->Ey() = (this->Ey()+v)/2.0f;
			this->Ez() = (this->Ez()+w)/2.0f;
			this->Ex() /= this->Ex().norm();
			this->Ey() /= this->Ey().norm();
			this->Ez() /= this->Ez().norm();
			float r = 0.0f;
			r += PTM::dot(this->Ex(), this->Ey())*PTM::dot(this->Ex(), this->Ey());
			r += PTM::dot(this->Ey(), this->Ez())*PTM::dot(this->Ey(), this->Ez());
			r += PTM::dot(this->Ez(), this->Ex())*PTM::dot(this->Ez(), this->Ex());
			if (r < 0.000001) {
				break;
			}
		}
	}
	
	///コンストラクタ
	void set_default(){PTM::init_unitize(*this);}
};

/**	TAffineのコンストラクタ定義のマクロ．コンストラクタは継承されないが，
	これをDefineすることで，コンストラクタを引き継ぐことができる．	*/
#define DEF_TAFFINE_CONSTRUCTORS(TAffine)												\
	TAffine(){*this=Unit();}															\
	TAffine(element_type px, element_type py, element_type pz){*this=Trn(px, py, pz);}	\
	template <class BUFX, class BUFY>													\
	TAffine(const PTM::TVectorBase<3, BUFX>& exi,								\
			const PTM::TVectorBase<3, BUFY>& eyi){								\
			PTM::init_direct(Rot(), exi, eyi, 'x');										\
			item(3, 0) = 0; item(3, 1) = 0; item(3, 2) = 0; item(3, 3) = 1;				\
			item(0, 3) = 0; item(1, 3) = 0; item(2, 3) = 0;								\
	}																					\
	template <class BUFX, class BUFY, class BUFP>										\
	TAffine(	const PTM::TVectorBase<3, BUFX>& exi,							\
			const PTM::TVectorBase<3, BUFY>& eyi,								\
			const PTM::TVectorBase<3, BUFP>& posi){								\
			PTM::init_direct(Rot(), exi, eyi, 'x');										\
			item(3, 0) = 0; item(3, 1) = 0; item(3, 2) = 0; item(3, 3) = 1;				\
			item(0, 3) = posi.X(); item(1, 3) = posi.Y(); item(2, 3) = posi.Z();		\
	}                                                                                   \
	template <class BUFA, class BUFB>													\
	TAffine(	const PTM::TVectorBase<3, BUFA>& a,								\
			const PTM::TVectorBase<3, BUFB>& b,									\
			char axis){																	\
			PTM::init_direct(Rot(), exi, eyi, axis);									\
			item(3, 0) = 0; item(3, 1) = 0; item(3, 2) = 0; item(3, 3) = 1;				\
			item(0, 3) = 0; item(1, 3) = 0; item(2, 3) = 0;								\
	}																					\
	template <class BUFA, class BUFB, class BUFP>										\
	TAffine(	const PTM::TVectorBase<3, BUFA>& a,							    \
			const PTM::TVectorBase<3, BUFB>& b,									\
			char axis, const PTM::TVectorBase<3, BUFP>& posi){					\
			PTM::init_direct(Rot(), exi, eyi, axis);									\
			item(3, 0) = 0; item(3, 1) = 0; item(3, 2) = 0; item(3, 3) = 1;				\
			item(0, 3) = posi.X(); item(1, 3) = posi.Y(); item(2, 3) = posi.Z();		\
			}

/**	TAffine2のコンストラクタ定義のマクロ．コンストラクタは継承されないが，
	これをDefineすることで，コンストラクタを引き継ぐことができる．	*/
#define DEF_TAFFINE_CONSTRUCTORS2(TAffine)												\
	TAffine(element_type th, char axis) {												\
		*this=Rot(th, axis);															\
	}																					\
	template <class BUF>																\
	TAffine(element_type th, char axis,													\
		const PTM::TVectorBase<3, BUF>& posi) {									\
		*this = Rot(th, axis); Pos() = posi; }											\
	template <class BUFA>																\
	TAffine(element_type th, const PTM::TVectorBase<3, BUFA>& axis){			\
		*this = Rot(th, axis.unit());			                                        \
	}                                                                                   \
	template <class BUFA, class BUFP>													\
	TAffine(element_type th, const PTM::TVectorBase<3, BUFA>& axis , const PTM::TVectorBase<3, BUFP>& posi){	\
		*this = Rot(th, axis.unit()); Pos() = posi;										\
	}																					\
	template <class BUFS, class BUFZ>													\
	TAffine(const PTM::TVectorBase<3, BUFS>& screen, const PTM::TVectorBase<2, BUFZ>& size, element_type front=1.0f, element_type back=10000.0f){	\
		*this = ProjectionGL(screen, size, front, back);								\
	}                                                                                   \
	template <class BUF, class BUFV>													\
	TAffine(const PTM::TMatrixOp<3, 3, BUF>& m, const PTM::TVectorBase<3, BUFV> posi){\
		Rot() = m; Pos() = posi; ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;			\
	}


#ifdef _WIN32
 #pragma warning (disable: 4700)
#endif

	
	
	///	TAffineとベクトルの掛け算
template <class TD, class TV>
TVec3<TV> operator * (const PTM::TMatrixBase<4,4, TD>& a, const TVec3<TV>& b){
	TVec3<TV> r;
	r[0] = a[0][0]*b[0] + a[0][1]*b[1] + a[0][2]*b[2] + a[0][3];
	r[1] = a[1][0]*b[0] + a[1][1]*b[1] + a[1][2]*b[2] + a[1][3];
	r[2] = a[2][0]*b[0] + a[2][1]*b[1] + a[2][2]*b[2] + a[2][3];
	return r;
}
#ifdef _WIN32
 #pragma warning (default: 4700)
#endif

///	float版2次元アフィン行列.
typedef TAffine2<float> Affine2f;
///	double版2次元アフィン行列.
typedef TAffine2<double> Affine2d;
///	float版3次元アフィン行列.
typedef TAffine<float> Affinef;
///	double版3次元アフィン行列.
typedef TAffine<double> Affined;
//@}
//@}


#ifndef PTM_PACK	//	単体で使用する場合は，namespace に入れない
}	//	namespace Spr
#endif

#endif

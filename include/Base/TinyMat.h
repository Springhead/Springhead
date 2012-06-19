/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef TINYMAT_H
#define TINYMAT_H

#include <Base/TMatrixUtility.h>
#include <Base/TMatrix.h>
#include <Base/BaseDebug.h>

#ifndef PTM_PACK	//	単体で使用する場合は，namespace に入れない
namespace Spr{;
#endif

/**	\addtogroup gpLinearAlgebra	*/
//@{
/**	@file TinyMat.h
	2x2, 3x3行列の定義
 */

//-----------------------------------------------------------------------------
//		TMatrix2
/**	2x2行列クラス.	*/
template <class T>
class TMatrix2:public PTM::TMatrixBase<2,2,
	PTM::TMatrixDescCol< TMatrix2<T>, PTM::TMatrixRow<2,2,T>, 2,2,2,T> >{
public:
	typedef PTM::TMatrixDescCol< TMatrix2<T>, PTM::TMatrixRow<2,2,T>, 2,2,2,T> desc;
	typedef PTM::TMatrixBase<2,2,desc> base_type;
	///	基本的なメンバの定義 @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TMatrix2);
	union{
		element_type data[2][2];
		struct {
			element_type xx, xy, yx, yy;
		};
	};
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[j][i]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[j][i]; }

	/**@name	基底ベクトルへのアクセス	*/
	//@{
	/// 
	TVec2<element_type>& Ex(){ return *(TVec2<element_type>*)&this->col(0); }
	/// 
	const TVec2<element_type>& Ex() const{ return *(TVec2<element_type>*)&this->col(0); }
	/// 
	/// 
	TVec2<element_type>& Ey(){ return *(TVec2<element_type>*)&this->col(1); }
	/// 
	const TVec2<element_type>& Ey() const{ return *(TVec2<element_type>*)&this->col(1); }
	//@}
	
	///@name 初期化と構築
	//@{
	///	コンストラクタ
	void set_default(){*this = Unit();}
	///	コンストラクタ(直接指定)
	TMatrix2(const TVec2<T>& exi, const TVec2<T>& eyi){
		Ex().X() = exi.X();
		Ex().Y() = exi.Y();
		Ey().X() = eyi.X();
		Ey().Y() = eyi.Y();
	}
	TMatrix2(T m11, T m12, T m21, T m22){
		Ex().X() = m11;
		Ex().Y() = m21;
		Ey().X() = m12;
		Ey().Y() = m22;
	}
	///零行列
	static TMatrix2<T> Zero(){return TMatrix2<T>(0, 0, 0, 0);}
	///単位行列
	static TMatrix2<T> Unit(){return TMatrix2<T>(1, 0, 0, 1);}
	///対角行列
	static TMatrix2<T> Diag(element_type x, element_type y){return TMatrix2<T>(x, 0, 0, y);}
	///回転行列
	///回転角を指定
	static TMatrix2<T> Rot(element_type rad){
        TMatrix2<T> m;
		PTM::init_rot(m, rad);
		return m;
	}
	///x/y軸の向きを指定
	template<typename BUFA>
	static TMatrix2<T> Rot(const PTM::TVectorBase<2, BUFA>& a, char axis){
		TMatrix2<T> m;
		PTM::init_rot(m, a, axis);
		return m;
	}
	//@}

	/**@name 要素へのアクセス */
	//@{
	/// 
	const element_type& ExX() const {return this->EX().X();}
	element_type& ExX() {return this->EX().X();}
	/// 
	const element_type& ExY() const {return this->Ex().Y();}
	element_type& ExY() {return this->Ex().Y();}
	/// 
	const element_type& EyX() const {return this->Ey().X();}
	element_type& EyX() {return this->Ey().X();}
	/// 
	const element_type& EyY() const {return this->Ey().Y();}
	element_type& EyY() {return this->Ey().Y();}
	//@}

	///@name 演算
	//@{
	/// 回転行列の回転角を返す
	element_type angle() const{
		return atan2(this->Ex().Y(), this->Ex().X());
	}
	//@}
};

//-----------------------------------------------------------------------------
//	TMatrix3

///	3x3行列クラス.
template <class T>
class TMatrix3:public PTM::TMatrixBase<3,3,
	PTM::TMatrixDescCol< TMatrix3<T>, PTM::TMatrixRow<3,3,T>, 3,3,3,T> >{
public:
	typedef PTM::TMatrixDescCol< TMatrix3<T>, PTM::TMatrixRow<3,3,T>, 3,3,3,T> desc;
	typedef PTM::TMatrixBase<3,3,desc> base_type;
	///	基本的なメンバの定義 @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TMatrix3);
	union{
		element_type data[3][3];
		struct {
			element_type xx, xy, xz;
			element_type yx, yy, yz;
			element_type zx, zy, zz;
		};
	};
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[j][i]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[j][i]; }

	/**@name	基底ベクトルへのアクセス	*/
	//@{
	/// 
	TVec3<element_type>& Ex(){ return *(TVec3<element_type>*) &this->item(0,0); }
	///
	const TVec3<element_type>& Ex() const { return *(TVec3<element_type>*) &this->item(0,0); }
	/// 
	TVec3<element_type>& Ey(){ return *(TVec3<element_type>*) &this->item(0,1); }
	///
	const TVec3<element_type>& Ey() const { return *(TVec3<element_type>*) &this->item(0,1); }
	/// 
	TVec3<element_type>& Ez(){ return *(TVec3<element_type>*) &this->item(0,2); }
	///
	const TVec3<element_type>& Ez() const { return *(TVec3<element_type>*) &this->item(0,2); }
	//@}
	/**@name 要素へのアクセス */
	//@{
	/// 
	const element_type& ExX() const {return Ex().X();}
	element_type& ExX() {return Ex().X();}
	/// 
	const element_type& ExY() const {return Ex().Y();}
	element_type& ExY() {return Ex().Y();}
	/// 
	const element_type& ExZ() const {return Ex().Z();}
	element_type& ExZ() {return Ex().Z();}
	/// 
	const element_type& EyX() const {return Ey().X();}
	element_type& EyX() {return Ey().X();}
	/// 
	const element_type& EyY() const {return Ey().Y();}
	element_type& EyY() {return Ey().Y();}
	/// 
	const element_type& EyZ() const {return Ey().Z();}
	element_type& EyZ() {return Ey().Z();}
	/// 
	const element_type& EzX() const {return Ez().X();}
	element_type& EzX() {return Ez().X();}
	/// 
	const element_type& EzY() const {return Ez().Y();}
	element_type& EzY() {return Ez().Y();}
	/// 
	const element_type& EzZ() const {return Ez().Z();}
	element_type& EzZ() {return Ez().Z();}
	//@}
	
	//---------------------------------
	/**@name	初期化と構築	*/
	//@{
	///デフォルトコンストラクタ
	void set_default(){*this = Unit();}
	///コンストラクタ（基底ベクトルを指定）
	template <class BX, class BY, class BZ>
	TMatrix3(const PTM::TVectorBase<3, BX>& exi, const PTM::TVectorBase<3, BY>&eyi, const PTM::TVectorBase<3, BZ>&ezi){
		Ex() = exi; Ey() = eyi; Ez() = ezi;
	}
	///コンストラクタ（成分を指定）
	TMatrix3(T m11, T m12, T m13, T m21, T m22, T m23, T m31, T m32, T m33)
	{
		Ex().X() = m11; Ey().X() = m12; Ez().X() = m13;
		Ex().Y() = m21; Ey().Y() = m22; Ez().Y() = m23;
		Ex().Z() = m31; Ey().Z() = m32; Ez().Z() = m33;
	}

	///零行列
	static TMatrix3<T> Zero(){TMatrix3<T> m; m.clear(); return m;}
	///単位行列
	static TMatrix3<T> Unit(){return TMatrix3<T>(1, 0, 0, 0, 1, 0, 0, 0, 1);}
	///対角行列
	static TMatrix3<T> Diag(element_type x, element_type y, element_type z){
		return TMatrix3<T>(x, 0, 0, 0, y, 0, 0, 0, z);
	}
	///回転行列
	///方向ベクトルで指定
	template <class BA, class BB>
	static TMatrix3<T> Rot(const PTM::TVectorBase<3,BA>& a, const PTM::TVectorBase<3,BB>&b, char axis = 'x') {
		TMatrix3<T> m;
		PTM::init_rot(m, a, b, axis);
		return m;
	}
	///回転軸で指定
	static TMatrix3<T> Rot(element_type th, char axis){
		TMatrix3<T> m;
		PTM::init_rot(m, th, axis);
		return m;
	}
	/**	任意軸まわり回転
		+																	   +
		|u^2+(1-u^2)cos(th)      uv(1-cos(th))-wsin(th)  wu(1-cos(th))+vsin(th)|
	R =	|uv(1-cos(th))+wsin(th)  v^2+(1-v^2)cos(th)      vw(1-cos(th))-usin(th)|
		|wu(1-cos(th))-vsin(th)  vw(1-cos(th))+usin(th)  w^2+(1-w^2)cos(th)    |
		+																	   +
	*/
	template <class B>
	static TMatrix3<T> Rot(element_type th, const PTM::TVectorBase<3, B>& axis){
		TMatrix3<T> m;
		PTM::init_rot(m, th, axis.unit());
		return m;
	}
	///	Qutaternionで指定
	template <class B>
	static TMatrix3<T> Rot(const PTM::TVectorBase<4, B>& q){
		TMatrix3<T> m;
		PTM::init_rot(m, q);
		return m;
	}
	
	///外積と等価な行列
	template <class B>
	static TMatrix3<T> Cross(const PTM::TVectorBase<3, B>& v){
		TMatrix3<T> m;
		PTM::init_cross(m, v);
		return m;
	}
	
	//@}
};

///	float版2×2行列．
typedef TMatrix2<float> Matrix2f;
///	double版2×2行列．
typedef TMatrix2<double> Matrix2d;
///	float版3×3行列．
typedef TMatrix3<float> Matrix3f;
///	double版3×3行列．
typedef TMatrix3<double> Matrix3d;


template <class T>
bool IsUnitary(TMatrix3<T> r){
	bool bGood = true;
#ifdef _DEBUG
	if(abs(r.Ex().norm()-1) > 1e-1) bGood = false;
	if(abs(r.Ey().norm()-1) > 1e-1) bGood = false;
	if(abs(r.Ez().norm()-1) > 1e-1) bGood = false;
	if(abs(r.Ex() * r.Ey()) > 1e-1) bGood = false;
	if(abs(r.Ey() * r.Ez()) > 1e-1) bGood = false;
	if(abs(r.Ez() * r.Ex()) > 1e-1) bGood = false;
	if (!bGood){
		DSTR << "PHSolid::SetRotation: " << r;
		DSTR << "x:" << r.Ex().norm() << " " << r.Ex()*r.Ey() << std::endl;
		DSTR << "y:" << r.Ey().norm() << " " << r.Ey()*r.Ez() << std::endl;
		DSTR << "z:" << r.Ez().norm() << " " << r.Ez()*r.Ex() << std::endl;
	}
#endif
	return bGood;
}
//@}

#ifndef PTM_PACK	//	単体で使用する場合は，namespace に入れない
}
#endif

#endif

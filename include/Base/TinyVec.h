/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef TINYVEC_H
#define TINYVEC_H

#include <Base/TVector.h>

#ifndef PTM_PACK	//	単体で使用する場合は，namespace に入れない
namespace Spr{;
#endif

/**	\addtogroup gpLinearAlgebra	*/
//@{
/**	@file TinyVec.h
	2,3,4次元のベクトルの定義
 */

//-----------------------------------------------------------------------------
//		TVec2
template <class EXP, class T> class TVecDesc: public PTM::TVectorDesc<1, EXP, EXP, T>{};
/**	2次元ベクトルクラス	*/
template<class T>
class TVec2:public PTM::TVectorBase<2, TVecDesc<TVec2<T>, T> >{
public:
	typedef TVecDesc<TVec2<T>, T> desc;
	typedef PTM::TVectorBase<2, desc> base_type;
	///	基本的なメンバの定義 @see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TVec2);

	union{
		T data[2];
		struct{
			T x,y;
		};
		struct{
			T lower,upper;
		};
	};
	///	要素のアクセス
	T& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }
	///@name 構築・初期化
	//@{
	///	コンストラクタ
	template <class TX, class TY>
		TVec2(TX xi, TY yi){ X() = element_type(xi); Y() = element_type(yi); }
	//@}

	///@name 初期化関数
	//@{
	/// 零ベクトル
	static TVec2<T> Zero(){TVec2<T> v; v.clear(); return v;}
	//@}

	///@name 変数アクセス
	//@{
	///	ベクトルのx成分
	const element_type& X() const { return this->item(0); }
	///	ベクトルのy成分
	const element_type& Y() const { return this->item(1); }
	///	ベクトルのx成分
	element_type& X() { return this->item(0); }
	///	ベクトルのy成分
	element_type& Y() { return this->item(1); }
	///	ベクトルのx成分
	const element_type& U() const { return this->item(0); }
	///	ベクトルのy成分
	const element_type& V() const { return this->item(1); }
	///	ベクトルのx成分
	element_type& U() { return this->item(0); }
	///	ベクトルのy成分
	element_type& V() { return this->item(1); }
	///	範囲としてみた場合の開始値（ベクトルのx成分）
	element_type& Lower() { return this->item(0); }
	///	範囲としてみた場合の終了値（ベクトルのy成分）
	element_type& Upper() { return this->item(1); }
	//@}
protected:
	void set_default(){this->clear();}
};

//-----------------------------------------------------------------------------
//		TVec3
///	3次元ベクトルクラス.
template<class T>
class TVec3:public PTM::TVectorBase<3, TVecDesc<TVec3<T>, T> >{
public:
	typedef TVecDesc<TVec3<T>, T> desc;
	typedef PTM::TVectorBase<3, desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec3);
	union{
		T data[3];
		struct{
			T x,y,z;
		};
	};
	///	要素のアクセス
	T& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }

	///@name 初期化関数
	//@{
	/// 零ベクトル
	static TVec3<T> Zero(){TVec3<T> v; v.clear(); return v;}
	//@}
	
	///@name 変数アクセス
	//@{
	///	x成分
	const element_type& X() const { return this->item(0); }
	///	y成分
	const element_type& Y() const { return this->item(1); }
	///	z成分
	const element_type& Z() const { return this->item(2); }
	///	x成分
	element_type& X(){ return this->item(0); }
	///	y成分
	element_type& Y(){ return this->item(1); }
	///	z成分
	element_type& Z(){ return this->item(2); }

	const element_type& R() const { return this->item(0); }
	const element_type& G() const { return this->item(1); }
	const element_type& B() const { return this->item(2); }
	element_type& R(){ return this->item(0); }
	element_type& G(){ return this->item(1); }
	element_type& B(){ return this->item(2); }
	//@}
	
	///@name 初期化・構築
	//@{
	///	コンストラクタ
	template <class TX, class TY, class TZ> TVec3 (TX xi, TY yi, TZ zi){ X() = element_type(xi); Y() = element_type(yi); Z() = element_type(zi); }
	//@}
	/**	内積(return *this * b).
		@param	b	同じサイズのベクトル.
		@return		内積の値(要素型)	*/
	template <class D>
	element_type dot(const PTM::VectorImp<D>& b) const {
		assert(b.size() == this->size());
		return this->item(0)*b[0] + this->item(1)*b[1] + this->item(2)*b[2];
	}
protected:
	void set_default(){this->clear();}
};

//-----------------------------------------------------------------------------
//		TVec4
///	4次元ベクトルクラス.
template<class T>
class TVec4:public PTM::TVectorBase<4, TVecDesc<TVec4<T>, T> >{
public:
	typedef TVecDesc<TVec4<T>, T> desc;
	typedef PTM::TVectorBase<4, desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec4);
	union{
		T data[4];
		struct{
			T x,y,z,w;
		};
	};
	///	要素のアクセス
	T& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }

	///@name 初期化関数
	//@{
	/// 零ベクトル
	static TVec4<T> Zero(){TVec4<T> v; v.clear(); return v;}
	//@}

	///@name 変数アクセス
	//@{
	///	x成分
	const element_type& X() const { return x; }
	///	y成分
	const element_type& Y() const { return y; }
	///	z成分
	const element_type& Z() const { return z; }
	///	w成分
	const element_type& W() const { return w; }
	///	x成分
	element_type& X(){ return x; }
	///	y成分
	element_type& Y(){ return y; }
	///	z成分
	element_type& Z(){ return z; }
	///	w成分
	element_type& W(){ return w; }

	const element_type& R() const { return x; }
	const element_type& G() const { return y; }
	const element_type& B() const { return z; }
	const element_type& A() const { return w; }
	element_type& R(){ return x; }
	element_type& G(){ return y; }
	element_type& B(){ return z; }
	element_type& A(){ return w; }
	//@}
	
	///@name 初期化・構築
	//@{
	///	コンストラクタ
	template <class TX, class TY, class TZ, class TW>
	TVec4 (TX xi, TY yi, TZ zi, TW wi){ X() = element_type(xi); Y() = element_type(yi); Z() = element_type(zi); W() = element_type(wi);}
	//@}
protected:
	void set_default(){this->clear();}
};

//-----------------------------------------------------------------------------
//		TVec6
///	6次元ベクトルクラス.
template<class T>
class TVec6:public PTM::TVectorBase<6, TVecDesc<TVec6<T>, T> >{
public:
	typedef TVecDesc<TVec6<T>, T> desc;
	typedef PTM::TVectorBase<6, desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec6);
	union{
		T data[6];
		struct{
			T vx,vy,vz,wx,wy,wz;
		};
		struct{
			TVec3<T> r, v;
		};
	};
	///	要素のアクセス
	T& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }

	///@name 初期化関数
	//@{
	/// 零ベクトル
	static TVec6<T> Zero(){TVec6<T> v; v.clear(); return v;}
	//@}

	///@name 変数アクセス
	//@{
	///	vx成分
	const element_type& VX() const { return vx; }
	///	vy成分
	const element_type& VY() const { return vy; }
	///	vz成分
	const element_type& VZ() const { return vz; }
	///	wx成分
	const element_type& WX() const { return wx; }
	///	wy成分
	const element_type& WY() const { return wy; }
	///	wz成分
	const element_type& WZ() const { return wz; }
	///	vx成分
	element_type& VX(){ return vx; }
	///	vy成分
	element_type& VY(){ return vy; }
	///	vz成分
	element_type& VZ(){ return vz; }
	///	wx成分
	element_type& WX(){ return wx; }
	///	wy成分
	element_type& WY(){ return wy; }
	///	wz成分
	element_type& WZ(){ return wz; }
	//@}
	
	///@name 初期化・構築
	//@{
	///	コンストラクタ
	template <class TVX, class TVY, class TVZ, class TWX, class TWY, class TWZ>
	TVec6 (TVX vxi, TVY vyi, TVZ vzi, TWX wxi, TWY wyi, TWZ wzi){
		VX() = element_type(vxi); VY() = element_type(vyi); VZ() = element_type(vzi);
		WX() = element_type(wxi); WY() = element_type(wyi); WZ() = element_type(wzi);
	}
	//@}
protected:
	void set_default(){this->clear();}
};


/// int版2次元ベクトル
typedef TVec2<int> Vec2i;
///	float版2次元ベクトル
typedef TVec2<float> Vec2f;
///	double版2次元ベクトル
typedef TVec2<double> Vec2d;
/// int版3次元ベクトル
typedef TVec3<int>	Vec3i;
///	float版3次元ベクトル
typedef TVec3<float> Vec3f;
///	double版3次元ベクトル
typedef TVec3<double> Vec3d;
/// int版4次元ベクトル
typedef TVec4<int> Vec4i;
///	float版4次元ベクトル
typedef TVec4<float> Vec4f;
///	double版4次元ベクトル
typedef TVec4<double> Vec4d;
/// int版6次元ベクトル
typedef TVec6<int> Vec6i;
/// float版6次元ベクトル
typedef TVec6<float> Vec6f;
/// double版6次元ベクトル
typedef TVec6<double> Vec6d;
//@}

#ifndef PTM_PACK	//	単体で使用する場合は，namespace に入れない
}
#endif

#endif

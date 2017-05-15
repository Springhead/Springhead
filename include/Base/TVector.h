/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PTMATRIX_TVECTOR_H
#define PTMATRIX_TVECTOR_H

#include <algorithm>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <stddef.h>
#include <vector>

/** \addtogroup gpLinearAlgebra */
//@{

/**	@file
	C++ の template 機能を生かしたベクトルライブラリ．
	メモリと管理情報(ベクトルのサイズなど)の持ち方で3種ある。
	- TVector : 管理情報をテンプレートの引数にすることで，
			余分なメモリを一切使わずに一般のベクトルを扱うことができるベクトルの型．
			配列をキャストしてベクトルにすることもできる．
	- VVector : 管理情報をメモリに持ち，サイズの変更ができるベクトルの型．
	- EVector : 管理情報だけを持ち，データは外部の配列などを使うベクトルの型．
	互いに内積などの演算を行うことができる．また，TMatrix, VMatrix, EMatrix 型の行列
	との演算もできる．	*/

//-----------------------------------------------------------------------------
#ifndef TYPENAME	//<	互換性確保のためのマクロ．typename をつけると動かないコンパイラもある．
# define TYPENAME typename
#endif

#ifdef _MSC_VER
	#if _MSC_VER <= 1310
		#define for if(0); else for	// openMPで#pragma omp parallel forに影響を与える 2013.3.1susa
	#endif
 #pragma warning (disable: 4786)
 #pragma warning (disable: 4200)
#endif

#ifdef __BORLANDC__
 #pragma warn -8027
#endif


namespace PTM {;

//-----------------------------------------------------------------------------

template <class EXP, class RET, class E, class Z=E>
class VectorDesc{
public:
	typedef EXP		exp_type;			///<	実体
	typedef RET		ret_type;			///<	返り値の型
	typedef E		element_type;		///<	要素の型
	typedef Z		zero;				///<	zero(0) が 要素の型の0を返す型
};
template <size_t STR, class EXP, class RET, class E, class Z=E>
class TVectorDesc:public VectorDesc<EXP, RET, E, Z>{
public:
	static const size_t STRIDE = STR;	///<	stride
};

///@name 演算.
//@{
template <class D> class VectorImp;
template <size_t N, class D> class TVectorBase;
/**	ベクトルの要素を全てvにする.
	@param v 要素型の値.	*/
template <class D>
void clear(VectorImp<D>& a, const TYPENAME D::element_type v= D::zero(0)){
	for(size_t i=0; i<a.size(); ++i) a.item(i) = v;
}
/**	代入(a = b).
	@param b 同じサイズのベクトル.	*/
template <class AD, class BD>
void assign(VectorImp<AD>& a, const VectorImp<BD>& b) {
	a.resize(b.size());
	assert(a.size() == b.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) = b.item(i);
}
///	代入(a = b) 2要素専用
template <class AD, class BD>
void assign(TVectorBase<2, AD>& a, const TVectorBase<2, BD>& b) {
	assert(a.size() == b.size());
	a.item(0) = (TYPENAME AD::element_type)b.item(0);
	a.item(1) = (TYPENAME AD::element_type)b.item(1);
}
///	代入(a = b) 3要素専用
template <class AD, class BD>
void assign(TVectorBase<3, AD>& a, const TVectorBase<3, BD>& b) {
	assert(a.size() == b.size());
	a.item(0) = (TYPENAME AD::element_type)b.item(0);
	a.item(1) = (TYPENAME AD::element_type)b.item(1);
	a.item(2) = (TYPENAME AD::element_type)b.item(2);
}
///	代入(a = b) 4要素専用
template <class AD, class BD>
void assign(TVectorBase<4, AD>& a, const TVectorBase<4, BD>& b) {
	assert(a.size() == b.size());
	a.item(0) = (TYPENAME AD::element_type)b.item(0);
	a.item(1) = (TYPENAME AD::element_type)b.item(1);
	a.item(2) = (TYPENAME AD::element_type)b.item(2);
	a.item(3) = (TYPENAME AD::element_type)b.item(3);
}
/**	代入(a = b).
	@param a ベクトル.
	@param b 同じサイズのベクトル.
	*/
template <class AD>
void assign(VectorImp<AD>& a, const TYPENAME AD::element_type* b) {
	for(size_t i=0; i<a.size(); ++i) a.item(i) = b[i];
}

/**	加算(a += b).
	@param a ベクトル.
	@param b 同じサイズのベクトル.	*/
template <class AD, class BD>
void add(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() ==a. size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) += (TYPENAME AD::element_type) b.item(i);
}
/**	減算(a -= b).
	@param a ベクトル.
	@param b 同じサイズのベクトル.	*/
template <class AD, class BD>
void sub(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) -= (TYPENAME AD::element_type) b.item(i);
}
/**	定数倍(*this *= b).
	@param b 要素型.				*/
template <class AD>
void multi(VectorImp<AD>& a, const TYPENAME AD::element_type& b){
	for(size_t i=0; i<a.size(); ++i) a.item(i) *= b;
}
/**	定数倍(*this *= b).
	@param b 要素型.				*/
template <class AD, class BD>
void multi_each(VectorImp<AD>& a, const VectorImp<BD>& b){
	for(size_t i=0; i<a.size(); ++i) a.item(i) *= b.item(i);
}
/**	定数分の１(*this /= b).
	@param b 要素型.				*/
template <class AD>
void div(VectorImp<AD>& a, const TYPENAME AD::element_type& b){
	for(size_t i=0; i<a.size(); ++i) a.item(i) /= b;
}
/**	内積(return *this * b).
	@param	b	同じサイズのベクトル.
	@return		内積の値(要素型)	*/
template <class AD, class BD>
inline TYPENAME AD::element_type dot(const VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	typedef TYPENAME AD::zero zero;
	TYPENAME AD::element_type rv = zero(0);
	for(size_t i=0; i<a.size(); ++i) rv += a.item(i)*b.item(i);
	return rv;
}
template <class AD, class BD>
inline TYPENAME AD::element_type dot(const TVectorBase<3, AD>& a, const TVectorBase<3, BD>& b){
        typedef TYPENAME AD::element_type ET;
	return ET(a.item(0)*b.item(0) + a.item(1)*b.item(1) + a.item(2)*b.item(2));
}
/**	比較(return *this == b).
	@param	b	同じサイズのベクトル.
	@return		bool値.	*/
template <class AD, class BD>
bool equal(const VectorImp<AD>& a, const VectorImp<BD>& b){
	if (b.size() != a.size()) return false;
	for(size_t i=0; i<a.size(); ++i){
		if (a.item(i) != b.item(i)) return false;
	}
	return true;
}

/**	要素ごとの最小値(min(*this, b)).
	@param b 同じサイズのベクトル.	*/
template <class AD, class BD>
void element_min_impl(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) = std::min(a.item(i), b.item(i));
}
template <class AD, class BD>
TYPENAME AD::ret_type element_min(const VectorImp<AD>& a, const VectorImp<BD>& b){
	TYPENAME AD::ret_type r(a);
	element_min_impl(r, b);
	return r;
}
/**	要素ごとの最大値(max(*this, b)).
	@param b 同じサイズのベクトル.	*/
template <class AD, class BD>
void element_max_impl(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) = std::max(a.item(i), b.item(i));
}
template <class AD, class BD>
TYPENAME AD::ret_type element_max(const VectorImp<AD>& a, const VectorImp<BD>& b){
	TYPENAME AD::ret_type r(a);
	element_max_impl(r, b);
	return r;
}
///	クリッピングする(bとeを頂点とする矩形内に収まるようにする).
template <class AD, class BD, class ED>
void clip(const VectorImp<AD>& a, const VectorImp<BD>& b, const VectorImp<ED>& e){
	for(size_t i=0; i<a.size(); ++i){
		if (a.item(i) < b.item(i)) a.item(i) = b.item(i);
		if (a.item(i) > e.item(i)) a.item(i) = e.item(i);
	}
}
//@}


template<size_t SZ, size_t STR, class OD> class TVectorSlice;
template<size_t SZ, class OD> class TSubVector;
template<class D> class EVectorSlice;

template<class D> class ConstEVectorSlice;

///	ベクトルの演算などの実装
template <class D>
class VectorImp{
public:
	typedef D desc;
	typedef TYPENAME desc::exp_type exp_type;
	typedef TYPENAME desc::ret_type ret_type;
	typedef TYPENAME desc::element_type element_type;
	typedef TYPENAME desc::zero zero;
	///	コンストラクタ
	VectorImp(){}

	///@name 基本操作.
	//@{
	///	実体の取得
	inline exp_type& exp(){ return *(exp_type*)this; }
	inline const exp_type& exp() const { return *(const exp_type*)this; }
	///	n番目の要素を返す(基数は0).
	inline const element_type& item(size_t n) const { return exp().item_impl(n); }
	inline element_type& item(size_t n) { return exp().item_impl(n); }
	///	サイズの取得
	inline size_t size() const { return exp().size_impl(); }
	///	サイズの設定
	inline void resize(size_t sz){ exp().resize_impl(sz); }
	/// サイズの設定とベクトルの要素を全てvにする
	inline void resize(size_t sz, const element_type v){
		resize(sz);
		clear(v);
	}
	///	ストライドの取得
	inline size_t stride() const { return exp().stride_impl(); }
	//@}
	
	///@name 部分ベクトル
	//@{
	/**	スライスを返す．テンプレート版
		このベクトルのSZ::OFFSET要素から SZ::OFFSET + SZ::SIZE要素までの
		部分ベクトルへの参照を返す．
		@param	SZ::OFFSET	部分ベクトルの最初の要素の位置.
		@param	SZ::SIZE	部分ベクトルのサイズ.
		@return	SZ で指定されたベクトルへの参照を返す.	*/
	template <class SZ> TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&
	t_slice(SZ){ return (TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&)item(SZ::OFFSET); }
	template <class SZ>	const TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&
	t_slice(SZ) const { return (TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&)item(SZ::OFFSET); }
	///	部分ベクトルを返す．テンプレート版
	template <class SZ> TSubVector<SZ::SIZE, desc>&
	sub_vector(SZ){ return (TSubVector<SZ::SIZE, desc>&)item(SZ::OFFSET); }
	template <class SZ>	const TSubVector<SZ::SIZE, desc>&
	sub_vector(SZ) const { return (TSubVector<SZ::SIZE, desc>&)item(SZ::OFFSET); }
	/**	スライスを返す．サイズだけテンプレート版
		このベクトルの off 要素から off + SZ::SIZE要素までの
		部分ベクトルへの参照を返す．
		@param	SZ::OFFSET	部分ベクトルの最初の要素の位置.
		@param	SZ::SIZE	部分ベクトルのサイズ.
		@return	SZ で指定されたベクトルへの参照を返す.	*/
	template <class SZ> TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&
	t_slice(size_t off, SZ){ return (TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&)item(off); }
	template <class SZ>	const TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&
	t_slice(size_t off, SZ) const { return (TVectorSlice<SZ::SIZE, SZ::STRIDE, desc>&)item(off); }
	///	部分ベクトルを返す．サイズだけテンプレート版
	template <class SZ> TSubVector<SZ::SIZE, desc>&
	sub_vector(size_t off, SZ){ return (TSubVector<SZ::SIZE, desc>&)item(off); }
	template <class SZ>	const TSubVector<SZ::SIZE, desc>&
	sub_vector(size_t off, SZ) const { return (TSubVector<SZ::SIZE, desc>&)item(off); }
	
	///	部分ベクトルを返す．変数版
	EVectorSlice<element_type> v_range(size_t off, size_t sz){
		return EVectorSlice<element_type>(sz, stride(), &item(off));
	}
	ConstEVectorSlice<element_type> v_range(size_t off, size_t sz) const {
		return ConstEVectorSlice<element_type>(sz, stride(), &item(off));
	}
	///	ベクトルのスライスを返す．
	EVectorSlice<element_type> v_slice(size_t off, size_t sz, size_t str){
		return EVectorSlice<element_type>(sz, stride()*str, &item(off));
	}
	ConstEVectorSlice<element_type> v_slice(size_t off, size_t sz, size_t str) const {
		return ConstEVectorSlice<element_type>(sz, stride()*str, &item(off));
	}
	//@}
	
	///@name 演算.
	//@{
	/**	ベクトルの要素を全てvにする.
		@param v 要素型の値.	*/
	void clear(const element_type v=zero(0)){ PTM::clear(exp(), v);	}
	/**	代入(*this = b).
		@param b 同じサイズのベクトル.	*/
	template <class B>
		void assign(const VectorImp<B>& b) { PTM::assign(exp(), b.exp()); }
	/**	代入(*this = b).
		@param b 同じサイズのベクトル.	*/
	void assign(const element_type* b) { PTM::assign(exp(), b); }
	/**	加算(*this += b).
		@param b 同じサイズのベクトル.	*/
	template <class B>
		void add(const VectorImp<B>& b){ PTM::add(exp(), b.exp()); }
	/**	減算(*this -= b).
		@param b 同じサイズのベクトル.	*/
	template <class B>
		void sub(const VectorImp<B>& b){ PTM::sub(exp(), b.exp()); }
	/**	定数倍(*this *= b).
		@param b 要素型.				*/
	void multi(const element_type& b){ PTM::multi(exp(), b); }
	/**	定数分の１(*this /= b).
		@param b 要素型.				*/
	void div(const element_type& b){ PTM::div(exp(), b); }
	/**	内積(return *this * b).
		@param	b	同じサイズのベクトル.
		@return		内積の値(要素型)	*/
	template <class B>
		inline element_type dot(const VectorImp<B>& b) const { return PTM::dot(exp(), b.exp()); }
	/**	比較(return *this == b).
		@param	b	同じサイズのベクトル.
		@return		bool値.	*/
	template <class B>
		bool equal(const VectorImp<B>& b) const { return PTM::equal(exp(), b.exp()); }

	/**	要素ごとの最小値(min(*this, b)).
		@param b 同じサイズのベクトル.	*/
	template <class B>
		void element_min(const VectorImp<B>& b){ element_min_impl(exp(), b.exp()); }
	/**	要素ごとの最大値(max(*this, b)).
		@param b 同じサイズのベクトル.	*/
	template <class B>
		void element_max(const VectorImp<B>& b){ element_max_impl(exp(), b.exp()); }
	///	ベクトルのノルム(大きさ)の2乗を返す.
	element_type square() const { return PTM::dot(exp(),exp()); }
	///	ベクトルのノルム(大きさ)を返す.
	element_type norm() const { return element_type( sqrt((double)square()) ); }
	///	単位ベクトルにする．
	void unitize(){ div(norm()); }
	///	クリッピングする(bとeを頂点とする矩形内に収まるようにする).
	template <class BD, class ED>
		void clip(const VectorImp<BD>& b, const VectorImp<ED>& e){ clip(exp(), b.exp(), e.exp()); }
	//@}

	///@name 入出力
	//@{
	/**	表示.
		@param os 出力先ストリーム	*/
	void print(std::ostream& os, char* sep="( )") const {
		if (sep[0]) os << sep[0];
		if (size()){ os.width(6); os << item(0); }
		for(size_t i=1; i<size(); ++i){
			if (sep[1]) os << sep[1];
			os.width(6); os << item(i);
		}
		if (sep[2]) os << sep[2];
	}
	/**	表示.
		@param is 入力元ストリーム	*/
	template <class T>
	void input(T& is) {
		is >> std::ws;
		if (is.peek() != '('){
			is.setstate(std::ios::badbit);
			return;
		}
		is.get();
		std::vector<element_type> v;
		while(1){
			v.push_back(element_type());
			is >> v.back();
			if (!is.good()) break;
			if (size()<v.size()) resize(v.size());
			if (size()<v.size()) break;
		}
		v.pop_back();
		for(unsigned i=0; i<v.size(); ++i){
			item(i) = v[i];
		}
		is.clear();
		is >> std::ws;
		if (is.peek() == ')'){
            is.get();
			return;
		}
		is.setstate(std::ios::badbit);
	}
	//@}

	///@name 演算子
	//@{
	///	[] 演算子(基数は0).
	template <class I> element_type& operator[] (I n) { return item(n); }
	///	[] 演算子(基数は0) (const 版).
	template <class I> const element_type& operator[] (I n) const { return item(n); }
	///	element_type * への変換
	operator element_type* (){ return &item(0); }
	operator const element_type* () const { return &item(0); }
	/**	ベクトルのスカラー倍(return this * b).
		@param	b	要素型
		@return	thisと同じベクトル型	*/
	ret_type operator * (element_type b) const { ret_type rv(*this); rv.multi(b); return rv; }
	/**	ベクトルのスカラー分の１(return this / b).
		@param	b	要素型
		@return	thisと同じベクトル型	*/
	ret_type operator / (element_type b) const { ret_type rv(*this); rv.div(b); return rv; }
	/**	+= 演算子(*this = *this + b).
		@param b サイズが等しいベクトル型.	*/
	template <class BD>
	ret_type operator+= (const VectorImp<BD>& b) { add(b); return *this; }
	/**	-= 演算子(*this = *this - b)
		@param b サイズが等しいベクトル型.	*/
	template <class BD>
	ret_type operator-= (const VectorImp<BD>& b) { sub(b); return *this; }
	/**	*= 演算子 (*this = *this * b)
		@param b 要素型.	*/
	template <class T> ret_type operator*= (T b) { multi(element_type(b)); return *this; }
	/**	/= 演算子 (*this = 1/b * *this).
		@param b 要素型.	*/
	template <class T> ret_type operator/= (T b) { div(element_type(b)); return *this; }
	/**	- 演算子 (return -*this).	*/
	ret_type operator- () const { ret_type r(*this); r*=element_type(-1); return r; }
	///	単位ベクトルを返す．
	ret_type unit() const { ret_type r(*this); r.unitize(); return r; }
	//@}
protected:
	///	バッファの初期化．何もしない．
	void init_buffer(){}
	///	デフォルトコンストラクタ
	void set_default(){}
};

///	次元をテンプレートで持つベクトルの基本型
template<size_t N, class D>
class TVectorBase: public VectorImp<D> {
protected:
	TVectorBase(){}
public:
	static const size_t SIZE = N;
	size_t size_impl() const { return SIZE; }
	void resize_impl(size_t sz) { assert(sz==SIZE); }
};


/**	ベクトル型派生クラスに必要なメンバの定義.
	派生クラスを作るたびに，このマクロを使ってメンバを作る.
	デフォルトコンストラクタ， 同じサイズのベクトルによる
	初期化・代入などが定義される．
	@param	THIS	新たに宣言する派生クラスの型名.
	@see	TVector	*/
#define DEF_VECTOR_BASIC_MEMBER(THIS)										\
	typedef THIS this_type;			/*  このクラスの型を定義しておく. */	\
	typedef TYPENAME desc::element_type element_type;						\
	typedef TYPENAME desc::ret_type ret_type;								\
	/*	コピーコンストラクタ	*/											\
	THIS& operator =(const THIS& b){										\
		this->exp().assign(b); return *this;											\
	}																		\
	/*  ベクトル b を代入	*/												\
	template <class D>														\
	THIS& operator =(const PTM::VectorImp<D>& b){							\
		this->exp().assign(b); return *this;										\
	}																		\
	/*  ベクトル b を加算	*/												\
	template <class D>														\
	THIS& operator +=(const PTM::VectorImp<D>& b){							\
		this->exp().add(b); return *this;											\
	}																		\
	/*  ベクトル b を減算	*/												\
	template <class D>														\
	THIS& operator -=(const PTM::VectorImp<D>& b){							\
		this->exp().sub(b); return *this;											\
	}																		\
	/*  要素数が等しい配列 p の代入	*/										\
	THIS& operator =(const element_type* p){								\
		this->exp().assign(p);											\
		return *this;														\
	}																		\

#define DEF_TVECTOR_BASIC_MEMBER(THIS)										\
	DEF_VECTOR_BASIC_MEMBER(THIS)											\
	/*  ベクトル b による初期化		*/										\
	template <class D>														\
	THIS(const PTM::VectorImp<D>& b){										\
		this->init_buffer(); this->assign(b);}								\
	/*  要素数が等しい配列 p による初期化	*/								\
	THIS(const element_type* p){											\
		this->assign(p);															\
	}																		\
	/*	デフォルトコンストラクタ	*/										\
	THIS(){ this->init_buffer(); this->set_default(); }			\

//----------------------------------------------------------------------------
/**	部分ベクトル型作成のためのユーティリティークラス.
	TVecDim<次元数> で部分ベクトル型を取得できる。	*/
template <const size_t SOFF, const size_t SDIM>
class TSubVectorDim{
public:
	static const size_t OFFSET=SOFF;
	static const size_t SIZE=SDIM;
	static const size_t STRIDE=1;
};
//----------------------------------------------------------------------------
/**	ベクトル型. TVector<3, float> v; のように使う
	@param	N	要素数.
	@param	T	要素の型.
	@see		TVector型の演算子
*/
template <size_t N, class T>
class TVector:public TVectorBase<N, TVectorDesc<1, TVector<N,T>, TVector<N,T>, T> >{
public:
	static const size_t STRIDE=1;
	typedef TVectorDesc<1, TVector<N,T>, TVector<N,T>, T> desc;	///<	型情報
	typedef TVectorBase<N, desc> base_type;						///<	基本クラス型
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVector);
private:
	element_type data[N];										///<	データ
public:	
	///	要素のアクセス
	T& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }
};

template<size_t SZ, size_t STR, class OD>
class TVectorSlice: public TVectorBase<SZ,	TVectorDesc< STR*OD::STRIDE, 
		TVectorSlice<SZ, STR, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> > {
public:
	static const size_t SIZE = SZ;
	static const size_t STRIDE = STR;
	typedef void array_type;
	typedef void const_array_type;
	typedef TVectorDesc< STR*OD::STRIDE, TVectorSlice<SZ, STR, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> desc;
	typedef TVectorBase<SZ, desc> base_type;
	///	継承されない基本的なメンバの定義  @see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(TVectorSlice);
	///	要素のアクセス
	element_type& item_impl(size_t i){ return data[i][0]; }
	const element_type& item_impl(size_t i) const { return data[i][0]; }
	///	ストライド
	size_t stride_impl() const { return STRIDE; }
protected:
	element_type data[SIZE][STRIDE];
};


template<size_t SZ, class OD>
class TSubVector: public TVectorBase<SZ, TVectorDesc< OD::STRIDE, TSubVector<SZ, OD>, 
	TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> > {
public:
	static const size_t SIZE = SZ;
	static const size_t STRIDE = OD::STRIDE;
	typedef void array_type;
	typedef void const_array_type;
	typedef TVectorDesc< OD::STRIDE, TSubVector<SZ, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> desc;
	typedef TVectorBase<SZ, desc> base_type;
	///	継承されない基本的なメンバの定義  @see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(TSubVector);
	///	要素のアクセス
	element_type& item_impl(size_t i){ return data[i][0]; }
	const element_type& item_impl(size_t i) const { return data[i][0]; }
	///	ストライド
	size_t stride_impl() const { return STRIDE; }
protected:
	element_type data[SIZE][STRIDE];
};

//----------------------------------------------------------------------------

/**	ベクトル型. VVector<float> v; のように使う
	@param	T	要素の型.
	@see		TVector型の演算子
*/
template <class T>
class VVector:public VectorImp<TVectorDesc<1, VVector<T>,VVector<T>,T> >{
public:
	typedef TVectorDesc<1,VVector<T>,VVector<T>,T> desc;
	typedef VectorImp<desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_VECTOR_BASIC_MEMBER	*/
	DEF_VECTOR_BASIC_MEMBER(VVector);
	///	基本型の定義
	typedef element_type* array_type;
	typedef const element_type* const_array_type;
private:
	///	メモリの実体
	array_type data;
	///
	size_t size_;
public:
	///	デフォルトコンストラクタ
	VVector(){
		init_buffer();
		this->set_default();
	}
	///	コピーコンストラクタ
	VVector(const VVector& s){
		init_buffer();
		resize(s.size_);
		memcpy(data, s.data, sizeof(element_type) * size_);
	}
	template <class D>
	VVector(const VectorImp<D>& b){
		init_buffer();
		this->assign(b);
	}
	///	
	~VVector(){
    	delete [] data;
    }
	///	ベクトルのサイズ.
	size_t size_impl() const { return size_; }
	///	ベクトルのサイズの設定
	void resize_impl(size_t s){
		if (s > size_){
			delete data;
			data = new element_type[s];
		}
		size_ = s;
	}
	///	ストライド
	size_t stride_impl() const { return 1; }
	///	要素のアクセス
	T& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
protected:
	void init_buffer(){data=0; size_=0;}
};

/**	ベクトル型. EVector<float> v; のように使う
	@param	T	要素の型.
	@see		Vector型の演算子
*/
template <class T>
class ConstEVector:public VectorImp< TVectorDesc<1, ConstEVector<T>, VVector<T>,T,T> >{
public:
	typedef TVectorDesc<1, ConstEVector<T>,VVector<T>,T,T> desc;
	typedef VectorImp<desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_VECTOR_BASIC_MEMBER	*/
	DEF_VECTOR_BASIC_MEMBER(ConstEVector);
	typedef const element_type* array_type;
private:
	///	メモリの実体
	array_type data;
	///
	size_t size_;
public:
	///
	ConstEVector(array_type v, size_t sz):data(v), size_(sz){}
	//
	ConstEVector(size_t sz, size_t str, array_type v):data(v), size_(sz){ assert(str==1);}
	///	ベクトルのサイズ.
	size_t size_impl() const { return size_; }
	///	ベクトルのサイズの設定
	void resize_impl(size_t s){
		assert(size_ == s);
	}
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }
};

template <class T>
class EVector:public VectorImp< TVectorDesc<1, EVector<T>, VVector<T>,T,T> >{
public:
	typedef const ConstEVector<T> const_type;
	typedef TVectorDesc<1, EVector<T>,VVector<T>,T,T> desc;
	typedef VectorImp<desc> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_VECTOR_BASIC_MEMBER	*/
	DEF_VECTOR_BASIC_MEMBER(EVector);
	typedef element_type* array_type;
private:
	///	メモリの実体
	array_type data;
	///
	size_t size_;
public:
	///
	EVector(size_t sz, array_type v):data(v), size_(sz){}
	EVector(size_t sz, size_t str, array_type v):data(v), size_(sz){ assert(str==1); }
	///	ベクトルのサイズ.
	size_t size_impl() const { return size_; }
	///	ベクトルのサイズの設定
	void resize_impl(size_t s){
		assert(size_ == s);
	}
	///	要素のアクセス
	const T& item_impl(size_t i) const { return data[i]; }
	T& item_impl(size_t i) { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }
};
template<class T>
class ConstEVectorSlice: public VectorImp< VectorDesc<ConstEVectorSlice<T>, VVector<T>, T> >{
public:
	typedef void array_type;
	typedef void const_array_type;
	typedef VectorDesc<EVectorSlice<T>, VVector<T>, T> desc;
	typedef VectorImp<desc> base_type;
	///	継承されない基本的なメンバの定義.	@see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(ConstEVectorSlice);
	///
	ConstEVectorSlice(size_t sz, size_t str, const T* d):data(d), size_(sz), stride_(str){}
	///
	ConstEVectorSlice(const ConstEVectorSlice& v):data(v.data), size_(v.size_), stride_(v.stride_){}
	///	要素のアクセス
	element_type& item_impl(size_t i){ return (element_type&)data[index(i)]; } ///< const外し．もっとelegantな方法があると思うが… tazz
	///	要素のアクセス
	const element_type& item_impl(size_t i) const { return data[index(i)]; }
	///	サイズ
	size_t size_impl() const { return size_; }
	///	サイズの設定
	void resize_impl(size_t sz) { size_ = sz; }
	///	ストライド
	size_t stride_impl() const { return stride_; }
protected:
	const element_type* data;
	size_t size_;
	size_t stride_;
	///	元の行列へのアクセス
	size_t index(size_t i) const { return i*stride_; }
};
template<class T>
class EVectorSlice: public VectorImp< VectorDesc<EVectorSlice<T>, VVector<T>, T> >{
public:
	typedef const ConstEVectorSlice<T> const_type;
	typedef void array_type;
	typedef void const_array_type;
	typedef VectorDesc<EVectorSlice<T>, VVector<T>, T> desc;
	typedef VectorImp<desc> base_type;
	///	継承されない基本的なメンバの定義.	@see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(EVectorSlice);
	///
	EVectorSlice(size_t sz, size_t str, T* d):data(d), size_(sz), stride_(str){}
	///
	EVectorSlice(const EVectorSlice& v):data(v.data), size_(v.size_), stride_(v.stride_){}
	///	要素のアクセス
	element_type& item_impl(size_t i){ return data[index(i)]; }
	///	要素のアクセス
	const element_type& item_impl(size_t i) const { return data[index(i)]; }
	///	サイズ
	size_t size_impl() const { return size_; }
	///	サイズの設定
	void resize_impl(size_t sz) { size_ = sz; }
	///	ストライド
	size_t stride_impl() const { return stride_; }
protected:
	element_type* data;
	size_t size_;
	size_t stride_;
	///	元の行列へのアクセス
	size_t index(size_t i) const { return i*stride_; }
};

//----------------------------------------------------------------------------
//	グローバル関数・演算子

/**	ベクトルの和(return a + b).
	@param	a	ベクトル型
	@param	b	aと同じ次元数のベクトル型
	@return		aと同じ次元数のTVector型	*/
template <class AD, class BD>
TYPENAME AD::ret_type operator + (const VectorImp<AD>& a, const VectorImp<BD>& b) {
	TYPENAME AD::ret_type rv(a);
	rv.add(b);
	return rv;
}
/**	ベクトルの差(return a - b).
	@param	a	ベクトル型
	@param	b	aと同じ次元数のベクトル型
	@return		aと同じ次元数のベクトル型	*/
template <class AD, class BD>
TYPENAME AD::ret_type operator - (const VectorImp<AD>& a, const VectorImp<BD>& b) {
	TYPENAME AD::ret_type rv(a);
	rv.sub(b);
	return rv;
}

/**	スカラーとベクトルの積(return a * b).
	@param	a	ベクトル型
	@param	b	要素型
	@return		ベクトル型	*/
template <class BD>
TYPENAME BD::ret_type operator * (const TYPENAME BD::element_type& a, const VectorImp<BD>& b) {
	TYPENAME BD::ret_type r(b);
	r.multi(a);
	return r;
}

/**	スカラーとベクトルの積(return a * b).ベクトルとスカラーは可換なため．
	@param	a	要素型
	@param	b	ベクトル型
	@return		ベクトル型	*/
template <class BD>
TYPENAME BD::ret_type operator * (const VectorImp<BD>& a, const TYPENAME BD::element_type& b) {
	TYPENAME BD::ret_type r(a);
	r.multi(b);
	return r;
}

/**	ベクトルの内積(return a * b).
	@param	a	ベクトル型
	@param	b	aと同じ次元数のベクトル型
	@return		要素型	*/
template <class AD, class BD>
inline typename AD::element_type operator * (const VectorImp<AD>& a, const VectorImp<BD>& b) {
	return a.dot(b);
}

/**	比較(return a == b).
	@param	a	ベクトル型
	@param	b	要素型aと同じ次元数のベクトル型
	@return		bool	*/
template <class AD, class BD>
bool operator == (const VectorImp<AD>& a, const VectorImp<BD>& b) { return a.equal(b); }
/**	比較(return a != b).
	@param	a	ベクトル型
	@param	b	要素型aと同じ次元数のベクトル型
	@return		bool	*/
template <class AD, class BD>
bool operator != (const VectorImp<AD>& a, const VectorImp<BD>& b) { return !a.equal(b); }

///	ストリームに書式付で出力する.
template <class BD>
std::ostream& operator << (std::ostream& os, const VectorImp<BD>& v){
	v.print(os);
	return os;
}
///	ストリームから書式付で入力する.
template <class BD>
std::istream& operator >> (std::istream& is, VectorImp<BD>& v){
	v.input(is);
	return is;
}
/**	2次元ベクトルの外積(return a x b).
	@param	a	2次元ベクトル型
	@param	b	2次元ベクトル型
	@return		要素型	*/
template <class	AD, class BD>
typename AD::element_type cross (const TVectorBase<2, AD>& a, const TVectorBase<2, BD>& b) {
	return a[0] * b[1] - a[1] * b[0];
}
template <class	AD, class BD>
typename AD::element_type operator % (const TVectorBase<2, AD>& a, const TVectorBase<2, BD>& b) {
	return cross(a, b);
}
template <class	AD, class BD>
typename AD::element_type operator ^ (const TVectorBase<2, AD>& a, const TVectorBase<2, BD>& b) {
	return cross(a, b);
}

/**	3次元ベクトルの外積(return a x b).
	@param	a	3次元ベクトル型
	@param	b	3次元ベクトル型
	@return		3次元ベクトル型	*/
template <class	AD, class BD>
TYPENAME AD::ret_type cross (const TVectorBase<3, AD>& a, const TVectorBase<3, BD>& b) {
	TYPENAME AD::ret_type r;
	typedef TYPENAME AD::element_type ADET;
	r[0] = ADET(a[1] * b[2] - a[2] * b[1]);
	r[1] = ADET(a[2] * b[0] - a[0] * b[2]);
	r[2] = ADET(a[0] * b[1] - a[1] * b[0]);
	return r;
}

template <class	AD, class BD>
TYPENAME AD::ret_type operator % (const TVectorBase<3, AD>& a, const TVectorBase<3, BD>& b) {
	return cross(a, b);
}
template <class	AD, class BD>
TYPENAME AD::ret_type operator ^ (const TVectorBase<3, AD>& a, const TVectorBase<3, BD>& b) {
	return cross(a, b);
}
/**	補間
	@param t 補間量0-1
	@param a ベクトル
	@param b 同じサイズのベクトル
*/
template <class AD, class BD>
TYPENAME AD::ret_type interpolate(TYPENAME AD::element_type& t,const VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a. size());
	TYPENAME AD::ret_type c(a);
	if(t > (TYPENAME AD::element_type)1.0) t = (TYPENAME AD::element_type)1.0;
	return c = a + ((TYPENAME AD::ret_type)b - a) * t;
}
//@}

}	//	namespace PTM
#endif


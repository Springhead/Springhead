/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PTMATRIX_TMATRIX_H
#define PTMATRIX_TMATRIX_H

/**	\addtogroup gpLinearAlgebra	*/
//@{
/** 
	@page PTM ポータブル テンプレート 行列クラスライブラリ
	
	@author 長谷川 晶一

	@date 2001年6月10日,2003年10月20日更新

	@contents
	
	@section introM はじめに
		このドキュメントはポータブル テンプレート 行列クラスライブラリ
		のドキュメントです．
		ポータブル テンプレート 行列クラスライブラリは，
		テンプレートによるM×N行列のクラスライブラリです．
		行列のサイズをテンプレートで持つバージョンと変数で持つバージョンがあります．
		
	@section specM このライブラリの特徴
	@subsection tmpM テンプレートで解決
		テンプレート版は，全てをテンプレートで静的に解決しています．
		行列のサイズやポインタなどを保持するための管理領域を持ちません．
		そのため，
		<ul>
			<li> 行列の一部(部分行列，ベクトル，行ベクトル，列ベクトル)などを
				直接参照することができる(代入も可能)．
			<li> 配列をキャストして行列として使用することも可能．
			<li> 行列の掛け算などで，行列のサイズが合わない場合，コンパイル時に
				コンパイラがエラーを出力する．
			<li> 行列のサイズを動的に変更することができない．
		</ul>
		といった特徴をもちます．
	@subsection portM 移植性
		処理系依存部分の多いテンプレート機能を活用していながら，全機能が，
		3つのコンパイラで使用できます．サポートしているコンパイラは，
		<ul>
			<li> CL (MS Visual C++ 6.0)
			<li> bcc32(Borland C++ 5.5.1)	ごめんなさいまだ未確認です．多分動作しません．
			<li> gcc(GNU c compiler 2.95.3-5)
		</ul>
		です．
	@subsection why 新たにクラスライブラリを作成した理由
		すでに多くの行列ライブラリがありながら，新たに作成した理由は，
		- TNT，MTL の行列は管理領域をメモリに持つため，配列をキャストして
			行列として使用することができない．
		- Blitz は TinyMatrix, TinyVector を持つが，Visual C++ で
			使用できない．
		- 私の知る限り，部分行列，部分ベクトルへの参照を返す行列ライブラリ
			は存在しない．

	からです．
	
	@section ptm_usage 使い方
	ポータブル テンプレート 行列クラスライブラリは，ヘッダファイルだけからなる
	クラスライブラリなので, TMatrix.h, TMatrixUtility.h, TVector.h
	を同じフォルダに入れておき，.cppファイルからヘッダをインクルードするだけで
	使用できます．
	@subsection sampleM サンプル
		簡単なサンプルです．適当なファイル名(たとえば sample.cpp) で保存して
		コンパイルしてください．コンパイルするためには，
		<DL>
		<DT> visual C++ の場合
		<DD> cl -GX sample.cpp
		<DT> gccの場合
		<DD> g++ sample.cpp
		<DT> bcc の場合
		<DD> bcc32 sample.cpp
		</DL>
	としてください．
	@verbatim
#include "TMatrix.h"    //  行列ライブラリのインクルードする．
#include <iostream>
using namespace PTM;    //  行列クラスはPTM名前空間の中で宣言されている．
void main(){
    TMatrixRow<2,2,float> mat;     //  2行2列の行列を宣言
    mat[0][0] = 1;  mat[0][1] = 2;
    mat[1][0] = 3;  mat[1][1] = 4;
    TVector<2,float> vec;       //  2次元のベクトルを宣言
    vec[0] = 1; vec[1] = 0;
    std::cout << mat;
    std::cout << vec << std::endl;
    std::cout << mat * vec << std::endl;	//	掛け算
    std::cout << mat + mat << std::endl;	//	足し算
    std::cout << mat - mat << std::endl;	//	引き算
    std::cout << mat.trans() << std::endl;	//	転置
    std::cout << mat.inv() << std::endl;	//	逆行列
}
@endverbatim
	@subsection vecfunc ベクトルの機能
	次の演算ができます．
	<ul>
	<li> +:和, -:差, *:内積/定数倍, /:定数分の1
	<li> ==:比較, =:代入
	<li> <<:出力, >>:入力
	<li> %:外積(2・3次元のみ)
	</ul>
	次のメンバ関数を持ちます．
	<ul>
	<li> unit(): 向きが等しい単位ベクトルを返す．
	<li> norm(): ベクトルの大きさ(ノルム)を返す．
	<li> sub_vector(): 部分ベクトルへの参照を返す．
	</ul>
	変数版はサイズの変更ができます．
	<ul>
	<li> resize(int h, int w):	サイズの変更
	<li> height():	高さ＝行列の行数
	<li> width():	幅＝行列の列数
	</ul>
	@subsection matfunc 行列の機能
	次の演算ができます．
	<ul>
		<li> +:和, -:差, *:積/定数倍, /:定数分の1
		<li> ==:比較, =:代入
		<li> <<:出力, >>:入力
	</ul>
	次のメンバ関数を持ちます．
	<ul>
		<li> det(): 行列式を返す．
		<li> inv(): 逆行列を返す．
		<li> gauss(): ガウスの消去法で方程式を解く．
		<li> sub_matrix(): 部分行列への参照を返す．
		<li> row(): 行ベクトルへの参照を返す．
		<li> col(): 列ベクトルへの参照を返す．
	</ul>
		sub_matrix()やrow()には値を代入することもできます．
		@verbatim
	TMatrixRow<3,3,float> mat; TVector<3, float> vec;
	mat.row() = vec;@endverbatim
	@section pub 再配布の条件
	再配布する場合は，原著作者の署名・連絡先を改変・削除しないでください．
	閉鎖的な会社でソフトウエアを書く人にも自由に使用できるようにしたい
	ので，GPL・LGPLにしませんでした．
	もちろんGPL・LGPLに改変して再配布してくださっても結構です．
	@section support サポート
	バグ，不具合，意味不明なコンパイルエラーなどを見つけた場合は，
	長谷川 晶一 (hase@hi.pi.titech.ac.jp) までご連絡ください．
	できる限りサポートし，より良いライブラリにしていくつもりです．<br>
	特にこのライブラリはテンプレートクラスライブラリなので，使用時に，
	内部の動作が分らないと意味のわからないコンパイルエラーに出会うことも
	あると思います．そのような問題には対応するつもりですので，まずはご連絡ください．
	@section thanksM 謝辞
	LU分解，逆行列，ガウス消去法などの行列計算アルゴリズムは，<br>
    「『Ｃ言語による最新アルゴリズム事典』全ソースコード」<br>
    ftp://ftp.matsusaka-u.ac.jp/pub/algorithms<br>
	奥村 晴彦 Haruhiko Okumura<br>
	を改変して流用させていただきました．
	自由にコードを使わせてくださって，ありがとうございます．	*/
//-----------------------------------------------------------------------------
/**	@file TMatrix.h
	テンプレートによるN×M行列型の定義.
	要素の型とサイズをテンプレートの引数にすることで，
	管理情報をメモリに持たずに，一般の行列を扱う．
	配列をキャストして行列にすることもできる．
	部分行列や行や列を行列やベクトルとして取り出すこともできる．
	sparse matrix には対応していない．										*/
//------------------------------------------------------------------------------

#include "TVector.h"

///	Portable Template Matrixライブラリの名前空間
namespace PTM{;

#ifdef _WIN32
 #pragma pack(push, 4)
 #ifdef _DEBUG
  #pragma optimize ("awgity", on)
  #pragma auto_inline(on)
  #pragma inline_recursion(on)
 #endif
#endif

/**	部分行列型作成のためのユーティリティークラス.
	TSubMatrixDim<top, left, height, width> と次元を指定できる。*/
template <size_t T, size_t L, size_t H, size_t W>
class TSubMatrixDim{
public:
	DIMDEF(DIMENC(T),		TOP);
	DIMDEF(DIMENC(L),		LEFT);
	DIMDEF(DIMENC(H),		HEIGHT);
	DIMDEF(DIMENC(W),		WIDTH);
};
/**	部分行列型作成のためのユーティリティークラス.
	TMatDim<height, width> と次元を指定できる。	*/
template <size_t H, size_t W>
class TMatDim{
public:
	DIMDEF(DIMENC(H),		HEIGHT);
	DIMDEF(DIMENC(W),		WIDTH);
};

template <size_t H, size_t W, class OD> class TSubMatrixRow;
template <class T, class Z=T, class U=Z> class ESubMatrixRow;
template <size_t H, size_t W, class OD> class TSubMatrixCol;
template <class T, class Z=T, class U=Z> class ESubMatrixCol;
///	行ベクトルの基本型	サイズ：テンプレート
template <class desc>
class TMakeSubMatrixRow{
public:
	typedef TYPENAME desc::element_type element_type;
	typedef TYPENAME desc::exp_type exp_type;
	exp_type& exp(){ return *(exp_type*)this;}
	const exp_type& exp() const { return *(exp_type*)this;}
	///	@name 部分行列
	//@{
	///	部分行列（テンプレート版）
	template <class SUB>
		TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(SUB){
		return (TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(DIMDEC(SUB::TOP), DIMDEC(SUB::LEFT));
	}
	template <class SUB>
		const TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(SUB) const {
		return (TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(DIMDEC(SUB::TOP), DIMDEC(SUB::LEFT));
	}
	///	部分行列（サイズだけテンプレート版）
	template <class SUB>
		TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(size_t t, size_t l, SUB){
		return (TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(t,l);
	}
	template <class SUB>
		const TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(size_t t, size_t l, SUB) const {
		return (TSubMatrixRow<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(t,l);
	}
	///	部分行列（変数版）
	ESubMatrixRow<element_type> vsub_matrix(int t, int l, int h, int w){
		return ESubMatrixRow<element_type>(h, w, exp().stride(), &exp().item(t,l));
	}
	//@}
};
///	列ベクトルの基本型	サイズ：テンプレート
template <class desc>
class TMakeSubMatrixCol{
public:
	typedef TYPENAME desc::element_type element_type;
	typedef TYPENAME desc::exp_type exp_type;
	exp_type& exp(){ return *(exp_type*)this;}
	const exp_type& exp() const { return *(exp_type*)this;}
	///	@name 部分行列
	//@{
	///	部分行列（テンプレート版）
	template <class SUB>
		TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(SUB){
		return (TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(DIMDEC(SUB::TOP), DIMDEC(SUB::LEFT));
	}
	template <class SUB>
		const TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(SUB) const {
		return (TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(DIMDEC(SUB::TOP), DIMDEC(SUB::LEFT));
	}
	///	部分行列（サイズだけテンプレート版）
	template <class SUB>
		TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(size_t t, size_t l, SUB){
		return (TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(t,l);
	}
	template <class SUB>
		const TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>& sub_matrix(size_t t, size_t l, SUB) const {
		return (TSubMatrixCol<DIMDEC(SUB::HEIGHT), DIMDEC(SUB::WIDTH), desc>&)exp().item(t,l);
	}
	///	部分行列（変数版）
	ESubMatrixCol<element_type> vsub_matrix(int t, int l, int h, int w){
		return ESubMatrixCol<element_type>(h, w, exp().stride(), &exp().item(t,l));
	}
	//@}
};
///	行ベクトルの基本型	サイズ：変数
template <class desc>
class EMakeSubMatrixRow{
public:
	typedef TYPENAME desc::element_type element_type;
	typedef TYPENAME desc::exp_type exp_type;
	exp_type& exp(){ return *(exp_type*)this;}
	const exp_type& exp() const { return *(exp_type*)this;}
	///	@name 部分行列
	//@{
	///	部分行列（変数版）
	ESubMatrixRow<element_type> vsub_matrix(int t, int l, int h, int w){
		return ESubMatrixRow<element_type>(h, w, exp().stride(), &exp().item(t,l));
	}
	//@}
};
///	列ベクトルの基本型	サイズ：変数
template <class desc>
class EMakeSubMatrixCol{
public:
	typedef TYPENAME desc::element_type element_type;
	typedef TYPENAME desc::exp_type exp_type;
	exp_type& exp(){ return *(exp_type*)this;}
	const exp_type& exp() const { return *(exp_type*)this;}
	///	@name 部分行列
	//@{
	///	部分行列（変数版）
	ESubMatrixCol<element_type> vsub_matrix(int t, int l, int h, int w){
		return ESubMatrixCol<element_type>(h, w, exp().stride(), &exp().item(t,l));
	}
	//@}
};


//----------------------------------------------------------------------------
///	@name 行列演算の実装
//@{
template <class DESC> class MatrixImp;
template <DIMTYPE H, DIMTYPE W, class D> class TMatrixBaseBase;
template <DIMTYPE H, DIMTYPE W, class D> class TMatrixBase;
template <class DESC> class EMatrixBase;

/**	代入(*this = b).
	@param b 同じサイズの行列.	*/
template <class AD, class BD>
void assign(MatrixImp<AD>& a, const MatrixImp<BD>& b) {
	a.resize(b.height(), b.width());
	a.size_assert(b);
	for(size_t i=0; i<a.height(); ++i)
		for(size_t j=0; j<a.width(); ++j)
			a.item(i,j) = (TYPENAME AD::element_type)b.item(i,j);
}
/**	代入(*this = b).
	@param b 同じサイズのベクトル.	*/
template <class AD>
void assign(MatrixImp<AD>& a, const TYPENAME AD::element_type* b) {
	for(size_t i=0; i<a.height(); ++i)
		for(size_t j=0; j<a.width(); ++j)
			a.item(i,j) = b[i*a.width()+j];
}
///	比較
template <class AD, class BD>
bool equal(const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	if (!a.size_check(b)) return false;
	for(size_t i=0; i<a.height(); ++i)
		for(size_t j=0; j<a.width(); ++j)
			if (a.item(i,j) != b.item(i,j)) return false;
	return true;
}
///	加算
template <class AD, class BD>
void add(MatrixImp<AD>& a, const MatrixImp<BD>& b){
	a.size_assert(b);
	for(size_t i=0; i<a.height(); ++i)
		for(size_t j=0; j<a.width(); ++j)
			a.item(i,j) += b.item(i,j);
}
///	減算
template <class AD, class BD>
void sub(MatrixImp<AD>& a, const MatrixImp<BD>& b){
	a.size_assert(b);
	for(size_t i=0; i<a.height(); ++i)
		for(size_t j=0; j<a.width(); ++j)
			a.item(i,j) -= b.item(i,j);
}
///	行列とスカラーの掛け算
template <class AD>
void multi(MatrixImp<AD>& a, TYPENAME AD::element_type b){
	for(size_t i=0; i<a.height(); ++i)
		for(size_t j=0; j<a.width(); ++j)
			a.item(i,j) *= b;
}
///	行列とベクトルの掛け算
template <class RD, class AD, class BD>
void multi(VectorImp<RD>& r, const MatrixImp<AD>& a, const VectorImp<BD>& b){
	r.resize(a.height());
	for(size_t n=0; n<r.size(); ++n){
		r.item(n) = a.row(n) * b;
	}
}
///	行列とベクトルの掛け算	:	3x3
template <class RD, class AD, class BD>
void multi(TVectorBase<DIMENC(3), RD>& r, const TMatrixBase<DIMENC(3),DIMENC(3),AD>& a, const TVectorBase<DIMENC(3), BD>& b){
	r(0) = a.row(0) * b;
	r(1) = a.row(1) * b;
	r(2) = a.row(2) * b;
}
///	行列の掛け算
template <class RD, class AD, class BD>
void multi(MatrixImp<RD>& r, const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	typedef TYPENAME RD::zero zero;
	assert(a.width()==b.height());
	r.resize(a.height(), b.width());
	for(size_t i=0; i<a.height(); ++i){
		for(size_t j=0; j<b.width(); ++j){
			r.item(i,j) = zero(0);
			for(size_t k=0; k<a.width(); ++k){
				r.item(i,j) += a.item(i,k) * b.item(k,j);
			}
		}
	}
}
///	行列の掛け算	サイズ：3x3
#ifndef __BORLANDC__
template <class RD, class AD, class BD>
void multi(TMatrixBase<DIMENC(3), DIMENC(3), RD>& r, const TMatrixBase<DIMENC(3), DIMENC(3), AD>& a, const TMatrixBase<DIMENC(3), DIMENC(3), BD>& b){
        typedef TYPENAME RD::element_type ET;
#define CALC(i,j) r.item(i,j) = ET( a.item(i,0)*b.item(0,j) + a.item(i,1)*b.item(1,j) + a.item(i,2)*b.item(2,j) )
	CALC(0,0);	CALC(0,1);	CALC(0,2);
	CALC(1,0);	CALC(1,1);	CALC(1,2);
	CALC(2,0);	CALC(2,1);	CALC(2,2);
#undef CALC
}
#endif
///	行列の掛け算	サイズ：4x4
template <class RD, class AD, class BD>
void multi(TMatrixBase<DIMENC(4), DIMENC(4), RD>& r, const TMatrixBase<DIMENC(4), DIMENC(4), AD>& a, const TMatrixBase<DIMENC(4), DIMENC(4), BD>& b){
        typedef TYPENAME RD::element_type ET;
#define CALC(i,j) r.item(i,j) = ET( a.item(i,0)*b.item(0,j) + a.item(i,1)*b.item(1,j) + a.item(i,2)*b.item(2,j) + a.item(i,3)*b.item(3,j) )
	CALC(0,0);	CALC(0,1);	CALC(0,2);	CALC(0,3);
	CALC(1,0);	CALC(1,1);	CALC(1,2);	CALC(1,3);
	CALC(2,0);	CALC(2,1);	CALC(2,2);	CALC(2,3);
	CALC(3,0);	CALC(3,1);	CALC(3,2);	CALC(3,3);
#undef CALC
}

///	行列式	サイズ：変数
template <class AD>
TYPENAME AD::element_type det(const MatrixImp<AD>& a){
	TYPENAME AD::ret_type tmp(a);
	VVector<int> ip;
	VVector<TYPENAME AD::element_type> w;
	ip.resize(a.height());
	w.resize(a.height());
	return lu(tmp, ip, w);
}
///	行列式	サイズ：テンプレート
template <DIMTYPE H, DIMTYPE W, class AD>
TYPENAME AD::element_type det(const TMatrixBaseBase<H,W,AD>& a){
	TYPENAME AD::ret_type tmp(a);
	TVector<DIMDEC(H), int> ip;
	TVector<DIMDEC(H), TYPENAME AD::element_type> w;
	return lu(tmp, ip, w);
}
#ifndef __BORLANDC__
///	行列式	サイズ：2x2
template <class AD>
TYPENAME AD::element_type det(const TMatrixBase<DIMENC(2),DIMENC(2),AD>& a){
	return a.item(0,0) * a.item(1,1) - a.item(0,1) * a.item(1,0);
}
#endif
///	行列式	サイズ：3x3
template <class AD>
TYPENAME AD::element_type det(const TMatrixBase<DIMENC(3),DIMENC(3),AD>& a){
	return 
		( a.item(0,0) * a.item(1,1) * a.item(2,2)  +  a.item(1,0) * a.item(2,1) * a.item(0,2)  +  a.item(2,0) * a.item(0,1) * a.item(1,2) ) -
		( a.item(2,0) * a.item(1,1) * a.item(0,2)  +  a.item(0,0) * a.item(2,1) * a.item(1,2)  +  a.item(1,0) * a.item(0,1) * a.item(2,2) );
}
///	LU分解を行う。a を書き換える。行列式を返す。
template <class AD>
TYPENAME AD::element_type lu(MatrixImp<AD>& a, int* ip, TYPENAME AD::element_type* weight){
#define ABS_LU_MATRIX(a)	((a)>0 ? (a) : -(a))
	assert(a.width() == a.height());
	int i, j, k, ii, ik;
	int n = (int)a.height();
	TYPENAME AD::element_type t, u, det_;
	
	det_ = 0;                   // 行列式
	for (k = 0; k < n; k++) {  // 各行について
		ip[k] = k;             // 行交換情報の初期値
		u = 0;                 // その行の絶対値最大の要素を求める
		for (j = 0; j < n; j++) {
			t = ABS_LU_MATRIX(a.item(k,j));
            if (t > u) u = t;
		}
		if (u == 0) goto PTM_EXIT; // 0 なら行列はLU分解できない
		weight[k] = 1 / u;     // 最大絶対値の逆数
	}
	det_ = 1;                   // 行列式の初期値
	for (k = 0; k < n; k++) {  // 各行について
		u = -1;
		for (i = k; i < n; i++) {  // より下の各行について
			ii = ip[i];            // 重み×絶対値 が最大の行を見つける
			t = ABS_LU_MATRIX(a.item(ii, k)) * weight[ii];
			if (t > u) {  u = t;  j = i;  }
		}
		ik = ip[j];
		if (j != k) {
			ip[j] = ip[k];  ip[k] = ik;  // 行番号を交換
			det_ = -det_;  // 行を交換すれば行列式の符号が変わる
		}
		u = a.item(ik, k);  det_ *= u;  // 対角成分
		if (u == 0) goto PTM_EXIT;    // 0 なら行列はLU分解できない
		for (i = k + 1; i < n; i++) {  // Gauss消去法
			ii = ip[i];
			t = (a.item(ii, k) /= u);
			for (j = k + 1; j < n; j++)
				a.item(ii, j) -= t * a.item(ik, j);
		}
	}
	PTM_EXIT:
	return det_;           // 戻り値は行列式
}
//	a x + b = 0 の1次方程式を解く．LU分解済みの必要あり．
template <class AD, class XD, class BD>
void solve(MatrixImp<AD>& a, VectorImp<XD>& x, const VectorImp<BD>& b, int* ip){
	int i, j, ii;
	TYPENAME XD::element_type t;
	const int n = a.height();
	for (i = 0; i < n; i++) {       // Gauss消去法の残り
		ii = ip[i];  t = b[ii];
		for (j = 0; j < i; j++) t -= a.item(ii, j) * x[j];
		x[i] = t;
	}
	for (i = n - 1; i >= 0; i--) {  // 後退代入
		t = x[i];  ii = ip[i];
		for (j = i + 1; j < n; j++) t -= a.item(ii, j) * x[j];
		x[i] = t / a.item(ii, i);
	}
}
///	コレスキー法．a,sを書き換える．
template <class AD, class BD>
void cholesky(MatrixImp<AD>& a, VectorImp<BD>& s){
	int i,j,k;
	int num = a.height();
	
	//reduction  foreward
	a.item(0,0) = sqrt(a.item(0,0));
	s.item(0) /= a.item(0, 0);
	for(i=1;i<num;i++) {
		a.item(0,i) /= a.item(0,0);
	}

	for(i=1;i<num;i++){
		for(k=0;k<i;k++){
			a.item(i,i) -= a.item(k,i)*a.item(k,i);
			s.item(i) -= a.item(k,i)*s.item(k);
		}
		a.item(i,i) = sqrt(a.item(i,i));
		for(j=i+1; j<num; j++){
			for (k=0; k<i; k++){
				a.item(i,j) -= a.item(k,i) * a.item(k,j);
			}
			a.item(i,j) /= a.item(i,i);
		}
		s.item(i) /= a.item(i,i);
	}

	// backwark substitution 
	for(i=num-1; i>=0; i--){
		for(j=i+1; j<num; j++){			
			s.item(i) -= a.item(i,j) * s.item(j);
		}
		s.item(i) /= a.item(i,i);
	}
}
///	ガウスの消去法，作業領域(行交換の記録)として， int ip[height()];  が必要．
template <class AD, class XD, class BD>
TYPENAME AD::element_type gauss(MatrixImp<AD>& a, VectorImp<XD>& x, const VectorImp<BD>& b, int* ip){
	TYPENAME AD::element_type det_;		// 行列式
	TYPENAME AD::col_vector_type::ret_type w;
	det_ = lu(a, ip, w);				// LU分解
	if (det_ != 0) solve(a, x, b, ip);	// LU分解の結果を使って連立方程式を解く
	return det_;						// 戻り値は行列式
}

/**	逆行列を求める．
	@param a		元の行列(LU分解される)
	@param b		逆行列
	@param ip		作業領域(行交換の記録)
	@param weight	作業領域(行の重み付け)
*/
template <class RD, class AD>
TYPENAME AD::element_type inv(MatrixImp<RD>& r, MatrixImp<AD>& a, int* ip, TYPENAME AD::element_type* weight) {
	assert(a.height() == a.width());
	r.resize(a.height(), a.width());
	int i, j, k, ii;
	int n = (int)a.height();
	TYPENAME AD::element_type t, det;

	det = a.lu(ip, weight);
	if (det != 0){
		for (k = 0; k < n; k++) {
			for (i = 0; i < n; i++) {
				ii = ip[i];  t = (ii == k);
				for (j = 0; j < i; j++)
					t -= a.item(ii, j) * r.item(j, k);
				r.item(i, k) = t;
			}
			for (i = n - 1; i >= 0; i--) {
				t = r.item(i, k);  ii = ip[i];
				for (j = i + 1; j < n; j++)
					t -= a.item(ii, j) * r.item(j, k);
				r.item(i, k) = t / a.item(ii, i);
			}
		}
	}
	return det;
}

///	 逆行列を返す．
template <class AD>
TYPENAME AD::ret_type inv(const MatrixImp<AD>& a){
	typedef TYPENAME AD::ret_type ret_type;
	ret_type r, tmp(a);
	VVector<int> ip;
	ip.resize(a.height());
	VVector<TYPENAME AD::element_type> w;
	w.resize(a.height());
	inv(r, tmp, (int*)ip, (TYPENAME AD::element_type*)w);
	return r;
}
///	 逆行列を返す．	サイズ：テンプレート
template <class AD, DIMTYPE H, DIMTYPE W>
TYPENAME AD::ret_type inv(const TMatrixBaseBase<H,W,AD>& a){
	TYPENAME AD::ret_type r, tmp(a);
	TVector<DIMDEC(H), int> ip;
	TVector<DIMDEC(H), TYPENAME AD::element_type> w;
	inv(r, tmp, (int*)ip, (TYPENAME AD::element_type*)w);
	return r;
}
#ifndef __BORLANDC__
///	逆行列を返す
template <class AD>
TYPENAME AD::ret_type inv(const TMatrixBase<DIMENC(2), DIMENC(2), AD>& a){
	TYPENAME AD::element_type d = a.det();
	TYPENAME AD::ret_type rv;
	rv.item(0,0) =  a.item(1,1) / d;
	rv.item(0,1) = -a.item(0,1) / d;
	rv.item(1,0) = -a.item(1,0) / d;
	rv.item(1,1) =  a.item(0,0) / d;
	return rv;
}
#endif
///	逆行列
template <class AD>
TYPENAME AD::ret_type inv(const TMatrixBase<DIMENC(3), DIMENC(3), AD>& a){
#define DET2_INV_TMATRIXBASE(a,b,c,d)	(a*d - b*c)
	TYPENAME AD::ret_type rtv;
	TYPENAME AD::element_type det_ = 1 / a.det();
	rtv.item(0,0) = DET2_INV_TMATRIXBASE(a.item(1,1), a.item(1,2), a.item(2,1), a.item(2,2)) * det_;
	rtv.item(1,0) = DET2_INV_TMATRIXBASE(a.item(1,2), a.item(1,0), a.item(2,2), a.item(2,0)) * det_;
	rtv.item(2,0) = DET2_INV_TMATRIXBASE(a.item(1,0), a.item(1,1), a.item(2,0), a.item(2,1)) * det_;
		
	rtv.item(0,1) = DET2_INV_TMATRIXBASE(a.item(2,1), a.item(2,2), a.item(0,1), a.item(0,2)) * det_;
	rtv.item(1,1) = DET2_INV_TMATRIXBASE(a.item(2,2), a.item(2,0), a.item(0,2), a.item(0,0)) * det_;
	rtv.item(2,1) = DET2_INV_TMATRIXBASE(a.item(2,0), a.item(2,1), a.item(0,0), a.item(0,1)) * det_;
	
	rtv.item(0,2) = DET2_INV_TMATRIXBASE(a.item(0,1), a.item(0,2), a.item(1,1), a.item(1,2)) * det_;
	rtv.item(1,2) = DET2_INV_TMATRIXBASE(a.item(0,2), a.item(0,0), a.item(1,2), a.item(1,0)) * det_;
	rtv.item(2,2) = DET2_INV_TMATRIXBASE(a.item(0,0), a.item(0,1), a.item(1,0), a.item(1,1)) * det_;
	return rtv;
#undef DET2_INV_TMATRIXBASE
}
//----------------------------------------------------------------------------
/**	行列のインタフェース ．行列クラスの基本クラスとなり，行列へのアクセスを提供する．
	行列の実体は，TMatrix / VMatrix / EMatrix があり，
	テンプレート版，変数版，変数版外部バッファとなっている．
	行列の添え字とサイズの意味は以下の通り．
	@verbatim
    (0,0) (0,1)....................(0,m)...  ^
    (1,0) (1,1)                        :...  | 
      :                                :... height()
      :                                :...  |
    (n,0) (n,1)....................(n,m)...  V
    <---------  width()  -------------->
    <---------  stride()  ---------------->@endverbatim
	メモリのイメージとしては，複数の行で構成される行列(???Row)と
	複数の列で構成される行列(???Col)がある．
*/
///	行列計算の実装
template <class DESC>
class MatrixImp: public DESC::make_sub_matrix{
public:
	typedef DESC desc;
	typedef TYPENAME desc::exp_type				exp_type;
	typedef TYPENAME desc::ret_type				ret_type;
	typedef TYPENAME desc::element_type			element_type;
	typedef TYPENAME desc::row_vector_ref		row_vector_ref;
	typedef TYPENAME desc::const_row_vector_ref	const_row_vector_ref;
	typedef TYPENAME desc::col_vector_ref		col_vector_ref;
	typedef TYPENAME desc::const_col_vector_ref	const_col_vector_ref;
	typedef TYPENAME desc::trans_ref				trans_ref;
	typedef TYPENAME desc::const_trans_ref		const_trans_ref;
	typedef TYPENAME desc::zero					zero;
	typedef TYPENAME desc::unit					unit;
	///	コンストラクタ
	MatrixImp(){}

	///@name 行列の参照
	//@{
	///	実体の取得
	exp_type& exp(){ return *(exp_type*)this; }
	const exp_type& exp() const { return *(const exp_type*)this; }
	///	n番目の要素を返す(基数は0).
	element_type& item(size_t n, size_t m){ return exp().item_impl(n,m); }
	const element_type& item(size_t n, size_t m) const { return exp().item_impl(n,m); }
	///	行ベクトル
	row_vector_ref row(size_t n){ return exp().row_impl(n) ;}
	const_row_vector_ref row(size_t n) const { return exp().row_impl(n) ;}
	template <class I>
	row_vector_ref operator [] (I n){ return exp().row_impl(n) ;}
	template <class I>
	const_row_vector_ref operator [] (I n) const { return exp().row_impl(n) ;}
	///	列ベクトル
	col_vector_ref col(size_t m){ return exp().col_impl(m) ;}
	const_col_vector_ref col(size_t m) const { return exp().col_impl(m) ;}
	///	転置行列
	trans_ref trans() { return exp().trans_impl(); }
	const_trans_ref trans() const { return exp().trans_impl(); }
	///	element_type* へ変換
	operator element_type*(){ return &item(0,0); }
	operator const element_type*() const { return &item(0,0); }
	
	///	行数の取得
	size_t height() const { return exp().height_impl(); }
	///	列数の取得
	size_t width() const { return exp().width_impl(); }
	///	サイズの設定
	void resize(size_t h, size_t w){ exp().resize_impl(h,w); }
	///	stride の取得
	size_t stride() const { return exp().stride_impl(); }
	/**	ベクトルの要素を全てvにする.
		@param v 要素型の値.	*/
	void clear(const element_type v=zero(0)){
		for(size_t i=0; i<height(); ++i)
			for(size_t j=0; j<width(); ++j)
				item(i,j) = v;
	}
	//@}

	///	@name チェック
	//@{
	template <class B> void size_assert(const MatrixImp<B>& b) const {
		assert(height() == b.height() && width() == b.width());
	}
	template <class B> bool size_check(const MatrixImp<B>& b) const {
		return height() == b.height() && width() == b.width();
	}
	//@}

	///	@name 演算
	//@{
	/**	代入(*this = b)	@param b 同じサイズの行列.	*/
	template <class BD> void assign(const MatrixImp<BD>& b) { PTM::assign(exp(), b); }
	/**	代入(*this = b).@param b 同じサイズのベクトル.	*/
	void assign(const element_type* b) { PTM::assign(exp(), b); }
	///	比較
	template <class BD> bool equal(const MatrixImp<BD>& b) const { return PTM::equal(exp(), b); }
	///	加算
	template <class BD> void add(const MatrixImp<BD>& b){ PTM::add(exp(), b); }
	///	減算
	template <class BD> void sub(const MatrixImp<BD>& b){ PTM::sub(exp(), b); }
	///	スカラーの掛け算
	void multi(element_type b){ PTM::multi(exp(), b); }
	///	LU分解を行う。thisを書き換える。行列式を返す。
	element_type lu(int* ip, element_type* weight){ return PTM::lu(exp(), ip, weight); }
	//	(*this) x + b = 0 の1次方程式を解く．LU分解済みの行列でないとだめ．
	template <class XD, class BD> void solve(VectorImp<XD>& x, const VectorImp<BD>& b, int* ip){ PTM::solve(exp(), x, b, ip); }
	///	コレスキー法
	template <class VBASE> void cholesky(VectorImp<VBASE>& s){ PTM::cholesky(exp(), s); }
	///	ガウスの消去法，作業領域(行交換の記録)として， int ip[height()];  が必要．
	template <class XD, class BD> element_type gauss(VectorImp<XD>& x, const VectorImp<BD>& b, int* ip){ return PTM::gauss(exp(), x, b, ip); }
	///	逆行列を求める。
	template <class B> element_type inv(MatrixImp<B>& a_inv, int* ip, element_type* weight) { return PTM::inv(a_inv, exp(), ip, weight); }
	///	逆行列を求める。
	ret_type inv() const { return PTM::inv(exp()); }
	///	行列式
	element_type det() const { return PTM::det(exp()); }
	//@}
	///@name	ストリーム入出力
	//@{
	///	出力
	void print(std::ostream& os, char* sep="( )") const {
//		os << "sz:" << height() << "," << width() << std::endl;
		int w = os.width();
		os.width(0);
		for(size_t i=0; i<height(); ++i){
			for(int j=0; j<w; ++j){
				if (sep[1]) os << sep[1];
			}
			if (i == 0){
				if(sep[0]) os << sep[0];
			}else{
				if(sep[1]) os << sep[1];
			}
			row(i).print(os);
			if (i==height()-1){
				if(sep[2]) os << sep[2];
			}
			os << std::endl;
		}
		os.width(w);
	}
	///	入力
	void input(std::istream& is){
		char ch;
		is >> ch;
		for(int i=0; i<height(); ++i) is >> row(i);
		is >> ch;
	}
	//@}
protected:
	///	バッファの初期化．
	void init_buffer(){}
	///	デフォルトコンストラクタが呼ぶ．
	void set_default(){}
};

/**	MatrixImp型派生クラスに必要なメンバの定義.
	派生クラスを作るたびに，このマクロを使ってメンバを作る.
	@param	THIS	新たに宣言する派生クラスの型名.
	@see	MatrixImp	*/

#define DEF_MATRIXD_BASIC_MEMBER(THIS)										\
	typedef THIS						this_type;							\
	typedef TYPENAME desc::exp_type				exp_type;					\
	typedef TYPENAME desc::ret_type				ret_type;					\
	typedef TYPENAME desc::row_vector_ref		row_vector_ref;				\
	typedef TYPENAME desc::const_row_vector_ref	const_row_vector_ref;		\
	typedef TYPENAME desc::col_vector_ref		col_vector_ref;				\
	typedef TYPENAME desc::const_col_vector_ref	const_col_vector_ref;		\
	typedef TYPENAME desc::element_type			element_type;				\
	typedef TYPENAME desc::zero					zero;						\
	typedef TYPENAME desc::unit					unit;						\
	typedef TYPENAME desc::trans_ref			trans_ref;					\
	typedef TYPENAME desc::const_trans_ref		const_trans_ref;			\
	/*  行列 b を代入	*/													\
	template <class B>														\
	THIS& operator =(const PTM::MatrixImp<B>& b){							\
		assign(b); return *this;											\
	}																		\
	THIS& operator =(const THIS& b){										\
		assign(b); return *this;											\
	}																		\
	/**	+=演算子(*this = *this + b).										\
		@param	b	次元が等しい行列	*/									\
	template <class B>														\
	this_type& operator +=(const PTM::MatrixImp<B>& b){						\
		add(b); return *this;												\
	}																		\
	/**	-=演算子(*this = *this - b). @param	b	次元が等しい行列	*/		\
	template <class B>														\
	this_type& operator -=(const PTM::MatrixImp<B>& b){						\
		sub(b); return *this;												\
	}																		\
	/**	- 演算子 (return -*this).	*/										\
	ret_type operator- () { ret_type r(*this); r*=-1; return r; }			\
	/**	*=演算子(*this = b * *this). @param	b	要素型	*/					\
	this_type operator*= (element_type b){									\
		multi(b);															\
		return *this;														\
	}																		\
	/**	/=演算子(*this = *this / b). @param	b	要素型	*/					\
	this_type operator/= (element_type b){									\
		div(b);																\
		return *this;														\
	}																		\

#define DEF_MATRIX_BASIC_MEMBER(THIS)										\
	DEF_MATRIXD_BASIC_MEMBER(THIS)											\
	/*	デフォルトコンストラクタ	*/										\
	THIS(){ this->init_buffer(); this->set_default();} 			\
	/*  行列 b による初期化		*/											\
	template <class B>														\
	THIS(const PTM::MatrixImp<B>& b){ this->init_buffer(); this->assign(b);}	\

//----------------------------------------------------------------------------
//	次元をテンプレートで持つベクトル	T???Matrix
//
///	次元をテンプレートで持つベクトルの基本型
template<DIMTYPE H, DIMTYPE W, class D>
class TMatrixBaseBase: public MatrixImp<D> {
protected:
	///	バッファの初期化は不用
	void init_buffer(){};
	///	継承専用
	TMatrixBaseBase(){} 
public:
	DIMDEF(H, HEIGHT);			///<	行数
	DIMDEF(W, WIDTH);			///<	列数
	DIMDEF(D::STRIDE, STRIDE);	///<	列(行)の間隔
	typedef D desc;
	typedef MatrixImp<desc> base_type;
	DEF_MATRIXD_BASIC_MEMBER(TMatrixBaseBase);

	///	行数
	size_t height_impl() const { return DIMDEC(H); }
	///	列数
	size_t width_impl() const { return DIMDEC(W); }
	size_t stride_impl() const { return DIMDEC(D::STRIDE); }
	void resize_impl(size_t h, size_t w) { assert(h==this->height() && w==this->width()); }
	///	行ベクトル
	row_vector_ref row_impl(size_t n){ return (row_vector_ref)this->item(n,0); }
	const_row_vector_ref row_impl(size_t n) const { return (row_vector_ref)this->item(n,0); }
	///	列ベクトル
	col_vector_ref col_impl(size_t m){ return (col_vector_ref)this->item(0,m); }
	const_col_vector_ref col_impl(size_t m) const { return (col_vector_ref)this->item(0,m); }
	///	転置
	trans_ref trans_impl() { return (trans_ref)this->item(0,0); }
	const_trans_ref trans_impl() const { return (const_trans_ref)this->item(0,0); }
};
template<DIMTYPE H, DIMTYPE W, class D>
class TMatrixBase: public TMatrixBaseBase<H,W,D> {
public:
	DIMDEF(H, HEIGHT);			///<	行数
	DIMDEF(W, WIDTH);			///<	列数
	DIMDEF(D::STRIDE, STRIDE);	///<	列(行)の間隔
	typedef D desc;
	typedef TMatrixBaseBase<H,W,D> base_type;
	DEF_MATRIXD_BASIC_MEMBER(TMatrixBase);
};

template <class EXP, class TRANS, size_t H, size_t W, size_t STR, class T, class Z=T, class U=Z>
class TMatrixDescBase{
public:
	DIMDEF(DIMENC(STR),				STRIDE);				///<	行列バッファの幅
	typedef EXP						exp_type;				///<	実体
	typedef exp_type				ret_type;				///<	返り値型
	typedef T						element_type;			///<	要素の型
	typedef Z						zero;					///<	zero(0)が 0 を返す型
	typedef U						unit;					///<	unit(1)が 1 を返す型
	typedef TRANS					trans_type;				///<	転地行列の型
	typedef trans_type&				trans_ref;				///<	転地行列の参照型
	typedef const trans_type&		const_trans_ref;		///<	転地行列の参照型
};
template <class EXP, class TRANS, size_t H, size_t W, size_t STR, class T, class Z=T, class U=Z>
class TMatrixDescRow: public TMatrixDescBase<EXP,TRANS,H,W,STR,T,Z,U>{
public:
	typedef TMakeSubMatrixRow< TMatrixDescRow<EXP,TRANS,H,W,STR,T,Z,U> > make_sub_matrix;
	typedef TVector<W,T>			row_vector_type;		///<	行ベクトル型
	typedef row_vector_type&		row_vector_ref;			///<	行ベクトルの参照
	typedef const row_vector_type&	const_row_vector_ref;	///<	行ベクトルの参照
	typedef TVectorSlice<H,STR,TVector<H*STR,T> >
									col_vector_type;		///<	列ベクトル型
	typedef col_vector_type&		col_vector_ref;			///<	列ベクトルの参照
	typedef const col_vector_type&	const_col_vector_ref;	///<	列ベクトルの参照
};
template <class EXP, class TRANS, size_t H, size_t W, size_t STR, class T, class Z=T, class U=Z>
class TMatrixDescCol: public TMatrixDescBase<EXP,TRANS,H,W,STR,T,Z,U>{
public:
	typedef TMakeSubMatrixCol< TMatrixDescCol<EXP,TRANS,H,W,STR,T,Z,U> > make_sub_matrix;
	typedef TVectorSlice<W,STR,TVector<W*STR,T> >
									row_vector_type;		///<	行ベクトル型
	typedef row_vector_type&		row_vector_ref;			///<	行ベクトルの参照
	typedef const row_vector_type&	const_row_vector_ref;	///<	行ベクトルの参照
	typedef TVector<H,T>			col_vector_type;		///<	列ベクトル型
	typedef col_vector_type&		col_vector_ref;			///<	列ベクトルの参照
	typedef const col_vector_type&	const_col_vector_ref;	///<	列ベクトルの参照
};
template <class EXP, class TRANS, size_t H, size_t W, class OD>
class TSubMatrixDescRow: public TMatrixDescRow<EXP, TRANS, H, W, DIMDEC(OD::STRIDE), TYPENAME OD::element_type, TYPENAME OD::zero, TYPENAME OD::unit>{
public:
	typedef TMakeSubMatrixRow< TSubMatrixDescRow<EXP,TRANS,H,W,OD> > make_sub_matrix;
};
template <class EXP, class TRANS, size_t H, size_t W, class OD>
class TSubMatrixDescCol: public TMatrixDescCol<EXP, TRANS, H, W, DIMDEC(OD::STRIDE), TYPENAME OD::element_type, TYPENAME OD::zero, TYPENAME OD::unit>{
public:
	typedef TMakeSubMatrixCol< TSubMatrixDescCol<EXP,TRANS,H,W,OD> > make_sub_matrix;
};

template <size_t H, size_t W, class T, class Z=T, class U=Z> class TMatrixCol;
/**	行列型. TMatrixRow<3,3, float> m; のように使う
	@param	H	行数．
	@param	W	列数．
	@param	T	要素の型.	*/
template <size_t H, size_t W, class T, class Z=T, class U=Z>
class TMatrixRow:public TMatrixBase<DIMENC(H), DIMENC(W), TMatrixDescRow<TMatrixRow<H,W,T,Z,U>, TMatrixCol<W,H,T,Z,U>, H, W, W, T, Z, U> >{
public:
	///	型情報
	typedef TMatrixDescRow<TMatrixRow<H,W,T,Z,U>, TMatrixCol<W,H,T,Z,U>, H, W, W, T, Z, U> desc;
	typedef TMatrixBase<DIMENC(H),DIMENC(W),desc> base_type;		///<	基本クラス型
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TMatrixRow);

public:
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[i][j]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[i][j]; }
private:
	element_type data[H][W];										///<	データ
};

/**	列行列型. TMatrixCol<3,3, float> m; のように使う
	@param	H	行数．
	@param	W	列数．
	@param	T	要素の型.	*/
template <size_t H, size_t W, class T, class Z, class U>
class TMatrixCol:public TMatrixBase<DIMENC(H), DIMENC(W), TMatrixDescCol<TMatrixCol<H,W,T,Z,U>, TMatrixRow<W,H,T,Z,U>, H,W,H,T,Z,U> >{
public:
	///	型情報
	typedef TMatrixDescCol<TMatrixCol<H,W,T,Z,U>, TMatrixRow<H,W,T,Z,U>, H, W, H, T, Z, U> desc;
	typedef TMatrixBase<DIMENC(H),DIMENC(W),desc> base_type;		///<	基本クラス型
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TMatrixCol);

public:
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[j][i]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[j][i]; }
private:
	element_type data[W][H];				///<	データ
};

template <size_t H, size_t W, class OD> class TSubMatrixCol;

///	部分行列(テンプレート版)
template <size_t H, size_t W, class OD>
class TSubMatrixRow:public TMatrixBase<DIMENC(H),DIMENC(W),TSubMatrixDescRow<TSubMatrixRow<H,W,OD>,TSubMatrixCol<W,H,OD>, H,W,OD> >{
public:
	typedef TSubMatrixDescRow<TSubMatrixRow<H,W,OD>,TSubMatrixCol<W,H,OD>, H,W,OD> desc;
	typedef TMatrixBase<DIMENC(H),DIMENC(W),desc> base_type;
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TSubMatrixRow);
    DIMDEF(base_type::HEIGHT, HEIGHT);
    DIMDEF(base_type::WIDTH, WIDTH);

	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[i][j]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[i][j]; }
protected:
	element_type data[DIMDEC(HEIGHT)][DIMDEC(base_type::STRIDE)];
};

///	部分行列(テンプレート版)
template <size_t H, size_t W, class OD>
class TSubMatrixCol:public TMatrixBase<DIMENC(H),DIMENC(W),TSubMatrixDescCol<TSubMatrixCol<H,W,OD>,TSubMatrixRow<W,H,OD>, H,W,OD> >{
public:
	typedef TSubMatrixDescCol<TSubMatrixCol<H,W,OD>,TSubMatrixRow<W,H,OD>, H,W,OD> desc;
	typedef TMatrixBase<DIMENC(H),DIMENC(W),desc> base_type;
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(TSubMatrixCol);
    DIMDEF(base_type::HEIGHT, HEIGHT);
    DIMDEF(base_type::WIDTH, WIDTH);

	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return data[j][i]; }
	const element_type& item_impl(size_t i, size_t j) const { return data[j][i]; }
protected:
	element_type data[DIMDEC(WIDTH)][DIMDEC(base_type::STRIDE)];
};

//-----------------------------------------------------------------------------
//	サイズを変数で持つベクトル
//

template <class T> class EMatrixRow;
template <class T> class VMatrixRow;
template <class T> class EMatrixCol;
template <class T> class VMatrixCol;
template <class EXP, class TRANS, class T, class Z=T, class U=Z> class EMatrixDescBase{
public:
	typedef EXP							exp_type;				///<	実体
	typedef T							element_type;			///<	要素の型
	typedef Z							zero;					///<	zero(0)が 0 を返す型
	typedef U							unit;					///<	unit(1)が 1 を返す型
	typedef	TRANS						trans_type;				///<	転地行列
	typedef trans_type					trans_ref;				///<	転地行列の参照型
	typedef trans_type					const_trans_ref;		///<	転地行列の参照型
};
template <class EXP, class TRANS, class T, class Z=T, class U=Z>
class EMatrixDescRow: public EMatrixDescBase<EXP, TRANS, T, Z, U>{
public:
	typedef VMatrixRow<T>							ret_type;				///<	返り値型
	typedef EVector<T>								row_vector_type;		///<	行ベクトル型
	typedef row_vector_type							row_vector_ref;			///<	列ベクトルの参照
	typedef TYPENAME row_vector_type::const_type	const_row_vector_ref;	///<	行ベクトルの参照
	typedef EVectorSlice<T>							col_vector_type;		///<	列ベクトル型
	typedef col_vector_type							col_vector_ref;			///<	行ベクトルの参照
	typedef TYPENAME col_vector_type::const_type	const_col_vector_ref;	///<	列ベクトルの参照
	typedef EMakeSubMatrixRow< EMatrixDescRow<EXP, TRANS, T, Z, U> > make_sub_matrix;
};
template <class EXP, class TRANS, class T, class Z=T, class U=Z>
class EMatrixDescCol: public EMatrixDescBase<EXP, TRANS, T, Z, U>{
public:
	typedef VMatrixCol<T>							ret_type;				///<	返り値型
	typedef EVectorSlice<T>							row_vector_type;		///<	行ベクトル型
	typedef row_vector_type							row_vector_ref;			///<	列ベクトルの参照
	typedef TYPENAME row_vector_type::const_type	const_row_vector_ref;	///<	行ベクトルの参照
	typedef EVector<T>								col_vector_type;		///<	列ベクトル型
	typedef col_vector_type							col_vector_ref;			///<	行ベクトルの参照
	typedef TYPENAME col_vector_type::const_type	const_col_vector_ref;	///<	列ベクトルの参照
	typedef EMakeSubMatrixCol< EMatrixDescCol<EXP, TRANS, T, Z, U> > make_sub_matrix;
};

///	サイズを変数でもつ行列の基本型
template <class D>
class EMatrixBase:public MatrixImp<D>{
public:
	///	型情報
	typedef D desc;
	typedef MatrixImp<desc> base_type;		///<	基本クラス型
	DEF_MATRIXD_BASIC_MEMBER(EMatrixBase);

	size_t height_impl() const { return height_; }
	size_t width_impl() const { return width_; }
		
	///	行ベクトル
	row_vector_ref row_impl(size_t n){
		return row_vector_ref(this->width(), 1, &this->item(n,0)); 
	}
	const_row_vector_ref row_impl(size_t n) const {
		return const_row_vector_ref(this->width(), 1, &this->item(n,0));
	}
	///	列ベクトル
	col_vector_ref col_impl(size_t m){
		return col_vector_ref(this->height(), this->stride(), &this->item(0,m));
	}
	const_col_vector_ref col_impl(size_t m) const {
		return const_col_vector_ref(this->height(), this->stride(), &this->item(0,m));
	}
	///	転置
	trans_ref trans_impl(){
		return trans_ref(this->width(), this->height(), this->stride(), &this->item(0,0));
	}
	const_trans_ref trans_impl() const {
		return const_trans_ref(this->width(), this->height(), this->stride(), &this->item(0,0));
	}
	///
	void resize_impl(size_t h, size_t w) { assert(h==height_ && w==width_);}

protected:
	size_t height_;
	size_t width_;
	element_type* data;						///<	データ
	void init_buffer(){height_=0; width_=0; data=0; }
	EMatrixBase(size_t h, size_t w, const element_type* d):height_(h), width_(w), data((element_type*)d){}
	EMatrixBase():height_(0), width_(0), data(0){}
};

///	サイズを変数でもつ行列の基本型
template <class D>
class EMatrixBaseRow:public EMatrixBase<D>{
protected:
	EMatrixBaseRow(){}
	EMatrixBaseRow(const EMatrixBaseRow& m): EMatrixBase<D>(m){}
public:
	typedef D desc;
	typedef EMatrixBase<D> base_type;
	DEF_MATRIXD_BASIC_MEMBER(EMatrixBaseRow)
	EMatrixBaseRow(size_t h, size_t w, const element_type* d):EMatrixBase<D>(h,w,d){}
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return this->data[i*this->stride()+j]; }
	const element_type& item_impl(size_t i, size_t j) const { return this->data[i*this->stride()+j]; }
	///	ストライド
	size_t stride_impl() const { return this->width_; }
	///	行ベクトル
	row_vector_ref row_impl(size_t n){
		return row_vector_ref(this->width(), 1, &this->item(n,0));
	}
	const_row_vector_ref row_impl(size_t n) const {
		return const_row_vector_ref(this->width(), 1, &this->item(n,0));
	}
	///	列ベクトル
	col_vector_ref col_impl(size_t m){
		return col_vector_ref(this->height(), this->stride(), &this->item(0,m));
	}
	const_col_vector_ref col_impl(size_t m) const {
		return const_col_vector_ref(this->height(), this->stride(), &this->item(0,m));
	}
};

///	サイズを変数でもつ行列の基本型
template <class D>
class EMatrixBaseCol:public EMatrixBase<D>{
protected:
	EMatrixBaseCol(){}
	EMatrixBaseCol(const EMatrixBaseCol& m): EMatrixBase<D>(m){}
public:
	typedef D desc;
	typedef EMatrixBase<D> base_type;
	DEF_MATRIXD_BASIC_MEMBER(EMatrixBaseCol)
	EMatrixBaseCol(size_t h, size_t w, const element_type* d):EMatrixBase<D>(h,w,d){}
	///	要素のアクセス
	element_type& item_impl(size_t i, size_t j){ return this->data[j*this->stride()+i]; }
	const element_type& item_impl(size_t i, size_t j) const { return this->data[j*this->stride()+i]; }
	///	ストライド
	size_t stride_impl() const { return this->height_; }
	///	行ベクトル
	row_vector_ref row_impl(size_t n){
		return row_vector_ref(this->width(), this->stride(), &this->item(n,0));
	}
	const_row_vector_ref row_impl(size_t n) const {
		return const_row_vector_ref(this->width(), this->stride(), &this->item(n,0));
	}
	///	列ベクトル
	col_vector_ref col_impl(size_t m){
		return col_vector_ref(this->height(), 1, &this->item(0,m));
	}
	const_col_vector_ref col_impl(size_t m) const {
		return const_col_vector_ref(this->height(), 1, &this->item(0,m));
	}
};

/**	行列型. EMatrixRow<float> m(3,3,buf); のように使う
	@param	T	要素の型.	*/
template <class T>
class EMatrixRow:public EMatrixBaseRow< EMatrixDescRow<EMatrixRow<T>, EMatrixCol<T>, T> >{
public:
	///	型情報
	typedef EMatrixDescRow<EMatrixRow<T>, EMatrixCol<T>, T> desc;
	typedef EMatrixBaseRow<desc> base_type;		///<	基本クラス型
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(EMatrixRow);
	EMatrixRow(const EMatrixRow& m):base_type(m.height_, m.width_,m.data){}
	EMatrixRow(size_t h, size_t w, size_t str, const element_type* d):base_type(h,w,d){ assert(str == w); }
};

/**	行列型. VMatrixRow<float> m(3,3); のように使う
	@param	T	要素の型.	*/
template <class T>
class VMatrixRow:public EMatrixBaseRow< EMatrixDescRow<VMatrixRow<T>, EMatrixCol<T>, T> >{
public:
	///	型情報
	typedef EMatrixDescRow<VMatrixRow<T>, EMatrixCol<T>, T> desc;
	typedef EMatrixBaseRow<desc> base_type;		///<	基本クラス型
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(VMatrixRow);
	VMatrixRow(const VMatrixRow& m){ this->init_buffer(); this->assign(m); }
	~VMatrixRow(){ delete [] this->data; }
	///
	void resize_impl(size_t h, size_t w) {
		if (this->height()*this->width()<h*w){
			delete [] this->data;
			this->data = new T[h*w];
			this->height_ = h;
			this->width_ = w;
		}
	}
};

/**	行列型. EMatrixCol<float> m(3,3,buf); のように使う
	@param	T	要素の型.	*/
template <class T>
class EMatrixCol:public EMatrixBaseCol< EMatrixDescCol<EMatrixCol<T>, EMatrixRow<T>, T> >{
public:
	///	型情報
	typedef EMatrixDescCol<EMatrixCol<T>, EMatrixRow<T>, T> desc;
	typedef EMatrixBaseCol<desc> base_type;		///<	基本クラス型
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(EMatrixCol);
	EMatrixCol(const EMatrixCol& m):base_type(m.height_, m.width_, m.data){}
	EMatrixCol(size_t h, size_t w, size_t str, const element_type* d):base_type(h,w,d){ assert(str == h); }
};

/**	行列型. VMatrixCol<float> m(3,3); のように使う
	@param	T	要素の型.	*/
template <class T>
class VMatrixCol:public EMatrixBaseCol< EMatrixDescCol<VMatrixCol<T>, EMatrixRow<T>, T> >{
public:
	///	型情報
	typedef EMatrixDescCol<VMatrixCol<T>, EMatrixRow<T>, T> desc;
	typedef EMatrixBaseCol<desc> base_type;		///<	基本クラス型
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(VMatrixCol);
	VMatrixCol(const VMatrixCol& m){ this->init_buffer(); this->assign(m); }
	~VMatrixCol(){ delete [] this->data; }
	///
	void resize_impl(size_t h, size_t w) {
		if (this->height()*this->width()<h*w){
			delete [] this->data;
			this->data = new T[h*w];
		}
		this->height_ = h;
		this->width_ = w;
	}
};

template <class T, class Z, class U> class ESubMatrixCol;
///	部分行列(変数版)
template <class T, class Z, class U>
class ESubMatrixRow:public EMatrixBaseRow< EMatrixDescRow<ESubMatrixRow<T>, ESubMatrixCol<T>, T,Z,U> >{
public:
	typedef EMatrixDescRow<ESubMatrixRow<T>, ESubMatrixCol<T>, T,Z,U> desc;
	typedef EMatrixBaseRow<desc> base_type;
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(ESubMatrixRow);
	///
	ESubMatrixRow(size_t h, size_t w, size_t str, const element_type* d):base_type(h, w, d), stride_(str){}
	///
	ESubMatrixRow(const ESubMatrixRow& m):base_type(m.height_, m.width_, m.data), stride_(m.stride_){}
	///
	size_t stride_impl() const { return stride_; }
protected:
	size_t stride_;
	void init_buffer() { this->height_=0; this->width_=0; this->stride_=0; this->data=0; }
};

///	部分行列(変数版)
template <class T, class Z, class U>
class ESubMatrixCol:public EMatrixBaseCol< EMatrixDescCol<ESubMatrixCol<T>, ESubMatrixRow<T>, T,Z,U> >{
public:
	typedef EMatrixDescCol<ESubMatrixCol<T>, ESubMatrixRow<T>, T,Z,U> desc;
	typedef EMatrixBaseCol<desc> base_type;
	///	継承されない基本的なメンバの定義. @see ::DEF_MATRIX_BASIC_MEMBER
	DEF_MATRIX_BASIC_MEMBER(ESubMatrixCol);
	///
	ESubMatrixCol(size_t h, size_t w, size_t str, const element_type* d):base_type(h,w,d), stride_(str){}
	///
	ESubMatrixCol(const ESubMatrixCol& m):base_type(m.height_, m.width_, m.data), stride_(m.stride_){}
	///
	size_t stride_impl() const { return stride_; }
protected:
	size_t stride_;
	void init_buffer() { this->height_=0; this->width_=0; this->stride_=0; this->data=0; }
};


//----------------------------------------------------------------------------
///	@name 行列の演算子
//@{
///	行列の比較
template <class AD, class BD>
bool operator == (const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	return a.equal(b);
}
///	行列の比較
template <class AD, class BD>
bool operator != (const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	return !a.equal(b);
}
///	行列の和
template <class AD, class BD>
TYPENAME AD::ret_type operator + (const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	TYPENAME AD::ret_type r(a);
	r.add(b);
	return r;
}
///	行列の差
template <class AD, class BD>
TYPENAME AD::ret_type operator - (const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	TYPENAME AD::ret_type r(a);
	r.sub(b);
	return r;
}
///	行列の掛け算	サイズ：テンプレート
template <DIMTYPE AH, DIMTYPE AW, class AD, DIMTYPE BW, class BD>
TMatrixCol<DIMDEC(AH), DIMDEC(BW), TYPENAME AD::element_type> operator * (
	const TMatrixBase<AH, AW, AD>& a,
	const TMatrixBase<AW, BW, BD>& b){
	TMatrixCol<DIMDEC(AH), DIMDEC(BW), TYPENAME AD::element_type> r;
	multi(r, a, b);
	return r;
}
///	行列の掛け算	サイズ：変数
template <class AD, class BD>
VMatrixCol<TYPENAME AD::element_type> operator * (const MatrixImp<AD>& a, const MatrixImp<BD>& b){
	VMatrixCol<TYPENAME AD::element_type> r;
	multi(r, a, b);
	return r;
}

//	ベクトルと行列の掛け算
///	ベクトルと行列の掛け算	サイズ：テンプレート
template <DIMTYPE AH, DIMTYPE AW, class AD, class BD>
TVector<DIMDEC(AH), TYPENAME AD::element_type> operator * (const TMatrixBase<AH, AW, AD>& a, const VectorImp<BD>& b){
	TVector<DIMDEC(AH), TYPENAME AD::element_type> r;
	multi(r, a, b);
	return r;
}
///	ベクトルと行列の掛け算	サイズ：変数
template <class AD, class BD>
VVector<TYPENAME BD::element_type> operator * (const MatrixImp<AD>& a, const VectorImp<BD>& b){
	VVector<TYPENAME BD::element_type> r;
	multi(r, a, b);
	return r;
}
///	ベクトルと行列の掛け算	サイズ：テンプレート
template <class AD, DIMTYPE BH, DIMTYPE BW, class BD>
TVector<DIMDEC(BW), TYPENAME AD::element_type> operator * (const VectorImp<AD>& a, const TMatrixBase<BH, BW, BD>& b){
	TVector<DIMDEC(BW), TYPENAME AD::element_type> r;
	multi(r, b.trans(), a);
	return r;
}
///	ベクトルと行列の掛け算	サイズ：変数
template <class AD, class BD>
VVector<TYPENAME AD::element_type> operator * (const VectorImp<AD>& a, const MatrixImp<BD>& b){
	VVector<TYPENAME AD::element_type> r;
	multi(r, b.trans(), a);
	return r;
}
///	行列の定数倍
template <class D>
TYPENAME D::ret_type operator * (const MatrixImp<D>& a, TYPENAME D::element_type b){
	TYPENAME D::ret_type r(a);
	r.multi(b);
	return r;
}
///	行列の定数倍
template <class D>
TYPENAME D::ret_type operator * (TYPENAME D::element_type a, const MatrixImp<D>& b){
	TYPENAME D::ret_type r(b);
	r.multi(a);
	return r;
}

///	表示
template <class D>
std::ostream& operator << (std::ostream& os, const MatrixImp<D>& m){
	m.print(os);
	return os;
}
///	入力
template <class D>
std::istream& operator >> (std::istream& is, MatrixImp<D>& m){
	m.input(is);
	return is;
}
//@}

#ifdef _WIN32
 #ifdef _DEBUG
  #pragma optimize ("", on)
  #pragma auto_inline(off)
  #pragma inline_recursion(off)
 #endif
 #pragma pack(pop)
#endif

}	//	namespace PTM
#endif

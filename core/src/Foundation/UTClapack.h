#ifndef UT_CLAPACK_H
#define UT_CLAPACK_H

#include <SprDefs.h>

#include <float.h>  // DBL_MIN DBL_MAX

#ifdef USE_LAPACK
  #include "ClapackLibsDef.h"
#endif /* USE_LAPACK */

/*
/ リンクするlibファイルは_cdeclで呼び出し
/ _fastcallでリンクしたい場合はpringhead2\src\boost\numeric\bindings\lapack\lapack.h
/ の関数をすべて_cdecl呼び出しにすること
*/

#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <boost/numeric/ublas/triangular.hpp>

#include <boost/numeric/ublas/matrix_sparse.hpp>

#define ublas boost::numeric::ublas
#define bindings boost::numeric::bindings
#define lapack bindings::lapack

namespace Spr{;

/** @brief 一般化固有値問題を解くAx = lamda Bx (inv(B)Ax = lamda x)
	@param a 対称行列
	@param b 対称行列(aと同サイズ)
	@param e 固有値をまとめたベクトル(必要な分だけ）
	@param v 固有ベクトルをまとめたベクトル(aのサイズ、eのサイズ)
	@return 計算に成功したかどうか。0:成功、それ以外:失敗
*/
template <class AD, class BD> 
int sprsygv(const PTM::MatrixImp<AD>& a, const PTM::MatrixImp<AD>& b, PTM::VectorImp<BD>& e, PTM::MatrixImp<AD>& v){
#ifdef USE_LAPACK
	e.clear();
	v.clear();
	int size = a.height();
	assert(a.width() == size);
	assert(b.height() == size || b.width() == size);
	assert(e.size() == size);
	assert(v.height() == size || b.width() == size);

	typedef TYPENAME AD::element_type element_type;
	typedef typename bindings::remove_imaginary<element_type>::type real_type ;
	typedef ublas::vector< real_type > vector_type;
	typedef ublas::matrix< element_type, ublas::column_major > matrix_type;

	vector_type evec(size);
	matrix_type la(size, size), lb(size, size);

	// 行列の下三角を使う
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j <= i; ++j) {
			la(i, j) = a.item(i,j);
			lb(i, j) = b.item(i,j);
		}
	}
	ublas::symmetric_adaptor< matrix_type, ublas::lower> la_uplo( la ) ;
	ublas::symmetric_adaptor< matrix_type, ublas::lower> lb_uplo( lb ) ;
	int info = lapack::sygv(1,'V',la_uplo, lb_uplo, evec);

	int esize = e.size();
	for(int i = 0; i < esize; i++)
		e.item(i) = evec[i];

	for(int i = 0; i < size; i++)
		for(int j = 0; j < esize; j++)
			v.item(i, j) = la(i, j);
	return info;
#else
# pragma message("sprsygv: define USE_LAPACK in SprDefs.h to use this function")
	return 0;
#endif
}

/** @brief 一般化固有値問題（選択分）を解くAx = lamda Bx (inv(B)Ax = lamda x)
	@param a 対称行列
	@param b 対称行列(aと同サイズ)
	@param e 固有値をまとめたベクトル(選択した数分）
	@param v 固有ベクトルをまとめたベクトル(aのサイズ、選択した数分)
	@param start 固有値のインデックス(最小は0)
	@param interval startからの固有値の数(最大aのサイズ）
	@return 計算に成功したかどうか。0:成功、それ以外:失敗
*/
template <class AD, class BD> 
int sprsygvx(const PTM::MatrixImp<AD>& a, const PTM::MatrixImp<AD>& b, PTM::VectorImp<BD>& e, PTM::MatrixImp<AD>& v, int start, int interval){
#ifdef USE_LAPACK
	e.clear();
	v.clear();
	int size = (int)a.height();
	assert((int)a.width() == size);
	assert((int)b.height() == size || (int)b.width() == size);
	assert((int)e.size() == interval);
	assert((int)v.height() == size || (int)b.width() == interval);

	typedef TYPENAME AD::element_type element_type;
	typedef typename bindings::remove_imaginary<element_type>::type real_type ;
	typedef ublas::vector< real_type > vector_type;
	typedef ublas::matrix< element_type, ublas::column_major > matrix_type;
	matrix_type la(size, size), lb(size, size); 
	// 行列の下三角を使う
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j <= i; ++j) {
			la(i, j) = a.item(i,j);
			lb(i, j) = b.item(i,j);
		}
	}
	ublas::symmetric_adaptor< matrix_type, ublas::lower> la_uplo( la ) ; 
	ublas::symmetric_adaptor< matrix_type, ublas::lower> lb_uplo( lb ) ;

	real_type vl = DBL_MIN;				// 求めたい固有値の下限と上限
	real_type vu = DBL_MAX;		
	int il = start + 1;					// 求めたい固有値の最小インデックス（最小は1)
	int iu = start + interval;			// 求めたい固有値の最大インデックス（最大はsize)
	real_type abstol = 0.0;				// 許容誤差
	int m = 0;							// 求められた固有値の数
	vector_type w(size);				// 固有値ベクトル
	matrix_type z(size, interval);		// 固有ベクトル
	ublas::vector< int > ifail(size);	// 固有値を求めた際の収束に関する情報
	int info = lapack::sygvx(1, 'V', 'I', la_uplo, lb_uplo, vl, vu, il, iu, abstol, m, w, z, ifail);

	for(int i = 0; i < interval; i++)
		e.item(i) = w[i];

	for(int i = 0; i < size; i++)
		for(int j = 0; j < interval; j++)
			v.item(i, j) = z(i, j);
	return info;
#else
# pragma message("sprsygvx: define USE_LAPACK in SprDefs.h to use this function")
	return 0;
#endif
}

// c.f.) http://d.hatena.ne.jp/blono/20080922/1222049807
inline void svd(const ublas::matrix<double>& A, ublas::matrix<double>& U, ublas::diagonal_matrix<double>& D, ublas::matrix<double>& VT) {
#ifdef USE_LAPACK
	ublas::vector<double> s((std::min)(A.size1(), A.size2()));
    ublas::matrix<double, ublas::column_major> CA(A), CU(A.size1(), A.size1()), CVT(A.size2(), A.size2());
    int info;

    info = (int) lapack::gesdd('A', CA, s, CU, CVT);
    BOOST_UBLAS_CHECK(info == 0, ublas::internal_logic());

    ublas::matrix<double> CCU(CU), CCVT(CVT);
    ublas::diagonal_matrix<double> CD(A.size1(), A.size2());

    for (std::size_t i = 0; i < s.size(); ++i) {
        CD(i, i) = s[i];
    }

	#if BOOST_UBLAS_TYPE_CHECK
		BOOST_UBLAS_CHECK(
			ublas::detail::expression_type_check(ublas::prod(ublas::matrix<double>(ublas::prod(CCU, CD)), CCVT), A),
			ublas::internal_logic()
		);
	#endif

    U.assign_temporary(CCU);
    D.assign_temporary(CD);
    VT.assign_temporary(CCVT);
#else
# pragma message("svd: define USE_LAPACK in SprDefs.h to use this function")
#endif
}

inline void svd_sparse(const ublas::mapped_matrix<double>& A, ublas::matrix<double>& U, ublas::diagonal_matrix<double>& D, ublas::matrix<double>& VT) {
#ifdef USE_LAPACK
	ublas::vector<double> s((std::min)(A.size1(), A.size2()));
	ublas::matrix<double, ublas::column_major> CA(A), CU(A.size1(), A.size1()), CVT(A.size2(), A.size2());
	int info;

	info = (int)lapack::gesdd('A', CA, s, CU, CVT);
	BOOST_UBLAS_CHECK(info == 0, ublas::internal_logic());

	ublas::matrix<double> CCU(CU), CCVT(CVT);
	ublas::diagonal_matrix<double> CD(A.size1(), A.size2());

	for (std::size_t i = 0; i < s.size(); ++i) {
		CD(i, i) = s[i];
	}

#if BOOST_UBLAS_TYPE_CHECK
	BOOST_UBLAS_CHECK(
		ublas::detail::expression_type_check(ublas::prod(ublas::matrix<double>(ublas::prod(CCU, CD)), CCVT), A),
		ublas::internal_logic()
	);
#endif

	U.assign_temporary(CCU);
	D.assign_temporary(CD);
	VT.assign_temporary(CCVT);
#else
# pragma message("svd: define USE_LAPACK in SprDefs.h to use this function")
#endif
}

// c.f.) http://yano.hatenadiary.jp/entry/20080923/1222126757
inline void qr(const ublas::matrix<double>& A, ublas::matrix<double>& Q, ublas::triangular_matrix<double, ublas::upper>& R) {
#ifdef USE_LAPACK
	BOOST_UBLAS_CHECK(A.size1() >= A.size2(), ublas::external_logic());

	ublas::vector<double>   tau(A.size2());
	ublas::matrix<double, ublas::column_major> CQ(A);
	int info;

	info = lapack::geqrf(CQ, tau);
	BOOST_UBLAS_CHECK(info == 0, ublas::internal_logic());

	ublas::triangular_matrix<double, ublas::upper> CR = ublas::triangular_adaptor<const ublas::matrix_range<ublas::matrix<double, ublas::column_major> >, ublas::upper>(
		ublas::project(CQ, ublas::range(0, A.size2()), ublas::range(0, A.size2()))
		);

	info = lapack::orgqr(CQ, tau);
	BOOST_UBLAS_CHECK(info == 0, ublas::internal_logic());

#if BOOST_UBLAS_TYPE_CHECK
	BOOST_UBLAS_CHECK(ublas::detail::expression_type_check(ublas::prod(CQ, CR), A), ublas::internal_logic());
#endif

	ublas::matrix<double> CCQ(CQ);

	Q.assign_temporary(CCQ);
	R.assign_temporary(CR);
#else
# pragma message("qr: define USE_LAPACK in SprDefs.h to use this function")
#endif
}

// LQ decomposition (by QR decomposing transpose of given matrix)
inline void lq(const ublas::matrix<double>& A, ublas::triangular_matrix<double, ublas::lower>& L, ublas::matrix<double>& Q) {
#ifdef USE_LAPACK
	ublas::matrix<double, ublas::column_major> At(A.size2(), A.size1());
	ublas::matrix<double> Qt;
	At = ublas::trans(A);
	ublas::triangular_matrix<double, ublas::upper> Lt;
	qr(At, Qt, Lt);
	//L = ublas::triangular_adaptor<const ublas::matrix_range<ublas::matrix<double, ublas::column_major> >, ublas::lower>(ublas::trans(Lt));
	ublas::matrix<double> Qtt = ublas::trans(Qt);
	ublas::triangular_matrix<double, ublas::lower> Ltt = ublas::trans(Lt);
	Q.assign_temporary(Qtt);
	L.assign_temporary(Ltt);
#endif
}

// c.f.) http://yano.hatenadiary.jp/entry/20080923/1222126757
inline void qrnoproj(const ublas::matrix<double>& A, ublas::matrix<double>& Q, ublas::matrix<double>& R) {
#ifdef USE_LAPACK
	BOOST_UBLAS_CHECK(A.size1() >= A.size2(), ublas::external_logic());
	
	ublas::vector<double>   tau(A.size2());
	ublas::matrix<double, ublas::column_major> CQ(A), CR(A);
	int info;
	
	info = lapack::geqrf(CR, tau);
	BOOST_UBLAS_CHECK(info == 0, ublas::internal_logic());
	
	// <!!> これ、Q1しか返さない!!
	info = lapack::orgqr(CQ, tau);
	BOOST_UBLAS_CHECK(info == 0, ublas::internal_logic());

#if BOOST_UBLAS_TYPE_CHECK
	//BOOST_UBLAS_CHECK(ublas::detail::expression_type_check(ublas::prod(CQ, CR), A), ublas::internal_logic());
#endif

	ublas::matrix<double> CCQ(CQ), CCR(CR);

	Q.assign_temporary(CCQ);
	R.assign_temporary(CCR);
#else
# pragma message("qr: define USE_LAPACK in SprDefs.h to use this function")
#endif
}

// LQ decomposition (by QR decomposing transpose of given matrix)
inline void lqnoproj(const ublas::matrix<double>& A, ublas::matrix<double>& L, ublas::matrix<double>& Q) {
#ifdef USE_LAPACK
	ublas::matrix<double> At(A.size2(), A.size1());
	ublas::matrix<double> Qt;
	At = ublas::trans(A);
	ublas::matrix<double> Lt;
	qrnoproj(At, Qt, Lt);
	//L = ublas::triangular_adaptor<const ublas::matrix_range<ublas::matrix<double, ublas::column_major> >, ublas::lower>(ublas::trans(Lt));
	ublas::matrix<double> Qtt = ublas::trans(Qt);
	ublas::matrix<double> Ltt = ublas::trans(Lt);
	Q.assign_temporary(Qtt);
	L.assign_temporary(Ltt);
#endif
}

// c.f.) http://yano.hatenadiary.jp/entry/20080916/1221572201
template<class M>
double determinant(const M& m) {
#ifdef USE_LAPACK
	BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());

	ublas::matrix<double>       lu(m);
	ublas::permutation_matrix<> pm(m.size1());

	ublas::lu_factorize(lu, pm);

	double det(1);

	typedef ublas::permutation_matrix<>::size_type size_type;

	for (size_type i = 0; i < pm.size(); ++i) {
		det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
	}

	return det;
#else
	return 1;
#endif
}
}

#endif

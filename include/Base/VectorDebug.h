/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VECTOR_DEBUG_H
#define VECTOR_DEBUG_H
#include "TMatrix.h"
#include "math.h"

namespace Spr{;

#define FP_DOMAIN_CHECK		//	


#ifdef FP_DOMAIN_CHECK
 #define FPCK_ASSERT(exp)	(void)( (exp) || (FpCheckAssert(#exp, __FILE__, __LINE__), 0) )
 #define FPCK_FINITE(exp)	FpCheckFinite(exp, #exp, __FILE__, __LINE__)
 #define FPCK_NOTZERO(exp)	FpCheckNotZero(exp, #exp, __FILE__, __LINE__)
 #define FPCK_POSITIVE(exp)	FpCheckPositive(exp, #exp, __FILE__, __LINE__)
 #define FPCK_NEGATIVE(exp)	FpCheckNegative(exp, #exp, __FILE__, __LINE__)
 #define FPCK_UNIT(exp)		FpCheckUnit(exp, #exp, __FILE__, __LINE__)
#else
 #define FPCK_ASSERT(exp)	((void)0)
 #define FPCK_FINITE(exp)	((void)0)
 #define FPCK_NOTZERO(exp)	((void)0)
 #define FPCK_POSITIVE(exp)	((void)0)
 #define FPCK_UNIT(exp)		((void)0)
#endif

const double fpCheckExpsilon = 1e-6;

//	浮動小数点とVectorの定義域チェック
inline void FpCheckPrint(const char* f, const char* exp, const char* file, int line){
	DSTR << f << ": '" << exp << "' at " << file << "(" << line << ")" << std::endl;
}
inline void FpCheckAssert(const char* exp, const char* file, int line){
	FpCheckPrint("FpCheckAssert", exp, file, line);
	while(1);
}

template <class D>
bool IsFinite(const PTM::VectorImp<D>& v){
	for(unsigned i=0; i<v.exp().size(); ++i){
		if (!finite(v.exp()[i])) return false;
	}
	return true;
}
template <class D>
bool IsFinite(const PTM::MatrixImp<D>& v){
	for(unsigned i=0; i<v.exp().height(); ++i){
		if ( IsFinite(v.exp().row(i)) ) return false;
	}
	return true;
}
inline bool IsFinite(const double& v){ return finite(v)!=0; }

template <class D>
bool IsNotZero(const PTM::VectorImp<D>& v){
	for(unsigned i=0; i<v.size(); ++i){
		if (!finite(v[i])) return false;
		if (v[i] == 0) return false;
	}
	return true;
}
inline bool IsNotZero(const double& v){
	if (!finite(v)) return false;
	if (v == 0) return false;
	return true;
}

template <class D>
bool IsPositive(const PTM::VectorImp<D>& v){
	for(unsigned i=0; i<v.size(); ++i){
		if ( !(v[i] > 0) ) return false;
	}
	return true;
}
inline bool IsPositive(const double& v){ return v>0; }


template <class D>
bool IsUnit(const PTM::VectorImp<D>& v){
	double len = v.exp().norm();
	if (fpCheckExpsilon > len-1.0 && len-1.0 > -fpCheckExpsilon) return true;
	return false;
}
template <class D>
bool IsUnit(const PTM::MatrixImp<D>& v){
	double len = v.exp().det();
	if (fpCheckExpsilon > len-1.0 && len-1.0 > -fpCheckExpsilon) return true;
	return false;
}
inline bool IsUnit(const double& v){
	if (fpCheckExpsilon > v-1.0 && v-1.0 > -fpCheckExpsilon) return true;
	return false;
}



template <class V>
bool FpCheckFinite(const V& v, const char* exp, const char* file, int line){
	if (IsFinite(v)) return true;
	FpCheckPrint("FpCheckFinite", exp, file, line);
	DSTR << v << std::endl;
	return false;
}
template <class V>
bool FpCheckNotZero(const V& v, const char* exp, const char* file, int line){
	if (IsNotZero(v)) return true;
	FpCheckPrint("FpCheckNotZero", exp, file, line);
	DSTR << v << std::endl;
	return false;
}
template <class V>
bool FpCheckPositive(const V& v, const char* exp, const char* file, int line){
	if (IsPositive(v)) return true;
	FpCheckPrint("FpCheckPositive", exp, file, line);
	DSTR << v << std::endl;
	return false;
}
template <class V>
bool FpCheckUnit(const V& v, const char* exp, const char* file, int line){
	if (IsUnit(v)) return true;
	FpCheckPrint("FpCheckPositive", exp, file, line);
	DSTR << v << std::endl;
	return false;
}


}
#endif

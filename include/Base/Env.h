/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_BASE_ENV_H
#define SPR_BASE_ENV_H

/**	@file Env.h
	コンパイラ間の互換性を取るためのマクロ定義.	*/
#define TYPENAME typename

#if defined __BORLANDC__
 #pragma warn -8026
 #pragma warn -8027
#endif


/**	DLLのエクスポートの指定
	DLLにエクスポートするためには，
	class SPR_DLL DLLCLASS C{}; のように，クラス宣言の前に DLLCLASS をつける．
	DLLを作るときは，EXPORT_DLL をマクロ定義しておく．*/

#ifdef _MSC_VER
 #ifdef EXPORT_DLL
  #define SPR_DLL __declspec( dllexport )
  #pragma warning (disable: 4275 4251)
 #elif defined IMPORT_DLL
  #define SPR_DLL __declspec( dllimport )
 #else
  #define SPR_DLL
 #endif
#else
 #define SPR_DLL
#endif

/**	__cdecl
	Microsoft compiler または Borland C++ compiler の場合は、__cdeclを指定
 */
#if defined _MSC_VER || defined __BORLANDC__
 #define SPR_CDECL		__cdecl
 #define SPR_STDCALL 	__stdcall 
 #define FASTCALL		_fastcall
#else
 #define SPR_CDECL
 #define SPR_STDCALL 
 #define FASTCALL
#endif

/**	hdrstop
	　Microsoft compiler、Borland C++ compiler、 Intel C++ compiler などの場合は、 
	　プリコンパイルヘッダー制御を行う。
 */
#ifndef __GNUC__
# define USE_HDRSTOP
#endif

/** 数値演算範囲エラーのチェック
    VCが提供する関数では一部関数名が異なる。
 */
#ifdef _MSC_VER
# include <float.h>
# define isnan  _isnan
# define finite _finite
#else
# include <cmath>
# define isnan   std::isnan
# define finite  std::isfinite
#endif

//	for Visual C++ 's strange spec of stl.
#if defined _MSC_VER
 #if _MSC_VER <= 1300
  #undef min
  #undef max
  namespace std{
  template <class T>
  T min(T t1, T t2){
  	return t1 < t2 ? t1 : t2; 
  }
  template <class T>
  T max(T t1, T t2){
  	return t1 > t2 ? t1 : t2;
  }
  }
 #else
  #undef min
  #undef max
  #ifndef _MAX
   #define _MIN min
   #define _MAX max
  #endif
 #endif
#endif

//	x64対応
#ifdef _WIN64
typedef unsigned __int64 ulong_ptr;
#else
typedef unsigned long ulong_ptr;
#endif

//	SWIG用
#ifdef SWIGSPR
namespace PTM{
}
namespace Spr{
}
#endif

#endif

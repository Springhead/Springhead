#ifndef SPR_BASE_ENV_H
#define SPR_BASE_ENV_H

/**	@file Env.h
	コンパイラ間の互換性を取るためのマクロ定義.	*/

#if defined _MSC_VER && _MSC_VER <= 1300
 #define for if(0); else for
 #pragma warning (disable: 4786)
 #define TYPENAME
#else
 #define TYPENAME typename
#endif

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
 #define SPR_CDECL	__cdecl
#else
 #define SPR_CDECL
#endif

/**	hdrstop
　　Linuxの場合、「#pragma hdrstop を無視します」という警告が出る。
 */
#if !defined LINUX
 #define HDRSTOP hdrstop
#else
 #define HDRSTOP
#endif

/** 数値演算範囲エラーのチェック
    VCが提供する関数では一部関数名が異なる。
 */
#if defined _MSC_VER
# include <float.h>
# define isnan  _isnan
# define finite _finite
#endif

//	for VC6 class view
#define DOUBLECOLON ::

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

#endif

#ifndef GRBMPLOADER_H
#define GRBMPLOADER_H
#include <Foundation/UTDllLoader.h>

namespace Spr{;

static UTDllLoader dllLoader("loadBmp.dll", ".;$(SPRINGHEAD)\\bin");	//	グローバル変数でローダーを作る．

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpCreate			//	関数名
#define DLLFUNC_STR		"LoadBmpCreate"			//	関数名
#define DLLFUNC_ARGDEF	(const char* arg)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpGetBmp			//	関数名
#define DLLFUNC_STR		"LoadBmpGetBmp"			//	関数名
#define DLLFUNC_ARGDEF	(int h, char* arg)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(h, arg)				//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpRelease			//	関数名
#define DLLFUNC_STR		"LoadBmpRelease"			//	関数名
#define DLLFUNC_ARGDEF	(int arg)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpGetWidth			//	関数名
#define DLLFUNC_STR		"LoadBmpGetWidth"		//	関数名
#define DLLFUNC_ARGDEF		(int arg)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpGetHeight		//	関数名
#define DLLFUNC_STR		"LoadBmpGetHeight"		//	関数名
#define DLLFUNC_ARGDEF		(int arg)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpGetBytePerPixel	//	関数名
#define DLLFUNC_STR		"LoadBmpGetBytePerPixel"	//	関数名
#define DLLFUNC_ARGDEF		(int arg)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	呼び出し規約
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	LoadBmpGetSize			//	関数名
#define DLLFUNC_STR		"LoadBmpGetSize	"		//	関数名
#define DLLFUNC_ARGDEF		(int arg)			//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

}

#endif

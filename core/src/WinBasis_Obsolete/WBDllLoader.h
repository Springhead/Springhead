/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef WBDLLLOADER_H
#define WBDLLLOADER_H
#include "WBWin.h"
#include <base/BaseUtility.h>

namespace Spr {;
/**	DLLをロードするクラス．以下のようにすると，通常のライブラリと同様に
	DLLを動的にロードして使用できる．

使い方：
static WBDllLoader dllLoader("filename.dll");	//	グローバル変数でローダーを作る．

//	int DllFunc(int arg)  の場合
#define DLLFUNC_STR		"funcname"				//	dllの中での関数の名前
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	DllFunc					//	関数名
#define DLLFUNC_ARGDEF	(int arg)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include "WBDllLoaderImp.h"
*/
class SPR_DLL WBDllLoader{
	HMODULE module;
	char dllName[1024];
	char addPath[1024];
public:
	WBDllLoader(){ Init(); }
	WBDllLoader(const char* dllName, const char* addPath=NULL){
		Init();
		Load(dllName, addPath);
	}
	~WBDllLoader(){ Cleanup(); }
	bool Load(const char* dllName=NULL, const char* addPath=NULL);
	void* GetProc(const char* name);
protected:
	void Init();
	void Cleanup();
};

}	//	namespace Spr;

#endif

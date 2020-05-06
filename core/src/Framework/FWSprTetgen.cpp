#ifndef FWSPRTETGEN_H
#define FWSPRTETGEN_H
#include <Base/Affine.h>
#include <Graphics/SprGRMesh.h>
#include <Foundation/UTDllLoader.h>


namespace Spr{;

#if defined(_WIN64)
static UTDllLoader dllLoader("sprTetgen.dll", ".;$(SPRINGHEAD2)\\bin\\win64");	//	グローバル変数でローダーを作る．
#else
static UTDllLoader dllLoader("sprTetgen.dll", ".;$(SPRINGHEAD2)\\bin\\win32");	//	グローバル変数でローダーを作る．
#endif

#define DLLFUNC_CTYPE	__cdecl						//	呼び出し規約
#define DLLFUNC_RTYPE	bool						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	sprTetgen					//	関数名
#define DLLFUNC_STR		"sprTetgen"					//	関数名
#define DLLFUNC_ARGDEF	(int& nVtxsOut, Spr::Vec3d*& vtxsOut, int& nTetsOut, int*& tetsOut, int nVtxsIn, Spr::Vec3d* vtxsIn, int nFacesIn, Spr::GRMeshFace* facesIn, char* option)		//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(nVtxsOut, vtxsOut, nTetsOut, tetsOut, nVtxsIn, vtxsIn, nFacesIn, facesIn, option)						//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl						//	呼び出し規約
#define DLLFUNC_RTYPE	void						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	sprTetgenFree				//	関数名
#define DLLFUNC_STR		"sprTetgenFree"				//	関数名
#define DLLFUNC_ARGDEF	(Vec3d* vtxOut, int* tetsOut)	//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(vtxOut, tetsOut)				//	関数呼び出しの引数
#include <Foundation/UTDllLoaderImpl.h>


};

#endif

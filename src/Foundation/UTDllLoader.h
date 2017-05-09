#ifndef UTDLLLOADER_H
#define UTDLLLOADER_H
#include <Base/BaseUtility.h>

namespace Spr {;
/**	DLLをロードするクラス．以下のようにすると，通常のライブラリと同様に
	DLLを動的にロードして使用できる．

使い方：
static UTDllLoader dllLoader("filename.dll");	//	グローバル変数でローダーを作る．

//	int DllFunc(int arg)  の場合
#define DLLFUNC_STR		"funcname"				//	dllの中での関数の名前
#define DLLFUNC_RTYPE	int						//	返り値の型 voidの場合は定義してはならない．
#define DLLFUNC_NAME	DllFunc					//	関数名
#define DLLFUNC_ARGDEF	(int arg)				//	関数宣言時の引数
#define DLLFUNC_ARGCALL	(arg)					//	関数呼び出しの引数
#include "UTDllLoaderImp.h"
*/
class SPR_DLL UTDllLoader:public UTRefCount{
	void* module;
	char dllName[1024];
	char addPath[1024];
public:
	UTDllLoader(){ Init(); }
	UTDllLoader(const char* dllName, const char* addPath=NULL){
		Init();
		Load(dllName, addPath);
	}
	~UTDllLoader(){ Cleanup(); }
	bool Load(const char* dllName=NULL, const char* addPath=NULL);
	/// DLLが持つ、指定された関数のアドレスを取得する．
	void* GetProc(const char* name);
	bool IsLoaded() {
		return module != NULL;
	}
protected:
	void Init();
	void Cleanup();
};

}	//	namespace Spr;

#endif

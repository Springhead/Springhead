#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H
#include "Env.h"
#include <stdarg.h>
#include <iostream>

/**	@file BaseDebug.h デバッグ用ユーティリティークラス・関数の定義.	*/

/**	デバッグ用 printf 関数.
	@verbatim
	DPF("メッセージ:%s", msg);@endverbatim
	の様に使う．							*/
#define DPF	Spr::DebugPrintf::GetInstance()->FileLine(__FILE__, __LINE__)
/**	デバッグ用 出力ストリーム.
	@verbatim
	DSTR << "メッセージ:" << msg;@endverbatim
	の様に使う．							*/
#define DSTR (Spr::DebugPrintf::GetInstance()->Stream())

#include <assert.h>

namespace Spr {

class SPR_DLL DebugPrintf{
public:
	static DebugPrintf* GetInstance();
	struct SPR_DLL PrintfFunc{
		const char* file;
		int line;
		PrintfFunc(const char* f, int l):file(f), line(l){}
		int SPR_CDECL operator() (const char*, ...);
	};
	PrintfFunc FileLine(const char* f=0, int l=-1){
		return PrintfFunc(f, l);
	}
	std::ostream& Stream();
	static void Set(void (*out)(const char*));
};

#if 0	//	プログラムの動作を詳細に報告させるなら 1
 #define TRACEALL DebugPrintf
#else
 #define TRACEALL (void*)
#endif

#if defined(_DEBUG) && !defined(NO_DEBUG_EVAL)
 #define DEBUG_EVAL(x) x
#else
 #define DEBUG_EVAL(x)
#endif
}	//	namespace Spr

#endif

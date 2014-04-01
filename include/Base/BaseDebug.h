/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H
#include "Env.h"
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <sstream>

/**	@file BaseDebug.h デバッグ用ユーティリティークラス・関数の定義.	*/

#ifndef DBG_NEW
# if defined(_DEBUG)
#  include <crtdbg.h>
#  define _CRTDBG_MAP_ALLOC
///	Debug用 new リーク時に行番号を表示
#  define DBG_NEW  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
# else
#  define DBG_NEW new
# endif
#endif


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
	static DebugPrintf* FASTCALL GetInstance();
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


/**	デバッグ用 CSV出力ストリーム.
	@verbatim
	CSVout << "メッセージ:" << msg;@endverbatim
	の様に使う．							*/
#define CSVOUT (Spr::DebugCSV::GetInstance()->Stream())
/**	デバッグ用 CSV出力ストリーム.
	現在開いているファイルをcloseする．
	保存するには必ず呼ぶ必要がある。
	*/
#define CSVCLOSE (Spr::DebugCSV::GetInstance()->Close())

class SPR_DLL DebugCSV{
public:
	static DebugCSV* instance;
	std::ofstream fout;

	static DebugCSV* FASTCALL GetInstance();
	std::ostream& Stream();
	void Set(void (*out)(const char*));
	static void defcsvOutFunc(const char* str);
	void Close();
	std::string FileNameSearch();							//フォルダ内のCSVファイルをサーチし新しいファイル名を生成
};

}	//	namespace Spr

#endif

#include "FIFileContext.h"
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <WinBasis/WinBasis.h>
#endif

namespace Spr{;
//---------------------------------------------------------------------------
//	FIFileContext::FileInfo
// ファイル マッピング
// ファイルfnを読み属性でマップし、その先頭アドレスをポインタstartに取得
bool FIFileContext::FileInfo::Map(std::string fn){
#ifdef _WIN32
	//  ファイルをメモリにマッピングすることで高速かつ簡単にファイルを扱える
	// ファイルオープン
	hFile = CreateFile(fn.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);	
	if (!hFile){
		DSTR << "Cannot open input file: " << fn.c_str() << std::endl;
		return false;	
	}		
	// ファイルサイズの取得
	DWORD len = GetFileSize(hFile,NULL);	
	// ファイルマッピングオブジェクト作成
	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	// ファイルをマップ
	start = (const char*)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	end = start + len;
#else	
	hFile = fopen(fn.c_str(), "rb");
	if (!hFile) {
		DSTR << "Cannot open input file: " << fn.c_str() << std::endl;
		return false;	
	}		
	fseek(hFile, 0, SEEK_END);
	int const len = ftell(hFile);
	fseek(hFile, 0, SEEK_SET);
	buffer = DBG_NEW char[len];
	fread(buffer, 1, len, hFile);
	start = buffer;
	end = start + len;
#endif
	return true;
}
// ファイル アンマッピング
void FIFileContext::FileInfo::Unmap(){
#ifdef _WIN32
	UnmapViewOfFile(start);		// マップしたファイルをアンマップ
	CloseHandle(hFileMap);		// ファイルマッピングオブジェクトをクローズ
	CloseHandle(hFile);			// ファイルのハンドルをクローズ
#else
	fclose(hFile);
	delete[] buffer;
#endif
	start = end = NULL;
}
FIFileContext::FileInfo::~FileInfo(){
	if (start) Unmap();
}

//---------------------------------------------------------------------------
//	FIFileContext
ObjectIf* FIFileContext::Create(const IfInfo* ifInfo, const void* desc){
	for(IfStack::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it){
		ObjectIf* obj = (*it)->CreateObject(ifInfo, desc);
		if (obj) return obj;
	}
	return NULL;
}

};

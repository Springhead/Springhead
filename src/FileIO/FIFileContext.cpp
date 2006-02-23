#include "FIFileContext.h"
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <WinBasis/WinBasis.h>
#endif

namespace Spr{;
//---------------------------------------------------------------------------
//	FIFileContext::FileInfo
bool FIFileContext::FileInfo::Map(std::string fn){
#ifdef _WIN32
	hFile = CreateFile(fn.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
	if (hFile){
		DWORD len = GetFileSize(hFile,NULL);
		hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		start = (const char*)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
		end = start + len;
		return true;
	}
	return false;
#else	
	return false;
#endif
}
void FIFileContext::FileInfo::Unmap(){
#ifdef _WIN32
	UnmapViewOfFile(start);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
	start = end = NULL;
#else

#endif
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

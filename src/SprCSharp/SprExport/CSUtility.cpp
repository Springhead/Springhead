// CSUtility.cpp
#include <iostream>
#include <Windows.h>
#include "CSUtility.h"
#include "../../../include/Base/BaseDebug.h"

static char _message[1024];

void __cdecl se_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	throw SEH_Exception(code, ep);
}

const char* SEH_Exception::what() const throw() {
	_EXCEPTION_POINTERS* e = (_EXCEPTION_POINTERS*) _ep;
	size_t addr = (size_t) e->ExceptionRecord->ExceptionAddress;
#ifdef _WIN64
	char* fmt = "SEH_Exception code = %Xh  address:%X %8X";
	sprintf_s(_message, sizeof(_message), fmt, _code, (unsigned) (addr>>32), (unsigned) addr);
#else
	char* fmt = "SEH_Exception code = %Xh  address:%8X";
	sprintf_s(_message, sizeof(_message), fmt, _code, (unsigned) addr);
#endif
	return _message;
}

extern "C" {
	__declspec(dllexport) void __cdecl initUtility() {
		// OS—áŠO‚©‚çC++—áŠO‚Ö‚Ì–|–óİ’è
		_set_se_translator(se_translator);
	}
}

//end of CSUtility.cpp

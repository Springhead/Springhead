// CSUtility.cpp
#include <iostream>
#include <Windows.h>
#include <string.h>
#include "CSUtility.h"
#include "../../../include/Base/BaseDebug.h"

void __cdecl se_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	SEH_Exception se(code, ep);
	printf("%s\n", se.what());
	throw se;
}

static char _message[1024];
#ifdef	_WIN64
  #define  FRMc     "SEH_Exception: %s at "
  #define  FRMa     "addr = %08X %08X"
  #define  ADDR(x)  (unsigned) (((size_t) x) >> 32), (unsigned) ((size_t) x)
#else
  #define  FRMc     "SEH_Exception: %s at"
  #define  FRMa	    "addr = %08X"
  #define  ADDR(x)  (unsigned) ((size_t) x)
#endif
#define	RETURN_IF(c,v)  if (c == v) return #v
const char* get_reason(_EXCEPTION_POINTERS* e);
const char* get_additional_reason(_EXCEPTION_POINTERS* e);

const char* SEH_Exception::what() const throw() {
	_EXCEPTION_POINTERS* e = (_EXCEPTION_POINTERS*) _ep;
	size_t addr = (size_t) e->ExceptionRecord->ExceptionAddress;
	const char* reason = get_reason(e);
	sprintf_s(_message, sizeof(_message), FRMc FRMa, reason, ADDR(addr));
	const char* add_reason = get_additional_reason(e);
	if (add_reason) {
		strcat_s(_message, sizeof(_message)-strlen(_message), "\n               ");
		strcat_s(_message, sizeof(_message)-strlen(_message), add_reason);
	}
	return _message;
}
const char* get_reason(_EXCEPTION_POINTERS* e) {
	DWORD c = e->ExceptionRecord->ExceptionCode;
	RETURN_IF(c, EXCEPTION_ACCESS_VIOLATION);
	RETURN_IF(c, EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
	RETURN_IF(c, EXCEPTION_BREAKPOINT);
	RETURN_IF(c, EXCEPTION_DATATYPE_MISALIGNMENT);
	RETURN_IF(c, EXCEPTION_FLT_DENORMAL_OPERAND);
	RETURN_IF(c, EXCEPTION_FLT_DIVIDE_BY_ZERO);
	RETURN_IF(c, EXCEPTION_FLT_INEXACT_RESULT);
	RETURN_IF(c, EXCEPTION_FLT_INVALID_OPERATION);
	RETURN_IF(c, EXCEPTION_FLT_OVERFLOW);
	RETURN_IF(c, EXCEPTION_FLT_STACK_CHECK);
	RETURN_IF(c, EXCEPTION_FLT_UNDERFLOW);
	RETURN_IF(c, EXCEPTION_ILLEGAL_INSTRUCTION);
	RETURN_IF(c, EXCEPTION_IN_PAGE_ERROR);
	RETURN_IF(c, EXCEPTION_INT_DIVIDE_BY_ZERO);
	RETURN_IF(c, EXCEPTION_INT_OVERFLOW);
	RETURN_IF(c, EXCEPTION_INVALID_DISPOSITION);
	RETURN_IF(c, EXCEPTION_NONCONTINUABLE_EXCEPTION);
	RETURN_IF(c, EXCEPTION_PRIV_INSTRUCTION);
	RETURN_IF(c, EXCEPTION_SINGLE_STEP);
	RETURN_IF(c, EXCEPTION_STACK_OVERFLOW);
	return "Unknown Exception";
}
static char _buff[1024];
const char* get_additional_reason(_EXCEPTION_POINTERS* e) {
	DWORD c = e->ExceptionRecord->ExceptionCode;
	ULONG_PTR* ptr = e->ExceptionRecord->ExceptionInformation;

	if (c == EXCEPTION_ACCESS_VIOLATION) {
		ULONG rw_flag = ptr[0];
		ULONG address = ptr[1];
		char* rw = (rw_flag == 0) ? "Reading from" : "Writing to";
		sprintf_s(_buff, sizeof(_buff), "%s " FRMa, rw, ADDR(address));
		return _buff;
	}
	if (c == EXCEPTION_IN_PAGE_ERROR) {
		ULONG rw_flag = ptr[0];
		ULONG address = ptr[1];
		ULONG status  = ptr[2];
		char* rw = (rw_flag == 0) ? "Reading from" : "Writing to";
		sprintf_s(_buff, sizeof(_buff), "%s %Xh" FRMa, rw, ADDR(address), status);
		return _buff;
	}
	return NULL;
}

#define	SET_HOOK

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
#ifdef	SET_HOOK
	char* msg_pa[2] = { "statically", "dynamically" };
	char* msg_pd[2] = { "process termination", "FreeLibrary" };
	int idx = lpvReserved ? 0 : 1;
	_se_translator_function se_trans;

	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		printf("DllMain: DLL_PROCESS_ATTACH: DLL is loaded %s.\n", msg_pa[idx]);
		// OS例外からC++例外への翻訳設定
		_set_se_translator(se_translator);
		printf("set SEH translator (" FRMa ").\n", ADDR(se_translator));
		break;
	case DLL_THREAD_ATTACH:
		printf("DllMain: DLL_THREAD_ATTACH: new thread is creating.\n");
		// OS例外からC++例外への翻訳設定
		//_set_se_translator(se_translator);
		//printf("set SEH translator (" FRMa ").\n", ADDR(se_translator));
		break;
	case DLL_THREAD_DETACH:
		printf("DllMain: DLL_THREAD_DETACH: current thread is exiting.\n");
		// OS例外からC++例外への翻訳解除
		//se_trans = _set_se_translator(NULL);
		//printf("unset SEH translator (" FRMa ").\n", ADDR(se_trans));
		break;
	case DLL_PROCESS_DETACH:
		printf("DllMain: DLL_PROCESS_DETACH: DLL will be released by %s.\n", msg_pd[idx]);
		// OS例外からC++例外への翻訳解除
		se_trans = _set_se_translator(NULL);
		printf("unset SEH translator (" FRMa ").\n", ADDR(se_trans));
		break;
	}
#endif	//SET_HOOK
	return TRUE;
}

//end of CSUtility.cpp

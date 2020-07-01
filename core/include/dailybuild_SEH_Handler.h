// --------------------------------------------------------------------------
//  FILE:
//	dailybuild_SEH_Handler.h
//
//  DESCRIPTION:
//	Class and macros for Win32 Structured Exception.
//
//  VERSION:
//	Ver 1.0  2016/10/05 F.Kanehori	First version
//	Ver 1.1  2016/10/26 F.Kanehori	Avoid duplicated definition.
//	Ver 1.2  2017/09/11 F.Kanehori	Bug fixed.
// --------------------------------------------------------------------------
#ifndef SEH_HANDLER_H
#define SEH_HANDLER_H

#include <stdexcept>

// --------------------------------------------------------------------------
//  Kludge
// --------------------------------------------------------------------------
//	Including "Windows.h" cause compilation error!  So we define
//	following structure locally.  But we use ExceptionCode only.
//
#if !defined(AVOID_DUP_DEF)
#define	EXCEPTION_MAXIMUM_PARAMETERS	15
typedef unsigned long DWORD;
typedef void *PVOID;
#if defined(_WIN64)
	typedef unsigned __int64 ULONG_PTR;
#else
	typedef unsigned long ULONG_PTR;
#endif

#ifndef _INC_EXCPT
typedef struct _EXCEPTION_RECORD {
	DWORD ExceptionCode;
	DWORD ExceptionFlags;
	struct _EXCEPTION_RECORD *ExceptionRecord;
	PVOID ExceptionAddress;
	DWORD NumberParameters;
	ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;
typedef struct _EXCEPTION_POINTERS {
	PEXCEPTION_RECORD ExceptionRecord;
	//PCONTEXT ContextRecord;
	PVOID ContextRecord;		// kludge
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
#endif	//_INC_EXCPT
#endif	//AVOID_DUP_DEF

// --------------------------------------------------------------------------
//
class SEH_Exception : public std::exception {
private:
	SEH_Exception() {}
	unsigned int _code;
	void* _ep;
public:
	SEH_Exception(unsigned int code, void* ep) : _code(code), _ep(ep) {}
	~SEH_Exception() {}
	virtual const int exitcode() const;
};
const int SEH_Exception::exitcode() const {
	EXCEPTION_RECORD* er = ((_EXCEPTION_POINTERS*) _ep)->ExceptionRecord;
	DWORD exception_code = er->ExceptionCode;
	int code = -(10000 + (int) exception_code);
	switch (exception_code) {
	    case (DWORD) 0xC0000002L:	code = -9002; break;
	    case (DWORD) 0xC0000005L:	code = -9005; break;
	    case (DWORD) 0xC0000008L:	code = -9008; break;
	    case (DWORD) 0xC000008CL:	code = -9140; break;
	    case (DWORD) 0xC0000094L:	code = -9148; break;
	    case (DWORD) 0xC0000095L:	code = -9149; break;
	    case (DWORD) 0xC00000FDL:	code = -9253; break;
	    case (DWORD) 0xC0000194L:	code = -9404; break;
	}
	return (const int) code;
}
void __cdecl se_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	SEH_Exception se(code, ep);
	throw se;
}

// --------------------------------------------------------------------------
//
#ifdef	DAILYBUILD
  #define SEH_HANDLER_DEF \
	_se_translator_function se_trans = _set_se_translator(se_translator);
  #define SEH_HANDLER_TRY try {
  #define SEH_HANDLER_CATCH } catch (SEH_Exception e) { exit(e.exitcode()); }
#else
  #define SEH_HANDLER_DEF
  #define SEH_HANDLER_TRY
  #define SEH_HANDLER_CATCH
#endif

#endif //SEH_HANDLER_H

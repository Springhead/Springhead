// CSUtility.cpp
#include <sstream>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <process.h>
#include <stdlib.h>
#include <map>
#include "CSUtility.h"
#include "../../../include/Base/BaseDebug.h"

// --------------------------------------------------------------------------
//  Exception handling functions.
// --------------------------------------------------------------------------
const char* get_reason(DWORD code);
const char* get_additional_reason(EXCEPTION_RECORD* er);
const char* printStack(void* sample_address);
static bool first_occurence = true;

void __cdecl se_translator(unsigned int code, _EXCEPTION_POINTERS* ep) {
	SEH_Exception se(code, ep);
	if (first_occurence) {
		CSlog::Print("** enter: se_translator **\n");
		const char* stack_trace = se.trace();
		CSlog::Print(stack_trace);
		CSlog::Print("** leave: se_translator **\n");
		//first_occurence = false;
	}
	throw se;
}

#ifdef	_WIN64
  #define  HEXFMT  "0x%016llX"
#else
  #define  HEXFMT  "0x%08lX"
#endif
#define  ADDR(x)  ((size_t) (x))

static char _message[1024];
const char* SEH_Exception::what() const throw() {
	EXCEPTION_RECORD* er = ((_EXCEPTION_POINTERS*) _ep)->ExceptionRecord;
	size_t addr = (size_t) er->ExceptionAddress;
	const char* reason = get_reason(er->ExceptionCode);
	sprintf_s(_message, sizeof(_message), "SEH_Exception: %s at " HEXFMT, reason, ADDR(addr));
	const char* add_reason = get_additional_reason(er);
	if (add_reason) {
		strcat_s(_message, sizeof(_message)-strlen(_message), "\n               ");
		strcat_s(_message, sizeof(_message)-strlen(_message), add_reason);
	}
	return _message;
}

static std::string* _seh_exception_info = new std::string("** stack trace not available **");

const char* SEH_Exception::trace() const {
	std::stringstream out;
	out << std::endl << what() << std::endl;
	out << "stack trace:" << std::endl << printStack((void*) this);
	_seh_exception_info = new std::string(out.str());
	return _seh_exception_info->c_str();
}

#define	RETURN_IF(c,v)  if (c == v) return #v

const char* get_reason(DWORD code) {
	RETURN_IF(code, EXCEPTION_ACCESS_VIOLATION);
	RETURN_IF(code, EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
	RETURN_IF(code, EXCEPTION_BREAKPOINT);
	RETURN_IF(code, EXCEPTION_DATATYPE_MISALIGNMENT);
	RETURN_IF(code, EXCEPTION_FLT_DENORMAL_OPERAND);
	RETURN_IF(code, EXCEPTION_FLT_DIVIDE_BY_ZERO);
	RETURN_IF(code, EXCEPTION_FLT_INEXACT_RESULT);
	RETURN_IF(code, EXCEPTION_FLT_INVALID_OPERATION);
	RETURN_IF(code, EXCEPTION_FLT_OVERFLOW);
	RETURN_IF(code, EXCEPTION_FLT_STACK_CHECK);
	RETURN_IF(code, EXCEPTION_FLT_UNDERFLOW);
	RETURN_IF(code, EXCEPTION_ILLEGAL_INSTRUCTION);
	RETURN_IF(code, EXCEPTION_IN_PAGE_ERROR);
	RETURN_IF(code, EXCEPTION_INT_DIVIDE_BY_ZERO);
	RETURN_IF(code, EXCEPTION_INT_OVERFLOW);
	RETURN_IF(code, EXCEPTION_INVALID_DISPOSITION);
	RETURN_IF(code, EXCEPTION_NONCONTINUABLE_EXCEPTION);
	RETURN_IF(code, EXCEPTION_PRIV_INSTRUCTION);
	RETURN_IF(code, EXCEPTION_SINGLE_STEP);
	RETURN_IF(code, EXCEPTION_STACK_OVERFLOW);
	return "Unknown Exception";
}

static char _buff[1024];
const char* get_additional_reason(EXCEPTION_RECORD* er) {
	DWORD code = er->ExceptionCode;
	ULONG_PTR* ptr = er->ExceptionInformation;

	if (code == EXCEPTION_ACCESS_VIOLATION) {
		ULONG rw_flag = ptr[0];
		ULONG address = ptr[1];
		char* rw = (rw_flag == 0) ? "Reading from" : "Writing to";
		sprintf_s(_buff, sizeof(_buff), "%s address " HEXFMT, rw, ADDR(address));
		return _buff;
	}
	if (code == EXCEPTION_IN_PAGE_ERROR) {
		ULONG rw_flag = ptr[0];
		ULONG address = ptr[1];
		ULONG status  = ptr[2];
		char* rw = (rw_flag == 0) ? "Reading from" : "Writing to";
		sprintf_s(_buff, sizeof(_buff), "%s address " HEXFMT " 0x%X", rw, ADDR(address), status);
		return _buff;
	}
	return NULL;
}

// --------------------------------------------------------------------------
//  Managed exception raiser.
// --------------------------------------------------------------------------
typedef void (__cdecl* FunctionPointer)(const char*);
static FunctionPointer exception_raiser = NULL;

extern "C" {
	__declspec(dllexport) void __cdecl Spr_register_exception_raiser(FunctionPointer func) {
		if (exception_raiser == NULL) {
			exception_raiser = (FunctionPointer) func;
			CSlog::Print("** managed-exception raiser registered **\n");
		}
	}
	__declspec(dllexport) void __cdecl Spr_set_se_translator() {
		_se_translator_function se_trans = _set_se_translator(se_translator);
		CSlog::VPrint("set SEH translator: (" HEXFMT ").\n", ADDR(se_translator));
	}
}

void SEH_Exception::raise_managed_exception(char* msg) const throw() {
	(*exception_raiser)(msg);
	CSlog::Print("** call managed-exception raiser **\n");
}

// --------------------------------------------------------------------------
//  Transfer exception information from C++ to C#.
// --------------------------------------------------------------------------
extern "C" {
	__declspec(dllexport) HANDLE __cdecl Spr_SEH_Exception_what() {
		BSTR result = NULL;
		const char* cstr = _seh_exception_info->c_str();
		int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
		if (lenW > 0) {
			result = ::SysAllocStringLen(0, lenW);
			::MultiByteToWideChar(CP_ACP, 0, cstr, -1, result, lenW);
		}
        	return result;
	}
}

// --------------------------------------------------------------------------
//  Get stack trace. (from https://msdn.microsoft.com/en-us/library/windows/desktop/bb204633(v=vs.85).aspx)
// --------------------------------------------------------------------------
#include "DbgHelp.h"
#include <WinBase.h>
#pragma comment(lib, "Dbghelp.lib")

#ifdef	_WIN64
  #define  IMAGEHLP_SYMBOL_	IMAGEHLP_SYMBOL64
  #define  IMAGEHLP_MODULE_	IMAGEHLP_MODULE64
  #define  IMAGEHLP_LINE_	IMAGEHLP_LINE64
  #define  D_WORD		DWORD64
  #define  SymGetModuleInfo_	SymGetModuleInfo64
  #define  SymGetSymFromAddr_	SymGetSymFromAddr64
  #define  SymGetLineFromAddr_	SymGetLineFromAddr64
#else
  #define  IMAGEHLP_SYMBOL_	IMAGEHLP_SYMBOL
  #define  IMAGEHLP_MODULE_	IMAGEHLP_MODULE
  #define  IMAGEHLP_LINE_	IMAGEHLP_LINE
  #define  D_WORD		DWORD
  #define  SymGetModuleInfo_	SymGetModuleInfo
  #define  SymGetSymFromAddr_	SymGetSymFromAddr
  #define  SymGetLineFromAddr_	SymGetLineFromAddr
#endif

const char* convertAddress(HANDLE process, D_WORD addr) {
	std::stringstream out;
	//
	char IHS_tmp[MAX_PATH + sizeof(IMAGEHLP_SYMBOL_)];
	IMAGEHLP_SYMBOL_* pIHS = (IMAGEHLP_SYMBOL_*) IHS_tmp;
	IMAGEHLP_MODULE_ IHM;
	IMAGEHLP_MODULE_* pIHM = &IHM;
	IMAGEHLP_LINE_ IHL;
	IMAGEHLP_LINE_* pIHL = &IHL;
	memset(pIHS, 0, MAX_PATH + sizeof(IMAGEHLP_SYMBOL_));
	memset(pIHM, 0, sizeof(IMAGEHLP_MODULE_));
	memset(pIHL, 0, sizeof(IMAGEHLP_LINE_));
	//
	pIHS->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL_);
	pIHS->Address = addr;
	pIHS->MaxNameLength = MAX_PATH;
	pIHM->SizeOfStruct = sizeof(IMAGEHLP_MODULE_);
	pIHL->SizeOfStruct = sizeof(IMAGEHLP_LINE_);
	// address
	out << "0x" << (void*) addr << " ";
	// module
	if (::SymGetModuleInfo_(process, addr, pIHM)) {
		char* pName = strrchr(pIHM->ImageName, '\\');
		if (pName) {
			out << ++pName << ": ";
		} else {
			out << pIHM->ImageName << ": ";
		}
	} else {
		out << "<Unknown Module>: ";
	}
	// file and line
	D_WORD disp = 0;
	if (::SymGetSymFromAddr_(process, addr, &disp, pIHS)) {
		out << pIHS->Name;
		if (disp != 0) {
			//out << "+" << disp;
		}
		out << " ";
		DWORD disp32 = 0;
		if (::SymGetLineFromAddr_(process, addr, &disp32, pIHL)) {
			out << " " << pIHL->FileName << ": " << pIHL->LineNumber;
			if (disp32 != 0) {
				//out << "+" << disp32;
			}
		}
	} else {
		out << "<Unkonwn Symbol>";
	}
	std::string* outstr = new std::string(out.str());
	return outstr->c_str();
}

bool filterStackTrace(bool sw, const char* line) {
	if (!sw) return true;	// filter not applied
	if (strstr(line, ": Spr")) return true;
	return false;
}

const char* printStack(void* sample_address) {
	typedef USHORT (WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
	CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary("kernel32.dll"), "RtlCaptureStackBackTrace"));

	if (func == NULL) return ""; // WOE 29.SEP.2010
	std::stringstream out;

	// Quote from Microsoft Documentation:
	// ## Windows Server 2003 and Windows XP:  
	// ## The sum of the FramesToSkip and FramesToCapture parameters must be less than 63.
	const int kMaxCallers = 62; 

	void* callers_stack [kMaxCallers];
	unsigned short frames;
	SYMBOL_INFO* symbol;
	HANDLE process;
	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = (func)(0, kMaxCallers, callers_stack, NULL);
	symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

#define	APPLY_FILTER	true
	out << "0x" << sample_address << ": " << std::endl;
	const unsigned short  MAX_CALLERS_SHOWN = 32;
	frames = frames < MAX_CALLERS_SHOWN? frames : MAX_CALLERS_SHOWN;
	for (unsigned int i = 0, n = 0; i < frames; i++) {
 		const char* trace = convertAddress(process, (D_WORD)(callers_stack[i]));
		if (filterStackTrace(APPLY_FILTER, trace)) {
			out << "[" << n++ << "] " << trace << std::endl;
		}
	}

	free(symbol);
	std::string* str = new std::string(out.str());
	return str->c_str();
}

// --------------------------------------------------------------------------
//  Add exception translator to the process.
// --------------------------------------------------------------------------
#define	SET_HOOK
#define	CON	0x01
#define	LOG	0x02
#define	BOTH	(CON | LOG)
static void DllMainPrint(int out, const char* format, ...);
static std::map<DWORD, _se_translator_function> se_map;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
#ifdef	SET_HOOK
	char* msg_pa[2] = { "statically", "dynamically" };
	char* msg_pd[2] = { "process termination", "FreeLibrary" };
	int idx = lpvReserved ? 0 : 1;
	_se_translator_function se_trans;
	_se_translator_function se_trans_prev;
	DWORD thread_id = GetCurrentThreadId();

	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		// OS例外からC++例外への翻訳設定
		se_trans = _set_se_translator(se_translator);
		CSlog::Truncate();
		se_map.clear();
		se_map[(DWORD) 0] = se_trans;
		DllMainPrint(BOTH, "DllMain: DLL_PROCESS_ATTACH: DLL is loaded %s.\n", msg_pa[idx]);
		DllMainPrint(BOTH, " --- set SEH translator (" HEXFMT ").\n", ADDR(se_translator));
		break;
	case DLL_THREAD_ATTACH:
		// OS例外からC++例外への翻訳設定
		//se_trans = _set_se_translator(se_translator);
		//se_map[thread_id] = se_trans;
		//DllMainPrint(BOTH, "DllMain: DLL_THREAD_ATTACH: (" HEXFMT ").\n", ADDR(se_translator));
		break;
	case DLL_THREAD_DETACH:
		// OS例外からC++例外への翻訳解除
		//se_trans_prev = NULL;
		//if (se_map.find(thread_id) != se_map.end()) se_trans_prev = se_map[thread_id];
		//se_trans = _set_se_translator(se_trans_prev);
		////DllMainPrint(BOTH, "DllMain: DLL_THREAD_DETACH: (" HEXFMT ").\n", ADDR(se_trans_prev));
		break;
	case DLL_PROCESS_DETACH:
		// OS例外からC++例外への翻訳解除
		se_trans_prev = se_map[(DWORD) 0];
		se_trans = _set_se_translator(se_trans_prev);
		DllMainPrint(BOTH, "DllMain: DLL_PROCESS_DETACH: DLL will be released by %s.\n", msg_pd[idx]);
		DllMainPrint(BOTH, "- unset SEH translator (" HEXFMT ").\n", ADDR(se_trans_prev));
		break;
	}
#endif	//SET_HOOK
	return TRUE;
}

static void DllMainPrint(int out, const char* format, ...) {
	va_list args;
	char buff[1024];
	va_start(args, format);
	vsnprintf_s(buff, sizeof(buff), format, args);
	va_end(args);
	if (out&CON) printf(buff);
	if (out&LOG) CSlog::Print(buff);
}

// --------------------------------------------------------------------------
//  for execution tracking
// --------------------------------------------------------------------------
void CSlog::VPrint(const char* format, ...) {
	va_list args;
	char buff[1024];
	va_start(args, format);
	vsnprintf_s(buff, sizeof(buff), format, args);
	va_end(args);
	Print(buff);
}

void CSlog::Print(const char* str) {
	char* path;
	size_t len;
	errno_t err = _dupenv_s(&path, &len, "SPRCS_LOGFILE");
	if (err) return;
	std::ofstream ofs(path, std::ios::out | std::ios::app);
	free(path);
	//
	time_t now = time(NULL);
	struct tm tm;
	localtime_s(&tm, &now);
	ofs << tm.tm_year + 1990 << "/"
	    << std::setfill('0') << std::setw(2) << tm.tm_mon+1 << "/"
	    << std::setfill('0') << std::setw(2) << tm.tm_mday << " "
	    << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
	    << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
	    << std::setfill('0') << std::setw(2) << tm.tm_sec << " ";
	ofs << "[" << std::setw(5) << GetCurrentThreadId() << "] ";
	ofs << str;
	ofs.close();
}

void CSlog::Truncate() {
	char* path;
	size_t len;
	errno_t err = _dupenv_s(&path, &len, "SPRCS_LOGFILE");
	if (err) return;
	std::ofstream ofs(path, std::ios::trunc);
	free(path);
	ofs.close();
}

//end of CSUtility.cpp

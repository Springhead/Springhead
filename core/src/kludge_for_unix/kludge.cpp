// Some kludges for converting from Windws to unix.
//	Foundation module will include these functions.

#ifndef _WIN32

#include <cstddef>
#include <stdio.h>
#include <stdarg.h>

// -----------------------------
//  functions
//
int sprintf_s(char* s, size_t n, const char* f, ...) {
	va_list list;
	va_start(list, f);
	int r = sprintf(s, f, list);
	va_end(list);
	return r;
}
int sprintf_s(char* s, const char* f, ...) {
	va_list list;
	va_start(list, f);
	int r = sprintf(s, f, list);
	va_end(list);
	return r;
}

#include "kludge.h"

DWORD GetModuleFileName(HMODULE h, PTSTR p, DWORD n) {
	p[0] = '\0';
	return (DWORD) 0;
}
BOOL PathRemoveFileSpec(LPTSTR p) {
	return (BOOL) 0;
}
BOOL PathFileExists(LPCTSTR p) {
	return (BOOL) 0;
}

#endif

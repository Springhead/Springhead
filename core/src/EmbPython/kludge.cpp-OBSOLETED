// Some kludges for converting from Windws to unix.

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

#endif

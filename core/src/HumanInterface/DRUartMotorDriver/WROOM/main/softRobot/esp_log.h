#ifndef ESP_LOG_H
#define ESP_LOG_H
#include <stdio.h>

inline void ESP_LOGE(const char* tag, const char* fmt, ...){
	va_list args;
	va_start( args, fmt);
	printf("%s: ", tag);
	vprintf(fmt, args);
	va_end(args);
}
#endif //ESP_LOG_H

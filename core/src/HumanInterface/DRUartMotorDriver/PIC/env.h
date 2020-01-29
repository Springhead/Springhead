#ifndef ENV_H
#define ENV_H

/**	env.h
 * This file defines build enviroment and purpose related thigns.
 * For board type selection, please refer "boardType.h"
 */

//-----------------------------------------------------------
//	Macro
//#define DEBUG
//#define MODULETEST    //  module test mode for debugging

#ifdef __XC32
 #define PIC	1
#elif defined __xtensa__
 #define WROOM	1
#elif defined _WIN32
 #define WROOM	1
 #define __attribute__(x)
 #pragma pack(1)
#pragma warning(disable:4200)
#pragma warning(disable:4103)
typedef unsigned char	uint8_t;
 typedef unsigned short	uint16_t;
 typedef unsigned int	uint32_t;
 typedef unsigned long ulong;
#elif defined __GNUC__
#else
 #error processor macro is undefined.
#endif

#ifdef WROOM
typedef unsigned int uint32_t;
#endif
#ifdef PIC
//#define USE_MONITOR_RX   //   for debug monitor
#endif


#ifdef MODULETEST
#define DEBUG
#endif

//-----------------------------------------------------------
//	Log
extern int logLevel;
enum logLevelList{
	LOG_LEVEL_NONE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,	
};
#define PIC_LOGE	logPrintfE
#define PIC_LOGW	logPrintfW
#define PIC_LOGI	logPrintfI
#define PIC_LOGD	logPrintfD

void logPrintfE(const char* fmt,  ...);
void logPrintfW(const char* fmt,  ...);
void logPrintfI(const char* fmt,  ...);
void logPrintfD(const char* fmt,  ...);

#ifdef _WIN32
#ifdef __cplusplus
extern "C"{
#endif
	void logPrintf(const char* fmt, ...);
	void conPrintf(const char* fmt, ...);
	void filePrintf(const char* fmt, ...);
#ifdef __cplusplus
}
#endif
#else
#define  logPrintf	printf
#define  conPrintf	printf
#endif

#endif

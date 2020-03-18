#ifndef ADD_DECL_H
#define ADD_DECL_H

#ifdef _WIN32
#define __PYDECL __cdecl

#else
#define __PYDECL
#define __cdecl
#define	__fastcall
#define	__declspec(x)
#define Py_NO_ENABLE_SHARED
#define	WITH_THREAD

// please check your machine
#define	SIZEOF_SHORT 2
#define	SIZEOF_INT 4
#define	SIZEOF_LONG 8
#define	SIZEOF_LONG_LONG 8
#define	SIZEOF_FLOAT 4
#ifdef _WIN32
  #ifdef _WIN64
    #define	SIZEOF_SIZE_T 8
    #define	SIZEOF_VOID_P 8
  #else
    #define	SIZEOF_SIZE_T 4
    #define	SIZEOF_VOID_P 4
  #endif
#else
  #define	SIZEOF_SIZE_T 8
  #define	SIZEOF_VOID_P 8
#endif

#define PY_LONG_LONG long long
#define	PY_INT64_T long long
#define	PY_FORMAT_LONG_LONG ""
#if (SIZEOF_SIZE_T == SIZEOF_INT)
  #define PY_FORMAT_SIZE_T ""
#elif (SIZE_OF_SIZE_T == SIZEOF_LONG)
  #define PY_FORMAT_SIZE_T "l"
#endif

#endif	//_WIN32

#endif

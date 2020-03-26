#ifndef	_KLUDGE_H
#define	_KLUDGE_H

// Some kludges for converting from Windws to unix.
#include <cstddef>

// -----------------------------
//  functions
//
extern int sprintf_s(char* s, size_t n, const char* f, ...);
extern int sprintf_s(char* s, const char* f, ...);

// -----------------------------
//  data types
//
#define	BYTE		unsigned char
#define WORD   		unsigned short
#define DWORD   	unsigned long
#define DWORD64   	unsigned long
#define	LONGLONG	long long
#define	ULONGLONG	unsigned long long
#define PVOID   	void*
#define ULONG_PTR       unsigned long*

// -----------------------------
//  structures
//
struct M128A {
	ULONGLONG Low;
	LONGLONG High;
};

typedef struct _XMM_SAVE_AREA32 {
	WORD	ControlWord;
	WORD	StatusWord;
	BYTE	TagWord;
	BYTE	Reserved1;
	WORD	ErrorOpcode;
	DWORD	ErrorOffset;
	WORD	ErrorSelector;
	WORD	Reserved2;
	DWORD	DataOffset;
	WORD	DataSelector;
	WORD	Reserved3;
	DWORD	MxCsr;
	DWORD	MxCsr_Mask;
	M128A	FloatRegisters[8];
	M128A	XmmRegisters[16];
	BYTE	Reserved4[96];
} XMM_SAVE_AREA32, *PXMM_SAVE_AREA32;

typedef	struct _NEON128 {	// Kludge
	char	neon[16];
} NEON128;

typedef struct _CONTEXT {
	DWORD64	P1Home;
	DWORD64 P2Home;
	DWORD64 P3Home;
	DWORD64 P4Home;
	DWORD64 P5Home;
	DWORD64 P6Home;
	DWORD   ContextFlags;
	DWORD   MxCsr;
	WORD    SegCs;
	WORD    SegDs;
	WORD    SegEs;
	WORD    SegFs;
	WORD    SegGs;
	WORD    SegSs;
	DWORD   EFlags;
	DWORD64 Dr0;
	DWORD64 Dr1;
	DWORD64 Dr2;
	DWORD64 Dr3;
	DWORD64 Dr6;
	DWORD64 Dr7;
	DWORD64 Rax;
	DWORD64 Rcx;
	DWORD64 Rdx;
	DWORD64 Rbx;
	DWORD64 Rsp;
	DWORD64 Rbp;
	DWORD64 Rsi;
	DWORD64 Rdi;
	DWORD64 R8;
	DWORD64 R9;
	DWORD64 R10;
	DWORD64 R11;
	DWORD64 R12;
	DWORD64 R13;
	DWORD64 R14;
	DWORD64 R15;
	DWORD64 Rip;
	union {
		XMM_SAVE_AREA32 FltSave;
		NEON128         Q[16];
		ULONGLONG       D[32];
		struct {
			M128A	Header[2];
			M128A	Legacy[8];
			M128A	Xmm0;
			M128A	Xmm1;
			M128A	Xmm2;
			M128A	Xmm3;
			M128A	Xmm4;
			M128A	Xmm5;
			M128A	Xmm6;
			M128A	Xmm7;
			M128A	Xmm8;
			M128A	Xmm9;
			M128A	Xmm10;
			M128A	Xmm11;
			M128A	Xmm12;
			M128A	Xmm13;
			M128A	Xmm14;
			M128A	Xmm15;
		} DUMMYSTRUCTNAME;
    		DWORD           S[32];
	} DUMMYUNIONNAME;
	M128A   VectorRegister[26];
	DWORD64	VectorControl;
	DWORD64 DebugControl;
	DWORD64 LastBranchToRip;
	DWORD64 LastBranchFromRip;
	DWORD64 LastExceptionToRip;
	DWORD64 LastExceptionFromRip;
} CONTEXT, *PCONTEXT;

#define EXCEPTION_MAXIMUM_PARAMETERS    15
typedef struct _EXCEPTION_RECORD { 
        DWORD                      ExceptionCode;
        DWORD                      ExceptionFlags;
        struct _EXCEPTION_RECORED* ExceptionRecord;
        PVOID                      ExceptionAddress;
        DWORD                      NumberParameters;
        ULONG_PTR       ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;

typedef struct _EXCEPTION_POINTERS {
        PEXCEPTION_RECORD ExceptionRecord;
        PCONTEXT          ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;

#endif	//_KLUDGE_H

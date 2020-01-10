#ifndef _DECIMAL_H
#define _DECIMAL_H
#include "env.h"

//  Fixed decimal by 16bit short type
typedef short SDEC;
#define SDEC_BITS 10                   //  number of bits for decimal
#define SDEC_ONE (1 << SDEC_BITS)      //	1.0
#define SDEC_MASK (SDEC_ONE-1)         //  mask for decimal
#define GetDecimalS(x)   ((x)&SDEC_MASK)
#define GetIntegerS(x)   ((x)&~SDEC_MASK)
#define F2SDEC(x)       ((short)(SDEC_ONE*(x)))
#define SDEC2DBL(x)     ((double)(x) / SDEC_ONE)

SDEC atan2SDEC(SDEC s, SDEC c);


//  Fixed decimal by 32bit long type
typedef long LDEC;
#define  LDEC_BITS 16                   //  number of bits for decimal
#define  LDEC_ONE (1 << LDEC_BITS)      //	1.0
#define  LDEC_MASK (LDEC_ONE-1)         //  mask for decimal
#define GetDecimalL(x)   (((x)>=0) ? ((x)&LDEC_MASK) : ((x)|~LDEC_MASK))
#define GetIntegerL(x)   ((x)&~LDEC_MASK)
#define L2SDEC(x)       (SDEC)((long)(x) >> (LDEC_BITS - SDEC_BITS))
#define S2LDEC(x)       ((long)(x) << (LDEC_BITS - SDEC_BITS))
#define F2LDEC(x)       ((long)(LDEC_ONE*(x)))
#define LDEC2DBL(x)     (((double)(x)) / LDEC_ONE)

#ifdef MODULETEST
void atan2STest();
#endif
    
#endif

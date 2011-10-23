#ifndef SPREPUTILITY_H
#define SPREPUTILITY_H

#if _DEBUG
#define __EPDECL __cdecl
#else
#define __EPDECL __fastcall
#endif

#include "Utility/SprUTCriticalSection.h"//CriticalSection—p
#include <Python.h>

#define EP_MODULE_NAME "Spr"
//#define EP_USE_SUBMODULE

extern Spr::UTCriticalSection EPCriticalSection;
extern PyObject* PyErr_Spr_NullReference;

void __EPDECL initUtility();
#endif

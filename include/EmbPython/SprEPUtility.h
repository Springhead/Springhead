#ifndef SPREPUTILITY_H
#define SPREPUTILITY_H

#if _DEBUG
#define __EPDECL __cdecl
#else
#define __EPDECL __fastcall
#endif

#include "Utility/SprUTCriticalSection.h"//CriticalSection用

#include <EmbPython\/SprEPVersion.h>
#include PYTHON_H_PATH

#define EP_MODULE_NAME "Spr"
//#define EP_USE_SUBMODULE

extern Spr::UTCriticalSection EPCriticalSection;
extern PyObject* PyErr_Spr_NullReference;

void initUtility(PyObject *rootModule = NULL);
#endif

#ifndef SPREPUTILITY_H
#define SPREPUTILITY_H

#if _DEBUG
#define __EPDECL __cdecl
#else
#define __EPDECL __fastcall
#endif

#include "Utility/SprUTCriticalSection.h"//CriticalSection用

#include <EmbPython\/SprEPVersion.h>
#if EMB_PYTHON_VER==33
#include <Python33/Python.h>
#else
#include <Python32/Python.h>
#endif

#define EP_MODULE_NAME "Spr"
//#define EP_USE_SUBMODULE

extern Spr::UTCriticalSection EPCriticalSection;
extern PyObject* PyErr_Spr_NullReference;

void initUtility(PyObject *rootModule = NULL);
#endif

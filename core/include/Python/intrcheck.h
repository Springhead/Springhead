#include "Add_decl.h"

#ifndef Py_INTRCHECK_H
#define Py_INTRCHECK_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(int) __PYDECL  PyOS_InterruptOccurred(void);
PyAPI_FUNC(void) __PYDECL  PyOS_InitInterrupts(void);
PyAPI_FUNC(void) __PYDECL  PyOS_AfterFork(void);
PyAPI_FUNC(int) __PYDECL  _PyOS_IsMainThread(void);

#ifdef MS_WINDOWS
/* windows.h is not included by Python.h so use void* instead of HANDLE */
PyAPI_FUNC(void*) __PYDECL  _PyOS_SigintEvent(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_INTRCHECK_H */

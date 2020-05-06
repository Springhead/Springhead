#include "Add_decl.h"

/* System module interface */

#ifndef Py_SYSMODULE_H
#define Py_SYSMODULE_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(PyObject *) __PYDECL  PySys_GetObject(const char *);
#ifndef Py_LIMITED_API
PyAPI_FUNC(PyObject *) __PYDECL  _PySys_GetObjectId(_Py_Identifier *key);
#endif
PyAPI_FUNC(int) __PYDECL  PySys_SetObject(const char *, PyObject *);
PyAPI_FUNC(int) __PYDECL  _PySys_SetObjectId(_Py_Identifier *key, PyObject *);

PyAPI_FUNC(void) __PYDECL  PySys_SetArgv(int, wchar_t **);
PyAPI_FUNC(void) __PYDECL  PySys_SetArgvEx(int, wchar_t **, int);
PyAPI_FUNC(void) __PYDECL  PySys_SetPath(const wchar_t *);

PyAPI_FUNC(void) __PYDECL  PySys_WriteStdout(const char *format, ...)
                 Py_GCC_ATTRIBUTE((format(printf, 1, 2)));
PyAPI_FUNC(void) __PYDECL  PySys_WriteStderr(const char *format, ...)
                 Py_GCC_ATTRIBUTE((format(printf, 1, 2)));
PyAPI_FUNC(void) __PYDECL  PySys_FormatStdout(const char *format, ...);
PyAPI_FUNC(void) __PYDECL  PySys_FormatStderr(const char *format, ...);

PyAPI_FUNC(void) __PYDECL  PySys_ResetWarnOptions(void);
PyAPI_FUNC(void) __PYDECL  PySys_AddWarnOption(const wchar_t *);
PyAPI_FUNC(void) __PYDECL  PySys_AddWarnOptionUnicode(PyObject *);
PyAPI_FUNC(int) __PYDECL  PySys_HasWarnOptions(void);

PyAPI_FUNC(void) __PYDECL  PySys_AddXOption(const wchar_t *);
PyAPI_FUNC(PyObject *) __PYDECL  PySys_GetXOptions(void);

#ifndef Py_LIMITED_API
PyAPI_FUNC(size_t) __PYDECL  _PySys_GetSizeOf(PyObject *);
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_SYSMODULE_H */

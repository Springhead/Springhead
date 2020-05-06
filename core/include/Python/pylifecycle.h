#include "Add_decl.h"

/* Interfaces to configure, query, create & destroy the Python runtime */

#ifndef Py_PYLIFECYCLE_H
#define Py_PYLIFECYCLE_H
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(void) __PYDECL  Py_SetProgramName(wchar_t *);
PyAPI_FUNC(wchar_t *) __PYDECL  Py_GetProgramName(void);

PyAPI_FUNC(void) __PYDECL  Py_SetPythonHome(wchar_t *);
PyAPI_FUNC(wchar_t *) __PYDECL  Py_GetPythonHome(void);

#ifndef Py_LIMITED_API
/* Only used by applications that embed the interpreter and need to
 * override the standard encoding determination mechanism
 */
PyAPI_FUNC(int) __PYDECL  Py_SetStandardStreamEncoding(const char *encoding,
                                             const char *errors);
#endif

PyAPI_FUNC(void) __PYDECL  Py_Initialize(void);
PyAPI_FUNC(void) __PYDECL  Py_InitializeEx(int);
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) __PYDECL  _Py_InitializeEx_Private(int, int);
#endif
PyAPI_FUNC(void) __PYDECL  Py_Finalize(void);
PyAPI_FUNC(int) __PYDECL  Py_IsInitialized(void);
PyAPI_FUNC(PyThreadState *) __PYDECL  Py_NewInterpreter(void);
PyAPI_FUNC(void) __PYDECL  Py_EndInterpreter(PyThreadState *);


/* Py_PyAtExit is for the atexit module, Py_AtExit is for low-level
 * exit functions.
 */
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) __PYDECL  _Py_PyAtExit(void (*func)(void));
#endif
PyAPI_FUNC(int) __PYDECL  Py_AtExit(void (*func)(void));

PyAPI_FUNC(void) __PYDECL  Py_Exit(int);

/* Restore signals that the interpreter has called SIG_IGN on to SIG_DFL. */
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) __PYDECL  _Py_RestoreSignals(void);

PyAPI_FUNC(int) __PYDECL  Py_FdIsInteractive(FILE *, const char *);
#endif

/* Bootstrap __main__ (defined in Modules/main.c) */
PyAPI_FUNC(int) __PYDECL  Py_Main(int argc, wchar_t **argv);

/* In getpath.c */
PyAPI_FUNC(wchar_t *) __PYDECL  Py_GetProgramFullPath(void);
PyAPI_FUNC(wchar_t *) __PYDECL  Py_GetPrefix(void);
PyAPI_FUNC(wchar_t *) __PYDECL  Py_GetExecPrefix(void);
PyAPI_FUNC(wchar_t *) __PYDECL  Py_GetPath(void);
PyAPI_FUNC(void) __PYDECL       Py_SetPath(const wchar_t *);
#ifdef MS_WINDOWS
int _Py_CheckPython3();
#endif

/* In their own files */
PyAPI_FUNC(const char *) __PYDECL  Py_GetVersion(void);
PyAPI_FUNC(const char *) __PYDECL  Py_GetPlatform(void);
PyAPI_FUNC(const char *) __PYDECL  Py_GetCopyright(void);
PyAPI_FUNC(const char *) __PYDECL  Py_GetCompiler(void);
PyAPI_FUNC(const char *) __PYDECL  Py_GetBuildInfo(void);
#ifndef Py_LIMITED_API
PyAPI_FUNC(const char *) __PYDECL  _Py_hgidentifier(void);
PyAPI_FUNC(const char *) __PYDECL  _Py_hgversion(void);
#endif

/* Internal -- various one-time initializations */
#ifndef Py_LIMITED_API
PyAPI_FUNC(PyObject *) __PYDECL  _PyBuiltin_Init(void);
PyAPI_FUNC(PyObject *) __PYDECL  _PySys_Init(void);
PyAPI_FUNC(void) __PYDECL  _PyImport_Init(void);
PyAPI_FUNC(void) __PYDECL  _PyExc_Init(PyObject * bltinmod);
PyAPI_FUNC(void) __PYDECL  _PyImportHooks_Init(void);
PyAPI_FUNC(int) __PYDECL  _PyFrame_Init(void);
PyAPI_FUNC(int) __PYDECL  _PyFloat_Init(void);
PyAPI_FUNC(int) __PYDECL  PyByteArray_Init(void);
PyAPI_FUNC(void) __PYDECL  _PyRandom_Init(void);
#endif

/* Various internal finalizers */
#ifndef Py_LIMITED_API
PyAPI_FUNC(void) __PYDECL  _PyExc_Fini(void);
PyAPI_FUNC(void) __PYDECL  _PyImport_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyMethod_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyFrame_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyCFunction_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyDict_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyTuple_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyList_Fini(void);
PyAPI_FUNC(void) __PYDECL  PySet_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyBytes_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyByteArray_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyFloat_Fini(void);
PyAPI_FUNC(void) __PYDECL  PyOS_FiniInterrupts(void);
PyAPI_FUNC(void) __PYDECL  _PyGC_DumpShutdownStats(void);
PyAPI_FUNC(void) __PYDECL  _PyGC_Fini(void);
PyAPI_FUNC(void) __PYDECL  PySlice_Fini(void);
PyAPI_FUNC(void) __PYDECL  _PyType_Fini(void);
PyAPI_FUNC(void) __PYDECL  _PyRandom_Fini(void);

PyAPI_DATA(PyThreadState *) _Py_Finalizing;
#endif

/* Signals */
typedef void (*PyOS_sighandler_t)(int);
PyAPI_FUNC(PyOS_sighandler_t) __PYDECL  PyOS_getsig(int);
PyAPI_FUNC(PyOS_sighandler_t) __PYDECL  PyOS_setsig(int, PyOS_sighandler_t);

/* Random */
PyAPI_FUNC(int) __PYDECL  _PyOS_URandom (void *buffer, Py_ssize_t size);

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYLIFECYCLE_H */

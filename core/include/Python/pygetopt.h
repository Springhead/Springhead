#include "Add_decl.h"

#ifndef Py_PYGETOPT_H
#define Py_PYGETOPT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_LIMITED_API
PyAPI_DATA(int) _PyOS_opterr;
PyAPI_DATA(int) _PyOS_optind;
PyAPI_DATA(wchar_t *) _PyOS_optarg;

PyAPI_FUNC(void) __PYDECL  _PyOS_ResetGetOpt(void);
#endif

PyAPI_FUNC(int) __PYDECL  _PyOS_GetOpt(int argc, wchar_t **argv, wchar_t *optstring);

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYGETOPT_H */

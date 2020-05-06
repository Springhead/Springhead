#include "Add_decl.h"
#ifndef Py_LIMITED_API
#ifndef PY_NO_SHORT_FLOAT_REPR
#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(double) __PYDECL  _Py_dg_strtod(const char *str, char **ptr);
PyAPI_FUNC(char *) __PYDECL  _Py_dg_dtoa(double d, int mode, int ndigits,
                        int *decpt, int *sign, char **rve);
PyAPI_FUNC(void) __PYDECL  _Py_dg_freedtoa(char *s);
PyAPI_FUNC(double) __PYDECL  _Py_dg_stdnan(int sign);
PyAPI_FUNC(double) __PYDECL  _Py_dg_infinity(int sign);


#ifdef __cplusplus
}
#endif
#endif
#endif

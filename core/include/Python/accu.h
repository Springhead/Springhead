#include "Add_decl.h"
#ifndef Py_LIMITED_API
#ifndef Py_ACCU_H
#define Py_ACCU_H

/*** This is a private API for use by the interpreter and the stdlib.
 *** Its definition may be changed or removed at any moment.
 ***/

/*
 * A two-level accumulator of unicode objects that avoids both the overhead
 * of keeping a huge number of small separate objects, and the quadratic
 * behaviour of using a naive repeated concatenation scheme.
 */

#ifdef __cplusplus
extern "C" {
#endif

#undef small /* defined by some Windows headers */

typedef struct {
    PyObject *large;  /* A list of previously accumulated large strings */
    PyObject *small;  /* Pending small strings */
} _PyAccu;

PyAPI_FUNC(int) __PYDECL  _PyAccu_Init(_PyAccu *acc);
PyAPI_FUNC(int) __PYDECL  _PyAccu_Accumulate(_PyAccu *acc, PyObject *unicode);
PyAPI_FUNC(PyObject *) __PYDECL  _PyAccu_FinishAsList(_PyAccu *acc);
PyAPI_FUNC(PyObject *) __PYDECL  _PyAccu_Finish(_PyAccu *acc);
PyAPI_FUNC(void) __PYDECL  _PyAccu_Destroy(_PyAccu *acc);

#ifdef __cplusplus
}
#endif

#endif /* Py_ACCU_H */
#endif /* Py_LIMITED_API */

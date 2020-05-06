#include "Add_decl.h"

/* Interface for marshal.c */

#ifndef Py_MARSHAL_H
#define Py_MARSHAL_H
#ifdef __cplusplus
extern "C" {
#endif

#define Py_MARSHAL_VERSION 4

PyAPI_FUNC(void) __PYDECL  PyMarshal_WriteLongToFile(long, FILE *, int);
PyAPI_FUNC(void) __PYDECL  PyMarshal_WriteObjectToFile(PyObject *, FILE *, int);
PyAPI_FUNC(PyObject *) __PYDECL  PyMarshal_WriteObjectToString(PyObject *, int);

#ifndef Py_LIMITED_API
PyAPI_FUNC(long) __PYDECL  PyMarshal_ReadLongFromFile(FILE *);
PyAPI_FUNC(int) __PYDECL  PyMarshal_ReadShortFromFile(FILE *);
PyAPI_FUNC(PyObject *) __PYDECL  PyMarshal_ReadObjectFromFile(FILE *);
PyAPI_FUNC(PyObject *) __PYDECL  PyMarshal_ReadLastObjectFromFile(FILE *);
#endif
PyAPI_FUNC(PyObject *) __PYDECL  PyMarshal_ReadObjectFromString(const char *,
                                                      Py_ssize_t);

#ifdef __cplusplus
}
#endif
#endif /* !Py_MARSHAL_H */

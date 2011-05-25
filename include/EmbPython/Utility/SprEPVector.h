#ifndef SPREPVECTOR_H
#define SPREPVECTOR_H

#include <string.h>
#include <stdio.h>
#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"


using namespace Spr;

typedef double d;
typedef float f;


#define VEC_HEAD(_DIM,_TYPE)				\
	typedef struct							\
	{										\
		PyObject_HEAD						\
		Vec##_DIM##_TYPE *ptr;				\
	} EPVec##_DIM##_TYPE;					\
	\
	\
	extern PyTypeObject EPVec##_DIM##_TYPE##Type;																	\
	inline bool EPVec##_DIM##_TYPE##_Check(PyObject* ob){ return (ob->ob_type == &EPVec##_DIM##_TYPE##Type);}\
	inline bool EPVec##_DIM##_TYPE##_Check(EPVec##_DIM##_TYPE##* ob){ return (((PyObject*)ob)->ob_type == &EPVec##_DIM##_TYPE##Type);}\
	\
	PyObject* EPVec##_DIM##_TYPE##_new(PyTypeObject *type,PyObject *args, PyObject *kwds);	\
	EPVec##_DIM##_TYPE##* newEPVec##_DIM##_TYPE##();										\
	EPVec##_DIM##_TYPE##* newEPVec##_DIM##_TYPE##(Vec##_DIM##_TYPE);						\
	EPVec##_DIM##_TYPE##* newEPVec##_DIM##_TYPE##(Vec##_DIM##_TYPE##*);						\
	\
	PyMODINIT_FUNC initEPVec##_DIM##_TYPE##(void) ;		\



VEC_HEAD(2,d)
VEC_HEAD(3,d)
VEC_HEAD(2,f)
VEC_HEAD(3,f)
VEC_HEAD(4,f)


PyMODINIT_FUNC initEPVector(void);


PyMODINIT_FUNC initEPVector(void);

#endif
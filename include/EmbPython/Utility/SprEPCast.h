#ifndef SPREPCAST_H
#define SPREPCAST_H
#include <Python.h>

double PyObject_asDouble(PyObject* obj);

long PyObject_asLong(PyObject* obj);


template<class T> PyObject* PyFloat_fromAny(T val)
{
	return Py_BuildValue("d",(double)val);
}

template<class T> PyObject* PyLong_fromAny(T val)
{
	return Py_BuildValue("i",(long)val);
}

template PyObject* PyFloat_fromAny(double);
template PyObject* PyFloat_fromAny(int);
template PyObject* PyLong_fromAny(double);
template PyObject* PyLong_fromAny(int);

#endif
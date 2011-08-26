#ifndef SPREPCAST_H
#define SPREPCAST_H
#include <Python.h>
#include <string>
#include <map>
#include "SprEPObject.h"

double PyObject_asDouble(PyObject* obj);

long PyObject_asLong(PyObject* obj);

//char* PyObject_asChar(PyObject* obj);

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



class ConvertStr 
{
private:
	char* buff;
public:
	ConvertStr( PyObject *obj );
	~ConvertStr();
	std::string GetString();
	const char* GetBuffer();
};

#define CONVERT_WCHARSTR_TO_CHARSTR(wcp) ConvertStr(wcp).GetBuffer()
#define CONVERT_WCHARSTR_TO_STRING(wcp) ConvertStr(wcp).GetString()

//runtime downcast—p
typedef void (*EPCastfunc)(EPObject* obj);
typedef std::map<std::string,EPCastfunc> EPCastfuncMap;
extern EPCastfuncMap EPObject_CastfuncMap;
void EPObject_RegistCastfunc(std::string,EPCastfunc);
void EPObject_RuntimeDCast(EPObject* obj,std::string cls);
#endif
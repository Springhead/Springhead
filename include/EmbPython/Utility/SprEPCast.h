#ifndef SPREPCAST_H
#define SPREPCAST_H
#include <Springhead.h>

#include <EmbPython/SprEPVersion.h>
#include PYTHON_H_PATH

#include <string>
#include <map>
#include "SprEPObject.h"

double PyObject_asDouble(PyObject* obj);

long PyObject_asLong(PyObject* obj);

Vec3d PyObject_asVec3d(PyObject* obj);
Vec3f PyObject_asVec3f(PyObject* obj);


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

//runtime downcast用
typedef void (*EPCastfunc)(EPObject* obj);
typedef std::map<const IfInfo*,EPCastfunc> EPCastfuncMap;
extern EPCastfuncMap EPObject_CastMap;
void EPObject_RegistCastfunc(const IfInfo*,EPCastfunc);
void __PYDECL EPObject_RuntimeDCast(EPObject* obj,const IfInfo* ifinfo);
void __PYDECL EPObject_RuntimeDCast(EPObject* obj,const char* name);
#endif
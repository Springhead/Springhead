#include "../../include\EmbPython\SprEPUtility.h" 
#include "EPUtility.h" 
#include "../../include\EmbPython\Utility\SprEPObject.h"

#pragma warning(disable:4244) 
//*********** Decl Global variables ***********
Spr::UTCriticalSection EPCriticalSection;
PyObject* PyErr_Spr_NullReference;


void initUtility(PyObject *rootModule)
{
	initEPObject(rootModule);
	PyErr_Spr_NullReference = PyErr_NewException("Spr.NullReferenceError",NULL,NULL);
}
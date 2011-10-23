#include "../../include\EmbPython\SprEPUtility.h" 
#include "EPUtility.h" 

#pragma warning(disable:4244) 
//*********** Decl Global variables ***********
Spr::UTCriticalSection EPCriticalSection;
PyObject* PyErr_Spr_NullReference;

void __EPDECL initUtility()
{
	PyErr_Spr_NullReference = PyErr_NewException("Spr.NullReferenceError",NULL,NULL);
}
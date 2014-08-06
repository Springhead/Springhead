#include "../../include\EmbPython\SprEPUtility.h" 
#include "EPUtility.h" 
#include "../../include\EmbPython\Utility\SprEPObject.h"
#include <Windows.h>

#pragma warning(disable:4244) 
//*********** Decl Global variables ***********
Spr::UTCriticalSection EPCriticalSection;
PyObject* PyErr_Spr_NullReference;
PyObject* PyErr_Spr_OSException;

void mappingSEHtoCPPExceptions(unsigned int exceptionCode, _EXCEPTION_POINTERS* eptr){
        throw SEH_Exception(exceptionCode, eptr);
}
static char exceptionMessage[1024];
const char* SEH_Exception::what() const throw(){
	_EXCEPTION_POINTERS* e = (_EXCEPTION_POINTERS*) eptr;
	size_t adr = (size_t)e->ExceptionRecord->ExceptionAddress;
	sprintf(exceptionMessage, "SEH_Exception code = %Xh  address:%X %8X", seCode, (unsigned)(adr>>32), (unsigned)adr);
	return exceptionMessage;
}
const char* EPAssertException::what() const throw(){
	sprintf(exceptionMessage, "Assert(%s) in %s L%d", expr, file, line);
	return exceptionMessage;
}

void initUtility(PyObject *rootModule)
{
	initEPObject(rootModule);
	PyErr_Spr_NullReference = PyErr_NewException("Spr.NullReferenceError",NULL,NULL);
	PyErr_Spr_OSException = PyErr_NewException("Spr.OSException", NULL, NULL);	
	// OS例外からC++例外への翻訳設定
	_set_se_translator(mappingSEHtoCPPExceptions);
}
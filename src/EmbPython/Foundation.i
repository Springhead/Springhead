#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module Foundation
//-- 
#define PyAPI_FUNC(r)	r			
#define LONG_MAX		0X7FFFFFFFL	
#define UCHAR_MAX		0xFF		
#define NULL			0			
#undef __PYDECL					
#define __PYDECL					
#undef SPR_CDECL					
#define SPR_CDECL					
#undef SPR_DLL						
#define SPR_DLL					
//---
%header%{
void SPR_CDECL PyUTTimerFunc(int id, void* arg);
%}
%ignore Spr::UTPadding;
%ignore Spr::UTStringLess;
%ignore Spr::UTEatWhite;
%ignore Spr::UTTypeInfo;
%ignore Spr::UTTypeInfoObjectBase;
%ignore Spr::DebugPrintf;
%ignore Spr::DebugCSV;
%ignore Spr::UTTimerIf::SetCallback;
%extend Spr::UTTimerIf{						
	PyObject* SetCallback(PyObject* arg){
		if (PyCallable_Check($var1)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return NULL;
		}
		Py_XINCREF($var1);
			Py_XDECREF((PyObject*)($self->GetCallbackArg()));
		}
		if ($self->GetCallback() == PyUTTimerFunc 
			&& PyCallable_Check((PyObject*)$self->GetCallbackArg())){
			$self->SetCallback(PyUTTimerFunc, $var1);
		}
		ret_tmp = Py_None;
	}
}	
//---
%begin%{
#include "../../include/Springhead.h"
#include "../../include/Python/Python.h"
#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
%}
//--  
%include "../../include/Foundation/SprObject.h"
%include "../../include/Foundation/SprScene.h"
%include "../../include/Foundation/SprUTTimer.h"

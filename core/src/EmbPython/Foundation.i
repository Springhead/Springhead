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
%ignore Spr::UTTypeDescIf::GetFieldType;
%ignore Spr::UTTypeInfo;
%ignore Spr::UTTypeInfoObjectBase;
%ignore Spr::DebugPrintf;
%ignore Spr::DebugCSV;
%ignore Spr::UTTimerIf::GetCallback;
%ignore Spr::UTTimerIf::SetCallback;
%ignore Spr::UTAccessBase;
%ignore Spr::UTTypeDescIf::SetAccess;
%ignore Spr::UTTypeDescIf::Create;
%ignore Spr::UTPerformanceMeasureIf;

%extend Spr::UTTimerIf{
	PyObject* SetCallback(PyObject* arg){
		ret_tmp = Py_None;
		if (!PyCallable_Check($var1)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		}else{
			Py_XINCREF($var1);
			if ( $self->GetCallback() == PyUTTimerFunc 
				&& PyCallable_Check( (PyObject*)($self->GetCallbackArg()) ) ){
				Py_XDECREF((PyObject*)($self->GetCallbackArg()));
			}
			$self->SetCallback(PyUTTimerFunc, $var1);
		}
	}
}	
//---
%begin%{
#include "../../include/Springhead.h"

#include "../../include/EmbPython/SprEPVersion.h"
#include PYTHON_H_REL

#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
%}
//--  
%include "../../include/Foundation/SprObject.h"
%include "../../include/Foundation/SprScene.h"
%include "../../include/Foundation/SprUTTimer.h"
%include "../../include/Foundation/SprUTQPTimer.h"

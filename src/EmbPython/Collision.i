#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module Collision
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
%include "Utility/EPObject.i"
EXTEND_NEW(PHMaterial)
//---
%begin%{
#include "../../include/Springhead.h"

#include "../../include/EmbPython/SprEPVersion.h"
#if EMB_PYTHON_VER==33 
#include "../../include/Python33/Python.h" 
#else 
#include "../../include/Python32/Python.h" 
#endif 

#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
#include "../../include/EmbPython/SprEPFoundation.h"
%}
//--  
%include "../../include/Collision/SprCDShape.h"

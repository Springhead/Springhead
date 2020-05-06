#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module FileIO
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
%begin%{
#include "../../include/Springhead.h"

#include "../../include/EmbPython/SprEPVersion.h"
#include PYTHON_H_REL

#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
#include "../../include/EmbPython/SprEPFoundation.h"
%}
//--  
%include "../../include/FileIO/SprFIFile.h"
%include "../../include/FileIO/SprFIImport.h"
%include "../../include/FileIO/SprFISdk.h"

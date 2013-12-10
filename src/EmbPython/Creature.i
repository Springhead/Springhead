#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module Creature
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
EXTEND_NEW(CRContactInfo)
EXTEND_NEW(CRVisualInfo)
EXTEND_N_GET_TO_LIST(Spr::CRSdkIf		,NCreatures	,GetCreature	,CRCreatureIf	)
EXTEND_N_GET_TO_LIST(Spr::CRCreatureIf	,NEngines	,GetEngine		,CREngineIf		)
EXTEND_N_GET_TO_LIST(Spr::CRBodyIf		,NBones		,GetBone		,CRBoneIf		)
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
#include "../../include/EmbPython/SprEPPhysics.h"
#include "../../include/EmbPython/SprEPGraphics.h"
%}
//--  
%include "../../include/Creature/SprCRBody.h"
%include "../../include/Creature/SprCRBone.h"
%include "../../include/Creature/SprCRController.h"
%include "../../include/Creature/SprCRReachController.h"
%include "../../include/Creature/SprCRCreature.h"
%include "../../include/Creature/SprCREngine.h"
%include "../../include/Creature/SprCRSdk.h"
%include "../../include/Creature/SprCRTouchSensor.h"
%include "../../include/Creature/SprCRVisualSensor.h"

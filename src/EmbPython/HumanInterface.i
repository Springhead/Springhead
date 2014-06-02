#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module HumanInterface
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
%ignore Spr::DVKeyMouseIf::GetMousePosition; //éQè∆ìnÇµÇµÇƒílÇì¸ÇÍÇƒÇ‡ÇÁÇ§ÇÊÇ§Ç»ä÷êîÇÇ«Ç§Ç…Ç©Ç∑ÇÈ
%ignore Spr::HITrackballIf::GetAngle; //ìØè„
%ignore Spr::HITrackballIf::GetDistanceRange; //ìØè„
%ignore Spr::HITrackballIf::GetLatitudeRange; //ìØè„
%ignore Spr::HITrackballIf::GetLongitudeRange; //ìØè„
//---
%include "Utility/EPObject.i"
EXTEND_NEW(HISpidarMotorDesc)
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
#include "../../include/EmbPython/SprEPGraphics.h"
%}
//--  
%include "../../include/HumanInterface/SprHIBase.h"
%include "../../include/HumanInterface/SprHIDevice.h"
%include "../../include/HumanInterface/SprHIDRUsb.h"
%include "../../include/HumanInterface/SprHIJoyStick.h"
%include "../../include/HumanInterface/SprHIKeyMouse.h"
%include "../../include/HumanInterface/SprHINovint.h"
%include "../../include/HumanInterface/SprHISdk.h"
%include "../../include/HumanInterface/SprHISpaceNavigator.h"
%include "../../include/HumanInterface/SprHISpidar.h"
%include "../../include/HumanInterface/SprHITrackball.h"
%include "../../include/HumanInterface/SprHIXbox360Controller.h"
%include "../../include/HumanInterface/SprHISkeletonSensor.h"
%include "../../include/HumanInterface/SprHILeap.h"

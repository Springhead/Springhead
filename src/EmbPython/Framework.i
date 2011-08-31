#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module Framework
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
%ignore Spr::FWGraphicsAdaptee::Init;
%ignore Spr::FWInteractSceneIf::GetIAAdaptee; //APIの不整合がありコンパイル不可なので
%ignore Spr::FWSceneIf::GetObjects; // **が出てくるので未対応

//--
%begin%{
#include "../../include/Springhead.h"
#include "../../include/Python/Python.h"
#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPFoundation.h"
#include "../../include/EmbPython/SprEPFileIO.h"
#include "../../include/EmbPython/SprEPPhysics.h"
#include "../../include/EmbPython/SprEPGraphics.h"
#include "../../include/EmbPython/SprEPHumanInterface.h"
#include "../../include/Framework/SprFWBone.h"
%}
//--  
%include "../../include/Framework/SprFWBone.h"
%include "../../include/Framework/SprFWFemMesh.h"
%include "../../include/Framework/SprFWInteractPointer.h"
%include "../../include/Framework/SprFWInteractScene.h"
%include "../../include/Framework/SprFWObject.h"
%include "../../include/Framework/SprFWScene.h"
%include "../../include/Framework/SprFWSdk.h"

//--

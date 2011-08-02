#	Do not edit. RunSwig.bat will update this file.
%module Framework
//--  
%{
#include "../../include/Springhead.h"
#include "../../include/Python/Python.h"
#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPFoundation.h"
#include "../../include/EmbPython/SprEPPhysics.h"
#include "../../include/EmbPython/SprEPGraphics.h"
#include "../../include/EmbPython/SprEPHumanInterface.h"
%}
//--  
%include "../../include/base/Env.h"
//--  
#define PyAPI_FUNC(r)	r			
#define LONG_MAX		0X7FFFFFFFL	
#define UCHAR_MAX		0xFF		
#define NULL			0			
#undef __PYDECL					
#define __PYDECL					
//--  
%include "../../include/Framework/SprFWApp.h"
%include "../../include/Framework/SprFWBone.h"
%include "../../include/Framework/SprFWEditor.h"
%include "../../include/Framework/SprFWFemMesh.h"
%include "../../include/Framework/SprFWGraphicsAdaptee.h"
%include "../../include/Framework/SprFWInteractPointer.h"
%include "../../include/Framework/SprFWInteractScene.h"
%include "../../include/Framework/SprFWObject.h"
%include "../../include/Framework/SprFWScene.h"
%include "../../include/Framework/SprFWSdk.h"
%include "../../include/Framework/SprFWWin.h"

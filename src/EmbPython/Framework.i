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
%ignore Spr::FWSceneIf::AddHumanInterface;//HIForceDevice6DがAPIヘッダで定義されていない

%ignore Spr::FWApp::GetTimer;
%ignore Spr::FWApp::CreateTimer;
%ignore Spr::FWApp::timers;
%ignore Spr::FWApp::GRInit;
%ignore Spr::FWApp::Init;
%ignore Spr::FWTextBoxIf::GetIntRange;
%ignore Spr::FWTextBoxIf::GetFloatRange;
%ignore Spr::FWWinIf::CalcViewport;

%ignore Spr::EPApp::menus;
%ignore Spr::EPApp::ss;
%ignore Spr::EPApp::editor;
%ignore Spr::EPApp::timer;
%ignore Spr::EPApp::states;


%include "Utility/EPObject.i"
EXTEND_N_GETS_TO_LIST(Spr::FWSceneIf,NObject,GetObjects,FWObjectIf)

namespace Spr{
class UTRefCount {};
}

//--
%begin%{
#include "../../include/Springhead.h"
#include "../../include/Python/Python.h"
#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
#include "../../include/EmbPython/SprEPFoundation.h"
#include "../../include/EmbPython/SprEPFileIO.h"
#include "../../include/EmbPython/SprEPPhysics.h"
#include "../../include/EmbPython/SprEPGraphics.h"
#include "../../include/EmbPython/SprEPHumanInterface.h"
#include "../../include/Framework/SprFWHapticPointer.h"
#include "../../include/Framework/SprFWWin.h"
#include "../../include/Framework/SprFWApp.h"
#include "../../include/EmbPython/SprEPApp.h"
%}
//--  
%include "../../include/Framework/SprFWFemMesh.h"
%include "../../include/Framework/SprFWObject.h"
%include "../../include/Framework/SprFWScene.h"
%include "../../include/Framework/SprFWSdk.h"
%include "../../include/Framework/SprFWHapticPointer.h"
%include "../../include/Framework/SprFWWin.h"
%include "../../include/Framework/SprFWApp.h"
%include "../../include/EmbPython/SprEPApp.h"
//--



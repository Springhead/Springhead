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
%ignore Spr::FWInteractSceneIf::GetIAAdaptee; //API�̕s�����������R���p�C���s�Ȃ̂�
%ignore Spr::FWSceneIf::GetObjects; // **���o�Ă����̂Ŗ��Ή�
%ignore Spr::FWSceneIf::AddHumanInterface;//HIForceDevice6D��API�w�b�_�Œ��`�����Ă��Ȃ�
%ignore Spr::FWSceneIf::CreateOpHapticHandler;
%ignore Spr::FWSceneIf::GetOpHapticHandler;

%extend Spr::FWAppBase{
	Spr::UTTimerIf* CreateTimer(int mode){
		ret_tmp = c_self->CreateTimer((Spr::UTTimerIf::Mode)c_param1);
	}
}

%ignore Spr::FWApp::GRInit(int argc, char* argv[], int type);
%ignore Spr::FWApp::GRInit(int argc, char* argv[]);
%ignore Spr::FWApp::GRInit(int argc);
%ignore Spr::FWApp::Init(int argc, char* argv[]);
%ignore Spr::FWApp::Init(int argc);
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

#include "../../include/EmbPython/SprEPVersion.h"
#include PYTHON_H_REL

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
#include "../../include/Framework/SprFWScene.h"
#include "../../include/Framework/SprFWOpHapticHandler.h"
#include "../../include/EmbPython/SprEPApp.h"
%}
//--  
%include "../../include/Framework/SprFWFemMesh.h"
%include "../../include/Framework/SprFWFemMeshNew.h"
%include "../../include/Framework/SprFWObject.h"
%include "../../include/Framework/SprFWScene.h"
%include "../../include/Framework/SprFWSdk.h"
%include "../../include/Framework/SprFWHapticPointer.h"
%include "../../include/Framework/SprFWSkeletonSensor.h"
%include "../../include/Framework/SprFWWin.h"
%include "../../include/Framework/SprFWApp.h"
%include "../../include/EmbPython/SprEPApp.h"
//--



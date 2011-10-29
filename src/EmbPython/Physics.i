#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module Physics
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
%ignore Spr::PHIKActuatorIf::GetRawSolution;
%ignore Spr::PHSceneIf::CreateIKEndEffector;
%ignore Spr::PHSceneIf::CreateIKActuator;
%ignore Spr::PHSceneIf::GetSolids;
%ignore Spr::PHJoint1DIf::GetRange;
%ignore Spr::PHSceneIf::SetContactMode(PHSolidIf **,size_t,PHSceneDesc::ContactMode);
%ignore Spr::PHSceneIf::SetContactMode(PHSolidIf **,size_t);
%include "Utility/EPObject.i"
EXTEND_NEW(PHSolidDesc)
EXTEND_NEW(PHPathPoint)
//---
%begin%{
#include "../../include/Springhead.h"
#include "../../include/Python/Python.h"
#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
#include "../../include/EmbPython/SprEPFoundation.h"
#include "../../include/EmbPython/SprEPCollision.h"
%}
//--  
%include "../../include/Physics/SprPHDef.h"
%include "../../include/Physics/SprPHEngine.h"
%include "../../include/Physics/SprPHFemMesh.h"
%include "../../include/Physics/SprPHIK.h"
%include "../../include/Physics/SprPHJoint.h"
%include "../../include/Physics/SprPHNodeHandlers.h"
%include "../../include/Physics/SprPHScene.h"
%include "../../include/Physics/SprPHSdk.h"
%include "../../include/Physics/SprPHSolid.h"

%extend Spr::PHSceneIf{
	PyObject* GetSolids(){
		int nsolids = $self->NSolids();
		PyObject* pySolids = PyTuple_New(nsolids);
		PyObject* pySolid;
		PHSolidIf** solids = $self->GetSolids();

		for ( int i = 0 ; i < nsolids ; i ++){
			pySolid = newEPPHSolidIf(solids[i]);
			PyTuple_SET_ITEM(pySolids,i,pySolid);
		}
		return pySolids;
	}
}


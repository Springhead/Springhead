#	Do not edit. RunSwig.bat will update this file.
#pragma SWIG nowarn=-401-325
%module Graphics
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
%ignore Spr::GRRenderBaseIf::DrawIndexed;
%ignore Spr::GRRenderBaseIf::DrawArrays;
%ignore Spr::GRRenderBaseIf::DrawCurve;
%ignore Spr::GRRenderBaseIf::CreateShader;
%ignore Spr::GRFrameIf::GetChildren;
%ignore Spr::GRVertexElement::vfP2f;
%ignore Spr::GRVertexElement::vfP3f;
%ignore Spr::GRVertexElement::vfC4bP3f;
%ignore Spr::GRVertexElement::vfN3fP3f;
%ignore Spr::GRVertexElement::vfC4fN3fP3f;
%ignore Spr::GRVertexElement::vfT2fP3f;
%ignore Spr::GRVertexElement::vfT4fP4f;
%ignore Spr::GRVertexElement::vfT2fC4bP3f;
%ignore Spr::GRVertexElement::vfT2fN3fP3f;
%ignore Spr::GRVertexElement::vfT2fC4fN3fP3f;
%ignore Spr::GRVertexElement::vfT4fC4fN3fP4f;
%ignore Spr::GRVertexElement::typicalFormats;
%ignore Spr::GRVertexElement::vfP3fB4f;
%ignore Spr::GRVertexElement::vfC4bP3fB4f;
%ignore Spr::GRVertexElement::vfC3fP3fB4f;
%ignore Spr::GRVertexElement::vfN3fP3fB4f;
%ignore Spr::GRVertexElement::vfC4fN3fP3fB4f;
%ignore Spr::GRVertexElement::vfT2fP3fB4f;
%ignore Spr::GRVertexElement::vfT2fC4bP3fB4f;
%ignore Spr::GRVertexElement::vfT2fN3fP3fB4f;
%ignore Spr::GRVertexElement::vfT2fC4fN3fP3fB4f;
%ignore Spr::GRVertexElement::typicalBlendFormats;
%ignore Spr::GRMeshFace::indices;
//---
//---
%include "Utility/EPObject.i"
EXTEND_NEW(GRAnimationKey)
EXTEND_NEW(GRCameraDesc)
EXTEND_NEW(GRVertexElement)
EXTEND_NEW(GRKey)
EXTEND_NEW(GRMeshFace)
EXTEND_N_GETS_TO_LIST(Spr::GRFrameIf,NChildren,GetChildren,GRVisualIf)
//---
%begin%{
#include "../../include/Springhead.h"

#include "../../include/EmbPython/SprEPVersion.h"
#include PYTHON_H_REL

#include "../../include/EmbPython/SprEPUtility.h"
#include "../../include/EmbPython/SprEPBase.h"
#include "../../include/EmbPython/SprEPFoundation.h"
#include "../../include/EmbPython/SprEPPhysics.h"
%}
//--  
%include "../../include/Graphics/SprGRBlendMesh.h"
%include "../../include/Graphics/SprGRFrame.h"
%include "../../include/Graphics/SprGRMesh.h"
%include "../../include/Graphics/SprGRRender.h"
%include "../../include/Graphics/SprGRScene.h"
%include "../../include/Graphics/SprGRSdk.h"
%include "../../include/Graphics/SprGRShader.h"
%include "../../include/Graphics/SprGRSphere.h"
%include "../../include/Graphics/SprGRVertex.h"

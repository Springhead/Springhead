%module RBGraphics

%include <sprtypemaps.i>

%{
#include <SprGraphics.h>
using namespace Spr;
%}

%import	 <Base/Env.h>			// SPR_CDECL
%include <Foundation/SprObject.h>
%include <Foundation/SprScene.h>

%include "Graphics/SprGRFrame.h"
%include "Graphics/SprGRRender.h"
%include "Graphics/SprGRSdk.h"
%include "Graphics/SprGRScene.h"
%include "Graphics/SprGRMesh.h"
%include "Graphics/SprGRBlendMesh.h"
%include "Graphics/SprGRSphere.h"

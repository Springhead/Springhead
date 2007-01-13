%module RBPhysics

%include <sprtypemaps.i>

%{
#include <SprPhysics.h>
using namespace Spr;
%}

%import	 <Base/Env.h>			// SPR_CDECL
%include <Foundation/SprObject.h>
%include <Foundation/SprScene.h>
%include "Physics/SprPHSdk.h"
%include "Physics/SprPHScene.h"
%include "Physics/SprPHSolid.h"
%include "Physics/SprPHJoint.h"
%include "Physics/SprPHEngine.h"

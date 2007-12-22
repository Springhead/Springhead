%module Springhead

%include "workaround.i"

%{
#include <Base/TQuaternion.h>
#include <Base/Affine.h>
#include <SprFoundation.h>
#include <SprCollision.h>
#include <SprFileIO.h>
#include <SprGraphics.h>
#include <SprPhysics.h>
#include <SprFramework.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWAppGL.h>
//#include <Framework/SprFWAppGLUT.h>
#include "FWVFuncBridgeRuby.h"
#include <iostream>
using namespace std;
using namespace PTM;
using namespace Spr;
%}

%include "ptmtypemaps.i"
%include "sprtypemaps.i"

%typemap(argout) int *dummy_for_vfuncbridge %{
	arg1->vfBridge = DBG_NEW FWVFuncBridgeRuby();
	arg1->vfBridge->Link((void*)self);
%}
namespace Spr{
%extend FWApp {
	void Link(int* dummy_for_vfuncbridge){}
}
}

//以下ではヘッダ内の#includeによって参照される方を先に列挙することに注意．そうしないとランタイムエラーとなる．

%import	 <Base/Env.h>			// SPR_CDECL

namespace Spr{
struct IfInfo;
}



%include <Foundation/SprObject.h>
%include <Foundation/SprScene.h>

%include <Collision/SprCDShape.h>

%include <Physics/SprPHDef.h>
%include <Physics/SprPHSdk.h>
%include <Physics/SprPHSolid.h>
%include <Physics/SprPHJoint.h>
%include <Physics/SprPHScene.h>
%include <Physics/SprPHEngine.h>
%include <Physics/SprPHNodeHandlers.h>

%include <Graphics/SprGRFrame.h>
%include <Graphics/SprGRRender.h>
%include <Graphics/SprGRSdk.h>
%include <Graphics/SprGRScene.h>
%include <Graphics/SprGRMesh.h>
%include <Graphics/SprGRBlendMesh.h>
%include <Graphics/SprGRSphere.h>
%include <Graphics/SprGRScene.h>

%include <FileIO/SprFISdk.h>
%include <FileIO/SprFIFile.h>

%include <Framework/SprFWObject.h>
%include <Framework/SprFWScene.h>
%include <Framework/SprFWSdk.h>
%include <Framework/SprFWApp.h>
%include <Framework/SprFWAppGL.h>
//%include <Framework/SprFWAppGLUT.h>

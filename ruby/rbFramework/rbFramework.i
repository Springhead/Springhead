%module RBFramework

%include <sprtypemaps.i>

%{
#include <SprFramework.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWAppGL.h>
#include <Framework/SprFWAppGLUT.h>
#include "FWVFuncBridgeRuby.h"
#include <iostream>
using namespace std;
using namespace Spr;
%}

%typemap(argout) int *dummy_for_vfuncbridge %{
	arg1->vfBridge = DBG_NEW FWVFuncBridgeRuby();
	arg1->vfBridge->Link((void*)self);
%}
namespace Spr{
%extend FWApp {
	void Link(int* dummy_for_vfuncbridge){}
}
}

%import	 <Base/Env.h>			// SPR_CDECL
%include <SprFramework.h>
%include <Framework/SprFWApp.h>
%include <Framework/SprFWAppGL.h>
%include <Framework/SprFWAppGLUT.h>

%module SprPy 
%{
#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWEditor.h>
#include <Graphics/SprGRVertex.h>
using namespace Spr;
%}
//--  
namespace Spr{;
class Posef;
class Quaternionf;
class UTString;
class Vec4i;
struct FISdkIf;
struct FWSdkIf;
struct FWBoneIf;
struct HIRealDeviceIf;
struct IfInfo;
struct ImportIf;
struct PHBallJointIf;
struct PHConstraintsIf;
struct PHHingeJointIf;
struct PHSolidDesc;
struct PHSolidIf;
struct PHSolidPairForLCPIf;
struct FWBoneIf;
struct GRVertexElement;
struct GRVertexArray;
struct GRShaderFormat;
struct FWWinDesc;
class FWWin;
class FWUICamera;
class FWGraphicsAdaptee;
struct FWInteractSceneDesc;
struct FWInteractSceneIf;
struct FWSceneIf;
};

//--  
%warnfilter(462) Spr::GRVertexElement; 
%include "../../include/Base/Env.h" 
%include "../../include/Base/BaseUtility.h"

//--  
%warnfilter(462) Spr::GRVertexElement; 
%include "../../include/Base/Env.h" 
%include "PyPTM.i" 
%include "SprPyFiles.i"

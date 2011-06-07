%module SprPy 
%{
#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWEditor.h>
%}
//--  
%warnfilter(462) Spr::GRVertexElement; 
%include "../../include/Base/Env.h" 
%include "PyPTM.i" 
%include "SprPyFiles.i"

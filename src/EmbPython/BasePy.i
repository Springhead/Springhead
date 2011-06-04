%module Base
#define DOUBLECOLON :: 
%{
#include <Springhead.h>
%}
%include "PyPTM.i"

%include "../../include/Springhead.h"
%include "../../include/base/Env.h"
%include "../../include/Base/Affine.h"
%include "../../include/Base/BaseTypeInfo.h"
%include "../../include/Base/BaseUtility.h"
%include "../../include/Base/Env.h"
%include "../../include/Base/TinyMat.h"
%include "../../include/Base/TinyVec.h"
%include "../../include/Base/TMatrix.h"
%include "../../include/Base/TMatrixUtility.h"
%include "../../include/Base/TQuaternion.h"
%include "../../include/Base/TVector.h"

#include "Graphics.h"
#pragma hdrstop

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
OBJECTIMP(GRDeviceGL, GRDevice);
IF_IMP(GRDeviceGL, GRDevice);

void GRDeviceGL::DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){
}
void GRDeviceGL::DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){
}
void GRDeviceGL::SetMaterial(const GRMaterial& mat){
}


}	//	Spr


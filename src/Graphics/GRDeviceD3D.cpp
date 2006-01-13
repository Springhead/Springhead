#if 0
#include "Graphics.h"
#pragma hdrstop

namespace Spr {;
OBJECTIMP(GRDeviceD3D, GRDevice);
IF_IMP(GRDeviceD3D, GRDevice);

//----------------------------------------------------------------------------
//	GRDeviceD3D
void GRDeviceD3D::DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){
}
void GRDeviceD3D::DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){
}
void GRDeviceD3D::SetMaterial(const GRMaterial& mat){
}


}	//	Spr

#endif
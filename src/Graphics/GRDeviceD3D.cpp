#if 0
#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;
IF_OBJECT_IMP(GRDeviceD3D, GRDevice);

//----------------------------------------------------------------------------
//	GRDeviceD3D
void GRDeviceD3D::DrawDirect(TPrimitiveType ty, Vec3f* begin, size_t count, size_t stride){
}
void GRDeviceD3D::DrawIndexed(TPrimitiveType ty, size_t* idx, Vec3f* vtx, size_t count, size_t stride){
}
void GRDeviceD3D::SetMaterial(const GRMaterial& mat){
}


}	//	Spr

#endif

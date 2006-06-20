/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
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

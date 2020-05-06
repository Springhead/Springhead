/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <GraphicsD3D/GraphicsD3D.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "GraphicsD3D.h"

namespace Spr{;
GRSdkD3DIf* SPR_CDECL GRSdkD3DIf::CreateSdk(){
	GRSdkD3D* rv = DBG_NEW GRSdkD3D;
	return rv->Cast();
}

//----------------------------------------------------------------------------
//	GRSdkD3D
IF_SPR_OBJECTIMP(GRSdkD3D, Sdk);
GRSdkD3D::GRSdkD3D(const GRSdkD3DDesc& desc):GRSdkD3DDesc(desc){
}
GRDeviceD3DIf* GRSdkD3D::CreateDeviceD3D(){
	GRDeviceD3D* rv = DBG_NEW GRDeviceD3D();
	objects.push_back(rv);
	return rv->Cast();
}
GRDebugRenderD3DIf* GRSdkD3D::CreateDebugRenderD3D(){
	GRDebugRenderD3D* rv = DBG_NEW GRDebugRenderD3D;
	objects.push_back(rv);
	return rv->Cast();
}
GRAnimationMeshIf* GRSdkD3D::CreateAnimationMesh(const GRAnimationMeshDesc& desc){
	GRAnimationMesh* rv = DBG_NEW GRAnimationMesh(desc);
	objects.push_back(rv);
	return rv->Cast();
}
void GRSdkD3D::Clear(){
	Sdk::Clear();
	objects.clear();
}

}

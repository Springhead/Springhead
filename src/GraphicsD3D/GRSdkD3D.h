/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRSdkD3D.h
 *	@brief DirectXのグラフィックスSDK
 */
#ifndef GRSDKD3D_H
#define GRSDKD3D_H
#include <SprGraphicsD3D.h>
#include "IfStubGraphicsD3D.h"


namespace Spr {;

/**	@class	GRSdkD3D
    @brief	DirectXのグラフィックスSDK　 */
class SPR_DLL GRSdkD3D:public Sdk, public GRSdkD3DIfInit, public GRSdkD3DDesc{
protected:
	typedef std::vector< UTRef<Object> > Objects;
	Objects objects;
public:
	SPR_OBJECTDEF(GRSdkD3D, Sdk);
	GRSdkD3D(const GRSdkD3DDesc& = GRSdkD3DDesc());
	virtual GRDeviceD3DIf* CreateDeviceD3D();
	virtual GRDebugRenderD3DIf* CreateDebugRenderD3D();
	virtual GRAnimationMeshIf* CreateAnimationMesh(const GRAnimationMeshDesc& desc);
	virtual void Clear();
};

}
#endif

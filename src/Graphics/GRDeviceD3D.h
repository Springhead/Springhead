/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRDeviceD3D.h
 *	@brief Direct3Dによるグラフィックス描画の実装　　
 */
#if 0
#ifndef GRDEVICED3D_H
#define GRDEVICED3D_H

#include <SprGraphics.h>

namespace Spr{;

/**	Direct3Dによるグラフィックス描画の実装	*/
class GRDeviceD3D: public GRDevice, GRDeviceD3DIfInit{
	OBJECTDEF(GRDeviceD3D, GRDevice);
public:
	virtual void DrawDirect(TPrimitiveType ty, void* vtx, size_t count, size_t stride);
	virtual void DrawIndexed(TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride);
	virtual void SetMaterial(const GRMaterial& mat);
	virtual void Init();
};

}
#endif
#endif

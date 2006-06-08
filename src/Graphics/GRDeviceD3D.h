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
class GRDeviceD3D: public GRDevice, public GRDeviceD3DIf{
	OBJECT_DEF(GRDeviceD3D);
	BASEIMP_OBJECT(GRDevice);
public:
	virtual void DrawDirect(TPrimitiveType ty, void* vtx, size_t count, size_t stride);
	virtual void DrawIndexed(TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride);
	virtual void SetMaterial(const GRMaterial& mat);
	virtual void Init();
};

}
#endif
#endif

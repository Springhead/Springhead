#ifndef GRDEVICE_H
#define GRDEVICE_H

#include <SprGraphics.h>

namespace Spr{;

/**	グラフィックス描画の実装	*/
class GRDevice: public Object, public GRDeviceIf{
public:
	OBJECTDEFABST(GRDevice);
	BASEIMP_OBJECT(Object);
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end)=0;
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx)=0;
	virtual void SetMaterial(const GRMaterial& mat)=0;
	virtual void Print(std::ostream& os) const{}
};

}
#endif

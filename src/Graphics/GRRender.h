#ifndef GRRENDER_H
#define GRRENDER_H

#include <Springhead.h>

namespace Spr{;

/**	グラフィックスレンダラーの基本クラス	*/
class GRRender: public Object, public GRRenderIf{
	OBJECTDEF(GRRender);
	BASEIMP_OBJECT(Object);
protected:
	UTRef<GRDeviceIf> device;
public:
	virtual void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){
		device->DrawDirect(ty, begin, end);
	}
	virtual void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){
		device->DrawIndexed(ty, begin, end, vtx);
	}
	virtual void SetMaterial(const GRMaterial& mat){ device->SetMaterial(mat); }
	virtual void SetDevice(GRDeviceIf* dev){ device = dev; }
	virtual void Print(std::ostream& os) const;
};

#define BASEIMP_GRRENDER(base)											\
	BASEIMP_OBJECT(base)												\
	void DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){		\
		base::DrawDirect(ty, begin, end); }								\
	void DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end,		\
		Vec3f* vtx){base::DrawIndexed(ty, begin, end, vtx);}			\
	virtual void SetMaterial(const GRMaterial& mat){					\
		base::SetMaterial(mat); }										\
	virtual void SetDevice(GRDeviceIf* dev){ base::SetDevice(dev); }	\

}
#endif

#include "Collision.h"
#pragma hdrstop

namespace Spr {;
//------------------------------------------------------------------------
SGOBJECTIMP(CDSphere, CDConvex);
///	サポートポイントを求める．
Vec3f CDSphere::Support(const Vec3f& p_) const{
	Vec3f p = p_;
	p -= center;
	float s = p.norm();
	if (s > 1e-8f) {
		float r = radius / s;
		return r*p + center;
	}else{
		return center;
	}
}
void CDSphere::CalcBBox(Vec3f& bbMin, Vec3f& bbMax){
	bbMin.element_min(center - Vec3f(radius,radius,radius));
	bbMax.element_max(center + Vec3f(radius,radius,radius));
}

/**	球プリミティブをドキュメントからロード*/
class CDSphereLoader:public FIObjectLoader<CDSphere>{
public:
	virtual bool LoadData(FILoadScene* ctx, TLoaded* sph){
		ctx->docs.Top()->GetData(sph->radius, "Radius");
		return true;
	}
};
DEF_REGISTER_LOADER(CDSphere);


}


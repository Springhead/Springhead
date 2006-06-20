/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;
//------------------------------------------------------------------------
SGIF_OBJECT_IMP(CDSphere, CDConvex);
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


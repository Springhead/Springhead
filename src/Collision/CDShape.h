/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSHAPE_H
#define PHSHAPE_H

#include <SprCollision.h>
#include <Foundation/Object.h>

namespace Spr{;

/// 形状
/// 形状自体は位置情報を持たない．位置情報は形状を参照する各剛体が持つ．
class CDShape : public NamedObject{
public:
	PHMaterial material;

	SPR_OBJECTDEF_ABST(CDShape);
	virtual void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose)=0;
	virtual float CalcVolume(){ return FLT_MAX; }
	virtual Matrix3f CalcMomentOfInertia(){ return Matrix3f(); }
};

}	//	namespace Spr
#endif

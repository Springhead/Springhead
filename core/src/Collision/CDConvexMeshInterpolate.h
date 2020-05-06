/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDCONVEXMESHINTERPOLATE_H
#define CDCONVEXMESHINTERPOLATE_H

#include <Collision/CDConvexMesh.h>

namespace Spr{;

///	凸多面体
class CDConvexMeshInterpolate : public CDConvexMesh{
public:
	SPR_OBJECTDEF(CDConvexMeshInterpolate);
	SPR_DECLMEMBEROF_CDConvexMeshInterpolateDesc;

	CDConvexMeshInterpolate();
	CDConvexMeshInterpolate(const CDConvexMeshInterpolateDesc& desc);
	
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
};

}	//	namespace Spr
#endif

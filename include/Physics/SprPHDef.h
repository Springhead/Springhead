/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_PHDEF_H
#define SPR_PHDEF_H
#include <Foundation/SprScene.h>

namespace Spr {;
/** \addtogroup gpPhysics	*/
//@{

inline bool approx(const double x, const double y){
	const double eps = 1e-6;
	return ((x==y)
			|| (fabs(x-y) < eps)
			|| (fabs(x/y - 1.0) < eps));
}

inline bool approx(const Vec3d v1, const Vec3d v2){
	return (approx(v1.x, v2.x) && approx(v1.y, v2.y) && approx(v1.z, v2.z));
}

//@}
}	//	namespace Spr

#endif	


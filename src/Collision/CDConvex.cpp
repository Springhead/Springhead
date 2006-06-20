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

namespace Spr{;

//----------------------------------------------------------------------------
//	CDConvex
IF_OBJECT_IMP_ABST(CDConvex, CDShape);

void CDConvex::CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Posed& pose){
	Matrix3f rot;
	pose.Ori().Inv().ToMatrix(rot);
	for(int i=0; i<3; ++i){
		bbmin[i] = std::min(bbmin[i], (float)((pose.Ori() * Support(-rot.col(i)))[i] + pose.Pos()[i]));
		bbmax[i] = std::max(bbmax[i], (float)((pose.Ori() * Support( rot.col(i)))[i] + pose.Pos()[i]));
	}
}

}

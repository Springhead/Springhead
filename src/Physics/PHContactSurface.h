/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONTACTSURFACE_H
#define PHCONTACTSURFACE_H

#include <Physics/PHContactPoint.h>

namespace Spr{;

class PHShapePairForLCP;

class PHContactSurface : public PHContactPoint{
public:
	double	contactArea;
	double  contactRadius;

	/// ZMP
	Vec3d	fpoint;

	/// 交差断面の頂点（接触座標系での値）．トルクの制限のために保持
	std::vector<Vec3d>	section;

	/// コンストラクタ
	PHContactSurface(){}
	PHContactSurface(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1, std::vector<Vec3d> sec);

	// ----- PHConstraintの機能をオーバーライド
	virtual bool Iterate();

	// ----- このクラスで実装する機能
	void	ProjectionTorque  (SpatialVector& fnew);		///< トルク用Projection
};

}

#endif

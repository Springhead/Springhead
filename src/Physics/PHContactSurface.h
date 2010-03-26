/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONTACTSURFACE_H
#define PHCONTACTSURFACE_H

#include <SprPhysics.h>
#include "PHContactPoint.h"

class PHShapePairForLCP;

namespace Spr{;

class PHContactSurface : public PHContactPoint{
public:
	Vec3d	fpoint;		//ZMP
	std::vector<Vec3d>	section;	///< Œð·’f–Ê‚Ì’¸“_iÚGÀ•WŒn‚Å‚Ì’ljDƒgƒ‹ƒN‚Ì§ŒÀ‚Ì‚½‚ß‚É•ÛŽ
	int lastL,lastR;	///‘O‰ñAÅ‹ß“_‚ª‚ ‚Á‚½•Ó‚Ì’¸“_‚ð•\‚·section”Ô†
	virtual void SetConstrainedIndex(bool* con);
	virtual void CompBias();
	virtual void Projection(double& f, int k);
	virtual void IterateLCP();
	PHContactSurface(){}
	PHContactSurface(PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1, std::vector<Vec3d> sec);
	PHContactSurface(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1, std::vector<Vec3d> sec);
	void	ProjectionTorque(SpatialVector& fnew);	///< ƒgƒ‹ƒN—pProjection
	double	CalcSignedArea(Vec3d p, Vec3d q, Vec3d r);	///< ŽOŠpŒ`pqr‚Ì•„†•t‚«–ÊÏŽZo
	bool	CheckInside(int& L, int& R);		///< ì—p“_‚ª–Ê“à‚É‚ ‚é‚©”»’è
	void	SearchClosestPoint(int L, int R);	///< ì—p“_‚Ì–Ê“à‚ÌÅ‹ß“_‚ðŒ©‚Â‚¯‚é
	
	//‰ñ“]–€ŽC—p
	void	IntegrateArea();//ÚG–Ê‚ðÏ•ª
	double	rotateFriction;//‰ñ“]–€ŽC‚É–€ŽCŒW”‚Æ‚’¼R—Í‚ð‚©‚¯‚é‘O‚Ì’l‚ª“ü‚é
	int		segmentationNum;//–Ê‚Ì•ªŠ„”
	double  interval;//ÚG–Ê‚ð‹æØ‚é‚Ý•
};

}

#endif

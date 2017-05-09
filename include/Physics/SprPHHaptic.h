/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */


#ifndef SPR_PHHAPTIC_H
#define SPR_PHHAPTIC_H

#include <Physics/SprPHSolid.h>

namespace Spr{;

struct PHHapticPointerDesc{
	enum HapticRenderMode{
		PENALTY,
		CONSTRAINT,
		DYNAMICS_CONSTRAINT,
	};
};

struct PHHapticPointerIf : public PHSolidIf { // , public PHHapticPointerDesc
	SPR_IFDEF(PHHapticPointer);
	void	SetHapticRenderMode(PHHapticPointerDesc::HapticRenderMode m);
	void	EnableForce(bool b);
	void	EnableFriction(bool b);
	void	SetTimeVaryFriction(bool b);
	bool	GetTimeVaryFriction();
	void	EnableVibration(bool b);
	void	EnableMultiPoints(bool b);
	bool	IsMultiPoints();
	
	void	SetReflexSpring(float s);
	float	GetReflexSpring();
	void	SetReflexDamper(float d);
	float	GetReflexDamper();
	void	SetReflexSpringOri(float s);
	float	GetReflexSpringOri();
	void	SetReflexDamperOri(float d);
	float	GetReflexDamperOri();
	void	SetLocalRange(float r);
	float	GetLocalRange();
	void	SetPosScale(double scale);
	double	GetPosScale();
	void	SetRotationalWeight(double w);
	double	GetRotationalWeight();
	void	SetDefaultPose(Posed p);
	Posed	GetDefaultPose();

	int     NNeighborSolids();
	PHSolidIf*   GetNeighborSolid(int i);
	float   GetContactForce(int i);
};

}

#endif

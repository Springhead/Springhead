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
		PENALTY,				///<	ペナルティ法でProxyの行き先を求める
		CONSTRAINT,				///<	中間表現を拘束条件だと思って、Proxyの行き先を求める
		DYNAMICS_CONSTRAINT,	///<	Proxyに質量と動力学をもたせたCONSTRAINT
	};
	///	haptic renderingのモード
	HapticRenderMode renderMode;
};
struct PHSpringDamperCoeff{
	float spring;
	float damper;
	float rotationSpring;
	float rotationDamper;
	PHSpringDamperCoeff();
};
PHSpringDamperCoeff operator * (float s, PHSpringDamperCoeff);
PHSpringDamperCoeff operator * (PHSpringDamperCoeff, float s);

struct PHHapticPointerIf : public PHSolidIf { // , public PHHapticPointerDesc
	SPR_IFDEF(PHHapticPointer);
	void	SetHapticRenderMode(PHHapticPointerDesc::HapticRenderMode m);
	void	EnableForce(bool b);
	void	EnableFriction(bool b);
	void	EnableTimeVaryFriction(bool b);
	bool	IsTimeVaryFriction();
	void	EnableVibration(bool b);
	void	EnableMultiPoints(bool b);
	bool	IsMultiPoints();
	
	void	SetReflexCoeff(const PHSpringDamperCoeff& r);
	const PHSpringDamperCoeff& GetReflexCoeff();
	void	SetFrictionCoeff(const PHSpringDamperCoeff& r);
	const PHSpringDamperCoeff& GetFrictionCoeff();
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

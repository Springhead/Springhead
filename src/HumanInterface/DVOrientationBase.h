/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DVORIENTATION_BASE_H
#define VR_DVORIENTATION_BASE_H

#include <HumanInterface/HIVirtualDevice.h>
#include <base/Affine.h>

namespace Spr {
//----------------------------------------------------------------------------
//	DVOrientationBase

///	向きを返すデバイス
class SPR_DLL DVOrientationBase:public HIVirtualDevice{
public:
	///	向きの取得(回転行列であらわす)
	virtual Vec3f GetEuler() = 0;
	///	向きの取得(回転行列であらわす)
	virtual void GetMatrix(Matrix3f& ori) = 0;
	///	デバイスの名前
	virtual const char* Name() const = 0;
	///	デバイスの種類
	static const char* TypeS() { return "Orientation"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif
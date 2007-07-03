/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CREyeControllerIf_H
#define SPR_CREyeControllerIf_H

#include <SprFoundation.h>
#include <SprPhysics.h>

namespace Spr{;

//@{

struct CREyeControllerIf : SceneObjectIf{
	IF_DEF(CREyeController);
	virtual void SetLeftEyeSolid(PHSolidIf* eyeSolid)= 0;
	virtual void SetRightEyeSolid(PHSolidIf* eyeSolid)= 0;
	virtual void Step()= 0;
};

//@}

}

#endif//SPR_CREyeControllerIf_H

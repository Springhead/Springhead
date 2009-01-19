/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRDEBUGLINKBODY_H
#define CRDEBUGLINKBODY_H

#include <Springhead.h>
#include <Foundation/Object.h>
#include <Physics/PHConstraint.h>
#include <Physics/PhysicsDecl.hpp>

#include <vector>
#include "CRBody.h"

//@{
namespace Spr{;

class CRDebugLinkBody: public CRBody, public CRDebugLinkBodyDesc{

private:
	void SolidFactory(CDShapeMode m);
	void CreateBody();
	void InitBody();
	void InitContact();
	void InitControlMode(PHJointDesc::PHControlMode m = PHJointDesc::MODE_POSITION);
public:
	SPR_OBJECTDEF(CRDebugLinkBody);
	ACCESS_DESC(CRDebugLinkBody);

	CRDebugLinkBody(){}
	CRDebugLinkBody(const CRDebugLinkBodyDesc& desc, CRCreatureIf* c =NULL)
		: CRDebugLinkBodyDesc(desc), CRBody((const CRBodyDesc&)desc, c)
	{
		CreateBody();
		InitBody();
		InitContact();
		InitControlMode();
	}
	virtual void Init();
	int NBallJoints(){return joNBallJoints;}
	int NHingeJoints(){return joNHingeJoints;}

};

}
//@}

#endif //< end of CRDEBUGLINKBODY_H
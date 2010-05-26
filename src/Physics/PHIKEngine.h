/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_IKENGINE_H
#define PH_IKENGINE_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHEngine.h>

#include <vector>
#include <set>
#include <map>

namespace Spr{;

class PHIKActuator;
class PHIKEndEffector;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// IK�G���W��
// 

class PHIKEngine : public PHEngine{
public:
	SPR_OBJECTDEF(PHIKEngine);

	// --- --- --- --- ---

	std::vector<PHIKActuator*>		actuators;
	std::vector<PHIKEndEffector*>	endeffectors;

	// --- --- --- --- ---

	bool	bEnabled;
	size_t	numIter;

	// --- --- --- --- --- --- --- --- --- ---

	PHIKEngine();

	int  GetPriority() const {return SGBP_INVERSEKINEMATICS;}
	void Step();
	void Clear();
	void Enable(bool bEnabled){ this->bEnabled = bEnabled; }
	bool IsEnabled(){ return this->bEnabled; }
	void SetNumIter(int numIter){ this->numIter = numIter; }

	// --- --- --- --- ---

	PHIKActuator*		CreateIKActuator(const IfInfo* ii, const PHIKActuatorDesc& desc);
	PHIKEndEffector*	CreateIKEndEffector(const IfInfo* ii, const PHIKEndEffectorDesc& desc);
	virtual bool		AddChildObject(ObjectIf* o);
};

}

#endif
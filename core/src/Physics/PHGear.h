/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHGEAR_H
#define PHGEAR_H

#include "../Foundation/Object.h"
#include "../Physics/PHConstraint.h"
#include <Physics/SprPHJoint.h>
#include "PhysicsDecl.hpp"

namespace Spr{;

class PHScene;
class PHConstraintEngine;
class PH1DJoint;

/**
	ギア：１自由度関節を連動させる拘束
 */
class PHGear : public PHConstraintBase{
public:
	SPR_OBJECTDEF(PHGear);
	SPR_DECLMEMBEROF_PHGearDesc;

	PHScene*			scene;
	PHConstraintEngine* engine;
	PH1DJoint*		    joint[2];		///< 連動させる関節
	bool				bArticulated;

public:
	/// Objectの仮想関数
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual size_t NChildObject()const{ return 2; }
	virtual ObjectIf* GetChildObject(size_t pos);

	bool	IsFeasible();
	bool	IsArticulated();
	bool	IsCyclic();
	
	/// PHGearIfの実装
	void    Enable   (bool bEnable){ bEnabled = bEnable; }
	bool    IsEnabled(){ return bEnabled; }
	void    SetRatio (double r){ ratio = r; }
	double  GetRatio () const { return ratio; }
	void    SetOffset(double o){ offset = o; }
	double  GetOffset() const { return offset; }
	void    SetMode  (int m){ mode = m; }
	int     GetMode  (){ return mode; }

	/// PHConstraintBaseの仮想関数
	virtual void SetupAxisIndex    ();
	virtual void Setup             ();
	virtual bool Iterate           ();
	virtual void CompResponse      (double df, int i);
	
	PHGear(const PHGearDesc& desc = PHGearDesc());
};

class PHGears : public std::vector< UTRef<PHGear> >{
public:
	PHGear* FindByJointPair(PH1DJoint* lhs, PH1DJoint* rhs){
		for(iterator it = begin(); it != end(); it++){
			if((*it)->joint[0] == lhs && (*it)->joint[1] == rhs)
				return *it;
			if((*it)->joint[1] == lhs && (*it)->joint[0] == rhs)
				return *it;
		}
		return NULL;
	}
};

}

#endif

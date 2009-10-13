/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWVIRTUALCOUPLING_H
#define  FWVIRTUALCOUPLING_H

#include <Springhead.h>
#include <Framework/FWInteractAdaptee.h>

namespace Spr{;

class FWVirtualCoupling : public FWInteractAdaptee{
private:
	std::vector<PHSolidIf*> vcSolid;	
	std::vector<PHJointIf*> vcJoint;	
protected:
	volatile int hapticcount;
	volatile bool bSync;
	bool bCalcPhys;
	virtual void CreateHapticLoop(){}
public:
	FWVirtualCoupling();

	virtual void UpdatePointer();
	void		 UpdatePointerDirect(); //直接ポインタを更新する（力出力なし）
	virtual void Clear();
	virtual void Init(){
			FWInteractAdaptee::Init();
	};
	virtual void Step();
	void UpdateInterface();
	void CreateVCPointer();

};
}
#endif
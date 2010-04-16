/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FWMULTIRATE_HAPTIC_H
#define FWMULTIRATE_HAPTIC_H

#include <Springhead.h>
#include <Framework/FWInteractAdaptee.h>

namespace Spr{;

class FWMultiRateHaptic : public FWInteractAdaptee{
protected:
	volatile int hapticcount;
	volatile bool bSync;
	bool bCalcPhys;
	FWHapticLoopBase* hapticLoop;
	virtual void CreateHapticLoop(){}
public:
	FWMultiRateHaptic();
	FWHapticLoopBase* GetHapticLoop(){ return hapticLoop; }

	virtual void UpdatePointer(){}
	void Sync();						// 各スレッドを同期させる
	virtual void SyncHaptic2Phsyic(){};	// 継承先で実装
	virtual void SyncPhysic2Haptic(){};	// 継承先で実装
	void SyncPointer();
	virtual void Clear();

	virtual void Init(){
		FWInteractAdaptee::Init();
	}
	virtual void Step(){}

};
}
#endif
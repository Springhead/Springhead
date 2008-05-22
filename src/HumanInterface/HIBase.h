/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HIBASE_H
#define HIBASE_H

#include <Foundation/Object.h>
#include <SprHumanInterface.h>
#include <set>

namespace Spr{;
class HIRealDevice;

///	The base class of human interface classes.
class HIBase: public NamedObject{
protected:
	static std::set<HIRealDevice*> realDevices;
	static int deviceUpdateStep;
	int updateStep;
	bool isGood;
public:
	SPR_OBJECTDEF_ABST(HIBase);
	///
	HIBase(){ updateStep = deviceUpdateStep; isGood=false;}

	///	SDKの取得
	HISdkIf* GetSdk();
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalibration(){ return true; }
	///	キャリブレーション処理
	virtual bool Calibration(){ return true; }
	///	キャリブレーションのあとに行う処理
	virtual bool AfterCalibration(){ return true; }
	///	Is this human interface good for use?
	bool IsGood(){ return isGood; }
	///	Update state of the interface; read from/write to real device.
	virtual void Update(float dt);
	/**	Add an real device to the dependency list of real devices. 
		Update() will call real device's Update() function.	*/
	void AddDeviceDependency(HIRealDeviceIf* rd);
	///	Clear the dependency list.
	static void ClearDeviceDependency();
	///	Init human interface
	virtual bool Init(const void* desc){ return false; }
};
class HIPosition:public HIBase{
public:
	SPR_OBJECTDEF(HIPosition);
	virtual Vec3f GetPosition();
};
class HIOrientation:public HIBase{
public:
	SPR_OBJECTDEF(HIOrientation);
	virtual Quaternionf GetOrientation();
};

class HIPose:public HIPosition{
public:
	SPR_OBJECTDEF(HIPose);
	virtual Quaternionf GetOrientation();
	virtual Posef GetPose();
};
	
}


#endif


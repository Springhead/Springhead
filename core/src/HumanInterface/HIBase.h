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
#include <Physics/PHSolid.h>

namespace Spr{;
class HIRealDevice;

///	The base class of human interface classes.
class HIBase: public NamedObject{
public:
	struct RealDevices:public std::set<HIRealDevice*>, public UTRefCount{};
protected:
	static UTRef<RealDevices> GetRealDevices();
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

/*class HIPosition:public virtual HIBase{
public:
	SPR_OBJECTDEF(HIPosition);
	virtual Vec3f GetPosition();
};

class HIOrientation:public virtual HIBase{
public:
	SPR_OBJECTDEF(HIOrientation);
	virtual Quaternionf GetOrientation();
};*/

//class HIPose:public HIPosition, public HIOrientation{
class HIPose: public HIBase{
public:
	SPR_OBJECTDEF(HIPose);
	virtual Vec3f		GetPosition(){ return Vec3f(); }
	virtual Quaternionf GetOrientation(){ return Quaternionf(); }
	virtual Posef		GetPose(){
		Posef rv;
		rv.Ori() = GetOrientation();
		rv.Pos() = GetPosition();
		return rv;
	}
	virtual Affinef		GetAffine(){
		Affinef aff;
		GetPose().ToAffine(aff);
		return aff;
	}
	virtual Vec3f GetVelocity(){ return Vec3f(); }
	virtual Vec3f GetAngularVelocity(){ return Vec3f(); }
};


class SPR_DLL HIHaptic: public HIPose{
protected:
	Vec3f vel, angVel;
	float alpha;
	Vec3f lastPos;
	Quaternionf lastOri;
public:
	SPR_OBJECTDEF_ABST(HIHaptic);
	///
	HIHaptic(){ alpha = 0.8f; }
	///
	virtual ~HIHaptic(){}
	virtual void Update(float dt);

	/**@name	デバイスの状態	*/
	//@{
	///	デバイスの実際の提示トルクを返す
	virtual Vec3f GetTorque(){ return Vec3f(); }
	///	デバイスの実際の提示力を返す
	virtual Vec3f GetForce(){ return Vec3f(); }
	///	デバイスの目標出力とトルク出力を設定する
	virtual void SetForce(const Vec3f& f, const Vec3f& t = Vec3f()){}
	///	デバイスの速度を返す
	virtual Vec3f GetVelocity(){ return vel; }
	///	デバイスの角速度を返す
	virtual Vec3f GetAngularVelocity(){ return angVel; }
	//@}
};

class SPR_DLL HIHapticDummy : HIHapticDummyDesc, public HIHaptic{
public:
	SPR_OBJECTDEF(HIHapticDummy);
	HIHapticDummy() {}
	///
	HIHapticDummy(const HIHapticDummyDesc& desc) { *(HIHapticDummyDesc*)this = desc; }
	///
	virtual ~HIHapticDummy() {}
	void SetPose(Posed p) { pose = p; }
	void SetVelocity(Vec3d v) { vel = v; }
	void SetAngularVelocity(Vec3d v) { angVel = v; }
	virtual Vec3f		GetPosition() { return pose.Pos(); }
	virtual Quaternionf GetOrientation() { return pose.Ori(); }
};

}


#endif


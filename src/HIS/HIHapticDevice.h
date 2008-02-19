#ifndef VR_HAPTIC_HDHAPTIC_H
#define VR_HAPTIC_HDHAPTIC_H

#include "HIBase.h"
#include <Device/DRRealDevice.h>

namespace Spr {

//----------------------------------------------------------------------------
//	HIHapticDevice

///	力覚提示装置の基底クラス
class SPR_DLL HIHapticDevice: public HIBase{
protected:
	int updateStep;
	bool bGood;
public:
	HIOBJECTDEFABST(HIHapticDevice);
	/**@name	初期化など	*/
	//@{
	/// 
	HIHapticDevice(){ updateStep = deviceUpdateStep; bGood=false;}
	virtual ~HIHapticDevice(){}
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ return true; }
	///	キャリブレーション処理
	virtual bool Calib(){ return true; }
	///	キャリブレーションのあとに行う処理
	virtual bool AfterCalib(){ return true; }
	///
	void AddRealDeviceDependency(DRRealDevice* rd);
	///
	static void ClearRealDeviceDependency();
	//@}
	/**	デバイスの状態を更新する．	*/
	virtual void Update(float dt);
	///	
	bool IsGood(){ return bGood; }
};

//----------------------------------------------------------------------------
//	HIForceDevice3D

///	並進回転6自由度のデバイス
class SPR_DLL HIForceDevice6D: public HIHapticDevice{
protected:
	float alpha;
	Vec3f vel, angVel;
	Vec3f lastPos;
	Quaternionf lastOri;
public:
	HIOBJECTDEFABST(HIForceDevice6D);
	///
	HIForceDevice6D();
	///
	virtual ~HIForceDevice6D(){}
	virtual void Update(float dt);

	/**@name	デバイスの状態	*/
	//@{
	///	デバイスの現在位置を返す
	virtual Vec3f GetPos()=0;
	///	デバイスの向きを返す
	virtual Quaternionf GetOri()=0;
	///	デバイスの速度を返す
	virtual Vec3f GetVel(){ return vel; }
	///	デバイスの角速度を返す
	virtual Vec3f GetAngVel(){ return angVel; }
	///	デバイスの実際の提示トルクを返す
	virtual Vec3f GetTorque()=0;
	///	デバイスの実際の提示力を返す
	virtual Vec3f GetForce()=0;
	///	デバイスの目標出力とトルク出力を設定する
	virtual void SetForce(const Vec3f& f, const Vec3f& t)=0;
	//@}
};

/*class SPR_DLL HIForceDevice3DPlus: public HIForceDevice3D{
	virtual Vec3f GetRot()=0;
};
*/

//----------------------------------------------------------------------------
//	HIForceDevice3D
///	並進3自由度のデバイス
class SPR_DLL HIForceDevice3D: public HIForceDevice6D{
public:
	HIOBJECTDEFABST(HIForceDevice3D);
	HIForceDevice3D();
	///	デバイスの目標力出力を設定する
	virtual void SetForce(const Vec3f& f)=0;
	virtual void SetForce(const Vec3f& f, const Vec3f& t){ SetForce(f); }
	///	デバイスの実際の提示トルクを返す
	virtual Vec3f GetTorque(){ return Vec3f(); }
	virtual Quaternionf GetOri(){ return Quaternionf(); } 
};

//----------------------------------------------------------------------------
//	HIForceDevice7D
///	7自由度のデバイス
class SPR_DLL HIForceDevice7D: public HIForceDevice6D{
public:
	virtual ~HIForceDevice7D(){}
	///	デバイスの把持距離を返す
	virtual float GetGap()=0;
	///	デバイスの提示把持力を返す
	virtual float GetGrasp()=0;
	///	デバイスの目標力把持力を設定する
	virtual void SetGrasp(float g)=0;
};

}	//	namespace Spr
#endif

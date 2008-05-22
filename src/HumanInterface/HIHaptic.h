#ifndef HI_HAPITC_INTERFACE_H
#define HI_HAPITC_INTERFACE_H

#include "HIBase.h"
#include <Device/DRRealDevice.h>

namespace Spr {
//----------------------------------------------------------------------------
//	HIForceInterface6D

///	並進回転6自由度のデバイス
class SPR_DLL HIForceInterface6D: public HIPose{
protected:
	float alpha;
	Vec3f vel, angVel;
	Vec3f lastPos;
	Quaternionf lastOri;
public:
	SPR_OBJECTDEF_ABST(HIForceInterface6D);
	///
	HIForceInterface6D();
	///
	virtual ~HIForceInterface6D(){}
	virtual void Update(float dt);

	/**@name	デバイスの状態	*/
	//@{
	///	デバイスの現在位置を返す
	virtual Vec3f GetPosition()=0;
	///	デバイスの向きを返す
	virtual Quaternionf GetOrientation()=0;
	///	デバイスの速度を返す
	virtual Vec3f GetVelocity(){ return vel; }
	///	デバイスの角速度を返す
	virtual Vec3f GetAngularVelocity(){ return angVel; }
	///	デバイスの実際の提示トルクを返す
	virtual Vec3f GetTorque()=0;
	///	デバイスの実際の提示力を返す
	virtual Vec3f GetForce()=0;
	///	デバイスの目標出力とトルク出力を設定する
	virtual void SetForce(const Vec3f& f, const Vec3f& t)=0;
	//@}
};

}	//	namespace Spr
#endif

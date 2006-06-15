#ifndef HIORIENTATION_H
#define HIORIENTATION_H

#include "HIHapticDevice.h"
#include <Device/DVDeviceManager.h>
#include <Device/DVOrientationBase.h>

namespace Spr {;

class SPR_DLL HIOrientation: public HIForceDevice6D{
public:
	///	デバイスのタイプ
	HIOBJECTDEFABST(HIOrientation);

	DVOrientationBase* dev;
	/// 
	HIOrientation(){ bGood = false; }
	virtual ~HIOrientation(){}

	bool IsGood(){return bGood;}

	bool Init(DVDeviceManager& devMan);

	void Update(float dt);

	///	位置の取得
	Vec3f GetPos(){ return pos; }
	///	力の取得
	Vec3f GetForce(){ return force; }
	///	トルクの取得
	Vec3f GetTorque(){ return torque; }
	///	角度の取得
	Quaternionf GetOri(){ return ori; }

	/// 位置の設定
	void SetPos(Vec3f p){ pos = p; }
	/// 姿勢の設定
	void SetOri(Quaternionf o){ ori = o; }
	///	並進力・回転力の設定
	void SetForce(const Vec3f& f, const Vec3f& t){ force = f; torque = t; }
	void SetForce(const Vec3f& f){ force = f; }
	///	最低の力を設定する
	void SetMinForce(){ force = torque = Vec3f(); }	


	///	キャリブレーション(特に何もしない)
	bool Calib(){ return true; }

protected:
	// マウスがアクティブかどうかのフラグ
	bool bGood;
	// ポインタにかかる並進力
	Vec3f force;
	// ポインタにかかる回転力
	Vec3f torque;
	// ポインタの位置。Update()で更新される
	Vec3f pos;
	// ポインタの姿勢。Update()で更新される
	Quaternionf ori;
};

}	//	namespace Spr
#endif

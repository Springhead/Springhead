// HISpidar8MotorTest.h: HISpidar8MotorTest クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISPIDAR8MOTORTEST_H__AE02CC6C_28C1_4ED6_A6F2_F0C231F053AC__INCLUDED_)
#define AFX_HISPIDAR8MOTORTEST_H__AE02CC6C_28C1_4ED6_A6F2_F0C231F053AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "HIHapticDevice.h"

//#include "HIHapticDevice.h"
//#include "HISpidarMotor.h"
#include "HIS.h"
#include "Device/DVDeviceManager.h"
#include <vector>


namespace Spr {

//class HISpidar8MotorTest : public HDForceDevice3D  
class SPR_DLL HISpidar8MotorTest : public HIHapticDevice  
{
public:
	HISpidar8MotorTest();
	virtual ~HISpidar8MotorTest();

public:
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev);
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF);
	virtual void Update(float dt);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	最低張力を出力
	void SetMinForce();
	
	/// 力の入力
	void SetForce(int ch, float force);
	int GetCount(int ch);

protected:
	///	モータ
	HISpidarMotor motor[8];

	float realtens[8];	// computed tension + min tension
	float tension[8];	// computed tension

};

}//namespace Spr

#endif // !defined(AFX_HISPIDAR8MOTORTEST_H__AE02CC6C_28C1_4ED6_A6F2_F0C231F053AC__INCLUDED_)

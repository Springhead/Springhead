#ifndef VR_HAPTIC_HDSPIDARMOTORANGLE_H
#define VR_HAPTIC_HDSPIDARMOTORANGLE_H

#include "HISpidarMotor.h"

namespace Spr {

class SPR_DLL HISpidarMotorAngle : public HISpidarMotor
	{
public:
	///　糸の角度　／　パルス数
	float anglePerPulse;
	///　角度用のエンコーダカウンタ
	DVCounterBase* counter_angle;
	///　角度
	float ang;

public:
	///	
	HISpidarMotorAngle();
	///		virtual ~HISpidarMotorAngle(){}


	///　現在の糸の角度の設定
	void Angle(float a){
		if (counter_angle)
			counter_angle -> Count(long(1 / anglePerPulse));
	}

	///現在の糸の角度
	float Angle(){
		if (counter_angle) return
			counter_angle -> Count() * anglePerPulse;
		else return 0;
	}
};
}
#endif
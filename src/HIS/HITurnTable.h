#ifndef HI_TURNTABLE_H
#define HI_TURNTABLE_H

#include "Device/DVDeviceManager.h"
#include "Device/DVDaBase.h"
#include "Device/DVOrientationBase.h"
#include <vector>

namespace Spr {;
/**	ターンテーブルと足踏みセンサー
*/
class SPR_DLL HITurnTable{
public:
	DVOrientationBase* oriSensor;
	DVDaBase* da;
	bool bControl;

	HITurnTable();
	virtual ~HITurnTable();
	
	bool Init(DVDeviceManager& dev);
	void Stop();
	void Start();
	void Step(float);
	void Reset();
	float GetAngle(){return angle;}
	float GetTurnVel(){ return turnVel; }

	void PrintData(std::ostream& os) const;
	bool printTurnData;

private:
	float angle;
	float nowAngle , startAngle;
	float turnVel;
	unsigned long dwTime, dwLastTime;

	float printOriAngle;
	float printPressureAngle;
};

}
#endif

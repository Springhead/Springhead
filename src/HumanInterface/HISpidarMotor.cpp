#include "HumanInterface.h"
#ifdef _WIN32
 #pragma hdrstop
#endif

#include "HISpidarMotor.h"

namespace Spr {

HISpidarMotor::HISpidarMotor(){
	maxForce = 20.0f;
	minForce = 0.5f;
	voltPerNewton = 0.365296803653f;
	currentPerVolt = 1.0f;
	lengthPerPulse = 2.924062107079e-5f;
	da = NULL;
	counter = NULL;
	ad = NULL;
	force = 0;
	
	voltageInt = 0;
	currentInt = 0;
	lengthLast = 0;
	vdd = 21.11f;
	calibMat.resize(8, 4);
	calibVec.resize(8);
	calibMat.clear();
	calibVec.clear();
}
float HISpidarMotor::GetVelocity(){
	if (!ad || !calibResult[2]) return 0;
	float generatedVoltage = vdd - ad->Voltage() - GetCurrent()*calibResult[0];
	return generatedVoltage / calibResult[2];
//	float generatedVoltage = vdd - ad->Voltage() - GetCurrent()*3.45f;
//	return generatedVoltage / (-1.94f);
}
void HISpidarMotor::Calibrate(bool bUpdate){
	float current = GetCurrent();
	float length = GetLength();
	if (bUpdate){
		Vec4f v(currentInt, currentLast-current, length-lengthLast, timeInt);
		int maxPos = 0;
		float maxIp = 0;
		float alpha = 0.9f;
		calibMat *= alpha;
		calibVec *= alpha;
		for(unsigned i=0; i<calibMat.height(); ++i){
			float ip = calibMat[i].norm() ? 1/calibMat[i].norm() * calibMat[i].unit() * v.unit() : 100;
			if (ip < 0) ip = -ip;
			if (ip > maxIp){
				maxIp = ip;
				maxPos = i;
			}
		}
		calibMat[maxPos] = v;
		calibVec[maxPos] = -voltageInt;
		calibResult = (calibMat.trans()*calibMat).inv() * calibMat.trans() * calibVec;
	}

	currentLast = current;
	lengthLast = length;
	voltageInt = 0;
	currentInt = 0;
	timeInt = 0;
}
void HISpidarMotor::GetCalibrationData(float dt){
	if (!ad) return;
/*	float vMin=1000;
	float vMax=0;
	for(int i=0; i<10; ++i){
		float v = ad->Voltage();
		ad->RealDevice()->Update();
		if (v < vMin) vMin = v;
		if (v > vMax) vMax = v;
	}
	DSTR << vMax-vMin << " vMin" << vMin << " vMax" << vMax << std::endl;
*/
	voltageInt += ad->Voltage() * dt;
	currentInt += GetCurrent() * dt;
	timeInt += dt;
}
void HISpidarMotor::GetVdd(){
	if (!ad || !da) return;
	da->Voltage(0);
	da->RealDevice()->Update();
	vdd = 0;
	const int MEASURE_TIMES = 10;
	for(int i=0; i<MEASURE_TIMES; ++i){
		da->RealDevice()->Update();
		vdd += ad->Voltage();
	}
	vdd /= MEASURE_TIMES;
}

}	//	namespace Spr

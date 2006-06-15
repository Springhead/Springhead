#include "HIS.h"
#pragma hdrstop

//#include "HITurnTable.h"

namespace Spr{

const float V_CENTER=0.5f;
const float V_FULLSCALE=2.5f;
HITurnTable::HITurnTable(){
	oriSensor = NULL;
	da = NULL;
	dwTime = 0;
	nowAngle = 0;
	startAngle = 0;
	turnVel = 0;
	angle = 0;
	bControl = true;
	printTurnData = false;
}

HITurnTable::~HITurnTable(){
}

void HITurnTable::Stop(){
	bControl = false;
}
void HITurnTable::Start(){
	bControl = true;
}

bool HITurnTable::Init(DVDeviceManager& dev){
	dev.VPool().Rent(oriSensor);
	if (!oriSensor) return false;
	dev.VPool().Rent(da);
	if (!da){
		dev.VPool().Return(oriSensor);
		oriSensor = NULL;
		return false;
	}
	startAngle = oriSensor->GetEuler().Z();
	da->Voltage(V_FULLSCALE * V_CENTER);
	dwTime = GetTickCount();
	return true;
}
void HITurnTable::Step(float inner){
	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;
	///トーキンセンサを使って使用者の向きを検出する場合
	std::cout << "Tokin exit? : " << (oriSensor != NULL) << std::endl;
	if (oriSensor) {
		nowAngle = -(oriSensor->GetEuler().Z() - startAngle);
		float tokinAngle = oriSensor->GetEuler().Z();
		std::cout << "nowAngle by Tokin StartAngle : " << startAngle << std::endl;
		std::cout << "nowAngle by Tokin oriSensorAngle : " << tokinAngle << std::endl;
	}

	///トーキンセンサと圧力センサの検出データを比較する際のデータ
	printOriAngle = nowAngle;
	printPressureAngle = inner;
	std::cout << "nowAngle by Tokin : " << printOriAngle << std::endl;


	///圧力センサを使って使用者の向きを検出する場合
	if (oriSensor){ 
		nowAngle = inner;
//		std::cout << "nowAngle:" << nowAngle << std::endl;
	}

	const float acc = 2.0f;	//	[rad/sec^2]
	const float k = 2.0f;
	const float gap = 5;
	if(nowAngle > Rad(gap)){
		if(turnVel > - (k * nowAngle))
			turnVel -= acc*dt;
		if(turnVel < - (k * nowAngle))
			turnVel += acc*dt;
	}else if(nowAngle < -Rad(gap)){
		if(turnVel < - (k * nowAngle))
			turnVel += acc*dt;
		if(turnVel > - (k * nowAngle))
			turnVel -= acc*dt;
	}else if(turnVel < 0){
		turnVel += acc*dt;
		if (turnVel > 0) turnVel = 0;
	}else if(turnVel > 0){
		turnVel -= acc*dt;
		if (turnVel < 0) turnVel = 0;
	}
/*	2.5v のとき、ターンテーブルは時計回りに19.2[rpm]
	0v のとき、ターンテーブルは反時計回りに19.3[rpm]
	1.25v で 19.2[rpm] = 2.01	[rad/sec]
	0.622 [V/rad]
*/
	if (turnVel > 1.0f) turnVel = 1.0f;
	if (turnVel < -1.0f) turnVel = -1.0f;
//	const float voltPerRad = 0.622f;
	const float voltPerRad = 1.244f;
	if (bControl){
		if (da) da->Voltage((turnVel * voltPerRad) +1.25f);
	}else{
		da->Voltage(1.25);
	}
	angle += turnVel * dt;
}

void HITurnTable::Reset(){
	if (oriSensor){
		startAngle = oriSensor->GetEuler().Z();
//		startAngle = 3.141592 / 2.0;
	}
}

///トーキンセンサと圧力センサから検出された使用者の向きのデータを出力
void HITurnTable::PrintData(std::ostream& os) const{
	os << printOriAngle << "," << printPressureAngle << "," << dwTime << std::endl;
}

}

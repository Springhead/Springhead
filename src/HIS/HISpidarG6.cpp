#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
//#include "HISpidarG6.h"

namespace Spr {;
HIOBJECTIMP(HISpidarG6, HIForceDevice6D);
//----------------------------------------------------------------------------
HISpidarG6::HISpidarG6(){
	SetWeight();
}
HISpidarG6::~HISpidarG6(){SetMinForce();}
bool HISpidarG6::Init(DVDeviceManager& dev, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF){
	//	計算のための定数の設定
	nRepeat = 2;
	sigma = 0.001f; //sigma=sigma*sigma
	motor.resize(nMotor);
	VVector<float> minForce, maxForce;
	minForce.resize(nMotor);
	maxForce.resize(nMotor);
	for(int i=0; i<nMotor; ++i){
		motor[i].minForce = minForce[i] = minF;
		motor[i].maxForce = maxForce[i] = maxF;
		motor[i].pos = motorPos[i][0];
		motor[i].jointPos = motorPos[i][1];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
	}
	HISpidarCalc6Dof::Init(6, minForce, maxForce);

	//	ドライバの取得
	unsigned int i;
	for(i=0; i<motor.size(); ++i){
		if (!dev.VPool().Rent(motor[i].da)) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<motor.size()) return false;
	for(i=0; i<motor.size(); ++i){
		if (!dev.VPool().Rent(motor[i].counter)) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	button.resize(8);
	for(i=0; i<button.size(); ++i){
		if (!dev.VPool().Rent(button[i])) break;
		AddRealDeviceDependency(button[i]->RealDevice());
	}

	SetMinForce();
	BeforeCalib();
	Calib();
	AfterCalib();
	bGood = true;
	return true;
}
bool HISpidarG6::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(unsigned i=0; i<motor.size(); i++) motor[i].SetLength( (motor[i].pos - motor[i].jointPos).norm() );
	lengthDiffAve.clear();
	for(int i=0; i<4; ++i) HISpidarCalc6Dof::Update();	//	姿勢を更新
	return true;
}
void HISpidarG6::Update(float dt){
	HIForceDevice6D::Update(dt);
	HISpidarCalc6Dof::Update();
	for(unsigned int i=0; i<motor.size(); ++i){
		motor[i].SetForce(Tension()[i]);
	}
}
void HISpidarG6::SetMinForce(){
	for(unsigned int i=0; i<motor.size(); ++i){
		motor[i].SetForce(motor[i].minForce);
	}
	Update(0.001f);
}
void HISpidarG6::MakeWireVec(){
	for(unsigned int i=0; i<motor.size(); ++i){
		wireDirection[i] = motor[i].pos - (ori*motor[i].jointPos + pos);
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
		wireMoment[i] = (ori * motor[i].jointPos) ^ wireDirection[i];
	}
}
void HISpidarG6::UpdatePos(){
}
void HISpidarG6::MeasureWire(){
	for(unsigned int i=0; i<motor.size(); ++i){
		measuredLength[i] = motor[i].GetLength();
	}	
}

}//	namespace Spr

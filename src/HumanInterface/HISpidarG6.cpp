#include "HumanInterface.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
#include "HISpidarG6.h"

namespace Spr {;


HISpidarG6Desc::HISpidarG6Desc(){
	assert(0);
}
void HISpidarG6Desc::Init(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF, float maxF){
	assert(0);
}
void HISpidarG6Desc::Init(char* type){
	assert(0);
}

//----------------------------------------------------------------------------
HISpidarG6::HISpidarG6():bGood(false){
	SetWeight();
}
HISpidarG6::~HISpidarG6(){SetMinForce();}
bool HISpidarG6::Init(HISdkIf* sdk, const void* pDesc){
	HISpidarG6Desc& desc = *(HISpidarG6Desc*)pDesc;
	//	計算のための定数の設定
	nRepeat = 2;
	sigma = 0.001f; //sigma=sigma*sigma
	int nMotor = desc.motors.size();
	motors.resize(nMotor);
	VVector<float> minForce, maxForce;
	minForce.resize(desc.motors.size());
	maxForce.resize(desc.motors.size());
	for(unsigned i=0; i<desc.motors.size(); ++i){
		motors[i].SetDesc(&desc.motors[i]);
		minForce[i] = desc.motors[i].minForce;
		maxForce[i] = desc.motors[i].maxForce;
	}
	HISpidarCalc6Dof::Init(6, minForce, maxForce);

	//	ドライバの取得
	unsigned int i;
	for(i=0; i<motors.size(); ++i){
		motors[i].da = sdk->RentVirtualDevice(DVDaBase::TypeS())->Cast();
		if (!motors[i].da) break;
		AddDeviceDependency(motors[i].da->RealDevice()->Cast());
	}
	if (i<motors.size()) return false;
	for(i=0; i<motors.size(); ++i){
		motors[i].counter = sdk->RentVirtualDevice(DVCounterBase::TypeS())->Cast();
		if (!motors[i].counter) break;
		AddDeviceDependency(motors[i].counter->RealDevice()->Cast());
	}
	buttons.resize(8);
	for(i=0; i<buttons.size(); ++i){
		buttons[i] = sdk->RentVirtualDevice(DVPioBase::TypeS())->Cast();
		if (!buttons[i]) break;
		AddDeviceDependency(buttons[i]->RealDevice()->Cast());
	}

	SetMinForce();
	BeforeCalibration();
	Calib();
	AfterCalibration();
	bGood = true;
	return true;
}
bool HISpidarG6::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(unsigned i=0; i<motors.size(); i++) motors[i].SetLength( (motors[i].pos - motors[i].jointPos).norm() );
	lengthDiffAve.clear();
	for(int i=0; i<4; ++i) HISpidarCalc6Dof::Update();	//	姿勢を更新
	return true;
}
void HISpidarG6::Update(float dt){
	HIForceInterface6D::Update(dt);
	HISpidarCalc6Dof::Update();
	for(unsigned int i=0; i<motors.size(); ++i){
		motors[i].SetForce(Tension()[i]);
	}
}
void HISpidarG6::SetMinForce(){
	for(unsigned int i=0; i<motors.size(); ++i){
		motors[i].SetForce(motors[i].minForce);
	}
	Update(0.001f);
}
void HISpidarG6::MakeWireVec(){
	for(unsigned int i=0; i<motors.size(); ++i){
		wireDirection[i] = motors[i].pos - (ori*motors[i].jointPos + pos);
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
		wireMoment[i] = (ori * motors[i].jointPos) ^ wireDirection[i];
	}
}
void HISpidarG6::UpdatePos(){
}
void HISpidarG6::MeasureWire(){
	for(unsigned int i=0; i<motors.size(); ++i){
		measuredLength[i] = motors[i].GetLength();
	}	
}

}//	namespace Spr

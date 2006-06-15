// HISpidar8MotorTest.cpp: HISpidar8MotorTest クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "HIS.h"
#pragma hdrstop
///#include "HISpidar8MotorTest.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr {

HISpidar8MotorTest::HISpidar8MotorTest()
{
    for (int i=0;i<8;i++){
        tension[i] = 0;
        realtens[i] = 0;
    }

}

HISpidar8MotorTest::~HISpidar8MotorTest()
{

}

// links the HD to VD and RD
bool HISpidar8MotorTest::Init(DVDeviceManager& dev){
	//	ドライバの取得
	int i;
	for(i=0; i<8; ++i){
		if (!dev.VPool().Rent(motor[i].da)) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<8) return false;
	for(i=0; i<8; ++i){
		if (!dev.VPool().Rent(motor[i].counter)) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	if (i<8) return false;
	
	/*InitMat();
	BeforeCalib();
	Calib();
	AfterCalib();
	*/
	return true;
}
bool HISpidar8MotorTest::Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF){
	for(int i=0; i<4; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
        }
	return Init(dev);
}
void HISpidar8MotorTest::Update(float dt){
	HIHapticDevice::Update(dt);
	float len[8];
	for(int i=0; i<8; i++) len[i]=motor[i].GetLength();
//	TRACE("len = %1.3f, %1.3f, %1.3f, %1.3f\n", len[0], len[1], len[2], len[3]);
	/*
	pos = matPos * (
		Vec3f(	Square(len[0])-Square(len[1]),
		Square(len[1])-Square(len[2]),
		Square(len[2])-Square(len[3])	) + posSqrConst);
		*/
}
bool HISpidar8MotorTest::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	// calibration sets the center of the reference frame at the current
	// position of the spidar (motor[i].pos.norm())
	Update(0.001f);
	for(int i=0; i<8; i++) motor[i].SetLength(motor[i].pos.norm());
	return true;
}
void HISpidar8MotorTest::SetMinForce(){
	for(int i=0; i<8; i++) motor[i].SetForce(motor[i].minForce);
}
void HISpidar8MotorTest::SetForce(int ch, float force){
	if(0 <= ch && ch < 8){
		motor[ch].SetForce(force);
	}
}

int HISpidar8MotorTest::GetCount(int ch){
	if(0 <= ch && ch < 8){
		return motor[ch].counter->Count();
	}
	return 0;
}

}//namespace Spr {

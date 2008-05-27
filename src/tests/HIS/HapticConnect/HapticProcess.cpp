#include "HapticProcess.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>
#include <vector>
#include <windows.h>
#include <fstream>

void HapticProcess::Init(){
	InitDevice();
}

void HapticProcess::InitDevice(){
	// デバイスマネージャの初期化
	devMan.RPool().Register(new DRUsb20Sh4(0));	    // USB2.0版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(1));	    // USB2.0版コントローラ 8モータ
	devMan.Init();														// デバイスの初期化
	DSTR << devMan;												// 初期化の結果を表示

	// SPIDARの初期化
	spidarG6.Init(devMan, false);
	for(int i = 0; i < 8; ++i){
		spidarG6.motor[i].maxForce = 5.0f;
	}
	spidarG6.Calib();
}

void HapticProcess::Step(){
	UpdateSpidar();
	HapticRendering();
	LocalDynamics();
}

void HapticProcess::UpdateSpidar(){
	spidarG6.Update(dt);
	hpointer.SetFramePosition(spidarG6.GetPos() * 200);
	hpointer.SetOrientation(spidarG6.GetOri());
	hpointer.SetVelocity(spidarG6.GetVel());
	hpointer.SetAngularVelocity(spidarG6.GetAngVel());
}

void HapticProcess::HapticRendering(){
	addforce = Vec3d(0.0, 0.0, 0.0); 
	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		Vec3d cPoint = neighborObjects[i].phSolid.GetPose() * neighborObjects[i].closestPoint;
		Vec3d pPoint = hpointer.GetPose() * neighborObjects[i].pointerPoint;
		float	f = (pPoint-cPoint) * neighborObjects[i].direction;
		Vec3d ortho = f * neighborObjects[i].direction;
		if(f < 0.0){
			CalcForce(ortho);
			neighborObjects[i].phSolid.AddForce(-addforce * 20, cPoint);	// 力覚レンダリングで計算した力を剛体に加える
		}
	}
	if(bDisplayforce) spidarG6.SetForce(addforce);		// 力覚提示
//	cout << "addforce" << addforce << endl;
}

void HapticProcess::CalcForce(Vec3d dis){
	addforce += -K * dis;
};

void HapticProcess::LocalDynamics(){
	GenerateForce();
	Integrate();
	ClearForce();
}

void HapticProcess::GenerateForce(){}
void HapticProcess::Integrate(){
	// ツリーに属さない剛体の更新
	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		double minv  = neighborObjects[i].phSolid.GetMassInv();
		SpatialVector vel;			// 剛体の速度（ワールド座標系）
		vel.v() = neighborObjects[i].phSolid.GetVelocity();
		vel.w() = neighborObjects[i].phSolid.GetAngularVelocity();
		vel += (neighborObjects[i].A * neighborObjects[i].phSolid.nextForce + neighborObjects[i].b) * dt;
		neighborObjects[i].phSolid.SetVelocity(vel.v());
//		neighborObjects[i].phSolid.SetAngularVelocity(vel.w());
		neighborObjects[i].phSolid.SetCenterPosition(neighborObjects[i].phSolid.GetCenterPosition() + vel.v() * dt);
//		neighborObjects[i].phSolid.SetOrientation(neighborObjects[i].phSolid.GetOrientation() + Quaterniond::Rot(vel.w() * dt).unit());
 		neighborObjects[i].phSolid.SetUpdated(true);
		neighborObjects[i].phSolid.Step();
	}
}

void HapticProcess::ClearForce(){}

// ベクトルのノルムを返す
// ベクトルの方向はaからb
double HapticProcess::CalcDistance(Vec3d a2, Vec3d b){
	Vec3d c = b - a2;
	Vec3d d = a2 - b;
//	if(c.unit ==  )
	return c.norm();
}

void HapticProcess::Keyboard(unsigned char key){
	switch(key){
		case 'f':
			if(bDisplayforce){
				bDisplayforce = false;
				spidarG6.SetForce(Vec3d(0, 0, 0));
				DSTR << "Force OFF" << endl;
			}else{
				bDisplayforce = true;
				spidarG6.SetForce(Vec3d(0, 0, 0));
				DSTR << "Force ON" << endl;
			}
			break;
		case 'c':
			spidarG6.SetForce(Vec3d(0, 0, 0));
			spidarG6.Calib();
			DSTR << "Calibration" << endl;
			break;
		default:
			break;
	}
}
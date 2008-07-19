#include "HapticProcess.h"
#include "BoxStack.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>
#include <vector>
#include <windows.h>
#include <fstream>

bool vhaptic = false;

HapticProcess hprocess;	

HapticProcess::HapticProcess(){
	dt = 0.001f;
	K = 10;
	D = 0.0;//0.005;		// ダンパを，0にしたら床のがたがたがなくなった．
	posScale = 500;
	bDisplayforce = false;
	bInter = true;
	hpointer.SetDynamical(false);
	hpointer.SetFrozen(true);
	stepcount = 1;
	countmax = 100;
};

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
	hpointer.SetFramePosition(spidarG6.GetPos() * posScale);
//	hpointer.SetOrientation(spidarG6.GetOri());
	hpointer.SetVelocity(spidarG6.GetVel() * posScale);
//	hpointer.SetAngularVelocity(spidarG6.GetAngVel());
}

void HapticProcess::HapticRendering(){
	static double vibT = 0;
	static bool vibFlag = false;
	Vec3d vibV = spidarG6.GetVel();
	static Vec3d vibVo = vibV;
	double vibforce = 0;

	displayforce = Vec3d(0.0, 0.0, 0.0);								
	bool noContact = true;
	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		Vec3d cPoint = neighborObjects[i].phSolid.GetPose() * neighborObjects[i].closestPoint;			// 剛体の近傍点のワールド座標系
		Vec3d pPoint = hpointer.GetPose() * neighborObjects[i].pointerPoint;									// 力覚ポインタの近傍点のワールド座標系
		Vec3d force_dir = pPoint - cPoint;
		Vec3d interpolation_normal;																								// 提示力計算にしようする法線（前回の法線との間を補間する）
		if(bInter){
			// 剛体の面の法線補間
			// 前回の法線と現在の法線の間を補間しながら更新
			double synccount = bstack.dt / hprocess.dt;		// プロセスの刻み時間の比
			interpolation_normal = (stepcount * neighborObjects[i].face_normal + ((double)synccount - stepcount) * neighborObjects[i].last_face_normal) / (double)synccount;															
			if(stepcount > synccount)		interpolation_normal = neighborObjects[i].face_normal;
		}else{
			// 現在の法線を使う
			interpolation_normal = neighborObjects[i].face_normal;
		}

		float	f = force_dir * interpolation_normal;								// 剛体の面の法線と内積をとる
		if(f < 0.0){																			// 内積が負なら力を計算
			Vec3d ortho = f * interpolation_normal;								// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
			Vec3d dv = neighborObjects[i].phSolid.GetPointVelocity(cPoint) - hpointer.GetPointVelocity(pPoint);
			Vec3d dvortho = dv.norm() * interpolation_normal;
			Vec3d addforce = -K * ortho + D * dvortho;// * ortho.norm();						// 提示力計算 (*ダンパの項にorthoのノルムをかけてみた)

			if(!vibFlag){
				vibT = 0;
				vibVo = vibV;
			}
			vibFlag = true;
#if 0
			if(vhaptic){
				double vibA = -200;
				double vibB = 120;
				double vibW = 300;
				vibforce = vibA * (vibVo * addforce.unit()) * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT);		//振動計算
			}
#else
			if(vhaptic){
				double vibA = neighborObjects[i].phSolid.GetShape(0)->GetVibA();
				double vibB = neighborObjects[i].phSolid.GetShape(0)->GetVibB();
				double vibW = neighborObjects[i].phSolid.GetShape(0)->GetVibW();
				vibforce = vibA * (vibVo * addforce.unit()) * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT);		//振動計算
			}			
#endif
			displayforce += addforce + (vibforce * addforce.unit());			// ユーザへの提示力															 
			neighborObjects[i].phSolid.AddForce(-addforce, cPoint);			// 計算した力を剛体に加える
			neighborObjects[i].test_force_norm = addforce.norm();
			noContact = false;
		}
	}
	if (noContact) vibFlag = false;

	vibT += dt;
	if(bDisplayforce) spidarG6.SetForce(displayforce);								// 力覚提示
}

void HapticProcess::LocalDynamics(){
	for(unsigned i = 0; i < neighborObjects.size(); i++){
		if(!neighborObjects[i].blocal) continue;
		SpatialVector vel;																				// 剛体の速度（ワールド座標系）
		vel.v() = neighborObjects[i].phSolid.GetVelocity();
		vel.w() = neighborObjects[i].phSolid.GetAngularVelocity();
		if(stepcount == 1) vel += (neighborObjects[i].curb - neighborObjects[i].lastb) *  bstack.dt;		// 衝突の影響を反映
		vel += (neighborObjects[i].A * neighborObjects[i].phSolid.nextForce + neighborObjects[i].b) * dt;	// 力覚ポインタからの力による速度変化
		neighborObjects[i].phSolid.SetVelocity(vel.v());																		
		neighborObjects[i].phSolid.SetAngularVelocity(vel.w());
		neighborObjects[i].phSolid.SetCenterPosition(neighborObjects[i].phSolid.GetCenterPosition() + vel.v() * dt);
		neighborObjects[i].phSolid.SetOrientation(( Quaterniond::Rot(vel.w() * dt) * neighborObjects[i].phSolid.GetOrientation()).unit());
 		neighborObjects[i].phSolid.SetUpdated(true);
		neighborObjects[i].phSolid.Step();
	}
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
		case 'o':
			if(vhaptic){
				vhaptic = false;
				DSTR << "Vibration Disconnect" << endl;
			}else{
				vhaptic = true;
				DSTR << "Vibration Connect" << endl;
			}
			break;
		case 'c':
			spidarG6.SetForce(Vec3d(0, 0, 0));
			spidarG6.Calib();
			DSTR << "Calibration" << endl;
			break;
		case 'i':
			if(bInter){
				bInter = false;
				DSTR << "Use Current Face_Normal" << endl;
			}else{
				bInter = true;
				DSTR << "Use Interpolate Face_Normal" << endl;
			}
			break;
		default:
			break;
	}
}
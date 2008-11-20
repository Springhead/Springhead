#include "HapticProcess.h"
#include "PhysicsProcess.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>
#include <vector>
#include <windows.h>
#include <fstream>

HapticProcess hprocess;	
#define TORQUE

HapticProcess::HapticProcess(){
	dt = 0.001f;
	K = 10;
	D = 0.0;//0.005;		// ダンパを，0にしたら床のがたがたがなくなった．
	posScale = 300;
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
	hpointer.SetOrientation(spidarG6.GetOri());
	hpointer.SetVelocity(spidarG6.GetVel() * posScale);
	hpointer.SetAngularVelocity(spidarG6.GetAngVel());
}

void HapticProcess::HapticRendering(){
	displayforce = Vec3d(0.0, 0.0, 0.0);		
	displaytorque = Vec3d(0.0, 0.0, 0.0);

	for(unsigned i = 0; i < expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.blocal) continue;
		Vec3d cPoint = expandedObjects[i].phSolid.GetPose() * expandedObjects[i].syncInfo.neighborPoint.closestPoint;			// 剛体の近傍点のワールド座標系
		Vec3d pPoint = hpointer.GetPose() * expandedObjects[i].syncInfo.neighborPoint.pointerPoint;									// 力覚ポインタの近傍点のワールド座標系
		Vec3d force_dir = pPoint - cPoint;
		Vec3d interpolation_normal;																								// 提示力計算にしようする法線（前回の法線との間を補間する）

		// 剛体の面の法線補間
		// 前回の法線と現在の法線の間を補間しながら更新
		double synccount = pprocess.dt / hprocess.dt;		// プロセスの刻み時間の比
		interpolation_normal = (stepcount * expandedObjects[i].syncInfo.neighborPoint.face_normal + 
			((double)synccount - stepcount) * expandedObjects[i].syncInfo.neighborPoint.last_face_normal) / (double)synccount;															
		if(stepcount > synccount)		interpolation_normal = expandedObjects[i].syncInfo.neighborPoint.face_normal;

		float	f = force_dir * interpolation_normal;								// 剛体の面の法線と内積をとる
		if(f < 0.0){																			// 内積が負なら力を計算
			Vec3d ortho = f * interpolation_normal;								// 近傍点から力覚ポインタへのベクトルの面の法線への正射影
			Vec3d dv = expandedObjects[i].phSolid.GetPointVelocity(cPoint) - hpointer.GetPointVelocity(pPoint);
			Vec3d dvortho = dv.norm() * interpolation_normal;
			static Vec3d addforce = Vec3d(0,0,0);
			
			addforce = -K * ortho + D * dvortho;// * ortho.norm();						// 提示力計算 (*ダンパの項にorthoのノルムをかけてみた)
			//Vec3d addtorque = (pPoint - hpointer.GetCenterPosition()) % addforce ;

			displayforce += addforce;			// ユーザへの提示力		
//			displaytorque += addtorque;										 
			expandedObjects[i].phSolid.AddForce(-addforce, cPoint);			// 計算した力を剛体に加える
			expandedObjects[i].syncInfo.neighborPoint.test_force_norm = addforce.norm();
		}
	}
#ifdef TORQUE
	if(bDisplayforce) spidarG6.SetForce(displayforce);//, displaytorque);		// 力覚提示
#else
	if(bDisplayforce) spidarG6.SetForce(displayforce);								// 力覚提示
#endif
}

void HapticProcess::LocalDynamics(){
	for(unsigned i = 0; i < expandedObjects.size(); i++){
		if(!expandedObjects[i].flag.blocal) continue;
		SpatialVector vel;																				// 剛体の速度（ワールド座標系）
		vel.v() = expandedObjects[i].phSolid.GetVelocity();
		vel.w() = expandedObjects[i].phSolid.GetAngularVelocity();
		if(stepcount == 1) vel += (expandedObjects[i].syncInfo.motionCoeff.curb - expandedObjects[i].syncInfo.motionCoeff.lastb) *  pprocess.dt;		// 衝突の影響を反映
		vel += (expandedObjects[i].syncInfo.motionCoeff.A * expandedObjects[i].phSolid.nextForce + expandedObjects[i].syncInfo.motionCoeff.b) * dt;	// 力覚ポインタからの力による速度変化
		expandedObjects[i].phSolid.SetVelocity(vel.v());																		
		expandedObjects[i].phSolid.SetAngularVelocity(vel.w());
		expandedObjects[i].phSolid.SetCenterPosition(expandedObjects[i].phSolid.GetCenterPosition() + vel.v() * dt);
		expandedObjects[i].phSolid.SetOrientation(( Quaterniond::Rot(vel.w() * dt) * expandedObjects[i].phSolid.GetOrientation()).unit());
 		expandedObjects[i].phSolid.SetUpdated(true);
		expandedObjects[i].phSolid.Step();
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
		case 'c':
			spidarG6.SetForce(Vec3d(0, 0, 0));
			spidarG6.Calib();
			DSTR << "Calibration" << endl;
			break;
		default:
			break;
	}
}
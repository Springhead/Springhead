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
	K = 20;
	D = 0.1;
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
//	FindNearestPoint();
	HapticRendering();
	LocalDynamics();
}

void HapticProcess::UpdateSpidar(){
	const float posScale = 200;
	spidarG6.Update(dt);
	hpointer.SetFramePosition(spidarG6.GetPos() * posScale);
//	hpointer.SetOrientation(spidarG6.GetOri());
	hpointer.SetVelocity(spidarG6.GetVel() * posScale);
//	hpointer.SetAngularVelocity(spidarG6.GetAngVel());
}

//void HapticProcess::FindNearestPoint(){
//	for(unsigned i = 0; i < neighborObjects.size(); i++){
//		if(!neighborObjects[i].blocal) continue;																									// Localに剛体が入ってなかったらやめる
//
//		CDConvex* a = DCAST(CDConvex, neighborObjects[i].phSolidIf->GetShape(0));											// 剛体が持つ凸形状
//		CDConvex* b = DCAST(CDConvex, hpointer.GetShape(0));																		// 力覚ポインタの凸形状
//		Posed a2w, b2w;																																	// 剛体のワールド座標
///*		if(neighborObjects[i].blocal) 	a2w = neighborObjects[i].phSolid.GetPose();												// blocalがtrueなら最新の情報でやる
//		else */								a2w = neighborObjects[i].phSolidIf->GetPose();
//		b2w = hpointer.GetPose();																													// 力覚ポインタのワールド座標
//		Vec3d pa ,pb;																																		// pa:剛体の近傍点，pb:力覚ポインタの近傍点（ローカル座標）
//		pa = pb = Vec3d(0.0, 0.0, 0.0);
//		FindClosestPoints(a, b, a2w, b2w, pa, pb);																								// GJKで近傍点の算出
//		Vec3d wa = a2w * pa;																															// 剛体近傍点のワールド座標
//		Vec3d wb = b2w * pb;																															// 力覚ポインタ近傍点のワールド座標
//		Vec3d a2b = wb - wa;																															// 剛体から力覚ポインタへのベクトル
//		Vec3d normal = a2b.unit();
//		// 力覚ポインタと剛体がすでに接触していたらCCDGJKで法線を求める
//		if(a2b.norm() < 0.01){																																
//			pa = pb = Vec3d(0.0, 0.0, 0.0);
//			Vec3d dir = -neighborObjects[i].face_normal;
//			if(dir == Vec3f(0.0, 0.0, 0.0) ) dir = -(hpointer.GetCenterPosition() - wa);
//			double dist = 0.0;
//			int cp = ContFindCommonPoint(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
//			if(cp != 1){
//				ContFindCommonPointSaveParam(a, b, a2w, b2w, dir, -DBL_MAX, 1, normal, pa, pb, dist);
//				DSTR << "contfindcommonpoint don not find contact point" << endl;
//			}
//		}
//		neighborObjects[i].closestPoint = pa;			// 剛体近傍点のローカル座標
//		neighborObjects[i].pointerPoint = pb;			// 力覚ポインタ近傍点のローカル座標
//		neighborObjects[i].last_face_normal = neighborObjects[i].face_normal;
//		neighborObjects[i].face_normal = normal;		// 剛体から力覚ポインタへの法線
//	}
//}

void HapticProcess::HapticRendering(){
	double vibA = -200;
	double vibB = 120;
	double vibW = 300;
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
			interpolation_normal = (stepcount * neighborObjects[i].face_normal + (50 - stepcount) * neighborObjects[i].last_face_normal) / 50;															
			if(stepcount > 50)	interpolation_normal = neighborObjects[i].face_normal;				
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
			if(vhaptic){
				vibforce = vibA * (vibVo * addforce.unit()) * exp(-vibB * vibT) * sin(2 * M_PI * vibW * vibT);		//振動計算
			}
			displayforce += addforce + (vibforce * addforce.unit());																			 
			neighborObjects[i].phSolid.AddForce(-addforce, cPoint);			// 計算した力を剛体に加える
			neighborObjects[i].test_force_norm = addforce.norm();
			noContact = false;
		}
	}
	if (noContact) vibFlag = false;

	vibT += dt;
	if(bDisplayforce) spidarG6.SetForce(displayforce);													// 力覚提示
//	DSTR  << displayforce.norm() << "::" << displayforce << endl;
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
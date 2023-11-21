/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HIKorokoro.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

#ifdef _WIN32
#else
# define stricmp strcasecmp
#endif

#include <fstream>


namespace Spr {
	HIKorokoro::HIKorokoro(const HIKorokoroDesc& desc) {};//desc is null pointer here
	HIKorokoro::~HIKorokoro() {};

	bool HIKorokoro::Init(const void* pDesc) {
		HISdkIf* sdk = GetSdk();
		const HIKorokoroDesc& desc = *(const HIKorokoroDesc*)pDesc;
		size_t nMotor = desc.motors.size();
		motors.resize(nMotor);

		size_t i;
		for (i = 0; i < motors.size(); ++i) {
			motors[i].SetDesc(&desc.motors[i]);
			//DAだけ使う
			motors[i].da = DCAST(DVDaIf, sdk->RentVirtualDevice(DVDaIf::GetIfInfoStatic(), "", motors[i].ch));
			motors[i].counter = DCAST(DVCounterIf, sdk->RentVirtualDevice(DVCounterIf::GetIfInfoStatic(), "", motors[i].ch));
			if (!motors[i].da || !motors[i].counter)
				break;
			AddDeviceDependency(motors[i].da->GetRealDevice()->Cast());
			AddDeviceDependency(motors[i].counter->GetRealDevice()->Cast());
		}
		if (i < motors.size())
			return false;
		isGood = true;
		return true;
	}



	///「全てのモータに対して最小出力を設定」
	void HIKorokoro::SetLimitMinForce(float f) {
		for (int i = 0; i < (int)motors.size(); i++)
			motors[i].SetLimitMinForce(f);
		
	}
	void HIKorokoro::SetLimitMaxForce(float f) {
		for (int i = 0; i < (int)motors.size(); i++)
			motors[i].SetLimitMaxForce(f);
	}

	
	void HIKorokoro::SetPose(Posed pose) {
		setPosition = pose.Pos();
		setOrientation = pose.Ori();
		std::fstream log("log_SetPose.txt", std::ios::app);
		log << "SetPose" << pose << " called." << "\n";
	}


	void HIKorokoro::SetForce(const Vec3f& Force, const Vec3f&)	
	{
		//test
		counterR = motors[0].GetCount();
		std::fstream log("log_GetCount.txt", std::ios::app);
		log << "GetCount" << motors[0].GetCount() << ") called." << std::endl;

		counterR = motors[0].GetCount();

		// 引っ張られるのと逆方向に力を出す
		Vec3d springForce = Force;
		// 重さ分係数をかける
		springForce *= kForMass;
		// 垂直方向はそのまま
		korokoroForce.y = (float)springForce.y;

	   // 右方向の基本ベクトル
		Vec3d totteRight = setOrientation * right;
		totteRight.y = 0;   // y方向は余計なので消す
		float normalized = sqrt(totteRight.x* totteRight.x+totteRight.y* totteRight.y+ totteRight.z* totteRight.z);
		Vec3d normalizedR = Vec3d(totteRight.x / normalized, totteRight.y / normalized, totteRight.z / normalized);
		// 前方向の基本ベクトル
		Vec3d totteForward = setOrientation * forward;
		totteForward.y = 0;   // y方向は余計なので消す
		normalized = sqrt(totteForward.x * totteForward.x + totteForward.y * totteForward.y + totteForward.z * totteForward.z);
		Vec3d normalizedF = Vec3d(totteForward.x / normalized, totteForward.y / normalized, totteForward.z / normalized);

		// 左右方向の力
		korokoroForce.x = normalizedR.x * springForce.x + normalizedR.y * springForce.y + normalizedR.z * springForce.z;
		// 前後方向の力
		korokoroForce.z = normalizedF.x * springForce.x + normalizedF.y * springForce.y + normalizedF.z * springForce.z;


		// thetaとphiのsinとcosを求める（詳細はScrapBoxに載せる）
		//float sinPhi = (setOrientation * forward).y;
		float sinPhi = 0;
		float cosPhi = sqrt(1 - (sinPhi * sinPhi));
		float sinTheta = (setPosition.y - (l2 * sinPhi)) / l1;
		float cosTheta = sqrt(1 - (sinTheta * sinTheta));

		// f_1, f_2, T に合うようにそれぞれの力を求める
		float fItem1 = korokoroForce.z / 2.0f;
		float fItem2 = korokoroForce.x * (l1 * cosTheta + l2 * cosPhi) / l3;
		 f1 = (fItem1 + fItem2)* maxCommand /1.7f;
		 f2 = (fItem1 - fItem2) * maxCommand / 1.7f;
		 T = (korokoroForce.y * cosTheta + korokoroForce.z * sinTheta) * l1;	
		
	}

	void HIKorokoro::Update(float dt) {
		HIHaptic::Update(dt);
		SendForce = Vec3d(f1, f2, T);

		//モーター側に送る値のLog
		std::fstream log("log_SendForce.txt", std::ios::app);
		log << "SendForce(" << SendForce << ") called." << std::endl;

		//モーターに送る
		motors[0].SetForce(SendForce.x);
		motors[1].SetForce(SendForce.y);
		motors[2].SetForce(SendForce.z);

	}


	void HIKorokoroDesc::Init() {
		type = "Korokoro";
		int nMotor = 3;
		float vpn = 300;
		float minF = -512.0;
		float maxF = 512.0;

		motors.resize(nMotor);
		for (int i = 0; i < nMotor; ++i) {
			motors[i].voltPerNewton = vpn;		//	volt per newton
			motors[i].minForce = minF;
			motors[i].maxForce = maxF;
			motors[i].ch = i;
		}
	}
	
	
	//後で戻す
	//void HIKorokoro::SetForce(const Vec3f& Force, const Vec3f&) {
	//	// thetaとphiのsinとcosを求める（詳細はScrapBoxに載せる）
	//	//float sinPhi = (handRotation * forward).y;
	//	float sinPhi = 0;
	//	float cosPhi = sqrt(1 - (sinPhi * sinPhi));
	//	float sinTheta = (setPosition.y - (l2 * sinPhi)) / l1;
	//	float cosTheta = sqrt(1 - (sinTheta * sinTheta));

	//	// f_1, f_2, T に合うようにそれぞれの力を求める
	//	float fItem1 = Force.z / 2.0f;
	//	float fItem2 = Force.x * (l1 * cosTheta + l2 * cosPhi) / l3;
	//	float f1 = fItem1 + fItem2;
	//	float f2 = fItem1 - fItem2;
	//	float T = (Force.y * cosTheta + Force.z * sinTheta) * l1;
	//	
	//
	//	
	//	
	//}

	//void HIKorokoro::SetForce(Vec3d Force, Vec3d Pos) {
	//	// thetaとphiのsinとcosを求める（詳細はScrapBoxに載せる）
	//	//float sinPhi = (handRotation * forward).y;
	//	float sinPhi = 0;
	//	float cosPhi = sqrt(1 - (sinPhi * sinPhi));
	//	float sinTheta = (handPos.y - (l2 * sinPhi)) / l1;
	//	float cosTheta = sqrt(1 - (sinTheta * sinTheta));

	//	// f_1, f_2, T に合うようにそれぞれの力を求める
	//	float fItem1 = Force.z / 2.0f;
	//	float fItem2 = Force.x * (l1 * cosTheta + l2 * cosPhi) / l3;
	//	f1 = fItem1 + fItem2;
	//	f2 = fItem1 - fItem2;
	//	float T = (Force.y * cosTheta + Force.z * sinTheta) * l1;


	//	//SendForce = Vec3d(f1, f2, T);

	//	std::fstream log("SendForce.txt", std::ios::app);
	//	log << "SetForce(" << SendForce << ") called." << std::endl;


	//}
	
	

}	//	namespace Spr

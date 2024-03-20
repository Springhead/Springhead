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
			//DAとカウンタを使う
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
	}

	void HIKorokoro::SetForce(const Vec3f& Force, const Vec3f&)
	{
		springForce = Force;
		// 係数をかける
		springForce *= kForMass;		
	}

	void HIKorokoro::Update(float dt) {

		//エンコーダーの値取得
		counterR = motors[0].GetCount();
		counterL = motors[1].GetCount();
		counterT = motors[2].GetCount();
		//エンコーダの値の増減　スケール合わせた
		difR = (counterR - beforeR) * 6.13e-5f;
		difL = (counterL - beforeL) * 6.13e-5f;
		difT = (counterT - beforeT) * 2.95e-5f;

		//エンコーダによる現在位置の差分
		counterPos.z = (difR + difL) * 0.5;
		counterPos.x = l1*cosTheta*(difR - difL) /l3;
		counterPos.y = difT;
		//エンコーダによる現在値と新しいsetPositionに近づけた値をmix
		mixPos = setPosition+ counterPos;
		//新しい値が入ってきたのを判別
		if (temp != setPosition) {
			//新しい位置入力が来たらその位置を使う
			mixPos = setPosition;
		}
		temp = setPosition;
		
		//速度計算のためにgetpose渡してる　モータードライバーとの通信もこの中
		HIHaptic::Update(dt);

		//この値使ってsetForceする（一周遅れるけど）
		// 垂直方向はそのまま
		korokoroForce.y = (float)springForce.y;
		// 右方向の基本ベクトル
		Vec3d totteRight = setOrientation * right;
		totteRight.y = 0;   // y方向は余計なので消す
		float normalized = sqrt(totteRight.x * totteRight.x + totteRight.y * totteRight.y + totteRight.z * totteRight.z);
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
		sinPhi = 0;
		cosPhi = sqrt(1 - (sinPhi * sinPhi));
		sinTheta = (mixPos.y - (l2 * sinPhi)) / l1;
		cosTheta = sqrt(1 - (sinTheta * sinTheta));
		tanTheta = sinTheta / cosTheta;

		// f_1, f_2, T に合うようにそれぞれの力を求める
		float fItem1 = korokoroForce.z / 2.0f;
		float fItem2 = korokoroForce.x * (l1 * cosTheta + l2 * cosPhi) / l3;
		f1 = (fItem1 + fItem2);
		f2 = (fItem1 - fItem2);
		T = korokoroForce.y * lh + (l2+lh) * korokoroForce.z * tanTheta ;
		if (T<0.01 && T>-0.01) { T = 0; }

		//エンコーダの一個前の値を保存
		beforeR = counterR;
		beforeL = counterL;
		beforeT = counterT;

		
		//DRUart側に送る値
		SendForce = Vec3d(f1, f2, T);
		//測定値のログ
		//std::fstream log("log.txt", std::ios::app);
		//log << mixPos.x << "\t" << mixPos.y << "\t" << mixPos.z  << "\t"  << f1 << "\t" << f2 << "\t" << T <<  std::endl;
		
		//モーターに送る
		motors[0].SetForce(SendForce.x);
		motors[1].SetForce(SendForce.y);
		motors[2].SetForce(SendForce.z);

	}


	void HIKorokoroDesc::Init() {
		type = "Korokoro";
		int nMotor = 3;
		float vpn = 0.25f;
		float minF = -2.0;
		float maxF = 2.0;

		motors.resize(nMotor);
		for (int i = 0; i < nMotor; ++i) {
			motors[i].voltPerNewton = vpn;		//	volt per newton
			motors[i].minForce = minF;
			motors[i].maxForce = maxF;
			motors[i].ch = i;
		}
	}
	
	

	
	

}	//	namespace Spr

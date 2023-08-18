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
			if (!motors[i].da)
				break;
			AddDeviceDependency(motors[i].da->GetRealDevice()->Cast());
		}
		if (i < motors.size())
			return false;
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

	void HIKorokoro::Update(float dt) {
		HIHaptic::Update(dt);	
		CalculateHandForce(handForce,handPos);
		motors[0].SetForce(SumForce.x);
		motors[1].SetForce(SumForce.y);
		motors[2].SetForce(SumForce.z);	
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


	

	//void HIKorokoro::SetForce(const Vec3f& Force, const Vec3f&) {
	Vec3d HIKorokoro::CalculateHandForce(Vec3d Force,Vec3d hPos){
		// thetaとphiのsinとcosを求める（詳細はScrapBoxに載せる）
		float sinPhi = (handRotation * forward).y;
		//float sinPhi = 0;
		float cosPhi = sqrt(1 - (sinPhi * sinPhi));
		float sinTheta = (hPos.y - (l2 * sinPhi)) / l1;
		float cosTheta = sqrt(1 - (sinTheta * sinTheta));

		// f_1, f_2, T に合うようにそれぞれの力を求める
		float fItem1 = Force.z / 2.0f;
		float fItem2 = Force.x * (l1 * cosTheta + l2 * cosPhi) / l3;
		float f1 = fItem1 + fItem2;
		float f2 = fItem1 - fItem2;
		float T = (Force.y * cosTheta + Force.z * sinTheta) * l1;

		
		SumForce = Vec3d(f1, f2, T);
		
		return SumForce;
		
	}
	

}	//	namespace Spr

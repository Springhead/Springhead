#include "FWOpHapticHandler.h"
#include "Physics\PHOpHapticController.h"

namespace Spr {
	;

	void FWOpHapticHandler::SetHapticTimeInterval(float dt)
	{
		hapticTimeInterval = dt;
	}
	float FWOpHapticHandler::GetHapticTimeInterval()
	{
		return hapticTimeInterval;
	}
	void FWOpHapticHandler::SetHapticflag(bool flag)
	{
		hcReady = flag;
	}

	bool FWOpHapticHandler::IsHapticEnabled()
	{
		return hcReady;
	}

	void FWOpHapticHandler::UpdateHumanInterface(PHOpHapticController* opHC, float dt)
	{
		HIHapticIf* hiHaptic = DCAST(HIHapticIf, humanInterface);
		HIPoseIf*   hiPose = DCAST(HIPoseIf, humanInterface);

		// 力の出力
		if (hiHaptic) {
			hapticForce = opHC->GetHCOutput();
			if (opHC->GetHCForceReady()){
				hiHaptic->SetForce(hapticForce.v(), hapticForce.w());
				opHC->SetHCForceReady(false);
			}
			else{
				hiHaptic->SetForce(Vec3f(), Vec3f());
			}
		}

		// インタフェースの状態更新
		if (hiPose) {
			hiPose->Update(dt);
			SpatialVector vel;
			vel.v() = hiPose->GetVelocity();
			vel.w() = hiPose->GetAngularVelocity();
			Posed pose = hiPose->GetPose();

			//// デバックのための擬似入力
			//Vec3d debug_vel = Vec3d(-0.001, 0.0, 0.0);
			//static Posed debug_pose = Posed();
			//debug_pose.Pos() = debug_pose.Pos() + debug_vel * dt;
			//pose = debug_pose;
			//vel.v() = debug_vel;

			opHC->SetHCPosition(pose.Pos());
			opHC->SetHCPose(pose);
		}
	}

	/*void FWOpHapticHandler::SetCurrFeedbackForce()
	{
		currSpg->SetForce(opHcIf->GetCurrentOutputForce(),Vec3f());
	}
	

	void FWOpHapticHandler::UpdateHapticStats()
	{
		currSpg->Update(0.001f);
		opHcIf->SetHCPosition(currSpg->GetPosition());
		opHcIf->SetHCPose(currSpg->GetPose());
	}
*/
	//bool FWOpHapticHandler::initDevice(HISdkIf* hiSdk)
	//{
	//	// 力覚インタフェースとの接続設定
	//	//hiSdk = HISdkIf::CreateSdk();

	//	// win32
	//	/*DRUsb20SimpleDesc usbSimpleDesc;
	//	hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	//	DRUsb20Sh4Desc usb20Sh4Desc;
	//	for (int i = 0; i<10; ++i){
	//	usb20Sh4Desc.channel = i;
	//	hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	//	}*/
	//	// win64
	//	DRCyUsb20Sh4Desc cyDesc;
	//	for (int i = 0; i < 10; ++i){
	//		cyDesc.channel = i;
	//		hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
	//	}
	//	hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	//	hiSdk->Print(DSTR);
	//	//hiSdk->Print(std::DSTR);

	//	UTRef<HISpidarGIf> spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	//	hcReady = spg->Init(&HISpidarGDesc("SpidarG6X3R"));

	//	if (hcReady)
	//	{
	//		spg->Calibration();
	//		spg->Update(0.001f);
	//	}
	//	else return false;

	//	currSpg = spg;
	//	opHcIf->SetPhHCReady(hcReady);

	//	return true;
	//}
	bool FWOpHapticHandler::doCalibration(float dt)
	{
		HIHapticIf* hiHaptic = DCAST(HIHapticIf, humanInterface);
		
		if (hiHaptic->Calibration())
			{
			hiHaptic->Update(dt);
				return true;
			}
		
		return false;
	}
}//namespace
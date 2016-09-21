#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIDRUsb.h>
#include <iomanip>

#ifdef DAILYBUILD
  #include "dailybuild_SEH_Handler.h"
#endif

using namespace Spr;

int __cdecl main(){
	SEH_HANDLER_DEF
	SEH_HANDLER_TRY

	// HumanInterface SDKを作成
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();

	// Spidar制御用ハードウェアの初期化
	DRUsb20SimpleDesc usbSimpleDesc;
	sdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.channel = i;
		sdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	sdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());

	sdk->Print(DSTR);

	// Spidarインタフェース作成
	UTRef<HISpidar4If> spg = sdk->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	spg->Init(&HISpidar4Desc("SpidarG6X3R"));

	int t = 0;
	while(!_kbhit()){
		t += 1;
		spg->Update(0.001f);
#if 1
		Vec3f spgpos = spg->GetPosition();
		if(t%1000==1){
		std::cout << std::setprecision(2) << spgpos << std::endl;
		}
		Vec3f f(0.0, 0.0, 0.0);

		if(spgpos.y < -0.015){
			f[1] = 1.5;
		}
		spg->SetForce(f);
#else if
		for(size_t i=0; i<spg->NMotor(); ++i){
			std::cout << " " << std::setprecision(2) << spg->GetMotor(i)->GetLength();
		}
		std::cout << std::endl;
#endif
	}
	_getch();
/*
	DRKeyMouseWin32If* wif = DCAST(DRKeyMouseWin32If, sdk->FindRealDevice("KeyMouseWin32"));
	wif->Update();	
	UTRef<HIMouse6DIf> mouse6D = DCAST(HIMouse6DIf, sdk->CreateHumanInterface("HIMouse6D", NULL));
	while(1){
		if (mouse6D->GetKeyMouse()->GetKeyState('Q') & DVKeyMouseIf::PRESS) return 0;
		for(int i=0; i<200; ++i){
			if (mouse6D->GetKeyMouse()->GetKeyState(i) & DVKeyMouseIf::PRESS){
				std::cout << i << " '"<< (char)i << "' " << std::endl;
			}
		}
	}
*/

	SEH_HANDLER_CATCH
}

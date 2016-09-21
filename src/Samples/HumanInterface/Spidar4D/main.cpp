#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIDRUsb.h>
#include <iomanip>
using namespace Spr;

#ifdef DAILYBUILD
  #include "dailybuild_SEH_Handler.h"
#endif

int __cdecl main(){
	SEH_HANDLER_DEF
	SEH_HANDLER_TRY

	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	DRUsb20SimpleDesc usbSimpleDesc;
	sdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
	DRUsb20Sh4Desc usb20Sh4Desc;
	for(int i=0; i<10; ++i){
		usb20Sh4Desc.channel = i;
		sdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
	}
	sdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	sdk->Print(DSTR);

	UTRef<HISpidar4If> spg = sdk->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	UTRef<HISpidar4If> spg2 = sdk->CreateHumanInterface(HISpidar4If::GetIfInfoStatic())->Cast();
	//spg->Init(&HISpidar4Desc("SpidarG6X3R",Vec4i(1,3,6,8)));
	//spg2->Init(&HISpidar4Desc("SpidarG6X3L",Vec4i(2,4,5,7)));
	spg->Init(&HISpidar4Desc("SpidarR",Vec4i(1,2,3,4)));
	spg2->Init(&HISpidar4Desc("SpidarL",Vec4i(5,6,7,8)));
	
	int t = 0;
	while(!_kbhit()){
		t += 1;
		spg->Update(0.001f);
		spg2->Update(0.001f);
#if 1
		Vec3f spgpos = spg->GetPosition();
		Vec3f spgpos2 = spg2->GetPosition();
		if(t%1000==1){
		std::cout << std::setprecision(2) << spgpos << std::endl;
		}
		Vec3f f(0.0, 0.0, 0.0);
		Vec3f f2(0.0, 0.0, 0.0);

		if(spgpos.y < -0.015){
			f[1] = 1.5;
		}
		if(spgpos2.y < -0.015){
			f2[1] = 1.5;
		}
		spg->SetForce(f);
		spg2->SetForce(f2);
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

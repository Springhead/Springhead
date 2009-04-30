#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIKeyMouse.h>
#include <iomanip>
using namespace Spr;


int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	sdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());

	sdk->Init();
	sdk->Print(DSTR);

	DRKeyMouseWin32If* wif = sdk->FindRealDevice("KeyMouseWin32")->Cast();
	wif->Update();
	UTRef<HIMouse6DIf> mouse6D = sdk->CreateHumanInterface("HIMouse6D")->Cast();
	mouse6D->Init(NULL);
	for(int i=0; i<1000; ++i){
		if (mouse6D->GetKeyMouse()->GetKeyState('Q') & DVKeyMouseIf::PRESS) return 0;
		for(int i=0; i<200; ++i){
			if (mouse6D->GetKeyMouse()->GetKeyState(i) & DVKeyMouseIf::PRESS){
				std::cout << i << " '"<< (char)i << "' " << std::endl;
			}
		}
		std::cout << "pose:" << mouse6D->GetPose() << std::endl;
	}
}

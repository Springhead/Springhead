#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIKeyMouse.h>
#include <iomanip>
using namespace Spr;


int __cdecl main(){
	HISdkIf::RegisterSdk();
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	sdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
	sdk->Print(DSTR);

	UTRef<HITrackballIf> trackball = sdk->CreateHumanInterface("HITrackball")->Cast();
	trackball->Init(NULL);
	for(int i=0; i<1000; ++i){
		if (trackball->GetKeyMouse()->GetKeyState('Q') & DVKeySt::PRESSED) return 0;
		for(int i=0; i<200; ++i){
			if (trackball->GetKeyMouse()->GetKeyState(i) & DVKeySt::PRESSED){
				std::cout << i << " '"<< (char)i << "' " << std::endl;
			}
		}
		std::cout << "pose:" << trackball->GetPose() << std::endl;
	}
}

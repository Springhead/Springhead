#include <conio.h>
#include <Springhead.h>
using namespace Spr;


int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	HIXbox360ControllerIf* con = sdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	con->Init();
	while(!_kbhit()){
		con->Update(0.001f);
		std::cout << con->GetPose() << std::endl;
	}
	_getch();
}

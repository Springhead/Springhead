#include <conio.h>
#include <Springhead.h>
using namespace Spr;


int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	DSTR << HIXbox360ControllerIf::GetIfInfoStatic() << std::endl;
	HIXbox360ControllerIf* con = sdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	con->Init();
	while(!_kbhit()){
		con->Update(0.001f);
		//con->SetVibration(Vec2f(100.0, 10.0));
		std::cout << con->GetPose() << std::endl;
	}
	_getch();
}

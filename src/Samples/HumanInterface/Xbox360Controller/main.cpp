#include <conio.h>
#include <Springhead.h>
using namespace Spr;


int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	HIXbox360ControllerIf* con = sdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
	con->Init();
	while(!_kbhit()){
		con->Update(0.001f);
		static int count = 0;
		count++;
		double x = abs(sin(count/100.0)) * 200.0;
		double y = abs(sin(count/100.0)) * 200.0;
		con->SetVibration(Vec2f(x, y));
 		std::cout << con->GetPose() << std::endl;
	}
	con->SetVibration(Vec2f(0.0, 0.0));
	_getch();
}

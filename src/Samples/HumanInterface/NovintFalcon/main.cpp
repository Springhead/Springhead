#include <conio.h>
#include <Springhead.h>
#include <iomanip>
using namespace Spr;

int __cdecl main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	sdk->Print(DSTR);
	UTRef<HINovintFalconIf> intf = sdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
	intf->Init(NULL);
	int t = 0;
	for(int i=0; !kbhit() && i<100000; ++i){
		t += 1;
		intf->Update(0.001f);
#if 1
		Vec3f intfpos = intf->GetPosition();
		std::cout << std::setprecision(2) << intfpos << std::endl;
		Vec3f f(0.0, 0.0, 0.0);
		if(intfpos.y < -0.015){
			f[1] = -(intfpos.y - (-0.015)) * 1000;
		}
		intf->SetForce(f, Vec3f());
#endif

	}
	if (kbhit()) getch();
}

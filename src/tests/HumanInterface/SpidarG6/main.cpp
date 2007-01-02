#include <Springhead.h>
#include <HumanInterface/SprHIDRUsb.h>
using namespace Spr;

int main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	DRUsb20SimpleDesc rdesc;
	sdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &rdesc);
	HIMouse6DIf* mk = DCAST(HIMouse6DIf, sdk->CreateHumanInterface("HIMouse6D", NULL));
	return 0;
}

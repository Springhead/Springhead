#include <Springhead.h>
using namespace Spr;

int main(){
	UTRef<HISdkIf> sdk = HISdkIf::CreateSdk();
	HIPoseIf* pose = DCAST(HIPoseIf, sdk->CreateHumanInterface("HIPose"));
	return 0;
}

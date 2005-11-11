#include <Springhead.h>
#pragma hdrstop

using namespace Spr;

int main(int argc, char* argv[]){
	UTRef<PHSdkIf> sdk = CreatePHSdk();
	UTRef<PHSceneIf> scene = sdk->CreateScene();
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	UTRef<PHSolidIf> solid1 = scene->CreateSolid(desc);
	desc.center = Vec3f(1,0,0);
	UTRef<PHSolidIf> solid2 = scene->CreateSolid(desc);
	for(int i=0; i<10; ++i){
		scene->ClearForce();
		solid1->AddForce(Vec3f(10,0,0), Vec3f(0,2,0));
		solid2->AddForce(Vec3f(10,0,0), Vec3f(0,2,0));
		scene->GenerateForce();
		scene->Integrate();
		std::cout << solid1->GetFramePosition();
		std::cout << solid2->GetFramePosition() << std::endl;
//		std::cout << solid1->GetOrientation() << std::endl;
	}
}

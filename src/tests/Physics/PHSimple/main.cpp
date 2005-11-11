#include <Springhead.h>
#include <Physics/PHSolid.h>
#pragma hdrstop

using namespace Spr;

int main(int argc, char* argv[]){
	UTRef<PHSdkIf> sdk = CreatePHSdk();
	UTRef<PHSceneIf> scene = sdk->CreateScene();
	UTRef<PHSolidIf> solid = scene->CreateSolid();
	for(int i=0; i<10; ++i){
		scene->ClearForce();
		solid->AddForce(Vec3f(10,0,0), Vec3f(0,1,0));
		scene->GenerateForce();
		scene->Integrate();
		std::cout << solid->GetFramePosition();
		std::cout << solid->GetOrientation() << std::endl;
	}
}

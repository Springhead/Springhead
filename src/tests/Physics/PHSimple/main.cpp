#include <Springhead.h>		//	Springheadのインタフェース
#pragma hdrstop
using namespace Spr;

int main(int argc, char* argv[]){
	PHSdkIf* sdk = CreatePHSdk();					//	SDKの作成
	PHSceneIf* scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	PHSolidIf* solid1 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	desc.center = Vec3f(1,0,0);
	PHSolidIf* solid2 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
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
	delete sdk;	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
}

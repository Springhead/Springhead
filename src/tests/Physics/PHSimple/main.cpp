#include <Springhead.h>		//	Springheadのインタフェース
#include <gl/glut.h>
#pragma hdrstop
using namespace Spr;


PHSdkIf* sdk;
PHSceneIf* scene;
PHSolidIf* solid1, *solid2;

/*
void Display(){
}
void Idle(){
}
*/
int main(int argc, char* argv[]){
	sdk = CreatePHSdk();				//	SDKの作成
	scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	solid1 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	desc.center = Vec3f(1,0,0);
	solid2 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成

	/*
	glutInit(&argc, argv);
	glutCreateWindow("PHSimple");
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutMainLoop();
	*/
	for(int i=0; i<10; ++i){
		solid1->AddForce(Vec3f(10,0,0), Vec3f(0,2,0));
		solid2->AddForce(Vec3f(10,0,0), Vec3f(0,2,0));
		scene->Step();
		std::cout << solid1->GetFramePosition();
		std::cout << solid2->GetFramePosition() << std::endl;
//		std::cout << solid1->GetOrientation() << std::endl;
	}
//	delete sdk;	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
}

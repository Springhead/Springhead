#include <Springhead.h>		//	Springheadのインタフェース
#pragma hdrstop
using namespace Spr;


PHSdkIf* sdk;
PHSceneIf* scene;
PHSolidIf* solid1, *solid2;

int main(int argc, char* argv[]){
	sdk = CreatePHSdk();				//	SDKの作成
	scene = sdk->CreateScene();			//	シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	solid2 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	solid1 = scene->CreateSolid(desc);	//	剛体をdescに基づいて作成
	solid1->SetGravity(false);
	
	//	形状の作成
	CDConvexMeshDesc md;
	md.vertices.push_back(Vec3f(-1,-1,-1));
	md.vertices.push_back(Vec3f(-1,-1, 1));
	md.vertices.push_back(Vec3f(-1, 1,-1));
	md.vertices.push_back(Vec3f(-1, 1, 1));
	md.vertices.push_back(Vec3f( 1,-1,-1));
	md.vertices.push_back(Vec3f( 1,-1, 1));
	md.vertices.push_back(Vec3f( 1, 1,-1));
	md.vertices.push_back(Vec3f( 1, 1, 1));
	CDShapeIf* mesh2 = scene->CreateShape(md);

	for(unsigned i=0; i<md.vertices.size(); ++i){
		md.vertices[i].x *= 10;
	}
	CDShapeIf* mesh1 = scene->CreateShape(md);

	solid1->AddShape(mesh1);
	solid2->AddShape(mesh2);
	solid1->SetFramePosition(Vec3f(0,-1,0));
	solid2->SetFramePosition(Vec3f(0,2,0));

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	for(int i=0; i<200; ++i){
//		solid1->AddForce(Vec3f(10,0,0), Vec3f(0,2,0));
//		solid2->AddForce(Vec3f(10,0,0), Vec3f(0,2,0));
		scene->Step();
		std::cout << solid1->GetFramePosition();
		std::cout << solid2->GetFramePosition() << std::endl;
//		std::cout << solid1->GetOrientation() << std::endl;
	}
	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete sdk;

	//	メモリリークのテスト．
	//	デバッグありで実行するとメモリリークがVCのデバッグ出力に表示される．
	char* memoryLeak = new char[123];
}

#include <Framework/SprFWApp.h>

#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <FileIO/FIFileX.h>
#include <FileIO/FISdk.h>
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif


namespace Spr{;

FWApp::FWApp(){
	debugMode = FWApp::DM_NONE;
	isRunning = true;
}
FWApp::~FWApp(){
	fwScene->Clear();
	fwSdk->Clear();
}
void FWApp::LoadScene(UTString filename){
	//	デフォルトの先祖オブジェクトをを設定
	//	これらのCreateObjectが呼ばれてシーングラフが作られる。
	ObjectIfs objs;
	objs.Push(fwSdk->GetGRSdk());	//	GRSdk
	objs.Push(fwSdk->GetPHSdk());	//	PHSdk
	//	FWSdk	FWSceen は FWSdkの子になるので、FWSdkを最後にPushする必要がある。
	objs.Push(fwSdk);
	int first = fwSdk->NScene();	//	ロードされるFWSceneの位置を覚えておく

	//	ファイルローダーの作成
	UTRef<FISdkIf> fiSdk = fwSdk->GetFISdk();
	UTRef<FIFileXIf> fiFileX = fiSdk->CreateFileX();
	//	ファイルのロード
	if (! fiFileX->Load(objs, filename.data()) ) {
		DSTR << "Error: Cannot open load file. " << std::endl;
		exit(EXIT_FAILURE);
	}
	//	ロードしたシーンを取得
	DSTR << "Loaded " << fwSdk->NScene() - first << " scenes." << std::endl;
	DSTR << "LoadFile Complete." << std::endl;
	for(int i=first; i<fwSdk->NScene(); ++i){
		fwSdk->GetScene(i)->Print(DSTR);
		fwScene = fwSdk->GetScene(i);
	}
}
void FWApp::ProcessArguments(int argc, char* argv[]){
}

void FWApp::Step(){
	if (fwScene){
		fwScene->GetPHScene()->Step();
	}
}
void FWApp::Init(int argc, char* argv[]){
	ProcessArguments(argc, argv);
	fwSdk = FWSdkIf::CreateSdk();
}

void FWApp::Display(){
	// シーンがロードされていなくてもクリアは行う
	grRender->ClearBuffer();
	if (!fwScene) return;
	fwScene->Sync();
	grRender->BeginScene();
	if (debugMode == FWApp::DM_DEBUG){
		GRCameraIf* cam = NULL;
		if (fwScene->GetGRScene()) cam = fwScene->GetGRScene()->GetCamera();
		if (cam) cam->Render(grRender);
		
		GRLightDesc ld;
		ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
		ld.specular = Vec4f(1,1,1,1) * 0.8f;
		ld.ambient = Vec4f(1,1,1,1) * 0.4f;
		ld.position = Vec4f(1,1,1,0);
		grRender->PushLight(ld);
		grRender->DrawScene(fwScene->GetPHScene());
/*
//		grRender->SetVertexFormat(GRVertexElement::vfP3f);
		Vec3f v[6];
		v[3] = Vec3f(0,5,0);
		v[5] = Vec3f(5,0,0);
		grRender->SetMaterial(GRMaterialDesc(Vec4f(1,0,0,1))); 
		grRender->DrawDirect(GRRenderBaseIf::TRIANGLES, v, 3);
*/

		grRender->PopLight();
		if (cam) cam->Rendered(grRender);
	}else{
		fwScene->GetGRScene()->Render(grRender);
	}
	grRender->EndScene();	
}

void FWApp::Reshape(int w, int h){
	grRender->Reshape(Vec2f(), Vec2f(w,h));
}

void FWApp::Keyboard(unsigned char key, int x, int y){
	if (key == 27) {
		exit(0);
	}
	if (key == '.') {
		isRunning = false;
		Step();
	}
	if (key == ' ') {
		isRunning = true;
	}
}

void FWApp::MouseButton(int button, int state, int x, int y){
}
void FWApp::MouseMove(int x, int y){
}

}

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <SprDefs.h>
#include <Framework/SprFWApp.h>
#include <Framework/FWSdk.h>
#include <Framework/FWOldSpringheadNode.h>
#include <Framework/FWObject.h>
#include <Framework/FWScene.h>
#include <Framework/FWFemMesh.h>
#include <Framework/FWFemMeshNew.h>
#include <Framework/FWOpObj.h>
#include <Framework/FWTrajectoryPlanner.h>
#include <Framework/FWStaticTorqueOptimizer.h>
#include <Physics/PHSdk.h>
#include <Physics/PHScene.h>
#include <Graphics/GRSdk.h>
#include <Graphics/GRScene.h>
#include <SprFileIO.h>
#include <FileIO/FISdk.h>
#include <HumanInterface/HISdk.h>
#include <Foundation/UTPath.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
#undef CreateFile

using namespace std;

namespace Spr{;

void SPR_CDECL FWRegisterTypeDescs();
void SPR_CDECL FWSdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	FWRegisterTypeDescs();
	FWRegisterOldSpringheadNode();

	FWSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWScene));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWObject));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWFemMesh));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWFemMeshNew));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHSdk));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(PHScene));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRSdk));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(GRScene));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(HISdk));
	FWSceneIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWOpObj));
	FWSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWTrajectoryPlanner));
	FWSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWOptimizer));
	FWSdkIf::GetIfInfoStatic()->RegisterFactory(DBG_NEW FactoryImp(FWStaticTorqueOptimizer));
}

FWSdkIf* SPR_CDECL FWSdkIf::CreateSdk(){
	FWSdk* rv = DBG_NEW FWSdk;
	FWSdkIf::RegisterSdk();
	PHSdkIf::RegisterSdk();
	GRSdkIf::RegisterSdk();
	HISdkIf::RegisterSdk();
	
	return rv->Cast();
}

//----------------------------------------------------------------------------
//	FWSdk
FWSdk::FWSdk(){
	name="fwSdk";
	CreateSdks();
	//curScene = NULL;
	//curRender = NULL;
	//debugMode = false;
	//DSTRFlag = true;
}
void FWSdk::CreateSdks(){
	phSdk = PHSdkIf::CreateSdk();
	DCAST(PHSdk, phSdk)->SetNameManager(Cast());
	phSdk->SetName("phSdk");
	
	grSdk = GRSdkIf::CreateSdk();
	DCAST(GRSdk, grSdk)->SetNameManager(Cast());
	grSdk->SetName("grSdk");
	
	fiSdk = FISdkIf::CreateSdk();
	DCAST(FISdk, fiSdk)->SetNameManager(Cast());
	fiSdk->SetName("fiSdk");

	hiSdk = HISdkIf::CreateSdk();
	DCAST(HISdk, hiSdk)->SetNameManager(Cast());
	hiSdk->SetName("hiSdk");
}

FWSdk::~FWSdk(){
}
FWSceneIf* FWSdk::CreateScene(const PHSceneDesc& phdesc, const GRSceneDesc& grdesc){
	FWSceneDesc desc;
	FWSceneIf* scene = DCAST(FWSceneIf, CreateObject(FWSceneIf::GetIfInfoStatic(), &desc));
	scene->SetPHScene(GetPHSdk()->CreateScene(phdesc));
	scene->SetGRScene(GetGRSdk()->CreateScene(grdesc));
	AddChildObject(scene);
	return scene;
}

FIFileIf* FWSdk::CreateFile(UTString ext, const IfInfo* ii){
	FIFileIf* file;
	// IfInfo指定の場合
	if(ii)
		file = GetFISdk()->CreateFile(ii);
	// 拡張子判定
	else file = GetFISdk()->CreateFileFromExt(ext);

	// 判定不能はXとみなす
	if(!file){
		DSTR << "unknown file type. regarded as X file." << std::endl;
		file = GetFISdk()->CreateFileX();
	}
	return file;
}

bool FWSdk::LoadScene(UTString filename, ImportIf* ex, const IfInfo* ii, ObjectIfs* objs){

	//filename末端に改行コード( = 0x0a)が含まれているとロードされないので，あれば最初に削除する
	if(filename.at(filename.length()-1) == 0x0a){
		filename.erase(filename.length()-1);
	}
	//	デフォルトの先祖オブジェクトをを設定
	//	これらのCreateObjectが呼ばれてシーングラフが作られる。
	ObjectIfs defObjs;
	if(!objs){
		defObjs.Push(GetGRSdk());	//	GRSdk
		defObjs.Push(GetPHSdk());	//	PHSdk
		//	FWSdk	FWScene は FWSdkの子になるので、FWSdkを最後にPushする必要がある。
		defObjs.Push(Cast());
		objs = &defObjs;
	}

	int first = NScene();	//	ロードされるFWSceneの位置を覚えておく

	UTPath path;
	path.Path(filename);	
	
	FIFileIf* file = CreateFile(path.Ext(), ii);
	if (ex) file->SetImport(ex);
	//file->SetDSTR(DSTRFlag);
	//	ファイルのロード成否
	if(!file->Load(*objs, filename.data()) ) {
		DSTR << "Error: Cannot load file " << filename.c_str() << std::endl;
		//exit(EXIT_FAILURE);
		return false;
	}
	//	ロードしたシーンを取得
	//if(DSTRFlag) DSTR << "Loaded " << NScene() - first << " FWScene." << std::endl;
	//if(DSTRFlag) DSTR << "LoadFile Complete." << std::endl;
	//for(int i=first; i<NScene(); ++i){
	//	curScene = GetScene(i);
	//	if(DSTRFlag)curScene->Print(DSTR);
	//}
	return true;
}

bool FWSdk::SaveScene(UTString filename, ImportIf* ex, const IfInfo* ii, ObjectIfs* objs){
	// 保存
	ObjectIfs defObjs;
	if(!objs){
		for(unsigned int i=0; i<scenes.size(); ++i)
			defObjs.push_back(scenes[i]->Cast());
		objs = &defObjs;
	}

	UTPath path;
	path.Path(filename);

	FIFileIf* file = CreateFile(path.Ext(), ii);
	file->SetImport(ex);
	if(!file->Save(*objs, filename.c_str())){
		DSTR << "Error: Cannot save file " << filename.c_str() << std::endl;
		return false;
	}
	return true;
}

FWSceneIf* FWSdk::GetScene(int i){
	if(i == -1 && FWApp::GetApp()->GetCurrentWin() != NULL)
		return FWApp::GetApp()->GetCurrentWin()->GetScene();
    if(0 <= i && i < NScene())
		return scenes[i];
	return NULL;
}

void FWSdk::MergeScene(FWSceneIf* scene0, FWSceneIf* scene1){
	if(scene0 == scene1)
		return;
	DSTR << "merging " << scene0->GetName() << " and " << scene1->GetName() << endl;
	
	FWScenes::iterator it0, it1;
	it0 = find(scenes.begin(), scenes.end(), scene0);
	it1 = find(scenes.begin(), scenes.end(), scene1);
	if(it0 == scenes.end() || it1 == scenes.end())
		return;
	// PHSceneのマージ
	if(scene0->GetPHScene()){
		if(scene1->GetPHScene())
			GetPHSdk()->MergeScene(scene0->GetPHScene(), scene1->GetPHScene());
	}
	else if(scene1->GetPHScene())
		scene0->SetPHScene(scene1->GetPHScene());
	// GRSceneのマージ
	if(scene0->GetGRScene()){
		if(scene1->GetGRScene())
			GetGRSdk()->MergeScene(scene0->GetGRScene(), scene1->GetGRScene());
	}
	else if(scene1->GetGRScene())
		scene0->SetGRScene(scene1->GetGRScene());

	// FWObjectのマージ
	for(int i = 0; i < scene1->NObject(); i++){
		scene0->AddChildObject(scene1->GetObjects()[i]);
	}
	//if(curScene == scene1)
	//	curScene = scene0;

	scenes.erase(it1);
}


/*GRRenderIf*	FWSdk::CreateRender(){
	GRRenderIf* render = GetGRSdk()->CreateRender();
	GRDeviceIf* dev = GetGRSdk()->CreateDeviceGL();
	dev->Init();
	render->SetDevice(dev);

	//	仮の視点。このあとうまくシーンが設定されれば、シーンのカメラに上書きされる。
	Affinef view;
	view.Pos() = Vec3f(0.0, 3.0, 3.0);
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
	view = view.inv();		
	render->SetViewMatrix(view);

	renders.push_back(render);
	//curRender = render;
	return render;
}*/

bool FWSdk::AddChildObject(ObjectIf* o){
	FWScene* s = DCAST(FWScene, o);
	if (s){
		if (std::find(scenes.begin(), scenes.end(), s->Cast()) == scenes.end()){
			scenes.push_back(s->Cast());
			s->sdk = this;
			//curScene = s->Cast();
			return true;
		}
	}
	PHSdkIf* ps = DCAST(PHSdkIf, o);
	if (ps) {
		phSdk = ps;
		return true;
	}
	GRSdkIf* gs = DCAST(GRSdkIf, o);
	if (gs) {
		grSdk = gs;
		return true;
	}
	FISdkIf* fs = DCAST(FISdkIf, o);
	if (fs) {
		fiSdk = fs;
		return true;
	}
	HISdkIf* hs = DCAST(HISdkIf, o);
	if (hs) {
		hiSdk = hs;
		return true;
	}
	return false;
}

bool FWSdk::DelChildObject(ObjectIf* o){
	FWSceneIf* s = DCAST(FWSceneIf, o);
	if(s){
		FWScenes::iterator it = std::find(scenes.begin(), scenes.end(), s);
		if(it != scenes.end()){
			scenes.erase(it);
			//if(curScene == s)
			//	curScene = (scenes.empty() ? NULL : scenes[0]);
			return true;
		}
	}
	return false;
}

size_t FWSdk::NChildObject() const {
	return NScene();
}

ObjectIf* FWSdk::GetChildObject(size_t i){
	return GetScene((int)i);
}

void FWSdk::Clear(){
	// 一度全てをクリアしてSDKを作り直す
	Sdk::Clear();
	phSdk = NULL;
	grSdk = NULL;
	fiSdk = NULL;
	hiSdk = NULL;
	scenes.clear();
	//curScene = NULL;
	// レンダラはシーンオブジェクトではないのでここでは削除しない．
	// ClearRender APIを追加するかは要検討
	//renders.clear();
	//curRender = NULL;
	CreateSdks();
}

bool FWSdk::GetDebugMode(){
	return FWApp::GetApp()->GetCurrentWin()->GetDebugMode();
}
void FWSdk::SetDebugMode(bool debug){
	FWApp::GetApp()->GetCurrentWin()->SetDebugMode(debug);
}
void FWSdk::Step(){
	FWApp::GetApp()->GetCurrentWin()->GetScene()->Step();
}

void FWSdk::Draw(){
	FWWinIf* win = FWApp::GetApp()->GetCurrentWin();
	win->GetScene()->Draw(win->GetRender(), win->GetDebugMode());
}

GRRenderIf* FWSdk::GetRender(){
	return FWApp::GetApp()->GetCurrentWin()->GetRender();
}


}

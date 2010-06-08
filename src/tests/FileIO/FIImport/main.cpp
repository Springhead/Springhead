/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/**
【概要】
Importのインポート機能のテスト．
・シーンを手動構築
・インポート関係を埋め込みセーブ
・セーブしたデータをロード

【終了基準】
一連の手続きの完了

【Note】

*/

#include <Springhead.h>

using namespace Spr;
using namespace std;

/// SDK
UTRef<GRSdkIf> grSdk;
UTRef<PHSdkIf> phSdk;
UTRef<FISdkIf> fiSdk;

/// インポート情報
ImportIf*		import;

/// 手動構築
void Build(){
	DSTR << "building scene" << endl;

	PHSceneIf* phScene = phSdk->CreateScene();
	PHSolidIf* solid0 = phScene->CreateSolid();
	PHSolidIf* solid1 = phScene->CreateSolid();

	solid1->AddShape(phSdk->CreateShape(CDSphereDesc()));

	/// インポート情報の埋め込み
	ObjectIfs objs;
	objs.Push(solid0);
	ImportIf* import1 = fiSdk->CreateImport(import, "solids/solid0.x", phScene, objs);
	objs.clear();
	objs.Push(solid1);
	ImportIf* import2 = fiSdk->CreateImport(import, "solids/solid1/solid1.x", phScene, objs);
	/*objs.clear();
	objs.Push(phSdk->GetShape(0));
	ImportIf* import2 = fiSdk->CreateImport(import, "shapes/sphere.x", phSdk, objs);
	*/
}

void Save(const char* filename){
	DSTR << "saving scene" << endl;

	FIFileIf* file = fiSdk->CreateFileFromExt(filename);
	ObjectIfs objs;
	for(int i = 0; i < (int)phSdk->NChildObject(); i++)
		objs.push_back(phSdk->GetChildObject(i));

	file->SetImport(import);
	file->Save(objs, filename);
}

void Load(const char* filename){
	DSTR << "loading scene" << endl;

	FIFileIf* file = fiSdk->CreateFileFromExt(filename);
	ObjectIfs objs;
	objs.push_back(phSdk);

	file->SetImport(import);
	file->Load(objs, filename);
}

int main(int argc, char* argv[]){
	grSdk = GRSdkIf::CreateSdk();
	phSdk = PHSdkIf::CreateSdk();
	fiSdk = FISdkIf::CreateSdk();
	import = fiSdk->CreateImport();

	/// シーンを手動構築
	Build();
	phSdk->Print(DSTR);

	/// セーブ
	Save("test.x");

	/// クリア
	phSdk->Clear();
	phSdk->Print(DSTR);

	/// ロード
	Load("test.x");
	phSdk->Print(DSTR);

	/// もう一度セーブ
	Save("afterload/test.x");

	return 0;
}

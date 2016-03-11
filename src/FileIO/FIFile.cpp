/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <FileIO/FIFile.h>
#include <FileIO/FIImport.h>
#include <FileIO/FILoadContext.h>
#include <FileIO/FISaveContext.h>
#include <FileIO/FISdk.h>

using namespace std;

namespace Spr{;

//#define PDEBUG_EVAL(x)	x
#define PDEBUG_EVAL(x)

//---------------------------------------------------------------------------
//	FIFile
FIFile::FIFile(){
	import = NULL;
	DSTRFlag = true;
}
void FIFile::SetImport(ImportIf* im){
	import = im->Cast();
}
ImportIf* FIFile::GetImport(){
	return import->Cast();
}
bool FIFile::Load(ObjectIfs& objs, const char* fn){
	DSTR << "Loading " << fn << " ...." << std::endl;

	FILoadContext fc;
	fc.sdk = sdk;
	fc.objects.insert(fc.objects.end(), objs.begin(), objs.end());

	// インポートが設定されていない場合は作成する
	if(!import)
		import = sdk->CreateImport()->Cast();
	import->Clear();
	fc.importStack.Push(import->Cast());

	fc.PushFileMap(fn, true);

	bool ok = false;
	if(fc.IsGood()){
		Load(&fc);
		// ロードした最上位オブジェクトをスタックに積む（ユーザがアクセスできるように）
		objs.insert(objs.end(), fc.rootObjects.begin(), fc.rootObjects.end());
		ok = true;
	}

	fc.PopFileMap();

	return ok;
}
void CallLoadedRecursive(ObjectIf* o, FILoadContext* fc, std::set<ObjectIf*>& uniqueCheck){
	if (uniqueCheck.insert(o).second){
		for(unsigned i=0; i<o->NChildObject(); ++i){
			CallLoadedRecursive(o->GetChildObject(i), fc, uniqueCheck);
		}
		((Object*)o)->Loaded(fc);
	}
}
void FIFile::Load(FILoadContext* fc){
	LoadImp(fc);
	
	fc->SetDSTR(DSTRFlag);

	fc->LinkData();
	if(DSTRFlag) fc->loadedDatas.Print(DSTR);

	//	データからノードを作成
	fc->CreateScene();
	//	ノードをリンク
	fc->LinkNode();
	//	リンク後の処理
	fc->PostTask();
	//	ロード終了をロードしたノードに伝える
	std::set<ObjectIf*> uniqueCheck;
	for(unsigned i=0; i<fc->rootObjects.size();++i)
		if (fc->rootObjects[i])
			CallLoadedRecursive(fc->rootObjects[i], fc, uniqueCheck);
}

bool FIFile::Save(const ObjectIfs& objs, const char* fn){
	// 保存先ディレクトリへ移動(なければ作成)
	UTPath path(fn);
	UTPath::CreateDir(path.Dir());
	UTString oldCwd = UTPath::GetCwd();
	UTPath::SetCwd(path.Dir());

	FISaveContext sc;
	if(import)
		sc.importStack.Push(import->Cast());
	sc.sdk = sdk;

	bool ok = false;
	sc.PushFileMap(path.File().c_str(), IsBinary());
	if(sc.IsGood()){
		Save(objs, &sc);
		ok = true;
	}
	sc.PopFileMap();

	// 元ディレクトリへ復帰
	UTPath::SetCwd(oldCwd);

	return ok;
}
void FIFile::Save(const ObjectIfs& objs, FISaveContext* sc){
	OnSaveFileStart(sc);
	for(ObjectIfs::const_iterator it = objs.begin(); it != objs.end(); ++it){
		SaveNode(sc, *it);
	}
	OnSaveFileEnd(sc);
}
void FIFile::SaveNode(FISaveContext* sc, ObjectIf* obj){
	// 同一オブジェクトの2度目以降の保存は参照扱いになる
	// ＊所有者がロード時とセーブ時で変わる可能性がある	tazz
	if (!sc->savedObjects.insert(obj).second){
		sc->objects.Push(obj);
		OnSaveRef(sc);
		sc->objects.Pop();
		return;
	}
	
	//	セーブ中のノードを記録
	sc->objects.Push(obj);

	UTString tn = sc->GetNodeTypeName();
	tn.append("Desc");
	UTTypeDesc* type = sc->typeDbs.Top()->Find(tn);
	if(type){
		//	セーブ位置を設定
		sc->fieldIts.Push(UTTypeDescFieldIt(type));
		//	オブジェクトからデータを取り出す．
		void* data = (void*)obj->GetDescAddress();
		if (data){
			sc->datas.Push(DBG_NEW UTLoadedData(NULL, type, data));
		}else{
			sc->datas.Push(DBG_NEW UTLoadedData(NULL, type));
			data = sc->datas.back()->data;
			obj->GetDesc(data);
		}
		OnSaveNodeStart(sc);
		
		//	データのセーブ
		OnSaveDataStart(sc);
		SaveBlock(sc);
		OnSaveDataEnd(sc);
		sc->datas.Pop();
		sc->fieldIts.Pop();
	}
	else{
		UTString err("Node '");
		err.append(tn);
		err.append("' not found. can not save data.");
		sc->ErrorMessage(err.c_str());
	}

	ObjectIfs exportedObjs;
	
	// エクスポート対象の子オブジェクトを別ファイルに保存
	if(!sc->importStack.empty()){

		// objがオーナーオブジェクトであるエクスポートエントリを列挙
		vector<Import*>	imports;
		Import* im = sc->importStack.Top();
		for(int i = 0; i < (int)im->Children().size(); i++){
			Import* imChild = im->Children()[i];
			if(imChild->ownerObj == obj)
				imports.push_back(imChild);
		}

		// エントリごとに別ファイルに保存
		for(vector<Import*>::iterator it = imports.begin(); it != imports.end(); it++){
			Import* imChild = *it;

			if(!imChild->loadOnly){
				// 保存先ディレクトリを作成して移動
				UTPath path(imChild->path);
				bool changeDir = (path.Dir().compare("") != 0);
				UTString oldCwd;
				if(changeDir){
					UTPath::CreateDir(path.Dir());
					oldCwd = UTPath::GetCwd();
					UTPath::SetCwd(path.Dir());
				}

				// エクスポート対象のオブジェクトを別ファイルへ保存
				FIFile* file = sc->sdk->CreateFileFromExt(path.Ext())->Cast();
				sc->PushFileMap(path.File(), IsBinary());
				if(sc->IsGood()){
					OnSaveFileStart(sc);
					for(int i = 0; i < (int)imChild->childObjs.size(); i++){
						ObjectIf* childObj = imChild->childObjs[i];
						file->SaveNode(sc, childObj);
						exportedObjs.push_back(childObj);
					}
					OnSaveFileEnd(sc);
				}
				sc->PopFileMap();
				
				if(changeDir)
					UTPath::SetCwd(oldCwd);
			}

			// 保存中のファイルにはImportノードを書き込む
			SaveNode(sc, imChild->Cast());
		}
	}

	// エクスポート対象外の子オブジェクトを保存
	size_t nChild = obj->NChildObject();
	if (nChild){
		OnSaveChildStart(sc);
		for(size_t i=0; i<nChild; ++i){
			ObjectIf* child = obj->GetChildObject(i);
			assert(child);
			// エクスポート済みならスキップ
			if(find(exportedObjs.begin(), exportedObjs.end(), child) != exportedObjs.end())
				continue;
			SaveNode(sc, child);
		}
		OnSaveChildEnd(sc);
	}

	if(type)
		OnSaveNodeEnd(sc);
	
	//	記録をPOP
	sc->objects.Pop();
}
void FIFile::SaveBlock(FISaveContext* sc){
	OnSaveBlockStart(sc);
	void* base = sc->datas.Top()->data;
	while (sc->fieldIts.back().NextField(base)){
		//UTTypeDesc::Composit::iterator field = sc->fieldIts.back().field;	//	現在のフィールド型
		UTTypeDesc::Field* field = &*(sc->fieldIts.back().field);
		//	要素数の取得
		int nElements = 1;
		if (field->varType == UTTypeDesc::Field::VECTOR){
			nElements = (int)field->VectorSize(base);
		}else if (field->varType == UTTypeDesc::Field::ARRAY){
			nElements = field->length;
		}
		if (OnSaveFieldStart(sc, nElements)){
			PDEBUG_EVAL( DSTR << "Save field '" << field->name << "' : " << field->typeName << " = "; )
			for(int pos=0; pos<nElements; ++pos){
				OnSaveElementStart(sc, pos, (pos==nElements-1));
				switch(sc->fieldIts.back().fieldType){
					case UTTypeDescFieldIt::F_BLOCK:{
						PDEBUG_EVAL( DSTR << "=" << std::endl; )
						void* blockData = field->GetAddress(base, pos);
						sc->datas.Push(DBG_NEW UTLoadedData(NULL, field->type, blockData));
						sc->fieldIts.Push(UTTypeDescFieldIt(field->type));
						SaveBlock(sc);
						sc->fieldIts.Pop();
						sc->datas.Pop();
						}break;
					case UTTypeDescFieldIt::F_BOOL:{
						bool val = field->ReadBool(base, pos);
						PDEBUG_EVAL( DSTR << val ? "true" : "false"; )
						OnSaveBool(sc, val);
						}break;
					case UTTypeDescFieldIt::F_INT:{
						int val = (int)field->ReadNumber(base, pos);
						PDEBUG_EVAL( DSTR << val; )
						OnSaveInt(sc, val);
						}break;
					case UTTypeDescFieldIt::F_REAL:{
						double val = field->ReadNumber(base, pos);
						PDEBUG_EVAL( DSTR << val; )
						OnSaveReal(sc, val);
						}break;
					case UTTypeDescFieldIt::F_STR:{
						UTString val = field->ReadString(base, pos);
						PDEBUG_EVAL( DSTR << val; )
						OnSaveString(sc, val);
						}break;
					default:;
				}
				PDEBUG_EVAL( if (pos<nElements-1) DSTR << ", "; )
				OnSaveElementEnd(sc, pos, (pos==nElements-1));
			}
			PDEBUG_EVAL( DSTR << ";" << std::endl; )
			OnSaveFieldEnd(sc, nElements);
		}
	}
	OnSaveBlockEnd(sc);
}


}	//	namespace Spr

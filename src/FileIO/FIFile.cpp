#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "FIFile.h"
namespace Spr{;

IF_IMP(FIFile, Object);
OBJECTIMPABST(FIFile, Object);

bool FIFile::Load(ObjectIfs& objs, const char* fn){
	FILoadContext fc;
	fc.objects.insert(fc.objects.end(), objs.begin(), objs.end());
	fc.fileInfo.Push();
	fc.fileInfo.back().Map(fn);
	Load(&fc);
	if (fc.rootObjects.size()){
		objs.insert(objs.end(), fc.rootObjects.begin(), fc.rootObjects.end());
		return true;
	}
	return false;
}
void FIFile::Load(FILoadContext* fc){
	if (fc->IsGood()){
		fc->typeDb = &typeDb;
		fc->handlers = &handlers;
		LoadImp(fc);
	}
	fc->Link();
	fc->PostTask();
}

bool FIFile::Save(const ObjectIfs& objs, const char* fn){
	FISaveContext sc;
	sc.Open(fn);
	Save(objs, &sc);
	return false;
}
void FIFile::Save(const ObjectIfs& objs, FISaveContext* sc){
	OnSaveFileStart(sc);
	for(ObjectIfs::const_iterator it = objs.begin(); it != objs.end(); ++it){
		SaveNode(sc, *it);
	}
	OnSaveFileEnd(sc);
}
void FIFile::SaveNode(FISaveContext* sc, ObjectIf* obj){
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
	FITypeDesc* type = typeDb.Find(tn);
	if(type){
		//	セーブ位置を設定
		sc->fieldIts.Push(FIFieldIt(type));
		//	オブジェクトからデータを取り出す．
		void* data = (void*)obj->GetDescAddress();
		if (data){
			sc->datas.Push(DBG_NEW FINodeData(type, data));
		}else{
			sc->datas.Push(DBG_NEW FINodeData(type));
			data = sc->datas.back()->data;
			obj->GetDesc(data);
		}
		OnSaveNodeStart(sc);
		OnSaveDataStart(sc);
		//	データのセーブ
		SaveBlock(sc);
		sc->datas.Pop();
		sc->fieldIts.Pop();
		OnSaveDataEnd(sc);
		//	子ノードのセーブ
		size_t nChild = obj->NChildObject();
		if (nChild){
			OnSaveChildStart(sc);
			for(size_t i=0; i<nChild; ++i){
				ObjectIf* child = obj->GetChildObject(i);
				assert(child);
				SaveNode(sc, child);
			}
			OnSaveChildEnd(sc);
		}
		OnSaveNodeEnd(sc);
	}else{
		UTString err("Node '");
		err.append(tn);
		err.append("' not found. can not save data.");
		sc->ErrorMessage(err.c_str());
		//	子ノードのセーブ
		size_t nChild = obj->NChildObject();
		if (nChild){
			OnSaveChildStart(sc);
			for(size_t i=0; i<nChild; ++i){
				ObjectIf* child = obj->GetChildObject(i);
				assert(child);
				SaveNode(sc, child);
			}
			OnSaveChildEnd(sc);
		}
	}
	//	記録をPOP
	sc->objects.Pop();
}
void FIFile::SaveBlock(FISaveContext* sc){
	OnSaveBlockStart(sc);
	void* base = sc->datas.Top()->data;
	while(sc->fieldIts.back().NextField()){
		FITypeDesc::Composit::iterator field = sc->fieldIts.back().field;	//	現在のフィールド型
		//	要素数の取得
		int nElements = 1;
		if (field->varType == FITypeDesc::Field::VECTOR){
			nElements = field->VectorSize(base);
		}else if (field->varType == FITypeDesc::Field::ARRAY){
			nElements = field->length;
		}
		OnSaveFieldStart(sc, nElements);
		DSTR << "Save field '" << field->name << "' : " << field->typeName << " = ";
		for(int pos=0; pos<nElements; ++pos){
			OnSaveElementStart(sc, pos, (pos==nElements-1));
			switch(sc->fieldIts.back().fieldType){
				case FIFieldIt::F_BLOCK:{
					DSTR << "=" << std::endl;
					void* blockData = field->GetAddress(base, pos);
					sc->datas.Push(new FINodeData(field->type, blockData));
					sc->fieldIts.Push(FIFieldIt(field->type));
					SaveBlock(sc);
					sc->fieldIts.Pop();
					sc->datas.Pop();
					}break;
				case FIFieldIt::F_BOOL:{
					bool val = field->ReadBool(base, pos);
					DSTR << val ? "true" : "false";
					OnSaveBool(sc, val);
					}break;
				case FIFieldIt::F_INT:{
					int val = (int)field->ReadNumber(base, pos);
					DSTR << val;
					OnSaveInt(sc, val);
					}break;
				case FIFieldIt::F_REAL:{
					double val = field->ReadNumber(base, pos);
					DSTR << val;
					OnSaveReal(sc, val);
					}break;
				case FIFieldIt::F_STR:{
					UTString val = field->ReadString(base, pos);
					DSTR << val;
					OnSaveString(sc, val);
					}break;
			}
			if (pos<nElements-1) DSTR << ", ";
			OnSaveElementEnd(sc, pos, (pos==nElements-1));
		}
		DSTR << ";" << std::endl;
		OnSaveFieldEnd(sc, nElements);
	}
	OnSaveBlockEnd(sc);
}


}	//	namespace Spr

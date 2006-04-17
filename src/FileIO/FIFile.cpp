#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "FIFile.h"
namespace Spr{;

IF_IMP(FIFile, Object);
OBJECTIMPABST(FIFile, Object);

bool FIFile::Load(ObjectIfs& objs, const char* fn){
	FIFileContext fc;
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
void FIFile::Load(FIFileContext* fc){
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
	for(ObjectIfs::const_iterator it = objs.begin(); it != objs.end(); ++it){
		SaveNode(sc, *it);
	}
}
void FIFile::SaveNode(FISaveContext* sc, ObjectIf* obj){
	//	セーブ中のノードを記録
	sc->objects.Push(obj);
	OnSaveNodeStart(sc);

	UTString tn = obj->GetIfInfo()->ClassName();
	tn.append("Desc");
	FITypeDesc* type = typeDb.Find(tn);
	if(type){
		//	オブジェクトからデータを取り出す．
		void* data = obj->GetDescAddress();
		if (data){
			sc->datas.Push(DBG_NEW FINodeData(type, data));
		}else{
			sc->datas.Push(DBG_NEW FINodeData(type));
			data = sc->datas.back()->data;
			obj->GetDesc(data);
		}
		//	データのセーブ
		SaveBlock(sc);
	}else{
		UTString err("Node '");
		err.append(tn);
		err.append("' not found. can not save data.");
		sc->ErrorMessage(err.c_str());
	}
	//	子ノードのセーブ
	size_t nChild = obj->NChildObject();
	if (nChild){
		OnSaveChildStart(sc);
		for(size_t i=0; i<nChild; ++i){
			ObjectIf* child = obj->GetChildObject(i);
			SaveNode(sc, child);
		}
		OnSaveChildEnd(sc);
	}
	OnSaveNodeEnd(sc);
	//	記録をPOP
	sc->objects.Pop();
}
void FIFile::SaveBlock(FISaveContext* sc){
	OnSaveBlockStart(sc);
	sc->fieldIts.Push(FIFieldIt(sc->datas.back()->type));
	while(sc->fieldIts.back().NextField()){
		FITypeDesc::Composit::iterator field = sc->fieldIts.back().field;	//	現在のフィールド型
		void* fieldData = ((char*)sc->datas.back()->data) + field->offset;	//	フィールドのデータ
		//	要素数の取得
		int nElements = 1;
		if (field->varType == FITypeDesc::Field::VECTOR){
			nElements = field->type->VectorSize(fieldData);
		}else if (field->varType == FITypeDesc::Field::ARRAY){
			nElements = field->length;
		}
		for(int pos=0; pos<nElements; ++pos){
			void* elementData = fieldData;
			if (field->varType == FITypeDesc::Field::VECTOR){
				elementData = field->type->VectorAt(fieldData, pos);
			}else if (field->varType == FITypeDesc::Field::ARRAY){
				elementData = ((char*)fieldData) + field->type->GetSize()*pos;
			}
			switch(sc->fieldIts.back().fieldType){
				case FIFieldIt::F_BLOCK:
					sc->fieldIts.Push(FIFieldIt(field->type));
					sc->datas.Push(new FINodeData(field->type, elementData));
					SaveBlock(sc);
					break;
				case FIFieldIt::F_BOOL:{
					bool val = field->ReadBool(elementData, pos);
					OnSaveBool(sc, val);
					}break;
				case FIFieldIt::F_INT:{
					int val = field->ReadNumber(elementData, pos);
					OnSaveInt(sc, val);
					}break;
				case FIFieldIt::F_REAL:{
					double val = field->ReadNumber(elementData, pos);
					OnSaveReal(sc, val);
					}break;
				case FIFieldIt::F_STR:{
					UTString val = field->ReadString(elementData, pos);
					OnSaveString(sc, val);
					}break;
			}
		}
	}
	OnSaveBlockEnd(sc);
}


}	//	namespace Spr

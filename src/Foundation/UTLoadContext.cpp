/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "UTLoadContext.h"
#include "UTLoadHandler.h"
#include <Base/Affine.h>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

namespace Spr{;
//---------------------------------------------------------------------------
//	UTFileMap
bool UTFileMap::IsGood(){
	return start && end && (end != (char*)-1);
}


//---------------------------------------------------------------------------
//	UTLoadData
UTLoadData::UTLoadData(UTTypeDesc* t, void* d):type(t), data(d){
	if (!data && type){
		data = type->Create();
		haveData = true;
	}else{
		haveData = false;
	}
}
UTLoadData::~UTLoadData(){
	if (haveData) type->Delete(data);
}

//---------------------------------------------------------------------------
//	UTLoadTasks
OBJECT_IMP(UTLoadTask, NamedObject);

void UTLoadTasks::Execute(UTLoadContext* ctx){
	for(iterator it = begin(); it!=end(); ++it){
		(*it)->Execute(ctx);
	}
	clear();
}

//---------------------------------------------------------------------------
//	UTLinkTask
///	ノードへの参照を記録しておくクラス．全部ロードできてからリンクする．
class UTLinkTask: public UTLoadTask{
public:
	std::vector<NameManagerIf*> nameManagers;
	std::string ref;
	ObjectIf* object;
	UTRef<UTFileMap> info;
	const char* pos;
	UTLinkTask(const ObjectIfs& objs, UTFileMap* info, const char* p, ObjectIf* o, std::string r);
	void Execute(UTLoadContext* ctx);
};

UTLinkTask::UTLinkTask(const ObjectIfs& objs, UTFileMap* fi, const char* p, ObjectIf* o, std::string r):info(fi), pos(p), object(o), ref(r){
	for(int i=objs.size()-1; i>=0; --i){
		NameManagerIf* nm = DCAST(NameManagerIf, objs[i]);
		if (nm){
			nameManagers.push_back(nm);
		}
	}
}

void UTLinkTask::Execute(UTLoadContext* ctx){
	Spr::ObjectIf* refObj = NULL;
	for(unsigned i=0; i<nameManagers.size(); ++i){
		refObj = nameManagers[i]->FindObject(ref);
		if (refObj) break;
	}
	if (refObj){
		if (!object->AddChildObject(refObj)){
			std::string err("Can not link referenced object '");
			err.append(ref);
			err.append("' to '");
			err.append(object->GetIfInfo()->ClassName());
			err.append("'.");
			ctx->ErrorMessage(info, pos, err.c_str());
		}
	}else{
		std::string err("Referenced object '");
		err.append(ref);
		err.append("' not found.");
		ctx->ErrorMessage(info, pos, err.c_str());
	}
}

//---------------------------------------------------------------------------
//	UTLoadContext
void UTLoadContext::WriteBool(bool v){
	UTTypeDescFieldIt& curField = fieldIts.back();
	curField.field->WriteBool(datas.Top()->data, v, curField.arrayPos);
}
void UTLoadContext::WriteNumber(double v){
	UTTypeDescFieldIt& curField = fieldIts.back();
	curField.field->WriteNumber(datas.Top()->data, v, curField.arrayPos);
}
void UTLoadContext::WriteString(std::string v){
	UTTypeDescFieldIt& curField = fieldIts.back();
	curField.field->WriteString(datas.Top()->data, v.c_str(), curField.arrayPos);
}
void UTLoadContext::PushType(UTTypeDesc* type){
	//	ロードすべきtypeとしてセット
	fieldIts.PushType(type);
	//	読み出したデータを構造体の用意
	datas.Push(DBG_NEW UTLoadData(type));
}
void UTLoadContext::PopType(){
	datas.Pop();
	fieldIts.Pop();
}
bool UTLoadContext::IsGood(){
	if (!fileMaps.size()) return false;
	return fileMaps.Top()->IsGood();
}
void UTLoadContext::AddLink(std::string ref, const char* pos){
	links.push_back(DBG_NEW UTLinkTask(objects, fileMaps.Top(), pos, objects.back(), ref));
}
void UTLoadContext::Link(){
	links.Execute(this);
	links.clear();
}
void UTLoadContext::PostTask(){
	postTasks.Execute(this);
	postTasks.clear();
}

void UTLoadContext::ErrorMessage(UTFileMap* info, const char* pos, const char* msg){
	std::string m("error: ");
	m.append(msg);
	Message(info, pos, m.c_str());
}
void UTLoadContext::Message(UTFileMap* info, const char* pos, const char* msg){
	int lines=0;
	int returns=0;
	if (!info) info = fileMaps.Top();
	const char* ptr = info->start;
	const char* line=ptr;

	if (!pos) pos = info->curr;
	if (pos){
		for(;ptr < pos; ++ptr){
			if (*ptr == '\n'){
				++lines;
				line = ptr+1;
			}
			if (*ptr == '\r'){
				++returns;
				line = ptr+1;
			}
		}
		for(;ptr < info->end; ++ptr){
			if (*ptr == '\n' || *ptr == '\r'){
				break;
			}
		}
		lines = lines > returns ? lines : returns;
	}
	std::ostream& os = *errorStream;
	os << info->name << "(" << lines+1 << ") : ";
	os << msg << std::endl;
	os << std::string(line, ptr) << std::endl;
}
void UTLoadContext::PushCreateNode(const IfInfo* info, const void* data){
	ObjectIf* obj = NULL;
	for(UTStack<ObjectIf*>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it){
		if (*it) obj = (*it)->CreateObject(info, data);
		if (obj) break;
	}
	if (!obj) obj = SdkIf::CreateSdk(info, data);

	//	オブジェクトに名前を設定
	if (obj){
		NamedObjectIf* n = DCAST(NamedObjectIf, obj);
		if (datas.Top()->name.length()){
			if (n){
				n->SetName(datas.Top()->name.c_str());
			}else{
				UTString err("Can not give name to an object of '");
				err.append(obj->GetIfInfo()->ClassName());
				const IfInfo* i = obj->GetIfInfo();
				const IfInfo* b = NamedObjectIf::GetIfInfoStatic();
				if (i->Inherit(b)){
					DSTR << "i Inherits b.\n";
				}
				err.append("'.");
				ErrorMessage(NULL, NULL, err.c_str());
			}
		}
		//	親オブジェクトに追加
		if (objects.size() && objects.Top()){
			objects.Top()->AddChildObject(obj);
		}	
	}else{
		UTString err("Can not create '");
		err.append(info->ClassName());
		err.append("'. Ancestor objects don't know how to make it.");
		ErrorMessage(NULL, NULL, err.c_str());
	}
	//	オブジェクトスタックに積む
	objects.Push(obj);
	if (obj && objects.size() == 1) rootObjects.push_back(objects.Top());
}

};

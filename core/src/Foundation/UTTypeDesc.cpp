/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTTypeDesc.h>
#include <Foundation/Scene.h>
#include <sstream>


namespace Spr{;

//----------------------------------------------------------------------------
//	UTTypeDesc::Field
UTTypeDesc::Field::~Field(){
	if (type){
		UTRef<UTTypeDesc> t = type;
		type = NULL;
	}
}


size_t UTTypeDesc::Field::GetSize(){
	if (varType== UTTypeDescIf::VECTOR){
		return type->SizeOfVector();
	}else{
		size_t sz = 0;
		if (isReference){
			sz = sizeof(void*);
		}else{
			sz = type->GetSize();
		}
		if (varType== UTTypeDescIf::SINGLE){
			return sz; 
		}else if (varType== UTTypeDescIf::ARRAY){
			return sz * length;
		}
	}
	assert(0);
	return 0;
}
void UTTypeDesc::Field::AddEnumConst(std::string name, int val){
	enums.push_back(std::make_pair(name, val));
}
///
void UTTypeDesc::Field::AddEnumConst(std::string name){
	int val = 0;
	if (enums.size()) val = enums.back().second+1;
	enums.push_back(std::make_pair(name, val));
}
const void* UTTypeDesc::Field::GetAddress(const void* base, int pos){
	const void* ptr = (const char*)base + offset;
	if (varType == UTTypeDescIf::VECTOR){
		ptr = type->VectorAt(ptr, pos);
	}else if (varType == UTTypeDescIf::ARRAY){
		ptr = (const char*)ptr + type->GetSize()*pos;
	}
	return ptr;
}
void* UTTypeDesc::Field::GetAddressEx(void* base, int pos){
	void* ptr = (char*)base + offset;
	if (varType == UTTypeDescIf::VECTOR){
		while((int)type->VectorSize(ptr)<=pos) type->VectorPush(ptr);
		if (pos == -2){
			type->VectorPush(ptr);
			int back = (int)type->VectorSize(ptr)-1;
			ptr = type->VectorAt(ptr, back);
			//	DSTR << type->GetTypeName() << "ArrayPos of -2, get ptr of " << back << std::endl;
		}else{
			ptr = type->VectorAt(ptr, pos);
		}
	}else if (varType == UTTypeDescIf::ARRAY){
		ptr = (char*)ptr + type->GetSize()*pos;
	}
	return ptr;
}

void UTTypeDesc::Field::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	os << UTPadding(w) << name.c_str() << "(+" << offset << ") = ";
	if (varType==UTTypeDescIf::VECTOR || varType==UTTypeDescIf::ARRAY){
		os << "Vector<";
		if (isReference){
			os << "UTRef<" << type->GetTypeName().c_str() << ">";
		}else{
			if (type) os << type->GetTypeName().c_str();
			else os << "(null)";
		}
		os << ">";
		if (length>1) os << " [" << length << "]";
		os << " ";
		if (type) os << (unsigned int)type->GetSize() * length;
		else os << "?";
	}else if (isReference){
		os << "UTRef<" << (type ? type->GetTypeName().c_str() : "(null)") << ">";
		if (length>1) os << " [" << length << "]";
		os << " " << sizeof(UTRef<UTTypeDesc::Field>) * length;
	}else{
		os.width(w);
		if (type) type->Print(os);
		else os << "(null)";
		if (length>1) os << " [" << length << "]";
		os.width(0);
	}
	os.width(w);
}

//----------------------------------------------------------------------------
//	UTTypeDesc::Composit
int UTTypeDesc::Composit::Size(UTString id){
	int rv = 0;
	for(iterator it = begin(); it != end(); ++it){
		if (id.length()==0 || id.compare(it->name)){
			rv += (int)it->GetSize();
		}
	}
	return rv;
}
void UTTypeDesc::Composit::Print(std::ostream& os) const{
	for(const_iterator it = begin(); it != end(); ++it){
		it->Print(os);
	}
	if (size()){
		if (!back().type || !back().type->GetComposit().size()) DSTR << std::endl;
	}
}
void UTTypeDesc::Composit::Link(UTTypeDescDb* db) {
	for(iterator it = begin(); it != end(); ++it){
		if (it->type == NULL){
			it->type = db->Find(it->typeName);
#if 0
			if (it->type){
				DSTR << it->typeName << " " << it->name << " = " << it->type->GetTypeName() << std::endl;
			}
#endif
		}
	}
}

UTTypeDesc::Field* UTTypeDesc::Composit::Find(const char* id){
	for(unsigned i=0; i<size(); ++i){
		if (at(i).name.compare(id) == 0) return &at(i);
	}
	return NULL;
}
UTTypeDesc::BinaryType UTTypeDesc::CheckSimple(){
	if (bSimple == UNKNOWN_BINARY){
		if (IsPrimitive()){
			if (IsString()) bSimple = COMPLEX_BINARY;
			else bSimple = SIMPLE_BINARY;
		}else{
			bSimple = SIMPLE_BINARY;
			for(unsigned i=0; i<composit.size(); ++i){
				if (composit[i].type){
					bSimple = composit[i].type->CheckSimple();
					if (bSimple != SIMPLE_BINARY) break;
				}else{
					bSimple = UNKNOWN_BINARY;
					break;
				}
			}
		}
	}
	return bSimple;
}

UTTypeDesc::Field* UTTypeDesc::AddField(std::string pre, std::string tn, std::string n, std::string suf){
	composit.push_back(Field());
	if (pre.compare("vector") == 0){
		composit.back().varType = UTTypeDescIf::VECTOR;
		composit.back().length = UTTypeDesc::BIGVALUE;
	}
	if (pre.compare("UTRef") == 0) composit.back().isReference = true;
	if (pre.compare("pointer") == 0) composit.back().isReference = true;
	if (suf.size()){
		std::istringstream is(suf);
		is >> composit.back().length;
		if (!is.good()){
			composit.back().lengthFieldName = suf;
		}
		if (composit.back().varType == UTTypeDescIf::SINGLE){
			composit.back().varType = UTTypeDescIf::ARRAY;
		}
	}

	composit.back().typeName = tn;
	composit.back().name = n;

	return &composit.back();
};

UTTypeDesc::Field* UTTypeDesc::AddBase(std::string tn){
	Composit::iterator it;
	if (composit.size()){
		for(it = composit.begin(); it!=composit.end(); ++it){
			if (it->name.size()) break;
		}
		--it;
		it = composit.insert(it, Field());
	}else{
		composit.push_back(Field());
		it = composit.begin();
	}
	it->typeName = tn;
	return &*it;
}
void UTTypeDesc::Link(UTTypeDescDb* db) {
	composit.Link(db);
}
bool UTTypeDesc::LinkCheck(){
	for(size_t i=0; i<composit.size(); ++i){
		if (!composit[i].type){
			DSTR << "Error in UTTypeDesc::CheckLink() " << composit[i].typeName << " " << composit[i].name << " not linked" << std::endl;
			return false;
		}
	}
	return true;
}
void UTTypeDesc::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	os << typeName << " " << (int)size;
	if (composit.size()){
		os << "{" << std::endl;
		os.width(w+2);
		composit.Print(os);
		os.width(0);
		os << UTPadding(w) << "}" << std::endl;
	}
	os.width(w);
}

void UTTypeDesc::Write(std::ostream& os, void* base){
	if (IsSimple()){
		//	バイナリ書込でOKな場合
		os.write((char*)base, GetSize()); 
	}else if (IsPrimitive()){
		//	単純型の書き出し。ファイルにデータを書き込むのはここだけ。
		if (IsString()){
			std::string str = ReadString(base);
			unsigned len = (unsigned) str.length();
			os.write((char*)&len, sizeof(len));
			os.write(str.c_str(), str.length());
		}else{
			assert(0);	//	ここには来ないはず。
			os.write((char*)base, GetSize());
		}
	}else{
		for(unsigned i=0; i< composit.size(); ++i){
			UTTypeDesc::Field& field = composit[i];
			if (field.varType == UTTypeDescIf::SINGLE){
				field.type->Write(os, field.GetAddress(base, 0));
			}else if (field.varType == UTTypeDescIf::ARRAY){
				unsigned arrayLen = field.length;
				UTTypeDesc::Field* lf = composit.Find(field.lengthFieldName.c_str());
				if (lf) arrayLen = lf->ReadNumber(base);
				os.write((char*)&arrayLen, sizeof(arrayLen));	//	配列の要素数をまず書き出す
				for(unsigned i=0; i<arrayLen; ++i){
					field.type->Write(os, field.GetAddress(base, i));
				}
			}else if (composit[i].varType == UTTypeDescIf::VECTOR){
				unsigned vecLen = (unsigned) composit[i].VectorSize(base);
				os.write((char*)&vecLen, sizeof(vecLen));		//	vectorの要素数をまず書き出す
				for(unsigned i=0; i<vecLen; ++i){
					field.type->Write(os, field.GetAddress(base, i));
				}
			}
		} 
	}
}
void UTTypeDesc::Read(std::istream& is, void* base){
	if (IsSimple()){
		is.read((char*)base, GetSize());
	}else if (IsPrimitive()){
		//	単純型の書き出し。ファイルにデータを書き込むのはここだけ
		if (IsString()){
			unsigned len;
			is.read((char*)&len, sizeof(len));		//	まず長さを読みだして
			char* buf = new char [len+1];
			is.read(buf, len);				//	文字列を読み出す
			buf[len] = 0;
			WriteString(buf, base);			//	読みだした結果をbaseに格納
			delete[] buf;
		}else{
			assert(0);	//	ここには来ないはず
			is.read((char*)base, GetSize());
		}
	}else{
		for(unsigned i=0; i< composit.size(); ++i){
			UTTypeDesc::Field& field = composit[i];
			if (field.varType == UTTypeDescIf::SINGLE){
				field.type->Read(is, field.GetAddress(base,0));
			}else if (field.varType == UTTypeDescIf::ARRAY){
				unsigned arrayLen;
				is.read((char*)&arrayLen, sizeof(arrayLen));	//	配列の要素数をまず読み出す
				UTTypeDesc::Field* lf = composit.Find(field.lengthFieldName.c_str());
				if (lf){
					lf->WriteNumber(base, arrayLen);
				}else{
					assert(arrayLen == field.length);
				}
				for(unsigned i=0; i<arrayLen; ++i){
					field.type->Read(is, field.GetAddress(base, i));
				}
			}else if (composit[i].varType == UTTypeDescIf::VECTOR){
				unsigned vecLen = (unsigned) composit[i].VectorSize(base);
				is.read((char*)&vecLen, sizeof(vecLen));		//	vectorの要素数をまず読み出す
				for(unsigned i=0; i<vecLen; ++i){
					field.type->Read(is, field.GetAddressEx(base, i));
				}
			}
		} 
	}
}
std::string UTTypeDesc::ReadToString(int i, void* base, int pos) {
	std::string rv;
	if (composit[i].type->IsBool()) {
		bool b = composit[i].type->ReadBool(composit[i].GetAddress(base, pos));
		if (b) rv = "true";
		else rv = "false";
	}
	else if (composit[i].type->IsNumber()) {
		double d = composit[i].type->ReadNumber(composit[i].GetAddress(base, pos));
		std::ostringstream os;
		os << d;
		rv = os.str();
	}
	else if (composit[i].type->IsString()) {
		rv = composit[i].type->ReadString(composit[i].GetAddress(base, pos));
	}
	return rv;
}
void UTTypeDesc::WriteFromString(std::string in, int i, void* base, int pos) {
	if (composit[i].type->IsBool()) {
		std::string s;
		std::istringstream(in) >> s;
		bool b = true;
		if (s[0] == 'f' || s[0] == 'F' || s[0] == 'n' || s[0] == 'N' || s[0] == '0') b = false;
		composit[i].type->WriteBool(b, composit[i].GetAddress(base, pos));
	}
	else if (composit[i].type->IsNumber()) {
		double d;
		std::istringstream(in) >> d;
		composit[i].type->WriteNumber(d, composit[i].GetAddress(base, pos));
	}
	else if (composit[i].type->IsString()) {
		composit[i].type->WriteString(in.c_str(), composit[i].GetAddress(base, pos));
	}
}
//----------------------------------------------------------------------------
//	UTTypeDescDb
UTTypeDescDb::~UTTypeDescDb(){
	db.clear();
}
UTTypeDesc* UTTypeDescDb::Find(UTString tn){
	UTRef<UTTypeDesc> key = new UTTypeDesc;
	if (prefix.length() && tn.compare(0, prefix.length(), prefix)==0){
		tn = tn.substr(prefix.length());
	}
	key->typeName = tn;
	Db::iterator it = db.find(key);
	if (it != db.end()) return *it;
	return NULL;
}
void UTTypeDescDb::SetPrefix(UTString p){
	prefix = p;
	typedef std::vector< UTRef< UTTypeDesc > > Descs;
	Descs descs;
	for(Db::iterator it = db.begin(); it != db.end(); ++it){
		descs.push_back(*it);
	}
	db.clear();
	for(Descs::iterator it = descs.begin(); it != descs.end(); ++it){
		if (prefix.length() && (*it)->typeName.compare(0, prefix.length(), prefix) == 0){
			(*it)->typeName = (*it)->typeName.substr(prefix.length());
		}
	}
	db.insert(descs.begin(), descs.end());
}
void UTTypeDescDb::Link(UTTypeDescDb* lib) {
	if (!lib) lib = this;
	for(Db::iterator it=db.begin(); it!=db.end(); ++it){
		(*it)->Link(lib);
	}
}
bool UTTypeDescDb::LinkCheck(){
	bool rv = true;
	for(Db::iterator it=db.begin(); it!=db.end(); ++it){
		rv = rv && (*it)->LinkCheck();
	}
	for(Db::iterator it=db.begin(); it!=db.end(); ++it){
		(*it)->CheckSimple();
	}
	return rv;
}
void UTTypeDescDb::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	DSTR << UTPadding(w) << "Group: " << group << "   Prefix: " << prefix << std::endl;
	os.width(w);
	for(Db::const_iterator it = db.begin(); it != db.end(); ++it){
		(*it)->Print(os);
		os << std::endl;
	}
}

//---------------------------------------------------------------------------
//	UTTypeDescDbPool
UTRef<UTTypeDescDbPool> UTTypeDescDbPool::pool;
UTTypeDescDbPool* SPR_CDECL UTTypeDescDbPool::GetPool(){
	if (!pool) pool = new UTTypeDescDbPool();
	return pool;
}
UTTypeDescDb* SPR_CDECL UTTypeDescDbPool::Get(std::string gp){
	UTRef<UTTypeDescDb> key = DBG_NEW UTTypeDescDb(gp);
	std::pair<UTTypeDescDbPool::iterator, bool> r = GetPool()->insert(key);
	return *r.first;
}
void SPR_CDECL UTTypeDescDbPool::Print(std::ostream& os){
	for(const_iterator it = GetPool()->begin(); it != GetPool()->end(); ++it){
		(*it)->Print(os);
		os << std::endl;
	}
}

//---------------------------------------------------------------------------
//	UTTypeDescFieldIt

UTTypeDescFieldIt::UTTypeDescFieldIt(UTTypeDesc* d){
	type = d;
	if (type){
		field = type->GetComposit().end();
		if (type->IsPrimitive()){
			//	組み立て型でない場合
			DSTR << "Not a composit type" << std::endl;
			arrayPos = -1;
			arrayLength = 1;
			fieldType=GetTypeId(type);
		}else{
			arrayPos = -1;
			arrayLength = 0;
			fieldType=F_NONE;
		}
	}
}
bool UTTypeDescFieldIt::NextField(void* base){
	if (!type || !type->GetComposit().size()) return false;
	//	次のフィールドへ進む
	if (field == type->GetComposit().end()){
		field = type->GetComposit().begin();
	}else{
		++field;
		if (field == type->GetComposit().end()){
			fieldType = F_NONE;
			return false;
		}
	}
	SetFieldInfo(base);
	return true;
}
bool UTTypeDescFieldIt::PrevField(void* base){
	if (!type || !type->GetComposit().size()) return false;
	//	前のフィールドへ戻る
	if (field == type->GetComposit().begin()){
		field = type->GetComposit().end();
		fieldType = F_NONE;
		return false;
	}else{
		--field;
		SetFieldInfo(base);
		return true;
	}
}
static bool HaveFieldR(UTTypeDesc* type, UTString& name){
	if (type->IsPrimitive()) return false;
	for(UTTypeDesc::Composit::iterator f = type->composit.begin(); f!= type->composit.end(); ++f){
		if (f->name.length() == 0){
			if (HaveFieldR(f->type, name)) return true;
		}else if (name.compare(f->name) == 0){
			return true;
		}
	}
	return false;
}
bool UTTypeDescFieldIt::HaveField(UTString name){			///<	指定の名前のフィールドを持っているならtrue。継承元のフィールドも検索する。
	return HaveFieldR(type, name);
}

void UTTypeDescFieldIt::SetFieldInfo(void* base){
	//	フィールドの配列要素数を設定
	if (field->varType==UTTypeDescIf::SINGLE){
		arrayLength = 1;
	}else if(field->varType== UTTypeDescIf::VECTOR || field->varType==UTTypeDescIf::ARRAY){
		arrayLength = field->length;
		if (field->lengthFieldName.length()){
			for(UTTypeDesc::Composit::iterator it = type->composit.begin(); it != field && it != type->composit.end(); ++it){
				if (it->name.compare(field->lengthFieldName) == 0){
					arrayLength = it->ReadNumber(base, 0);
					break;
				}
			}
		}
	}
	//	配列カウントを初期化
	arrayPos = -1;
	//	フィールドの型を設定
	DSTR << field->name << std::endl;
	if (!field->type){
		DSTR << type->GetTypeName() << " has NULL field type" << std::endl;
		type->Print(DSTR);
		assert(0);
	}
	fieldType = GetTypeId(field->type);
}

UTTypeDescFieldIt::FieldType UTTypeDescFieldIt::GetTypeId(UTTypeDesc* type){
	UTTypeDescFieldIt::FieldType fieldType = F_NONE;
	if(type->IsPrimitive()){
		if(		type->GetTypeName().compare("BYTE")==0
			||	type->GetTypeName().compare("WORD")==0
			||	type->GetTypeName().compare("DWORD")==0
			||	type->GetTypeName().compare("char")==0
			||	type->GetTypeName().compare("short")==0
			||	type->GetTypeName().compare("int")==0
			||	type->GetTypeName().compare("unsigned")==0
			||	type->GetTypeName().compare("size_t")==0
			||	type->GetTypeName().compare("enum")==0){
			fieldType = F_INT;
		}else if (type->GetTypeName().compare("bool")==0
			||	type->GetTypeName().compare("BOOL")==0){
			fieldType = F_BOOL;
		}else if (type->GetTypeName().compare("float")==0
			||	type->GetTypeName().compare("double")==0
			||	type->GetTypeName().compare("FLOAT")==0
			||	type->GetTypeName().compare("DOUBLE")==0){
			fieldType = F_REAL;
		}else if (type->GetTypeName().compare("string")==0
			||  type->GetTypeName().compare("STRING")==0){
			fieldType = F_STR;
		}
	}else{
		fieldType = F_BLOCK;
	}
	return fieldType;
}
UTTypeDescIf* UTTypeDescIf::FindTypeDesc(const char* typeName, const char* moduleName) {
	UTTypeDescDb* db = UTTypeDescDbPool::Get(moduleName);
	return (UTTypeDescIf*)db->Find(typeName);
}
void UTTypeDescIf::PrintPool(std::ostream& os) {
	UTTypeDescDbPool::GetPool()->Print(os);
}


}

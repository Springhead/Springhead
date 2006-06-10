#include "UTTypeDesc.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
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
	if (varType==VECTOR){
		return type->SizeOfVector();
	}else{
		size_t sz = 0;
		if (isReference){
			sz = sizeof(void*);
		}else{
			sz = type->GetSize();
		}
		if (varType==SINGLE){
			return sz; 
		}else if (varType==ARRAY){
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
	if (varType == VECTOR){
		ptr = type->VectorAt(ptr, pos);
	}else if (varType == ARRAY){
		ptr = (const char*)ptr + type->GetSize()*pos;
	}
	return ptr;
}
void* UTTypeDesc::Field::GetAddressEx(void* base, int pos){
	void* ptr = (char*)base + offset;
	if (varType == VECTOR){
		while((int)type->VectorSize(ptr)<=pos) type->VectorPush(ptr);
		ptr = type->VectorAt(ptr, pos);
	}else if (varType == ARRAY){
		ptr = (char*)ptr + type->GetSize()*pos;
	}
	return ptr;
}

void UTTypeDesc::Field::Print(std::ostream& os) const{
	int w = os.width();
	os.width(0);
	os << UTPadding(w) << name.c_str() << "(+" << offset << ") = ";
	if (varType==VECTOR || varType==ARRAY){
		os << "Vector<";
		if (isReference){
			os << "UTRef<" << type->GetTypeName().c_str() << ">";
		}else{
			if (type) os << type->GetTypeName().c_str();
			else os << "(null)";
		}
		os << ">";
		if (length>1) os << " [" << length << "]";
		os << " " << (unsigned int)type->GetSize() * length;
	}else if (isReference){
		os << "UTRef<" << type->GetTypeName().c_str() << ">";
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
		if (!back().type || !back().type->IsComposit()) DSTR << std::endl;
	}
}
void UTTypeDesc::Composit::Link(UTTypeDescDb* db) {
	for(iterator it = begin(); it != end(); ++it){
		it->type = db->Find(it->typeName);
	}
}

//----------------------------------------------------------------------------
//	UTTypeDesc
UTTypeDesc::Field* UTTypeDesc::AddField(std::string pre, std::string tn, 
	std::string n, std::string suf){
	composit.push_back(Field());
	if (pre.compare("vector") == 0){
		composit.back().varType = UTTypeDesc::Field::VECTOR;
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
		if (composit.back().varType == UTTypeDesc::Field::SINGLE){
			composit.back().varType = UTTypeDesc::Field::ARRAY;
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

//----------------------------------------------------------------------------
//	UTTypeDescDb
UTRef<UTTypeDescDb> UTTypeDescDb::theTypeDescDb;
UTTypeDescDb::ProtoDescs UTTypeDescDb::protoDescs;

UTTypeDescDb::~UTTypeDescDb(){
	db.clear();
	protoDescs.clear();
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
void UTTypeDescDb::Link() {
	for(Db::iterator it=db.begin(); it!=db.end(); ++it){
		(*it)->Link(this);
	}
}
void UTTypeDescDb::Print(std::ostream& os) const{
	for(Db::const_iterator it = db.begin(); it != db.end(); ++it){
		(*it)->Print(os);
		os << std::endl;
	}
}
void UTTypeDescDb::RegisterProto(UTTypeDesc* n){
	protoDescs.push_back(ProtoDesc());
	protoDescs.back().fileType = prefix;
	protoDescs.back().desc = n;
}


//---------------------------------------------------------------------------
//	FIFieldIt

FIFieldIt::FIFieldIt(UTTypeDesc* d){
	type = d;
	if (type){
		field = &*type->GetComposit().end();
	}else{
		field = NULL;
	}
	arrayPos = -1;
	arrayLength = 0;
	fieldType=F_NONE;
}
bool FIFieldIt::NextField(){
	if (!type->IsComposit()) return false;
	//	次のフィールドへ進む
	if (field == &*type->GetComposit().end()){
		field = &*type->GetComposit().begin();
	}else{
		++field;
		if (field == &*type->GetComposit().end()){
			fieldType = F_NONE;
			return false;
		}
	}
	//	フィールドの配列要素数を設定
	if (field->varType==UTTypeDesc::Field::SINGLE){
		arrayLength = 1;
	}else if(field->varType==UTTypeDesc::Field::VECTOR){
		arrayLength = field->length;
	}else if(field->varType==UTTypeDesc::Field::ARRAY){
		arrayLength = field->length;
	}
	//	配列カウントを初期化
	arrayPos = -1;
	//	フィールドの型を設定
	if(		field->type->GetTypeName().compare("BYTE")==0
		||	field->type->GetTypeName().compare("WORD")==0
		||	field->type->GetTypeName().compare("DWORD")==0
		||	field->type->GetTypeName().compare("char")==0
		||	field->type->GetTypeName().compare("short")==0
		||	field->type->GetTypeName().compare("int")==0
		||	field->type->GetTypeName().compare("unsigned")==0
		||	field->type->GetTypeName().compare("size_t")==0
		||	field->type->GetTypeName().compare("enum")==0){
		fieldType = F_INT;
	}else if (field->type->GetTypeName().compare("bool")==0
		||	field->type->GetTypeName().compare("BOOL")==0){
		fieldType = F_BOOL;
	}else if (field->type->GetTypeName().compare("float")==0
		||	field->type->GetTypeName().compare("double")==0
		||	field->type->GetTypeName().compare("FLOAT")==0
		||	field->type->GetTypeName().compare("DOUBLE")==0){
		fieldType = F_REAL;
	}else if (field->type->GetTypeName().compare("string")==0
		||  field->type->GetTypeName().compare("STRING")==0){
		fieldType = F_STR;
	}else if (field->type->IsComposit()){
		fieldType = F_BLOCK;
	}
	return true;
}


}

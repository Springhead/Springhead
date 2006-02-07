#include "FISpirit.h"
#include "FITypeDesc.h"
#include <sstream>


namespace Spr{;
/**	構造体のアライメント(変数の位置あわせ)に対応するために作ったが
	簡単には行かないことが分かった．（アライメント8でも，DWORDが2個
	並ぶと，8バイトしか使わない．(隙間ができない)．	*/
const int STRUCTALIGN = 4;
const int VECTORALIGN = 1;
inline size_t AlignSize(size_t sz, size_t align){
	size_t asz = (sz/align)*align + (sz%align ? align : 0);
	return asz;
}

//----------------------------------------------------------------------------
//	FITypeDesc::Field
FITypeDesc::Field::~Field(){
	if (type){
		UTRef<FITypeDesc> t = type;
		type = NULL;
	}
}


size_t FITypeDesc::Field::GetSize(){
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
void FITypeDesc::Field::AddEnumConst(std::string name, int val){
	enums.push_back(std::make_pair(name, val));
}
///
void FITypeDesc::Field::AddEnumConst(std::string name){
	int val = 0;
	if (enums.size()) val = enums.back().second+1;
	enums.push_back(std::make_pair(name, val));
}
const void* FITypeDesc::Field::GetAddress(const void* base, int pos){
	const void* ptr = (const char*)base + offset;
	if (varType == VECTOR){
		ptr = type->VectorAt(ptr, pos);
	}else if (varType == ARRAY){
		ptr = (const char*)ptr + type->GetSize()*pos;
	}
	return ptr;
}

void FITypeDesc::Field::Print(std::ostream& os) const{
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
		os << " " << sizeof(FIVVector<int>) * length;
	}else if (isReference){
		os << "UTRef<" << type->GetTypeName().c_str() << ">";
		if (length>1) os << " [" << length << "]";
		os << " " << sizeof(UTRef<FITypeDesc::Field>) * length;
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
//	FITypeDesc::Composit
int FITypeDesc::Composit::Size(UTString id){
	int rv = 0;
	for(iterator it = begin(); it != end(); ++it){
		if (id.length()==0 || id.compare(it->name)){
			rv += (int)it->GetSize();
		}
	}
	return rv;
}
void FITypeDesc::Composit::Print(std::ostream& os) const{
	for(const_iterator it = begin(); it != end(); ++it){
		it->Print(os);
	}
	if (size()){
		if (!back().type || !back().type->IsComposit()) DSTR << std::endl;
	}
}
void FITypeDesc::Composit::Link(FITypeDescDb* db) {
	for(iterator it = begin(); it != end(); ++it){
		it->type = db->Find(it->typeName);
	}
}

//----------------------------------------------------------------------------
//	FITypeDesc
FITypeDesc::Field* FITypeDesc::AddField(std::string pre, std::string tn, 
	std::string n, std::string suf){
	composit.push_back(Field());
	if (pre.compare("vector") == 0) composit.back().varType = FITypeDesc::Field::VECTOR;
	if (pre.compare("UTRef") == 0) composit.back().isReference = true;
	if (pre.compare("pointer") == 0) composit.back().isReference = true;
	if (suf.size()){
		std::istringstream is(suf);
		is >> composit.back().length;
		if (!is.good()){
			composit.back().lengthFieldName = suf;
		}
		if (composit.back().varType == FITypeDesc::Field::SINGLE){
			composit.back().varType = FITypeDesc::Field::ARRAY;
		}
	}

	composit.back().typeName = tn;
	composit.back().name = n;

	return &composit.back();
};

FITypeDesc::Field* FITypeDesc::AddBase(std::string tn){
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
void FITypeDesc::Link(FITypeDescDb* db) {
	composit.Link(db);
}
void FITypeDesc::Print(std::ostream& os) const{
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
//	FITypeDescDb
FITypeDescDb::ProtoDescs FITypeDescDb::protoDescs;

FITypeDescDb::~FITypeDescDb(){
	db.clear();
}
FITypeDesc* FITypeDescDb::Find(UTString tn){
	UTRef<FITypeDesc> key = new FITypeDesc;
	if (prefix.length() && tn.compare(0, prefix.length(), prefix)==0){
		tn = tn.substr(prefix.length());
	}
	key->typeName = tn;
	Db::iterator it = db.find(key);
	if (it != db.end()) return *it;
	return NULL;
}
void FITypeDescDb::SetPrefix(UTString p){
	prefix = p;
	typedef std::vector< UTRef< FITypeDesc > > Descs;
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
void FITypeDescDb::Link() {
	for(Db::iterator it=db.begin(); it!=db.end(); ++it){
		(*it)->Link(this);
	}
}
void FITypeDescDb::Print(std::ostream& os) const{
	for(Db::const_iterator it = db.begin(); it != db.end(); ++it){
		(*it)->Print(os);
		os << std::endl;
	}
}
void FITypeDescDb::RegisterProto(FITypeDesc* n){
	protoDescs.push_back(ProtoDesc());
	protoDescs.back().fileType = prefix;
	protoDescs.back().desc = n;
}

}

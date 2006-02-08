#include "FIFileX.h"
#include "FITypeDesc.h"
#include <fstream>
#include <sstream>
#include <Springhead.h>

namespace Spr{;

namespace FileX{
static FIFileContext* fileContext;
//	XFileのdataの読み出しの関数
enum FieldType{
	F_NONE, F_INT, F_REAL, F_STR, F_BLOCK
};
static FieldType nextField;
static FIFileX* fileX;
struct Field{
	FITypeDesc* desc;
	FITypeDesc::Composit::iterator field;
	int arrayCount;
	int arrayLength;
	Field(FITypeDesc* d){
		desc = d;
		field = d->GetComposit().end();
		arrayCount = -1;
		arrayLength = 0;
	}
};
static std::vector<Field> fieldStack;
static FITypeDesc* tdesc;
static bool Desc(){return tdesc;}


static void DataStart(const char* b, const char* e){
	std::string tn(b,e);
	DSTR << "loading " << tn << std::endl;
	tdesc = fileX->GetDb()->Find(tn);
	if(tdesc) fieldStack.push_back(Field(tdesc));
	if (!tdesc){
		DSTR << "Error: " << tn << " not defined." << std::endl;
	}

	//	ロード用の構造体の用意
	fileContext->primitives.Push();
	fileContext->primitives.Top() = tdesc->Construct();
}
static void DataCreate(const char* b, const char* e){
	ObjectIf* obj = fileContext->Create(tdesc->GetIfInfo(), fileContext->primitives.Top());
	fileContext->objects.Push(obj);
}
static void DataEnd(char c){
	DSTR << "DataEnd" << std::endl;
	fileContext->objects.Pop();
	tdesc->Delete(fileContext->primitives.Top());
	fileContext->primitives.Pop();
}
static void BlockStart(const char* b, const char* e){
	DSTR << "blockStart" << std::endl;
	char* base = (char*)fileContext->primitives.Top();
	void* ptr = fieldStack.back().field->GetAddress(base, fieldStack.back().arrayCount);
	fileContext->primitives.Push(ptr);
	fieldStack.push_back(Field(fieldStack.back().field->type));
}
static void BlockEnd(const char* b, const char* e){
	DSTR << "blockEnd" << std::endl;
	fieldStack.pop_back();
	fileContext->primitives.Pop();
}
static bool NextField(){
	if (!fieldStack.back().desc->IsComposit()) return false;
	//	次のフィールドへ進む
	if (fieldStack.back().field==fieldStack.back().desc->GetComposit().end()){
		fieldStack.back().field = fieldStack.back().desc->GetComposit().begin();
	}else{
		fieldStack.back().field++;
		if (fieldStack.back().field == fieldStack.back().desc->GetComposit().end()){
			nextField = F_NONE;
			return false;
		}
	}
	//	フィールドの配列要素数を設定
	if (fieldStack.back().field->varType==FITypeDesc::Field::SINGLE){
		fieldStack.back().arrayLength = 1;
	}else if(fieldStack.back().field->varType==FITypeDesc::Field::VECTOR){
		//	TODO lengthFieldName からとってくる．
	}else if(fieldStack.back().field->varType==FITypeDesc::Field::ARRAY){
		fieldStack.back().arrayLength = fieldStack.back().field->length;
	}
	//	配列カウントを初期化
	fieldStack.back().arrayCount = -1;
	//	フィールドの型を設定
	if (	fieldStack.back().field->type->GetTypeName().compare("BYTE")==0
		||	fieldStack.back().field->type->GetTypeName().compare("WORD")==0
		||	fieldStack.back().field->type->GetTypeName().compare("DWORD")==0
		||	fieldStack.back().field->type->GetTypeName().compare("char")==0
		||	fieldStack.back().field->type->GetTypeName().compare("short")==0
		||	fieldStack.back().field->type->GetTypeName().compare("int")==0
		||	fieldStack.back().field->type->GetTypeName().compare("enum")==0){
		nextField = F_INT;
	}else if (fieldStack.back().field->type->GetTypeName().compare("float")==0
		||	fieldStack.back().field->type->GetTypeName().compare("double")==0
		||	fieldStack.back().field->type->GetTypeName().compare("FLOAT")==0
		||	fieldStack.back().field->type->GetTypeName().compare("DOUBLE")==0){
		nextField = F_REAL;
	}else if (fieldStack.back().field->type->GetTypeName().compare("string")==0
		||  fieldStack.back().field->type->GetTypeName().compare("STRING")==0){
		nextField = F_STR;
	}else if (fieldStack.back().field->type->IsComposit()){
		nextField = F_BLOCK;
	}
	return true;
}
static bool ArrayCount(){
	fieldStack.back().arrayCount++;
	return fieldStack.back().arrayLength-1 > fieldStack.back().arrayCount;
}
static bool IsFieldInt(){ return nextField==F_INT; }
static bool IsFieldReal(){ return nextField==F_REAL; }
static bool IsFieldStr(){ return nextField==F_STR; }
static bool IsFieldBlock(){ return nextField==F_BLOCK; }

static void NumSet(double v){
	if (fieldStack.back().arrayCount==0){
		DSTR << fieldStack.back().field->name << " = " ;
	}
	DSTR << v;
	if (fieldStack.back().arrayCount==fieldStack.back().arrayLength-1) DSTR << std::endl;

	fieldStack.back().field->WriteNumber(
		fileContext->primitives.Top(), v, fieldStack.back().arrayCount);
}
static void StrSet(const char* b, const char* e){
	std::string str(b,e);
	if (fieldStack.back().arrayCount==0){
		DSTR << fieldStack.back().field->name << " = " ;
	}
	DSTR << str;
	if (fieldStack.back().arrayCount==fieldStack.back().arrayLength-1) DSTR << std::endl;

	fieldStack.back().field->WriteString(
		fileContext->primitives.Top(), str.c_str(), fieldStack.back().arrayCount);
}
static void RefSet(const char* b, const char* e){
	DSTR << "ref(" << std::string(b,e) << ")";
}


//	XFileのtemplateの読み出しの関数
static void TempStart(const char* b, const char* e){
	tdesc = DBG_NEW FITypeDesc(std::string(b,e));
}
static void DefType(const char* b, const char* e){
	tdesc->GetComposit().push_back(FITypeDesc::Field());
	tdesc->GetComposit().back().typeName.assign(b, e);
}
static void DefId(const char* b, const char* e){
	tdesc->GetComposit().back().name.assign(b, e);
}
static void ArrayId(const char* b, const char* e){
	tdesc->GetComposit().back().varType = FITypeDesc::Field::VECTOR;
	tdesc->GetComposit().back().lengthFieldName.assign(b, e);
}
static void ArrayNum(int n){
	tdesc->GetComposit().back().varType = FITypeDesc::Field::ARRAY;
	tdesc->GetComposit().back().length = n;
}
static void TempEnd(char c){
	tdesc->Link(fileX->GetDb());
	DSTR << "load template:" << std::endl;
	tdesc->Print(DSTR);
	fileX->GetDb()->RegisterDesc(tdesc);
}
}
using namespace FileX;

void FIFileX::Init(FITypeDescDb* db_){
	typeDb = *db_;
	typeDb.RegisterAlias("Vec3f", "Vector");
	typeDb.RegisterAlias("Vec2f", "Coords2d");
	typeDb.RegisterAlias("Affinef", "Matrix3x3");
	typeDb.RegisterAlias("Affined", "Matrix4x4");

	using namespace std;
	using namespace boost::spirit;
	using namespace Spr;
	//	パーサの定義
	//	本文用パーサ
	start		= (str_p("xof 0302txt 0064") | str_p("xof 0303txt 0032")) 
					>> *(temp | data);

	temp		= str_p("template") >> id[&TempStart] >> ch_p('{') >> !uuid
					>> *define >> ch_p('}')[&TempEnd];
	uuid		= ch_p('<') >> *~ch_p('>') >> ch_p('>');
	define		= defArray | defNormal | defOpen | defRestrict;
	defNormal	= id[&DefType] >> id[&DefId] >> ";";
	defArray	= str_p("array") >> id[&DefType] >> id[&DefId] 
					>> '[' >> arraySuffix >> ']' >> ';';
	defOpen		= str_p("[...]");
	defRestrict	= ch_p('[') >> *(id >> ',') >> id >> ']';
	arraySuffix	= id[&ArrayId] | int_p[&ArrayNum];

	data		= id[&DataStart] >> !id >> '{' >>
				  if_p(&Desc)[ block[&DataCreate] >> *(data|ref) ].
				  else_p[ *(blockSkip | ~ch_p('}')) ]		//<	知らない型名の場合スキップ
				  >> ch_p('}')[&DataEnd];
	blockSkip	= ch_p('{') >> *(blockSkip|~ch_p('}')) >> ch_p('}');
	ref			= ch_p('{') >> id[&RefSet] >> ch_p('}');
	block		= while_p(&NextField)[
					while_p(&ArrayCount)[
						if_p(&IsFieldInt)[ iNum >> ',' ] >>
						if_p(&IsFieldReal)[ rNum  >> ',' ] >>
						if_p(&IsFieldStr)[ str >> ',' ] >>
						if_p(&IsFieldBlock)[ eps_p[&BlockStart] >> block[&BlockEnd] >> ',' ]
					] >> 
					if_p(&IsFieldInt)[ iNum >> ';' ] >>
					if_p(&IsFieldReal)[ rNum  >> ';' ] >>
					if_p(&IsFieldStr)[ str >> ';' ] >>
					if_p(&IsFieldBlock)[ eps_p[&BlockStart] >> block[&BlockEnd] >> ';' ]
				  ];
	id			= lexeme_d[ (alpha_p|'_') >> *(alnum_p|'_') ];
	iNum		= int_p[&NumSet];
	rNum		= real_p[&NumSet];
	str			= lexeme_d[ 
					ch_p('"') >> *( (ch_p('\\')>>anychar_p) | 
						~ch_p('"') ) >> ch_p('"') ][&StrSet];

	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	cmt		=	space_p
				|	"/*" >> *(~ch_p('*') | '*'>>~ch_p('/')) >> !ch_p('*') >> '/'
				|	"//" >> *~ch_p('\n') >> '\n';
}
void FIFileX::Load(FIFileContext* fc){
	using namespace std;
	using namespace boost::spirit;
	using namespace Spr;

	fileContext = fc;
	fileX = this;
	parse_info<const char*> info = parse(fileContext->fileInfo.back().start, 
		fileContext->fileInfo.back().end, start, cmt);
/*
	const char* e = b+strlen(b);
	int len = e-info.stop;
	if (len > 70) len = 70;
	string stop(info.stop, info.stop+len);
	DSTR << info.stop-b;
	DSTR << "Error: near \'" << stop << "\'" << std::endl;
*/
}

};

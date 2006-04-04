#include "FIFileX.h"
#include "FITypeDesc.h"
#include <fstream>
#include <sstream>
#include <Springhead.h>

namespace Spr{;

namespace FileX{
static FIFileContext* fileContext;

///	XFileのフィールドの種類
enum FIXFieldType{
	F_NONE, F_INT, F_REAL, F_STR, F_BLOCK
};
///	Xファイルのフィールド
struct FIXField{
	FITypeDesc* desc;
	FITypeDesc::Composit::iterator field;
	int arrayCount;
	int arrayLength;
	FIXFieldType nextField;
	FIXField(FITypeDesc* d){
		desc = d;
		field = d->GetComposit().end();
		arrayCount = -1;
		arrayLength = 0;
		nextField=F_NONE;
	}
};

static FIFileX* fileX;
///	ロード中のフィールド
static std::vector<FIXField> fieldStack;

///	テンプレートのTypeDescがあるかどうか．
static bool DescAvail(){return fieldStack.back().desc;}

///	ノード(テンプレートのデータ)のデータ読み出しの準備
static void NodeStart(const char* b, const char* e){
	std::string tn(b,e);
	DSTR << "NodeStart " << tn << std::endl;
	FITypeDesc* desc = fileX->GetDb()->Find(tn);
	if (!desc) desc = fileX->GetDb()->Find(tn + "Desc");
	if (!desc){
		DSTR << "Error: " << tn << " not defined." << std::endl;
	}
	if(desc) fieldStack.push_back(FIXField(desc));

	//	ロード用の構造体の用意
	fileContext->primitives.Push();
	fileContext->primitives.Top() = new FIFileContext::Primitive(desc);
}

///	ノードを読んで作ったObjectDescから，オブジェクトを作成する．
static void CreateNode(const char* b, const char* e){
	ObjectIf* obj = fileContext->Create(
		fieldStack.back().desc->GetIfInfo(), fileContext->primitives.Top()->obj);
	fileContext->objects.Push(obj);
}

///	ノード読み出しの後処理
static void NodeEnd(const char* b, const char* e){
	DSTR << "NodeEnd " << fieldStack.back().desc->GetTypeName() << std::endl;
	fileContext->objects.Pop();
	fileContext->primitives.Pop();
	fieldStack.pop_back();
}

///	ブロック型の読み出し準備
static void BlockStart(const char* b, const char* e){
	DSTR << "blockStart" << std::endl;
	char* base = (char*)fileContext->primitives.Top()->obj;
	void* ptr = fieldStack.back().field->GetAddressEx(base, fieldStack.back().arrayCount);
	fileContext->primitives.Push(new FIFileContext::Primitive(NULL, ptr));
	fieldStack.push_back(FIXField(fieldStack.back().field->type));
}

///	ブロック型の終了
static void BlockEnd(const char* b, const char* e){
	DSTR << "blockEnd" << std::endl;
	fieldStack.pop_back();
	fileContext->primitives.Pop();
}

/**	ブロック読み出し中，フィールドを読む前に呼ばれる．
	TypeDescを見て次に読み出すべきフィールドをセットする．
	読み出すべきフィールドがある間 true を返す．	*/
static bool NextField(){
	if (!fieldStack.back().desc->IsComposit()) return false;
	//	次のフィールドへ進む
	if (fieldStack.back().field==fieldStack.back().desc->GetComposit().end()){
		fieldStack.back().field = fieldStack.back().desc->GetComposit().begin();
	}else{
		fieldStack.back().field++;
		if (fieldStack.back().field == fieldStack.back().desc->GetComposit().end()){
			fieldStack.back().nextField = F_NONE;
			return false;
		}
	}
	//	フィールドの配列要素数を設定
	if (fieldStack.back().field->varType==FITypeDesc::Field::SINGLE){
		fieldStack.back().arrayLength = 1;
	}else if(fieldStack.back().field->varType==FITypeDesc::Field::VECTOR){
		fieldStack.back().arrayLength = fieldStack.back().field->length;
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
		fieldStack.back().nextField = F_INT;
	}else if (fieldStack.back().field->type->GetTypeName().compare("float")==0
		||	fieldStack.back().field->type->GetTypeName().compare("double")==0
		||	fieldStack.back().field->type->GetTypeName().compare("FLOAT")==0
		||	fieldStack.back().field->type->GetTypeName().compare("DOUBLE")==0){
		fieldStack.back().nextField = F_REAL;
	}else if (fieldStack.back().field->type->GetTypeName().compare("string")==0
		||  fieldStack.back().field->type->GetTypeName().compare("STRING")==0){
		fieldStack.back().nextField = F_STR;
	}else if (fieldStack.back().field->type->IsComposit()){
		fieldStack.back().nextField = F_BLOCK;
	}
	return true;
}
///	配列のカウント．まだ読み出すべきデータが残っていれば true を返す．
static bool ArrayCount(){
	fieldStack.back().arrayCount++;
	return fieldStack.back().arrayLength > fieldStack.back().arrayCount;
}

static bool IsFieldInt(){ return fieldStack.back().nextField==F_INT; }
static bool IsFieldReal(){ return fieldStack.back().nextField==F_REAL; }
static bool IsFieldStr(){ return fieldStack.back().nextField==F_STR; }
static bool IsFieldBlock(){ return fieldStack.back().nextField==F_BLOCK; }

static double numValue;
static std::string strValue;
static void NumSet(double v){
	numValue = v;
}
static void StrSet(const char* b, const char* e){
	strValue.assign(b,e);
}

///	ObjectDescに読み出した値を書き込む
static void SetVal(const char* b, const char* e){
	char ch = *b;

	FIXField& curField = fieldStack.back();
	//	debug 出力
	if (curField.nextField!=F_NONE){
		if (curField.nextField==F_BLOCK){
			DSTR << " => (" << curField.field->typeName << ") " << curField.field->name << std::endl;
		}else{
			if (curField.arrayCount==0){
				DSTR << "(" << curField.field->typeName << ") " << curField.field->name << " = " ;
			}
		}
		if (curField.nextField == F_REAL || curField.nextField == F_INT){
			DSTR << " " << numValue;
		}else if (curField.nextField == F_STR){
			DSTR << " " << strValue;
		}
		if (ch == ';') DSTR << std::endl;
	}
	//	ここまで

	if(curField.nextField == F_REAL || curField.nextField == F_INT){
		curField.field->WriteNumber(
			fileContext->primitives.Top()->obj, numValue, curField.arrayCount);
	}else if(curField.nextField == F_STR){
		curField.field->WriteString(
			fileContext->primitives.Top()->obj, strValue.c_str(), curField.arrayCount);
	}
	if (ch == ';'){
		curField.arrayCount=FITypeDesc::BIGVALUE;
	}
}

///	参照型を書き込む．(未完成)
static void RefSet(const char* b, const char* e){
	DSTR << "ref(" << std::string(b,e) << ") not yet implemented." << std::endl;
}

static FITypeDesc* tdesc;
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



class ExpectParser {
	std::string msg; 
	mutable const char* last;
public:
    ExpectParser(const char *m) : msg(m) {}
	typedef boost::spirit::nil_t result_t;
    
    template <typename ScannerT>
    int operator()(ScannerT const& scan, result_t& /*result*/) const { 
		if (last == scan.first) exit(-1);
		last = scan.first;
		int len = scan.last - scan.first;
		if (len > 60) len = 60;
		DSTR << "Error:";
		DSTR.write(scan.first, len);
		DSTR << std::endl;
		DSTR << " " << msg << " is expected." <<std::endl; 
		return 0;
    }
}; 

typedef boost::spirit::functor_parser<ExpectParser> ExpP;



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
	start		= (str_p("xof 0302txt 0064") | str_p("'xof 0303txt 0032'") | ExpP("'xof 0303txt 0032'")) 
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
	arraySuffix	= id[&ArrayId] | int_p[&ArrayNum] | ExpP("id or int value");

	data		= id[&NodeStart] >> !id >> (ch_p('{') | ExpP("'{'1")) >>
				  if_p(&DescAvail)[ block[&CreateNode] >> *(data|ref) ].
				  else_p[ *(blockSkip | ~ch_p('}')) ]		//<	知らない型名の場合スキップ
				  >> (ch_p('}') | ExpP("'}'"))[&NodeEnd];
	blockSkip	= ch_p('{') >> *(blockSkip|~ch_p('}')) >> ch_p('}');
	ref			= ch_p('{') >> (id[&RefSet] | ExpP("id")) >> (ch_p('}')|ExpP("'}'"));
	block		= while_p(&NextField)[
					while_p(&ArrayCount)[
						exp >> ((ch_p(',')|ch_p(';'))[&SetVal] | ExpP("',' or ';'"))
					]
				  ];
	exp			= if_p(&IsFieldInt)[ iNum | ExpP("int value") ] >>
				  if_p(&IsFieldReal)[ rNum | ExpP("numeric value") ] >>
				  if_p(&IsFieldStr)[ str | ExpP("string") ] >>
				  if_p(&IsFieldBlock)[ eps_p[&BlockStart] >> block[&BlockEnd] ];
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

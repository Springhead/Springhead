#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FIFileX.h"
#include "FITypeDesc.h"
#include "FINodeHandler.h"
#include <fstream>
#include <sstream>
#include <Springhead.h>

namespace Spr{;

IF_IMP(FIFileX, FIFile);
OBJECTIMP(FIFileX, FIFile);


//#define TRACE_PARSE
#ifdef TRACE_PARSE
# define PDEBUG(x)	x
#else 
# define PDEBUG(x)
#endif

extern void RegisterTypes();
extern void RegisterNodeHandlers();


namespace FileX{
static FIFileContext* fileContext;
static FIFileX* fileX;

///	テンプレートのTypeDescがあるかどうか．
static bool TypeAvail(){
	return fileContext->fieldIts.size() && fileContext->fieldIts.back().type;
}

///	ノードデータの読み出し準備
static void NodeStart(const char* b, const char* e){
	std::string tn(b,e);
	PDEBUG( DSTR << "NodeStart " << tn << std::endl );

	//	型情報の取得
	FITypeDesc* type = fileX->GetDb()->Find(tn);
	if (!type) type = fileX->GetDb()->Find(tn + "Desc");
	
	if (type){
		fileContext->PushType(type);	//	これからロードする型としてPush
	}else{
		tn.append(" not defined.");
		fileContext->ErrorMessage(b, tn.c_str());
	}
}
///	ノードの名前の設定
static void NameSet(const char* b, const char* e){
	fileContext->datas.back()->name = UTString(b,e);
}
///	読み出したデータ(ObjectDesc)から，オブジェクトを作成する．
static void LoadNode(const char* b, const char* e){
	fileContext->LoadNode();
}

///	ノード読み出しの後処理
static void NodeEnd(const char* b, const char* e){
	PDEBUG(DSTR << "NodeEnd " << fileContext->fieldIts.back().desc->GetTypeName() << std::endl);
	fileContext->EndNode();
	fileContext->PopType();
}

///	ブロック型の読み出し準備
static void BlockStart(const char* b, const char* e){
	PDEBUG(DSTR << "blockStart" << std::endl);
	fileContext->EnterBlock();
}

///	ブロック型の終了
static void BlockEnd(const char* b, const char* e){
	PDEBUG(DSTR << "blockEnd" << std::endl);
	fileContext->LeaveBlock();
}

/**	ブロック読み出し中，フィールドを読む前に呼ばれる．
	TypeDescを見て次に読み出すべきフィールドをセットする．
	読み出すべきフィールドがある間 true を返す．	*/
static bool NextField(){
	return fileContext->fieldIts.NextField();
}
///	配列のカウント．まだ読み出すべきデータが残っていれば true を返す．
static bool ArrayCount(){
	return fileContext->fieldIts.IncArrayPos();
}

static bool IsFieldInt(){ return fileContext->fieldIts.back().fieldType==FIFieldIt::F_INT; }
static bool IsFieldReal(){ return fileContext->fieldIts.back().fieldType==FIFieldIt::F_REAL; }
static bool IsFieldStr(){ return fileContext->fieldIts.back().fieldType==FIFieldIt::F_STR; }
static bool IsFieldBlock(){ return fileContext->fieldIts.back().fieldType==FIFieldIt::F_BLOCK; }
static bool IsFieldBool(){ return fileContext->fieldIts.back().fieldType==FIFieldIt::F_BOOL; }

static double numValue;
static std::string strValue;
static bool boolValue;
static void NumSet(double v){
	numValue = v;
}
static void BoolSet(const char* b, const char* e){
	UTString v(b,e);
	boolValue = (v.compare("true")==0) || (v.compare("TRUE")==0) || (v.compare("1")==0);
}
static void StrSet(const char* b, const char* e){
	strValue.assign(b+1,e-1);
}

///	ObjectDescに読み出した値を書き込む
static void SetVal(const char* b, const char* e){
	char ch = *b;

	FIFieldIt& curField = fileContext->fieldIts.back();
	//	debug 出力
#ifdef TRACE_PARSE
	if (curField.fieldType!=F_NONE){
		if (curField.fieldType==F_BLOCK){
			DSTR << " => (" << curField.field->typeName << ") " << curField.field->name << std::endl;
		}else{
			if (curField.arrayPos==0){
				DSTR << "(" << curField.field->typeName << ") " << curField.field->name << " = " ;
			}
		}
		if (curField.fieldType == F_REAL || curField.fieldType == F_INT){
			DSTR << " " << numValue;
		}else if (curField.fieldType == F_STR){
			DSTR << " " << strValue;
		}
		if (ch == ';') DSTR << std::endl;
	}
#endif
	//	ここまで
	
	if (fileContext->fieldIts.IsBool()){
		fileContext->WriteBool(boolValue);
	}else if (fileContext->fieldIts.IsNumber()){
		fileContext->WriteNumber(numValue);
	}else if (fileContext->fieldIts.IsString()){
		fileContext->WriteString(strValue);
	}
	if (ch == ';'){
		curField.arrayPos=FITypeDesc::BIGVALUE;
	}
}

///	参照型を書き込む．(未完成)
static void RefSet(const char* b, const char* e){
	//DSTR << "ref(" << std::string(b,e) << ") not yet implemented." << std::endl;
	std::string ref(b,e);
	fileContext->AddLink(ref, b);
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
	PDEBUG(DSTR << "load template:" << std::endl);
	tdesc->Print(DSTR);
	fileX->GetDb()->RegisterDesc(tdesc);
}
}
using namespace FileX;


class ExpectParser {
	std::string msg; 
public:
    ExpectParser(const char *m) : msg(m) {}
	static std::ostream* errorStr;

	typedef boost::spirit::nil_t result_t;
    
    template <typename ScannerT>
	int operator()(ScannerT const& scan, result_t& /*result*/) const {
		if (!scan.at_end()){
			std::string str = msg + std::string(" is expected");
			fileContext->ErrorMessage(scan.first, str.c_str());
		}
		return -1;
    }
}; 

typedef boost::spirit::functor_parser<ExpectParser> ExpP;


FIFileX::FIFileX(){
	Init();
}
void FIFileX::Init(FITypeDescDb* db, FINodeHandlers* h){
	if (!FITypeDescDb::theTypeDescDb) RegisterTypes();
	if (!FINodeHandlers::theNodeHandlers) RegisterNodeHandlers();
	if (h){
		handlers = *h;
	}else{
		handlers = *FINodeHandlers::theNodeHandlers;
	}
	if (db){
		typeDb = *db;
	}else{
		typeDb = *FITypeDescDb::theTypeDescDb;
		extern UTRef<FITypeDescDb> typeDescDb;
	}
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
					>> *(temp | data | ExpP("template or data"));

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

	data		= id[&NodeStart] >> !id[&NameSet] >> (ch_p('{') | ExpP("'{'")) >>
				  if_p(&TypeAvail)[ block[&LoadNode] >> *(data|ref) ].
//				  else_p[ *(blockSkip | ~ch_p('}')) ]		//<	知らない型名の場合スキップ
				  else_p[ *blockSkip ]		//<	知らない型名の場合スキップ
				  >> (ch_p('}') | ExpP("'}'"))[&NodeEnd];
	blockSkip	= ch_p('{') >> *(blockSkip|~ch_p('}')) >> ch_p('}');
	ref			= ch_p('{') >> (id[&RefSet] | ExpP("id")) >> (ch_p('}')|ExpP("'}'"));
	block		= while_p(&NextField)[
					while_p(&ArrayCount)[
						exp >> ((ch_p(',')|ch_p(';'))[&SetVal] | ExpP("',' or ';'"))
					]
				  ];
	exp			= if_p(&IsFieldBool)[ boolVal | ExpP("bool value") ] >>
				  if_p(&IsFieldInt)[ iNum | ExpP("int value") ] >>
				  if_p(&IsFieldReal)[ rNum | ExpP("numeric value") ] >>
				  if_p(&IsFieldStr)[ str | ExpP("string") ] >>
				  if_p(&IsFieldBlock)[ eps_p[&BlockStart] >> block[&BlockEnd] ];
	id			= lexeme_d[ (alpha_p|'_') >> *(alnum_p|'_') ];
	boolVal		= (str_p("true") | "TRUE" | "false" | "FALSE" | "1" | "0")[&BoolSet];
	iNum		= int_p[&NumSet];
	rNum		= real_p[&NumSet];
	str			= lexeme_d[ 
					ch_p('"') >> *( (ch_p('\\')>>anychar_p) | 
						~ch_p('"') ) >> ch_p('"') ][&StrSet];

	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	cmt		=	space_p
				|	"/*" >> *(~ch_p('*') | '*'>>~ch_p('/')) >> !ch_p('*') >> '/'
				|	"//" >> *~ch_p('\n') >> '\n'
				|	"#" >> *~ch_p('\n') >> '\n';
}



//------------------------------------------------------------------------------
void FIFileX::LoadImp(FIFileContext* fc){
	using namespace std;
	using namespace boost::spirit;
	using namespace Spr;
	fileContext = fc;
	fileX = this;
	parse_info<const char*> info = parse(
		fileContext->fileInfo.back().start, 
		fileContext->fileInfo.back().end, start, cmt);
}
#define INDENT(x)	UTPadding((sc->objects.size()+x)*2)
//<< (sc->objects.size()+x)
void FIFileX::OnSaveFileStart(FISaveContext* sc){
	sc->file << "xof 0302txt 0064" << std::endl;
}
static bool cont;
void FIFileX::OnSaveNodeStart(FISaveContext* sc){
	sc->file << INDENT(-1) << sc->datas.back()->type->GetTypeName();
	NamedObjectIf* n = ICAST(NamedObjectIf, sc->objects.back());
	if (n && n->GetName()) sc->file << " " << n->GetName();
	sc->file << "{" << std::endl;
	cont = false;
}
void FIFileX::OnSaveNodeEnd(FISaveContext* sc){
	sc->file << INDENT(-1) << "}" << std::endl;
}
void FIFileX::OnSaveDataEnd(FISaveContext* sc){
	if (cont) sc->file << std::endl;
}

void FIFileX::OnSaveFieldStart(FISaveContext* sc, int nElements){
	if (!cont){
		sc->file << INDENT(0);
		cont = true;
	}
}
void FIFileX::OnSaveFieldEnd(FISaveContext* sc, int nElements){
	if (!cont) sc->file << INDENT(0);
	sc->file << ";";
	cont = true;
	if (sc->fieldIts.Top().fieldType == FIFieldIt::F_BLOCK){
		sc->file << std::endl;
		cont = false;
	}
}
void FIFileX::OnSaveElementEnd(FISaveContext* sc, int nElements, bool last){
	if (!last) sc->file << ",";
}
void FIFileX::OnSaveBool(FISaveContext* sc, bool val){
	sc->file << (val ? "TRUE" : "FALSE");
}
void FIFileX::OnSaveInt(FISaveContext* sc, int val){
	sc->file << val;
}
///	real値の保存
void FIFileX::OnSaveReal(FISaveContext* sc, double val){
	sc->file << val;
}
///	string値の保存
void FIFileX::OnSaveString(FISaveContext* sc, UTString val){
	sc->file << '"' << val << '"' << std::endl;
}


};

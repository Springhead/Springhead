/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <FileIO/FIFileX.h>
#include <FileIO/FILoadContext.h>
#include <FileIO/FISaveContext.h>

namespace Spr{;

//#define TRACE_PARSE
#ifdef TRACE_PARSE
# define PDEBUG(x)	x
#else 
# define PDEBUG(x)
#endif

namespace FileX{
static UTStack<FILoadContext*> fileContexts;
static UTStack<FIFileX*> fileXs;
static FILoadContext* fileContext;
static FIFileX* fileX;

///	テンプレートのTypeDescがあるかどうか．
static bool TypeAvail(){
	return fileContext->fieldIts.size() && fileContext->fieldIts.back().type;
}

///	ノードの始まり．型を見つけてセット
static void NodeStart(const char* b, const char* e){
	UTString tn(b,e);
	PDEBUG( DSTR << "NodeStart " << tn << std::endl );
	fileContext->NodeStart(tn);
}
///	ノードの名前の設定
static void NameSet(const char* b, const char* e){
	UTString n(b,e);
	fileContext->datas.back()->SetName(UTString(b,e));
}

///	ノードの終わり
static void NodeEnd(const char* b, const char* e){
	PDEBUG(DSTR << "NodeEnd " << fileContext->fieldIts.back().type->GetTypeName() << std::endl);
	fileContext->NodeEnd();
}

///	ブロック型の読み出し準備
static void BlockStart(const char* b, const char* e){
	PDEBUG(DSTR << "blockStart" << std::endl);
	fileContext->CompositStart();
}

///	ブロック型の終了
static void BlockEnd(const char* b, const char* e){
	PDEBUG(DSTR << "blockEnd" << std::endl);
	fileContext->CompositEnd();
}
///	ブロック型のスキップ
static void NodeSkip(const char* b, const char* e){
	fileContext->datas.Top()->str = UTString(b,e);
}

/**	ブロック読み出し中，フィールドを読む前に呼ばれる．
	TypeDescを見て次に読み出すべきフィールドをセットする．
	読み出すべきフィールドがある間 true を返す．	*/
static bool NextField(){
	char* base = (char*)fileContext->datas.Top()->data;
	bool rv = fileContext->fieldIts.NextField(base);
	PDEBUG(
		if (rv){
			DSTR << "NextField:";
			fileContext->fieldIts.Top().field->Print(DSTR);
			DSTR << std::endl;
		}else {
			DSTR << "NextField failed." << std::endl;
		}
	)
	return rv;
}
///	配列のカウント．まだ読み出すべきデータが残っていれば true を返す．
static bool ArrayCount(){
	return fileContext->fieldIts.IncArrayPos();
}

static bool IsFieldInt(){ return fileContext->fieldIts.back().fieldType==UTTypeDescFieldIt::F_INT; }
static bool IsFieldReal(){ return fileContext->fieldIts.back().fieldType==UTTypeDescFieldIt::F_REAL; }
static bool IsFieldStr(){ return fileContext->fieldIts.back().fieldType==UTTypeDescFieldIt::F_STR; }
static bool IsFieldBlock(){ return fileContext->fieldIts.back().fieldType==UTTypeDescFieldIt::F_BLOCK; }
static bool IsFieldBool(){ return fileContext->fieldIts.back().fieldType==UTTypeDescFieldIt::F_BOOL; }

static double numValue;
static std::string strValue;
static bool boolValue;
static void NumSet(double v){
	numValue = v;
}
static void EnumSet(const char* b, const char* e){
	UTTypeDescFieldIt& curField = fileContext->fieldIts.back();
	for(unsigned i=0; i< curField.field->enums.size(); ++i){
		if (curField.field->enums[i].first.compare(UTString(b, e)) == 0){
			numValue = curField.field->enums[i].second;
			return;
		}
	}
	UTString str = "enum (";
	for(unsigned i=0; i< curField.field->enums.size(); ++i){
		str += curField.field->enums[i].first;
		str += " ";
	}
	str += ") expected.";
	fileContext->ErrorMessage(NULL, b, str.c_str());
	numValue = -1;
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

	UTTypeDescFieldIt& curField = fileContext->fieldIts.back();
	//	debug 出力
#ifdef TRACE_PARSE
	if (curField.fieldType!=UTTypeDescFieldIt::F_NONE){
		if (curField.fieldType==UTTypeDescFieldIt::F_BLOCK){
			DSTR << " => (" << curField.field->typeName << ") " << curField.field->name << std::endl;
		}else{
			if (curField.arrayPos==0){
				DSTR << "(" << curField.field->typeName << ") " << curField.field->name << " = " ;
			}
		}
		if (curField.fieldType == UTTypeDescFieldIt::F_REAL || curField.fieldType == UTTypeDescFieldIt::F_INT){
			DSTR << " " << numValue;
		}else if (curField.fieldType == UTTypeDescFieldIt::F_STR){
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
		curField.arrayPos=UTTypeDesc::BIGVALUE;
	}
}
static void StopArray(const char c){
	UTTypeDescFieldIt& curField = fileContext->fieldIts.back();
	curField.arrayPos=UTTypeDesc::BIGVALUE;
}
static void StopArrayStr(const char* b, const char* e){
	StopArray(' ');
}

///	参照型を書き込む．(未完成)
static void RefSet(const char* b, const char* e){
	//DSTR << "ref(" << std::string(b,e) << ") not yet implemented." << std::endl;
	std::string ref(b,e);
	fileContext->AddDataLink(ref, b);
}

static UTTypeDesc* tdesc;
//	XFileのtemplateの読み出しの関数
static void TempStart(const char* b, const char* e){
	tdesc = DBG_NEW UTTypeDesc(std::string(b,e));
}
static void DefType(const char* b, const char* e){
	tdesc->GetComposit().push_back(UTTypeDesc::Field());
	tdesc->GetComposit().back().typeName.assign(b, e);
}
static void DefId(const char* b, const char* e){
	tdesc->GetComposit().back().name.assign(b, e);
}
static void ArrayId(const char* b, const char* e){
	tdesc->GetComposit().back().varType = UTTypeDescIf::VECTOR;
	tdesc->GetComposit().back().lengthFieldName.assign(b, e);
}
static void ArrayNum(int n){
	tdesc->GetComposit().back().varType = UTTypeDescIf::ARRAY;
	tdesc->GetComposit().back().length = n;
}
static void TempEnd(char c){
	tdesc->Link(fileContext->typeDbs.Top());
	PDEBUG(DSTR << "load template:" << std::endl);
	PDEBUG(tdesc->Print(DSTR));
	fileContext->typeDbs.Top()->RegisterDesc(tdesc);
}

///	XXX expected. のエラーメッセージを出すパーサ
class ExpectParser {
	std::string msg; 
public:
    ExpectParser(const char *m) : msg(m) {}
	static std::ostream* errorStr;

	typedef boost::spirit::classic::nil_t result_t;
    
    template <typename ScannerT>
	int operator()(ScannerT const& scan, result_t& /*result*/) const {
		if (!scan.at_end()){
			std::string str = msg + std::string(" expected");
			fileContext->ErrorMessage(NULL, scan.first, str.c_str());
		}
		return -1;
    }
}; 
typedef boost::spirit::classic::functor_parser<ExpectParser> ExpP;
}
using namespace FileX;

FIFileX::FIFileX(const FIFileXDesc& desc){
	Init();
}
void FIFileX::Init(){
	using namespace std;
	using namespace boost::spirit::classic;
	using namespace Spr;
	//	パーサの定義
	//	本文用パーサ
	start		= (str_p("xof 0302txt 0064") | str_p("xof 0303txt 0032") | 
					str_p("xof 0303txt 0064") | ExpP("'xof 0303txt 0032'")) 
					>> *(temp | data | ExpP("template or data"));

	temp		= str_p("template") >> id[&TempStart] >> ch_p('{') >> !uuid
					>> *define >> ch_p('}')[&TempEnd];
	uuid		= ch_p('<') >> *~ch_p('>') >> ch_p('>');
	define		= defArray | defNormal | defOpen | defRestrict;
	defNormal	= id[&DefType] >> id[&DefId] >> ";";
	defArray	= str_p("array") >> id[&DefType] >> id[&DefId] 
					>> '[' >> arraySuffix >> ']' >> ';';
	defOpen		= str_p("[...]");
	//defRestrict	= ch_p('[') >> *(id >> ',') >> id >> ']';
	defRestrict	= ch_p('[') >> *(id >> ',') >> id >> !uuid >> ']';
	arraySuffix	= id[&ArrayId] | int_p[&ArrayNum] | ExpP("id or int value");

	data		= id[&NodeStart] >> !id[&NameSet] >> (ch_p('{') | ExpP("'{'")) >>
				  if_p(&TypeAvail)[ block >> !ch_p(';') >> *(data|ref) ].
				  else_p[ (!dataSkip)[&NodeSkip] ]	//<	知らない型名の場合スキップ
				  >> (ch_p('}') | ExpP("'}'"))[&NodeEnd];
	dataSkip	= *(~ch_p('{') & ~ch_p('}')) | blockSkip;
	blockSkip	= ch_p('{') >> *(blockSkip|~ch_p('}')) >> ch_p('}');
	ref			= ch_p('{') >> (id[&RefSet] | ExpP("id")) >> (ch_p('}')|ExpP("'}'"));
	block		= while_p(&NextField)[
					while_p(&ArrayCount)[
						ch_p(';')[&StopArray] | exp >> (
							(ch_p(',')|!ch_p(';'))[&SetVal] | ExpP("',' or ';'")[&StopArrayStr]  )
					]
				  ];
	exp			= if_p(&IsFieldBool)[ boolVal | ExpP("bool value") ] >>
				  if_p(&IsFieldInt)[ iNum | ExpP("int value") ] >>
				  if_p(&IsFieldReal)[ rNum | ExpP("numeric value") ] >>
				  if_p(&IsFieldStr)[ str | ExpP("string") ] >>
				  if_p(&IsFieldBlock)[ eps_p[&BlockStart] >> block[&BlockEnd] ];
	id			= lexeme_d[ (alpha_p|'_') >> *(alnum_p|'_'|'-'|'.') ];
	boolVal		= (str_p("true") | "TRUE" | "false" | "FALSE" | "1" | "0")[&BoolSet];
	iNum		= id[&EnumSet] | int_p[&NumSet];
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
void FIFileX::PushLoaderContext(FILoadContext* fc){
	fc->RegisterGroupToDb("Foundation Physics Graphics FileIO Framework Creature OldSpringhead");
	fc->typeDbs.Top()->RegisterAlias("Vec3f", "Vector");
	fc->typeDbs.Top()->RegisterAlias("Vec2f", "Coords2d");
	fc->typeDbs.Top()->RegisterAlias("Affinef", "Matrix3x3");
	fc->typeDbs.Top()->RegisterAlias("Affined", "Matrix4x4");

	fileContexts.Push(fc);
	fileXs.Push(this);
	fileContext = fileContexts.Top();
	fileX = fileXs.Top();
}
void FIFileX::PopLoaderContext(){
	fileContexts.Pop();
	fileXs.Pop();
	if (fileContexts.size()){
		fileContext = fileContexts.Top();
		fileX = fileXs.Top();
	}else{
		fileContext = NULL;
		fileX = NULL;
	}
}
void FIFileX::LoadImp(FILoadContext* fc){
	using namespace std;
	using namespace boost::spirit::classic;
	using namespace Spr;
	PushLoaderContext(fc);
	parse_info<const char*> info = parse(
		fileContext->fileMaps.Top()->start, 
		fileContext->fileMaps.Top()->end, start, cmt);
	PopLoaderContext();
}

//----------------------------------------------------
//	セーブ時のハンドラ
#define INDENT(x)	UTPadding(((int)sc->objects.size()+x)*2)
//<< (sc->objects.size()+x)
void FIFileX::OnSaveFileStart(FISaveContext* sc){
	sc->RegisterGroupToDb("Foundation Physics Graphics FileIO Framework Creature OldSpringhead");
	sc->typeDbs.Top()->RegisterAlias("Vec3f", "Vector");
	sc->typeDbs.Top()->RegisterAlias("Vec2f", "Coords2d");
	sc->typeDbs.Top()->RegisterAlias("Affinef", "Matrix3x3");
	sc->typeDbs.Top()->RegisterAlias("Affined", "Matrix4x4");

	sc->Stream() << "xof 0302txt 0064" << std::endl;
}
static bool cont;
void FIFileX::OnSaveNodeStart(FISaveContext* sc){
	sc->Stream() << INDENT(-1) << sc->GetNodeTypeName();
	UTString name = sc->GetNodeName();
	if (name.length()) sc->Stream() << " " << name;
	sc->Stream() << "{" << std::endl;
	cont = false;
}
void FIFileX::OnSaveNodeEnd(FISaveContext* sc){
	sc->Stream() << INDENT(-1) << "}" << std::endl;
}
void FIFileX::OnSaveDataEnd(FISaveContext* sc){
	if (cont) sc->Stream() << std::endl;
}

bool FIFileX::OnSaveFieldStart(FISaveContext* sc, int nElements){
	if (!cont){
		sc->Stream() << INDENT(0);
		cont = true;
	}
	return true;
}
void FIFileX::OnSaveFieldEnd(FISaveContext* sc, int nElements){
	if (!cont) sc->Stream() << INDENT(0);
	sc->Stream() << ";";
	cont = true;
	if (sc->fieldIts.Top().fieldType == UTTypeDescFieldIt::F_BLOCK){
		sc->Stream() << std::endl;
		cont = false;
	}
}
void FIFileX::OnSaveElementEnd(FISaveContext* sc, int nElements, bool last){
	if (!last) sc->Stream() << ",";
}
void FIFileX::OnSaveBool(FISaveContext* sc, bool val){
	sc->Stream() << (val ? "TRUE" : "FALSE");
}
void FIFileX::OnSaveInt(FISaveContext* sc, int val){
	sc->Stream() << val;
}
///	real値の保存
void FIFileX::OnSaveReal(FISaveContext* sc, double val){
	sc->Stream() << val;
}
///	string値の保存
void FIFileX::OnSaveString(FISaveContext* sc, UTString val){
	sc->Stream() << '"' << val << '"' << std::endl;
}
void FIFileX::OnSaveRef(FISaveContext* sc){
	NamedObjectIf* n = DCAST(NamedObjectIf, sc->objects.Top());
	sc->Stream() << INDENT(-1) << "{" << n->GetName() << "}" << std::endl;
}


};

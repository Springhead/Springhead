/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FIFileCollada.h"
#include "FIFileColladaHandler.h"
#include <Foundation/UTLoadHandler.h>
#include <fstream>
#include <sstream>
#include <Springhead.h>
#include <boost/bind.hpp>

namespace Spr{;

IF_OBJECT_IMP(FIFileCOLLADA, FIFile);


//#define TRACE_PARSE
#ifdef TRACE_PARSE
# define PDEBUG(x)	x
#else 
# define PDEBUG(x)
#endif

namespace FileCOLLADA{
static UTStack<FILoadContext*> fileContexts;
static UTStack<FIFileCOLLADA*> fileCOLLADAs;
static FILoadContext* fileContext;
static FIFileCOLLADA* fileCOLLADA;

///	テンプレートのTypeDescがあるかどうか．
static bool TypeAvail(){
	return fileContext->fieldIts.size() && fileContext->fieldIts.back().type;
}

///	ノードデータの読み出し準備
static void NodeStart(const char* b, const char* e){
	std::string tn(b,e);
	PDEBUG( DSTR << "NodeStart " << tn << std::endl );
	fileContext->PushType(tn);	//	これからロードする型としてPush
}
///	ノードの名前の設定
static void NameSet(const char* b, const char* e){
	fileContext->datas.back()->SetName(UTString(b,e).c_str());
}

///	ノード読み出しの後処理
static void NodeEnd(const char* b, const char* e){
	PDEBUG(DSTR << "NodeEnd " << fileContext->fieldIts.back().type->GetTypeName() << std::endl);
	fileContext->PopType();
	std::cout << "これはテストです" << std::endl;
}

///	ブロック型の読み出し準備
static void BlockStart(const char* b, const char* e){
	PDEBUG(DSTR << "blockStart" << std::endl);
	fileCOLLADA->LBlockStart(fileContext);
}

///	ブロック型の終了
static void BlockEnd(const char* b, const char* e){
	PDEBUG(DSTR << "blockEnd" << std::endl);
	fileCOLLADA->LBlockEnd(fileContext);
}

/**	ブロック読み出し中，フィールドを読む前に呼ばれる．
	TypeDescを見て次に読み出すべきフィールドをセットする．
	読み出すべきフィールドがある間 true を返す．	*/
static bool NextField(){
	bool rv = fileContext->fieldIts.NextField();
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

///	参照型を書き込む．(未完成)
static void RefSet(const char* b, const char* e){
	//DSTR << "ref(" << std::string(b,e) << ") not yet implemented." << std::endl;
	std::string ref(b,e);
	fileContext->AddDataLink(ref, b);
}

//--テスト--------------------------------------------------------
std::ofstream os("test.txt");
static void test(const std::string& str, const char* start, const char* end) {
	std::cout << str << ": ";
	os << str << ": ";
	std::string buf(start, end);
	while(start < end) {
		std::cout << *start;
		os << *start;
		++start;
	}
	std::cout << std::endl;
	os << '\n';
	os.flush();
}

//--Element読み込み開始-----------------------------------------------
static void elementStart(const char* start, const char* end)
{
	std::string tagName(start, end);
	std::cout << "elementStart :" << tagName << std::endl;
	os << "elementStart :" << tagName << std::endl;
}

static void elementEnd(const char* start, const char* end)
{
	std::string tagName(start, end);
	std::cout << "elementEnd :" << tagName << std::endl;
	os << "elementEnd :" << tagName << std::endl;
}

}
using namespace FileCOLLADA;


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
			fileContext->ErrorMessage(NULL, scan.first, str.c_str());
		}
		return -1;
    }
}; 

typedef boost::spirit::functor_parser<ExpectParser> ExpP;


FIFileCOLLADA::FIFileCOLLADA(){
	Init();
}
void FIFileCOLLADA::Init(){

	using namespace std;
	using namespace boost::spirit;
	using namespace Spr;
	//	パーサの定義
	//	本文用パーサ
	start	= head >> body;
	head	= str_p("<?") >> "xml" >> "version" >> "=" >> "\"1.0\"" >> "encoding" >> "=" >> "\"utf-8\"" >> "?>";
	body	= element;
	element	= emptyTag | startTag >> contents >> endTag;
	startTag= ch_p('<') >> id[&elementStart] >> *property >> '>';
	endTag	= str_p("</") >> id[&elementEnd] >> '>';
	emptyTag= ch_p('<') >> id >> *property >> "/>";
	property= id >> '=' >> (id | string);
	contents= *(~ch_p('<') | element);

	// xml の名前
	id			= lexeme_d[ (alpha_p|'_') >> *(alnum_p|'_'|'-') ];
	string		= lexeme_d[ ch_p('"') >> *~ch_p('"') >> '"' ];

	//	スキップパーサ(スペースとコメントを読み出すパーサ)の定義
	cmt		=	space_p
		|	"<!--" >> *(~ch_p('-') | '-' >> ~ch_p('-') | "--" >> ~ch_p('>')) >> "-->";
}



//------------------------------------------------------------------------------

class FINHC_library_physics_models: public UTLoadHandlerSetDb<library_physics_models>{
public:
	FINHC_library_physics_models():UTLoadHandlerSetDb<Desc>("library_physics_models"){
//	TODO:
//		handlerDb = ;
//		typeDb = ;
	}
};


void FIFileCOLLADA::PushLoaderContext(FILoadContext* fc){

	fileContexts.Push(fc);
	fileCOLLADAs.Push(this);
	fileContext = fileContexts.Top();
	fileCOLLADA = fileCOLLADAs.Top();
}
void FIFileCOLLADA::PopLoaderContext(){
	fileContexts.Pop();
	fileCOLLADAs.Pop();
	if (fileContexts.size()){
		fileContext = fileContexts.Top();
		fileCOLLADA = fileCOLLADAs.Top();
	}else{
		fileContext = NULL;
		fileCOLLADA = NULL;
	}
}
void FIFileCOLLADA::LoadImp(FILoadContext* fc){
	using namespace std;
	using namespace boost::spirit;
	using namespace Spr;
	PushLoaderContext(fc);
	parse_info<const char*> info = parse(
		fileContext->fileMaps.Top()->start, 
		fileContext->fileMaps.Top()->end, start, cmt);
	PopLoaderContext();
}
#define INDENT(x)	UTPadding((sc->objects.size()+x)*2)
//<< (sc->objects.size()+x)
void FIFileCOLLADA::OnSaveFileStart(FISaveContext* sc){
	sc->file << "xof 0302txt 0064" << std::endl;
}
static bool cont;
void FIFileCOLLADA::OnSaveNodeStart(FISaveContext* sc){
	sc->file << INDENT(-1) << sc->GetNodeTypeName();
	UTString name = sc->GetNodeName();
	if (name.length()) sc->file << " " << name;
	sc->file << "{" << std::endl;
	cont = false;
}
void FIFileCOLLADA::OnSaveNodeEnd(FISaveContext* sc){
	sc->file << INDENT(-1) << "}" << std::endl;
}
void FIFileCOLLADA::OnSaveDataEnd(FISaveContext* sc){
	if (cont) sc->file << std::endl;
}

void FIFileCOLLADA::OnSaveFieldStart(FISaveContext* sc, int nElements){
	if (!cont){
		sc->file << INDENT(0);
		cont = true;
	}
}
void FIFileCOLLADA::OnSaveFieldEnd(FISaveContext* sc, int nElements){
	if (!cont) sc->file << INDENT(0);
	sc->file << ";";
	cont = true;
	if (sc->fieldIts.Top().fieldType == UTTypeDescFieldIt::F_BLOCK){
		sc->file << std::endl;
		cont = false;
	}
}
void FIFileCOLLADA::OnSaveElementEnd(FISaveContext* sc, int nElements, bool last){
	if (!last) sc->file << ",";
}
void FIFileCOLLADA::OnSaveBool(FISaveContext* sc, bool val){
	sc->file << (val ? "TRUE" : "FALSE");
}
void FIFileCOLLADA::OnSaveInt(FISaveContext* sc, int val){
	sc->file << val;
}
///	real値の保存
void FIFileCOLLADA::OnSaveReal(FISaveContext* sc, double val){
	sc->file << val;
}
///	string値の保存
void FIFileCOLLADA::OnSaveString(FISaveContext* sc, UTString val){
	sc->file << '"' << val << '"' << std::endl;
}
void FIFileCOLLADA::OnSaveRef(FISaveContext* sc){
	NamedObjectIf* n = DCAST(NamedObjectIf, sc->objects.Top());
	sc->file << INDENT(-1) << "{" << n->GetName() << "}" << std::endl;
}


};

// VrmlParser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <FileIO/FIFileVrml.h>
#include <FileIO/FILoadContext.h>
#include <FileIO/FISaveContext.h>
#include <boost/spirit/include/classic_regex.hpp>

namespace Spr{;
using namespace boost::spirit::classic;

static UTStack<FILoadContext*> fileContexts;
static UTStack<FIFileVRML*> fileVRMLs;
static FILoadContext* fileContext;
static FIFileVRML* fileVRML;
static std::list<std::string> strs;
struct TFieldInfo{
    std::string type;
    std::string id;
};
struct TProtoInfo{
	std::vector<TFieldInfo> fieldInfos;
	std::string protoId;
};
static UTStack<TProtoInfo> protoInfoStack;
static std::list<FIPhraseParser> nodeBodyElementRules;
static std::list<FIPhraseParser> nodeBodyRules;

static void OnDecParser(){
	DSTR << "OnDecParser()" << std::endl;
}

static void OnBeginProto(const char* first, const char* last){
	DSTR << "beginProto " << std::string(first, last) << std::endl;
	protoInfoStack.Push(TProtoInfo());
	protoInfoStack.Top().protoId= std::string(first, last);
}
/*static */void OnEndProto(const char* first, const char* last){
	DSTR << "endProto" << std::endl;
	TProtoInfo info= protoInfoStack.Top();
	protoInfoStack.Pop();
	//新しく定義されたノードのパーサを作る
	FIPhraseParser elmParser;
	elmParser=
		fileVRML->routeStatement |
		fileVRML->protoStatement |
	fileVRML->id >> str_p("IS") >> fileVRML->id;
	for (size_t i= 0; i<info.fieldInfos.size(); ++i){
		strs.push_back(info.fieldInfos[i].id);
		elmParser= elmParser | str_p(strs.back().c_str()) 
			>> fileVRML->valueParsers[info.fieldInfos[i].type];
	}
	nodeBodyElementRules.push_back(FIPhraseParser());
	FIPhraseParser& elmRule= nodeBodyElementRules.back();
	nodeBodyRules.push_back(FIPhraseParser());
	FIPhraseParser& bodyRule= nodeBodyRules.back();
	elmRule= elmParser;
	bodyRule= *elmRule;
	strs.push_back(info.protoId);
	fileVRML->nodeParser=
	fileVRML->nodeParser | str_p(strs.back().c_str()) >> ch_p('{') >> bodyRule >> ch_p('}');
	fileVRML->node= fileVRML->nodeParser;
}
static void OnFieldType(const char* first, const char* last){
	DSTR << "fieldType " << std::string(first, last) << std::endl;
	assert(!protoInfoStack.empty());
	protoInfoStack.Top().fieldInfos.push_back(TFieldInfo());
	protoInfoStack.Top().fieldInfos.back().type= std::string(first, last);
}
static void OnFieldId(const char* first, const char* last){
	DSTR << "fieldId " << std::string(first, last) << std::endl;
	assert(!protoInfoStack.empty());
	assert(protoInfoStack.Top().fieldInfos.size()>0);
	protoInfoStack.Top().fieldInfos.back().id= std::string(first, last);
}


//----------------------------------------------------------------------------
//	FIFileVRML

FIFileVRML::FIFileVRML(const FIFileVRMLDesc& desc){
	Init();
}
void FIFileVRML::PushLoaderContext(FILoadContext* fc){
	fc->RegisterGroupToDb("Foundation Physics Graphics FileIO Framework OldSpringhead");
	fc->typeDbs.Top()->RegisterAlias("Vec3f", "Vector");
	fc->typeDbs.Top()->RegisterAlias("Vec2f", "Coords2d");
	fc->typeDbs.Top()->RegisterAlias("Affinef", "Matrix3x3");
	fc->typeDbs.Top()->RegisterAlias("Affined", "Matrix4x4");

	fileContexts.Push(fc);
	fileVRMLs.Push(this);
	fileContext = fileContexts.Top();
	fileVRML = fileVRMLs.Top();
}
void FIFileVRML::PopLoaderContext(){
	fileContexts.Pop();
	fileVRMLs.Pop();
	if (fileContexts.size()){
		fileContext = fileContexts.Top();
		fileVRML = fileVRMLs.Top();
	}else{
		fileContext = NULL;
		fileVRML = NULL;
	}
}
void FIFileVRML::LoadImp(FILoadContext* fc){
	using namespace std;
	using namespace boost::spirit::classic;
	using namespace Spr;
	PushLoaderContext(fc);
	parse_info<const char*> info = parse(
		fileContext->fileMaps.Top()->start, 
		fileContext->fileMaps.Top()->end, vrmlScene, cmt);
	PopLoaderContext();
}

void FIFileVRML::Init(){
	vrmlScene=
		statements;
	statements=
		*statement;
	statement=
		protoStatement |
		routeStatement |
		nodeStatement;
	nodeStatement=
		str_p("DEF") >> id >> node |
		str_p("USE") >> id |
		node;
	protoStatement=
		proto |
		externproto;
	protoStatements=
		*protoStatement;
	proto=
		(str_p("PROTO")
			>> id[&OnBeginProto]
			>> ch_p('[') >> interfaceDeclarations >> ch_p(']')
			>> ch_p('{') >> !protoBody >> ch_p('}'))[&OnEndProto];
		//既定のノードをPROTOで定義するため、本物のVRMLの定義と違って、protoBodyの省略を許している。
	protoBody=
		protoStatements >> nodeStatement >> statements;
	interfaceDeclarations=
		*interfaceDeclaration;
	externproto=
		str_p("EXTERNPROTO") >> id 
			>> ch_p('[') >> !externInterfaceDeclarations >> ch_p(']')
			>> URLList;
	externInterfaceDeclarations=
		*externInterfaceDeclaration;
	externInterfaceDeclaration=
		str_p("eventIn") >> fieldType >> id |
		str_p("eventOut") >> fieldType >> id |
		str_p("field") >> fieldType >> id |
		str_p("exposedField") >> fieldType >> id;
	routeStatement=
		str_p("ROUTE") >> id >> ch_p('.') >> id >> str_p("TO")
			>> id >> ch_p('.') >> id;
	URLList=
		mfstringValue;
	nodeParser=
		str_p("Script") >> ch_p('{') >> scriptBody >> ch_p('}');
	node= nodeParser;
	scriptBody=
		*scriptBodyElement;
	scriptBodyElement=
		str_p("eventIn") >> fieldType >> id >> str_p("IS") >> id |
		str_p("eventOut") >> fieldType >> id >> str_p("IS") >> id |
		str_p("field") >> fieldType >> id >> str_p("IS") >> id |
		restrictedInterfaceDeclaration |
		nodeBodyElement;//ここで使われるのはどこのfieldId？
	nodeBodyElement=
		routeStatement |
		protoStatement |
		id >> str_p("IS") >> id |
		id >> fieldValue;
	id= regex_p("[^\\x30-\\x39\\x00-\\x20\\x22\\x23\\x27\\x2b\\x2c\\x2d\\x2e\\x5b"
		"\\x5c\\x5d\\x7b\\x7d\\x7f]"
		"[^\\x00-\\x20\\x22\\x23\\x27\\x2c\\x2e\\x5b\\x5c\\x5d\\x7b\\x7d\\x7f]*");
	fieldType=
		str_p("MFColor") | str_p("MFFloat") | str_p("MFInt32") | str_p("MFNode") | 
		str_p("MFRotation") | str_p("MFString") | str_p("MFTime") | str_p("MFVec2f") | 
		str_p("MFVec3f") |
		str_p("SFBool") | str_p("SFColor") | str_p("SFFloat") | str_p("SFImage") | 
		str_p("SFInt32") | str_p("SFNode") | str_p("SFRotation") | str_p("SFString") | 
		str_p("SFTime") | str_p("SFVec2f") | str_p("SFVec3f");
	fieldValue=
		sfboolValue | +sffloatValue | sfnodeValue | sfstringValue | 
		ch_p('[') >> *(sfboolValue | +sffloatValue | sfnodeValue | sfstringValue) >> ch_p(']');
	sfboolValue=
		str_p("TRUE") |
		str_p("FALSE");
	sfcolorValue=
		sffloatValue >> sffloatValue >> sffloatValue;
	sffloatValue=
		regex_p("([+\\-]?((([0-9]+(\\.)?)|([0-9]*\\.[0-9]+))([eE][+\\-]?[0-9]+)?))");
	sfimageValue=
		+sfint32Value;
	sfint32Value=
		regex_p("([+\\-]?(([0-9]+)|(0[xX][0-9a-fA-F]+)))");
	sfnodeValue= str_p("NULL") | nodeStatement;
	sfrotationValue=
		sffloatValue >> sffloatValue >> sffloatValue >> sffloatValue;
	sfstringValue=
		regex_p("\"([^\\\\\"]|\\\\.)*\"");
	sftimeValue=
		sffloatValue;
	mftimeValue=
		sftimeValue |
		ch_p('[') >> *sftimeValue >> ch_p(']');
	sfvec2fValue=
		sffloatValue >> sffloatValue;
	sfvec3fValue=
		sffloatValue >> sffloatValue >> sffloatValue;
	mfcolorValue=
		sfcolorValue |
		ch_p('[') >> *sfcolorValue >> ch_p(']');
	mffloatValue=
		sffloatValue |
		ch_p('[') >> *sffloatValue >> ch_p(']');
	mfint32Value=
		sfint32Value |
		ch_p('[') >> *sfint32Value >> ch_p(']');
	mfnodeValue= ch_p('[') >> *nodeStatement >> ch_p(']')
		| nodeStatement;
	mfrotationValue=
		sfrotationValue |
		ch_p('[') >> *sfrotationValue >> ch_p(']');
	mfstringValue=
		sfstringValue |
		ch_p('[') >> *sfstringValue >> ch_p(']');
	mfvec2fValue=
		sfvec2fValue |
		ch_p('[') >> *sfvec2fValue >> ch_p(']');
	mfvec3fValue=
		sfvec3fValue |
		ch_p('[') >> *sfvec3fValue >> ch_p(']');

	valueParsers["MFColor"]= mfcolorValue;
	valueParsers["MFFloat"]= mffloatValue;
	valueParsers["MFInt32"]= mfint32Value;
	valueParsers["MFNode"]= mfnodeValue;
	valueParsers["MFRotation"]= mfrotationValue;
	valueParsers["MFString"]= mfstringValue;
	valueParsers["MFTime"]= mftimeValue;
	valueParsers["MFVec2f"]= mfvec2fValue;
	valueParsers["MFVec3f"]= mfvec3fValue;
	valueParsers["SFBool"]= sfboolValue;
	valueParsers["SFColor"]= sfcolorValue;
	valueParsers["SFFloat"]= sffloatValue;
	valueParsers["SFImage"]= sfimageValue;
	valueParsers["SFInt32"]= sfint32Value;
	valueParsers["SFNode"]= sfnodeValue;
	valueParsers["SFRotation"]= sfrotationValue;
	valueParsers["SFString"]= sfstringValue;
	valueParsers["SFTime"]= sftimeValue;
	valueParsers["SFVec2f"]= sfvec2fValue;
	valueParsers["SFVec3f"]= sfvec3fValue;

	//	decParser (:= fieldType fieldId fieldValue)の定義
	for (std::map<std::string, FIPhraseParser>::iterator it= valueParsers.begin(); 
			it!=valueParsers.end(); ++it){
		strs.push_back(it->first);
		decParser = str_p(strs.back().c_str())[&OnFieldType]
				>> id[&OnFieldId] >> (it->second) | decParser;
	}
	
	restrictedInterfaceDeclaration=
		str_p("eventIn") >> fieldType >> id |
		str_p("eventOut") >> fieldType >> id |
		str_p("field") >> decParser;
	interfaceDeclaration=
		restrictedInterfaceDeclaration |
		str_p("exposedField") >> decParser;

	cmt	= space_p
		|	"/*" >> *(~ch_p('*') | '*'>>~ch_p('/')) >> !ch_p('*') >> '/'
		|	"//" >> *~ch_p('\n') >> '\n'
		|	"#" >> *~ch_p('\n') >> '\n';
}


}	//	namespace Spr;

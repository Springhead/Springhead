/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * Springhead.cxx
 *
 * Interface stubs and type descriptions generator for Springhead2
 * ----------------------------------------------------------------------------- */

#include "swigmod.h"
#include <string>
#include <vector>
#include <set>
using namespace std;
typedef std::vector<string> Strings;
typedef std::vector<Node*> Nodes;

static const char *usage = "\
Springhead Options (available with -spr)\n";

std::string TrimSuffix(DOHString* s, char* suffix){
	int len = DohLen(s);
	char buf[1024];
	int sufLen = (int)strlen(suffix);
	DohSeek(s, sufLen, SEEK_END);
	DohRead(s, buf, 1024);
	buf[sufLen] = 0;
	if (strcmp(buf, suffix)==0){
		DohSeek(s, 0, SEEK_SET);
		DohRead(s, buf, len-sufLen);
		buf[len-sufLen] = 0;
		return std::string(buf);
	}
	return std::string();
}

std::string Trim(char* pre, DOHString* s, char* suf){
	int len = DohLen(s);
	int sufLen = (int)strlen(suf);
	int preLen = (int)strlen(pre);
	
	char bufSuf[1024];
	DohSeek(s, sufLen, SEEK_END);
	DohRead(s, bufSuf, 1024);
	bufSuf[sufLen] = 0;
	char bufPre[1024];
	DohSeek(s, 0, SEEK_SET);
	DohRead(s, bufPre, preLen);
	bufPre[preLen] = 0;

	if (strcmp(bufSuf, suf)==0 && strcmp(bufPre, pre)==0){
		char buf[1024];
		DohSeek(s, preLen, SEEK_SET);
		DohRead(s, buf, len-sufLen-preLen);
		buf[len-sufLen-preLen] = 0;
		return std::string(buf);
	}
	return std::string();
}
std::string GetString(DOHString* s){
	if (!s) return std::string();
	char buf[4096];
	DohSeek(s, 0, SEEK_SET);
	int l = DohRead(s, buf, 4095);
	buf[l] = 0;
	return std::string(buf);
}
#ifdef	_WIN32
std::string DecodeType(DOHString* n, std::string& post = string()){
#else
std::string DecodeType(DOHString* n, std::string& post){
#endif
	DOH* t = NewString( Getattr(n, "type") );
	Replaceall(t, "<(", "< ");
	Replaceall(t, ")>", " >");
	string type = GetString(t);
	bool refOrPtr = false;
	while(1){
		if (type.compare(0,2,"p.")==0){
			type = type.substr(2);
			type.append("*");
			refOrPtr = true;
		}else if (type.compare(0,2,"r.")==0){
			type = type.substr(2);
			type.append("&");					
			refOrPtr = true;
		}else{
			break;
		}
	}
	string decl = GetString(Getattr(n, "decl"));
	while (1){
		if (decl.length()>=2 && decl.compare(decl.length()-2, 2, "p.")==0){
			type.append("*");
			decl = decl.substr(0, decl.length()-2);
		}else if (decl.length()>=2 && decl.compare(decl.length()-2, 2, "r.")==0){
			type.append("&");
			decl = decl.substr(0, decl.length()-2);
		}else{
			break;
		}
	}
	if (decl.compare(0, 9, "q(const).")==0){
		post = "const";
	}
	if (type.compare(0,9,"q(const).")==0){
		type = type.substr(9);
//		if (refOrPtr) 
			type = string("const ").append(type);
//		else post = "const";
	}
	return type;
}


void GetBaseList(Strings& rv, DOH* baseList, char* suffix){
	if (baseList){
		for(int i=0; i<DohLen(baseList); ++i){
			DOHString* b = DohGetitem(baseList, i);
			Replaceall(b, "<(", "< ");
			Replaceall(b, ")>", " >");
			std::string bb = TrimSuffix(b, suffix);
			if (bb.length()){
				rv.push_back(bb);
			}
		}
	}
}

struct Field{
	string type;
	string name;
};
typedef std::vector<Field> Fields;

//	enum field
struct EnumItem{
	string name;
	string value;
};
struct Enum{
	string name;
	string fullname;
	std::vector<EnumItem> fields;
};
bool operator < (const Enum& a, const Enum& b){
	return a.fullname.compare(b.fullname) < 0;
}

class Springhead:public Language {
	int indent_level;
	File *cpp, *hpp, *sprh, *log;
	bool errorFlag;
public:
	Springhead():indent_level(0), cpp(NULL), log(NULL), errorFlag(false){
	}
	virtual ~ Springhead(){
	}

	void DescImp(DOHFile* file, Node* n){
		bool found = false;
		Nodes cons;
		FindNode(cons, n, "constructor");
		for(unsigned i=0; i<cons.size(); ++i){
			if (Cmp(Getattr(cons[i], "name"), "SPR_DESCDEF")==0){
				found = true;
				break;
			}
		}
		if (!found) return;
		
		DOHString* name = Getattr(n, "sym:name");
		Strings bases;
		GetBaseList(bases, Getattr(n, "baselist"), "Desc");
		//	DESCIMP
		Printf(file, "SPR_DESCIMP%d(%s", bases.size(), name);
 		for(unsigned i=0; i<bases.size(); ++i){
			Printf(file, ", %s", bases[i].c_str());
		}
		Printf(file, ");\n");
	}
	//	構造体ごとの処理(TypeDescの作成など)
	void TypeDesc(DOHFile* cpp, DOHFile* hpp, Node* n){		
		DOHString* fullname = Getattr(n, "name");
		DOHString* name = Getattr(n, "sym:name");
		enum ClsType{
			NORMAL,
			STATE,
			DESC,
		} clsType = NORMAL;
		string cls = TrimSuffix(name, "Desc");
		if (cls.length()){
			clsType = DESC;
		}else{
			cls = TrimSuffix(name, "State");
			if (cls.length()) clsType = STATE;
		}
		
		//	TypeDesc
		Printf(cpp, "	%s* p%s = NULL;\n", fullname, name);
		Printf(cpp, "	desc = UTTypeDescIf::Create(\"%s\");\n", name);
		Printf(cpp, "	desc->SetSize(sizeof(%s));\n", fullname);
		if (cls.length()){
			Printf(cpp, "	desc->SetIfInfo(%sIf::GetIfInfoStatic());\n", cls.c_str());
			if (clsType == DESC){
				Printf(cpp, "	((IfInfo*)%sIf::GetIfInfoStatic())->SetDesc(desc);\n", cls.c_str());
			}else if (clsType == STATE){
				Printf(cpp, "	((IfInfo*)%sIf::GetIfInfoStatic())->SetState(desc);\n", cls.c_str());
			}
		}
		Printf(cpp, "	desc->SetAccess(DBG_NEW UTAccess<%s>);\n", fullname);
		//	baselist
		Strings bases;
		GetBaseList(bases, Getattr(n, "baselist"), "");
		for(unsigned i = 0; i < bases.size(); ++i){
			Printf(cpp, "	field = desc->AddBase(\"%s\");\n", bases[i].c_str());
			Printf(cpp, "	desc->SetOffset(field, int((char*)(%s*)p%s - (char*)p%s));\n", bases[i].c_str(), name, name);
		}

		std::set<Enum> enums;
		Nodes enumNodes;
		FindNode(enumNodes, n, "enum");
		for(unsigned i=0; i<enumNodes.size(); ++i){
			Node* e = firstChild(enumNodes[i]);
			Enum en;
			en.name = GetString(Getattr(enumNodes[i], "sym:name"));
			en.fullname = GetString(Getattr(enumNodes[i], "name"));
			while(e){
				EnumItem ei;
				ei.name = GetString(Getattr(e, "name"));
				ei.value = GetString(Getattr(e, "value"));
				en.fields.push_back(ei);
				e = nextSibling(e);
			}
			enums.insert(en);
		}
		
		//	member variable
		Nodes memberVars;
		FindNode(memberVars, n, "cdecl");
		Fields normalVars;
		for(unsigned i = 0; i < memberVars.size(); ++i){
			Node* m = memberVars[i];
			if (Cmp(Getattr(m, "kind"), "variable") != 0) continue;			//変数でなければパス
			if (Cmp(Getattr(m, "storage"), "static") == 0) continue;		//static 変数はパス
			if (Cmp(Getattr(m, "feature:immutable"), "1") == 0) continue;	//const 変数はパス
			normalVars.push_back(Field());
			Node* memName = Getattr(m, "sym:name");
			normalVars.back().name = GetString(memName);
			string typeSuffix = Trim("a(", Getattr(m, "decl"), ").");
			int pos = typeSuffix.find_last_of(':');
			if (pos != std::string::npos) typeSuffix = typeSuffix.substr(pos+1);
			Node* type = Getattr(m, "type");
			normalVars.back().type = GetString(type);
			string typeBase;
			string typePrefix;
			typeBase = Trim("std::vector<(UTRef<(", type, ")>)>");
			if (!typeBase.length()) typeBase = Trim("vector<(UTRef<(", type, ")>)>");
			if (typeBase.length()){
				typePrefix = "vectorUTRef";
			}else{
				typeBase = Trim("std::vector<(", type, ")>");
				if (!typeBase.length()) typeBase = Trim("vector<(", type, ")>");
				if (typeBase.length()){
					typePrefix = "vector";
				}else{
					typeBase = Trim("UTRef<(", type, ")>");
					if (!typeBase.length()) typeBase = Trim("UTRef<(", type, ")>");
					if (typeBase.length()){
						typePrefix = "UTRef";
					}else{
						typeBase = GetString(Getattr(m, "type"));				
						if (Cmp(Getattr(m, "decl"), "p.")==0){
							typePrefix = "pointer";
							normalVars.back().type.append("*");
						}
					}
				}
			}
			if (typeBase.compare(0, 5, "enum ")==0){	//	enum型
				string enumFullname = typeBase.substr(5);
				size_t f = enumFullname.rfind("::");
				string enumName = enumFullname;
				if (f!=string::npos){
					enumName = enumName.substr(f+2);
				}
				Printf(cpp, "	field = desc->AddField(\"%s\", \"%s\", \"%s\", \"%s\");\n", 
					enumName.c_str(), "enum", memName, typeSuffix.c_str());
				Enum en;
				en.fullname = enumFullname;
				std::set<Enum>::iterator it = enums.find(en);
				if (it != enums.end()){
					for(unsigned i=0; i<it->fields.size(); ++i){
						Printf(cpp, "	desc->AddEnumConst(field, \"%s\", %s);\n", 
							it->fields[i].name.c_str(), it->fields[i].value.c_str());
					}
				}
			}else{
				size_t f = typeBase.rfind("::");
				string typeBaseBase = typeBase;
				if (f!=string::npos){
					typeBaseBase = typeBase.substr(f+2);
				}
				if (typeBaseBase.compare("unsigned int")==0) typeBaseBase = "unsigned";
				Printf(cpp, "	field = desc->AddField(\"%s\", \"%s\", \"%s\", \"%s\");\n", 
					typePrefix.c_str(), typeBaseBase.c_str(), memName, typeSuffix.c_str());
			}
			Printf(cpp, "	desc->SetOffset(field, int((char*)&(p%s->%s) - (char*)p%s));\n",
				name, memName, name);
		}
		Printf(cpp, "	db->RegisterDesc(desc);\n");

		if (clsType == DESC || clsType == STATE){
			//	Desc Stateをクラスメンバとして宣言し，GetDesc() SetDesc()を宣言するマクロの宣言
			Printf(hpp, "#define SPR_DECLMEMBEROF_%s \\\n", name);
			Printf(hpp, "protected:\\\n");
			for (unsigned i=0; i<normalVars.size(); ++i){
				Field& var = normalVars[i];
				std::string type = var.type;
				for(int i=0; i<(int)type.length(); ++i){
					if (type[i] == '(') type[i] = ' ';
					if (type[i] == ')') type[i] = ' ';
				}
				Printf(hpp, "	%s	%s;	\\\n", type.c_str(), var.name.c_str());
			}
			Printf(hpp, "public:\\\n");

			//	Descの場合，GetDescAddressを無効にする．
			if (clsType == DESC) {
				Printf(hpp, "	virtual const void* GetDescAddress() const { return NULL; }\\\n");
			}
			
			//	SetDesc関数
			const char* suffix = (clsType==STATE) ? "State" : "Desc";
			Printf(hpp, "	virtual void Set%s(const void* ptr){ \\\n", suffix);
			//	基本クラスのSetDesc/SetStateを呼ぶ
			bases.clear();
			GetBaseList(bases, Getattr(n, "baselist"), "Desc");
			for (unsigned i=0; i<bases.size(); ++i)
				Printf(hpp, "		%s::SetDesc((%sDesc*)(%s*)ptr);	\\\n", bases[i].c_str(), bases[i].c_str(), name);
			bases.clear();
			GetBaseList(bases, Getattr(n, "baselist"), "State");
			for (unsigned i=0; i<bases.size(); ++i)
				Printf(hpp, "		%s::SetState((%sState*)(%s*)ptr);	\\\n", bases[i].c_str(), bases[i].c_str(), name);
			//	メンバ変数を設定する
			for (unsigned i=0; i<normalVars.size(); ++i){
				Field& var = normalVars[i];
				Printf(hpp, "		%s = ((%s*)ptr)->%s;	\\\n", var.name.c_str(), name, var.name.c_str());
			}
			if (clsType == DESC){
				Printf(hpp, "		AfterSetDesc();	\\\n");
			}
			Printf(hpp, "	}\\\n");

			//	GetDesc関数
			Printf(hpp, "	virtual bool Get%s(void* ptr) const { \\\n", suffix);
			if (clsType == DESC){
				Printf(hpp, "		BeforeGetDesc();	\\\n");
			}
			//	基本クラスのGetDesc/GetStateを呼ぶ
			bases.clear();
			GetBaseList(bases, Getattr(n, "baselist"), "Desc");
			for (unsigned i=0; i<bases.size(); ++i)
				Printf(hpp, "		%s::GetDesc((%sDesc*)(%s*)ptr);	\\\n", bases[i].c_str(), bases[i].c_str(), name);
			bases.clear();
			GetBaseList(bases, Getattr(n, "baselist"), "State");
			for (unsigned i=0; i<bases.size(); ++i)
				Printf(hpp, "		%s::GetState((%sState*)(%s*)ptr);	\\\n", bases[i].c_str(), bases[i].c_str(), name);
			//	メンバ変数を読み出す
			for (unsigned i=0; i<normalVars.size(); ++i){
				Field& var = normalVars[i];
				Printf(hpp, "		((%s*)ptr)->%s = %s;	\\\n", name, var.name.c_str(), var.name.c_str());
			}
			Printf(hpp, "		return true;	\\\n");
			Printf(hpp, "	}\\\n");
			Printf(hpp, "\n");
		}
	}
	void IfImp(DOHFile* cpp, DOHFile* sprh, Node* n, bool isVintf){
		DOHString* name = Getattr(n, "sym:name");
		string cls = TrimSuffix(name, "If");
		Strings bases;
		GetBaseList(bases, Getattr(n, "baselist"), "If");
		Printf(cpp, "SPR_IFIMP%d(%s", bases.size(), cls.c_str());
 		for(unsigned i=0; i<bases.size(); ++i){
			Printf(cpp, ", %s", bases[i].c_str());
		}
		Printf(cpp, ");\n");
		Printf(sprh, "#define SPR_OVERRIDEMEMBERFUNCOF_%sIf(base)	\\\n", cls.c_str());

		Nodes members;
		FindNode(members, n, "cdecl");
		for(unsigned i = 0; i < members.size(); ++i){
			Node* m = members[i];
			if (Cmp(Getattr(m, "kind"), "function") != 0) continue;		//関数でなければパス
			if (Cmp(Getattr(m, "storage"), "static") == 0) continue;	//static 関数はパス
			if (Cmp(Getattr(m, "storage"), "virtual") == 0){
				Printf(stderr, "Error: %s::%s() is a virtual function.\n", name, Getattr(m,"name"));
				errorFlag = true;
				continue;	//XXIf には，virtual 関数があってはならない．
			}
			if (Getattr(m, "defaultargs")) continue;					//デフォルト引数によるバリエーションは不要
			string post;
			string type = DecodeType(m, post);
			Printf(cpp, "%s %s::%s(", type.c_str(), Getattr(n, "name"), Getattr(m, "name"));
			Printf(sprh, "	%s %s(", type.c_str(), Getattr(m, "name"));
			Node* p = Getattr(m, "parms");
			while(p){
#ifdef	_WIN32
				string type = DecodeType(p);
#else
				string dummy_post;
				string type = DecodeType(p, dummy_post);
#endif
				Printf(cpp, "%s %s", type.c_str(), Getattr(p, "name"));
				Printf(sprh, "%s %s", type.c_str(), Getattr(p, "name"));
				p = nextSibling(p);
				if (p){
					Printf(cpp, ", ");
					Printf(sprh, ", ");
				}
			}
			Printf(cpp, ")%s{\n", post.c_str());
			Printf(sprh, ")%s{", post.c_str());
			if ( !(Cmp(Getattr(m, "type"), "void")==0 && Cmp(Getattr(m, "decl"), "f().")==0) ){
				Printf(cpp, "	return");
				Printf(sprh, "	return");
			}
			if (isVintf) Printf(cpp, "	DCAST(%s, (Object*)(ObjectIf*)this)->%s(", cls.c_str(), Getattr(m, "name"));
			else Printf(cpp, "	((%s*)(Object*)(ObjectIf*)this)->%s(", cls.c_str(), Getattr(m, "name"));
			Printf(sprh, "	base::%s(", Getattr(m, "name"));
			p = Getattr(m, "parms");
			while(p){
				Printf(cpp, "%s", Getattr(p, "name"));
				Printf(sprh, "%s", Getattr(p, "name"));
				p = nextSibling(p);
				if (p){
					Printf(cpp, ", ");
					Printf(sprh, ", ");
				}
			}
			Printf(cpp, ");\n}\n");
			Printf(sprh, ");}	\\\n");
		}
		Printf(sprh, "\n");
	}

	void FindNodeR(Nodes& rv, Node* n, char* type){
		if (Cmp(nodeType(n), type)==0){
			rv.push_back(n);
		}
		n = firstChild(n);
		while(n){
			FindNodeR(rv, n, type);
			n = nextSibling(n);
		}
	}
	void FindNode(Nodes& rv, Node* n, char* type){
		n = firstChild(n);
		while(n){
			if (Cmp(nodeType(n), type)==0){
				rv.push_back(n);
			}
			n = nextSibling(n);
		}
	}

	/* Top of the parse tree */
	virtual int top(Node *top) {
		String *cppfile = NewString(Getattr(top, "outfile"));
		Replaceall(cppfile, "_wrap.cxx", "Stub.cpp");
		String *hppfile = NewString(Getattr(top, "outfile"));
		Replaceall(hppfile, "_wrap.cxx", "Decl.hpp");
		String *sprhfile = NewString(Getattr(top, "outfile"));
		Replaceall(sprhfile, "_wrap.cxx", "Decl.hpp");
		std::string sshf = GetString(sprhfile);
#ifdef	_WIN32
		size_t pos = sshf.rfind('\\');
#else
		size_t pos = sshf.rfind('/');
#endif
		if (pos == std::string::npos) pos = -1;
		std::string fsshf = sshf.substr(0, pos+1);
		fsshf.append("Spr");
		fsshf.append( sshf.substr(pos+1));
		sprhfile = NewString(fsshf.c_str());

		cpp = NewFile(cppfile, "w", NULL);
		if (!cpp) {
			FileErrorDisplay(cppfile);
			SWIG_exit(EXIT_FAILURE);
		}
		hpp = NewFile(hppfile, "w", NULL);
		if (!hpp) {
			FileErrorDisplay(hppfile);
			SWIG_exit(EXIT_FAILURE);
		}
		sprh = NewFile(sprhfile, "w", NULL);
		if (!sprh) {
			FileErrorDisplay(sprhfile);
			SWIG_exit(EXIT_FAILURE);
		}
		Nodes modules;
		FindNodeR(modules, top, "module");
		for(unsigned i=0; i<modules.size(); ++i){
			Node* include = nextSibling(modules[i]);
			while(include){
				if (Cmp(nodeType(include), "include")==0){
					Printf(cpp, "#include \"%s\"\n", Getattr(include, "name"));
				}
				include = nextSibling(include);
			}
		}
		Printf(cpp, "namespace Spr{\n");
		Nodes incs;
		FindNodeR(incs, top, "include");
		Nodes classes;
		Nodes externClasses;
		for(unsigned i=0; i<incs.size(); ++i){
#ifdef	_WIN32
			string key("\\");
#else
			string key("/");
#endif
			key.append(GetString( Getattr(top, "name")) );
			if (Strstr(Getattr(incs[i], "name"), key.c_str())){
				FindNodeR(classes, incs[i], "class");
			}else if (!Strstr(Getattr(incs[i], "name"), ".i")){
				FindNodeR(externClasses, incs[i], "class");
			}
		}
		Nodes descs;
		Nodes intfs;
		Nodes vintfs;
		Nodes objs;
		Nodes objDefs;
		Nodes externalObjs;
		std::vector<bool> absts;
		for(unsigned i=0; i<classes.size(); ++i){
			Node* n = classes[i];
			Node* cn = firstChild(n);
			while(cn){
				if ( Cmp(nodeType(cn), "constructor") == 0){
					if (Strstr(Getattr(cn, "name"), "SPR_OBJECTDEF")){
						objs.push_back(n);
						objDefs.push_back(cn);
						if (Strstr(Getattr(cn, "name"), "SPR_OBJECTDEF_ABST")){
							absts.push_back(true);
						}else{
							absts.push_back(false);
						}
						break;
					}else if (Strstr(Getattr(cn, "name"), "SPR_IFDEF")){
						intfs.push_back(n);
						break;
					}else if (Strstr(Getattr(cn, "name"), "SPR_VIFDEF")){
						vintfs.push_back(n);
						break;
					}
				}
				cn = nextSibling(cn);
			}
			if (!cn && Cmp(Getattr(n, "kind"), "struct") == 0){
				descs.push_back(n);
			}
		}
		for(unsigned i=0; i<externClasses.size(); ++i){
			Node* n = externClasses[i];
			Node* cn = firstChild(n);
			while(cn){
				if ( Cmp(nodeType(cn), "constructor") == 0){
					if (Strstr(Getattr(cn, "name"), "SPR_OBJECTDEF")){
						externalObjs.push_back(n);
						break;
					}
				}
				cn = nextSibling(cn);
			}
		}
		for(unsigned i=0; i<descs.size(); ++i){
			DescImp(cpp, descs[i]);
		}
		//	TypeDesc登録関数
		Printf(cpp, "\nvoid RegisterTypeDesc%s(UTTypeDescDbIf* db){\n", Getattr(modules.front(), "name"));
		Printf(cpp, "	static bool bFirst=true;\n");
		Printf(cpp, "	if (!bFirst) return;\n");
		Printf(cpp, "	bFirst = false;\n\n");

		Printf(cpp, "	UTTypeDescIf* desc;\n");
		Printf(cpp, "	int field;\n" );
		for(unsigned i=0; i<descs.size(); ++i){
			TypeDesc(cpp, hpp, descs[i]);
		}
		Printf(cpp, "}\n\n");
		//	SPR_IFIMP
		for(unsigned i=0; i<intfs.size(); ++i){
			IfImp(cpp, sprh, intfs[i], false);
		}
		for(unsigned i=0; i<vintfs.size(); ++i){
			IfImp(cpp, sprh, vintfs[i], true);
		}
		//	SPR_OBJECTIMP
		for(unsigned i=0; i<objs.size(); ++i){
			Strings bases;
			bool specBase = false;
			string def = GetString(Getattr(objDefs[i], "name"));
			size_t s = def.find("::");
			if (s != string::npos) def = def.substr(s+2);
			if (def.compare(0,13, "SPR_OBJECTDEF")==0) def = def.substr(13);
			if (def.compare(0,5, "_ABST")==0) def = def.substr(5);
			if (def.compare(0,5, "_NOIF")==0) def = def.substr(5);
			if (def.length()) specBase = true;
			if (specBase){
				string args = GetString(Getattr(objDefs[i], "decl"));
				args = args.substr(2,args.length()-4);
				while(1){
					size_t p = args.find(",");
					if (p==string::npos){
						bases.push_back(args);
						break;
					}else{
						string base = args.substr(0, p);
						args = args.substr(p+1);
						bases.push_back(base);
					}
				}
				if (bases.size()) bases.erase(bases.begin());
			}else{
				Node* base = Getattr(objs[i], "baselist");
				DohIterator it = DohFirst(base);
				while(it.item){
					for(unsigned j=0; j<objs.size(); ++j){
						if (Cmp(it.item, Getattr(objs[j], "sym:name")) == 0){
							bases.push_back( GetString(it.item));
						}
					}
					for(unsigned j=0; j<externalObjs.size(); ++j){
						if (Cmp(it.item, Getattr(externalObjs[j], "sym:name")) == 0){
							bases.push_back( GetString(it.item));
						}
					}
					it = DohNext(it);
				}
			}
			if (bases.size() == 0 && Cmp(Getattr(objs[i],"sym:name"), "Object")!=0){
				Printf(stderr, "Error: %s do not have base class.\n", Getattr(objs[i],"sym:name"));
				SWIG_exit(EXIT_FAILURE);
			}
			if (absts[i]){
				Printf(cpp, "SPR_OBJECTIMP_ABST");
			}else{
				Printf(cpp, "SPR_OBJECTIMP");
			}
			Printf(cpp, "%d(%s", bases.size(), Getattr(objs[i],"sym:name"));
			for(unsigned j=0; j<bases.size(); ++j){
				Printf(cpp, ", %s", bases[j].c_str());
			}
			Printf(cpp, ");\n");
		}
		//	
		Printf(cpp, "}\n");

		//	ツリーのダンプを出力
		log = NewFile((DOH*) "swig_spr.log", "w", NULL);
		if (!log) {
			FileErrorDisplay((DOH*) "swig_spr.log");
			SWIG_exit(EXIT_FAILURE);
		}
		DumpNode(log, top);
		if (errorFlag){
			SWIG_exit(EXIT_FAILURE);
		}
		return SWIG_OK;
	}
	virtual void main(int argc, char *argv[]){
	    SWIG_typemap_lang("Springhead");
		for (int iX = 0; iX < argc; iX++) {
			if (strcmp(argv[iX], "-spr") == 0) {
				char *extension = 0;
				if (iX + 1 >= argc) continue;
				extension = argv[iX + 1] + strlen(argv[iX + 1]) - 4;
				if (strcmp(extension, ".spr")) continue;
				iX++;
				Swig_mark_arg(iX);
				String *outfile = NewString(argv[iX]);
				log = NewFile(outfile, "w", NULL);
				if (!log) {
					FileErrorDisplay(outfile);
					SWIG_exit(EXIT_FAILURE);
				}
				continue;
			}
			if (strcmp(argv[iX], "-help") == 0) {
				fputs(usage, stdout);
			}
		}
		// Add a symbol to the parser for conditional compilation
		Preprocessor_define("SWIGSPR 1", 0);
	}

	void DumpNode(DOHFile* file, Node *obj) {
		PrintIndent(file, 0);
		//	ノードの表示
		Printf(file, "+++ %s ----------------------------------------\n", nodeType(obj));
		Iterator ki;
		Node *cobj;
		ki = First(obj);
		//	ノードのもつ属性の表示
		while (ki.key) {
			String *k = ki.key;
			if ((Cmp(k, "nodeType") == 0) || (Cmp(k, "firstChild") == 0) || (Cmp(k, "lastChild") == 0) ||
			(Cmp(k, "parentNode") == 0) || (Cmp(k, "nextSibling") == 0) || (Cmp(k, "previousSibling") == 0) || (*(Char(k)) == '$')) {
				/* Do nothing */
			} else if (Cmp(k, "parms") == 0) {
				PrintIndent(file, 2);
				Printf(file, "%-12s - %s\n", k, ParmList_protostr(Getattr(obj, k)));
			} else {
				char *trunc = "";
				PrintIndent(file, 2);
				DOH* attr = Getattr(obj, k);
				if (DohIsString(attr)) {
					DOH* o = Str(attr);
					if (Len(o) > 80) {
						trunc = "...";
					}
					Printf(file, "%-12s = \"%(escape)-0.80s%s\"\n", k, o, trunc);
					Delete(o);
				} 
				/*else if (DohIsSequence(attr)){
					Printf(file, "%-12s - {", k);
					for(int i=0; i<DohLen(attr); ++i){
						DOH* item = DohGetitem(attr, i);
						if (DohIsString(item)){
							DOH* o = Str(item);
							Printf(file, "%s ", o);
						}
					}
					Printf(file, "}\n");
				} else if (DohIsMapping(attr)){
					Printf(file, "%-12s - {", k);
					DohIterator it = DohFirst(attr);
					do{
						Printf(file, "%s-", it.key);
						if (DohIsString(it.object)){
							DOH* o = Str(it.object);
							Printf(file, "%s ", o);
						}else{
							Printf(file, "0x%x ", it.object);
						}
					}while((it = DohNext(it)).key);
					Printf(file, "}\n");
				}
				*/
				else{
					char* type="";
					if (DohIsFile(attr)) type="File";
					if (DohIsMapping(attr)) type="Mapping";
					Printf(file, "%-12s - 0x%x : %s\n", k, attr, type);
				}
			}
			ki = Next(ki);
		}
		//	子ノードの表示
		cobj = firstChild(obj);
		if (cobj) {
			indent_level += 4;
			while (cobj) {
				DumpNode(file, cobj);
				cobj = nextSibling(cobj);
			}
			indent_level -= 4;
		}
	}
	void PrintIndent(DOHFile* file, int l) {
		int i;
		for (i = 0; i < indent_level; i++) {
			Printf(file, " ");
		}
		if (l) {
			Printf(file, "|");
			Printf(file, " ");
		}
	}
};


static Language *new_swig_spr() {
  return new Springhead();
}
extern "C" Language *swig_spr(void) {
  return new_swig_spr();
}

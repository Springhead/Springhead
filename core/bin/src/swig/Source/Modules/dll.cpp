/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * Dll.cxx
 *
 * Interface stubs and type descriptions generator for Dll2
 * ----------------------------------------------------------------------------- */

#include "swigmod.h"
#include <string>
#include <vector>
#include <set>
using namespace std;
typedef std::vector<string> Strings;
typedef std::vector<Node*> Nodes;

static const char *usage = "\
Dll loader (available with -dll)\n";

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

extern bool operator < (const Enum& a, const Enum& b);
extern std::string GetString(DOHString* s);
#ifdef	_WIN32
extern std::string DecodeType(DOHString* n, std::string& post = string());
#else
extern std::string DecodeType(DOHString* n, std::string& post);
#endif
extern void GetBaseList(Strings& rv, DOH* baseList, char* suffix);
extern void FindNodeR(Nodes& rv, Node* n, char* type);
extern void DescImp(DOHFile* file, Node* n);


class Dll:public Language {
	int indent_level;
	File *cpp, *hpp, *log;
	bool errorFlag;
public:
	Dll():indent_level(0), cpp(NULL), hpp(NULL), log(NULL), errorFlag(false){
	}
	virtual ~ Dll(){
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
	/* Top of the parse tree */
	virtual int top(Node *top) {
		String *cppfile = NewString(Getattr(top, "outfile"));
		Replaceall(cppfile, "_wrap.cxx", "StubImpl.cxx");
		String *hppfile = NewString(Getattr(top, "outfile"));
		Replaceall(hppfile, "_wrap.cxx", "Stub.hpp");
		cpp = NewFile(cppfile, "w", NULL);
		if (!cpp) {
			FileErrorDisplay(cppfile);
			SWIG_exit(EXIT_FAILURE);
		}
		hpp = NewFile(hppfile, "w", NULL);
		if (!cpp) {
			FileErrorDisplay(hppfile);
			SWIG_exit(EXIT_FAILURE);
		}
		Nodes modules;
		FindNodeR(modules, top, "module");
/*		for(unsigned i=0; i<modules.size(); ++i){
			Node* include = nextSibling(modules[i]);
			while(include){
				if (Cmp(nodeType(include), "include")==0){
					Printf(cpp, "#include \"%s\"\n", Getattr(include, "name"));
				}
				include = nextSibling(include);
			}
		}
*/		
		Nodes allFuncs;
		Nodes funcs;
		FindNodeR(allFuncs, top, "cdecl");
		std::set<std::string> funcNames;
		for(unsigned i=0; i<allFuncs.size(); ++i){
			Node* n = allFuncs[i];
			DOH* code = Getattr(n, "code");
			if (code) continue;
			DOH* storage = Getattr(n, "storage");
			//if (Cmp(storage, "extern") != 0) continue;
			std::string name = GetString(Getattr(n, "name"));
			std::pair<std::set<std::string>::iterator, bool> rv;
			rv = funcNames.insert(name);
			if (rv.second) funcs.push_back(n);
		}
		for(unsigned i=0; i<funcs.size(); ++i){
			Node* n = funcs[i];
			DOH* name = Getattr(n, "name");
			DOH* type = Getattr(n, "type");
			DOH* parms = Getattr(n, "parms");
			DOH* argDef = ParmList_str_defaultargs(parms);
			DOH* arg = ParmList_str(parms);
			//Printf(cpp, "name: %s\n", attr);
			Printf(hpp, "extern %s (* %s) (%s);\n", type, name, argDef);
			Printf(cpp, "%s (* %s) (%s);\n", type, name, arg, name);
		}
		Printf(cpp, "\n");
		Printf(cpp, "void Init%sFunc(){\n", Getattr(top, "name"));
		for(unsigned i=0; i<funcs.size(); ++i){
			Node* n = funcs[i];
			DOH* name = Getattr(n, "name");
			Printf(cpp, "\t*(void **)&%s = SWIGDLL_GETPROC(\"%s\");\n", name, name);
		}
		Printf(cpp, "}\n");

		//	ツリーのダンプを出力
		log = NewFile((DOH*) "swig_dll.log", "w", NULL);
		if (!log) {
			FileErrorDisplay((DOH*) "swig_dll.log");
			SWIG_exit(EXIT_FAILURE);
		}
		DumpNode(log, top);
		if (errorFlag){
			SWIG_exit(EXIT_FAILURE);
		}
		return SWIG_OK;
	}
	virtual void main(int argc, char *argv[]){
	    SWIG_typemap_lang("Dll");
		for (int iX = 0; iX < argc; iX++) {
			if (strcmp(argv[iX], "-dll") == 0) {
				char *extension = 0;
				if (iX + 1 >= argc) continue;
				extension = argv[iX + 1] + strlen(argv[iX + 1]) - 4;
				if (strcmp(extension, ".dll")) continue;
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


static Language *new_swig_dll() {
  return new Dll();
}
extern "C" Language *swig_dll(void) {
  return new_swig_dll();
}

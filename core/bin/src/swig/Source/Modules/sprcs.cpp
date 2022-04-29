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
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <regex>
using namespace std;
typedef std::vector<string> Strings;
typedef std::vector<Node*> Nodes;

#ifndef _WIN32
#define vsnprintf_s vsnprintf
int sprintf_s(char* s, size_t n, const char* f, ...) {
  va_list list;
  va_start(list, f);
  int r = sprintf(s, f, list);
  va_end(list);
  return r;
}
#include <algorithm>
#endif

#define ALERT(node_or_str,message) printf("(Alert) %s : " #message "\n", DohCheck(node_or_str) ? Char(Getattr((DOH*)node_or_str,"name")): (char*) node_or_str )

// Compile conditions
//	FREE_UNMANAGED_MEMORY		C++側でstringからBSTRへの変換に使用したメモリをC#側から解放するコードを生成する
//	GENERATE_TRY_CATCH		C++側での関数呼出し時に発生した例外を捕捉するコードを生成する
//	ADOPT_TEMPLATE_CLASS		templateとして定義されたクラスを生成する ** 未完成 **
//	GENERATE_OPT_CONSTRUCTOR	すべての引数を指定したコンストラクタを自動的に生成する
//
#define	FREE_UNMANAGED_MEMORY		1
#define	GENERATE_TRY_CATCH		1
#define	ADOPT_TEMPLATE_CLASS		0
#define	GENERATE_OPT_CONSTRUCTOR	0
#define	USE_SIGNATURE_FILE		0
#define	ALLOW_INHERITANCE_ONLY_DESC_AND_IF	0

#define	CAN_GET_RETURNED_ARRAY_SIZE	0
//					関数が配列へのポインタを返す場合にその配列のサイズを得る手段がない
//					現状では要素数 1 の配列 (T[1]) として返す
//					関数呼び出しはでき正常に機能するが、戻り値の2番目以降の要素にアクセスすることはできない

// for avoiding duplicated wrapper class generation
#if (USE_SIGNATURE_FILE == 1)
#define	SIGNATURE_FILE	"swig_sprcs.signature"
std::ifstream sig_ifs;
std::ofstream sig_ofs;
#endif	// USE_SIGNATURE_FILE

//static const char *usage = "\
//Springhead C++/CLI Options (available with -sprcli)\n";
static const char *usage = "\
Springhead C++/C# Options (available with -sprcs)\n";

std::string TrimSuffix(DOHString* s, char* suffix);
std::string Trim(char* pre, DOHString* s, char* suf);
std::string GetString(DOHString* s);
#ifdef _WIN32
std::string DecodeType(DOHString* n, std::string& post = string());
#else
std::string DecodeType(DOHString* n, std::string& post);
#endif
void GetBaseList(Strings& rv, DOH* baseList, char* suffix);

struct Field{
	string type;
	string name;
};
typedef std::vector<Field> Fields;

static int node_no = 0;
struct nodeinfo_t {
	int no;
	char* sym_name;
	char* name;
	char* decl;
	char* access;
	char* storage;
	char* kind;
	char* type;
	char* uq_name;
	char* uq_type;
	char* overname;
	char* cpp_name;
	char* cpp_type;
	char* cs_name;
	char* cs_type;
	char* cs_im_type;
	char* cs_marshaltype;
	int is_typedef;
	int is_variable;
	int is_intrinsic;
	int is_vector;
	int is_string;
	int is_function;
	int is_struct;
	int is_array;		// 1 if array_size != NULL
	int is_pointer;		// 1 if pointer_level > 0
	int is_reference;
	int is_const;
	int is_volatile;
	int is_enum;
	int is_void;
	int is_void_ptr;
	int is_bool;
	int is_static;
	int is_virtual;
	char* array_size;
	int pointer_level;
	int num_args;
	struct nodeinfo_t* funcargs;
};
typedef struct nodeinfo_t NodeInfo;

struct typeconv_t {
	int kind;
	char* cpp_type;
	char* cs_type;
	char* cs_marshaltype;
	int count;
};
typedef struct typeconv_t TypeConv;
static TypeConv type_conv[] = {
	{  1, "char",		"sbyte",	"I1",		0 },	// kind 1: intrinsic data type
	{  1, "short",		"short",	"I2",		0 },
	{  1, "int",		"int",		"I4",		0 },
	{  1, "long",		"long",		"I8",		0 },
	{  1, "unsigned char",	"byte",		"U1",		0 },
	{  1, "unsigned short",	"ushort",	"U2",		0 },
	{  1, "unsigned",	"uint",		"U4",		0 },
	{  1, "unsigned int",	"uint",		"U4",		0 },
	{  1, "unsigned long",	"uint",		"U4",		0 },
	{  1, "bool",		"bool",		"U1",		0 },
	{  1, "float",		"float",	"R4",		0 },
	{  1, "double",		"double",	"R8",		0 },
	{  1, "void",		"void",		NULL,		0 },
#ifdef _WIN64
	{  1, "size_t",		"ulong",	"U8",		0 },
#else
	{  1, "size_t",		"uint",		"U4",		0 },
#endif
	{  2, "void*",		"IntPtr",	"SysUInt",	0 },
	{  3, "string",		"string",	"BStr",		0 },	// kind 3: string type
	{  4, "vector",		NULL,		NULL,		0 },	// kind 4: vector type
	{  0, "",		"object",	"LPStruct",	0 }	// MUST BE last element
};

static const char* type_modifiers[] = {
	"p.",
	"r.",
	"q(const).",
	"qconst.",
	"q(volatile).",
	"qvolatile.",
	NULL
};

#define	DLLIMPORT	"[DllImport(\"SprExport.dll\", CallingConvention=CallingConvention.Cdecl)]"

struct struct_info_t {
	char* name;
	int num_members;
	struct struct_members_info_t** members;
};
struct struct_members_info_t {
	Node* node;
	char* name;
	char* cs_name;
	char* cs_type;
	char* cpp_name;
	char* cpp_type;
	int is_bool;
	int is_vector;
	int is_array;
	int is_string;
	int is_struct;
	int is_pointer;
	int is_reference;
};
typedef struct struct_info_t StructInfo;
typedef struct struct_members_info_t StructMembersInfo;

struct template_class_info_t {
	Node* template_node;
	char* template_name;
	char* instance_name;
	char* temp_param;	// template parameter
	char* inst_param;	// type name corresponding to 'temp_param'
	char inst_param_ch;	// one charater representation of 'inst_param'
};
typedef struct template_class_info_t TemplateClassInfo;

// ファイル出力の制御（ソースを見易くするため）
//
const int FD_CPP	= 0x00000001;	// SprExport .cpp file
const int FD_CS		= 0x00000010;	// SprCSharp .cs file
const int FD_CSP	= 0x00000100;	// SprImport .cs file
const int FD_LOG	= 0x00001000;	// swig_sprcs_<module>.log file
const int FD_INFO	= 0x01000000;	// Cs.info file
const int FD_ERR	= 0x10000000;	// stderr
const int FD_NULL	= 0;
const int FD_ALL	= FD_CPP | FD_CS | FD_CSP;
#define	FP(fd)		((fd == FD_CPP) ? fps[0] : (fd == FD_CS) ? fps[1] : (fd == FD_CSP) ? fps[2] : stderr)

#define	CPP		fps[0]
#define	CS		fps[1]
#define	CSP		fps[2]

#define	EQ(p,q)		(strcmp((p),(q)) == 0)
#define	EQc(p,q)	((p) && (q) && (strcmp((p),(q)) == 0))
#define EQ2(p,q)	((p[0] == q[0]) && p[1] == q[1])
#define	ENDWITH(p,s)	(strcmp(p + strlen(p) - strlen(s), s) == 0)
#define	BEGINWITH(p,s)	(strncmp(p, s, strlen(s)) == 0)
#define	NON_NULL(s)	((s) ? (s) : (""))
#ifdef _WIN64
  #define ALIGNMENT	8
#else
  #define ALIGNMENT	4
#endif

// try-catch code を出力するか否か
//
#define TC_PTR		true
#define TC_VAL		false
#define TC_I(n)		(((n) == 1) ? " " : ((n)==4) ? "    " : ((n)==8) ? "\t" : "\t    ")
#if (GENERATE_TRY_CATCH == 1)
  #define CATCH_code		"catch (SEH_Exception e) { e.raise_managed_exception(\"SprExport.dll\"); }"
  #define RETURN_p		return TC_VAR_p
  #define RETURN_v		return _val
  //
  #define TRY_begin(n,T,V)	Printf(CPP, "%s%s %s = (%s) NULL;\n%stry { %s = ", TC_I(n), T, #V, T, TC_I(n), #V)
  #define TRY_begin_new(n,T,V)	Printf(CPP, "\n%s%s %s = (%s) NULL;\n%stry { %s = ", TC_I(n), #T, #V, #T, TC_I(n), #V)
  #define TRY_begin_NEW(n,T,V)	TRY_begin(n,T,V)
  #define TRY_begin_str(n,T,V)	Printf(CPP, "%s%s %s = \"\";\n%stry { %s = ", TC_I(n), T, #V, TC_I(n), #V)
  #define TRY_begin_void(n)	Printf(CPP, "%stry { ", TC_I(n))
  #define TRY_cont(n)		Printf(CPP, " ")/*;*/
  #define TRY_end()		Printf(CPP, " }\n")
  #define TRY_end_new()		Printf(CPP, " }\n")
  #define TRY_end_NEW()		TRY_end_new()
  #define CATCH(n)		Printf(CPP, "%s%s\n", TC_I(n), CATCH_code)
  #define RETURN(n,V)		Printf(CPP, "%sreturn %s;\n", TC_I(n), #V)
  #define RETURN_new(n,V)	Printf(CPP, "%sreturn %s;\n%s}\n", TC_I(n), #V, TC_I(n-4))
  #define RETURN_NEW(n,V)	RETURN(n,V)
#else
  #define TRY_begin(n,T,V)	Printf(CPP, "%s%s %s = ", TC_I(n), T, #V)
  #define TRY_begin_new(n,T,p)	Printf(CPP, " return ")/*;*/
  #define TRY_begin_NEW(n,T,p)	Printf(CPP, "%sreturn ", TC_I(n))/*;*/
  #define TRY_begin_str(n,T,V)	Printf(CPP, "%s%s %s = ", TC_I(n), T, #V)
  #define TRY_begin_void(n)	Printf(CPP, "%s", TC_I(n))
  #define TRY_cont(n)		Printf(CPP, "\n%s", TC_I(n))/*;*/
  #define TRY_end()		Printf(CPP, "\n")
  #define TRY_end_new()		Printf(CPP, " }\n")
  #define TRY_end_NEW()		Printf(CPP, "\n")/*;*/
  #define CATCH(n)
  #define RETURN(n,V)		Printf(CPP, "%sreturn %s;\n", TC_I(n), #V)
  #define RETURN_new(n,V)
  #define RETURN_NEW(n,V)
#endif

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
bool operator < (const Enum& a, const Enum& b);
map<string, string> typedef_map;
map<string, string> typedef_inverse_map;
map<char*, int> class_map;
map<char*, int> enum_map;
map<char*, int> intf_map;
map<char*, int> vintf_map;
map<string, int> delegate_key_map;
map<string, string> delegate_func_map;
map<string, string> delegate_type_map;
map<string, string> delegate_args_map;
map<string, int> signature_map;
map<string, string> signature_args_map;
map<string, int> wrapper_map;
map<string, int> fd_pair_map;
map<string, string> operator_macros_map;
map<string, int> function_generation_map;
map<string, int> accessor_generation_map;
map<string, Node*> template_class_map;
map<string, Nodes> template_function_map;
map<string, TemplateClassInfo*> template_class_info_map;
map<string, NodeInfo> class_node_map;
map<string, char*> class_parent_map;
static int function_count = 0;

#define	MAX_NAMELEN	256
#define	HACK		1

//#define	_DEBUG
#ifdef	_DEBUG
  #define SNAP			1
  #define DUMP			1
  #define DUMP_TREE		1
  #define DUMP_NODE		1
  #define DUMP_STRUCT		0
  #define SHOW_ALL_TYPES	0
  #define GATHER_INFO		1
  #define BREAK_NODE_NO		0
#else
  #define SNAP			0
  #define DUMP			0
  #define DUMP_TREE		0
  #define DUMP_NODE		0
  #define DUMP_STRUCT		0
  #define SHOW_ALL_TYPES	0
  #define GATHER_INFO		0
  #define BREAK_NODE_NO		0
#endif

#if (SNAP == 1)
  #define SNAP_AT_LINE(x,y)		snap_at_line(x,y,__LINE__)
  #define SNAP_ANA_PATH2(x,y,a,b)	snap_path_info(x,y,__LINE__,a,b)
  #define SNAP_ANA_PATH1(x,y,a)		snap_path_info(x,y,__LINE__,a)
  #define SNAP_ANA_PATH1L(x,y,L,a)	snap_path_info(x,y,L,a)
  #define SNAP_ANA_PATH2L(x,y,L,a,b)	snap_path_info(x,y,L,a,b)
  #define WRAPPER_NAME_PRINT(x,y,L,a,b)	wrapper_name_print(x,y,L,a,b)
#else
  #define SNAP_AT_LINE(x,y)
  #define SNAP_ANA_PATH2(x,y,a,b)
  #define SNAP_ANA_PATH1(x,y,a)
  #define SNAP_ANA_PATH1L(x,y,L,a)
  #define SNAP_ANA_PATH2L(x,y,L,a,b)
  #define WRAPPER_NAME_PRINT(x,y,L,a,b)
#endif
#if (DUMP_NODE == 1)
  #define DUMP_NODE_INFO(x,y,a,b)	dump_node_info(x,y,__LINE__,a,b)
  #define DUMP_NODE_INFO_L(x,y,L,a,b)	dump_node_info(x,y,L,a,b)
#else
  #define DUMP_NODE_INFO(x,y,a,b)
  #define DUMP_NODE_INFO_L(x,y,L,a,b)
#endif
#if (DUMP_STRUCT == 1)
  #define DUMP_STRUCT_INFO(x,y,a)	dump_struct_info(x,y,__LINE__,a)
  #define DUMP_STRUCT_INFO_L(x,y,L,a)	dump_struct_info(x,y,L,a)
#else
  #define DUMP_STRUCT_INFO(x,y,a)
  #define DUMP_STRUCT_INFO_L(x,y,L,a)
#endif
#if (GATHER_INFO == 1)
  DOHFile* gip;
  #if (DUMP_TREE == 1)
    int tree_dumped = 0;
  #endif
#else
  DOHFile* gip = NULL;
#endif
#if (BREAK_NODE_NO != 0)
  #define DEBUG_BREAK_AT(n)		debug_break_at(n)
#else
  #define DEBUG_BREAK_AT(n)
#endif

class SpringheadCs:public Language {
	int indent_level;
	File *cpp, *csp, *cs, *log, *fps[4];
	bool errorFlag;
	std::map<string, std::vector<string> > childClassMap;

public:
	SpringheadCs():indent_level(0), cpp(NULL), cs(NULL), csp(NULL), log(NULL), errorFlag(false){
	}
	virtual ~ SpringheadCs(){
	}

#if (ADOPT_TEMPLATE_CLASS == 1)
	void DescImp(DOHFile* fps[], Node* topnode, Node* n, bool both_desc_and_if = false, char* tc_name = NULL) {
#else
	void DescImp(DOHFile* fps[], Node* topnode, Node* n, bool both_desc_and_if = false) {
#endif
		NodeInfo& ci = get_node_info(fps, n);
  		DEBUG_BREAK_AT(ci);
		ci.name = unqualified_name(ci.sym_name);

		DUMP_NODE_INFO(fps, FD_ALL, "DescImp", ci);

		Strings bases;
		char* suffix = "Desc";
		GetBaseList(bases, Getattr(n, "baselist"), suffix);
#if (ALLOW_INHERITANCE_ONLY_DESC_AND_IF == 0)
		if (bases.size() == 0) {
			suffix = "";
			GetBaseList(bases, Getattr(n, "baselist"), suffix);
		}
#endif	

#if (ADOPT_TEMPLATE_CLASS == 1)
		bool template_class = false;
		if ((Cmp(nodeType(n), "template") == 0) && tc_name) {
			template_class = (template_function_map.find(tc_name) != template_function_map.end());
		}
		if (template_class) {
			// change Node "name" into template instance name
			ci.name = tc_name;
		}
		if (ci.name) {
			generate_new_class(fps, topnode, n, bases, suffix, ci.name, ci.uq_name, ci.cpp_name);
		}

		Nodes members;
		if (template_class) {
			members = template_function_map[tc_name];
		} else {
			FindNode(members, n, "cdecl");
		}
#else
		if (ci.name) {
			generate_new_class(fps, topnode, n, bases, suffix, ci.name, ci.uq_name, ci.cpp_name);
		}

		Nodes members;
		FindNode(members, n, "cdecl");
#endif // ADOPT_TEMPLATE_CLASS

		bool feature = false;
		for (int i = 0; i < (int) members.size(); i++) {
			if (GetFlagAttr(members[i], "feature:ignore")) { // %ignoreにあったらパス
				ALERT(members[i],"ignored:'%%ignore'");
				continue;
			}

			NodeInfo& ni = get_node_info(fps, members[i]);
			if (ni.cs_type && ci.name) analyze_cs_type_hook_2(&ni, ci.name);
#if (SHOW_ALL_TYPES == 1)
			DUMP_NODE_INFO(fps, FD_ALL, "DescImp cdecl", ni);
#endif

			if (ni.is_variable == 0)	continue;	// 変数でなければパス
			if (ni.is_function == 1)	continue;	// 変数でなければパス
			if (EQ(ni.kind, "typedef"))	continue;	// typedef はパス
			if (EQc(ni.storage, "static")) {
				if (ni.is_variable && EQ(ni.type, "q(const).int")) {
					// static const
					DUMP_NODE_INFO(fps, FD_CS, "DescImp cdecl: static", ni);
					Printf(CS, "    const int %s = %s;\n", ni.uq_name, Getattr(members[i], "value"));
				}
				continue;				// static 変数はパス
			}
			if (EQc(ni.access, "protected"))	continue;	// protected 変数はパス
			if (EQc(ni.access, "private"))		continue;	// private 変数はパス

			if (ni.is_function == 2 && ni.is_pointer) {
				// typedef された関数ポインタ
				if ((EQ(ci.name, "UTTimerIf") && EQ(ni.name, "TimerFunc")) ||
				    (EQ(ci.name, "PHHapticEngineIf") && EQ(ni.name, "Callback"))) {
					Printf(CPP, "// SKIP: %s::%s (typedef'd function pointer)\n", ci.name, ni.name);
					continue;
				}
			}
			feature |= (GetFlagAttr(members[i], "feature:returns_array") != NULL);

#if (ADOPT_TEMPLATE_CLASS == 1)
			if (template_class) {
				// template class のときは template parameter の置き換えをしないといけない
				TemplateClassInfo* tci = template_class_info_map[tc_name];
				//replace_template_parameter(ni, *tci);
			}
#endif

#if (SHOW_ALL_TYPES != 1)
			DUMP_NODE_INFO(fps, FD_ALL, "DescImp cdecl", ni);
#endif
			PRINTinfo(gip, "variable: %s%s\n", (ni.is_struct ? "struct: " : ""), ni.cpp_type);

			// vector or array
			if (ni.is_vector || ni.is_array) {
				if (ni.is_struct) {
					string wrapper_key(make_wrapper_name(fps, FD_ALL, __LINE__, ni, ci, "generate: wrapper key"));
					generate_wrapper_accessor(fps, topnode, ni, ci, "vector or array", __LINE__);
					if (wrapper_map.find(wrapper_key) == wrapper_map.end()) {
						generate_wrapper_accessor_struct(topnode, members[i], ni, ci, "vector/array member", __LINE__);
						wrapper_map[wrapper_key] = 1;
					}
				}
				else {
					generate_wrapper_accessor(fps, topnode, ni, ci, "variable", __LINE__);
				}
			}

			// C#に対応する型がある
			else if (ni.is_intrinsic) {
				// ポインタ
				if (ni.is_pointer) {
					SNAP_ANA_PATH1(fps, FD_ALL, "variable: pointer");
					generate_accessor_for_type_intrinsic_pointer(fps, ni, ci);
				}
				// bool（C++ bool <=> byte で受け渡し <=> C# bool）
				else if (ni.is_bool) {
					SNAP_ANA_PATH1(fps, FD_ALL, "variable: intrinsic: bool");
					generate_accessor_for_type_intrinsic_bool(fps, ni, ci);
				}
				//
				else {
					SNAP_ANA_PATH2(fps, FD_ALL, "variable: intrinsic: value", (ni.is_reference ? "reference" : ""));
					generate_accessor_for_type_intrinsic(fps, ni, ci);
				}
			}
			// string
			else if (ni.is_string) {
				SNAP_ANA_PATH1(fps, FD_ALL, "variable: string");
				generate_accessor_for_type_string(fps, ni, ci);
			}
			// struct
			else if (ni.is_struct) {
				Node* is_enum_node = FindNodeByAttrR(topnode, "enumtype", ni.type);
				// vector
				if (ni.is_vector) {
					SNAP_ANA_PATH1(fps, FD_ALL, "PAN: struct: vector");
					generate_accessor_for_type_struct(fps, ni, ci);
				}
				// array
				else if (ni.is_array) {
					SNAP_ANA_PATH1(fps, FD_ALL, "PAN: struct: array");
					generate_accessor_for_type_struct(fps, ni, ci);
				}
				// enum
				else if (is_enum_node) {
					SNAP_ANA_PATH1(fps, FD_ALL, "variable: struct: enum");
					DUMP_NODE_INFO(fps, FD_CS, "is_enum_node", get_node_info(fps, is_enum_node));
					generate_accessor_for_type_struct_enum(fps, topnode, ni, ci);
				}
				// pointer
				else if (ni.is_pointer) {
					SNAP_ANA_PATH1(fps, FD_ALL, "variable: struct: pointer");
					generate_accessor_for_type_struct_pointer(fps, ni, ci);
				}
				// struct or reference
				else {
					SNAP_ANA_PATH1(fps, FD_ALL, "variable: struct");
					generate_accessor_for_type_struct(fps, ni, ci);
				}
				string wrapper_key(make_wrapper_name(fps, FD_ALL, __LINE__, ni, ci, "generate: wrapper key"));
				if (wrapper_map.find(wrapper_key) == wrapper_map.end()) {
					generate_wrapper_accessor_struct(topnode, members[i], ni, ci, "struct", __LINE__);
					wrapper_map[wrapper_key] = 1;
				}
			}

			// enum
			else if (ni.is_enum) {
				SNAP_ANA_PATH1(fps, FD_ALL, "variable: enum");
				Nodes enumNodes;
				FindNode(enumNodes, members[i], "enum");
				if (enumNodes.size() == 0) {
					Node* en = FindNodeByAttrR(topnode, "enumtype", ni.type);
					if (en) {
						enumNodes.push_back(en);
					}
				}
				for (unsigned int i = 0; i < enumNodes.size(); i++) {
					NodeInfo& ei = ni;
					FP_generate_enum_def(fps, FD_CS, __LINE__, "\t", enumNodes[i], ei.cs_type, "DescImp");
					if (ni.name) {
						generate_accessor_for_type_enum(fps, ni, ci);
					}
				}
			}

			else {
				analyze_error(fps, __LINE__, "Unknown node", NULL);
				DUMP_NODE_INFO(fps, FD_ERR, "Unknown node", ni);
			}
		}

		// class 直下の enum 定義
		//
		Nodes enums;
		FindNodeR(enums, n, "enum");
		for (unsigned i = 0; i < enums.size(); ++i) {
			char* name = cs_qualified_name(Char(Getattr(enums[i], "type")));
			if (!name || !BEGINWITH(name, "enum ")) continue;
			name += 5;
			if (EQ(name, "")) continue;
			FP_generate_enum_def(fps, FD_CS, __LINE__, "\t", enums[i], NULL, "DescImp");
		}

		// 関数生成が指示されていたならば
		if (both_desc_and_if) {
#if (ADOPT_TEMPLATE_CLASS == 1)
			IfImp(fps, topnode, n, true, tc_name);
#else
			IfImp(fps, topnode, n, true);
#endif
		}

		// class の終了
		if (ci.name) {
			// [cs]
			//
			Printf(CS,  "    }\n");
		}

		// Struct クラスの生成
		if (ci.name) {
			generate_new_class_struct(fps, topnode, n, ci.name, ci.uq_name, feature);
		}
	}

#ifdef	ORIGINAL_CODE
	/***
	//	構造体ごとの処理(TypeDescの作成など)
	void TypeDesc(DOHFile* cpp, DOHFile* csp, Node* n){		
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
		Printf(cpp, "	desc = DBG_NEW UTTypeDesc(\"%s\");\n", name);
		Printf(cpp, "	desc->size = sizeof(%s);\n", fullname);

		if (cls.length()){
			Printf(cpp, "	desc->ifInfo = %sIf::GetIfInfoStatic();\n", cls.c_str());
			if (clsType == DESC){
				Printf(cpp, "	((IfInfo*)%sIf::GetIfInfoStatic())->desc = desc;\n", cls.c_str());
			}else if (clsType == STATE){
				Printf(cpp, "	((IfInfo*)%sIf::GetIfInfoStatic())->state = desc;\n", cls.c_str());
			}
		}
		Printf(cpp, "	desc->access = DBG_NEW UTAccess<%s>;\n", fullname);
		//	baselist
		Strings bases;
		GetBaseList(bases, Getattr(n, "baselist"), "");

		for(unsigned i = 0; i < bases.size(); ++i){
			Printf(cpp, "	field = desc->AddBase(\"%s\");\n", bases[i].c_str());
			Printf(cpp, "	field->offset = int((char*)(%s*)p%s - (char*)p%s);\n", bases[i].c_str(), name, name);
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
						Printf(cpp, "	field->AddEnumConst(\"%s\", %s);\n", 
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
			Printf(cpp, "	field->offset = int((char*)&(p%s->%s) - (char*)p%s);\n",
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
			char* suffix = (clsType==STATE) ? "State" : "Desc";
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
	***/
#endif	// ORIGINAL_CODE

	void TypeMapImp(DOHFile* cs, Node* topnode, Node* n){
		NodeInfo& ci = get_node_info(fps, n);
		if (ENDWITH(ci.cpp_name, "If")) {
			Printf(CS, "\t\t\t{%s.GetIfInfoStatic(), typeof(%s)},\n", ci.cpp_name, ci.cpp_name);
		}
	}


#if (ADOPT_TEMPLATE_CLASS == 1)
	void IfImp(DOHFile* fps[], Node* topnode, Node* n, bool class_already_defined = false, char* tc_name = NULL) {
#else
	void IfImp(DOHFile* fps[], Node* topnode, Node* n, bool class_already_defined = false) {
#endif
		NodeInfo& ci = get_node_info(fps, n);
		DEBUG_BREAK_AT(ci);
		ci.name = unqualified_name(ci.sym_name);

		DUMP_NODE_INFO(fps, FD_ALL, "IfImp", ci);

		Strings bases;
		char* suffix = "If";
		GetBaseList(bases, Getattr(n, "baselist"), suffix);
#if (ALLOW_INHERITANCE_ONLY_DESC_AND_IF == 0)
		if (bases.size() == 0) {
			suffix = "";
			GetBaseList(bases, Getattr(n, "baselist"), suffix);
		}
#endif	

#if (ADOPT_TEMPLATE_CLASS == 1)
		bool template_class = false;
		if ((Cmp(nodeType(n), "template") == 0) && tc_name) {
			template_class = (template_function_map.find(tc_name) != template_function_map.end());
		}
		if (template_class) {
			// change Node "name" into template instance one
			ci.name = tc_name;
		}
#endif

		if (ci.name && !class_already_defined) {
			generate_new_class(fps, topnode, n, bases, suffix, ci.name, ci.uq_name, ci.cpp_name);
		}

		Nodes members;
#if (ADOPT_TEMPLATE_CLASS == 1)
		if (template_class) {
			members = template_function_map[tc_name];
		} else {
			FindNode(members, n, "cdecl");
		}
#else
		FindNode(members, n, "cdecl");
#endif

		for (int i = 0; i < (int) members.size(); i++) {
			if (GetFlagAttr(members[i], "feature:ignore")) { // %ignoreにあったらパス
				ALERT(members[i],"ignored:'%%ignore'");
				continue;
			}

			NodeInfo& ni = get_node_info(fps, members[i]);
			if (ni.cs_type && ci.name) analyze_cs_type_hook_2(&ni, ci.name);
#if (SHOW_ALL_TYPES == 1)
			DUMP_NODE_INFO(fps, FD_ALL, "IfImp cdecl", ni);
#endif

			if (!ni.is_function)		continue;	// 関数でなければパス
#if (GATHER_INFO == 1)
			const char* drop_reason = NULL;
			if (EQc(ni.access, "private"))		drop_reason = "private";
			if (EQc(ni.access, "protected"))	drop_reason = "protected";
			//if (Getattr(members[i], "defaultargs"))	drop_reason = "defaultargs";
			if (BEGINWITH(ni.uq_name, "operator"))	drop_reason = "operator";
			if (BEGINWITH(ni.uq_type, "UTStack"))	drop_reason = "special";
			if (drop_reason) PRINTinfo(gip, "drop_function: %s.%s (%s)\n", ni.uq_type, ni.sym_name, drop_reason);
#endif
			if (EQc(ni.access, "private"))	continue;	// private 関数はパス
			if (EQc(ni.access, "protected"))	continue;	// protected 関数はパス
			//if (EQc(ni.storage, "static"))	continue;	// static 関数はパス
			if (EQc(ni.storage, "virtual") && !class_already_defined) {
				Printf(stderr, "Error: %s::%s() is a virtual function.\n", ni.sym_name, ni.name);
				errorFlag = true;
				continue;					//XXIf には，virtual 関数があってはならない．
			}
			//if (Getattr(members[i], "defaultargs"))	continue;	//デフォルト引数によるバリエーションは不要
			if (BEGINWITH(ni.uq_name, "operator"))	continue;
			// specical trap
			if (BEGINWITH(ni.uq_type, "UTStack"))	continue;

#if (ADOPT_TEMPLATE_CLASS == 1)
			if (template_class) {
				// template class のときは template parameter の置換えをしないといけない
				TemplateClassInfo* tci = template_class_info_map[tc_name];
				replace_template_parameter(ni, *tci);
			}
#endif
			// ポインタを返す関数が実際に配列を返す場合（%feature("returns_array") の処理）
			if (GetFlagAttr(members[i], "feature:returns_array")) {
				PRINTinfo(gip, "returns_array: %s %s\n", ci.name, ni.name);
				ni.pointer_level = 0;
				ni.is_pointer = 0;
				ni.is_array = 1;
				ni.array_size = "0";
				ni.cpp_type[strlen(ni.cpp_type)-1] = '\0';
			}
			

#if (SHOW_ALL_TYPES != 1)
			DUMP_NODE_INFO(fps, FD_ALL, "IfImp cdecl", ni);
#endif
#if (GATHER_INFO == 1)
			PRINTinfo(gip, "function: %s%s\n", (ni.is_struct ? "struct: " : ""), ni.cpp_type);
			for (int j = 0; j < ni.num_args; j++) {
				NodeInfo& ai = ni.funcargs[j];
				PRINTinfo(gip, "argument: %s%s\n", (ai.is_struct ? "struct: " : ""), ai.cpp_type);
			}
#endif
			// 関数引数を delegate するための前処理
			//	typedef された関数
			function_count++;
			int need_comma = 0;
			string signature_args("(");
			string delegate_args("(");
			if (!ni.is_static) {
				signature_args.append("(IntPtr) _this");
				delegate_args.append("IntPtr _this");
				need_comma = 1;
			}
			for (int j = 0; j < ni.num_args; j++) {
				NodeInfo& ai = ni.funcargs[j];
				if (need_comma) signature_args.append(", ");
				signature_args.append("(");
				signature_args.append(ai.cs_type);
				signature_args.append(") ");
				signature_args.append(argname(ai.uq_name, j));
				if (need_comma) delegate_args.append(", ");
				delegate_args.append(ai.cs_type);
				delegate_args.append(" ");
				delegate_args.append(argname(ai.uq_name, j));
				need_comma = 1;
			} 
			signature_args.append(");");
			delegate_args.append(");");
			//
			string signature("SprExport.Spr_");
			signature.append(ci.uq_name);
			signature.append("_");
			signature.append(ni.uq_name);
			signature.append(signature_args);
			if (signature_map.find(signature) == signature_map.end()) {
				signature_map[signature.c_str()] = 1;
			}
			else {
#ifdef _DEBUG
				Printf(CS, "        //ignore: same signature: %s\n", signature.c_str());
#endif
#if (GATHER_INFO == 1)
				if (signature_map.find(signature) == signature_map.end()) {
					PRINTinfo(gip, "signature_map: %s\n", signature);
				}
#endif
				continue;
			}
			//
			if (ni.is_function) {
				string delegate_key(ci.uq_name);
				delegate_key.append(".");
				delegate_key.append(ni.uq_name);
				//
				delegate_key_map[delegate_key] = 1;
				delegate_type_map[delegate_key] = string(ni.cs_type);
				delegate_func_map[delegate_key] = delegate_key;
				delegate_args_map[delegate_key] = delegate_args;
				signature_args_map[delegate_key] = signature_args;
				PRINTinfo(gip, "delegate_func_map: %s %s%s\n", ni.cs_type, delegate_key.c_str(), delegate_args.c_str());
			}
			//if (ni.is_function == 2) continue;	// typedef された関数自体はパス

			// [cpp]
			//
			SNAP_ANA_PATH1(fps, FD_CPP, "function: cdecl");
			Node* is_enum_node = (ni.is_struct) ? FindNodeByAttrR(topnode, "enumtype", ni.type) : NULL;
			// 関数呼出し処理
			char* return_type;
			if	(ni.is_void)		{ return_type = "void"; }
			else if (ni.is_bool)		{ return_type = "char"; }
			else if (ni.is_vector)		{ return_type = "HANDLE"; }
			else if (ni.is_array)		{ return_type = "HANDLE"; }
			else if (ni.is_string)		{ return_type = "BSTR"; }
			else if (ni.is_void_ptr)	{ return_type = "HANDLE"; }
			else if (ni.is_intrinsic && !ni.is_pointer)	{ return_type = ni.uq_type; }
			else if (is_enum_node)		{ return_type = "int"; }
			else				{ return_type = "HANDLE"; }
			char** argnames = NULL;
			void** cleanup1 = NULL;
			void** cleanup2 = NULL;
			if (ni.num_args > 0) {
				argnames = new char*[ni.num_args];
			}
			// 関数定義
			Printf(CPP, "    __declspec(dllexport) %s __cdecl Spr_%s_%s%s(", return_type, ci.uq_name, ni.uq_name, overname(ni));
			int sep_needed = 0;
			if (!ni.is_static) {
				Printf(CPP, "HANDLE _this");
				sep_needed = 1;
			}
			// 仮引数並び
			for (int j = 0; j < ni.num_args; j++) {
				NodeInfo& ai = ni.funcargs[j];
				Node* is_enum_node_a = FindNodeByAttrR(topnode, "enumtype", ai.type);
				char* arg_type = (char*) ((ai.is_intrinsic && !ai.is_pointer) ? ai.cpp_type : ((ai.is_string) ? "BSTR" : "HANDLE"));
				if (ai.is_vector)   arg_type = "HANDLE";
				if (ai.is_array)    arg_type = "HANDLE";
				if (ai.is_bool)	    arg_type = "char";		// bool（C++ bool <=> byte で受け渡し <=> C# bool）
				if (is_enum_node_a) arg_type = "int";		// ai.cpp_type;
				if (sep_needed) Printf(CPP, ", ");
				Printf(CPP, "%s %s", arg_type, argname(ai.uq_name, j));
				sep_needed = 1;
			}
			Printf(CPP, ") {\n");
			// 実引数に関する前処理
			for (int j = 0; j < ni.num_args; j++) {
				NodeInfo& ai = ni.funcargs[j];
				Node* is_enum_node_a = FindNodeByAttrR(topnode, "enumtype", ai.type);
				char argbuff[MAX_NAMELEN+1];
				char* tmpname = argname(NULL, j);
				// vector or array
				if (ai.is_vector || ai.is_array) {
					WRAPPER_NAME_PRINT(fps, FD_CPP, __LINE__, ai, "function args");
					if (ai.is_vector) {
						sprintf(argbuff, "*((vector<%s>*) %s)", ai.cpp_type, ai.name);
					}
					else if (EQ(ai.uq_type, "char")) {
						sprintf(argbuff, "&(((%s*) %s)[0])", ai.cpp_type, ai.name);
					}
					else {
						sprintf(argbuff, "(%s*) %s", ai.cpp_type, ai.name);
					}
					argnames[j] = Char(NewString(argbuff));
				}
				// C#に対応する型がある
				else if (ai.is_intrinsic) {
					// ポインタ
					if (ai.is_void_ptr) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: intrinsic: pointer");
						sprintf(argbuff, "%s", ai.name);
					}
					else if (ai.is_pointer) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: intrinsic: pointer");
						sprintf(argbuff, "(%s) %s", ai.cpp_type, ai.name);
					}
					// bool（C++ bool <=> byte で受け渡し <=> C# bool）
					else if (ai.is_bool) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: intrinsic: bool");
						Printf(CPP, "\tbool %s = (%s == 0) ? false : true;\n", tmpname, ai.name);
						sprintf(argbuff, "(%s) %s", ai.cpp_type, tmpname);
					}
					// 値または参照
					else {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: intrinsic: value or value&");
						sprintf(argbuff, "%s", argname(ai.name, j));
					}
					argnames[j] = Char(NewString(argbuff));
				}
				// string
				else if (ai.is_string) {
					// ポインタ
					if (ai.is_pointer) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: string: pointer  ** NOT IMPLEMENTED **");
						sprintf(argbuff, "(%s) %s", ai.cpp_type, ai.name);
					}
					// string or string&（受け渡しは BSTR）
					else {
						PRINTF(fps, FD_CPP, "//_[%s: %d] %s\n", "function_args: string", __LINE__, ai.is_reference ? " reference" : "");
						Printf(CPP, "\tstring %s(\"\");\n", tmpname);
						generate_string_set(CPP, j+1, "\t", ai.name, NULL, tmpname);
						sprintf(argbuff, "(%s) %s", ai.cpp_type, tmpname);
					}
				}
				// struct
				else if (ai.is_struct) {
					// ポインタ
					if (ai.is_pointer) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: struct pointer");
						sprintf(argbuff, "(%s) %s", ai.cpp_type, ai.name);
					}
					// 参照
					else if (ai.is_reference) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: struct reference");
						sprintf(argbuff, "(%s) *((%s*) %s)", ai.cpp_type, ai.uq_type, ai.name);
					}
					// Enum
					else if (is_enum_node_a) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: enum");
						Printf(CPP, "\t%s %s = ((%s) %s);\n", ai.uq_type, tmpname, ai.uq_type, ai.name);
						sprintf(argbuff, "(%s) %s", ai.cpp_type, tmpname);
					}
					// struct
					else {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_args: struct");
						Printf(CPP, "\t%s %s = *((%s*) %s);\n", ai.uq_type, tmpname, ai.uq_type, ai.name);
						sprintf(argbuff, "(%s) %s", ai.cpp_type, tmpname);
					}
				}
				else {
					analyze_error(fps, __LINE__, "Unknown function argument type", NULL);
					DUMP_NODE_INFO(fps, FD_ERR, "Unknown type", ai);
				}
				argnames[j] = Char(NewString(argbuff));
			}
			// 関数本体
			SNAP_ANA_PATH2(fps, FD_CPP, "function_body", ni.cpp_type);
			is_enum_node = (ni.is_struct) ? FindNodeByAttrR(topnode, "enumtype", ni.type) : NULL;
#if (DUMP == 1)
			if (is_enum_node) {
				NodeInfo& en = get_node_info(fps, is_enum_node);
				DUMP_NODE_INFO(fps, FD_CS, "is_enum_node", en);
			}
#endif
#define	TRY_CASE_VOID	0
#define	TRY_CASE_RCNV	1
#define	TRY_CASE_RPTR	2
			int try_case = TRY_CASE_VOID;
			char cpp_type[MAX_NAMELEN+33];
			if (ni.is_void) {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: void");
				try_case = TRY_CASE_VOID;
				TRY_begin_void(8);
			}
			else if (ni.is_vector) {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: vector");
				try_case = TRY_CASE_RPTR;
				sprintf_s(cpp_type, sizeof(cpp_type), "vector<%s>*", ni.cpp_type);
				Printf(CPP, "        vector<%s>* _ptr = (vector<%s>*) NULL;\n", ni.cpp_type, ni.cpp_type);
				Printf(CPP, "        try { _ptr = new vector<%s>;\n", ni.cpp_type, ni.cpp_type);
				Printf(CPP, "              vector<%s> _vec = (", ni.cpp_type);
			}
			else if (ni.is_array) {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: array");
				try_case = TRY_CASE_RPTR;
				sprintf_s(cpp_type, sizeof(cpp_type), "%s*", ni.cpp_type);
				//TRY_begin(8, cpp_type, _ptr);
				Printf(CPP, "        %s* _ptr = (%s*) NULL;\n", ni.cpp_type, ni.cpp_type);
				Printf(CPP, "        try { %s* _ary = (", ni.cpp_type);
			}
			else if (ni.is_struct) {
				if (is_enum_node) {
					SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: struct: enum");
					try_case = TRY_CASE_RCNV;
					TRY_begin(8, ni.cpp_type, _val);
				} else if (ni.is_pointer) {
					SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: struct: pointer");
					try_case = TRY_CASE_RPTR;
#if (GENERATE_TRY_CATCH == 1)
					sprintf_s(cpp_type, sizeof(cpp_type), "%s*", ni.uq_type);
					TRY_begin(8, cpp_type, _ptr);
					Printf(CPP, "(%s) ", cpp_type);
#else
					sprintf_s(cpp_type, sizeof(cpp_type), "%s", ni.cpp_type);
					TRY_begin(8, cpp_type, _ptr);
#endif
				} else if (ni.is_array || (ni.is_pointer == 1 && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr)) {
				//} else if (ni.is_array) {
					SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: struct: array");
					try_case = TRY_CASE_RPTR;
#if (GENERATE_TRY_CATCH == 1)
					sprintf_s(cpp_type, sizeof(cpp_type), "%s*", ni.uq_type);
					TRY_begin(8, cpp_type, _ptr);
					Printf(CPP, "(%s) ", cpp_type);
#else
					sprintf_s(cpp_type, sizeof(cpp_type), "%s", ni.cpp_type);
					TRY_begin(8, cpp_type, _ptr);
#endif
				} else {
					SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: struct");
					try_case = TRY_CASE_RPTR;
					sprintf_s(cpp_type, sizeof(cpp_type), "%s*", ni.uq_type);
					TRY_begin(8, cpp_type, _ptr);
					Printf(CPP, "new %s();", ni.uq_type);
					TRY_cont(8);
					Printf(CPP, "(*_ptr) = ");
				}
			}
			//else if (ni.is_array || (ni.is_pointer == 1 && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr)) {
			else if (ni.is_array) {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: array");
				//int isptr = ni.is_pointer;
				//int ptrlevel = ni.pointer_level;
				//ni.is_pointer = 1;
				//ni.pointer_level = 1;
				////try_case = TRY_CASE_RPTR;
				try_case = TRY_CASE_RCNV;
				TRY_begin(8, ni.uq_type, _ptr);
				//ni.is_pointer = isptr;
				//ni.pointer_level = ptrlevel;
			}
			else if (ni.is_pointer) {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: pointer");
				try_case = TRY_CASE_RPTR;
				TRY_begin(8, ni.cpp_type, _ptr);
			}
			else if (ni.is_string) {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: string");
				try_case = TRY_CASE_RCNV;
				TRY_begin_str(8, ni.uq_type, _val);
			}
			else {
				SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: function_return: others");
				try_case = TRY_CASE_RCNV;
				TRY_begin(8, ni.uq_type, _val);
			}
			
			//
			if (ni.is_void_ptr && ni.is_const) {
				Printf(CPP, "(void*)");
			}
			if (EQ(ni.kind, "typedef") && EQ(ni.storage, "typedef")) {
				Printf(CPP, "%s::%s(", ci.uq_name, ni.uq_name);
			}	
			else if (ni.is_static) {
				Printf(CPP, "%s::%s(", ci.uq_name, ni.uq_name);
			}
			else if (ni.is_vector || ni.is_array) {
				Printf(CPP, "((%s*) _this)->%s(", ci.uq_name, ni.uq_name);
			}
			else if (ni.is_struct) {
				if (is_enum_node) {
					Printf(CPP, "((%s*) _this)->%s(", ci.uq_name, ni.uq_name);
				}
				else if (ni.is_pointer) {
					Printf(CPP, "((%s*) _this)->%s(", ci.uq_name, ni.uq_name);
				}
				else {
					Printf(CPP, "((%s*) _this)->%s(", ci.uq_name, ni.uq_name);
				}
			}
			else {
				Printf(CPP, "((%s*) _this)->%s(", ci.uq_name, ni.uq_name);
			}
			for (int j = 0; j < ni.num_args; j++) {
				NodeInfo& ai = ni.funcargs[j];
				Printf(CPP, "%s", argnames[j]);
				if (j < ni.num_args - 1) Printf(CPP, ", ");
			}
			if (ni.is_vector) {
				Printf(CPP, "));\n");
				Printf(CPP, "              for (int n = 0; n < (int) _vec.size(); n++) {\n");
				Printf(CPP, "                  _ptr->push_back(_vec[n]);\n");
				Printf(CPP, "              }\n");
				Printf(CPP, "        }\n");
			} else if (ni.is_array) {
				Printf(CPP, "));\n");
				Printf(CPP, "              int _size = %s;\n", ni.array_size);
				Printf(CPP, "              _ptr = new %s[_size];\n", ni.cpp_type);
				Printf(CPP, "              for (int n = 0; n < (int) _size; n++) {\n");
				Printf(CPP, "                  *(_ptr+n) = _ary[n];\n");
				Printf(CPP, "              }\n");
				Printf(CPP, "        }\n");
			} else {
				Printf(CPP, ");");
				TRY_end();
			}

			CATCH(8);

			// 関数値に関する処理
			if (!ni.is_void) {
				PRINTinfo(gip, "returns: %s\n", ni.cpp_type);
				// vector or array
				if (ni.is_vector || ni.is_array) {
					WRAPPER_NAME_PRINT(fps, FD_CPP, __LINE__, ni, "function_return");
					Printf(CPP, "\treturn (HANDLE) _ptr;\n");
				}
				// C# に対応する型がある
				else if (ni.is_intrinsic) {
					// ポインタ型は HANDLE にして返す
					if (ni.is_pointer) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_return: intrinsic: pointer");
						Printf(CPP, "\treturn (HANDLE) _ptr;\n");
					}
					// bool（C++ bool <=> byte で受け渡し <=> C# bool）
					else if (ni.is_bool) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_return: intrinsic: bool");
						Printf(CPP, "\tchar _ret = _val ? 1 : 0;\n");
						Printf(CPP, "\treturn _ret;\n");
					}
					// 値または参照はそのまま返す
					else {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_return: intrinsic");
						Printf(CPP, "\treturn _val;\n");
					}
				}
				// string 型
				else if (ni.is_string) {
					// ポインタ型（BSTR にしてそのポインタを返す）
					if (ni.is_pointer) {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_return: string: pointer  ** NOT IMPLEMENTED **");
						Printf(CPP, "\tBSTR wstr = NULL;\n");
						generate_string_get(CPP, "\t", "wstr", NULL, "_val");
						Printf(CPP, "\treturn (HANDLE) &result;\n");
					}
					// BSTR にして返す
					else {
						SNAP_ANA_PATH1(fps, FD_CPP, "function_return: string");
						Printf(CPP, "\tBSTR wstr = NULL;\n");
						generate_string_get(CPP, "\t", "wstr", NULL, "_val");
						Printf(CPP, "\treturn (BSTR) wstr;\n");
					}
				}
				// struct 型
				else if (ni.is_struct) {
					PRINTF(fps, FD_CPP, "//_[%s: %d] %s\n", "function_return: struct", __LINE__, 
						is_enum_node ? " enum" : ni.is_pointer ? " pointer" : ni.is_reference ? " reference" : "");
					if (is_enum_node) {
						Printf(CPP, "\treturn (int) _val;\n");
					} else {
						Printf(CPP, "\treturn (HANDLE) _ptr;\n");
					}
				}
			}
			Printf(CPP, "    }\n");
#if (FREE_UNMANAGED_MEMORY == 1)
			if (ni.is_string) {
				Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_FreeString_%s%s(BSTR ptr) {\n", ci.uq_name, ni.uq_name, overname(ni));
				Printf(CPP, "        try { ::SysFreeString(ptr); }\n");
				Printf(CPP, "        %s\n", CATCH_code);
				Printf(CPP, "    }\n");
			}
#endif
			if (argnames) {
				delete argnames;
			}

			// [cs]
			//
			if (GetFlagAttr(members[i], "feature:only_cs_ignore")) {
				Printf(CS, "// feature:only_cs_ignore %s %s \n", ni.cs_name, ni.cs_type);
			}
			if (GetFlagAttr(members[i], "feature:only_cs_ignore") == NULL) {
				SNAP_ANA_PATH1(fps, FD_CS, "function: cdecl");
				argnames = NULL;
				cleanup1 = NULL;
				cleanup2 = NULL;
				if (ni.num_args > 0) {
					argnames = new char*[ni.num_args];
					cleanup1 = new void*[ni.num_args];	// clean up code for argument type string
					cleanup2 = new void*[ni.num_args];	// clean up code for argument type structure
					memset(cleanup1, 0, sizeof(void*) * ni.num_args);
					memset(cleanup2, 0, sizeof(void*) * ni.num_args);
				}
				// 関数が引数になっているときは前処理が必要
				for (int j = 0; j < ni.num_args; j++) {
					NodeInfo& ai = ni.funcargs[j];
					// 引数が関数でなければ前処理は不要
					if (!ai.is_function) {
						continue;
					}
					string key(ci.uq_name);
					key.append(".");
					key.append(argname(ai.uq_name, j));
					if (delegate_key_map.find(key) != delegate_key_map.end()) {
						const char* type = delegate_type_map[key].c_str();
						const char* func = delegate_func_map[key].c_str();
						const char* args = delegate_args_map[key].c_str();
						Printf(CS, "\tpublic delegate %s delegate_func_%d_%d%s\n", type, function_count, j + 1, args);
						Printf(CS, "\tdelegate_func_%d_%d %s_%d_%d = %s;\n", function_count, j + 1, argname(ai.uq_name, j), function_count, j + 1, func);
					}
				}

				// 関数定義
				if (ni.is_vector || ni.is_array) {
					char* wrapper_class = make_wrapper_name(fps, FD_CS, __LINE__, ni, ci, "function_return_type");
					Printf(CS, "\tpublic %s%s", (ni.is_static ? "static" : ""), wrapper_class);

					if (ni.is_struct || ni.is_array) {
						if (wrapper_map.find(wrapper_class) == wrapper_map.end()) {
							generate_wrapper_accessor_struct(topnode, members[i], ni, ci, "function_return_type", __LINE__);
							wrapper_map[wrapper_class] = 1;
						}
					}
				}
				else {
					Printf(CS, "\tpublic %s%s", (ni.is_static ? "static " : ""), ni.cs_type);
					if (ni.is_pointer && !ni.is_void_ptr && !EQ(ni.uq_type, "char") && !is_special_struct(ni.cs_type)) {
						//DUMP_NODE_INFO(fps, FD_CS, "function definition: ni", ni);
						Printf(CS, "[]");
					}
				}
				Printf(CS, " %s(", ni.cs_name);
				// 引数並び
				for (int j = 0; j < ni.num_args; j++) {
					NodeInfo& ai = ni.funcargs[j];
					char* uqname = argname(ai.uq_name, j);
					char* csname = argname(ai.cs_name, j);
					string key(ci.uq_name);
					key.append(".");
					key.append(uqname);
					if (ai.is_function && (delegate_func_map.find(key) != delegate_func_map.end())) {
						// 関数
						Printf(CS, "delegate_func_%d_%d %s_%d_%d", function_count, j + 1, uqname, function_count, j + 1);
					}
					/**
									else if (ai.is_struct && !ai.is_pointer && !ai.is_reference) {
										// struct
										Printf(CS, "%s %s", cs_qualified_name(ai.uq_type), csname);
									}
					**/
					else if (ai.is_vector || ai.is_array) {
						// vector or array
						char* wrapper_name = make_wrapper_name(fps, FD_NULL, __LINE__, ai, ci, "function_args");
						Printf(CS, "%s %s", wrapper_name, csname);
					}
					/**/
					else if (ai.is_struct && !ai.is_pointer && !ai.is_reference) {
						// struct
						Printf(CS, "%s %s", cs_qualified_name(ai.uq_type), csname);
					}
					/**/
					else if (ai.is_void_ptr) {
						Printf(CS, "CsObject %s", csname);
					}
					else {
						Printf(CS, "%s %s", ai.cs_type, csname);
					}
					if (j < ni.num_args - 1) Printf(CS, ", ");
					argnames[j] = csname;
				}
				Printf(CS, ") {\n");
				// 引数に関する前処理
				SNAP_ANA_PATH1(fps, FD_CS, "function_prep");
				//DUMP_NODE_INFO(fps, FD_CS, "function_prep", ni);
				for (int j = 0; j < ni.num_args; j++) {
					NodeInfo& ai = ni.funcargs[j];
					char tmpbuff[MAX_NAMELEN + 6 + 1];
					if (ai.is_string) {
						SNAP_ANA_PATH1(fps, FD_CS, "function_args: string");
						sprintf(tmpbuff, "remote%d", j + 1);
						Printf(CS, "            IntPtr %s = Marshal.StringToBSTR(%s);\n", tmpbuff, argname(ai.uq_name, j));
						argnames[j] = Char(NewString(tmpbuff));
						cleanup1[j] = argnames[j];
					}
					else if (delegate_func_map.find(cs_qualified_name(ai.uq_type)) != delegate_func_map.end()) {
						SNAP_ANA_PATH1(fps, FD_CS, "function_args: function");
					}
					/*
					else if (ai.is_struct && !ai.is_pointer && !ai.is_reference) {
						SNAP_ANA_PATH1(fps, FD_CS, "function_args: struct");
						sprintf(tmpbuff, "remote%d", j+1);
						Printf(CS, "            IntPtr %s = Marshal.AllocHGlobal(Marshal.SizeOf(%s));\n", tmpbuff, argname(ai.uq_name, j));
						Printf(CS, "            Marshal.StructureToPtr(%s, %s, false);\n", argname(ai.uq_name, j), tmpbuff);
						argnames[j] = Char(NewString(tmpbuff));
						cleanup2[j] = argnames[j];
					}
					*/
				}
				// 関数本体
				SNAP_ANA_PATH1(fps, FD_CS, "function_body");
				is_enum_node = (ni.is_struct) ? FindNodeByAttrR(topnode, "enumtype", ni.type) : NULL;
				if (ni.is_bool) {
					Printf(CS, "\t    char ret = ");
				}
				else if (ni.is_string) {
					Printf(CS, "\t    IntPtr ptr = ");
				}
				else if (ni.is_vector) {
					Printf(CS, "\t    IntPtr ptr = ");
				}
				else if (ni.is_array || (ni.is_pointer == 1 && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr)) {
					// 組み込み型のポインタを返す関数は配列を返すものとして扱う
					SNAP_AT_LINE(fps, FD_CS);
					Printf(CS, "\t    IntPtr ptr = ");
				}
				else if (ni.is_array) {
					SNAP_AT_LINE(fps, FD_CS);
					Printf(CS, "\t    IntPtr ptr = ");
				}
				else if (ni.is_struct) {
					if (is_enum_node) {
						Printf(CS, "\t    int result = ");
					}
					else {
						Printf(CS, "\t    IntPtr ptr = ");
					}
				}
				else if (ni.is_pointer && EQ(ni.uq_type, "char")) {
					Printf(CS, "\t    IntPtr ptr = ");
				}
				else if (ni.is_intrinsic && !ni.is_void) {
					if (ni.is_pointer && !ni.is_void_ptr) {
						Printf(CS, "\t    %s[] result = ", ni.cs_type, ni.cs_type);
					}
					else {
						Printf(CS, "\t    %s result = (%s) ", ni.cs_type, ni.cs_type);
					}
				}
				else if (!ni.is_void) {
					Printf(CS, "\t    %s result = (%s)(Object) ", ni.cs_type, ni.cs_type);
				}
				else {
					Printf(CS, "\t    ");
				}
				Printf(CS, "SprExport.Spr_%s_%s%s(", ci.uq_name, ni.uq_name, overname(ni));
				// 引数並び
				sep_needed = 0;
				if (!ni.is_static) {
					Printf(CS, "(IntPtr) _this");
					sep_needed = 1;
				}
				for (int j = 0; j < ni.num_args; j++) {
					NodeInfo& ai = ni.funcargs[j];
					Node* is_enum_node_a = FindNodeByAttrR(topnode, "enumtype", ai.type);
#if (DUMP == 1)
					if (is_enum_node_a) {
						//NodeInfo& ei = get_node_info(fps, is_enum_node);
						//DUMP_NODE_INFO(fps, FD_CS, "argtype is enum", ei);
					}
#endif
					if (sep_needed) Printf(CS, ", ");
					if (is_enum_node_a) { Printf(CS, "(int) "); }
					else if (ai.is_struct) { Printf(CS, "(IntPtr) "); }
					else if (ai.is_string) { Printf(CS, "(IntPtr) "); }
					else if (ai.is_vector || ai.is_array) { Printf(CS, "(IntPtr) "); }
					else { Printf(CS, "(%s) ", ai.cs_im_type); }
					if (ai.is_vector || ai.is_array)
						Printf(CS, "%s._this", argnames[j]);
					else
						Printf(CS, "%s", argnames[j]);
					sep_needed = 1;
				}
				Printf(CS, ");\n");
				// 作業変数の解放
				if (argnames) delete argnames;
				for (int j = 0; j < ni.num_args; j++) {
					if (cleanup1[j]) Printf(CS, "            Marshal.FreeBSTR(%s);\n", cleanup1[j]);
					if (cleanup2[j]) Printf(CS, "            Marshal.FreeHGlobal(%s);\n", cleanup2[j]);
				}
				if (cleanup1) delete cleanup1;
				if (cleanup2) delete cleanup2;
				// 関数戻り値のための後処理
				if (ni.is_bool) {
					SNAP_ANA_PATH1(fps, FD_CS, "function_return: intrinsic: bool");
					Printf(CS, "\t    return (ret == 0) ? false : true;\n");
				}
				else if (ni.is_pointer && EQ(ni.uq_type, "char")) {
					Printf(CS, "\t    return Marshal.PtrToStringAnsi(ptr);\n");
				}
				else if (ni.is_string) {
					SNAP_ANA_PATH1(fps, FD_CS, "function_return: string");
					Printf(CS, "            string bstr = Marshal.PtrToStringBSTR(ptr);\n");
					Printf(CS, "            if (bstr.Substring(bstr.Length - 1, 1) == \"\\0\") {\n");
					Printf(CS, "                    bstr = bstr.Substring(0, bstr.Length - 1);\n");
					Printf(CS, "            }\n");
#if (FREE_UNMANAGED_MEMORY == 1)
					Printf(CS, "            SprExport.Spr_%s_FreeString_%s(ptr);\n", ci.uq_name, ni.uq_name);
#endif
					Printf(CS, "            return bstr;\n");
				}
				else if (ni.is_vector) {
					SNAP_ANA_PATH1(fps, FD_CS, "function_return: vector");
					char* wrapper_name = make_wrapper_name(fps, FD_CS, __LINE__, ni, ci, "function_return");
					Printf(CS, "            return new %s(ptr);\n", wrapper_name);
				}
				else if (ni.is_array || (ni.is_pointer == 1 && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr)) {
					// 組み込み型のポインタを返す関数は配列を返すものとして扱う
					//	配列として扱うのでwrapper名に"_p"は付けない
					SNAP_ANA_PATH1(fps, FD_CS, "function_return: array");
					int pointer_level_save = ni.pointer_level;
					ni.pointer_level = 0;
					char* wrapper_name = make_wrapper_name(fps, FD_CS, __LINE__, ni, ci, "function_return");
					ni.pointer_level = pointer_level_save;
#if (CAN_GET_RETURNED_ARRAY_SIZE == 1)
					Printf(CS, "            int size = 0;  //Set some appropreate code\n");
#else
					Printf(CS, "            int size = 1;   // no way to know array size!\n");
#endif	/* CAN_GET_RETURNED_ARRAY_SIZE */
					Printf(CS, "            return new %s(ptr, size);\n", wrapper_name);
				}
				else if (ni.is_struct) {
					if (is_enum_node) {
						SNAP_ANA_PATH1(fps, FD_CS, "function_return: enum");
						Printf(CS, "            return (%s) result;\n", cs_qualified_name(unqualified_name(ni.type)));
					}
					else if (ENDWITH(ni.cs_type, "If")) {
						Printf(CS, "            if (ptr == IntPtr.Zero) { return null; } \n", ni.cs_type, ni.cs_type);
						Printf(CS, "            %s obj = new %s(ptr);\n", ni.cs_type, ni.cs_type);
						for (int j = 0; j < (int)childClassMap[ni.cs_type].size(); j++) {
							Printf(CS, "            if (obj.GetIfInfo() == %s.GetIfInfoStatic()) { return new %s(ptr); }\n", childClassMap[ni.cs_type][j].c_str(), childClassMap[ni.cs_type][j].c_str());
						}
						Printf(CS, "            return obj;\n");
					}
					else if (ni.is_pointer) {
						SNAP_ANA_PATH1(fps, FD_CS, "function_return: struct pointer");
						// 構造体変数のポインタなのか構造体配列へのポインタなのかは区別がつかない
						//   →  要素数１の構造体配列へのポインタとして扱う
						//	 構造体配列であった場合は２番目以降の要素にアクセスすることはできない
						if (is_special_struct(ni.cs_type)) {
							Printf(CS, "            return new %s(ptr);\n", ni.cs_type);
						}
						else {
#if (CAN_GET_RETURNED_ARRAY_SIZE == 1)
							Printf(CS, "            int size = 0;  //Set some appropreate code\n");
#else
							Printf(CS, "            int size = 1;    // no way to know array size\n");
#endif	/* CAN_GET_RETURNED_ARRAY_SIZE */
							Printf(CS, "            return new arraywrapper_%s(ptr, size);\n", ni.cs_type);
						}
						//Printf(CS, "            return new %s(ptr);\n", ni.cs_type);
					}
					else {
						SNAP_ANA_PATH1(fps, FD_CS, "function_return: struct");
						DUMP_NODE_INFO(fps, FD_CS, "function_return: struct", ni);
						Printf(CS, "            return new %s(ptr, true);\n", ni.cs_type);
					}
				}
				else if (!ni.is_void) {
					SNAP_ANA_PATH1(fps, FD_CS, "function_return: non-void");
					Printf(CS, "\t    return result;\n");
				}
				Printf(CS, "\t}\n");
			}
			// [csp]
			//
			// 関数宣言
			SNAP_ANA_PATH1(fps, FD_CSP, "function: cdecl");
			Printf(CSP, "\t%s\n", DLLIMPORT);
			//
			if (ni.is_vector) {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: vector");
				Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s%s(", ci.uq_name, ni.uq_name, overname(ni));
			}
			else if (ni.is_array || (ni.is_pointer == 1 && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr)) {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: array");
				Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s%s(", ci.uq_name, ni.uq_name, overname(ni));
			}
			else if (ni.is_string || (ni.is_pointer && EQ(ni.uq_type, "char"))) {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: string");
				Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s%s(", ci.uq_name, ni.uq_name, overname(ni));
			}
			else if (is_enum_node) {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: enum");
				Printf(CSP, "\tpublic static extern int Spr_%s_%s%s(", ci.uq_name, ni.uq_name, overname(ni));
			}
			else if (ni.is_pointer && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr) {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: pointer");
				Printf(CSP, "\tpublic static extern %s[] Spr_%s_%s%s(", ni.cs_type, ci.uq_name, ni.uq_name, overname(ni));
			}
			else if (ni.is_pointer && !ni.is_void_ptr) {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: pointer");
				Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s%s(", ci.uq_name, ni.uq_name, overname(ni));
			}
			else {
				SNAP_ANA_PATH1(fps, FD_CSP, "function_return: ");
				char* cpp_return_type = (ni.is_bool) ? (char*) "char" : ni.cs_im_type;
				Printf(CSP, "\tpublic static extern %s Spr_%s_%s%s(", cpp_return_type, ci.uq_name, ni.uq_name, overname(ni));
			}
			// 引数並び
			sep_needed = 0;
			if (!ni.is_static) {
				Printf(CSP, "IntPtr _this");
				sep_needed = 1;
			}
			for (int j = 0; j < ni.num_args; j++) {
				NodeInfo& ai = ni.funcargs[j];
				char* csname = argname(ai.cs_name, j);
				Node* is_enum_node_a = FindNodeByAttrR(topnode, "enumtype", ai.type);
				if (sep_needed) Printf(CSP, ", ");
				if (is_enum_node_a)	{ Printf(CSP, "%s %s", "int" /*ai.cs_type*/, csname); }
				else if (ai.is_struct)	{ Printf(CSP, "IntPtr %s", csname); }
				else if (ai.is_string)	{ Printf(CSP, "IntPtr %s", csname); }
				else if (ai.is_vector || ai.is_array)	{ Printf(CSP, "IntPtr %s", csname); }
				else			{ Printf(CSP, "%s %s", ai.cs_im_type, csname); }
				sep_needed = 1;
			}
			Printf(CSP, ");\n");
#if (FREE_UNMANAGED_MEMORY == 1)
			if (ni.is_string) {
				Printf(CSP, "\t%s\n", DLLIMPORT);
				Printf(CSP, "\tpublic static extern void Spr_%s_FreeString_%s%s(IntPtr ptr);\n", ci.uq_name, ni.uq_name, overname(ni));
			}
#endif
		}

		// IfImp にも enum がある
		//
		if (!class_already_defined) {
			Nodes enums;
			FindNodeR(enums, n, "enum");
			for (unsigned i = 0; i < enums.size(); ++i) {
				char* name = cs_qualified_name(Char(Getattr(enums[i], "type")));
				if (!name || !BEGINWITH(name, "enum ")) continue;
				name += 5;
				if (EQ(name, "")) continue;
				FP_generate_enum_def(fps, FD_CS, __LINE__, "\t", enums[i], NULL, "IfImp");
			}
		}

		if (!class_already_defined) {
			Printf(CS,  "    }\n");
		}
	}

	char* argname(char* name, int n) {
		if (name) return name;
		char buff[17];
		sprintf(buff, "arg%02d_", (n+1));
		return Char(NewString(buff));
	}

	Node* FindNodeByAttrR(Node* node, char* attr, char* name) {
		Node* child = firstChild(node);
		while (child) {
			if (Cmp(nodeType(child), "classforward")) {
				if (Cmp(Getattr(child, attr), name) == 0) {
					return child;
				}
				Node* cc = FindNodeByAttrR(child, attr, name);
				if (cc) {
					return cc;
				}
			}
			child = nextSibling(child);
		}
		return NULL;
	}

	void FindNodeR(Nodes& rv, Node* n, char* type/*, int level = 0*/) {
		if (Cmp(nodeType(n), type) == 0) {
			rv.push_back(n);
		}
		n = firstChild(n);
		while(n){
			FindNodeR(rv, n, type/*, level+1*/);
			n = nextSibling(n);
		}
	}

	string resolve_inheritance(DOHFile* fps[], Node* top, Node* n, Nodes& parents) {
		Nodes direct;
		DOH* baselist = Getattr(n, "baselist");
		for (int i = 0; i < DohLen(baselist); i++) {
			DOHString* b = DohGetitem(baselist, i);
			if (b) {
				if (is_if_class(fps, n, Char(b))) {
					Node* node = FindNodeByAttrR(top, "sym:name", Char(b));
					if (node) direct.push_back(node);
				}
			}
		}

		const char* name = Char(Getattr(n, "name"));
		bool is_desc  = ENDWITH(name, "Desc");
		bool is_state = ENDWITH(name, "State");

		if (direct.size() == 0) {
			// 継承なし
			return string();
		}
		else if (direct.size() == 1) {
			// 直接の継承元は一つのみ
			(void) resolve_inheritance(fps, top, direct[0], parents);
			const char* pname = Char(Getattr(direct[0], "name"));
			bool is_p_desc  = ENDWITH(pname, "Desc");
			if (is_desc && is_p_desc) {
				parents.push_back(direct[0]);
			}
			return Char(Getattr(direct[0], "name"));
		}
		else if (is_desc) {
			for (unsigned int i = 0; i < direct.size(); i++) {
				const char* pname = Char(Getattr(direct[i], "name"));
				bool is_p_desc  = ENDWITH(pname, "Desc");
				bool is_p_state = ENDWITH(pname, "State");
				if (is_p_desc) {
					// Desc 継承リストに加える
					(void) resolve_inheritance(fps, top, direct[i], parents);
					parents.push_back(direct[i]);
				} else if (is_p_state) {
					// C#　の継承元は State を継承元とする
					(void) resolve_inheritance(fps, top, direct[i], parents);
					return pname;
				}
			}
		}
		else if (is_state) {
			for (unsigned int i = 0; i < direct.size(); i++) {
				const char* pname = Char(Getattr(direct[i], "name"));
				bool is_p_state = ENDWITH(pname, "State");
				if (is_p_state) {
					// State は State のみ継承可
					(void) resolve_inheritance(fps, top, direct[i], parents);
					return pname;
				} else {
					Printf(stderr, "Error: %s inherits %s\n", name, pname);
				}
			}
		}
		return string();
	}

	bool is_if_class(DOHFile* fps[], Node* n, char* name) {
		Node* pn = parentNode(n);
		while (pn) {
			if (Cmp(nodeType(pn), "include") == 0) {
				char* pname = Char(Getattr(pn, "name"));
				if (strstr(pname, "\\include\\")) {
					//PRINTF(fps, FD_CS, "// class [%s] is defined in \"%s\"\n", Char(Getattr(n, "sym:name")), pname);
					return true;
				}
			}
			pn = parentNode(pn);
		}
		return false;
	}

	bool is_special_struct(char* cs_type) {
		string s(cs_type);
		if (s.substr(s.size()-2, 2) == "If")
			return true;
		//
		const char* names[] = { "IfInfo", "FWApp", "PHCollisionListener" };
		int size = sizeof(names) / sizeof(const char*);
		for (int n = 0; n < size; n++) {
			if (EQ(cs_type, names[n]))
				return true;
		}
		return false;
	}

#if (ADOPT_TEMPLATE_CLASS == 1)
	void AddTemplateClassR(Node* node, map<string, Node*>& cmap, map<string, Nodes>& fmap, map<string, TemplateClassInfo*>& tmap) {
		while (node) {
			if (Cmp(nodeType(node), "template") == 0) {
				string* name = new string(Char(Getattr(node, "name")));
				char* kind = Char(Getattr(node, "kind"));
				bool inc = EQc(kind, "class") || EQc(kind, "struct");
				if (inc && (cmap.find(*name) == cmap.end())) {
					Nodes funclist = FindTemplateClassMember(node, cmap, fmap, tmap);
					//vector<TemplateClassInfo*> instances = find_template_instances(node, *name);
					if (cmap.find(*name) == cmap.end()) {
						cmap[*name] = node;
						vector<TemplateClassInfo*> instances = find_template_instances(node, *name);
						for (unsigned int i = 0; i < instances.size(); i++) {
							string instance_name = instances[i]->instance_name;
							cmap[instance_name] = node;
							fmap[instance_name] = funclist;
							tmap[instance_name] = instances[i];
						}
#if (GATHER_INFO == 1)
						PRINTinfo(gip, "template: add: %s %s\n", kind, name->c_str());
						for (unsigned int i = 0; i < instances.size(); i++) {
							string instance_name = instances[i]->instance_name;
							PRINTinfo(gip, "template: add: %s %s -> %s\n", kind, name->c_str(), instance_name.c_str());
						}
#endif
					}
				}
			}
			Node* child = firstChild(node);
			while (child) {
				AddTemplateClassR(child, cmap, fmap, tmap);
				child = nextSibling(child);
			}
			node = nextSibling(node);
		}
	}

	Nodes FindTemplateClassMember(Node* node, map<string, Node*>& cmap, map<string, Nodes>& fmap, map<string, TemplateClassInfo*> tmap) {
		Nodes members;
		Node* child = firstChild(node);
		while (child) {
			if (Cmp(nodeType(child), "template") == 0) {
				AddTemplateClassR(child, cmap, fmap, tmap);
			} else {
				char* name = NON_NULL(Char(Getattr(child, "name")));
				char* kind = NON_NULL(Char(Getattr(child, "kind")));
				bool inc = (!BEGINWITH(name, "operator")) && EQ(kind, "function");
				if (inc) {
#if (GATHER_INFO == 1)
					char* type = NON_NULL(Char(Getattr(child, "type")));
					char* access = NON_NULL(Char(Getattr(child, "access")));
					PRINTinfo(gip, "template: add:     %s %s(%s) %s\n", kind, name, type, access);
#endif
					members.push_back(child);
				}
			}
			child = nextSibling(child);
		}
		return members;
	}

	vector<TemplateClassInfo*> find_template_instances(Node* node, string name) {
		vector<string> types = {"int", "float", "double"};
		vector<TemplateClassInfo*> instances;
		TemplateClassInfo* tci = new TemplateClassInfo;
		for (unsigned int i = 0; i < types.size(); i++) {
			string* key = new string(name);
			(*key).append("<").append(types[i]).append(">");
			if (typedef_inverse_map.find(*key) != typedef_inverse_map.end()) {
				string* instance = new string(unqualified_name((char*) typedef_inverse_map[*key].c_str()));
				string* tname = new string(name);
				string* itype = new string(types[i].c_str());
				tci->template_node = node;
				tci->template_name = (char*) tname->c_str();
				tci->instance_name = (char*) instance->c_str();
				Node* tpn = Getattr(node, "templateparms");
				tci->temp_param = Char(Getattr(tpn, "name"));
				tci->inst_param = (char*) itype->c_str();
				tci->inst_param_ch = tci->inst_param[0];
				instances.push_back(tci);
			}
		}
		return instances;
	}

	void replace_template_parameter(NodeInfo& ni, TemplateClassInfo& tci) {
		replace_template_parameter_1(ni, tci);
		for (int i = 0; i < ni.num_args; i++) {
			NodeInfo& ai = ni.funcargs[i];
			replace_template_parameter_1(ai, tci);
		}
	}

	void replace_template_parameter_1(NodeInfo& ni, TemplateClassInfo& tci) {
		ni.type = replace_template_parameter_2(ni, ni.type, tci.temp_param, tci.inst_param);
		ni.uq_type = replace_template_parameter_2(ni, ni.uq_type, tci.temp_param, tci.inst_param, true);
		ni.cpp_type = replace_template_parameter_2(ni, ni.cpp_type, tci.temp_param, tci.inst_param);
		ni.cs_type = replace_template_parameter_2(ni, ni.cs_type, tci.temp_param, tci.inst_param, true);
		ni.cs_im_type = replace_template_parameter_2(ni, ni.cs_im_type, tci.temp_param, tci.inst_param);
	}

	char* replace_template_parameter_2(NodeInfo& ni, char* var, char* from, char* to, bool need_uq = false) {
		string* v_str = replace_template_parameter_string(var, from, to);
		if (typedef_inverse_map.find(*v_str) != typedef_inverse_map.end()) {
			v_str = replace_template_parameter_string((char*) v_str->c_str(), "<(", "<");
			v_str = replace_template_parameter_string((char*) v_str->c_str(), ")>", ">");
			*v_str = typedef_inverse_map[*v_str];
			if (need_uq) {
				*v_str = string(unqualified_name((char*) v_str->c_str()));
			}
		}
		TypeConv* tct = is_basic_type(*v_str);
		if (tct != NULL) {
			ni.is_intrinsic = 1;
			ni.is_struct = 0;
			ni.cs_im_type = tct->cs_type;
			ni.cs_marshaltype = tct->cs_marshaltype;
			analyze_cs_type_hook(&ni);
		}
		return (char*) v_str->c_str();
	}

	string* replace_template_parameter_string(char* var, char* from, char* to) {
		string* v_str = new string(var);
		int pos = (int) v_str->find(from);
		while (pos >= 0) {
			v_str->replace(pos, strlen(from), to);
			pos = (int) v_str->find(from);
		}
		return v_str;
	}

	TypeConv* is_basic_type(string type) {
		char* tname = (char*) type.c_str();
		for (TypeConv* tct = type_conv; tct->kind > 0; tct++) {
			if (tct->kind != 1)	continue;
			if (strstr(tname, "<"))	continue;
			if (strstr(tname, tct->cpp_type) || (tct->cs_type && strstr(tname, tct->cs_type))) {
				return tct;
			}
		}
		return NULL;
	}
#endif // ADOPT_TEMPLATE_CLASS

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
		Replaceall(cppfile, "_wrap.cxx", "Cs.cpp");
		String *csfile = NewString(Getattr(top, "outfile"));
		Replaceall(csfile, "_wrap.cxx", "Cs.cs");
		String *cspfile = NewString(Getattr(top, "outfile"));
		Replaceall(cspfile, "_wrap.cxx", "CsP.cs");

		cpp = NewFile(cppfile, "w", NULL);
		if (!cpp) {
			FileErrorDisplay(cppfile);
			SWIG_exit(EXIT_FAILURE);
		}
		cs = NewFile(csfile, "w", NULL);
		if (!cs) {
			FileErrorDisplay(csfile);
			SWIG_exit(EXIT_FAILURE);
		}
		csp = NewFile(cspfile, "w", NULL);
		if (!csp) {
			FileErrorDisplay(cspfile);
			SWIG_exit(EXIT_FAILURE);
		}
		fps[0] = cpp;
		fps[1] = cs;
		fps[2] = csp;

#if (GATHER_INFO == 1)
		String *infofile = NewString(Getattr(top, "outfile"));
		Replaceall(infofile, "_wrap.cxx", "Cs.info");
		gip = NewFile(infofile, "w", NULL);
		if (!gip) {
			FileErrorDisplay(gip);
			SWIG_exit(EXIT_FAILURE);
		}
		//DumpNode(gip, top);
		//PrintTemplateNodeR(top);
#endif
		// take wrapper class name previously generated
		signature_map.clear();
#if (USE_SIGNATURE_FILE == 1)
		sig_ifs.open(SIGNATURE_FILE);
		while (1) {
			char inbuff[1024];
			sig_ifs.getline(inbuff, sizeof(inbuff));
			if (sig_ifs.eof()) break;
			string key = trim(string(inbuff));
			if (key.front() == '#') {
				PRINTinfo(gip, "signature: init: skip ... %s\n", key.c_str()); 
				continue;
			}
			if (signature_map.find(key) == signature_map.end()) {
				signature_map[key] = 1;
			}
		}
		sig_ifs.close();
#if (GATHER_INFO == 1)
		for (auto itr = signature_map.begin(); itr != signature_map.end(); itr++) {
			PRINTinfo(gip, "signature: init: %s\n", itr->first.c_str());
		}
#endif
		sig_ofs.open(SIGNATURE_FILE, ios_base::app);
		if (sig_ofs.good()) {
			char buff[MAX_NAMELEN+5];
			sprintf(buff, "#[%s]\n", Char(Getattr(top, "name")));
			sig_ofs.write(buff, strlen(buff));
		}
#endif	// USE_SIGNATURE_FILE

#if (GATHER_INFO == 1)
		Nodes includes;
		FindNodeR(includes, top, "include");
		for (int i = 0; i < (int) includes.size(); i++) {
			Node* inc = includes[i];
			PRINTinfo(gip, "include: %s\n", Char(Getattr(inc, "name")));
		}
#endif

		Nodes cdecls;
		FindNodeR(cdecls, top, "cdecl");
		for (int i = 0; i < (int) cdecls.size(); i++) {
			Node* inc = cdecls[i];
			if (Cmp(Getattr(inc, "kind"), "typedef") == 0) {
				NodeInfo& ti = get_node_info(fps, inc);
				//DUMP_NODE_INFO(fps, FD_INFO, "typedef", ti);
				string* key = new string(ti.name);
				string* str;
				if (ti.is_vector) {
					str = new string("vector<(");
					str->append(ti.uq_type);
					str->append(")>");
				} else {
					str = new string(ti.uq_type);
				}
				typedef_map[*key] = *str;
				typedef_inverse_map[*str] = *key;
			}
		}
		// Kludge
		typedef_map["UTString"] = "std::string";
		typedef_map["GLuint"] = "unsigned int";
		typedef_map["FWApp::Timers"] = "std::vector<(UTRef<(UTTimerIf)>)>";
		typedef_map["PHEngines::base::iterator"] = "std::vector<(UTRef<(PHEngine)>)>::iterator";
		typedef_map["PHEngines::base::const_iterator"] = "std::vector<(UTRef<(PHEngine)>)>::const_iterator";
		//
#if (GATHER_INFO == 1)
		for (auto itr = typedef_map.begin(); itr != typedef_map.end(); itr++) {
			PRINTinfo(gip, "typedef: def: %s -> %s\n", itr->first.c_str(), itr->second.c_str());
		}
		for (auto itr = typedef_inverse_map.begin(); itr != typedef_inverse_map.end(); itr++) {
			PRINTinfo(gip, "typedef: inv: %s -> %s\n", itr->first.c_str(), itr->second.c_str());
		}
#endif

#if (ADOPT_TEMPLATE_CLASS == 1)
		// template class 情報を集める
		AddTemplateClassR(top, template_class_map, template_function_map, template_class_info_map);
#if (GATHER_INFO == 1)
		for (auto itr = template_class_map.begin(); itr != template_class_map.end(); itr++) {
			PRINTifno(gip, "template_class_map: class: %s\n", itr->first.c_str());
		}
		for (auto itr = template_function_map.begin(); itr != template_function_map.end(); itr++) {
			Nodes vec = itr->second;
			PRINTinfo(gip, "template_function_map: class: %s\n", itr->first.c_str());
			for (unsigned int i = 0; i < vec.size(); i++) {
				PRINTinfo(gip, "template_function_map:   %s\n", Char(Getattr(vec[i], "sym:name")));
			}
		}
#endif
#endif // ADOPT_TEMPLATE_CLASS

		Nodes modules;
		FindNodeR(modules, top, "module");

		// ----- ----- ----- ----- -----
		// [cpp]
		//
		Printf(cpp, "#include <Springhead.h>\n");
		Printf(cpp, "#include <Windows.h>\n");
		Printf(cpp, "#include <sstream>\n");
		Printf(cpp, "#include <Spr%s.h>\n", Getattr(modules.front(), "name"));
		Printf(cpp, "#include <Framework/SprFWApp.h>\n");
		Printf(cpp, "#include <Physics/SprPHJointMotor.h>\n");
		Printf(cpp, "#include <Framework/SprFWEditor.h>\n");
		Printf(cpp, "#include <Framework/SprFWOpObj.h>\n");
		Printf(cpp, "#include <Framework/FWScene.h>\n");
#if (GENERATE_TRY_CATCH == 1)
		Printf(cpp, "#include <../src/SprCSharp/SprExport/CSUtility.h>\n");
#endif
		Printf(cpp, "using namespace Spr;\n");
		Printf(cpp, "using namespace std;\n");
		Printf(cpp, "extern \"C\" {\n");

		// [cs]
		//
		SNAP_ANA_PATH1(fps, FD_CS, "module");
		Printf(cs, "using System;\n");
		Printf(cs, "using System.Collections.Generic;\n");
		Printf(cs, "using System.Linq;\n");
		Printf(cs, "using System.Text;\n");
		Printf(cs, "using System.Runtime.InteropServices;\n");
		Printf(cs, "#pragma warning disable 0108\n");
		/**
		Nodes nss;
		FindNodeR(nss, top, "namespace");
		for (unsigned int i = 0; i < nss.size(); i++) {
			Node* ns = nss[i];
			String* name = Getattr(ns, "name");
			if (Cmp(name, "std") && Cmp(name, "Spr")) {
				Printf(cs,  "using %s;\n", Getattr(ns, "name"));
			}
		}
		**/
		Printf(cs,  "namespace SprCs {\n");

		// [csp]
		//
		SNAP_ANA_PATH1(fps, FD_CSP, "module");
		Printf(csp, "using System;\n");
		Printf(csp, "using System.Runtime.InteropServices;\n");
		Printf(csp, "namespace SprCs {\n");
		Printf(csp, "    public partial class SprExport {\n");

		// ----- ----- ----- ----- -----

		Nodes incs;
		FindNodeR(incs, top, "include");
		Nodes classes;
		Nodes externClasses;
		for(unsigned i=0; i<incs.size(); ++i){
			string key("\\");
			key.append(GetString( Getattr(top, "name")) );
			if (Strstr(Getattr(incs[i], "name"), key.c_str())){
				FindNodeR(classes, incs[i], "class");
			} else if (!Strstr(Getattr(incs[i], "name"), ".i")){
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
			if( GetFlagAttr(n,"feature:ignore") ) { ALERT(n,"ignored:'%%ignore'"); continue; }
			while(cn){
				if( GetFlagAttr(cn,"feature:ignore") ) { ALERT(cn,"ignored:'%%ignore'"); cn = nextSibling(cn); continue; }
				if ((Cmp(nodeType(cn), "constructor") == 0) || (Cmp(nodeType(cn), "cdecl") == 0)) {
					if (Strstr(Getattr(cn, "name"), "SPR_OBJECTDEF")){
						objs.push_back(n);
						objDefs.push_back(cn);
						if (Strstr(Getattr(cn, "name"), "SPR_OBJECTDEF_ABST")){
							absts.push_back(true);
						}else{
							absts.push_back(false);
						}
						break;
					/***
					} else if (Strstr(Getattr(cn, "name"), "SPR_IFDEF")){
					***/
					} else if (ENDWITH(Char(Getattr(n, "name")), "If") || GetFlagAttr(n, "feature:struct")) {
						char* name = Char(Getattr(n, "name"));
						if (intf_map.find(name) == intf_map.end()) {
							intfs.push_back(n);
							intf_map[name] = 1;

							Strings bases;
							GetBaseList(bases, Getattr(n, "baselist"), "If");
							for (unsigned j = 0; j < bases.size(); j++) {
								string basetypename = bases[j] + "If";
								childClassMap[basetypename].push_back(string(name + 5));
								// std::cout << basetypename << " <-- " << string(name + 5) << std::endl;
							}
						}
						break;
					} else if (Strstr(Getattr(cn, "name"), "SPR_VIFDEF")) {
						char* name = Char(Getattr(n, "name"));
						if (vintf_map.find(name) == vintf_map.end()) {
							vintfs.push_back(n);
							vintf_map[name] = 1;
						}
						break;
					}
				}
				mark_function_generation(n, cn);
				cn = nextSibling(cn);
			}
			if (!cn && ((Cmp(Getattr(n, "kind"), "struct") == 0) || Cmp(Getattr(n, "kind"), "class") == 0)) {
				char* name = Char(Getattr(n, "name"));
				if (class_map.find(name) == class_map.end()) {
					class_map[name] = 1;
					descs.push_back(n);
					PRINTinfo(gip, "class: %s\n", Char(Getattr(n, "name")));
				}
			}
			Nodes enums;
			FindNodeR(enums, n, "enum");
			for (int i = 0; i < (int) enums.size(); ++i) {
				char* name = cs_qualified_name(Char(Getattr(enums[i], "type")));
				if (!name || EQ(name, "")) continue;
				if (EQ(name, "enum ")) continue;
				name += 5;
				if ((enum_map.find(name) == enum_map.end())
					 && (class_map.find(name) == class_map.end()) && (intf_map.find(name) == intf_map.end())) {
					enum_map[name] = 1;
					//descs.push_back(n);
					PRINTinfo(gip, "enum: %s\n", name);
				}
			}
			// Vec3f,Vec3d のように float/double のペアとなっているクラスを見つける
			char* name = unqualified_name(Char(Getattr(n, "name")));
			if (!ENDWITH(name, "If")) {
				char last_ch = name[strlen(name)-1];
				if (last_ch == 'f' || last_ch == 'd') {
					name[strlen(name)-1] = '\0';
					if (fd_pair_map.find(name) != fd_pair_map.end()) {
						fd_pair_map[name] += 1;
					} else {
						fd_pair_map[name] = 1;
					}
					//PRINTinfo(gip, "pair_candidate: %s %c\n", name, last_ch);
				}
			}
			// ---- ---- ---- ----
			// .i ファイルで定義された %feature:operator:macro 情報をここで収集する
			void* attrline = GetFlagAttr(n, "feature:operator:macro");
			if (attrline) {
				string macroline = strip_whites(Char(attrline));
				vector<string> macros = split(macroline, ')');
				for (unsigned int i = 0; i < macros.size(); i++) {
					vector<string> macro = split(macros[i], '(');
					if (macro.size() != 2) {
						abort(__LINE__, "bad macro definition: %s", macros[i].c_str());
					}	
					string name = macro[0];
					string def = macro[1];
					if (operator_macros_map.find(name) == operator_macros_map.end()) {
						operator_macros_map[name] = def;
					}
				}
			}
		}
#if (GATHER_INFO == 1)
		for (auto itr = fd_pair_map.begin(); itr != fd_pair_map.end(); itr++) {
			PRINTinfo(gip, "FD_pair: (%d) %s\n", itr->second, (itr->first).c_str());
		}
		for (auto itr = operator_macros_map.begin(); itr != operator_macros_map.end(); itr++) {
			PRINTinfo(gip, "operator_macro defined: %s = %s\n", (itr->first).c_str(), (itr->second).c_str());
		}
#endif
		for(unsigned i=0; i<externClasses.size(); ++i){
			Node* n = externClasses[i];
			Node* cn = firstChild(n);
			if( GetFlagAttr(n,"feature:ignore") ) { ALERT(n,"ignored:'%%ignore'"); continue; }
			while(cn){
				if( GetFlagAttr(cn,"feature:ignore") ) { ALERT(cn,"ignored:'%%ignore'"); cn = nextSibling(cn); continue; }
				if ( Cmp(nodeType(cn), "constructor") == 0){
					if (Strstr(Getattr(cn, "name"), "SPR_OBJECTDEF")){
						externalObjs.push_back(n);
						break;
					}
				}
				cn = nextSibling(cn);
			}
		}

#if (GATHER_INFO == 1)
		print_nodes_info("DESCs", descs, "name");
		print_nodes_info("EXTNs", externalObjs, "name");
		print_nodes_info("INTFs", intfs, "name");
		print_nodes_info("VINTFs", vintfs, "name");
#endif
#if (DUMP_TREE == 1 && GATHER_INFO == 1)
		dump_tree(top);
#endif
		// Collect class node and its parent information.
		for(unsigned i=0; i<descs.size(); ++i) {
			NodeInfo& ci = get_node_info(fps, descs[i]);
			if (class_node_map.find(ci.sym_name) == class_node_map.end()) {
				class_node_map[ci.sym_name] = ci;
				// get parent list
				Nodes parents;
				DOH* baselist = Getattr(descs[i], "baselist");
				for (int i = 0; i < DohLen(baselist); i++) {
					DOHString* b = DohGetitem(baselist, i);
					if (b) {
						if (is_if_class(fps, descs[i], Char(b))) {
							Node* node = FindNodeByAttrR(top, "sym:name", Char(b));
							if (node) parents.push_back(node);
						}
					}
				}
				// get direct parent
				char* parent_name = NULL;
				if (parents.size() > 0) {
					parent_name = Char(Getattr(parents[0], "sym:name"));
				}
				class_parent_map[ci.sym_name] = parent_name;
				//PRINTF(fps, FD_ALL, "//_[class list: %d] %s : %s\n", __LINE__, ci.sym_name, (parent_name != NULL) ? parent_name : "");
			}
		}
		// ----- ----- ----- ----- -----
		for(unsigned i=0; i<descs.size(); ++i){
			string csname = Char(Getattr(descs[i], "sym:name"));
			bool both_desc_and_if = (function_generation_map.find(csname) != function_generation_map.end());
			DescImp(fps, top, descs[i], both_desc_and_if);
		}
		for(unsigned i=0; i<externalObjs.size(); ++i){
			string csname = Char(Getattr(externalObjs[i], "sym:name"));
			bool both_desc_and_if = (function_generation_map.find(csname) != function_generation_map.end());
			DescImp(fps, top, externalObjs[i], both_desc_and_if);
		}

#if (ADOPT_TEMPLATE_CLASS == 1)
		// ----- ----- ----- ----- -----
		for (auto itr = template_class_map.begin(); itr != template_class_map.end(); itr++) {
			string cname = itr->first;
			if (template_function_map.find(cname) != template_function_map.end()) {
				DescImp(fps, top, itr->second, true, (char*) itr->first.c_str());
			}
		}
#endif

		// ----- ----- ----- ----- -----
		// 子クラスマップを孫以下に拡大
		bool updated = true;

		while (updated) {
			updated = false;
			for (std::map<string, std::vector<string> >::iterator it = childClassMap.begin(); it != childClassMap.end(); it++) {

				for (int i = 0; i < (int) it->second.size(); i++) {
					string child = it->second[i];
					for (int j = 0; j < (int) childClassMap[child].size(); j++) {
						string grandChild = childClassMap[child][j];

						bool found = false;
						for (int k = 0; k < (int) it->second.size(); k++) {
							if (it->second[k] == grandChild) {
								found = true;
								break;
							}
						}

						if (!found) {
							it->second.push_back(grandChild);
							updated = true;
						}
					}
				}
			}
		}

		/* （デバッグ用表示）
		std::cout << "  -----  " << std::endl;
		for (std::map<string, std::vector<string> >::iterator it = childClassMap.begin(); it != childClassMap.end(); it++) {
			std::cout << it->first << " <-- ";
			for (int i = 0; i < it->second.size(); i++) {
				std::cout << it->second[i] << ", ";
			}
			std::cout << std::endl;
		}
		std::cout << "  -----  " << std::endl;
		*/

		// ----- ----- ----- ----- -----
		for(unsigned i=0; i<intfs.size(); ++i){
			IfImp(fps, top, intfs[i], false);
		}
		for(unsigned i=0; i<vintfs.size(); ++i){
			IfImp(fps, top, vintfs[i], false);
		}

		// ----- ----- ----- ----- -----
		Printf(cs, "\tpublic partial class IfInfoToCsType {\n");
		Printf(cs, "\t\tpublic static Dictionary<IntPtr, Type> map%s = new Dictionary<IntPtr, Type>() {\n", Getattr(modules.front(), "name"));
		for (unsigned i = 0; i<intfs.size(); ++i){
			TypeMapImp(cs, top, intfs[i]);
		}
		for (unsigned i = 0; i<vintfs.size(); ++i){
			TypeMapImp(cs, top, vintfs[i]);
		}
		Printf(cs, "\t\t};\n");
		Printf(cs, "\t}\n");


#ifdef	ORIGINAL_CODE
		// ----- ----- ----- ----- -----

		/*
		Printf(cpp, "\nvoid RegisterTypeDesc%s(UTTypeDescDb* db){\n", Getattr(modules.front(), "name"));
		Printf(cpp, "	static bool bFirst=true;\n");
		Printf(cpp, "	if (!bFirst) return;\n");
		Printf(cpp, "	bFirst = false;\n\n");

		Printf(cpp, "	UTTypeDesc* desc;\n");
		Printf(cpp, "	UTTypeDesc::Field* field;\n" );
		for(unsigned i=0; i<descs.size(); ++i){
			TypeDesc(cpp, hpp, descs[i]);
		}
		Printf(cpp, "}\n\n");
		//	SPR_IFIMP
		for(unsigned i=0; i<intfs.size(); ++i){
			IfImp(cpp, sprh, intfs[i], false);
		}
		for(unsigned i=0; i<vintfs.size(); ++i){
			IfImp(cpp, sprh, vintfs[i], false);
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
		*/
#endif	// ORIGINAL_CODE

		Printf(cpp, "}\n");	// extern "C"
		Printf(cs,  "}\n");	// namespace SprCs
		Printf(csp, "    }\n");	// partial class
		Printf(csp, "}\n");	// namespace SprCs

		//	ツリーのダンプを出力
	/***
		char filename[] = "swig_sprcs_\0                       ";
		strcat(filename, Char(Getattr(modules.front(), "name")));
		strcat(filename, ".log");
		log = NewFile(filename, "w", NULL);
	***/
		string filename("swig_sprcs_");
		filename.append(Char(Getattr(modules.front(), "name")));
		filename.append(".log");
		log = NewFile((char*)filename.c_str(), "w", NULL);
		if (!log) {
			FileErrorDisplay((char*) "swig_sprcs.log");
			SWIG_exit(EXIT_FAILURE);
		}
		DumpNode(log, top);
		if (errorFlag){
			SWIG_exit(EXIT_FAILURE);
		}
#if (USE_SIGNATURE_FILE == 1)
		sig_ofs.close();
#endif	// USE_SIGNATURE_FILE
		return SWIG_OK;
	}

	void create_wrapper_accessor_file(DOHFile* fps[3], Node* top, char* cname) {
		// ------------------------------------------------------------------------------
		//  出力するディレクトリは、".i" ファイルのあるディレクトリからの相対で指定する。
		//
		char* wf_outdir[3] = { "SprExport\\tmp", "SprCSharp\\tmp", "SprImport\\tmp" };
		// ------------------------------------------------------------------------------
		char* fname[3];
		fname[0] = make_wrapper_tmp_filename(top, wf_outdir[0], cname, "cpp");
		fname[1] = make_wrapper_tmp_filename(top, wf_outdir[1], cname, "cs");
		fname[2] = make_wrapper_tmp_filename(top, wf_outdir[2], cname, "cs");
		for (int n = 0; n < 3; n++) {
			fps[n] = NewFile(fname[n], "w", NULL);
			if (!fps[n]) {
				FileErrorDisplay(fname[n]);
				SWIG_exit(EXIT_FAILURE);
			}
		}
	}

	char* make_wrapper_tmp_filename(Node* top, char* dir, char* cname, char* suffix) {
		string ofile(Char(Getattr(top, "outfile")));
		int pos = (int) ofile.find_last_of("/\\");
		string* fname = NULL;
		if (pos >= 0) {
			fname = new string(ofile.substr(0, pos));
		} else {
			fname = new string(".");
		}
		fname->append("\\");
		fname->append(dir);
		fname->append("\\");
		fname->append(cname);
		fname->append(".");
		fname->append(suffix);
		return (char*) fname->c_str();
	}

	void mark_function_generation(Node* pn, Node* cn) {
		char* pname = Char(Getattr(pn, "sym:name"));
		char* cname = Char(Getattr(cn, "sym:name"));
		char* oname = Char(Getattr(cn, "sym:overname"));
		if (cname == NULL)		return;
		string* psname = new string(pname);
		char* ctype = Char(nodeType(cn));
		char* ckind = Char(Getattr(cn, "kind"));
		ctype = ctype ? ctype : (char*) "";
		if (ENDWITH(pname, "If"))	return;
		if (ENDWITH(pname, "Dedc"))	return;
		if (GetFlagAttr(pn, "feature:struct"))	return;
		if (EQc(ctype, "constructor"))	return;
		if (!EQc(ckind, "function"))	return;
		//
		if (function_generation_map.find(*psname) == function_generation_map.end()) {
			function_generation_map[*psname] = 1;
		}
#if (GATHER_INFO == 1)
		if (function_generation_map.find(*psname) != function_generation_map.end()) {
			PRINTinfo(gip, "Has nonIF function: %s, %s %s, %s, %s\n", pname, cname, oname, ctype, ckind);
		}
#endif
	}

	virtual void main(int argc, char *argv[]) {
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
#ifdef	ORIGINAL_CODE
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
#endif	// ORIGINAL_CODE
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

	//=============================================================
	//
	void generate_new_class_struct(DOHFile* fps[], Node* topnode, Node* node, char* name, char* uq_name, bool feature) {
		bool need_apply[2];
		need_apply[0] = false;	// need generate ApplyTo/ApplyFrom
		need_apply[1] = false;	// need apply to base classes
		if (feature) {
			need_apply[0] = true;
		} else {
			if (ENDWITH(uq_name, "If")) return;
			need_apply[0] |= ENDWITH(uq_name, "Desc");
			need_apply[0] |= ENDWITH(uq_name, "State");
			if (ENDWITH(uq_name, "Material")) { need_apply[0] = true; }
			if (!need_apply[0] && !(ENDWITH(uq_name, "f") || ENDWITH(uq_name, "d"))) return;
		}

		StructInfo* struct_info = get_struct_info(fps, topnode, name);
		int is_newable = is_newable_class(node) && !is_virtual(struct_info->name);
		if (is_newable == 0) return;

		// 継承元クラスのリストを取得する (Desc/State のみ)
		SNAP_ANA_PATH2(fps, FD_CS, "generate: class", uq_name);
		Nodes parents;
		string inherit = resolve_inheritance(fps, topnode, node, parents);
		std::reverse(parents.begin(), parents.end());
		Strings bases;
		if (!inherit.empty()) {
			bases.push_back(unqualified_name((char*) inherit.c_str()));
		}
		if (!inherit.empty() && parents.size() > 0) {
			char* inherit_name = unqualified_name((char*) inherit.c_str());
			char* directp_name = unqualified_name(Char(Getattr(parents[0], "name")));
			if (EQ(inherit_name, directp_name)) {
				need_apply[1] = true;
			}
		}
#ifdef	SNAP
		PRINTF(fps, FD_CS, "// resolve inheritance:\n");
		PRINTF(fps, FD_CS, "//     direct inherit..: %s\n", inherit.empty() ? "" : bases[0].c_str());
		PRINTF(fps, FD_CS, "//     desc parent list:");
		for (unsigned int i = 0; i < parents.size(); i++) {
			if (i > 0) PRINTF(fps, FD_CS, ",");
			char* name = Char(Getattr(parents[i], "sym:name"));
			PRINTF(fps, FD_CS, " %s", name ? name : "(null)");
		}
		PRINTF(fps, FD_CS, "\n");
#endif

		// [cs]
		Printf(CS, "    [System.Serializable]\n");
		Printf(CS, "    public class %sStruct", name);
		if (bases.empty()) {
			Printf(CS, " : CsObject");
		} else {
			Printf(CS, " : ");
 			for (int i = 0; i < (int) bases.size(); ++i) {
				if (i > 0) Printf(CS, ", ");
				Printf(CS, "%sStruct", bases[i].c_str());
			}
		}
		Printf(CS,  " {\n", name);

		// change name to wrapper name if needed
		NodeInfo& ci = get_node_info(fps, node);
		for (int i = 0; i < struct_info->num_members; i++) {
			StructMembersInfo* mp = struct_info->members[i];
			NodeInfo& ni = get_node_info(fps, mp->node);
			//DUMP_NODE_INFO(fps, FD_CS, "generate new class struct", ni);
			if (ni.is_vector || ni.is_array) {
				char* wrapper_type = make_wrapper_name(fps, FD_CS, __LINE__, ni, ci, "new class struct");
				mp->cs_type = wrapper_type;
			}
		}

		// generate constructors
		SNAP_ANA_PATH1(fps, FD_CS, "generate: class: constructor");
		Printf(CS, "        public %sStruct() {\n", name);
		Printf(CS, "            %s desc = new %s();\n", name, name);
		Printf(CS, "            ApplyFrom(desc, %s);\n", need_apply[1] ? "true" : "false");
		Printf(CS, "        }\n");

		// generate members
		SNAP_ANA_PATH1(fps, FD_CS, "generate: class: members");
		map<string, int> name_duplicated_map;
		for (int i = 0; i < struct_info->num_members; i++) {
			StructMembersInfo* mp = struct_info->members[i];
			NodeInfo& ni = get_node_info(fps, mp->node);
			Node* is_enum_node = FindNodeByAttrR(topnode, "enumtype", ni.type);
			if (mp->is_struct && !(mp->is_array || mp->is_vector || is_enum_node)) {
				Printf(CS, "\tpublic %sStruct %s;\n", mp->cs_type, mp->cs_name);
			} else if (mp->is_vector) {
				std::string cpp_type = std::regex_replace(mp->cpp_type, std::regex(" "), "_");
                        	Printf(CS, "\tpublic vectorwrapper_%s %s = new vectorwrapper_%s();\n", cpp_type.c_str(), mp->cs_name, cpp_type.c_str());
			} else if (mp->is_array) {
				std::string cpp_type = std::regex_replace(mp->cpp_type, std::regex(" "), "_");
                        	Printf(CS, "\tpublic arraywrapper_%s %s = new arraywrapper_%s(%s);\n", cpp_type.c_str(), mp->cs_name, cpp_type.c_str(), ni.array_size);
                	} else {
                    		Printf(CS, "\tpublic %s %s;\n", mp->cs_type, mp->cs_name);
			}
			string key(mp->cs_name);
			name_duplicated_map[key] = 1;
		}

		/******* DO NOT GENERATE !! ********
		// generate accessors to parent classes
		if (!parents.empty()) {
			SNAP_ANA_PATH1(fps, FD_CS, "generate: class: accsessors to parent class");
			for (unsigned int i = 0; i < parents.size(); i++) {
				generate_accessors_for_parents(fps, topnode, parents[i], name_duplicated_map);
			}
		}
		*******/

		// generate type conversion operators
		need_apply[0] = true;
		generate_type_conversion_operators(fps, topnode, struct_info, need_apply, name, parents);
		//
		int len = strlen(name);
		char* name_tmp = new char[len+1];
		strcpy(name_tmp, name);
		char last_ch = name_tmp[strlen(name_tmp)-1];
		name_tmp[strlen(name_tmp)-1] = '\0';
		if (fd_pair_map.find(name_tmp) != fd_pair_map.end()) {
			if ((fd_pair_map[name_tmp] == 2) && (last_ch == 'f' || last_ch == 'd')) {
				generate_type_conversion_operator(fps, topnode, name, name_tmp, last_ch, "Struct");
			}
		}
		delete name_tmp;

		Printf(CS,  "    }\n");
	}

	/*******
	void generate_size_and_pushback(DOHFile* fps[], Node* node, char* name, char* uq_name, char* cpp_name) {
		Printf(CS, "// @@ name: %s, uq_name: %s, cpp_name: %s\n", name, uq_name, cpp_name);
	}
	*******/

	void generate_type_conversion_operator(DOHFile* fps[], Node* topnode, char* complete_name, char* name, char from_type, char* suffix = "") {
		StructInfo* struct_info = get_struct_info(fps, topnode, complete_name);
		if (from_type == 'f') {
			SNAP_ANA_PATH1(fps, FD_CS, "generate: type conversion operator: f to d");
			Printf(CS, "\tpublic static implicit operator %sd%s(%sf%s f) {\n", name, suffix, name, suffix);
			Printf(CS, "\t    %sd%s d = new %sd%s();\n", name, suffix, name, suffix);
			generate_type_conversion_operator_copy_members(cs, struct_info, "d", "f");
			Printf(CS, "\t    return d;\n");
			Printf(CS, "\t}\n");
		} else {
			SNAP_ANA_PATH1(fps, FD_CS, "generate: type conversion operator: d to f");
			Printf(CS, "\tpublic static explicit operator %sf%s(%sd%s d) {\n", name, suffix, name, suffix);
			Printf(CS, "\t    %sf%s f = new %sf%s();\n", name, suffix, name, suffix);
			generate_type_conversion_operator_copy_members(cs, struct_info, "f", "(float) d");
			Printf(CS, "\t    return f;\n");
			Printf(CS, "\t}\n");
		}
		free_struct_info(struct_info);
	}

	void generate_type_conversion_operators(DOHFile* fps[], Node* topnode, StructInfo* struct_info, bool need_apply[2], char* name, Nodes parents) {
		if (need_apply[0]) {	// need generate ApplyTo/ApplyFrom
			SNAP_ANA_PATH1(fps, FD_CS, "generate: type conversion operators");
  			DUMP_STRUCT_INFO(fps, FD_CS, struct_info);
			char* pname = NULL;
			if (class_parent_map.find(name) != class_parent_map.end()) {
				pname = class_parent_map[name];
			}
			//
			Printf(cs, "// _[Find class node: %d] %s\n", __LINE__, name);
			StructInfo* class_struct_info = get_struct_info(fps, topnode, name);
  			DUMP_STRUCT_INFO(fps, FD_CS, class_struct_info);

			// ApplyTo (XX <- XXStruct)
			Printf(cs, "\tpublic void ApplyTo(%s r, bool apply_to_base_class=true) {\n", name);
			for (int i = 0; i < class_struct_info->num_members; i++) {
				StructMembersInfo* mp = class_struct_info->members[i];
				if (mp->is_vector) {
					Printf(cs, "\t\tr.%s.clear();\n", mp->cs_name);
					Printf(cs, "\t\tint _%s_count_%04d = %s.size();\n", name, i, mp->cs_name);
					Printf(cs, "\t\tfor (int i = 0; i < _%s_count_%04d; i++) {\n", name, i);
					Printf(cs, "\t\t\tr.%s.push_back(%s[i]);\n", mp->cs_name, mp->cs_name);
					Printf(cs, "\t\t}\n");
				} else {
					Printf(cs, "\t\tr.%s = %s;\n", mp->cs_name, mp->cs_name);
				}
			}
			if (pname != NULL && need_apply[1]) {	// need apply to base classes
				Printf(cs, "\t\tif (apply_to_base_class) {\n");
				Printf(cs, "\t\t    base.ApplyTo((%s) r, apply_to_base_class);\n", pname);
				Printf(cs, "\t\t}\n");
			}
			Printf(cs, "\t}\n");

			// ApplyFrom (XXStruct <- XX)
			Printf(cs, "\tpublic void ApplyFrom(%s r, bool apply_to_base_class=true) {\n", name);
			for (int i = 0; i < struct_info->num_members; i++) {
				StructMembersInfo* mp = struct_info->members[i];
				if (mp->is_vector) {
					Printf(cs, "\t\t%s.clear();\n", mp->cs_name);
					Printf(cs, "\t\tint _%s_count_%04d = r.%s.size();\n", name, i, mp->cs_name);
					Printf(cs, "\t\tfor (int i = 0; i < _%s_count_%04d; i++) {\n", name, i);
					Printf(cs, "\t\t\t%s.push_back(r.%s[i]);\n", mp->cs_name, mp->cs_name);
					Printf(cs, "\t\t}\n");
				} else {
					Printf(cs, "\t\t%s = r.%s;\n", mp->cs_name, mp->cs_name);
				}
			}
			if (pname != NULL && need_apply[1]) {	// need apply to base classes
				Printf(cs, "\t\tif (apply_to_base_class) {\n");
				//Printf(cs, "\t\t    base.ApplyFrom((%sStruct) r, apply_to_base_class);\n", pname);
				Printf(cs, "\t\t    base.ApplyFrom((%s) r, apply_to_base_class);\n", pname);
				Printf(cs, "\t\t}\n");
			}
			Printf(cs, "\t}\n");
			Printf(cs, "\tpublic static implicit operator %s(%sStruct m) {\n", name, name);
			Printf(cs, "\t    %s r = new %s();\n", name, name);
			Printf(cs, "\t    m.ApplyTo(r, true);\n");
			Printf(cs, "\t    return r;\n");
			Printf(cs, "\t}\n");

			Printf(cs, "\tpublic static implicit operator %sStruct(%s r) {\n", name, name);
			Printf(cs, "\t    %sStruct m = new %sStruct();\n", name, name);
			Printf(cs, "\t    m.ApplyFrom(r, true);\n");
			Printf(cs, "\t    return m;\n");
			Printf(cs, "\t}\n");
		} else {
			SNAP_ANA_PATH1(fps, FD_CS, "generate: type conversion operators: not Desc");
  			DUMP_STRUCT_INFO(fps, FD_CS, struct_info);
			Printf(cs, "\tpublic static implicit operator %s(%sStruct m) {\n", name, name);
			Printf(cs, "\t    %s r = new %s();\n", name, name);
			generate_type_conversion_operator_copy_members(cs, struct_info, "r", "m");
			Printf(cs, "\t    return r;\n");
			Printf(cs, "\t}\n");

			Printf(cs, "\tpublic static implicit operator %sStruct(%s r) {\n", name, name);
			Printf(cs, "\t    %sStruct m = new %sStruct();\n", name, name);
			generate_type_conversion_operator_copy_members(cs, struct_info, "m", "r");
			Printf(cs, "\t    return m;\n");
			Printf(cs, "\t}\n");
		}
	}

	void generate_type_conversion_operator_copy_members(DOHFile* cs, StructInfo* struct_info, char* to, char* from) {
		for (int i = 0; i < struct_info->num_members; i++) {
			StructMembersInfo* mp = struct_info->members[i];
			Printf(cs, "\t    %s.%s = %s.%s;\n", to, mp->cs_name, from, mp->cs_name);
		}
	}

	void generate_new_class(DOHFile* fps[], Node* topnode, Node* node, Strings bases, char* suffix, char* name, char* uq_name, char* cpp_name) {
		StructInfo* struct_info = get_struct_info(fps, topnode, name);
		int is_newable = is_newable_class(node) && !is_virtual(struct_info->name);

		// class
		//
		// [cpp]
		SNAP_ANA_PATH2(fps, FD_CPP, "generate: class", name);
		// [cs]
		SNAP_ANA_PATH2(fps, FD_CS, "generate: class", name);
  		DUMP_STRUCT_INFO(fps, FD_CS, struct_info);
		//Printf(CS, "    [StructLayout(LayoutKind.Sequential,Pack=%d)]\n", ALIGNMENT);
		Printf(CS, "    public partial class %s", name);

		if (bases.size() > 0) {
			// UTRefCount は継承元には入れない
			std::vector<string> inherits;
 			for (int i = 0; i < (int) bases.size(); ++i) {
				if (!EQ(bases[i].c_str(), "UTRefCount")) {
					inherits.push_back(bases[i]);
				}
			}
			if (inherits.size() == 0) {
				Printf(CS, " : CsObject");
			} else {
				Printf(CS, " : ");
 				for (int i = 0; i < (int) inherits.size(); ++i) {
					if (i > 0) Printf(CS, ", ");
					Printf(CS, "%s%s", inherits[i].c_str(), suffix);
				}
			}
		} else {
			Printf(CS, " : CsObject");
		}
		Printf(CS,  " {\n", name);
		// [csp]
		//
		SNAP_ANA_PATH2(fps, FD_CSP, "generate: class", name);

		// constructors and destructor
		//
		if (is_newable) {
			// constructor
			generate_constructor(fps, node, name, uq_name, cpp_name, struct_info);

			// destructor
			// [cpp]
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_delete_%s(HANDLE v) {\n", uq_name);
			Printf(CPP, "        if (v == NULL) return;\n");
			Printf(CPP, "        try { delete (%s*) v; }\n", cpp_name);
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
			// [cs]
			Printf(CS, "\t~%s() { if (_flag) { SprExport.Spr_delete_%s(_this); _flag = false; } }\n", uq_name, uq_name);
			// [csp]
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_delete_%s(IntPtr _this);\n", uq_name);
		} else {
			generate_constructor(fps, node, name, uq_name, cpp_name, struct_info, 0 /*newable*/);
		}

		free_struct_info(struct_info);

		// generate type conversion operators
		int len = strlen(name);
		char* name_tmp = new char[len+1];
		strcpy(name_tmp, name);
		char last_ch = name_tmp[strlen(name_tmp)-1];
		name_tmp[strlen(name_tmp)-1] = '\0';
		if (fd_pair_map.find(name_tmp) != fd_pair_map.end()) {
			if ((fd_pair_map[name_tmp] == 2) && (last_ch == 'f' || last_ch == 'd')) {
				char conv_type = (last_ch == 'f') ? 'd' : 'f';
				generate_type_conversion_operator(fps, topnode, name, name_tmp, conv_type);
			}
		}
		delete name_tmp;

		SNAP_ANA_PATH1(fps, FD_CS, "GENERATE APPLYTO/FROM");


		// generate overload operators
		void* attrline = GetFlagAttr(node, "feature:operator:def");
		if (attrline != NULL) {
			//
			// attribute specification
			//	%feature("operator", def="spec[:spec]...") Spr::CLASS
			//	sepc:		op-sym,types,[func-name][,type[,type]]
			//	op-sym:		演算子記号（+,- など）
			//	types="r12":	"r12" はそれぞれ 演算結果,オペランド1,オペランド2 の型指定
			//	func-name:	演算を識別する名前（typesとfunc-nameで一意になること）【省略可】
			//	type:		types で S または O を指定したとき その具体的な型名
			//	
			//	"r12" に指定できるのもの
			//			'C'	この演算子を定義するクラス
			//			'E'	このクラスの要素の型（float, double など）
			//			'v'	Vec** クラス
			//			'm'	Matrix** クラス
			//			'q'	Quaternion* クラス
			//			'p'	Pose* クラス
			//			'S'	スカラ型（type に int, float, double, bool などと指定する）
			//			'O'	他のクラス（type に具体的に指定する）
			//			
			string defline = strip_whites(Char(attrline));
			vector<string> org_defs = split(defline, ':');
			vector<string> defs;

			// begin macro expansion
			map<string, int> dupe_check;
			for (unsigned int i = 0; i < org_defs.size(); i++) {
				string defsinfo = org_defs[i];
				defs = expand_operator_macro(defs, defsinfo, dupe_check);
			}
			// end macro expansion

			for (unsigned int i = 0; i < defs.size(); i++) {
				vector<string> specs = split(defs[i], ';');
				for (unsigned int j = 0; j < specs.size(); j++) {
					vector<string> fields = split(specs[j], ',');
					unsigned int num_fields = fields.size();
					if (num_fields < 2 || 5 < num_fields) {
						abort(__LINE__, "bad number of fileds: %s", specs[j].c_str());
					}
					if (num_fields == 2) {
						fields.push_back(".");	// when func name is omitted
						num_fields++;
					}
					char* symb = (char*) fields[0].c_str();	// <op-sym>
					char* type = (char*) fields[1].c_str();	// <types>
					char* func = (char*) fields[2].c_str();	// <func-name>
					if (EQ(func, ".") || (fields[2] == "")) {
						func = default_operator_func_name(symb, type);
					}
					bool binary_op = (strlen(type) == 3);
					unsigned int count = count_extra_fields_in_operator_def(type);
					if (num_fields != count + 3) {
						abort(__LINE__, "field count does not match with type 'E/S': %s", specs[j].c_str());
					}
					string rtrn_str = operator_type_analyzer(type[0], uq_name);
					string opr1_str = operator_type_analyzer(type[1], uq_name);
					string opr2_str;
					int list_ix = 3;
					if (rtrn_str == "*type-list*") rtrn_str = fields[list_ix++];
					if (opr1_str == "*type-list*") opr1_str = fields[list_ix++];
					char* rtrn = (char*) rtrn_str.c_str();
					char* opr1 = (char*) opr1_str.c_str();
					char* opr2 = "dummy";
					if (binary_op) {
						opr2_str = operator_type_analyzer(type[2], uq_name);
						if (opr2_str == "*type-list*") opr2_str = fields[list_ix++];
						opr2 = (char*) opr2_str.c_str();
					}
					SNAP_ANA_PATH2(fps, FD_ALL, "feature:operator:def", (void*) specs[j].c_str());
					generate_operator(fps, uq_name, symb, func, type, rtrn, opr1, opr2);
				}
			}
		}

		// generate ToString/print method
		int print_facility_type = has_print_facility(node);
		PRINTinfo(gip, "has print facility: %s (%s)\n", uq_name, print_facility_name(print_facility_type));
		generate_to_string(fps, uq_name, cpp_name, print_facility_type);
	}

	int is_newable_class(Node* node) {
		int newable = 1;
		Nodes constructors;
		FindNode(constructors, node, "constructor");
		for (int i = 0; i < (int) constructors.size(); i++) {
			NodeInfo& ci = get_node_info(fps, constructors[i]);
			if (EQc(ci.access, "private") || EQc(ci.access, "protected")) {
#if (DUMP == 1)
				Printf(cpp, "//class %s's constructor is %s!\n", (ci.sym_name ? ci.sym_name : ci.name), ci.access);
#endif
				newable = 0;
			}
		}
		//** Special trap **//
		if (EQc(Char(Getattr(node, "name")), "Spr::IfInfo")) newable = 0;
		//**//
		return newable;
	}

	int is_virtual(char* name) {
		static char* names[] = {
		    	"UTAccessBase",
		    	"UTTimerProvider",
		    	"FWSceneFactory",
		    	"FWSdkFactory",
		    	"DRUpp",
		    	"DVInputPort",
		    	"DVOutputPort",
		    	"DVOrientation",
		    	"HISpidarCalcBase",
		    	"HISpidarCalc3Dof",
		    	"HISpidarCalc6Dof",
		    	"CRSdkFactory",
			NULL
		};
		char** ptr = names;
		while (*ptr) {
			if (EQ(*ptr, name)) return 1;
			ptr++;
		}
		return 0;
	}

	void generate_constructor(DOHFile* fps[], Node* node, char* name, char* uq_name, char* cpp_name, StructInfo* struct_info, int newable = 1) {
		// default constructor は必ず作る
		//
		if (newable) {
			// [cpp]
			SNAP_ANA_PATH1(fps, FD_CPP, "generate: default constructor");
			SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: default constructor");
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_new_%s() {", uq_name);
			TRY_begin_new(8, HANDLE, _ptr);
			Printf(CPP, "new %s();", cpp_name);
			TRY_end_new();
			CATCH(8);
			RETURN_new(8, _ptr);

			// [cs]
			SNAP_ANA_PATH1(fps, FD_CS, "generate: default constructor");
			Printf(CS, "\tpublic %s() { _this = SprExport.Spr_new_%s(); _flag = true; }\n", name, uq_name);
#if (FREE_UNMANAGED_MEMORY == 1)
			Printf(CS, "\tpublic %s(IntPtr ptr) : base(ptr) {}\n", name);
			Printf(CS, "\tpublic %s(IntPtr ptr, bool flag) : base(ptr, flag) {}\n", name);
#else
			Printf(CS, "\tpublic %s(IntPtr ptr, bool flag = false) { _this = ptr; _flag = flag; }\n", name, uq_name);
#endif
			// [csp]
			SNAP_ANA_PATH1(fps, FD_CSP, "generate: default constructor");
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_new_%s();\n", uq_name);
		} else {
			// [cs]
			Printf(CS, "\tprotected %s() {}\n", name);
#if (FREE_UNMANAGED_MEMORY == 1)
			Printf(CS, "\tpublic %s(IntPtr ptr) : base(ptr) { }\n", name);
			Printf(CS, "\tpublic %s(IntPtr ptr, bool flag) : base(ptr, flag) { }\n", name);
#else
			Printf(CS, "\tpublic %s(IntPtr ptr, bool flag = false) { _this = ptr; _flag = flag; }\n", name, uq_name);
#endif
		}
		// Cast Operator
		if (ENDWITH(uq_name, "If")) {
			Printf(CS, "\tpublic static implicit operator %s(CsCastObject target) {\n\t\treturn (target._info.Inherit(%s.GetIfInfoStatic()) ? new %s(target._this, target._flag) : null);\n\t}\n", name, name, name);
		}

		// If クラスではこれ以上の constructor は作らない
		if (!ENDWITH(uq_name, "If")) {
               		int has_constructor = Cmp(Getattr(node, "allocate:has_constructor"), "1") == 0;
               		int public_constructor = Cmp(Getattr(node, "allocate:public_constructor"), "1") == 0;
               		// int copy_constructor = Cmp(Getattr(node, "allocate:copy_constructor"), "1") == 0;
               		// int has_destructor = Cmp(Getattr(node, "allocate:has_destructor"), "1") == 0;
               		// int default_destructor = Cmp(Getattr(node, "allocate:default_destructor"), "1") == 0;
#if (DUMP == 1)
			Printf(CS, "//  has constructors: %s, is public: %s\n", (has_constructor ? "yes" : "no"), (public_constructor ? "yes" : "no")); 
#endif
			// has_constructor が "1" で、ツリーにノードがあるものだけ作る
			// copy_coonstructor は無視する 
			if (has_constructor) {
				int count = 1;
				NodeInfo& ni = get_node_info(fps, node);
				Node* child = firstChild(node);
				while (child) {
					if (Cmp(Getattr(child, "nodeType"), "constructor") == 0) {
						NodeInfo& ci = get_node_info(fps, child);
						DUMP_NODE_INFO(fps, (FD_CPP | FD_CS), "constructor", ci);
						char* access = (char*) (public_constructor ? "public " : "");
#define	new_args(f,v)		string formal_args_##f((v)); string actual_args_##f((v))
#define	append_args_F1(f,t)	formal_args_##f.append((t))
#define	append_args_F2(f,t,n)	formal_args_##f.append((t)).append(" ").append((n))
#define	append_args_A(f,n)	actual_args_##f.append((n))
#define	append_args_both(f,s)	formal_args_##f.append((s)); actual_args_##f.append((s))
						new_args(cpp, "(");
						new_args(cs,  "(");
						new_args(csp, "(");
						for (int i = 0; i < ci.num_args; i++) {
							NodeInfo& ai = ci.funcargs[i];
							DUMP_NODE_INFO(fps, FD_CS, "constructor argument", ai);
							if (ai.is_vector || ai.is_array) {
								char* tmp = ai.cs_name ? ai.cs_name : (char*) "";
								//ai.pointer_level = 0;
								char* name = make_wrapper_name(fps, FD_CS, __LINE__, ai, ci, "formal arg");
								string str;
								if (ai.is_vector) {
									str.append("*((std::vector<").append(ai.cpp_type).append(">*) ").append(tmp).append(")");
								} else if (ai.is_reference) {
									str.append("((").append(ai.cpp_type).append(") ").append(tmp).append(")");
								} else if (ai.is_array) {
									str.append("((").append(ai.cpp_type).append("*) ").append(tmp).append(")");
								} else {
									str.append("*((").append(ai.cpp_type).append("*) ").append(tmp).append(")");
								}
								append_args_F2(cpp, "HANDLE", tmp);
								append_args_F2(cs,  name, tmp);
								append_args_F2(csp, "IntPtr", tmp);
								append_args_A(cpp, str);
								append_args_A(cs,  tmp);
								append_args_A(csp, tmp);
							}
							else if (ai.is_struct) {
								char* name = ai.cs_name ? ai.cs_name : (char*) "";
								string str;
								if (ai.is_reference) {
									str.append("((").append(ai.cpp_type).append(") ").append(name).append(")");
								} else {
									str.append("*((").append(ai.cpp_type).append("*) ").append(name).append(")");
								}
								append_args_F2(cpp, "HANDLE", name);
								if (ai.is_pointer) {
									append_args_F2(cs,  "IntPtr", name);
								} else {
									append_args_F2(cs,  ai.cs_type, name);
								}
								append_args_F2(csp, "IntPtr", name);
								append_args_A(cpp, str);
								append_args_A(cs,  name);
								append_args_A(csp, name);
							}
							else {
								char* type = (char*) (EQ(ai.cs_im_type, "IntPtr") ? "HANDLE" : ai.cpp_type);
								char* name = (char*) (ai.cs_name ? ai.cs_name : "");
								append_args_F2(cpp, type, name);
								if (ai.is_pointer && !EQc(ai.uq_type, "char")) {
									append_args_F2(cs,  "IntPtr", name);
									append_args_F2(csp, "IntPtr", name);
								} else {
									append_args_F2(cs,  ai.cs_im_type, name);
									append_args_F2(csp, ai.cs_im_type, name);
								}
								append_args_A(cpp, name);
								append_args_A(cs,  name);
								append_args_A(csp, name);
							}
							if (i < ci.num_args -1) {
								append_args_both(cpp, ", ");
								append_args_both(cs,  ", ");
								append_args_both(csp, ", ");
							}
						}
						append_args_both(cpp, ")");
						append_args_both(cs,  ")");
						append_args_both(csp, ")");

						if (ci.num_args > 0) {
							// [cpp]
							SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: constructor");
							Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_new_%s_%d%s {", struct_info->name, count, formal_args_cpp.c_str());
							TRY_begin_new(8, HANDLE, _ptr);
							Printf(CPP, "new %s%s;", uq_name, actual_args_cpp.c_str());
							TRY_end_new();
							CATCH(8);
							RETURN_new(8, _ptr);

							// [cs]
							Printf(CS, "\t%s%s%s { _this = SprExport.Spr_new_%s_%d%s; _flag = true; }\n", access, name, formal_args_cs.c_str(), uq_name, count, actual_args_cs.c_str());
							// [csp]
							Printf(CSP, "\t%s\n", DLLIMPORT);
							Printf(CSP, "\tpublic static extern IntPtr Spr_new_%s_%d%s;\n", uq_name, count, formal_args_csp.c_str());
						}
					}
					child = nextSibling(child);
					count++;
				}
			}
#if (GENERATE_OPT_CONSTRUCTOR == 1)
			if (need_generate_constructor(uq_name)) {
				// Kludge
				//	全メンバを引数とする constructor を生成する
				generate_constructor_with_full_members(fps, /*CPP, CS, CSP,*/ name, uq_name, struct_info);
			}
#endif
		}
	}

	int need_generate_constructor(char* name) {	// These are defined in "src/Foundation/UTBaseType.h".
		char* names[] = {
			"Vec2f", "Vec2d", "Vec3f", "Vec3d", "Vec4f", "Vec4d",
			"Quaternionf", "Quaterniond",
			"Posef", "Posed",
			"Matrix3f", "Matrix3d",
			"Vec2i", "Vec3i", "Vec6d",
			"Curve3f",
			NULL
		};
		for (char** p = names; *p; p++) {
			if (EQ(name, *p)) return 1;
		}
		return 0;
	}

	void generate_constructor_with_full_members(DOHFile* fps[], char* name, char* uq_name, StructInfo* struct_info) {
		if (struct_info->num_members > 0) {
			// [cpp]
			SNAP_ANA_PATH1(fps, FD_CPP, "generate: constructor: with full argument");
			SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: constructor: with full argument");
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_new_%s_99(", struct_info->name);
			for (int i = 0; i < struct_info->num_members; i++) {
				StructMembersInfo* mp = struct_info->members[i];
				if (mp->is_array || mp->is_vector) {
					Printf(CPP, "HANDLE %s", mp->cpp_name);
				}
				else {
					char* cpp_type = strip_type_modifier(mp->cpp_type);
					char* p = strchr(cpp_type, '<');
					if (p) *p = '\0';
					Printf(CPP, "%s %s", cpp_type, mp->cpp_name);
				}
				if (i < struct_info->num_members - 1) Printf(CPP, ", ");
			}
			Printf(CPP, ") {");
			TRY_begin_new(8, HANDLE, _ptr);
			Printf(CPP, "new %s(", name);
			for (int i = 0; i < struct_info->num_members; i++) {
				StructMembersInfo* mp = struct_info->members[i];
				if (mp->is_array || mp->is_vector) {
					Printf(CPP, "%s", mp->cpp_name);
				}
				else {
					Printf(CPP, "%s", mp->cpp_name);
				}
				if (i < struct_info->num_members - 1) Printf(CPP, ", ");
			}
			Printf(CPP, ");");
			TRY_end_new();
			CATCH(8);
			RETURN_new(8, _ptr);

			// [cs]
			SNAP_ANA_PATH1(fps, FD_CS, "generate: constructor: with full argument");
			Printf(CS, "\tpublic %s(", struct_info->name);
			for (int i = 0; i < struct_info->num_members; i++) {
				StructMembersInfo* mp = struct_info->members[i];
				char* cs_type = strip_type_modifier(mp->cs_type);
				char* p = strchr(cs_type, '<');
				if (p) *p = '\0';
				Printf(CS, "%s %s", mp->cs_type, mp->cs_name);
				if (i < struct_info->num_members - 1) Printf(CS, ", ");
			}
			Printf(CS, ") {\n");

			Printf(CS, "\t    _this = SprExport.Spr_new_%s_99(", name);
			for (int i = 0; i < struct_info->num_members; i++) {
				StructMembersInfo* mp = struct_info->members[i];
				Printf(CS, "%s", mp->cs_name);
				if (i < struct_info->num_members - 1) Printf(CS, ", ");
			}
			Printf(CS, ");\n");
			Printf(CS, "\t    _flag = true;\n");
			Printf(CS, "\t}\n");

			// [csp]
			SNAP_ANA_PATH1(fps, FD_CSP, "generate: constructor: with full argument");
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_new_%s_99(", uq_name);
			for (int i = 0; i < struct_info->num_members; i++) {
				StructMembersInfo* mp = struct_info->members[i];
				if (mp->is_array || mp->is_vector) {
					Printf(CSP, "IntPtr %s", mp->cpp_name);
				}
				else {
					char* cs_type = strip_type_modifier(mp->cs_type);
					char* p = strchr(cs_type, '<');
					if (p) *p = '\0';
					Printf(CSP, "%s %s", mp->cs_type, mp->cs_name);
				}
				if (i < struct_info->num_members - 1) Printf(CSP, ", ");
			}
			Printf(CSP, ");\n");
		}
	}

	vector<string> expand_operator_macro(vector<string> defs, string def_str, map<string, int>& dupe_check) {
		if (operator_macros_map.find(def_str) != operator_macros_map.end()) {
			string macro_body = operator_macros_map[def_str];
			if (dupe_check.find(def_str) != dupe_check.end()) {
				abort(__LINE__, "recursively defined macro: %s", def_str.c_str());
			}
			dupe_check[def_str] = 1;
			PRINTinfo(gip, "operator_macro expanded: %s = %s\n", def_str.c_str(), macro_body.c_str());
			vector<string> bodies = split(macro_body, ':');
			for (unsigned int j = 0; j < bodies.size(); j++) {
				string def_str_tmp = bodies[j];
				if (operator_macros_map.find(def_str_tmp) != operator_macros_map.end()) {
					defs = expand_operator_macro(defs, def_str_tmp, dupe_check);
				} else {
					defs.push_back(bodies[j]);
				}
			}
		} else {
			defs.push_back(def_str);
		}
		return defs;
	}

	void generate_indexer(DOHFile* fps[], char* cname, char* fname, char rtype, char* rname) {
		string str = string("unary_operator_") + string(fname);
		char* func = (char* ) str.c_str();
		bool is_ptr = (rtype != 'E' && rtype != 'S');

		// [CPP]
		char* rt = (char*) (is_ptr ? "HANDLE" : rname);
		SNAP_ANA_PATH1(fps, FD_CPP, "generate: indexer");
		if (is_ptr) {
			SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: indexer: pointer");
			Printf(CPP, "    __declspec(dllexport) %s __cdecl Spr_%s_get_%s(HANDLE ptr, int i) {\n", rt, func, cname);
			TRY_begin_NEW(8, rt, _ptr);
			Printf(CPP, "new %s((*((%s*) ptr))[i]);", rname, cname);
			TRY_end_NEW();
			CATCH(8);
			RETURN_NEW(8, _ptr);
			Printf(CPP, "    }\n");

			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE ptr1, int i, HANDLE ptr2) {\n", func, cname);
			TRY_begin_void(8);
			Printf(CPP, "(*((%s*) ptr1))[i] = *((%s*) ptr2);", cname, rname);
			TRY_end();
			CATCH(8);
			Printf(CPP, "    }\n");
		} else {
			SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: indexer");
			Printf(CPP, "    __declspec(dllexport) %s __cdecl Spr_%s_get_%s(HANDLE ptr, int i) {\n", rt, func, cname);
			TRY_begin_NEW(8, rt, _val);
			Printf(CPP, "(%s) (*((%s*) ptr))[i];", rt, cname);
			TRY_end_NEW();
			CATCH(8);
			RETURN_NEW(8, _val);
			Printf(CPP, "    }\n");

			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE ptr, int i, %s value) {\n", func, cname, rt);
			TRY_begin_void(8);
			Printf(CPP, "(*((%s*) ptr))[i] = value;", cname);
			TRY_end();
			CATCH(8);
			Printf(CPP, "    }\n");
		}

		// [CS]
		SNAP_ANA_PATH1(fps, FD_CS, "generate: indexer");
		rt = (char*) (is_ptr ? "IntPtr" : rname);
		Printf(CS,  "\tpublic %s this[int i] {\n", rname);
		if (EQ(rt, "IntPtr")) {
			Printf(CS,  "\t    get {\n");
			Printf(CS,  "\t        IntPtr ptr = SprExport.Spr_%s_get_%s(this, i);\n", func, cname);
			Printf(CS,  "\t        return new %s(ptr, true);\n", rname);
			Printf(CS,  "\t    }\n");
			Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(this, i, value); }\n", func, cname);
		} else {
			Printf(CS,  "\t    get { return (%s) SprExport.Spr_%s_get_%s(this, i); }\n", rt, func, cname);
			Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(this, i, value); }\n", func, cname);
		}
		Printf(CS,  "\t}\n");

		// [CSP]
		SNAP_ANA_PATH1(fps, FD_CSP, "generate: indexer");
		char* aname = (char*) (is_ptr ? "ptr" : "value");
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern %s Spr_%s_get_%s(IntPtr _this, int i);\n", rt, func, cname);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, int i, %s %s);\n", func, cname, rt, aname);
	}

	char* default_operator_func_name(char* symb, char* type) {
		char* name = NULL;
		bool unary_op = (type[2] == '\0');
		if (unary_op) {
			if (EQ(symb, "-"))		name = "neg";
			else if (EQ(symb, "[]"))	name = "index";
		} else {
			if (EQ(symb, "+"))		name = "add";
			else if (EQ(symb, "-"))		name = "sub";
			else if (EQ(symb, "*"))		name = (char*) ((type[1] == 'E' || type[2] == 'E') ? "mul" : "sprod");
			else if (EQ(symb, "/"))		name = "div";
			else if (EQ(symb, "%"))		name = "vprod1";
			else if (EQ(symb, "^"))		name = "vprod2";
			else if (EQ(symb, "=="))	name = "eq";
			else if (EQ(symb, "!="))	name = "ne";
		}
		if (name == NULL) {
			static int serial = 0;
			char buff[8];
			sprintf(buff, "%s%d", (unary_op ? "uop" : "bop"), serial++);
			string* str = new string(buff);
			name = (char*) str->c_str();
		}
		return name;
	}

	int count_extra_fields_in_operator_def(char* type) {
		int count = 0;
		while (*type) {
			char ch = *type++;
			if (ch == 'S' || ch == 'O') count++;
		}
		return count;
	}

	string operator_type_analyzer(char type, char* my_class) {
		switch (type) {
		case 'C':	// my class
			return string(my_class);
			break;
		case 'S':	// scalar type
		case 'O':	// other class
			return string("*type-list*");
			break;
		case 'E':	// element type
			return string(element_type(my_class));
			break;
		case 'v':	// Vec**
			return string("Vec") + dimension_and_type(my_class);
			break;
		case 'm':	// Matrix**
			return string("Matrix") + dimension_and_type(my_class);
			break;
		case 'q':	// Quaternion*
			return string("Quaternion") + dimension_and_type(my_class).substr(0, 1);
			break;
		case 'p':	// Quaternion*
			return string("Pose") + dimension_and_type(my_class).substr(0, 1);
			break;
		}
		abort(__LINE__, "bad type specifier '%c' in %s operator definition", type, my_class);
		return NULL;	// DO NOT COME HERE
	}

	void generate_operator(DOHFile* fps[], char* cname, char* op, char* fname, char* type, char* rname, char* aname, char* bname) {
		// cname, op, fname:	class name, operator symbol and function name component
		// type:		[0]: result type, [1]: operand 1 type, [2]: operand 2 type
		// 			'C': my class,		'E': element type,
		//			'v': type Vec**,	'm': type Matrix**,	'q': type Qauternion*,	'p': type Pose*
		//			'S': scalar type (int, float, double or bool),
		//			'O': other class
		// rname, aname, bname:	result class/type name, operand 1 class/type name and operand 2 class/type name
		bool binaryop = (type[2] != '\0');
		bool indexer = EQ(op, "[]");
		char rtype = type[0];
		char atype = type[1];
		char btype = binaryop ? type[2] : '-';
		char* rt = (char*) (generate_operator_is_type_scalar(rtype) ? rname : "HANDLE");
		char* rn = (char*) (generate_operator_is_type_scalar(rtype) ? "value" : "ptr");
		char* at = (char*) (generate_operator_is_type_scalar(atype) ? aname : "HANDLE");
		char* an = (char*) (generate_operator_is_type_scalar(atype) ? "a" : "ptr1");
		char* bt = (char*) (generate_operator_is_type_scalar(btype) ? bname : "HANDLE");
		char* bn = (char*) (generate_operator_is_type_scalar(btype) ? "b" : "ptr2");
		char arg1[4*MAX_NAMELEN+64], arg2[4*MAX_NAMELEN+64];
		char func[MAX_NAMELEN+8];
		char argsbuff[8*MAX_NAMELEN+128];
		sprintf(func, "%s_%s", fname, improve_type_representaion(string(type), cname, rname, aname, bname).c_str());
		PRINTF(fps, FD_ALL, "//_[generate: %snary operator: %s operator(%s) _%s_ (%s)]\n", (binaryop ? "bi" : "u"), cname, op, fname, type);

		// subscript operator
		if (indexer) {
			generate_indexer(fps, cname, func, rtype, rname);
			return;
		}

		// [cpp]
		SNAP_ANA_PATH1(fps, FD_CPP, "generate_operator");
		SNAP_ANA_PATH1(fps, FD_CPP, "try-catch: operator");
		char* rtif = (char*) (EQ(rt, "bool") ? "char" : rt);
		char* opname = (char*) (binaryop ? "binary" : "unary");
		sprintf(argsbuff, (binaryop ? "%s %s, %s %s" : "%s %s"), at, an, bt, bn);
		Printf(CPP, "    __declspec(dllexport) %s __cdecl Spr_%s_operator_%s_%s(%s) {\n", rtif, opname, func, cname, argsbuff);
		if (generate_operator_is_type_scalar(atype)) { strcpy(arg1, "a"); } else { sprintf(arg1, "*((%s*) %s)", aname, an); }
		if (generate_operator_is_type_scalar(btype)) { strcpy(arg2, "b"); } else { sprintf(arg2, "*((%s*) %s)", bname, bn); }
		if (EQ(rtif, "HANDLE")) {
			TRY_begin_NEW(8, rtif, _ptr);
		} else {
			TRY_begin_NEW(8, rtif, _val);
		}
		if (binaryop) {
			sprintf(argsbuff, "(%s) %s (%s)", arg1, op, arg2);
		} else {
			sprintf(argsbuff, "%s(%s)", op, arg1);
		}
		if (generate_operator_is_type_scalar(rtype)) {
			if (EQ(rname, "bool")) {
				Printf(CPP, "(char) (%s);", argsbuff);
			} else {
				Printf(CPP, "(%s) (%s);", rt, argsbuff);
			}
		} else {
			Printf(CPP, "new %s(%s);", rname, argsbuff);
		}
		TRY_end_NEW();
		CATCH(8);
		if (EQ(rtif, "HANDLE")) {
			RETURN_NEW(8, _ptr);
		} else {
			RETURN_NEW(8, _val);
		}
		Printf(CPP, "    }\n");

		// [cs]
		SNAP_ANA_PATH1(fps, FD_CS, "generate_operator");
		if (EQ(rt, "HANDLE")) rt = (char*) "IntPtr";
		if (EQ(at, "HANDLE")) at = (char*) "IntPtr";
		if (EQ(bt, "HANDLE")) bt = (char*) "IntPtr";
		sprintf(argsbuff, (binaryop ? "%s a, %s b" : "%s a"), aname, bname);
		Printf(CS,  "\tpublic static %s operator%s (%s) {\n", rname, op, argsbuff);
		if (!generate_operator_is_type_scalar(atype)) {
			Printf(CS,  "\t    bool ap = (object) a == null;\n");
		}
		if (!generate_operator_is_type_scalar(btype) && binaryop) {
			Printf(CS,  "\t    bool bp = (object) b == null;\n");
		}
		if (EQ(op, "==")) {	// these are binary operators
			Printf(CS,  "\t    if (ap || bp) { return ap && bp; }\n");
		} else if (EQ(op, "!=")) {
			Printf(CS,  "\t    if (ap || bp) { return !ap || !bp; }\n");
		}
		strcpy(argsbuff, (binaryop ? "a, b" : "a"));
		if (generate_operator_is_type_scalar(rtype)) {
			if (EQ(rname, "bool")) {
				Printf(CS,  "\t    byte ret = (byte) SprExport.Spr_%s_operator_%s_%s(%s);\n", opname, func, cname, argsbuff);
				Printf(CS,  "\t    return (ret == 0) ? false : true;\n");
			} else {
				Printf(CS,  "\t    return (%s) SprExport.Spr_%s_operator_%s_%s(%s);\n", rt, opname, func, cname, argsbuff);
			}
		} else {
			Printf(CS,  "\t    IntPtr ptr = SprExport.Spr_%s_operator_%s_%s(%s);\n", opname, func, cname, argsbuff);
			Printf(CS,  "\t    return new %s(ptr, true);\n", rname);
		}
		Printf(CS,  "\t}\n");
		if (EQ(op, "==")) {
			// operator "!=" should always be overloaded with "==".
			Printf(CS,  "\tpublic override int GetHashCode() { return base.GetHashCode(); }\n");
			Printf(CS,  "\tpublic override bool Equals(object o) { return true; }\n");
		}

		// [csp]
		SNAP_ANA_PATH1(fps, FD_CSP, "generate_operator");
		rtif = (char*) (EQ(rt, "bool") ? "char" : rt);	// 'rt' may be changed at [cs] generation
		sprintf(argsbuff, (binaryop ? "%s %s, %s %s" : "%s %s"), at, an, bt, bn);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern %s Spr_%s_operator_%s_%s(%s);\n", rtif, opname, func, cname, argsbuff);
	}

	bool generate_operator_is_type_scalar(char type) {
		if (type == 'E' || type == 'S') {
			return true;
		}
		return false;
	}

	int has_print_facility(Node* node) {
		char* name = unqualified_name(Char(Getattr(node, "name")));
		if (force_generate_tostring(name)) {	// Kludge!!
			return 9;
		}
		Node* c = firstChild(node);
		while (c) {
			char* c_name = unqualified_name(Char(Getattr(c, "name")));
			if (EQc(c_name, "Print"))	return 1;
			if (EQc(c_name, "print"))	return 2;
			if (EQc(c_name, "operator <<"))	return 3;
			c = nextSibling(c);
		}
		if (!ENDWITH(name, "If")) {
			return -1;
		}
		return 0;
	}

#if (GATHER_INFO == 1)
	char* print_facility_name(int print_facility_type) {
		switch (print_facility_type) {
		case 0:	return "check if instance of ObjectIf";
		case 1:	return "Print";
		case 2:	return "print";
		case 3:	return "operator <<";
		case 9:	return "forced";
		}
		return "no";
	}
#else
	char* print_facility_name(int print_facility_type) { return NULL; }
#endif

	int force_generate_tostring(char* name) {
		static char* names[] = {
			"Vec", "Quaternion", "Pose", "Matrix", "Affine", NULL
		};
		static char* excludes[] = {
			"MatrixExtension", NULL
		};
		for (char **p = names; *p; p++) {
			if (BEGINWITH(name, *p)) {
				for (char **q = excludes; *q; q++) {
					if (EQ(name, *q)) return 0;
				}
				return 1;
			}
		}
		return 0;
	}

	void generate_to_string(DOHFile* fps[], char* c_name, char* v_name, int print_facility_type) {
		if (print_facility_type < 0) return;
		// [cpp]
		//
		SNAP_ANA_PATH2(fps, FD_CPP, "generate: ToString", print_facility_name(print_facility_type));
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_ToString(HANDLE _this) {\n", c_name);
		Printf(CPP, "        BSTR result = NULL;\n");
		Printf(CPP, "        try {\n");
		Printf(CPP, "            ostringstream oss;\n");
		char* descname = NULL;
		switch (print_facility_type) {
		case 0:		// instance of ObjectIf
			Printf(CPP, "            string str = \"\";\n");
			Printf(CPP, "            if (dynamic_cast<ObjectIf*>((%s*) _this)) {\n", c_name);
			Printf(CPP, "                (*((%s*) _this)).GetObjectIf()->Print(oss);\n", c_name);
			Printf(CPP, "                str = oss.str();\n");
			Printf(CPP, "            }\n");
			break;
		case 1:		// has 'Print'
			Printf(CPP, "            string str;\n");
			Printf(CPP, "            (*((%s*) _this)).Print(oss);\n", c_name);
			Printf(CPP, "            str = oss.str();\n");
			break;
		case 2:		// has 'print'
			Printf(CPP, "            string str;\n");
			Printf(CPP, "            (*((%s*) _this)).print(oss);\n", c_name);
			Printf(CPP, "            str = oss.str();\n");
			break;
		case 3:		// has 'operator <<'
		case 9:		// force generate
			Printf(CPP, "            string str;\n");
			Printf(CPP, "            oss << (*((%s*) _this));\n", c_name);
			Printf(CPP, "            str = oss.str();\n");
			break;
		}
		Printf(CPP, "            const char* cstr = str.c_str();\n");
		Printf(CPP, "            int lenA = str.size();\n");
		Printf(CPP, "            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);\n");
		Printf(CPP, "            if (lenW >= 0) {\n");
		Printf(CPP, "                result = ::SysAllocStringLen(0, lenW);\n");
		Printf(CPP, "                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);\n");
		Printf(CPP, "            }\n");
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return result;\n");
		Printf(CPP, "    }\n");
#if (FREE_UNMANAGED_MEMORY == 1)
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_FreeString(BSTR ptr) {\n", c_name);
		Printf(CPP, "        try { ::SysFreeString(ptr); }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");
#endif
		// [cs]
		//
		SNAP_ANA_PATH2(fps, FD_CS, "generate: ToString", print_facility_name(print_facility_type));
		Printf(CS, "        public override string ToString() {\n");
		Printf(CS, "            IntPtr ptr = SprExport.Spr_%s_ToString((IntPtr) _this);\n", c_name);
		Printf(CS, "            string bstr = Marshal.PtrToStringBSTR(ptr);\n");
#if (FREE_UNMANAGED_MEMORY == 1)
		Printf(CS, "            SprExport.Spr_%s_FreeString((IntPtr) ptr);\n", c_name);
#endif
		Printf(CS, "            return bstr;\n");
		Printf(CS, "        }\n");
		// [csp]
		//
		SNAP_ANA_PATH2(fps, FD_CSP, "generate: ToString", print_facility_name(print_facility_type));
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_ToString(IntPtr _this);\n", c_name);
#if (FREE_UNMANAGED_MEMORY == 1)
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_FreeString(IntPtr ptr);\n", c_name);
#endif
	}

	void FP_generate_enum_def(DOHFile* fps[], int flag, int line, char* indent, Node* node, char* name, char* msg) {
		// [cs]
		//
		/***
		NodeInfo& e = get_node_info(fps, node);
		DUMP_NODE_INFO_L(fps, flag, line, msg, e);
		Node* dec = firstChild(node);
		while (dec) {
			NodeInfo& deci = get_node_info(fps, dec);
			DUMP_NODE_INFO_L(fps, flag, line, msg, deci);
			dec = nextSibling(dec);
		}
		***/
		if (name == NULL) {
			NodeInfo& e = get_node_info(fps, node);
			name = e.cs_type;
		}
		DOHFile* file = FP(flag);
		Printf(file, "%spublic enum %s : int {\n", indent, name);
		Printf(file, "%s    ", indent);
		Node* ec = firstChild(node);
		while (ec) {
			String* value = Getattr(ec, "enumvalueex");
			if (value) {
				Printf(file, "%s = %s", Getattr(ec, "name"), value);
			} else {
				Printf(file, "%s", Getattr(ec, "name"));
			}
			if ((ec = nextSibling(ec)) != NULL) {
				Printf(file, ", ");
			}
		}
		Printf(file, "\n%s}\n", indent);
	}


	/******* DO NOT GENERATE !! ********
	void generate_accessors_for_parents(DOHFile* fps[], Node* topnode, Node* node, map<string, int>& dupe_map) {
		NodeInfo& ci = get_node_info(fps, node);
		PRINTF(fps, FD_ALL, "// accessors to %s: %d\n", ci.sym_name, __LINE__);
		StructInfo* struct_info = get_struct_info(fps, parentNode(node), ci.sym_name);
		StructMembersInfo** infos = struct_info->members;
		for (int i = 0; i < struct_info->num_members; i++) {
			StructMembersInfo* info = infos[i];
			NodeInfo& ni = get_node_info(fps, info->node);
			PRINTF(fps, FD_ALL, "//   (%d/%d): %s %s\n", (i+1), struct_info->num_members, ni.cpp_type, ni.cpp_name);
			string key(ni.cs_name);
			string wrapper_key(make_wrapper_name(fps, FD_ALL, __LINE__, ni, ci, "generate: wrapper key for parents"));
			bool duplicated = (dupe_map.find(key) != dupe_map.end());
			bool generated = (wrapper_map.find(wrapper_key) != wrapper_map.end());
			if (!duplicated && !generated) {
				generate_wrapper_accessor(fps, topnode, ni, ci, "parent", __LINE__);
				if (!duplicated) dupe_map[key] = 1;
				if (!generated) wrapper_map[wrapper_key] = 1;
			}
#ifdef	SNAP
			else {
				PRINTF(fps, FD_ALL, "//   (%d/%d): ** THIS NAME IS HIDDEN BY SUCCESSOR **\n", (i+1), struct_info->num_members);
			}
#endif
		}
	}
	*******/

	char* make_wrapper_type(NodeInfo& ni) {
		// 組み込み型のポインタを返す関数は配列を返すものとして扱う
		bool is_array = ni.is_array || (ni.is_pointer && ni.is_intrinsic && ni.is_variable && !ni.is_void_ptr);
		//return (char*) (ni.is_vector ? "vector" : (ni.is_array ? "array" : "struct"));
		return (char*) (ni.is_vector ? "vector" : (is_array ? "array" : "struct"));
	}

	char* make_wrapper_name(DOHFile* fps[], int flag, int line, NodeInfo& ni, NodeInfo& ci, char* label) {
		if (fps != NULL) {
			WRAPPER_NAME_PRINT(fps, flag, line, ni, label);
		}
		string wrapper_name(make_wrapper_type(ni));
		wrapper_name.append("wrapper_");
#ifdef	INCLUDE_CLASSNAME_TO_WRAPPERNAME
		if (ni.is_struct) {
			wrapper_name.append(ci.uq_name);
			wrapper_name.append("_");
		}
#endif	//INCLUDE_CLASSNAME_TO_WRAPPERNAME
		wrapper_name.append(make_wrapper_name_type_part(ni.uq_type, ni.pointer_level));
		char* name = new char[wrapper_name.length() + 1];
		strcpy(name, wrapper_name.c_str());
		return name;
	}

	char* make_wrapper_name_type_part(char* type_name, int pointer_level) {
		if (!type_name) return NULL;
		type_name = strip_type_modifier(type_name);
		char* p = strchr(type_name, '<');
		if (p) *p = '\0';
		char* buff = new char[strlen(type_name) + 1];
		char* d = buff;
		char* s = type_name;
		while (*s) {			// replace "::" to '_'
			if (*s == ':') { *d++ = '_'; s += 2; }
			else { *d++ = *s++; }
		}
		*d = '\0';
		string* type_str = new string(buff);
		if (strchr(buff, '<') && strchr(buff, '>')) {
			// TODO: 取り敢えずコンパイルが通るようにする。
			// TODO: 後でちゃんと考えること。
			char* kind = "";
			char* p;
			if ((p = strstr(buff, "vector"))) kind = "vector";
			else if ((p = strstr(buff, "list"))) kind = "list";
			else if ((p = strstr(buff, "TVector"))) kind = "TVector";
			if (p) {
				char* sp = strchr(p, '<') + 1;
				char* ep = strrchr(p, '>') - 1;
				while (isspace(*sp)) sp++;
				while (isspace(*ep)) ep--;
				ep[1] = 0;
				char* q = sp;
				while ((q = strchr(q, ','))) *q++ = '_';
				if (EQ(sp, "unsigned int")) sp = "unsigned_int";
				if (EQ(sp, "unsigned long")) sp = "unsigned_long";
				type_str = new string(kind);
				type_str->append("_");
				type_str->append(sp);
			}
		} else {
			if (EQ(buff, "unsigned int"))  type_str = new string("unsigned_int");
			if (EQ(buff, "unsigned long")) type_str = new string("unsigned_long");
		}
		if (pointer_level > 0) {
			type_str->append("_");
			while (--pointer_level >= 0) type_str->append("p");
		}
		free(buff);
		return (char*) type_str->c_str();
	}

#if (SNAP == 1)
	void wrapper_name_print(DOHFile* fps[], int flag, int line, NodeInfo& ni, char* label) {
		string buff(label);
		buff.append(": ");
		if (ni.is_intrinsic)	buff.append("intrinsic: ");
		if (ni.is_string) 	buff.append("string: ");
		if (ni.is_struct)	buff.append("struct: ");
		buff.append(make_wrapper_type(ni));
		SNAP_ANA_PATH1L(fps, flag, line, (char*) buff.c_str());
	}
#endif

	void generate_wrapper_accessor(DOHFile* fps[], Node* topnode, NodeInfo& ni, NodeInfo& ci, char* label, int line) {
		if (ni.is_intrinsic) {
			if (ni.is_vector) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrapper accessor: intrinsic: vector");
				generate_accessor_for_type_vector_intrinsic(fps, ni, ci);
			} else if (ni.is_array) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrapper accessor: intrinsic: array");
				generate_accessor_for_type_array_intrinsic(fps, ni, ci);
			} else {
				if (ni.is_pointer) {
					SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: accessor: intrinsic: pointer");
					generate_accessor_for_type_intrinsic_pointer(fps, ni, ci);
				} else if (ni.is_bool) {
					SNAP_ANA_PATH2L(fps, FD_ALL, line, "generate: accessor: intrinsic: bool", label);
					generate_accessor_for_type_intrinsic_bool(fps, ni, ci);
				} else {
					SNAP_ANA_PATH2L(fps, FD_ALL, line, "generate: accessor: intrinsic", label);
					generate_accessor_for_type_intrinsic(fps, ni, ci);
				}
			}
		} else if (ni.is_string) {
			if (ni.is_vector) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrapper accessor: string: vector");
				generate_accessor_for_type_vector_string(fps, ni, ci);
			} else if (ni.is_array) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrapper accessor: string: array");
				generate_accessor_for_type_array_string(fps, ni, ci);
			} else {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrapper accessor: string");
				generate_accessor_for_type_string(fps, ni, ci);
			}
		} else if (ni.is_struct) {
			Node* is_enum_node = FindNodeByAttrR(topnode, "enumtype", ni.type);
			if (is_enum_node) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrpper accessor: struct: enum");
				DUMP_NODE_INFO(fps, FD_ALL, "is_enum_node", get_node_info(fps, is_enum_node));
				generate_accessor_for_type_struct_enum(fps, topnode, ni, ci);
			}
			else if (ni.is_pointer) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrpper accessor: struct: pointer");
				generate_accessor_for_type_struct_pointer(fps, ni, ci);
			}
			else if (ni.is_vector) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrpper accessor: struct: vector");
				generate_accessor_for_type_vector_struct(fps, ni, ci);
			}
			else if (ni.is_array) {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrpper accessor: struct: array");
				generate_accessor_for_type_array_struct(fps, ni, ci);
			}
			else {
				SNAP_ANA_PATH1L(fps, FD_ALL, line, "generate: wrpper accessor: struct");
				generate_accessor_for_type_struct(fps, ni, ci);
			}
			
		} else {
			DUMP_NODE_INFO(fps, FD_CS, "ELSE", ni);
			SNAP_ANA_PATH1(fps, FD_ALL, "UNIMPLEMENTED!!");
		}
	}

	bool is_already_generated(NodeInfo& ni, NodeInfo& ci) {
		string key("Spr_");
		key.append(ci.uq_name).append("_get_").append(ni.uq_name);
		bool ret = accessor_generation_map.find(key) != accessor_generation_map.end();
		if (!ret) {
			accessor_generation_map[key] = 1;
		}
		return ret;
	}

	void generate_accessor_for_type_intrinsic_pointer(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: intrinsic: pointer");
		// [cs]
		Printf(CS,  "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS,  "\t    get { return SprExport.Spr_%s_get_%s(_this); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(_this, value); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t}\n");
		if (is_already_generated(ni, ci)) return;

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = (*((%s*)_this)).%s; }\n", ci.cpp_name, ni.cpp_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, HANDLE value) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try { (*((%s*)_this)).%s = (*((%s*)value)); }\n", ci.cpp_name, ni.cpp_name, ni.cpp_type);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern %s Spr_%s_get_%s(IntPtr _this);\n", ni.cs_im_type, ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, IntPtr value);\n", ci.uq_name, ni.uq_name, ni.cs_type);
	}

	void generate_accessor_for_type_intrinsic_bool(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: intrinsic: bool");
		// [cs]
		Printf(CS, "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS, "\t    get {\n");
		Printf(CS, "\t        byte ret = (byte) SprExport.Spr_%s_get_%s(_this);\n", ci.uq_name, ni.uq_name);
		Printf(CS, "\t        return (ret == 0) ? false : true;\n");
		Printf(CS, "\t    }\n");
		Printf(CS, "\t    set {\n");
		Printf(CS, "\t        byte val = (byte) (value ? 1 : 0);\n");
		Printf(CS, "\t        SprExport.Spr_%s_set_%s(_this, (char) val);\n", ci.uq_name, ni.uq_name);
		Printf(CS, "\t    }\n");
		Printf(CS, "\t}\n");
		if (is_already_generated(ni, ci)) return;

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) char __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        char _val = 0;\n");
		Printf(CPP, "        try { _val = (char) (*((%s*)_this)).%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _val;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, char value) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try { (*((%s*)_this)).%s = (value == 0) ? false : true; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern char Spr_%s_get_%s(IntPtr _this);\n", ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, char value);\n", ci.uq_name, ni.uq_name);
	}

	void generate_accessor_for_type_intrinsic(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: intrinsic");
		// [cs]
		Printf(CS,  "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS,  "\t    get { return SprExport.Spr_%s_get_%s(_this); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(_this, value); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t}\n");
		if (is_already_generated(ni, ci)) return;

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) %s __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ni.cpp_type, ci.uq_name, ni.uq_name);
		if (ni.is_reference) {
			Printf(CPP, "        %s _tmp = 0;\n", ni.type);
			Printf(CPP, "        %s _val = _tmp;\n", ni.cpp_type);
		} else {
			Printf(CPP, "        %s _val = 0;\n", ni.cpp_type);
		}
		Printf(CPP, "        try { _val = (*((%s*)_this)).%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _val;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, %s value) {\n", ci.uq_name, ni.uq_name, ni.cpp_type);
		Printf(CPP, "        try { (*((%s*)_this)).%s = value; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern %s Spr_%s_get_%s(IntPtr _this);\n", ni.cs_im_type, ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, %s value);\n", ci.uq_name, ni.uq_name, ni.cs_im_type);
	}

	void generate_accessor_for_type_string(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: string");
		// [cs]　受け渡しは BSTR
		Printf(CS, "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS, "\t    get {\n");
		Printf(CS, "\t        IntPtr ptr = SprExport.Spr_%s_get_%s(_this);\n", ci.uq_name, ni.uq_name);
		Printf(CS, "\t        string bstr = Marshal.PtrToStringBSTR(ptr);\n");
#if (FREE_UNMANAGED_MEMORY == 1)
		Printf(CS, "\t        SprExport.Spr_%s_FreeString_%s(ptr);\n", ci.uq_name, ni.uq_name);
#endif
		Printf(CS, "\t        return bstr;\n");
		Printf(CS, "\t    }\n");
		Printf(CS, "\t    set {\n");
		Printf(CS, "\t        IntPtr pbstr = Marshal.StringToBSTR(value);\n");
		Printf(CS, "\t        SprExport.Spr_%s_set_%s(_this, pbstr);\n", ci.uq_name, ni.uq_name);
		Printf(CS, "\t    }\n");
		Printf(CS, "\t}\n");
		if (is_already_generated(ni, ci)) return;

		// [cpp]　受け渡しは BSTR
//vector<string>は未対応
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        BSTR result = NULL;\n");
		Printf(CPP, "        try {\n");
		generate_string_get(CPP, "\t    ", "result", ci.name, ni.name);
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return result;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, HANDLE value) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try {\n");
		generate_string_set(CPP, 0, "\t    ", "value", ci.name, ni.name);
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");
#if (FREE_UNMANAGED_MEMORY == 1)
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_FreeString_%s(BSTR ptr) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try { ::SysFreeString(ptr); }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");
#endif

		// [csp]　受け渡しは BSTR
		Printf(CSP, "\t%s\n", DLLIMPORT);
		//Printf(CSP, "\t[return: MarshalAs(UnmanagedType.%s)]\n", ni.cs_marshaltype);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_get_%s(IntPtr _this);\n", ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, IntPtr value);\n", ci.uq_name, ni.uq_name);
#if (FREE_UNMANAGED_MEMORY == 1)
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_FreeString_%s(IntPtr ptr);\n", ci.uq_name, ni.uq_name);
#endif
	}

	void generate_accessor_for_type_struct_enum(DOHFile* fps[], Node* topnode, NodeInfo& ni, NodeInfo& ci) {
		Node* is_enum_node = FindNodeByAttrR(topnode, "enumtype", ni.type);

		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: struct: enum");
		// [cs]
		Printf(CS,  "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS,  "\t    get { return (%s) SprExport.Spr_%s_get_%s(_this); }\n", ni.cs_type, ci.uq_name, ni.uq_name);
		Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(_this, (int) value); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t}\n");

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) %s __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ni.cpp_type, ci.uq_name, ni.uq_name);
		Printf(CPP, "        %s _val = (%s) 0;\n", ni.cpp_type, ni.cpp_type);
		Printf(CPP, "        try { _val = (*((%s*)_this)).%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _val;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, int value) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try { (*((%s*)_this)).%s = (%s) value; }\n", ci.cpp_name, ni.uq_name, ni.cpp_type);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern int Spr_%s_get_%s(IntPtr _this);\n", ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, int value);\n", ci.uq_name, ni.uq_name);
	}
	void generate_accessor_for_type_struct_pointer(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: struct: pointer");
		// [cs]
		Printf(CS,  "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS,  "\t    get { return new %s(SprExport.Spr_%s_get_%s(_this)); }\n", ni.uq_type, ci.uq_name, ni.uq_name);
		Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(_this, value); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t}\n");

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = ((%s*)_this)->%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, HANDLE _ptr) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try {\n");
		Printf(CPP, "            if (&(((%s*)_this)->%s) != _ptr) {\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "                ((%s*)_this)->%s = ((%s*)_ptr);\n", ci.cpp_name, ni.uq_name, ni.uq_type);
		Printf(CPP, "            }\n");
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_get_%s(IntPtr _this);\n", ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, IntPtr _ptr);\n", ci.uq_name, ni.uq_name);
	}

	void generate_accessor_for_type_struct(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: struct");
		// [cs]
		char* cs_type = cs_qualified_name(strip_type_modifier(ni.cs_type));
		Printf(CS,  "\tpublic %s %s {\n", cs_type, ni.cs_name);
		Printf(CS,  "\t    get { return new %s(SprExport.Spr_%s_addr_%s(_this)); }\n", cs_type, ci.uq_name, ni.uq_name);
		Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(_this, value); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t}\n");

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_addr_%s(HANDLE _this) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = &((%s*)_this)->%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, HANDLE _ptr) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try {\n");
		Printf(CPP, "            if (&(((%s*)_this)->%s) != _ptr) {\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "                ((%s*)_this)->%s = *((%s*)_ptr);\n", ci.cpp_name, ni.uq_name, ni.uq_type);
		Printf(CPP, "            }\n");
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_addr_%s(IntPtr _this);\n", ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, IntPtr _ptr);\n", ci.uq_name, ni.uq_name);
	}

	void generate_accessor_for_type_enum(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: enum");
		// [cs]
		Printf(CS,  "\tpublic %s %s {\n", ni.cs_type, ni.cs_name);
		Printf(CS,  "\t    get { return (%s) SprExport.Spr_%s_get_%s(_this); }\n", ni.cs_type, ci.uq_name, ni.uq_name);
		Printf(CS,  "\t    set { SprExport.Spr_%s_set_%s(_this, (int) value); }\n", ci.uq_name, ni.uq_name);
		Printf(CS,  "\t}\n");

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) int __cdecl Spr_%s_get_%s(HANDLE _this) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        int _val = 0;\n");
		Printf(CPP, "        try { _val = (*((%s*)_this)).%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _val;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE _this, int value) {\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        try { (*((%s*)_this)).%s = (%s) value; }\n", ci.cpp_name, ni.uq_name, ni.type);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern int Spr_%s_get_%s(IntPtr _this);\n", ci.uq_name, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _this, int value);\n", ci.uq_name, ni.uq_name);
	}

	void generate_wrapper_accessor_struct(/*DOHFile* org_fps[],*/ Node* topnode, Node* node, NodeInfo& ni, NodeInfo& ci, char* label, int line ) {
		char* wrapper_type = make_wrapper_type(ni);
		char* wrapper_name = make_wrapper_name(NULL/*fps*/, FD_ALL, line, ni, ci, label);
		char* type_name = make_wrapper_name_type_part(ni.uq_type, ni.pointer_level);
#if (USE_SIGNATURE_FILE == 1)
		if (signature_map.find(string(wrapper_name)) != signature_map.end()) {
			PRINTinfo(gip, "signature: already generated: %s\n", wrapper_name);
			return;
		}
		if (sig_ofs.good()) {
			sig_ofs.write(wrapper_name, strlen(wrapper_name));
			sig_ofs.write("\n", 1);
		}
#endif

		DOHFile* fps[3] = { NULL, NULL, NULL };
		create_wrapper_accessor_file(fps, topnode, wrapper_name);
		WRAPPER_NAME_PRINT(fps, FD_ALL, line, ni, label);
		DUMP_NODE_INFO(fps, FD_ALL, "generate_wrapper_accessor_struct ci", ci);
		DUMP_NODE_INFO(fps, FD_ALL, "generate_wrapper_accessor_struct ni", ni);

		char* cpp_type = strip_type_modifier(ni.cpp_type);
		Node* is_enum_node = FindNodeByAttrR(topnode, "enumtype", ni.type);

		// [cpp]
		SNAP_ANA_PATH2(fps, FD_CPP, "generate_wrapper_accessor_struct", wrapper_type);
		if (EQ(wrapper_type, "vector")) {
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_new_%s() {\n", wrapper_type, type_name);
			Printf(CPP, "        HANDLE _ptr = NULL;\n");
			Printf(CPP, "        try { _ptr = new vector<%s>; }\n", cpp_type);
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "        return _ptr;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_delete_%s(HANDLE ptr) {\n", wrapper_type, type_name);
			Printf(CPP, "        if (ptr == NULL) return;\n");
			Printf(CPP, "        vector<%s>* vec = (vector<%s>*) ptr;\n", type_name, type_name);
			Printf(CPP, "        vector<%s>().swap(*vec);\n", type_name);
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_get_%s(HANDLE ptr, int index) {\n", wrapper_type, type_name);
			Printf(CPP, "        HANDLE _ptr = NULL;\n");
			Printf(CPP, "        try {\n");
			Printf(CPP, "            vector<%s>* vecptr = (vector<%s>*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            _ptr = (HANDLE) &(*vecptr)[index];\n");
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "        return _ptr;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE ptr, int index, HANDLE value) {\n", wrapper_type, type_name);
			Printf(CPP, "        try {\n");
			Printf(CPP, "            vector<%s>* vecptr = (vector<%s>*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            (*vecptr)[index] = *((%s*) value);\n", cpp_type);
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) int __cdecl Spr_%s_size_%s(HANDLE ptr) {\n", wrapper_type, type_name);
			Printf(CPP, "        int _val = 0;\n");
			Printf(CPP, "        try {\n");
			Printf(CPP, "            vector<%s>* vecptr = (vector<%s>*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            _val = (int) (*vecptr).size();\n");
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "        return _val;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_push_back_%s(HANDLE ptr, HANDLE value) {\n", wrapper_type, type_name);
			Printf(CPP, "        try {\n");
			Printf(CPP, "            vector<%s>* vecptr = (vector<%s>*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            (*vecptr).push_back(*((%s*) value));\n", cpp_type);
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_clear_%s(HANDLE ptr) {\n", wrapper_type, type_name, cpp_type);
			Printf(CPP, "        try {\n");
			Printf(CPP, "            vector<%s>* vecptr = (vector<%s>*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            (*vecptr).clear();\n");
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
		}
		else if (EQ(wrapper_type, "array")) {
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_new_%s(int size) {\n", wrapper_type, type_name);
			Printf(CPP, "        HANDLE _ptr = NULL;\n");
			Printf(CPP, "        try { _ptr = new %s[size]; }\n", cpp_type);
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "        return _ptr;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_delete_%s(HANDLE ptr) {\n", wrapper_type, type_name);
			Printf(CPP, "        if (ptr == NULL) return;\n");
			Printf(CPP, "        try { delete[] (%s*) ptr; }\n", cpp_type, cpp_type);
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_get_%s(HANDLE ptr, int index) {\n", wrapper_type, type_name);
			Printf(CPP, "        HANDLE _ptr = NULL;\n");
			Printf(CPP, "        try {\n");
			Printf(CPP, "            %s* aryptr = (%s*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            _ptr = (HANDLE) &aryptr[index];\n");
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "        return _ptr;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_set_%s(HANDLE ptr, int index, HANDLE value) {\n", wrapper_type, type_name);
			Printf(CPP, "        try {\n");
			Printf(CPP, "            %s* aryptr = (%s*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            aryptr[index] = *((%s*) value);\n", cpp_type);
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
		}
		else if (is_enum_node) {
			char* cpp_type = unqualified_name(ni.cpp_type, 0/*force*/);
			Printf(CPP, "    __declspec(dllexport) int __cdecl Spr_%s_%s_get_%s(HANDLE ptr, int index) {\n", ci.uq_name, wrapper_type, type_name);
			Printf(CPP, "        int _val = 0;\n");
			Printf(CPP, "        try {\n");
			Printf(CPP, "            %s* aryptr = (%s*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            _val = (int) aryptr[index];\n");
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "        return _val;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_%s_new_%s() {\n", ci.uq_name, wrapper_type, type_name);
			Printf(CPP, "        vector<%s>* ptr = new vector<%s>;\n", ci.uq_name, ci.uq_name);
			Printf(CPP, "        return (HANDLE) ptr;\n");
			Printf(CPP, "    }\n");
			Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_%s_set_%s(HANDLE ptr, int index, int value) {\n", ci.uq_name, wrapper_type, type_name);
			Printf(CPP, "        try {\n");
			Printf(CPP, "            %s* aryptr = (%s*) ptr;\n", cpp_type, cpp_type);
			Printf(CPP, "            aryptr[index] = (%s) value;\n", cpp_type);
			Printf(CPP, "        }\n");
			Printf(CPP, "        %s\n", CATCH_code);
			Printf(CPP, "    }\n");
		}

		// [cs]
		SNAP_ANA_PATH2(fps, FD_CS, "generate_wrapper_accessor_struct", wrapper_type);
		char* cs_type = cs_qualified_name(strip_type_modifier(ni.uq_type));

		if (EQ(wrapper_type, "vector")) {
			SNAP_AT_LINE(fps, FD_CS);
			Printf(CS,  "    public class %s : vectorwrapper {\n", wrapper_name);
			Printf(CS,  "        public %s() {\n", wrapper_name);
			Printf(CS,  "            _this = SprExport.Spr_%s_new_%s();\n", wrapper_type, type_name);
			Printf(CS,  "            _flag = true;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public %s(IntPtr ptr) : base(ptr) {}\n", wrapper_name);
			Printf(CS,  "        public %s(IntPtr ptr, bool flag) : base(ptr, flag) {}\n", wrapper_name);
			Printf(CS,  "        ~%s() {\n", wrapper_name);
			Printf(CS,  "            if (_flag) { SprExport.Spr_%s_delete_%s(_this); _flag = false; }\n", wrapper_type, type_name);
			Printf(CS,  "        }\n");
			Printf(CS,  "        public %s this[int index] {\n", type_name);
			Printf(CS,  "            get { return new %s(SprExport.Spr_%s_get_%s(_this, index)); }\n", type_name, wrapper_type, type_name);
			Printf(CS,  "            set { SprExport.Spr_%s_set_%s(_this, index, value); }\n", wrapper_type, type_name);
			Printf(CS,  "        }\n");
			Printf(CS,  "        public int size() { return (int) SprExport.Spr_%s_size_%s(_this); }\n", wrapper_type, type_name);
			Printf(CS,  "        public void push_back(%s value) { SprExport.Spr_%s_push_back_%s(_this, value); }\n", type_name, wrapper_type, type_name);
			Printf(CS,  "        public void clear() { SprExport.Spr_%s_clear_%s(_this); }\n", wrapper_type, type_name);
			//
			Printf(CS,  "        public static implicit operator List<%s>(%s m) {\n", ni.cs_type, wrapper_name);
			Printf(CS,  "            List<%s> r = new List<%s>();\n", ni.cs_type, ni.cs_type);
			Printf(CS,  "            int size = SprExport.Spr_%s_size_%s(m._this);\n", wrapper_type, type_name);
			Printf(CS,  "            for (int i = 0; i < size; i++) {\n");
			Printf(CS,  "                IntPtr member = SprExport.Spr_%s_get_%s(m._this, i);\n", wrapper_type, type_name);
			Printf(CS,  "                r.Add(new %s(member));\n", type_name);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return r;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public static implicit operator %s(List<%s> r) {\n", wrapper_name, type_name);
			Printf(CS,  "            IntPtr ptr = SprExport.Spr_%s_new_%s();\n", wrapper_type, type_name);
			Printf(CS,  "            %s m = new %s(ptr, true);\n", wrapper_name, wrapper_name);
			Printf(CS,  "            for (int i = 0; i < (int) r.Count; i++) {\n");
			Printf(CS,  "                SprExport.Spr_%s_push_back_%s(ptr, r[i]);\n", wrapper_type, type_name);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return m;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public static implicit operator %s[](%s m) {\n", ni.cs_type, wrapper_name);
			Printf(CS,  "            int size = SprExport.Spr_%s_size_%s(m._this);\n", wrapper_type, type_name);
			Printf(CS,  "            %s[] r = new %s[size];\n", ni.cs_type, ni.cs_type);
			Printf(CS,  "            for (int i = 0; i < size; i++) {\n");
			Printf(CS,  "                 IntPtr ptr = SprExport.Spr_%s_get_%s(m._this, i);\n", wrapper_type, type_name);
			Printf(CS,  "                 r[i] = new %s(ptr);\n", ni.cs_type);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return r;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public static implicit operator %s(%s[] r) {\n", wrapper_name, type_name);
			Printf(CS,  "            IntPtr ptr = SprExport.Spr_%s_new_%s();\n", wrapper_type, type_name);
			Printf(CS,  "            %s m = new %s(ptr, true);\n", wrapper_name, wrapper_name);
			Printf(CS,  "            for (int i = 0; i < r.Length; i++) {\n");
			Printf(CS,  "                SprExport.Spr_%s_push_back_%s(m._this, r[i]);\n", wrapper_type, type_name);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return m;\n");
			Printf(CS,  "        }\n");
		}
		else if (EQ(wrapper_type, "array")) {
			SNAP_AT_LINE(fps, FD_CS);
			Printf(CS,  "    public class %s : arraywrapper {\n", wrapper_name);
			Printf(CS,  "        public %s(int nelm) : base(nelm) {\n", wrapper_name);
			Printf(CS,  "            _this = SprExport.Spr_%s_new_%s(nelm);\n", wrapper_type, type_name);
			Printf(CS,  "            _flag = true;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public %s(IntPtr ptr, int nelm) : base(ptr, nelm) {}\n", wrapper_name);
			Printf(CS,  "        public %s(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}\n", wrapper_name);
			Printf(CS,  "        ~%s() {\n", wrapper_name);
			Printf(CS,  "            if (_flag) { SprExport.Spr_%s_delete_%s(_this); _flag = false; }\n", wrapper_type, type_name);
			Printf(CS,  "        }\n");
			Printf(CS,  "        public %s this[int index] {\n", type_name);
			Printf(CS,  "            get { return new %s(SprExport.Spr_%s_get_%s(_this, index)); }\n", type_name, wrapper_type, type_name);
			Printf(CS,  "            set { SprExport.Spr_%s_set_%s(_this, index, value); }\n", wrapper_type, type_name);
			Printf(CS,  "        }\n");
			Printf(CS,  "        public int size() { return (int) _nelm; }\n");
			//
			Printf(CS,  "        public static implicit operator List<%s>(%s m) {\n", ni.cs_type, wrapper_name);
			Printf(CS,  "            List<%s> r = new List<%s>();\n", ni.cs_type, ni.cs_type);
			Printf(CS,  "            for (int i = 0; i < (int) m._nelm; i++) {\n");
			Printf(CS,  "                IntPtr member = SprExport.Spr_%s_get_%s(m._this, i);\n", wrapper_type, type_name);
			Printf(CS,  "                r.Add(new %s(member));\n", type_name);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return r;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public static implicit operator %s(List<%s> r) {\n", wrapper_name, type_name);
			Printf(CS,  "            int size = r.Count;\n");
			Printf(CS,  "            IntPtr ptr = SprExport.Spr_%s_new_%s(size);\n", wrapper_type, type_name);
			Printf(CS,  "            %s m = new %s(ptr, size, true);\n", wrapper_name, wrapper_name);
			Printf(CS,  "            for (int i = 0; i < size; i++) {\n");
			Printf(CS,  "                SprExport.Spr_%s_set_%s(m._this, i, r[i]);\n", wrapper_type, type_name);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return m;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public static implicit operator %s[](%s m) {\n", ni.cs_type, wrapper_name);
			Printf(CS,  "            int size = (int) m._nelm;\n");
			Printf(CS,  "            %s[] r = new %s[size];\n", ni.cs_type, ni.cs_type);
			Printf(CS,  "            for (int i = 0; i < size; i++) {\n");
			Printf(CS,  "                 IntPtr ptr = SprExport.Spr_%s_get_%s(m._this, i);\n", wrapper_type, type_name);
			Printf(CS,  "                 r[i] = new %s(ptr);\n", ni.cs_type);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return r;\n");
			Printf(CS,  "        }\n");
			Printf(CS,  "        public static implicit operator %s(%s m) {\n", ni.cs_type, wrapper_name);
			Printf(CS,  "            int size = (int) m._nelm;\n");
			Printf(CS,  "            IntPtr ptr = SprExport.Spr_%s_get_%s(m._this, 0);\n", wrapper_type, type_name);
			Printf(CS,  "            %s r = new %s(ptr);\n", ni.cs_type, ni.cs_type);
			Printf(CS,  "            return r;\n");
			Printf(CS,  "        }\n");

			Printf(CS,  "        public static implicit operator %s(%s[] r) {\n", wrapper_name, type_name);
			Printf(CS,  "            int size = r.Length;\n");
			Printf(CS,  "            IntPtr ptr = SprExport.Spr_%s_new_%s(size);\n", wrapper_type, type_name);
			Printf(CS,  "            %s m = new %s(ptr, size, true);\n", wrapper_name, wrapper_name);
			Printf(CS,  "            for (int i = 0; i < size; i++) {\n");
			Printf(CS,  "                SprExport.Spr_%s_set_%s(m._this, i, r[i]);\n", wrapper_type, type_name);
			Printf(CS,  "            }\n");
			Printf(CS,  "            return m;\n");
			Printf(CS,  "        }\n");
		}
		else if (is_enum_node) {
			SNAP_AT_LINE(fps, FD_CS);
			Printf(CS,  "    public class %s : wrapper {\n", wrapper_name);
			Printf(CS,  "        private IntPtr _ptr;\n");
			Printf(CS,  "        public %s(IntPtr ptr) { _ptr = ptr; }\n", wrapper_name);
			Printf(CS,  "        public %s(IntPtr ptr, bool flag) { _ptr = ptr; _flag = flag; }\n", wrapper_name);
			Printf(CS,  "        protected %s() {}\n", wrapper_name);
			Printf(CS,  "        ~%s() {}\n", wrapper_name);
			Printf(CS,  "        public %s this[int index] {\n", cs_type);
			Printf(CS,  "            get { return (%s) SprExport.Spr_%s_%s_get_%s(_ptr, index); }\n", cs_type, ci.uq_name, wrapper_type, type_name);
			Printf(CS,  "            set { SprExport.Spr_%s_%s_set_%s(_ptr, index, (int) value); }\n", ci.uq_name, wrapper_type, type_name);
			Printf(CS,  "        }\n");
		} else {
			SNAP_AT_LINE(fps, FD_CS);
			Printf(CS,  "    public class %s : wrapper {\n", wrapper_name);
			Printf(CS,  "        private IntPtr _ptr;\n");
			Printf(CS,  "        public %s(IntPtr ptr) { _ptr = ptr; }\n", wrapper_name);
			Printf(CS,  "        public %s(IntPtr ptr, bool flag) { _ptr = ptr; _flag = flag; }\n", wrapper_name);
			Printf(CS,  "        protected %s() {}\n", wrapper_name);
			Printf(CS,  "        ~%s() {}\n", wrapper_name);
			Printf(CS,  "        public %s this[int index] {\n", cs_type);
			Printf(CS,  "            get { return new %s(SprExport.Spr_%s_%s_get_%s(_ptr, index)); }\n", ni.cs_type, ci.uq_name, wrapper_type, type_name);
			Printf(CS,  "            set { SprExport.Spr_%s_%s_set_%s(_ptr, index, value); }\n", ci.uq_name, wrapper_type, type_name);
			Printf(CS,  "        }\n");
		}
		Printf(CS,  "    }\n");

		// [csp]
		SNAP_ANA_PATH2(fps, FD_CSP, "generate_wrapper_accessor_struct", wrapper_type);
		if (EQ(wrapper_type, "vector")) {
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_%s_new_%s();\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_delete_%s(IntPtr _ptr);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_%s_get_%s(IntPtr _ptr, int index);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _ptr, int index, IntPtr value);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern int Spr_%s_size_%s(IntPtr _ptr);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_push_back_%s(IntPtr _ptr, IntPtr value);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_clear_%s(IntPtr _ptr);\n", wrapper_type, type_name);
		}
		else if (EQ(wrapper_type, "array")) {
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_%s_new_%s(int _size);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_delete_%s(IntPtr _ptr);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_%s_get_%s(IntPtr _ptr, int index);\n", wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_set_%s(IntPtr _ptr, int index, IntPtr value);\n", wrapper_type, type_name);
		}
		else if (is_enum_node) {
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern int Spr_%s_%s_get_%s(IntPtr _ptr, int index);\n", ci.uq_name, wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_%s_set_%s(IntPtr _ptr, int index, int value);\n", ci.uq_name, wrapper_type, type_name);
		} else {
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s_get_%s(IntPtr _ptr, int index);\n", ci.uq_name, wrapper_type, type_name);
			Printf(CSP, "\t%s\n", DLLIMPORT);
			Printf(CSP, "\tpublic static extern void Spr_%s_%s_set_%s(IntPtr _ptr, int index, IntPtr value);\n", ci.uq_name, wrapper_type, type_name);
		}
	}

	void generate_accessor_for_type_vector_intrinsic(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: vector: intrinsic");
		DUMP_NODE_INFO(fps, FD_ALL, "generate_wrapper_accessor_struct ci", ci);
		DUMP_NODE_INFO(fps, FD_ALL, "generate_wrapper_accessor_struct ni", ni);

		char* wrapper_name = make_wrapper_name(fps, FD_ALL, __LINE__, ni, ci, "vector");
		char* wrapper_type = make_wrapper_type(ni);
		char* type_name = make_wrapper_name_type_part(ni.uq_type, ni.pointer_level);

		// [cs]
		Printf(CS,  "        public %s %s {\n", wrapper_name, ni.cs_name);
		Printf(CS,  "            get { return new %s(SprExport.Spr_%s_%s_addr_%s(_this)); }\n", wrapper_name, ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CS,  "            set { SprExport.Spr_%s_%s_set_%s(_this, value); }\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CS,  "        }\n");

		/***  クラス名＋メンバ名で識別するようにしたので重複は生じない ***
		char wrapper_key[1024];
		sprintf_s(wrapper_key, sizeof(wrapper_key), "%s_%s_%s", ci.uq_name, wrapper_type, ni.uq_name);
		bool already_generated = (wrapper_map.find(wrapper_key) != wrapper_map.end());
		if (!already_generated) {
			Printf(CS,  "//_[wrapper name: NEW: %d] %s\n", __LINE__, wrapper_key);
			wrapper_map[wrapper_key] = 1;
		} else {
			Printf(CS,  "//_[wrapper name: DUP: %d] %s\n", __LINE__, wrapper_key);
			return;
		}
		***/

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_%s_addr_%s(HANDLE _this) {\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = &(*((%s*)_this)).%s; }\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_%s_set_%s(HANDLE _this, HANDLE _ptr) {\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CPP, "        try {\n");
		Printf(CPP, "            if (&(*((%s*)_this)).%s != _ptr) {\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "                vector<%s>* src = (vector<%s>*) _ptr;\n", ni.cpp_type, ni.cpp_type);
		Printf(CPP, "                vector<%s>* dst = &(*((%s*)_this)).%s;\n", ni.cpp_type, ci.cpp_name, ni.uq_name);
		Printf(CPP, "                copy(src->begin(), src->end(), back_inserter(*dst));\n");
		Printf(CPP, "            }\n");
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s_addr_%s(IntPtr _this);\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_%s_set_%s(IntPtr _this, IntPtr _ptr);\n", ci.uq_name, wrapper_type, ni.uq_name);
	}

	void generate_accessor_for_type_vector_struct(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: vector: struct");
		DUMP_NODE_INFO(fps, FD_ALL, "generate_wrapper_accessor_struct ci", ci);
		DUMP_NODE_INFO(fps, FD_ALL, "generate_wrapper_accessor_struct ni", ni);

		char* wrapper_name = make_wrapper_name(fps, FD_ALL, __LINE__, ni, ci, "vector");
		char* wrapper_type = make_wrapper_type(ni);
		char* cpp_type = strip_type_modifier(ni.cpp_type);
		char* type_name = make_wrapper_name_type_part(ni.uq_type, ni.pointer_level);
		
		// [cs]
		char* cs_type = cs_qualified_name(strip_type_modifier(ni.cs_type));
		Printf(CS,  "    public %s %s {\n", wrapper_name, ni.uq_name);
		Printf(CS,  "        get { return new %s(SprExport.Spr_%s_%s_get_%s(_this)); }\n", wrapper_name, ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CS,  "        set { SprExport.Spr_%s_%s_set_%s(_this, value._this); }\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CS,  "    }\n");

		/***  クラス名＋メンバ名で識別するようにしたので重複は生じない ***
		char wrapper_key[1024];
		sprintf_s(wrapper_key, sizeof(wrapper_key), "%s_%s_%s", ci.uq_name, wrapper_type, type_name);
		bool already_generated = (wrapper_map.find(wrapper_key) != wrapper_map.end());
		if (!already_generated) {
			Printf(CS,  "//_[wrapper name: NEW: %d] %s\n", __LINE__, wrapper_key);
			wrapper_map[wrapper_key] = 1;
		} else {
			Printf(CS,  "//_[wrapper name: DUP: %d] %s\n", __LINE__, wrapper_key);
			return;
		}
		***/

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_%s_get_%s(HANDLE _this) {\n", ci.uq_name, wrapper_type, ni.cpp_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = (vector<%s>*) &((%s*)_this)->%s; }\n", cpp_type, ci.uq_name, ni.cpp_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_%s_set_%s(HANDLE _this, HANDLE value) {\n", ci.uq_name, wrapper_type, ni.cpp_name);
		Printf(CPP, "        try { ((%s*)_this)->%s = *((std::vector<%s>*) value); }\n", ci.uq_name, ni.cpp_name, cpp_type);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s_get_%s(IntPtr _this);\n", ci.uq_name, wrapper_type, ni.cpp_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_%s_set_%s(IntPtr _this, IntPtr value);\n", ci.uq_name, wrapper_type, ni.cpp_name);
	}

	void generate_accessor_for_type_vector_string(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		// vector<strig> は未対応
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: vector: struct");
		SNAP_ANA_PATH1(fps, FD_ALL, "@@ sorry not implemented @@");
	}

	void generate_accessor_for_type_array_intrinsic(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: array: intrinsic");
		DUMP_NODE_INFO(fps, FD_ALL, "accessor: array: intrinsic: ci:", ci);
		DUMP_NODE_INFO(fps, FD_ALL, "accessor: array: intrinsic: ni:", ni);

		char* wrapper_name = make_wrapper_name(fps, FD_ALL, __LINE__, ni, ci, "array");
		char* wrapper_type = make_wrapper_type(ni);
		char* type_name = make_wrapper_name_type_part(ni.uq_type, ni.pointer_level);

		// [cs]
		Printf(CS,  "        public %s %s {\n", wrapper_name, ni.cs_name);
		Printf(CS,  "            get { %s wapper = new %s(SprExport.Spr_%s_%s_addr_%s(_this), %s);\n", wrapper_name, wrapper_name, ci.uq_name, wrapper_type,ni.uq_name, ni.array_size);
		Printf(CS,  "                  return wapper;\n");
		Printf(CS,  "            }\n");
		Printf(CS,  "            set { SprExport.Spr_%s_%s_set_%s(_this, value); }\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CS,  "        }\n");
		if (is_already_generated(ni, ci)) return;

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_%s_addr_%s(HANDLE _this) {\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = &((%s*) _this)->%s[0]; }\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		int size = atoi(ni.array_size);
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_%s_set_%s(HANDLE _this, HANDLE _ptr) {\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CPP, "        try {\n");
		Printf(CPP, "            if (&(*((%s*)_this)).%s != _ptr) {\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "                %s* src = (%s*) _ptr;\n", ni.cpp_type, ni.cpp_type);
		Printf(CPP, "                %s* dst = &((%s*) _this)->%s[0];\n", ni.cpp_type, ci.cpp_name, ni.uq_name);
		Printf(CPP, "                memcpy((void*) dst, (const void*) src, sizeof(((%s*) _this)->%s));\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "            }\n");
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s_addr_%s(IntPtr _this);\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_%s_set_%s(IntPtr _this, IntPtr _ptr);\n", ci.uq_name, wrapper_type, ni.uq_name);
	}

	void generate_accessor_for_type_array_struct(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: array: struct");
		char* wrapper_name = make_wrapper_name(fps, FD_NULL, __LINE__, ni, ci, "array of struct");
		char* wrapper_type = make_wrapper_type(ni);
		char* cs_type = cs_qualified_name(strip_type_modifier(ni.cs_type));

		// [cs]
		Printf(CS,  "\tpublic %s %s {\n", wrapper_name, ni.cs_name);
		Printf(CS,  "\t    get { %s wrapper = new %s(SprExport.Spr_%s_%s_addr_%s(_this), %s);\n", wrapper_name, wrapper_name, ci.uq_name, wrapper_type, ni.uq_name, ni.array_size);
		Printf(CS,  "\t          return wrapper;\n");
		Printf(CS,  "\t    }\n");
		Printf(CS,  "\t    set { SprExport.Spr_%s_%s_set_%s(_this, value); }\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CS,  "\t}\n");

		// [cpp]
		Printf(CPP, "    __declspec(dllexport) HANDLE __cdecl Spr_%s_%s_addr_%s(HANDLE _this) {\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CPP, "        HANDLE _ptr = NULL;\n");
		Printf(CPP, "        try { _ptr = &((%s*) _this)->%s[0]; }\n", ci.uq_name, ni.uq_name);
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "        return _ptr;\n");
		Printf(CPP, "    }\n");
		int size = atoi(ni.array_size);
		Printf(CPP, "    __declspec(dllexport) void __cdecl Spr_%s_%s_set_%s(HANDLE _this, HANDLE _ptr) {\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CPP, "        try {\n");
		Printf(CPP, "            if (&(*((%s*)_this)).%s != _ptr) {\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "                %s* src = (%s*) _ptr;\n", ni.cpp_type, ni.cpp_type);
		Printf(CPP, "                %s* dst = &((%s*) _this)->%s[0];\n", ni.cpp_type, ci.cpp_name, ni.uq_name);
		Printf(CPP, "                memcpy((void*) dst, (const void*) src, sizeof(((%s*) _this)->%s));\n", ci.cpp_name, ni.uq_name);
		Printf(CPP, "            }\n");
		Printf(CPP, "        }\n");
		Printf(CPP, "        %s\n", CATCH_code);
		Printf(CPP, "    }\n");

		// [csp]
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern IntPtr Spr_%s_%s_addr_%s(IntPtr _this);\n", ci.uq_name, wrapper_type, ni.uq_name);
		Printf(CSP, "\t%s\n", DLLIMPORT);
		Printf(CSP, "\tpublic static extern void Spr_%s_%s_set_%s(IntPtr _this, IntPtr _ptr);\n", ci.uq_name, wrapper_type, ni.uq_name);
	}

	void generate_accessor_for_type_array_string(DOHFile* fps[], NodeInfo& ni, NodeInfo& ci) {
		// string[] は未対応
		SNAP_ANA_PATH1(fps, FD_ALL, "accessor: vector: struct");
	}


	void generate_string_get(DOHFile* file, char* indent, char* result, char* c_name, char* v_name) {
		if (c_name) {
			Printf(file, "%sstring str = ((%s*) _this)->%s;\n", indent, c_name, v_name);
			Printf(file, "%sconst char* cstr = str.c_str();\n", indent);
		} else {
			Printf(file, "%sconst char* cstr = %s.c_str();\n", indent, v_name);
		}
		Printf(file, "%sint lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);\n", indent);
		Printf(file, "%sif (lenW > 0) {\n", indent);
		Printf(file, "%s    %s = ::SysAllocStringLen(0, lenW);\n", indent, result);
		Printf(file, "%s    ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, %s, lenW);\n", indent, result);
		Printf(file, "%s}\n", indent);
	}

	void generate_string_set(DOHFile* file, int id, char* indent, char* value, char* c_name, char* v_name, bool push = false) {
		Printf(file, "%sint lenMB%d = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) %s, -1, NULL, 0, NULL, NULL);\n", indent, id, value);
		Printf(file, "%sif (lenMB%d > 0) {\n", indent, id);
		//Printf(file, "%s    LPCWSTR addr%d = ::SysAllocStringLen(0, lenMB%d);\n", indent, id, id);
		Printf(file, "%s    LPSTR addr%d = (LPSTR) ::SysAllocStringLen(0, lenMB%d);\n", indent, id, id);
		Printf(file, "%s    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) %s, -1, addr%d, lenMB%d, NULL, NULL);\n", indent, value, id, id);
		if (c_name) {
			if (push) {
				Printf(file, "%s    ((%s*) _this)->%s.push_back((string) addr%d);\n", indent, c_name, v_name, id);
			} else {
				Printf(file, "%s    ((%s*) _this)->%s = (string) addr%d;\n", indent, c_name, v_name, id);
			}
		} else {
			Printf(file, "%s    %s = (string) addr%d;\n", indent, v_name, id);
		}
		Printf(file, "%s}\n", indent);
	}

	NodeInfo& get_node_info(DOHFile* fps[], DOH* node, int use_type_string = 0) {
		NodeInfo* ni = new NodeInfo();
		memset(ni, 0, sizeof(NodeInfo));
		ni->no = ++node_no;
		DEBUG_BREAK_AT(*ni);
		ni->sym_name = Char(Getattr(node, "sym:name"));
		ni->name = Char(Getattr(node, "name"));
		ni->decl = Char(Getattr(node, "decl"));
		ni->access = Char(Getattr(node, "access"));
		ni->storage = Char(Getattr(node, "storage"));
		ni->kind = Char(Getattr(node, "kind"));
		ni->type = Char(Getattr(node, "type"));
		ni->uq_name = unqualified_name(ni->name, 1 /*force*/);
		ni->uq_type = unqualified_name(ni->type);
////		ni->overname = (Getattr(node, "defaultargs")) ? "defaultargs" : Char(Getattr(node, "sym:overname"));
		ni->overname = Char(Getattr(node, "sym:overname"));
		ni->is_function = (EQc(ni->kind, "function") && !EQc(ni->kind, "typedef")) ? 1 : 0;	// 実体のある関数 (=1)
		if (EQc(ni->kind, "typedef") && EQc(ni->storage, "typedef")) {
			ni->is_function = 2;	// typedef された関数 (=2)
		}
		ni->is_struct = EQc(ni->kind, "struct");
		if (use_type_string) {
			(void) analyze_syntax(fps, node, ni, ni->type, 0);
		} else {
			(void) analyze_syntax(fps, node, ni, ni->decl, 0);
		}
		//set_type_info(fps, ni, ni->uq_type);
		analyze_type_hook(ni);
		analyze_name_hook(ni);
		set_type_info(fps, ni, ni->uq_type);
		ni->is_void = EQc(ni->uq_type, "void") && !ni->is_pointer;
		ni->is_void_ptr = EQc(ni->uq_type, "void") && ni->is_pointer;
		ni->is_bool = EQc(ni->uq_type, "bool");
		ni->is_static = EQc(ni->storage, "static");
		ni->is_virtual = EQc(ni->storage, "virtual");
#if (HACK == 1)
		if (ni->is_function == 2) {
			if (EQ(ni->name, "TimerFunc")) {
				ni->num_args = 1;		//
			}
		}
		if (EQc(ni->uq_type, "UTTimerIf::TimerFunc") || EQc(ni->uq_type, "PHHapticEngineIf::Callback")) {
			ni->is_struct = 0;			//
			ni->is_intrinsic = 1;			//
			ni->is_void_ptr = 1;			//
		}
#endif
		// template で定義されたクラスの中に template を使った関数があるときの処理
		//	template の逆引きをする
		char* cstype = ni->cs_type;
		if (cstype && strstr(cstype, "<")) {
			string key(cstype);
			while (typedef_inverse_map.find(key) != typedef_inverse_map.end()) {
				key = typedef_inverse_map[key];
			}
			cstype = unqualified_name((char*) key.c_str());
			if (!EQ(cstype, ni->cs_type)) {
				PRINTinfo(gip, "typedef: inv: replace: %s -> %s\n", ni->cs_type, cstype);
				ni->cs_type = cstype;
			}
		}
		return *ni;
	}

	char* unqualified_name(char* type, int force = 0) {
		if (type == NULL) return NULL;
		char* p;
		if (force) {
			p = strrchr(type, ':');
			return p ? p+1 : type;
		}
		char* q = type;
		do {
			p = q;
			if (BEGINWITH(q, "Spr::")) q += 5;
			if (BEGINWITH(q, "std::")) q += 5;
		} while (q != p);
		return Char(NewString(p));
	}

	char* cs_qualified_name(char* type) {
		if (type == NULL) return NULL;
		if (!strchr(type, ':')) return type;
		char* buff = new char[strlen(type) + 1];
		char* s = type;
		char* d = buff;
		while (*s) {
			if (*s == ':') {
				*d++ = '.';
				s += 2;
			} else {
				*d++ = *s++;
			}
		}
		*d = '\0';
		char* cs_type = Char(NewString(buff));
		delete buff;
		return cs_type;
	}

	void set_type_info(DOHFile* fps[], NodeInfo* ni, char* p) {
		if (p == NULL) return;
		if (analyze_enum(ni, p)) return;
		analyze_vector(fps, ni, p);
		if (ni->is_function && EQ2(p, "q(")) {
			char* q = analyze_qualifier(fps, ni, p);
			if (q != p) {
				ni->type = q;
				ni->uq_type = unqualified_name(q);
				p = q;
			}
		}
		//
		TypeConv* tcp = &type_conv[0];
		while (tcp->kind > 0) {
			if (EQ(ni->uq_type, tcp->cpp_type)) {
				tcp->count++;
				break;
			}
			tcp++;
		}
		ni->uq_type = strip_paren(ni->uq_type);
		ni->cpp_type = analyze_cpp_type_repr(*ni);
		ni->cs_type = tcp->cs_type;
		ni->is_typedef = EQc(ni->kind, "typedef");
		ni->is_variable = EQc(ni->kind, "variable") || (tcp->kind == 1) || (tcp->kind == 2) || (tcp->kind == 3);
		ni->is_intrinsic = (tcp->kind == 1);
		ni->is_struct = (tcp->kind == 0);
		ni->is_string = EQc(ni->uq_type, "string") || (tcp->kind == 3);
		ni->cs_marshaltype = analyze_marshaltype(ni, tcp);

		if (ni->is_pointer && EQ(ni->uq_type, "char")) {
			ni->cs_type = "string";
		}
		analyze_cs_type_hook(ni);
	}

	char* analyze_marshaltype(NodeInfo* ni, TypeConv* tcp) {
		char* type = tcp->cs_marshaltype;
		if (ni->is_array) {
			char buff[128];
			if (ni->is_intrinsic) {
				sprintf(buff, "LPArray,SizeConst=%s", cs_qualified_name(ni->array_size));
				type = Char(NewString(buff));
			}
			else if (ni->is_string) {
				sprintf(buff, "LPArray,ArraySubType=UnmanagedType.LPWStr,SizeConst=%s", cs_qualified_name(ni->array_size));
				type = Char(NewString(buff));
			}
		}
		return type;
	}

	char* analyze_vector(DOHFile* fps[], NodeInfo* ni, char* p) {
		char* org = p;
		if (BEGINWITH(p, "std::")) p += 5;
		if (BEGINWITH(p, "vector")) {
			p += 6;
			while (*p == ' ' || *p == '<' || *p == '(') p++;
			char* basetype = analyze_get_id(p);
			if (basetype != NULL) {
				p += strlen(basetype);
				ni->is_vector++;
				ni->type = basetype;
				ni->uq_type = unqualified_name(basetype);
			}
			if (basetype == NULL || (!EQ2(p, ")>"))) {
				analyze_error(fps, __LINE__, "Invalid vector syntax: \"%s\"", p);
				p = analyze_skip(p);
			}
			p += 2;
		} else {
			p = org;
		}
		return p;
	}

	char* analyze_enum(NodeInfo* ni, char* p) {
		if (BEGINWITH(ni->type, "enum ")) {
			ni->type = ni->type + 5;
			ni->uq_type = unqualified_name(ni->type);
			ni->cs_type = cs_qualified_name(ni->uq_type);
			ni->is_variable = 1;
			ni->is_enum = 1;
			return p;
		}
		return NULL;
	}

	char* analyze_syntax(DOHFile* fps[], DOH* node, NodeInfo* ni, char* p, int nest = 0) {
		if (p == NULL) return NULL;
		while (*p) {
			if	(EQ2(p, "p."))	{ ni->pointer_level++; p += 2; }
			else if (EQ2(p, "r."))	{ ni->is_reference++; p += 2; }
			else if (EQ2(p, "f("))	{ p = analyze_function(fps, node, ni, p, nest); }
			else if (EQ2(p, "a("))	{ p = analyze_array(fps, ni, p); }
			else if (EQ2(p, "q("))	{ p = analyze_qualifier(fps, ni, p); }
			else if (nest > 0 && (p[0] == ',' || p[0] == ')')) break;
			else {
				p = analyze_vector(fps, ni, p);
				if (!ni->is_vector) {
					char* id = analyze_get_id(p);
					if (id != NULL) {
						p += strlen(id);
						ni->type = id;
						ni->uq_type = unqualified_name(id);
					} else {
						analyze_error(fps, __LINE__, "Identifier expected: \"%s\"", p);
						p = analyze_skip(p);
					}
				}
			}
		}
		//
		ni->is_array = (ni->array_size != NULL);
		if (EQc(ni->array_size, "0")) ni->array_size = "";
		ni->is_pointer = (ni->pointer_level > 0);
		//
		return p;
	}

	char* analyze_get_id(char* p) {
		while (*p == ' ') p++;
		char* q = p;
		int level = 0;
		while (isalnum(*q) || *q == '_' || *q == ':' || *q == '.' || *q == ' ' || *q == '<' || *q == '>' || *q == '(' || *q == ')' || *q == ',') {
			if (*q == '<') level++;
			else if (*q == '>') {
				level--;
				if (level <= 0) {
					q++;
					break;
				}
			}
			else if (*q == ')' && level == 0) break;
			else if (*q == ',' && level == 0) break;
			q++;
		}
		if (q == p) return NULL;
		char* id = Char(NewString(p));
		id[q - p] = '\0';
		return id;
	}

	char* analyze_function(DOHFile* fps[], DOH* node, NodeInfo* ni, char* p, int nest) {
		analyze_init_funcargs(ni, p);
		p += 2;		// just after "f("
		Node* pp = Getattr(node, "parms");
		int pi = 0;
		while (pp) {
			NodeInfo& arg = get_node_info(fps, pp, /* use_type_string */ 1);
			ni->funcargs[pi++] = arg;
			pp = nextSibling(pp);
		}
		int paren = 1;
		while (paren > 0) {
			if (*p == '(') paren++;
			else if (*p == ')') paren--;
			p++;
		}
		if (*p != '.') {
			analyze_error(fps, __LINE__, "Invalid node info syntax: \"%s\"", p);
			p = analyze_skip(p);
		} else {
			p++;
		}
		return p;
	}

	char* analyze_array(DOHFile* fps[], NodeInfo* ni, char* p) {
		p += 2;		// just after "a("
		char* size = NULL;
		if (isdigit(*p)) {
			size = Char(NewString(p));
			char* q = size;
			while (isdigit(*q)) q++;
			*q = '\0';
		} else if (*p == ')') {
			size = "0";
            		p--;    // size != NULL in this case
		} else {
			size = analyze_get_id(p);
		}
		if (size != NULL) {
			ni->array_size = unqualified_name(size);
			p += strlen(size);
		}
		if (EQ2(p, ").")) {
			p += 2;
		} else{
			analyze_error(fps, __LINE__, "Invalid array info syntax: \"%s\"", p);
			p = analyze_skip(p);
		}
		return p;
	}

	char* analyze_qualifier(DOHFile* fps[], NodeInfo* ni, char* p) {
		p += 2;		// just after "q("
		if (BEGINWITH(p, "const")) {
			ni->is_const++;
			p += 5;
		}
		if (EQ2(p, ").")) {
			p += 2;
		} else {
			analyze_error(fps, __LINE__, "Invalid qualifier syntax: \"%s\"", p);
			p = analyze_skip(p);
		}
		return p;
	}

	char* analyze_type_modifier(NodeInfo* ni, char* type) {
		int *flags[] = {		// MUST correspond to "char* type_modifiers[]"
			&ni->pointer_level,	// p.
			&ni->is_reference,	// r.
			&ni->is_const,		// q(coonst)
			&ni->is_const,		// qcoonst
			&ni->is_volatile,	// q(volatile)
			&ni->is_volatile,	// qvolatile
			NULL
		};
		char* stripped = strip_type_modifier_1(type, flags);
		if (ni->pointer_level > 0) ni->is_pointer = 1;
		return stripped;
	}

	char* strip_type_modifier(char* type, int cpp = 0) {
		if (type == NULL) return NULL;
		NodeInfo ni;
		string* str;
		char* op = strchr(type, '<');
		if (op) {
			char* cp = strrchr(op, '>');
			char* op_save = op;
			if (cp == NULL) {
				return type;	// something wrong!
			}
			*op = *cp = '\0';
			str = new string("");
			str->append(unqualified_name(strip_type_modifier(type, cpp)));
			str->append("<");
			char* comma = strchr(op+1, ',');
			while (comma) {
				*comma = '\0';
				str->append(unqualified_name(strip_type_modifier(op+1, cpp)));
				str->append(",");
				op = comma;
				*comma = ',';
				comma = strchr(op+1, ',');
			}
			str->append(unqualified_name(strip_type_modifier(op+1, cpp)));
			str->append(">");
			str->append(unqualified_name(strip_type_modifier(cp+1, cpp)));
			//type = (char*) str->c_str();
			*op_save = '<';
			*cp = '>';
		}
		else {
			str = new string(strip_type_modifier_1(type));
			if (cpp) {
				memset(&ni, 0, sizeof(NodeInfo));
				analyze_type_modifier(&ni, type);
				for (int i = 0; i < ni.pointer_level; i++) { str->append("*"); }
				if (ni.is_reference) str->append("&");
			}
		}
		return (char*) str->c_str();
	}

	char* strip_type_modifier_1(char* type, int* flags[] = NULL) {
		if (type == NULL) return NULL;
		char* p = NULL;
		while (type != p) {
			p = type;
			const char** key = type_modifiers;
			for (int i = 0; key[i]; i++) {
				if (BEGINWITH(type, key[i])) {
					if (flags) (*flags[i])++;
					type += strlen(key[i]);
					break;
				}
			}
		}
		return type;
	}

	void analyze_type_hook(NodeInfo* ni) {
		analyze_type_hook_1(ni);
		for (int i = 0; i < ni->num_args; i++) {
			NodeInfo& ai = ni->funcargs[i];
			analyze_type_hook_1(&ai);
		}
	}

	void analyze_type_hook_1(NodeInfo* ni) {
		if (ni->type == NULL) return;
		char* td_name = ni->uq_type;
		//
		string key(td_name);
		while (typedef_map.find(key) != typedef_map.end()) {
			PRINTinfo(gip, "typedef: replace: %s -> %s\n", key.c_str(), typedef_map[key].c_str());
			key = typedef_map[key];
		}
		if (!EQ((char*) key.c_str(), td_name)) {
			td_name = (char*) key.c_str();
		}
		//
		td_name = unqualified_name(td_name);
		if (td_name) {
			if (EQ(td_name, "void") && !ni->is_pointer) {
				ni->uq_type = "void";
				ni->cpp_type = "void";
				ni->cs_type = "void";
			}
			else if (EQ(td_name, "string")) {
				ni->uq_type = "string";
				ni->cpp_type = "string";
				ni->is_string = 1;
			}
			else {
				ni->uq_type = td_name;
			}
		}
	}

	void analyze_cs_type_hook(NodeInfo* ni) {
		if (ni->cs_type == NULL) return;
		if (EQ(ni->cs_type, "object")) {
			// ni->cs_type = "IntPtr";
			// ni->cs_marshaltype = "SysUInt";
			ni->cs_type = cs_qualified_name(ni->uq_type);
			ni->cs_im_type = "IntPtr";
		} else if (EQ(ni->cs_type, "void") && ni->is_pointer) {
			ni->cs_type = "IntPtr";
			ni->cs_im_type = "IntPtr";
			ni->cs_marshaltype = "SysUInt";
		} else {
			ni->cs_type = cs_qualified_name(ni->cs_type);
			ni->cs_im_type = cs_qualified_name(ni->cs_type);
		}
	}

	void analyze_cs_type_hook_2(NodeInfo* ni, char* name) {
		char* d = strrchr(ni->cs_type, '.');
		if (d == NULL) return;
		int len1 = d - ni->cs_type;
		int len2 = strlen(name);
		if (len1 != len2 ) return;
		if (strncmp(ni->cs_type, name, len1) != 0) return;
		ni->cs_type = Char(NewString(d+1));
	}

	void analyze_name_hook(NodeInfo* ni) {
		static char* name_hook_table[] = {
			/* name, cpp_name, cs_name */
			// C# reserved word
			"checked", "checked", "checked_",
			"base", "base", "base_",
			"is", "is", "is_",
			"ptr", "ptr", "ptr_",
			// Kludge
			//	namespace
			"physics_material", "SprCOLLADA::physics_material", "physics_material",
			"physics_materialCommon", "SprCOLLADA::physics_materialCommon", "physics_materialCommon",
			"ip_address", "gimite::ip_address", "ip_address",
			"socket_address", "gimite::socket_address", "socket_address",
			"socket_streambuf", "gimite::socket_streambuf", "socket_streambuf",
			"socket_stream", "gimite::socket_stream", "socket_stream",
			"bound_socket", "gimite::bound_socket", "bound_socket",
			"server_stream_socket", "gimite::server_stream_socket", "server_stream_socket",
			"diagram_socket", "gimite::diagram_socket", "diagram_socket",
			NULL
		};
		if (ni->name == NULL) return;
		ni->cpp_name = ni->uq_name;
		ni->cs_name = replace_partial_string(ni->uq_name, "::", "_");
		for (int i = 0; name_hook_table[i]; i += 3) {
			if (EQ(ni->uq_name, name_hook_table[i])) {
				ni->cpp_name = name_hook_table[i+1];
				ni->cs_name = name_hook_table[i+2];
				break;
			}
		}
	}

	char* strip_paren(char* p) {
		if ((p == NULL) || (strchr(p, '(') == NULL)) return p;
		char* buff = Char(NewString(p));
		char* q = buff;
		while (*p) {
			if (*p != '(' && *p != ')') *q++ = *p;
			p++;
		}
		*q = '\0';
		return buff;
	}

	char* replace_partial_string(char* org, char* from, char* to) {
		char* str = new char[MAX_NAMELEN+1];
		int len1 = strlen(from);
		int len2 = strlen(to);
		char* s = org;
		char* d = str;
		while (*s) {
			if (strncmp(s, from, len1) == 0) {
				s += len1;
				for (int i = 0; i < len2; i++) { *d++ = to[i]; }
			} else {
				*d++ = *s++;
			}
			if (d >= str + MAX_NAMELEN) {
				abort(__LINE__, "buffer size too short (%d)", MAX_NAMELEN);
			}
		}
		*d = '\0';
		return str;
	}

	char* analyze_skip(char* p) {
		while (*p != '.') p++;
		return p + 1;
	}

	void analyze_init_funcargs(NodeInfo* ni, char* p) {
		ni->num_args = analyze_count_funcargs(p);
		if (ni->num_args > 0) {
			ni->funcargs = new struct nodeinfo_t[ni->num_args];
			memset(ni->funcargs, 0, sizeof(NodeInfo) * ni->num_args);
		}
	}

	int analyze_count_funcargs(char* p) {
		char* q = p + 2;	// just after "f("
		int level = 1;
		int count = (*q == ')') ? 0 : 1;
		while (*q) {
			if (*q == ')') { if (--level <= 0) break; }
			else if (*q == '(') level++;
			else if (*q == ',') count++;
			q++;
		}
		return count;
	}

	char* overname(NodeInfo& ni) {
		char* p = NULL;
		if (ni.overname) p = strrchr(ni.overname, '_');
		return (char*) ((p && !EQ(p, "_0")) ? p : "");
	}

	char* analyze_cpp_type_repr(NodeInfo& ni) {
		char buff[MAX_NAMELEN+1];
		if (strlen(ni.uq_type) + ni.pointer_level + ni.is_reference > MAX_NAMELEN) {
			Printf(stderr, "Error: %d: buffer size too short (%d)\n", __LINE__, MAX_NAMELEN);
			errorFlag = true;
			SWIG_exit(EXIT_FAILURE);
		}
		strcpy(buff, ni.is_const ? "const " : "");
		strcat(buff, ni.uq_type);
		for (int i = 0; i < ni.pointer_level; i++) strcat(buff, "*");
		if (ni.is_reference) strcat(buff, "&");
		return Char(NewString(buff));
	}

	void analyze_error(DOHFile* fps[], int line, char* fmt, void* ptr) {
		char buff[1024];
		sprintf(buff, "Error: %d: %s (at node %d)\n", line, fmt, node_no);
		Printf(stderr, buff, ptr);
		PRINTF(fps, FD_ALL, buff, ptr);
	}

	StructInfo* get_struct_info(DOHFile* fps[], Node* topnode, char* type) {
		StructInfo* info = new StructInfo;
		info->name = string_on_heap(type);
		info->num_members = 0;
		info->members = NULL;
		string name("Spr::");
		name.append(type);
		Node* n = FindNodeByAttrR(topnode, "name", (char*) name.c_str());
		if (n == NULL) {
			n = FindNodeByAttrR(topnode, "name", type);
		}
		if (n) {
			Node* c;
			int count = 0;
			for (c = firstChild(n); c; c = nextSibling(c)) {
				count++;
			}
			if (count > 0) {
				info->members = new StructMembersInfo*[count];
				if (info->members == NULL) return NULL;	// out of memory!
				NodeInfo& ni = get_node_info(fps, n);
				if (!ni.cpp_type) ni.cpp_type = ni.cpp_name;	// Kludge
				int ix = 0;
				for (c = firstChild(n); c; c = nextSibling(c)) {
					NodeInfo& ci = get_node_info(fps, c);
					//DUMP_NODE_INFO(fps, FD_CS, "struct info: member", ci);
					if (ci.is_function)	continue;
					if (!ci.is_variable)	continue;
					if (ci.is_enum)		continue;
					if (BEGINWITH(ci.type, "q(const).")) continue;
					char* cs_type = ci.cs_type;
					if (ci.is_vector || ci.is_array) {
						//cs_type = make_wrapper_name(NULL, NULL, NULL, ci, ni, "struct info", __LINE__);
						cs_type = make_wrapper_name(fps, FD_NULL, __LINE__, ci, ni, "struct info");
					}
					StructMembersInfo* member = new StructMembersInfo;
					member->node = c;
					member->name = string_on_heap(ci.uq_name);
					member->cs_name = string_on_heap(ci.cs_name);
					member->cs_type = string_on_heap(cs_type);
					member->cpp_name = string_on_heap(ci.cpp_name);
					member->cpp_type = string_on_heap(ci.cpp_type);
					member->is_bool = ci.is_bool;
					member->is_vector = ci.is_vector;
					member->is_array = ci.is_array;
					member->is_string = ci.is_string;
					member->is_struct = ci.is_struct;
					member->is_pointer = ci.is_pointer;
					member->is_reference = ci.is_reference;
					info->members[ix++] = member;
				}
				info->num_members = ix;
			}
		}
		return info;
	}

	void free_struct_info(StructInfo* info) {
		if (info == NULL) return;
		for (int i = 0; i < info->num_members; i++) {
			StructMembersInfo* mp = info->members[i];
			if (mp->name) free(mp->name);
			if (mp->cs_name) free(mp->cs_name);
			if (mp->cs_type) free(mp->cs_type);
		}
		if (info->members) free(info->members);
		free(info);
	}

	char* string_on_heap(const char* str1, const char* str2 = "") {
		char* alloc = new char[strlen(str1) + strlen(str2) + 1];
		if (!alloc) abort(__LINE__, "out of memory!");
		strcpy(alloc, str1);
		strcat(alloc, str2);
		return alloc;
	}

	char* element_type(const char* cname) {
		char ch = cname[strlen(cname)-1];
		if (ch == 'i') return "int";
		if (ch == 'f') return "float";
		if (ch == 'd') return "double";
		return "";	// something wrong!
	}

	char* dimension(const char* cname) {
		char tmp[2] = { ' ', '\0' };
		tmp[0] = cname[strlen(cname)-2];
		string str = tmp;
		return (char*) str.c_str();
	}

	string dimension_and_type(const char* cname) {
		char tmp[3] = { '3', ' ', '\0' };
		char ch = cname[strlen(cname)-2];
		if (isdigit(ch)) {
			tmp[0] = ch - '\0';
		}
		tmp[1] = cname[strlen(cname)-1];
		return string(tmp);
	}

	string strip_whites(string str) {
		const char* s = str.c_str();
		char* alloc = new char[str.length()+1];
		if (alloc == NULL) abort(__LINE__, "out of memory!");
		char* p = alloc;
		while (*s) {
			char ch = *s++;
			if (ch != ' ' && ch != '\t' && ch != '\n') *p++ = ch;
		}
		*p = '\0';
		string ret = alloc;
		free(alloc);
		return ret;
	}

	vector<string> split(string str, char sep) {
		vector<string> vec;
		int s = 0, e = 0;
		int l = (int) str.size();
		while (s < l) {
			e = s;
			while ((e < l) && (str[e] != sep)) e++;
			if (e >= l) break;
			vec.push_back(trim(str.substr(s, e-s)));
			s = e + 1;
		}
		if (e > s) vec.push_back(trim(str.substr(s, e-s)));
		return vec;
	}
	string trim(string str) {
		int s = 0;
		int l = (int) str.size();
		while ((s < l) && (str[s] == ' ')) s++;
		int e = l - 1;
		while ((e >= 0) && (str[e] == ' ')) e--;
		return str.substr(s, e-s+1);
	}

	string improve_type_representaion(string str1, char* str2, char* rtype, char* atype, char* btype) {
		string str = str1;
		str = replace_all(str, "E", tolower(str2[strlen(str2)-1]));
		str = replace_all(str, "C", tolower(str2[0]));
		str = replace_one_at(str, 0, "S", rtype[0]);
		str = replace_one_at(str, 0, "O", rtype[0]);
		str = replace_one_at(str, 1, "S", atype[0]);
		str = replace_one_at(str, 1, "O", atype[0]);
		str = replace_one_at(str, 2, "S", btype[0]);
		str = replace_one_at(str, 2, "O", btype[0]);
		return str;
	}

	string replace_all(string str, const char* from, const char to) {
		char to_buff[2] = { to, '\0' };
		int s = 0;
		while ((int) (s = str.find(from, s)) >= 0) {
			str.replace(s++, 1, to_buff);
		}
		return str;
	}

	string replace_one_at(string str, int at, const char* from, const char to) {
		char buff_to[2] = { (from[0] == 'S') ? (char) tolower(to) : to, '\0' };
		if (str.find(from, 0) == at) {
			str.replace(at, 1, buff_to);
		}
		return str;
	}

#if (GATHER_INFO == 1)
	void PRINTinfo(DOHFile* gip, const char* format, ...) {
		va_list args;
		va_start(args, format);
		char buff[1024];
		vsnprintf_s(buff, sizeof(buff), format, args);
		va_end(args);
		Printf(gip, buff);
	}
#else
	void PRINTinfo(DOHFile* gip, const char* format, ...) {
	}
#endif

#if (SNAP == 1)
#define	IS_SET(flag,mask)	((flag & mask) != 0)
	void PRINTF(DOHFile* fps[], int flag, const char* format, ...) {
		va_list args;
		va_start(args, format);
		char buff[1024];
		vsnprintf_s(buff, sizeof(buff), format, args);
		va_end(args);
		if (IS_SET(flag, FD_CPP)) Printf(fps[0], buff);
		if (IS_SET(flag, FD_CS )) Printf(fps[1], buff);
		if (IS_SET(flag, FD_CSP)) Printf(fps[2], buff);
		if (IS_SET(flag, FD_LOG)) Printf(log, buff);
		if (IS_SET(flag, FD_INFO)) Printf(gip, buff);
		if (IS_SET(flag, FD_ERR)) Printf(stderr, buff);
	}
#undef	IS_SET
#else
	void PRINTF(DOHFile* fps[], int flag, const char* format, ...) {
	}
#endif

	void abort(int line, const char* format, ...) {
		va_list args;
		va_start(args, format);
		char buff[1024];
		vsnprintf_s(buff, sizeof(buff), format, args);
		va_end(args);
		Printf(stderr, "Error: %d: %s\n", line, buff);
		errorFlag = true;
		SWIG_exit(EXIT_FAILURE);
	}

	void snap_at_line(DOHFile* fps[], int flag, int line) {
		PRINTF(fps, flag, "//_[line: %d]\n", line);
	}

	void snap_path_info(DOHFile* fps[], int flag, int line, char* msg, void* ptr = NULL) {
		PRINTF(fps, flag, "//_[%s: %d] %s\n", msg, line, ptr ? ptr : "");
	}

#if (DUMP == 1)
	void dump_node_info(DOHFile* fps[], int flag, int line, char* id, NodeInfo& ni, int nest = 0) {
		const int max_indent = 16;		// enough?
		if (nest > max_indent) nest = max_indent;
		char indent[2*max_indent + 1];
		for (int i = 0; i < 2*(nest+1); i++) {
			indent[i] = ' ';
		}
		indent[2*(nest+1)] = '\0';
		if (nest == 0)		PRINTF(fps, flag, "//+%d\n", ni.no);
		if (nest == 0)		PRINTF(fps, flag, "//_NodeInfo: %s: %d\n", id, line);
		if (ni.sym_name)	PRINTF(fps, flag, "//%ssym:name: %s\n", indent, ni.sym_name);
		if (ni.name)		PRINTF(fps, flag, "//%sname: %s\n", indent, ni.name);
		if (ni.decl)		PRINTF(fps, flag, "//%sdecl: %s\n", indent, ni.decl);
		if (ni.access)		PRINTF(fps, flag, "//%saccess: %s\n", indent, ni.access);
		if (ni.storage)		PRINTF(fps, flag, "//%sstorage: %s\n", indent, ni.storage);
		if (ni.kind)		PRINTF(fps, flag, "//%skind: %s\n", indent, ni.kind);
		if (ni.type)		PRINTF(fps, flag, "//%stype: %s\n", indent, ni.type);
		if (ni.uq_name)		PRINTF(fps, flag, "//%suq_name: %s\n", indent, ni.uq_name);
		if (ni.uq_type)		PRINTF(fps, flag, "//%suq_type: %s\n", indent, ni.uq_type);
		if (ni.overname)	PRINTF(fps, flag, "//%sovername: %s\n", indent, ni.overname);
		if (ni.cpp_name)	PRINTF(fps, flag, "//%scpp_name: %s\n", indent, ni.cpp_name);
		if (ni.cpp_type)	PRINTF(fps, flag, "//%scpp_type: %s\n", indent, ni.cpp_type);
		if (ni.cs_type)		PRINTF(fps, flag, "//%scs_type: %s\n", indent, ni.cs_type);
		if (ni.cs_im_type)	PRINTF(fps, flag, "//%scs_im_type: %s\n", indent, ni.cs_im_type);
		if (ni.cs_name)		PRINTF(fps, flag, "//%scs_name: %s\n", indent, ni.cs_name);
		if (ni.cs_marshaltype)	PRINTF(fps, flag, "//%scs_marshaltype: %s\n", indent, ni.cs_marshaltype);
		if (ni.is_typedef)	PRINTF(fps, flag, "//%sis_typedef: %d\n", indent, ni.is_typedef);
		if (ni.is_variable)	PRINTF(fps, flag, "//%sis_variable: %d\n", indent, ni.is_variable);
		if (ni.is_intrinsic)	PRINTF(fps, flag, "//%sis_intrinsic: %d\n", indent, ni.is_intrinsic);
		if (ni.is_vector)	PRINTF(fps, flag, "//%sis_vector: %d\n", indent, ni.is_vector);
		if (ni.is_string)	PRINTF(fps, flag, "//%sis_string: %d\n", indent, ni.is_string);
		if (ni.is_function)	PRINTF(fps, flag, "//%sis_function: %d\n", indent, ni.is_function);
		if (ni.is_struct)	PRINTF(fps, flag, "//%sis_struct: %d\n", indent, ni.is_struct);
		if (ni.is_array)	PRINTF(fps, flag, "//%sis_array: %d, size %s\n", indent, ni.is_array, ni.array_size);
		if (ni.is_pointer)	PRINTF(fps, flag, "//%sis_pointer: %d, level %d\n", indent, ni.is_pointer, ni.pointer_level);
		if (ni.is_reference)	PRINTF(fps, flag, "//%sis_reference: %d\n", indent, ni.is_reference);
		if (ni.is_const)	PRINTF(fps, flag, "//%sis_const: %d\n", indent, ni.is_const);
		if (ni.is_volatile)	PRINTF(fps, flag, "//%sis_volatile: %d\n", indent, ni.is_volatile);
		if (ni.is_enum)		PRINTF(fps, flag, "//%sis_enum: %d\n", indent, ni.is_enum);
		if (ni.is_void)		PRINTF(fps, flag, "//%sis_void: %d\n", indent, ni.is_void);
		if (ni.is_void_ptr)	PRINTF(fps, flag, "//%sis_void_ptr: %d\n", indent, ni.is_void_ptr);
		if (ni.is_bool)		PRINTF(fps, flag, "//%sis_bool: %d\n", indent, ni.is_bool);
		if (ni.is_static)	PRINTF(fps, flag, "//%sis_static: %d\n", indent, ni.is_static);
		if (ni.is_virtual)	PRINTF(fps, flag, "//%sis_virtual: %d\n", indent, ni.is_virtual);
		if (ni.num_args)	PRINTF(fps, flag, "//%snum_args: %d\n", indent, ni.num_args);
		for (int i = 0; i < ni.num_args; i++) {
			PRINTF(fps, flag, "//%sarg %d:\n", indent, i+1);
			dump_node_info(fps, flag, line, "", ni.funcargs[i], nest+1);
		}
	}
#endif

#if (DUMP == 1)
	void dump_struct_info(DOHFile* fps[], int flag, int line, StructInfo* si) {
		PRINTF(fps, flag, "// _StructInfo: %s: %d\n", si->name, line);
		StructMembersInfo** members = si->members;
		for (int n = 0; n < si->num_members; n++) {
			StructMembersInfo* mp = members[n];
			PRINTF(fps, flag, "// member %d\n", n);
			if (mp->name)	 	 PRINTF(fps, flag, "//     name: %s\n", mp->name);
			if (mp->cs_name)	 PRINTF(fps, flag, "//     cs_name: %s\n", mp->cs_name);
			if (mp->cs_type)	 PRINTF(fps, flag, "//     cs_type: %s\n", mp->cs_type);
			if (mp->cpp_name)	 PRINTF(fps, flag, "//     cpp_name: %s\n", mp->cpp_name);
			if (mp->cpp_type)	 PRINTF(fps, flag, "//     cpp_type: %s\n", mp->cpp_type);
			if (mp->is_bool)	 PRINTF(fps, flag, "//     is_bool: %d\n", mp->is_bool);
			if (mp->is_vector)	 PRINTF(fps, flag, "//     is_vector: %d\n", mp->is_vector);
			if (mp->is_array)	 PRINTF(fps, flag, "//     is_array: %d\n", mp->is_array);
			if (mp->is_string)	 PRINTF(fps, flag, "//     is_string: %d\n", mp->is_string);
			if (mp->is_struct)	 PRINTF(fps, flag, "//     is_struct: %d\n", mp->is_struct);
			if (mp->is_pointer)	 PRINTF(fps, flag, "//     is_pointer: %d\n", mp->is_pointer);
			if (mp->is_reference)    PRINTF(fps, flag, "//     is_reference: %d\n", mp->is_reference);
		}
	}
#endif

#if (GATHER_INFO == 1)
	void print_nodes_info(char* title, Nodes nodes, char* item) {
		for(unsigned i = 0; i < nodes.size(); ++i) {
			PRINTinfo(gip, "%s: %s\n", title, Char(Getattr(nodes[i], item)));
		}
	}
#endif

#if (DUMP_TREE == 1 && GATHER_INFO == 1)
#define OKSTR(s)	((s) ? (s) : "-")
	void dump_tree(Node* root, int nest = 0) {
		if (tree_dumped) return;
		if (nest == 0) {
			PRINTinfo(gip, "---- tree dump start ----\n");
			char* name = Char(Getattr(root, "name"));
			char* kind = Char(Getattr(root, "kind"));
			PRINTinfo(gip, "tree: 0 %s %s %s\n", Char(nodeType(root)), OKSTR(name), OKSTR(kind));
		}
		Node* child = firstChild(root);
		while (child) {
			char* name = Char(Getattr(child, "name"));
			char* kind = Char(Getattr(child, "kind"));
			PRINTinfo(gip, "tree: %d ", nest+1);
			for (int i = 0; i <= nest; i++) Printf(gip, "--");
			PRINTinfo(gip, " %s %s %s\n", Char(nodeType(child)), OKSTR(name), OKSTR(kind));
			dump_tree(child, nest+1);
			child = nextSibling(child);
		}
		if (nest == 0) {
			PRINTinfo(gip, "---- end of tree dump ----\n");
			tree_dumped = 1;
		}
	}
#endif

	void debug_break_at(NodeInfo& node) {
		if (node.no == BREAK_NODE_NO) {
			int tmp = 0;
		}
	}

//==============================================
};

static Language *new_swig_sprcs() {
  return new SpringheadCs();
}
extern "C" Language *swig_sprcs(void) {
  return new_swig_sprcs();
}

#include "SprpyInfoObject.h"

namespace SwigSprpy
{;

typedef vector<char> Chars;
typedef struct _SwigSprpyVars
{
	const char *classnamePrefix;
	const char *filenamePrefix;
	const char *VARNAME_TUPLE;
	const char *VARNAME_SELF;
	const char *VARNAME_ARG;
	vector<string> UTRefList;
	set<Enum> enuminfos;
	Chars spacer;
} SwigSprpyVars;

class SprpyInfoObjectUtility;
struct Variable 
{
	SprpyInfoObjectUtility *Util;
	string cName;
	string cType;
	string cCode;
	string pyName;
	string pyType;
	string pyCode;
		
	Variable(SprpyInfoObjectUtility*);
	// C -> Python 変数の変換　　int => PyIntObject*   Vec3d => SPRPYVec3d*  Vec3d* => SPRPYVec3d*
	void WrapValue();

	// Python -> C 変数の変換　　PyIntObject* => int SPRPYVec3d* => Vec3d,
	void UnWrapValue();
};




struct CElem
{
	//クラスキーワード
	//class,struct,union   ,enum

	//装飾
	//* と &
	//cv 修飾子（const と volatile）
	//名前指定子（std:: など）
	//() 内の関数パラメータ
	//[] 内の配列の長さ
	//例外処理の指定
	//イニシャライザ

	// access storage cv classkeyword usigned nspace::classA::classB::type pr name
	//							                      ^^^^^^^^^^^^^^^^^^^^→fulltype
	// type<child> name
	string nspace;
	//string access;
	string storage;
	string cv;
	string classkeyword;
	string pr;	//* と & ,const*,const&も
	string type;
	string fulltype;
	string usigned; //unsigned
	//string name;
	struct CElem *child;//template
	CElem();
	CElem(CElem&);
	~CElem();
};

class SprpyInfoObjectUtility
{
public:
	SwigSprpyVars Vars;
	SprpyInfoObjectUtility();


	bool IsSpacer(char c);

	vector<string> ParmtoStrings( DOH* parm );

	vector<string> ParmtoStrings( const char* parm );

	const Enum* GetEnum( string cls );

	string TrimEnd( string str , string key);

	string ResolveTypedef(CElem cls);
	 
	string GetPySymbolName(string cls);
	
	string GetPyObjectName(string cls);

	int DelConst(string& cls);

	void DelRef(string& cls);
	bool IsRef(string& cls);

	void DelSpace(string& cls);

	int DelPointer(string &cls);

	void DelNamespace(string& cls);

	bool DelqConst(string& cls);

	void CreateCElem(string cls,CElem** alloc);

	CElem CreateCElem(string cls);

	int DelArray(string &cls);

	int CountPointer(string &cls);

	void StrReplaceall(string& str, string from , string to);

	/////型文字列から、Pythonの型チェック関数名を返す
	string GetCheckfuncName(string cls, string name);
	
	string GetPyTypeObjectName(string cls);

	SprClassType GetSprClassType(string cls);

};
}//namespace
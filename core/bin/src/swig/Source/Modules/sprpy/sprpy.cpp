/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * Springhead.cxx
 *
 * Interface stubs and type descriptions generator for Springhead2
 * ----------------------------------------------------------------------------- */

//for OutputDebugString
//#include <windows.h>

#include "../swigmod.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include "sprpy.h"
#include "SprpyInfoObject.h"
#include "SprpyInfoObjectUtility.h"

using namespace std;

#define CLASSNAME_PREFIX "EP"
#define FILENAME_PREFIX "EP"

#define PrintfTRY   Printf(cpp,"try{");
#define PrintfCATCH Printf(cpp,"}");\
                    Printf(cpp,"catch (const std::exception& e) {");\
                    Printf(cpp,"PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));");\
                    Printf(cpp,"return NULL;");\
                    Printf(cpp,"}\n");
#define PrintfCATCH_int \
		    Printf(cpp,"}");\
                    Printf(cpp,"catch (const std::exception& e) {");\
                    Printf(cpp,"PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));");\
                    Printf(cpp,"return 0;");\
                    Printf(cpp,"}\n");



static const char *usage = "\
Springhead Options (available with -spr)\n";

namespace SwigSprpy{;


//グローバル変数
//Utility
SprpyInfoObjectUtility Util;

#pragma region グローバル関数
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
	std::string DecodeType(DOHString* n, std::string& post){
		DOH* t = NewString( Getattr(n, "type") );
		Replaceall(t, ")>", " >");
		Replaceall(t, "<(", "< ");
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
			decl = decl.substr(9);
		}
		if (type.compare(0,9,"q(const).")==0){
			type = type.substr(9);
	//		if (refOrPtr) 
				type = string("const ").append(type);
	//		else post = "const";
		}

		if (decl.compare(0,2,"a(")==0){
			type.append("[");
			decl = decl.substr(2);
			int indexLen = decl.find(")");
			type.append( decl.substr(0,indexLen));
			type.append("]");
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

	string ToUpperString(string s)
	{
		for(int i = 0 ; i < (int)s.length() ; i++)
		{
			s[i] = toupper((int) s[i] );
		}
		return s;
	}

#pragma endregion
bool operator < (const Enum& a, const Enum& b){
	//return a.fullname.compare(b.fullname) < 0;
	return a.name.compare(b.name) < 0;
}

class Springhead:public Language {
	File *cpp, *hpp, *sprh, *log;
	bool errorFlag;
	int indent_level;
	
public:
	Springhead():indent_level(0),cpp(NULL), log(NULL), errorFlag(false)
	{
		Util.Vars.classnamePrefix = CLASSNAME_PREFIX;
		Util.Vars.filenamePrefix  = FILENAME_PREFIX;
		//UTRefを継承しているクラスのリスト(後方一致)
		Util.Vars.UTRefList.push_back("If");
		
		Util.Vars.VARNAME_SELF = "self";
		Util.Vars.VARNAME_ARG = "arg";
		Util.Vars.spacer.push_back(' ');
		Util.Vars.spacer.push_back('*');
		Util.Vars.spacer.push_back('+');
		Util.Vars.spacer.push_back('!');
		Util.Vars.spacer.push_back('^');
		Util.Vars.spacer.push_back('\\');
		Util.Vars.spacer.push_back('/');
		Util.Vars.spacer.push_back(';');
		Util.Vars.spacer.push_back(':');
		Util.Vars.spacer.push_back('[');
		Util.Vars.spacer.push_back(']');
		Util.Vars.spacer.push_back('(');
		Util.Vars.spacer.push_back(')');
		Util.Vars.spacer.push_back('{');
		Util.Vars.spacer.push_back('}');
		Util.Vars.spacer.push_back('\n');
		Util.Vars.spacer.push_back('\r');
		Util.Vars.spacer.push_back('&');
		Util.Vars.spacer.push_back('\"');
		Util.Vars.spacer.push_back('=');
		Util.Vars.spacer.push_back('|');
		Util.Vars.spacer.push_back('.');
		Util.Vars.spacer.push_back(',');
	}
	virtual ~ Springhead(){
	}
	
	/* Top of the parse tree */
	virtual int top(Node *top) {

		String *filename = NewString(Getattr(top, "outfile"));
		Replaceall(filename, "_wrap.cxx", "");

		String *cppfile;
		String *hppfile;
		String *sprhfile;
		string hppfileName;
		string cppfileName;
		string sprhfileName;

		hppfileName = GetString(filename);
		hppfileName.insert(0, Util.Vars.filenamePrefix);
		hppfileName.append(".h");

		cppfileName = GetString(filename);
		cppfileName.insert(0, Util.Vars.filenamePrefix);
		cppfileName.append(".cpp");

		sprhfileName = GetString(filename);
		sprhfileName.insert(0, Util.Vars.filenamePrefix);
		sprhfileName.insert(0, "Spr");
		sprhfileName.append(".h");

		hppfile = (void*) hppfileName.c_str();
		cppfile = (void*) cppfileName.c_str();
		sprhfile = (void*) sprhfileName.c_str();

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


		//モジュールは１つだけ
		Nodes module;
		FindNodeR(module, top, "module");

		//moduleがない場合のエラー
		if ( module.size() == 0 ) {Swig_error(input_file,line_number," モジュールの宣言が見つかりませんでした\n"); SWIG_exit(EXIT_FAILURE);}
		//moduleが複数の場合のエラー
		if ( module.size() >1 ) {Swig_error(input_file,line_number," .i モジュールの宣言が複数見つかりました\n"); SWIG_exit(EXIT_FAILURE);}

		string moduleNameStr = GetString(Getattr(module[0],"name")); const char* moduleName = moduleNameStr.c_str();

		Node* moduleTop;
		moduleTop = parentNode(module[0]);

		//.iファイル直書きのノードの親ノード（ FindNodeRではなく、 FindNodeで子ノードが検索される
		Nodes directTop;
		directTop.push_back(moduleTop);
		FindNode(directTop,moduleTop,"namespace");


		Nodes inserts;
		FindNodeR(inserts,moduleTop,"insert");

		Nodes incs;
		FindNodeR(incs,moduleTop,"include");
		//moduleTopもincludeノードなので除外する
		incs.erase( incs.begin() );



		/////////////// 以下iファイルでincludeされたクラス
		/////////////// Swigのソースに直書きされたものは未対応
		//クラスの列挙
		Nodes classes;
		for(unsigned i=0; i<directTop.size(); ++i){
			FindNode(classes, directTop[i], "class");
		}
		for(unsigned i=0; i<incs.size(); ++i){
			FindNodeR(classes, incs[i], "class");
		}
		//enum列挙
		Nodes enumNodes;
		for(unsigned i=0; i<directTop.size(); ++i){
			FindNode(enumNodes, directTop[i], "enum");
		}
		for(unsigned i=0 ; i < incs.size(); i++){ 
			FindNodeR(enumNodes, incs[i], "enum");
		}
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
			Util.Vars.enuminfos.insert(en);
		}
		

		//typedef列挙
		std::map<string,string> typedefs;
		Nodes typedefNodes;
		for(unsigned i=0; i<directTop.size(); ++i){
			FindNode(typedefNodes, directTop[i], "cdecl");
		}
		for(unsigned i=0; i<incs.size(); i++){
			FindNodeR(typedefNodes,incs[i],"cdecl");
		}
		for ( unsigned j = 0 ; j < typedefNodes.size() ; j++)
		{
			Node* n = typedefNodes[j];
			if(Cmp(Getattr(n,"kind"),"typedef") == 0)
			{
				string name(GetString(Getattr(n,"name")));
				string value(GetString(Getattr(n,"type")));
					
				typedefs.insert( map<string,string>::value_type( name, value ));
			}
		}

		//グローバル変数列挙
		Nodes gVariableNodes;
		{
			Nodes tempNodes;
			for(unsigned i=0; i<directTop.size(); ++i){
				FindNode(tempNodes, directTop[i], "cdecl");
			}
			for(unsigned i=0 ; i < incs.size(); i++)
			{
				FindNodeR(tempNodes, incs[i], "cdecl");
			}
			for(unsigned i=0; i<tempNodes.size(); ++i){
				if(!GetFlagAttr(tempNodes[i],"feature:ignore") && Checkattr(tempNodes[i],"kind","variable") && !GetFlagAttr(tempNodes[i],"ismember"))
					gVariableNodes.push_back(tempNodes[i]);
			}
		}


		Nodes descs;
		Nodes intfs;
		Nodes vintfs;
		Nodes objs;
		Nodes objDefs;
		Nodes externalObjs;
		std::vector<bool> absts;
	


		//必要なノードを抜き出す
		vector<ClassInfo> classinfos;

#pragma region classのノードから必要な情報を作成
		for(unsigned i=0; i<classes.size(); ++i){
			Node* n = classes[i];
			Node* cn = firstChild(n);
			ClassInfo ci = ClassInfo::CreateClassInfo(n,moduleName);

			//%ignoreにあったら無視する
			if( GetFlagAttr(n,"feature:ignore") ) {
				ALERT(n,"ignored:'%%ignore'");
				continue;
			}
			
			//アクセス指定がpublicで無い場合は無視する
			if( Checkattr(n,"access","private") || Checkattr(n,"access","protected") ) {
				ALERT(n,"ignored:'not public'");
				continue;
			}

			/////// Node*からMethodInfoExを作成する。 function , constructor , extendノードにおいて作成される
			while ( cn )
			{
				//アクセス指定がpublicで無い場合は無視する
				if( Checkattr(cn,"access","private") || Checkattr(cn,"access","protected") ) {
					ALERT(cn,"ignored:'not public'");
					cn = nextSibling(cn);
					continue;
				}

				//%ignore
				//%extendのメソッド場合はignoreを無視
				if( GetFlagAttr(cn,"feature:ignore") && !Checkattr(cn,"nodeType","extend") ){
					ALERT(cn,"ignored");
					cn = nextSibling(cn);
					continue;
				}
				
				//function constructor
				if( Checkattr(cn,"kind","function") || Checkattr(cn,"nodeType","constructor") )
					ci.BuildMethodInfoEx(cn);

				//extend
				if( Checkattr(cn,"nodeType","extend") )
				{
					Node* ec = firstChild(cn);
					while( ec )
					{
						ci.BuildMethodInfoEx(ec);
						ec = nextSibling(ec);
					}
				}

				
				//member variable
				if( Checkattr(cn,"kind","variable") && GetFlagAttr(cn,"ismember") && Checkattr(cn,"access","public") )
				{
					ci.BuildMemberInfo(cn);
				}


				cn = nextSibling(cn);
			}

			///////// MethodInfo やMethodInfoExを埋める

			//Swigでは認識されない(.iファイルにかかれない)メソッドをMethodInfoなどに追加
			// （おもにGetIfInfo）
			ci.SetAdditionalMethods();

			//オーバーロードのフラグを元に、CreateMethodInfoで埋められなかった項目をうめる
			ci.UpdateMethodInfos();
			
			//MethodInfoなどの情報を元に関数テーブルなどを作成
			ci.UpdateClassInfo();

			classinfos.push_back(ci);
		}
#pragma endregion

////////////////////////////////////ファイル出力開始

		//////////////////////////////////////　APIヘッダーファイル  ////////////////////////////////
		{
			//インクルードガード
			string upper = "SPR";
			upper.append(Util.Vars.filenamePrefix);
			upper.append(moduleName);
			upper.append("_H");
			Printf(sprh,"#ifndef %s\n",ToUpperString(upper).c_str());
			Printf(sprh,"#define %s\n",ToUpperString(upper).c_str());


			// .iファイルの %begin  { #include ～～ %} を元ににインクルード
			for ( int i = 0 ; i < (int)inserts.size() ; i++ )
			{
				if(Checkattr(inserts[i],"section","begin"))
					Printf(sprh, Char( Getattr(inserts[i],"code") ) );
			}

			//グローバル変数の宣言
			for ( int i = 0 ; i < (int)gVariableNodes.size() ; i++)
			{
				Node *n = gVariableNodes[i];
				Printf(sprh, "extern %s %s;\n",Char( Getattr(n,"type") ),Char( Getattr(n,"sym:name"))); 
			}

#pragma region ////////////////////// クラスごとに

			//////typedefも列挙
			////for(vector<string>::iterator it = typedefs.begin() ; it != typedefs.end() ; it++)
			////{
			////	string str = *it;
			////	//PyObject構造体
			////	Printf(sprh,"%s\n" ,str );
			////}

			///////////////////////class毎に処理
			for(vector<ClassInfo>::iterator itc = classinfos.begin() ; itc != classinfos.end() ; itc++)
			{
				ClassInfo cls = *itc;

				////////////クラス始まり//////////////////
				Printf(sprh,"\n//{*********%s*******\n",cls.pysymbolName);
				//TypeObject
				Printf(sprh,"extern PyTypeObject %s;\n",cls.pytypeobjectName);

				//define (チェック関数 checkfunc
				Printf(sprh,"#define %s_Check(ob) PyObject_TypeCheck(ob, &%s)\n",cls.pysymbolName,cls.pytypeobjectName);

					
				//newPyObject
				Printf(sprh,"PyObject* new%s(const %s*);\n",cls.pysymbolName,cls.classNameEx);
		
				///クラス終わり
				Printf(sprh,"//}%s\n",cls.pysymbolName);
			}
#pragma endregion

			
			// .iファイルの %header  %{ extern int i; ～～ %} をここに挿入 （グローバル関数宣言など
			for ( int i = 0 ; i < (int)inserts.size() ; i++ )
			{
				if(Checkattr(inserts[i],"section","header"))
					Printf(sprh, Char( Getattr(inserts[i],"code") ) );
			}

			///////////////////////モジュールの初期化関数
			Printf(sprh,"void init%s(PyObject *rootModule = NULL) ;\n",moduleName);


			//インクルードガード終わり
			Printf(sprh,"#endif\n");
		}

		//////////////////////////////////　APIヘッダーファイル 終わり  ///////////////////////////


		//////////////////////////////////　ヘッダーファイル  ///////////////////////////
		{
			
			//インクルードガード
			string upper = "";
			upper.append(Util.Vars.filenamePrefix);
			upper.append(moduleName);
			upper.append("_H");
			Printf(hpp,"#ifndef %s\n",ToUpperString(upper).c_str());
			Printf(hpp,"#define %s\n",ToUpperString(upper).c_str());

			//// .iファイルの %begin  { #include ～～ %} を元ににインクルード
			//for ( int i = 0 ; i < inserts.size() ; i++ )
			//{
			//	if(Checkattr(inserts[i],"section","begin"))
			//		Printf(hpp, Char( Getattr(inserts[i],"code") ) );
			//}
			
#pragma region ////////////////////// クラスごとに
			for(vector<ClassInfo>::iterator itc = classinfos.begin() ; itc != classinfos.end() ; itc++)
			{
				ClassInfo cls = *itc;

				////////////クラス始まり//////////////////
				Printf(hpp,"\n//{*********%s*******\n",cls.pysymbolName);

				//runtime downcast 用
				Printf(hpp,"void to%s( EPObject* obj);\n",cls.pysymbolName);

				//プロトタイプ宣言
				for(map<string,MethodInfoEx>::iterator itmex = cls.methods.begin() ; itmex != cls.methods.end() ; itmex++)
				{
					MethodInfoEx methEx = (*itmex).second;
					if( methEx.methods.empty() ) continue;
					Printf(hpp,"PyObject* __PYDECL %s( %s );%s\n",methEx.pyfuncNameCode.c_str(),methEx.pyparamCode.c_str() , methEx.isOverloaded ? "//Overloaded":"");
					
					if( !methEx.methods.empty() && methEx.methods[0].node && Checkattr(methEx.methods[0].node,"code",""))
						Printf(hpp,"//%s\n",Char(Getattr(methEx.methods[0].node,"code")));
				}
				
				///クラス終わり
				Printf(hpp,"//}%s\n",cls.pysymbolName);
			}
#pragma endregion

			//
			//// .iファイルの %header  %{ extern int i; ～～ %} をここに挿入 （グローバル関数宣言など
			//for ( int i = 0 ; i < inserts.size() ; i++ )
			//{
			//	if(Checkattr(inserts[i],"section","header"))
			//		Printf(hpp, Char( Getattr(inserts[i],"code") ) );
			//}
			
			//インクルードガード終わり
			Printf(hpp,"#endif\n");
		}
		////////////////////////////// ヘッダーファイル 終わり  ///////////////////////////


		//////////////////////////////////////　ソースファイル  ////////////////////////////////
		//インクルードはバッチファイルで追加する


		//グローバル変数の定義
		Printf(cpp,"//*********** Decl Global variables ***********\n");
		for ( int i = 0 ; i < (int)gVariableNodes.size() ; i++)
		{
			Node *n = gVariableNodes[i];
			Printf(cpp, "%s %s;\n",Char( Getattr(n,"type") ),Char( Getattr(n,"sym:name"))); 
		}
		Printf(cpp,"\n\n");

		
#pragma region ///////////////////////class毎に処理
		for(vector<ClassInfo>::iterator itc = classinfos.begin() ; itc != classinfos.end() ; itc++)
		{
			ClassInfo cls = *itc;

			////////////クラス始まり//////////////////
			Printf(cpp,"\n//{*********%s*******\n",cls.pysymbolName);


			////////その１				
			////
			//FUNCTION :=  
			//
			//PyObject* PY_FUNCNAME( PY_PARAMS )		<- MethInfoEx   MethInfo
			//{
			//  PUBLIC_BODY;				# PUBLIC_BODY := UTAutoLock(CriticalSection); UNPACK_ARG;
			//	if(CHECK){ BODY; RETURN; }	# CHECK := (
			//	if(CHECK){ BODY; RETURN; }	# BODY := { CALL_FUNC_AND_ASSIGN ret= NULL } or { CALL_FUNC_AND_ASSIGN  ret = WrawpValue(ret_tmp); } 
			//								# CALL_FUNC_AND_ASSIGN := { ret_tmp=NULL; /* C Code */ ret_tmp= .... ; } or 
			//														  { ret_tmp=NULL; ret_tmp = FUNCNAME(PARAM) } or
			//														  { FUNCNAME(PARAM) }
			//	if(CHECK){ BODY; RETURN; }	# RETURN { if ( ret ) return ret; else Py_Retern_None; }
			//	Retrun_PyArgError;
			//}


			//constructor 
			{
				MethodInfoEx& methEx = *cls.constructors;
				Printf(cpp,"int __PYDECL %s( %s ) {",methEx.pyfuncNameCode.c_str() , methEx.pyparamCode.c_str() );
				{
                                        PrintfTRY
					//PUBLIC_BODY
					Printf(cpp,"%s\n",methEx.publicBodyCode.c_str());

					//コンストラクタの引数がNULLのとき　（C++側から呼ばれたとき
					Printf(cpp,"if(!%s) return 0;\n",Util.Vars.VARNAME_ARG);
					
					if ( methEx.methods.size() == 1 )
					{
						Printf(cpp,"%s %s\n",methEx.methods[0].bodyCode.c_str(),methEx.methods[0].returnCode.c_str());
					}
					else
					{
						for( unsigned i = 0 ; i < methEx.methods.size() ; i++)
						{
							Printf(cpp,"if(%s){%s %s}\n",methEx.methods[i].checkfuncCode.c_str(),methEx.methods[i].bodyCode.c_str(),methEx.methods[i].returnCode.c_str());
						}
						Printf(cpp,"PyErr_BadArgument(); return -1;\n");
					}
                                        PrintfCATCH_int
				}
				Printf(cpp,"}\n");

			}
			//function
			for(map<string,MethodInfoEx>::iterator itmex = cls.methods.begin() ; itmex != cls.methods.end() ; itmex++)
			{
				MethodInfoEx methEx = (*itmex).second;
				//有効な関数がない場合は出力せずにcontinue
				if( methEx.methods.empty() ) continue;

				Printf(cpp,"PyObject* __PYDECL %s( %s ) {",methEx.pyfuncNameCode.c_str() , methEx.pyparamCode.c_str() );
				{
                                        PrintfTRY
					//PUBLIC_BODY
					Printf(cpp,"%s\n",methEx.publicBodyCode.c_str());

					//static関数は引数が0 (NULLのPyObject*)なので、Nullチェック不要
					if( !methEx.isStatic ){
						//self->ptrがNULLのとき
						Printf(cpp,"if( EPObject_Ptr(%s) == NULL ){\n",Util.Vars.VARNAME_SELF,Util.Vars.VARNAME_SELF);
						Printf(cpp,"PyErr_SetString( PyErr_Spr_NullReference , \"Null Reference in %s.%s\");\n",methEx.className,methEx.methodName);
						Printf(cpp,"return NULL;\n}\n");
					}

					for( unsigned i = 0 ; i < methEx.methods.size() ; i++)
					{
						Printf(cpp,"if(%s){%s %s}\n",methEx.methods[i].checkfuncCode.c_str(),methEx.methods[i].bodyCode.c_str(),methEx.methods[i].returnCode.c_str());
					}

					Printf(cpp,"PyErr_BadArgument(); return NULL;\n");
                                        PrintfCATCH
				}
				Printf(cpp,"}\n");
			}




			//その２
			{
				/////////// getset関数定義
				Printf(cpp,"%s\n",cls.getsetMethodCode.c_str());


				///////////関数テーブル構造体書き出し
				Printf(cpp,"%s",cls.methodTable.c_str());
				Printf(cpp,"%s",cls.mathMethodTable.c_str());
				Printf(cpp,"%s",cls.getsetTable.c_str());
			

				///////////内部から呼ばれるdealloc,str,init,newなど
				Printf(cpp,"void __PYDECL %s_dealloc(PyObject* self){\n",cls.pysymbolName);
				{
					//デバッグ出力
					Printf(cpp,"#ifdef DEBUG_OUTPUT\n");
					Printf(cpp,"printf(\"%s dealloc called (MemoryManager=\");\n",cls.className);
					Printf(cpp,"if( ((EPObject*)self)->mm == EP_MM_SPR ) printf(\"Springhead)\\n\");\n");
					Printf(cpp,"else if( ((EPObject*)self)->mm == EP_MM_PY ) printf(\"Python)\\n\");\n"); 
					Printf(cpp,"#endif\n");

					//値渡しされたとき、値保持のためにデータをコピーする
					//UTRefが使える場合はUTRefで保持する
					if ( cls.isUTRef )
						Printf(cpp,"//UTRef will delete\n"); 
					else 
					{
#ifdef _WIN32
// g++ complains "deleting ‘const void*’ is undefined"...
// the result is unpredictable!
						//Pythonのメモリ管理だった場合消す
						Printf(cpp,"if ( ((EPObject*)self)->mm == EP_MM_PY ) ");
						Printf(cpp,"delete EPObject_Ptr(self);\n");
#endif
					}
					//自身を開放
					Printf(cpp,"self->ob_type->tp_free(self);\n");
				}
				Printf(cpp,"}\n");


				Printf(cpp,"PyObject* __PYDECL %s_str(){return Py_BuildValue(\"s\",\"This is %s.\");}\n",cls.pysymbolName,cls.pyobjectName);

				//EPObjectのメモリ確保
				//中のptrはNULL ( EP～_～という名前の関数（init関数）でptrの中身が作られる
				Printf(cpp,"PyObject* __PYDECL %s_new(PyTypeObject *type,PyObject *args, PyObject *kwds)\n{\n",cls.pysymbolName);
				{
                                        PrintfTRY
					Printf(cpp,"PyObject* self;\n");
					Printf(cpp,"self = type->tp_alloc(type,0);\n");
					Printf(cpp,"if ( self != NULL ) {\n");
					{
						Printf(cpp,"EPObject_Ptr(self) = NULL;\n");
						Printf(cpp,"((EPObject*)self)->mm = EP_MM_PY;\n");
						Printf(cpp,"return self;\n");
					}
					Printf(cpp,"}");
					Printf(cpp,"return PyErr_NoMemory();\n");
                                        PrintfCATCH
				}
				Printf(cpp,"}\n");

			}
			

			/////////その３
			{
				////////////////////TypeObject	
				Printf(cpp,"%s", cls.typeobjectCode.c_str());



				//外部から呼ばれるinit,new
				Printf(cpp,"void init%s(PyObject *rootModule)\n{\n",cls.pysymbolName);
				{
					Printf(cpp,"if ( PyType_Ready( &%s ) < 0 ) return ;//Pythonクラスの作成\n",cls.pytypeobjectName);

					//辞書に追加されるTypeObjectのインデックス
					string indexName = cls.className;
					if( cls.classType == SPR_CLASSTYPE_IF ) //～Ifの場合はIfを取り除く
						indexName = Util.TrimEnd(indexName,"If");
					Printf(cpp,"string package;");
					Printf(cpp,"if(rootModule) package = PyModule_GetName(rootModule);\n");
					Printf(cpp,"else{// rootModuleが渡された場合はEP_MODULE_NAMEは無視される\n");
					Printf(cpp,"#ifdef EP_MODULE_NAME\n");
					Printf(cpp,"package = EP_MODULE_NAME \".\";\n");
					Printf(cpp,"rootModule = PyImport_AddModule( EP_MODULE_NAME );\n");
					Printf(cpp,"#else\n");
					Printf(cpp,"package = \"\";\n");
					Printf(cpp,"rootModule = PyImport_AddModule(\"__main__\");\n");
					Printf(cpp,"#endif\n");
					Printf(cpp,"}\n");


					Printf(cpp,"#ifdef EP_USE_SUBMODULE\n");
					Printf(cpp,"PyObject *subModule = PyImport_AddModule( (package+\"%s\").c_str() );\n",moduleName);
					Printf(cpp,"Py_INCREF(subModule);\n");
					Printf(cpp,"PyModule_AddObject(rootModule,\"%s\",subModule);\n",moduleName);
					Printf(cpp,"#else\n");
					Printf(cpp,"PyObject *subModule = rootModule;\n");
					Printf(cpp,"#endif\n");


					Printf(cpp,"Py_INCREF(&%s);\n",cls.pytypeobjectName);
					Printf(cpp,"PyModule_AddObject(subModule,\"%s\",(PyObject*)&%s);\n",indexName.c_str(),cls.pytypeobjectName);


					////builtinとしてSpringheadを取り込む場合
					//Printf(cpp,"#ifdef EP_AS_BUILTIN\n");
					//Printf(cpp,"m = PyImport_ImportModule(\"builtins\");\n");
					//Printf(cpp,"Py_INCREF(&%s);\n",cls.pytypeobjectName);
					//Printf(cpp,"PyModule_AddObject(m,\"%s\",(PyObject *)&%s);\n",Util.TrimEnd(cls.className,"If").c_str(),cls.pytypeobjectName);
					////moduleとしてSpringheadを取り込む場合
					//Printf(cpp,"#else\n");
					//Printf(cpp,"m = PyImport_AddModule(\"%s\");\n",moduleName);
					//Printf(cpp,"Py_INCREF(&%s);\n",cls.pytypeobjectName);
					//Printf(cpp,"PyModule_AddObject(m,\"%s\",(PyObject *)&%s);\n",Util.TrimEnd(cls.className,"If").c_str(),cls.pytypeobjectName);
					//Printf(cpp,"#endif\n");
				}
				Printf(cpp,"}\n");
			
				//値渡しは基本的になし　
				//Vectorなどは例外的に .iファイルに記述する

				//new
				Printf(cpp,"PyObject* new%s(const %s* org)\n{\n",cls.pysymbolName, cls.classNameEx);
				{
                                        PrintfTRY
					Printf(cpp,"if(org == NULL) { Py_RETURN_NONE; }\n");
					Printf(cpp,"PyObject *ret = %s_new(&%s,NULL,NULL);\n",cls.pysymbolName,cls.pytypeobjectName);
					Printf(cpp,"EPObject_Ptr(ret) = org;\n");
					Printf(cpp,"((EPObject*)ret)->mm = EP_MM_SPR;\n");
					Printf(cpp,"return ret;\n");
                                        PrintfCATCH
				}
				Printf(cpp,"}\n");


				// runtime downcast用関数
				Printf(cpp,"void to%s( EPObject* obj){\n",cls.pysymbolName);
				{
					//Printf(cpp,"Py_DECREF(obj->ob_base.ob_type);\n");
					//Printf(cpp,"Py_INCREF(&%s);\n",cls.pytypeobjectName);
					//型情報はRefCountで管理されない？

					Printf(cpp,"obj->ob_base.ob_type = &%s;\n",cls.pytypeobjectName);
				}
				Printf(cpp,"}\n");


			}

			///クラス終わり
			Printf(cpp,"//}%s\n",cls.className);
		}
#pragma endregion

		// .iファイルの %wrapper %{  %} をここに挿入
		for ( int i = 0 ; i < (int)inserts.size() ; i++ )
		{
			if(Checkattr(inserts[i],"section","wrapper"))
				Printf(cpp, Char( Getattr(inserts[i],"code") ) );
		}


		Printf(cpp,"/**************** for Module ******************/\n");
		//モジュール初期化関数
		Printf(cpp,"void init%s(PyObject *rootModule)\n{\n",moduleName);
		
		//Utilityモジュールの場合はEPObjectの初期化をついか　
		if ( moduleNameStr == "Utility" ) Printf(cpp,"initEPObject(rootModule);\n");
		for(vector<ClassInfo>::iterator itc = classinfos.begin() ; itc != classinfos.end() ; itc++)
		{
			Printf(cpp,"init%s(rootModule);\n",(*itc).pysymbolName);
			
			//runtime downcast用
			if( itc->classType == SPR_CLASSTYPE_IF )			
				Printf(cpp,"EPObject_RegistCastfunc(%s::GetIfInfoStatic(),to%s);\n",itc->classNameStr.c_str(),itc->pysymbolName); 
		}
		Printf(cpp,"}\n");
		//////////////////////////////////　ソースファイル 終わり  //////////////////////////////



		/////////////////////////////////////////////////////////////出力終わり/////////////////////////////////////////////////////////////////

		//	ツリーのダンプを出力
		string logfilename( string("swig_") + moduleNameStr + string(".log"));
		log = NewFile((DOH*)logfilename.c_str(), "w", NULL);
		if (!log) {
			FileErrorDisplay((DOH*)logfilename.c_str());
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
		Preprocessor_define("SWIGSPRPY 1", 0);
	}

	void DumpNode(DOHFile* file, Node *obj) {
		PrintIndent(file, 0);
		//	ノードの表示
		
		Printf(file, "+++ %s (0x%x) ----------------------------------------\n", nodeType(obj),obj);
		Iterator ki;
		Node *cobj;
		ki = First(obj);
		//	ノードのもつ属性の表示
		while (ki.key) {
			String *k = ki.key;
			if ((Cmp(k, "nodeType") == 0) ||
				(Cmp(k, "firstChild") == 0) || 
				(Cmp(k, "lastChild") == 0) ||
				(Cmp(k, "parentNode") == 0) || 
				(Cmp(k, "nextSibling") == 0) || 
				(Cmp(k, "previousSibling") == 0) || 
				(*(Char(k)) == '$')
				) {
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

				//baselistだけリストを展開
				else if (Cmp(k,"baselist") == 0 && DohIsSequence(attr)){
						Printf(file, "%-12s - { ", k);
						for(int i=0; i<DohLen(attr); ++i){
							DOH* item = DohGetitem(attr, i);
							if (DohIsString(item)){
								DOH* o = Str(item);
								Printf(file, "%s ", o);
							}
						}
						Printf(file, "}\n");
				}else if (DohIsMapping(attr) && false/*Mappingのダンプ無効*/){
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
			//Printf(file, " ");
			Printf(file, "\t");
		}
		if (l) {
			Printf(file, "|");
			Printf(file, " ");
		}
	}

};
};//namespace

static Language *new_swig_sprpy() {
	return new SwigSprpy::Springhead();
}
extern "C" Language *swig_sprpy(void) {
  return new_swig_sprpy();
}

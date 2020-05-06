#include "MethodInfo.h"
#include "SprpyInfoObjectUtility.h"
#include <sstream>



namespace SwigSprpy
{;

extern SprpyInfoObjectUtility Util;

MethodInfo::_MethodInfo():node(NULL),methodName(NULL),pymethodName(NULL),
	returnType(NULL),pyreturnType(NULL),isConstructor(false),isExtend(false),methodinfoex(NULL)
{
	//methodNameStr.assign("");
	//pymethodNameStr.assign("");
	//
	//checkfuncCode.assign("");
	//bodyCode.assign("");
	//returnCode.assign("");
	//paramCode.assign("");
	//params.assign("");
}
MethodInfo::_MethodInfo(const _MethodInfo& obj)
{
	assign(obj);
}

void MethodInfo::assign(const _MethodInfo& obj)
{
	node = obj.node;
	isConstructor = obj.isConstructor;
	isExtend = obj.isExtend;
	methodNameStr.assign(obj.methodNameStr);
	methodName = methodNameStr.c_str();
	extendCode = obj.extendCode;

	pymethodNameStr.assign(obj.pymethodNameStr);
	pymethodName = pymethodNameStr.c_str();
			
	params.assign(obj.params.begin(), obj.params.end());
	paramNames.assign(obj.paramNames.begin(),obj.paramNames.end());

	returnTypeStr.assign(obj.returnTypeStr);
	returnType = returnTypeStr.c_str();
	pyreturnTypeStr.assign(obj.pyreturnTypeStr);
	pyreturnType = pyreturnTypeStr.c_str();

	checkfuncCode.assign(obj.checkfuncCode);
	bodyCode.assign(obj.bodyCode);
	returnCode.assign(obj.returnCode);
	paramCode.assign(obj.paramCode);

	methodinfoex = obj.methodinfoex;
}

MethodInfo MethodInfo::CreateMethodInfo(Node* _node, const char* className)
{
	if(!_node) return MethodInfo();

	MethodInfo mi;
	mi.node = _node;
	mi.methodNameStr.assign(GetString(Getattr(mi.node,"sym:name") )) ;
	Util.DelNamespace(mi.methodNameStr);

	mi.methodName = mi.methodNameStr.c_str(); 
	mi.pymethodNameStr = Util.GetPySymbolName( className ); 
	mi.pymethodNameStr.append("_");
	mi.pymethodNameStr.append(mi.methodName); 
	mi.pymethodName = mi.pymethodNameStr.c_str();

	mi.checkfuncCode.assign("false");
	mi.bodyCode.assign("assert(0)");
	mi.returnCode.assign("Py_RETURN_NONE");
	mi.paramCode.assign(" ");

	if ( mi.methodNameStr.compare(className) == 0 ) mi.isConstructor = true;
	else mi.isConstructor = false;
		
	//DecodeTypeを使って関数の戻り値の型を取得
	string res;
	res = DecodeType(mi.node);
	mi.returnTypeStr.assign(res); 
	mi.returnType = mi.returnTypeStr.c_str();

	mi.pyreturnTypeStr = "PyObject*";
	mi.pyreturnType = mi.pyreturnTypeStr.c_str();
		
	//paramsの登録
	vector<string> tmp = Util.ParmtoStrings( Getattr(mi.node,"parms") );
	mi.params.assign(tmp.begin(),tmp.end());

	//paramにchar*を受け取るものがある関数は未対応（いまはconst char*のみ）
	for( vector<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		string s(*it);
		Util.DelSpace(s);
		if (s.compare("char*") == 0) {
			ALERT(_node,"constでないchar*を受け取る関数は未対応");
			return MethodInfo();
		}
		//if (s.find("&") != -1){
		//	ALERT(_node,"参照を受け取る関数は未対応");
		//	return MethodInfo();
		//}
	}

	return mi;
}

}
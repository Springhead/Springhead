#ifndef METHODINFO_H
#define METHODINFO_H

#include "../swigmod.h"
#include "sprpy.h"
#include <string>
#include <vector>
#include <map>

#include "MethodInfoEx.h"

using namespace std;
namespace SwigSprpy
{;

struct _MethodInfo
{
	Node* node;
	bool isConstructor;
	bool isExtend;
	const char* methodName;
	const char* pymethodName;
	const char* returnType;
	const char* pyreturnType;
	string methodNameStr;
	string pymethodNameStr;
	string returnTypeStr;
	string pyreturnTypeStr;

	vector<string> params;
	vector<string> paramNames;
	string extendCode;  //extendなどで与えられる実装コード
		
	//引数チェック関数の登録
	string checkfuncCode;
	//BODYの登録
	string bodyCode;
	//return文の登録
	string returnCode;
	//PARAM(型+変数名)の登録
	string paramCode;

	struct _MethodInfoEx *methodinfoex;

	_MethodInfo();
	_MethodInfo(const _MethodInfo& obj);
	void assign(const _MethodInfo& obj);

	static _MethodInfo CreateMethodInfo(Node* node, const char* className);
};
typedef _MethodInfo MethodInfo;

}

#endif

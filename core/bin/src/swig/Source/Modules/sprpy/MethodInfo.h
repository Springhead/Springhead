#ifndef METHODINFO_H
#define METHODINFO_H

#include "..\swigmod.h"
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
	string extendCode;  //extend‚È‚Ç‚Å—^‚¦‚ç‚ê‚éÀ‘•ƒR[ƒh
		
	//ˆø”ƒ`ƒFƒbƒNŠÖ”‚Ì“o˜^
	string checkfuncCode;
	//BODY‚Ì“o˜^
	string bodyCode;
	//return•¶‚Ì“o˜^
	string returnCode;
	//PARAM(Œ^+•Ï”–¼)‚Ì“o˜^
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
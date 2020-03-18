#ifndef METHODINFOEX_H
#define METHODINFOEX_H

#include "../swigmod.h"
#include "sprpy.h"
#include <string>
#include <vector>
#include <map>

#include "MethodInfo.h"

using namespace std;
namespace SwigSprpy
{;

struct _MethodInfo;

struct _MethodInfoEx 
{
	vector<_MethodInfo> methods;
	bool isOverloaded;
	bool isStatic;
	bool isConstructor;
	bool isMathMethod;

	vector<string> pyparams;
	vector<string> pyparamNames;

	const char* className;
	string classNameStr;
	const char* methodName;
	string methodNameStr;
	const char* pymethodName;
	string pymethodNameStr;

	//string returnTypeStr;
	//const char* returnType;
	//const char* pyreturnType;
	//string pyreturnTypeStr;

	string pyparamCode;
	string pyfuncNameCode;
	string publicBodyCode;
	
	struct _ClassInfo* classinfo;
				
	_MethodInfoEx();
	_MethodInfoEx(const _MethodInfoEx& obj);
	void assign(const _MethodInfoEx& obj);

	static _MethodInfoEx CreateMethodInfoEx(Node* _node ,const char* className);
};
typedef _MethodInfoEx MethodInfoEx;

}

#endif

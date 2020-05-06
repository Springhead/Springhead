#ifndef MODULEINFO_H
#define MODULEINFO_H

#include "..\swigmod.h"
#include "sprpy.h"
#include <string>
#include <vector>
#include <map>

#include "ClassInfo.h"

using namespace std;
namespace SwigSprpy
{;

struct _ModuleInfo
{
	string symbolStr;
	string moduleNameStr;
	vector<string> includeCodes;
	map<string,string> gVariable;	// <type,name>

	vector<ClassInfo> classinfos;
};
typedef _ModuleInfo ModuleInfo;


}

#endif
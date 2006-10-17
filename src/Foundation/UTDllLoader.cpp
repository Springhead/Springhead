#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "UTDllLoader.h"
#include <Base/BaseDebug.h>
#include <Windows.h>

namespace Spr {;
void UTDllLoader::Init(){
	module = NULL;
	dllName[0] = '\0';
	addPath[0] = '\0';
}

static char* GetEnv(char* buf, const char* var){
	int cur = 0;
	int bufCur = 0;
	while(var[cur]){
		if (var[cur] == '$' && var[cur+1] == '('){
			int start = cur+2;
			while(var[cur] && var[cur] != ')') cur ++;
			char varstr[1024];
			varstr[cur-start] = '\0';
			strncpy(varstr, var+start, cur-start);
			char* env = getenv(varstr);
			if (!env){
				DSTR << "UTDllLoader::Load fails to find environment value of '" << var << "'" << std::endl;
				return NULL;
			}
			strcpy(buf+bufCur, env);
			bufCur += strlen(env);
			cur ++;
		}else if (var[cur] == '%'){
			cur += 1;
			int start = cur;
			while(var[cur] && var[cur] != '%') cur ++;
			char varstr[1024];
			varstr[cur-start] = '\0';
			strncpy(varstr, var+start, cur-start);
			char* env = getenv(varstr);
			if (!env){
				DSTR << "UTDllLoader::Load fails to find environment value of '" << var << "'" << std::endl;
				return NULL;
			}
			strcpy(buf+bufCur, env);
			bufCur += strlen(env);
		}else{
			buf[bufCur] = var[cur];
			cur++;
			bufCur++;
		}
	}
	buf[bufCur] = '\0';
	return buf;
}

bool UTDllLoader::Load(const char* dllNameIn, const char* addPathIn){
	if (dllNameIn) strcpy(dllName, dllNameIn);
	if (addPathIn) strcpy(addPath, addPathIn);
	if (module && !dllNameIn && !addPathIn) return true;
	Cleanup();
	char dll[1024];
	GetEnv(dll, dllName);
	char path[1024];
	strcpy(path, addPath);
	char* token = strtok(path, ";");
	while(token){
		char fname[1024];
		GetEnv(fname, token);
		strcat(fname, "\\");
		strcat(fname, dll);
		module = LoadLibrary(fname);
		if (module){
			DSTR << "Module '" << fname << "' was loaded." << std::endl;
			return true;
		}
		token = strtok(NULL, ";");
	}
	module = LoadLibrary(dllName);
	if (module){
		DSTR << "Module '" << dllName << "' was loaded." << std::endl;
	}else{
		DSTR << "Unable to load '" << dllName << "'." << std::endl;
	}
	return module != NULL;
}
void UTDllLoader::Cleanup(){
	if (module) FreeLibrary((HMODULE)module);
}
void* UTDllLoader::GetProc(const char* name){
	if (!module) return NULL;
	return GetProcAddress((HMODULE)module, name);
}

}	//	namespace Spr

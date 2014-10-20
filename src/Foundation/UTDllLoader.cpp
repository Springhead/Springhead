#pragma hdrstop
#include "UTDllLoader.h"
#include <Base/BaseDebug.h>
#include <stdlib.h>
#ifdef _WIN32	
# include <Windows.h>
#else
# include <dlfcn.h>
#endif

namespace Spr {;
void UTDllLoader::Init(){
	module = NULL;
	dllName[0] = '\0';
	addPath[0] = '\0';
}

static char* GetEnv(char* buf, const char* var){
	size_t cur = 0;
	size_t bufCur = 0;
	while(var[cur]){
		if (var[cur] == '$' && var[cur+1] == '('){
			size_t start = cur+2;
			while(var[cur] && var[cur] != ')') cur ++;
			char varstr[1024];
			varstr[cur-start] = '\0';
			strncpy(varstr, var+start, cur-start);
			char* env = getenv(varstr);
			if (!env){
				env = "";
				DSTR << "UTDllLoader::Load fails to find environment value of '" << varstr << "' for resolving '" << var << "'" << std::endl;
			}
			strcpy(buf+bufCur, env);
			bufCur += strlen(env);
			cur ++;
		}else if (var[cur] == '%'){
			cur += 1;
			size_t start = cur;
			while(var[cur] && var[cur] != '%') cur ++;
			char varstr[1024];
			varstr[cur-start] = '\0';
			strncpy(varstr, var+start, cur-start);
			char* env = getenv(varstr);
			if (!env){
				env = "";
				DSTR << "UTDllLoader::Load fails to find environment value of '" << varstr << "' for resolving '" << var << "'" << std::endl;
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
	const size_t len = 1024*10;
	char dll[len];
	char path[len];
	char pathOrg[len];
	char pathNew[len];
	
	GetEnv(dll, dllName);
	GetEnv(path, addPath);
	sprintf(pathOrg, "PATH=%s", getenv("PATH"));
	sprintf(pathNew, "PATH=%s;%s", path, getenv("PATH"));
	putenv(pathNew);
	DSTR << "PathNew: " << pathNew << std::endl;
#ifdef _WIN32	
	module = LoadLibrary(dll);
#else
	module = dlopen(dll, RTLD_LAZY);
#endif		
	if (module){
		DSTR << "Module '" << dll << "' was loaded." << std::endl;
		return true;
	}
#ifdef _WIN32		
	module = LoadLibrary(dllName);
#else
	module = dlopen(dllName, RTLD_NOW | RTLD_GLOBAL);
#endif
	if (module){
		DSTR << "Module '" << dllName << "' was loaded." << std::endl;
	}else{
		DSTR << "Unable to load '" << dllName << "'." << std::endl;
	}
	putenv(pathOrg);

	return module != NULL;
}
void UTDllLoader::Cleanup(){
#ifdef _WIN32		
	if (module) FreeLibrary((HMODULE)module);
#else
	if (module) dlclose(module);
#endif
}
void* UTDllLoader::GetProc(const char* name){
	if (!module) { return NULL; };
#ifdef _WIN32
	return GetProcAddress((HMODULE)module, name);
#else
	return (void(*))dlsym(module, name);
#endif
}

}	//	namespace Spr

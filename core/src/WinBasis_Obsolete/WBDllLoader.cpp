/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "WinBasis.h"
#include "WBDllLoader.h"
#pragma hdrstop

namespace Spr {;
void WBDllLoader::Init(){
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
				DSTR << "WBDllLoader::Load fails to find environment value of '" << var << "'" << std::endl;
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
				DSTR << "WBDllLoader::Load fails to find environment value of '" << var << "'" << std::endl;
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

bool WBDllLoader::Load(const char* dllNameIn, const char* addPathIn){
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
void WBDllLoader::Cleanup(){
	if (module) FreeLibrary(module);
}
void* WBDllLoader::GetProc(const char* name){
	if (!module) return NULL;
	return GetProcAddress(module, name);
}

}	//	namespace Spr

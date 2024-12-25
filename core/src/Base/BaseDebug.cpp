/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Base.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

namespace Spr {

typedef void (DebugOutFunc)(const char*);
static DebugOutFunc* debugOutFunc;
static DebugOutFunc* csvOutFunc;


int SPR_CDECL DebugPrintf::PrintfFunc::operator ()(const char* fmt, ...){
	int rv = 0;
	if (debugOutFunc){
		va_list list;
		va_start(list, fmt);
		char buf[1024*10];
		rv = vsprintf(buf, fmt, list);
		debugOutFunc(buf);
		va_end(list);
	}
	return rv;
}
void DebugPrintf::Set(void (*out)(const char*)){
	debugOutFunc = out;
}

#ifdef _WIN32
void defDebugOutFunc(const char* str){
	OutputDebugString(str);
}
#else
 #ifdef __sh__
  #include <cyg/kernel/kapi.h>
 #endif
void defDebugOutFunc(const char* str){
	printf("%s", str);
	#ifdef __sh__
	cyg_thread_delay(200);
	#endif
}
#endif



DebugPrintf* DebugPrintf::GetInstance(){
	static DebugPrintf dpf;
	if (!debugOutFunc) dpf.Set(defDebugOutFunc);
	return &dpf;
}

class DPFStreambuf:public UTStreambufBase{
public:
#ifdef _MSC_VER
	DPFStreambuf(std::_Uninitialized):UTStreambufBase(std::_Noinit){}
#endif
	DPFStreambuf(char* gb, size_t gl, char* pb, size_t pl):UTStreambufBase(gb, (int)gl, pb ,(int)pl){
	}
	virtual unsigned int read(void* buf, unsigned int bufLen){
		return 0;
	}
	virtual unsigned int write(void* src, unsigned int len){
		if(csvOutFunc){
			char buf[4096];
			if (len > sizeof buf - 1) len = sizeof buf -1;
			memcpy(buf, src, len);
			buf[len] = '\0';
			csvOutFunc(buf);
			csvOutFunc = NULL;
		}
		else if (debugOutFunc){
			char buf[4096];
			if (len > sizeof buf - 1) len = sizeof buf -1;
			memcpy(buf, src, len);
			buf[len] = '\0';
			debugOutFunc(buf);
		}else{
			len = 0;
		}
		return len;
	}
};

using namespace std;
class DPFStream:public std::ostream{
	char getbuf[1024];
	char putbuf[1024];
	DPFStreambuf buf;
public:
#ifdef _MSC_VER
	DPFStream(std::_Uninitialized): ostream(std::_Noinit), buf(std::_Noinit){}
#endif
	DPFStream():
	ostream(NULL),
	buf(getbuf, sizeof getbuf, putbuf, sizeof putbuf)
	{
		init(&buf);
//		std::streambuf* sbuf = rdbuf();
	}
};

#if defined _MSC_VER && (_MSC_VER == 1200)
static DPFStream DebugPrintf_dpfStr(std::_Noinit);
static bool DebugPrintf_initialized = false;
void SPR_CDECL DebugPrintf_Release(){
	DebugPrintf_dpfStr.DPFStream::~DPFStream();
}
std::ostream& DebugPrintf::Stream(){
	if (!DebugPrintf_initialized){
		new(&DebugPrintf_dpfStr) DPFStream();
		DebugPrintf_initialized = true;
		atexit(DebugPrintf_Release);
	}
	DebugPrintf_dpfStr.flush();
	DebugPrintf_dpfStr.clear();
	return DebugPrintf_dpfStr;
}
#else
std::ostream& DebugPrintf::Stream(){
	static DPFStream DebugPrintf_dpfStr;
	DebugPrintf_dpfStr.flush();
	DebugPrintf_dpfStr.clear();
	return DebugPrintf_dpfStr;
}
#endif

//デバッグ用 CSV出力ストリーム----------------------------------------------------------------

static bool CSVFileOpenFlag;
DebugCSV* DebugCSV::instance;

void DebugCSV::defcsvOutFunc(const char* str){
	instance->fout<<str;
}

DebugCSV* DebugCSV::GetInstance(){
	static DebugCSV dpf;
	instance = &dpf;
	if (!csvOutFunc) dpf.Set(defcsvOutFunc);
	return &dpf;
}

void DebugCSV::Set(void (*out)(const char*)){
	csvOutFunc = out;
}

std::ostream& DebugCSV::Stream(){
	static DPFStream DebugCSV_dpfStr;
	DebugCSV_dpfStr.flush();
	DebugCSV_dpfStr.clear();

	if(CSVFileOpenFlag == false){
		std::string name = FileNameSearch(); //新規のファイル名を指定
		fout.open(name.c_str());
		CSVFileOpenFlag = true;
	}
	return DebugCSV_dpfStr;
}

void DebugCSV::Close(){
	fout.close();
	CSVFileOpenFlag = false;
}

std::string DebugCSV::FileNameSearch(){
	std::stringstream filenames;	
	std::ifstream ifs;

	for(int No=1;;No++){
		filenames<<"DebugData"<<No<<".csv";
		ifs.open(filenames.str().c_str());
	if(!ifs){
		ifs.close();
		return filenames.str();
	}
	filenames.str(""); // バッファをクリアする。
	ifs.close();
	}
	
}




}	//	namespace Spr

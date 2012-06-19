/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef STRBUFBASE_H
#define STRBUFBASE_H
#if defined _MSC_VER && _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <Base/Env.h>
#include <cstdio>
#include <iostream>
/**	@file StrBufBase.h iostream を派生するときに使うクラスの定義 */
#ifdef __BORLANDC__
 #define BCC_CDECL __cdecl
#else
 #define BCC_CDECL
#endif

namespace Spr {

template <class T>
class UTPrintfStream:public T
	{
	public:
	explicit UTPrintfStream(std::streambuf *sb)
		:T(sb)
		{
		}
	int SPR_CDECL printf(char* fmt, ...)
		{
		char buf[4096];
		//return vsprintf(buf, fmt, (char*)&fmt);
	 	va_list argp;
 		va_start(argp, fmt);
 		return vsprintf(buf, fmt, argp);
		}
	};
class SPR_DLL UTStreambufBase:public std::streambuf
	{
	char* pbuf,*gbuf;
	int plen, glen;
	public:
	int lahead;
#ifdef _MSC_VER
	UTStreambufBase(std::_Uninitialized);
#endif
	UTStreambufBase(char* gb, int gl, char* pb, int pl);
    BCC_CDECL ~UTStreambufBase();
	protected:
	virtual int BCC_CDECL overflow(int c = EOF);
	virtual int BCC_CDECL underflow();
	virtual int BCC_CDECL sync();
	virtual pos_type BCC_CDECL seekoff(off_type off, std::ios_base::seekdir way,
		std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
	//	新しいストリームを派生させるには、以下の３つの関数をオーバーロードしてください。
	virtual unsigned int read(void* buf, unsigned int bufLen)=0;	//	bufLen 以内の文字数入力し buf に格納する。入力文字数を返す。
	virtual unsigned int write(void* buf, unsigned int bufLen)=0;	//	buf に格納された bufLen 文字のデータを出力し、出力した文字数を返す。
	virtual int in_avail_stream(){return 0;}		//	入力可能な文字数を返す。
	};

}	//	namespace Spr
#endif

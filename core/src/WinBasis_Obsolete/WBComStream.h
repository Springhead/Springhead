/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef COMSTREAM_H
#define COMSTREAM_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/StrbufBase.h>
#include "WBWin.h"
#include <iostream>

namespace Spr {;

class WBComStream;
class WBComStreambuf;

///	シリアルの設定
struct WBComStatus{
	enum {FC_NONE, FC_DTRDSR, FC_RTSCTS, FC_HARDWARE,
		FC_XONXOFF, FC_SOFTWARE=FC_XONXOFF} flowControl;
	int		port;
	DWORD	boudRate;
	BYTE	byteSize;
	BYTE	parity;
	BYTE	stopBits;
	WBComStatus();
};

class SPR_DLL WBComStreambuf:public UTStreambufBase, public WBComStatus
	{
	int error;
	friend WBComStream;
	public:
	enum {COMBUFLEN = 4096};	
	enum {ASCII_XON = 0x11, ASCII_XOFF = 0x13};
	WBComStreambuf(const WBComStatus& cs,
		char* gb, int gl, char* pb, int pl);
	BCC_CDECL ~WBComStreambuf();
	protected:
	HANDLE hFile;
	bool init();
	bool init(const WBComStatus& cs);
	bool openConnection();
	bool setupConnection();
	void closeConnection();
	void cleanup();
	UINT read(void* buf, UINT bufLen);
	UINT write(void* buf, UINT bufLen);
	UINT comAvail();
	UINT comWaiting();
	virtual int in_avail_stream()
		{
		return comAvail();
		}
};
///	シリアル通信ストリーム
class SPR_DLL WBComStream:public std::iostream{
	char getbuf[1024];
	char putbuf[1024];
public:
	WBComStatus comStatus;
	WBComStream(const WBComStatus& cs);
	WBComStream();
	BCC_CDECL ~WBComStream();
	WBComStreambuf buf;
	int avail();						///<	読み出しバッファに溜まっているバイト数を返す．
	bool init(const WBComStatus& cs);	///<	初期化．
	};
}	//	namespace Spr
#endif

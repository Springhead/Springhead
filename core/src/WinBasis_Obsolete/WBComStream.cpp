/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "WinBasis.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "WBComStream.h"
#include <strstream>

namespace Spr {
using namespace std;
//----------------------------------------------------------------------------
//	WBComStatus
//
WBComStatus::WBComStatus()
	{
	flowControl	= FC_NONE;	//	フロー制御なし
	port		= 0;		//	COM1
	boudRate	= 9600;		//	9600bps
	byteSize	= 8;		//	8bit
	parity		= 0;		//	パリティなし
	stopBits	= 0;		//	ストップビット１
	}

//----------------------------------------------------------------------------
//	WBComStreamBuf
//
WBComStreambuf::WBComStreambuf(const WBComStatus& cs, char* gb, int gl, char* pb, int pl):
	UTStreambufBase(gb, gl, pb, pl),
	WBComStatus(cs)
	{
	hFile = NULL;
	}

BCC_CDECL WBComStreambuf::~WBComStreambuf()
	{
	}

bool WBComStreambuf::init(const WBComStatus& cs)
	{
	*(WBComStatus*)this = cs;
	return init();
	}
bool WBComStreambuf::init()
	{
	if (!openConnection()) return false;
	if (!setupConnection())
		{
		closeConnection();
		return false;
		}
	return true;
	}

bool WBComStreambuf::openConnection()
	{
	// load the COM prefix string and append port number
	char szPort[256];
	ostrstream(szPort, 256) << "COM" << port+1 << '\0';

	// open COMM device
	hFile = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
			0,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,//|FILE_FLAG_OVERLAPPED,
			NULL );
	if (hFile == (HANDLE)-1 ) return false;

	// get any early notifications
	if (!SetCommMask ( hFile, EV_RXCHAR ))
		error = GetLastError();

	// setup device buffers
	if (!SetupComm( hFile, COMBUFLEN, COMBUFLEN )) 
		error = GetLastError();

	// purge any information in the buffer
	if (!PurgeComm( hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
		error = GetLastError();
	
	// できるだけ読み出しのタイムアウトは短くする。
	COMMTIMEOUTS commTimeOuts;
	if (!GetCommTimeouts( hFile, &commTimeOuts ))
		error = GetLastError();
	commTimeOuts.ReadIntervalTimeout = 1;
	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
	commTimeOuts.ReadTotalTimeoutConstant = 1;
	// CBR_9600 is approximately 1byte/ms. For our purposes, allow
	// double the expected time per character for a fudge factor.
	commTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/ boudRate;
	commTimeOuts.WriteTotalTimeoutConstant = 0 ;
	if (!SetCommTimeouts( hFile, &commTimeOuts ))
		error = GetLastError();
	return true;
	}

bool WBComStreambuf::setupConnection()
	{
	DCB	dcb;
	dcb.DCBlength = sizeof( DCB ) ;
	if (!GetCommState(hFile, &dcb))
		error = GetLastError();
	dcb.BaudRate	= boudRate;
	dcb.ByteSize	= byteSize;
	dcb.Parity		= parity;
	dcb.StopBits	= stopBits;
	// setup hardware flow control
	bool fSet;
	fSet = (flowControl == FC_DTRDSR || flowControl == FC_HARDWARE);
	dcb.fOutxDsrFlow = fSet;
	if (fSet) dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;
	else dcb.fDtrControl = DTR_CONTROL_ENABLE ;

	fSet = (flowControl == FC_RTSCTS || flowControl == FC_HARDWARE);
	dcb.fOutxCtsFlow = fSet ;
	if (fSet) dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
	else dcb.fRtsControl = RTS_CONTROL_ENABLE ;


	// setup software flow control
	fSet = (flowControl == FC_XONXOFF);
	dcb.fInX = dcb.fOutX = fSet ;
	dcb.XonChar = ASCII_XON ;
	dcb.XoffChar = ASCII_XOFF ;
	dcb.XonLim = 100 ;
	dcb.XoffLim = 100 ;
	// other various settings
/*
	dcb.fBinary = TRUE ;
*/
	dcb.fParity = TRUE ;
	if (SetCommState(hFile, &dcb) == 0)
		{
		error = GetLastError();
		return false;
		};
	return true;
	}

void WBComStreambuf::closeConnection()
	{
	if (!hFile) return;

	// drop DTR
	EscapeCommFunction( hFile, CLRDTR ) ;

	// purge any outstanding reads/writes and close device handle
	PurgeComm( hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
	CloseHandle( hFile ) ;
	hFile = NULL;
	return;
	}
void WBComStreambuf::cleanup()
	{
	closeConnection();
	}

UINT WBComStreambuf::read(void* buf, UINT bufLen)
	{
	DWORD len = std::min(bufLen, comAvail());
	ReadFile(hFile, buf, len, &len, NULL);
	return len;
	}

UINT WBComStreambuf::write(void* buf, UINT bufLen)
	{
	DWORD len = std::min(bufLen, COMBUFLEN - comWaiting());
	if ((int)len <= 0) return 0;
	WriteFile(hFile, buf, len, &len, NULL);
	return len;
	}
UINT WBComStreambuf::comAvail()
	{
	COMSTAT comStat;
	DWORD dwErrorFlags;
	ClearCommError(hFile, &dwErrorFlags, &comStat);
	return comStat.cbInQue;
	}
UINT WBComStreambuf::comWaiting()
	{
	COMSTAT comStat;
	DWORD dwErrorFlags;
	ClearCommError(hFile, &dwErrorFlags, &comStat);
	return comStat.cbOutQue;
	}

//----------------------------------------------------------------------------
//	WBComStream
//
WBComStream::WBComStream():
	buf(comStatus, getbuf, sizeof getbuf, putbuf, sizeof putbuf),
	iostream(&buf)
	{
	init(comStatus);
	}
WBComStream::WBComStream(const WBComStatus& cs):
	buf(cs, NULL,0,NULL,0),
	iostream(&buf)
	{
	init(cs);
	}
int WBComStream::avail()
	{
	if (!good()) return -1;
	return buf.in_avail() + buf.comAvail();
	}
bool WBComStream::init(const WBComStatus& cs)
	{
	buf.closeConnection();
	if (buf.init(cs))
		{
		clear();
		return true;
		}
	setf (ios::badbit);
	return false;
	}

BCC_CDECL WBComStream::~WBComStream()
	{
	buf.cleanup();
	}
}	//	namespace Spr

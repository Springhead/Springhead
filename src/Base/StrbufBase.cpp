/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Base.h"
#include <string.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace std;
namespace Spr {;
#ifdef _MSC_VER
UTStreambufBase::UTStreambufBase(std::_Uninitialized):streambuf(_Noinit){
}
#endif
BCC_CDECL UTStreambufBase::~UTStreambufBase(){}
UTStreambufBase::UTStreambufBase(char* gb, int gl, char* pb, int pl):
	streambuf()
	{
	gbuf = gb;	glen = gl;
	pbuf = pb;	plen = pl;
	if (gl > 16) lahead = 8;
	else if (gl > 8) lahead = 4;
	else lahead = 0;
	setbuf(pbuf, pl);
	setp(pbuf, pbuf+pl);
	setg(gbuf, gbuf+gl, gbuf+gl);
//	unbuffered(!pbuf);
	}

int BCC_CDECL UTStreambufBase::overflow(int c)
	{
	sync();
	if (c!=EOF)
		{
		if (pptr() < epptr()) sputc(c); // guard against recursion
		else if (write(&c,1)!=1) return EOF;
		}
	return(1);  // return something other than EOF if successful
	}
int BCC_CDECL UTStreambufBase::underflow()
	{
	int count;
	unsigned char tbuf;

	if (in_avail()) return (int)(unsigned char) *gptr();
	if ((egptr()-eback()-lahead) <= 0 || !egptr() || !eback()){
		if (read((void *)&tbuf,1)<=0) return EOF;
		return (int)tbuf;
	}

	if ((count=read((void *)(eback()), (unsigned)(egptr()-eback()-lahead))) <= 0){
		return EOF;     // reached EOF
	}
	setg(eback(),(egptr()-count),egptr());   // _gptr = _egptr - count
	memmove(gptr(), eback(), count);        // overlapping memory!
	return (int)(unsigned char) *gptr();
}
int BCC_CDECL UTStreambufBase::sync(){
	long count, nout;
	
	if (pbase() && epptr()-pbase()>0){
		if ((count=pptr()-pbase())!=0){
			while ((nout = write((void *) pbase(),(unsigned int)count)) != count){
				if (nout > 0){
					// should set _pptr -= nout
					pbump(-(int)nout);
					count -= nout;
					memmove(pbase(), pbase()+nout, (int)count);
				}else{
					return EOF;
				}
			}
			setp(pbuf, pbuf+plen);
		}
	}
	return(0);
}
UTStreambufBase::pos_type BCC_CDECL UTStreambufBase::seekoff(off_type off, ios_base::seekdir way,
		ios_base::openmode which)
	{
	if (which&ios_base::in && way==ios_base::cur)
		{
		if (off)
			{
			for(int i=0; i<off; i++) sgetc();
			if (off<0) return -1;
			}
		pos_type av = egptr() - gptr();
		av += in_avail_stream();
		return av;
		}
	return streambuf::seekoff(off, way, which);
	}
}	//	namespace Spr

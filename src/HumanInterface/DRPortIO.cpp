/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/DRPortIO.h>
#include <HumanInterface/DRWinDriver.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {

void WBGetPortIO(){
	static bool bFirst = true;
	if (bFirst){
		bFirst = false;
		HANDLE h;
		h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(h == INVALID_HANDLE_VALUE) {
			DRLoadWindowsDriver giveIo;
			giveIo.File("giveio.sys");
			giveIo.Install();
			giveIo.Start();
			h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(h == INVALID_HANDLE_VALUE) {
				DPF("Couldn't access giveio device\n");
			}
		}
		CloseHandle(h);
	}
}

#ifdef __BORLANDC__

unsigned char bcb_inp(unsigned short adr){
	unsigned char rv;
	asm{
		xor     eax,eax
		mov     dx, adr
		in      al,dx
		mov     rv, al
	}
	return rv;
}
void bcb_outp(unsigned short adr, unsigned char v){
	asm{
		xor     eax,eax
		mov     dx, adr
		mov     al, v
		out     dx, al
	}
}
unsigned short bcb_inpw(unsigned short adr){
	unsigned short rv;
	asm{
		xor     eax,eax
		mov     dx, adr
		in      ax, dx
		mov     rv, ax
	}
	return rv;
}
void bcb_outpw(unsigned short adr, unsigned short v){
	asm{
		xor     eax,eax
		mov     dx, adr
		mov     ax, v
		out     dx, ax
	}
}


#endif

}	//	namespace Spr;

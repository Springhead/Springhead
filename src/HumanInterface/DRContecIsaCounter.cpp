/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#if ( _MSC_VER > 1500 )

#include <HumanInterface/DRContecIsaCounter.h>
#include <HumanInterface/DRPortIO.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {

DRContecIsaCounter::DRContecIsaCounter(int a){
	address = a;
}

bool DRContecIsaCounter::Init(){
	char str[256];
	sprintf(str, "Contec ISA Counter at 0x%03X", address);
	SetName(str);

	if (address == 0) return true;
	WBGetPortIO();
	//	ボードが存在することを確認
	_outp(address, 0x15);		//	割り込みマスク
	int data = _inp(address+0x1);
	if (data == 0xFF)			//	割り込みマスクは0xFFではないはず
		{
		DPF("Could not find Contec ISA counter borad at 0x%x.\n", address);
		return false;
		}
	DPF("Contec ISA counter borad was found at 0x%x.\n", address);
	//	カウンタボードの初期化
	//	operation command は解説書P.3-4参照
	for(int channel=0; channel<4; channel++){
		_outp(address    , channel*5+1);			//	mode set
		_outp(address+0x1, 0x86);
		_outp(address    , channel*5+2);			//	z-pulse invalidity
		_outp(address+0x1, 0x2);					//	z-pulse を用いない
		_outp(address    , 0x16);				//	sense reset
		_outp(address+0x1, 0xff);
	}

	for(int i=0; i<4; i++){
		AddChildObject((new DV(this, i))->Cast());
	}

	return true;
}
/*void DRContecIsaCounter::Register(HIVirtualDevicePool& vpool){
	for(int i=0; i<4; i++){
		vpool.Register(new DV(this, i));
	}
}*/
void DRContecIsaCounter::Count(int ch, long n){
	if (address == 0) return;
	unsigned char low,middle,high;
	high   = (unsigned char)(n >>16);
	middle = (unsigned char)(n >> 8);
	low    = (unsigned char)n;

	_outp(address    , ch*5 );	//	実際に値を代入する部分
	_outp(address+0x1, low);
	_outp(address+0x1, middle);
	_outp(address+0x1, high);
}
long DRContecIsaCounter::Count(int ch){
	if (address == 0) return 0;
	unsigned long low,middle,high;
	_outp(address    , 0x14  );					 	    //	data latch
	_outp(address+0x1, 0xf   );
	_outp(address    , ch*5 );							//	read data

	low    = _inp(address+0x1) & 0xff;	//	0x000000??
  	middle = _inp(address+0x1) & 0xff;	//	0x0000??00
	high   = _inp(address+0x1) & 0xff;	//	0x00??0000
	if (high & 0x80) high |= 0xFF00;	//	0x??000000
	return (long) (low+(middle<<8)+(high<<16));
}

}	//	namespace Spr

#endif	// ( _MSC_VER > 1500 )

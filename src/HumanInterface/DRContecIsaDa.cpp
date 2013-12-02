/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#if ( _MSC_VER > 1500 )

#include <HumanInterface/DRContecIsaDa.h>
#include <HumanInterface/DRPortIO.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {

DRContecIsaDa::DRContecIsaDa(int a){
	address = a;
}
bool DRContecIsaDa::Init(){
	char str[256];
	sprintf(str, "Contec ISA D/A at 0x%03X", address);
	SetName(str);
	WBGetPortIO();

	if (address == 0) return false;
	_outp(address+0x2,0x01);					//	range set mode にする
	_outp(address+0x0,0x00);					//	channel 0 to 3
	_outp(address+0x1,0x00);					//	range data set 0～10[V]
	int in = _inp(address+0x1);
	if (in == 0xFF) return false;
	_outp(address+0x0,0x04);					//	channel 4 to 7
	_outp(address+0x1,0x00);					//	range data set 0～10[V]
	_outp(address+0x2,0x00);					//	通常モード
	
	for(int i=0; i<8; i++){
		AddChildObject((new Da(this, i))->Cast());
	}
	
	return true;
}
/*void DRContecIsaDa::Register(HIVirtualDevicePool& vpool){
	for(int i=0; i<8; i++){
		vpool.Register(new DV(this, i));
	}
}*/
void DRContecIsaDa::Voltage(int ch, float volt){
	if (address == 0) return;

	int value = (int)(volt * 409.6f);
	Digit(ch, value);
}
void DRContecIsaDa::Digit(int ch, int value){
	//	最大値最小値の範囲を超えないようにする
	if (0xFFF < value) value = 0xFFF;
	//	出力
	_outp(address+0x0,(unsigned char)(ch + ((value << 4) & 0x0f0) ));
														//	OUTPUT DATA (LOW)
	_outp(address+0x1,(unsigned char)(value >> 4));		//	            (HIGH)
}

}	//	namespace Spr

#endif	// ( _MSC_VER > 1500 )

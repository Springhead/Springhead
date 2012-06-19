/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/DRUPP.h>
#include <algorithm>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif
// UPP control class
//     Original works S.HASEGAWA (for ARMS-IV G.O.D.)
//     Modefied by T.NAKAUE (for ARMS-VII)
//     Modefied by S.HASEGAWA (for SPIDAR)


//#include "DRUpp.h"

namespace Spr {
//----------------------------------------------------------------------------
//	UppFunc
//
DRUpp::UppFunc::UppFunc(int aCmd, int anAr, int anBr,
	int anIoa, int anIob, int anIoc, int anIod, char* anId)
	{
	strncpy(id, anId, IDLEN);
	id[IDLEN] = '\0';
	cmd = aCmd;	ar = anAr; br = anBr;
	ioa = anIoa; iob = anIob; ioc = anIoc; iod = anIod;
	}
bool DRUpp::UppFunc::operator ==(const UppFunc& f) const
	{
	return strcmp(id, f.id) == 0;
	}
bool DRUpp::UppFunc::operator <(const UppFunc& f) const
	{
	return strcmp(id, f.id) < 0;
	}
bool DRUpp::UppFunc::operator >(const UppFunc& f) const
	{
	return strcmp(id, f.id) > 0;
	}
bool DRUpp::UppFunc::operator !=(const UppFunc& f) const
	{
	return strcmp(id, f.id) != 0;
	}

//----------------------------------------------------------------------------
//	DRUpp
//
int DRUpp::FNR[17] = 
	{
	//	0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
		1, 2, 3, 4, 6, 7, 8, 9, 11,12,13,14,16,17,18,19,20
	//	0.25us * 20 = 5us = 200KHz
	//	0.25us * 5 = 1.25us = 800KHz
	};

DRUpp::DRUpp()
	{
	bInitalized = false;
	}
bool DRUpp::Init()
	{
	Run(0);
	Out(0x21, FNR[0]);	//MFNR = 1 = FNR[0]
	Out(0x22, FNR[0]);	//FNR = 1 = FNR[0]
	
	Out(0x13, 0);		//UOR1 = 0
	Out(0x12, 0);		//UOR2 = 0
	Out(0x36, 0);		//UOR3 = 0
	Out(0x03, 0);		//Port1 = 0
	Out(0x02, 0);		//Port2 = 0
	PortOwner();
	PortDir();
	func.clear();
	bInitalized = true;
	return true;
	}
bool DRUpp::Run()
	{
	return (In(0x20) & 2) == 2;
	}
void DRUpp::Run(bool r)
	{
	if (r) Out(0x20, 2);
	else Out(0x20, 0);
	}
void DRUpp::PortDir(unsigned dir)
	{
	//	dir		MSB	0x 0    0    F    F LSB
	//	port		FEDC BA98 7654 3210
	Out(0x00, dir >> 8);
	Out(0x01, dir & 0xFF);
	}
void DRUpp::PortOwner(unsigned owner)
	{
	//	owner	MSB	0x F    F    F    F LSB
	//	port		FEDC BA98 7654 3210
	Out(0x10, owner >> 8);
	Out(0x11, owner & 0xFF);
	}
void DRUpp::SetFunc()
	{
	bool r = Run();
	Run(0);
	Out (0x21, 0);	//	MFNR = 0
	unsigned int i;
	for(i=0; i<func.size(); i++)
		{
		Out(0x22, FNR[i]);
		Out(0x23, func[i].cmd);
		Out(0x24, func[i].ar);
		Out(0x25, func[i].br);
		Out(0x26, func[i].ioa);
		Out(0x27, func[i].iob);
		Out(0x28, func[i].ioc);
		Out(0x29, func[i].iod);
		}
	for(; i<16; i++)
		{
		Out(0x22, FNR[i]);
		Out(0x23, 0xFF);
		}
	Out(0x21, FNR[func.size()]);
	Out(0x22, FNR[0]);
	Run(r);
	}
void DRUpp::Add(const UppFunc& f)
	{
	func.push_back(f);
	SetFunc();
	}
void DRUpp::Del(char* id)
	{
	UppFunc key(0, 0,0, 0,0,0,0, id);
	std::vector<UppFunc>::iterator itf = std::find(func.begin(), func.end(), key);
	if (itf != func.end()) func.erase(itf);
	SetFunc();
	}
unsigned short DRUpp::PioRead()
	{
	unsigned short d;
	d = In(PORT2) << 8;
	d |= In(PORT1);
	return d;
	}
void	DRUpp::PioWrite(unsigned short d)
	{
	Out(PORT2, d >> 8);
	Out(PORT1, d & 0xFF);
	}
void	DRUpp::UdrWrite(unsigned no,unsigned short v)
	{
	Out(UDR_BASE + no * 2    , (v >> 8) & 0xff);
	Out(UDR_BASE + no * 2 + 1, v & 0xff);
	}
unsigned short	DRUpp::UdrRead(unsigned no)
	{
	unsigned short d;
	d = In(UDR_BASE + no * 2) << 8;
	d |= In(UDR_BASE + no * 2 + 1);
	return d;
	}

}	//	namespace Spr

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <SprDefs.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/HumanInterface/DRCyUsb20Sh4.cpp"
#else
#include "DRCyUsb20Sh4.h"
namespace Spr {;
DRCyUsb20Sh4::DRCyUsb20Sh4(const DRCyUsb20Sh4Desc& d):DRUsb20Sh4(d), sendBuf(NULL), recvBuf(NULL){}
DRCyUsb20Sh4::~DRCyUsb20Sh4(){}
void* DRCyUsb20Sh4::UsbOpen(int id){ 
	DSTR << "You have to define USE_CLOSED_SRC SprDefs.h to use Spidar" << std::endl;
	assert(0);
	return 0; }
bool DRCyUsb20Sh4::UsbClose(void*& h){ return false; }
void DRCyUsb20Sh4::Reset(){}
void DRCyUsb20Sh4::SetupBuffer(){}
void DRCyUsb20Sh4::UsbSend(unsigned char* outBuffer){}
void DRCyUsb20Sh4::UsbRecv(unsigned char* inBuffer){}
unsigned DRCyUsb20Sh4::UsbVidPid(void* h){return 0;}
} //namespace Spr
#endif

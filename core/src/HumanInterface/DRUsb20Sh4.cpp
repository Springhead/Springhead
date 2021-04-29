/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
// DRUsb20Sh4.cpp: DRUsb20Sh4 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include <SprDefs.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/HumanInterface/DRUsb20Sh4.cpp"
#else
#include "DRUsb20Sh4.h"
namespace Spr {;
DRUsb20Sh4::DRUsb20Sh4(const DRUsb20Sh4Desc& d):DRUsb20Simple(d){}
bool DRUsb20Sh4::Init(){
	DSTR << "You have to define USE_CLOSED_SRC in SprDefs.h to use SPIDARs by Cyverse" << std::endl;
	//	assert(0);
	return false;}
void DRUsb20Sh4::Update(){}
void DRUsb20Sh4::UsbUpdate(){}
void DRUsb20Sh4::UsbSend(unsigned char* outBuffer){}
void DRUsb20Sh4::UsbRecv(unsigned char* inBuffer){}
void DRUsb20Sh4::Reset(){}
} //namespace Spr
#endif

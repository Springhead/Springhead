/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Springhead.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/HumanInterface/DRUsb20Simple.cpp"
#else
#include "DRUsb20Simple.h"
namespace Spr {;
DRUsb20Simple::DRUsb20Simple(const DRUsb20SimpleDesc& d){}
DRUsb20Simple::~DRUsb20Simple(){}
int DRUsb20Simple::FindDevice(int ch){ return -1; }
bool DRUsb20Simple::Init(){ return false; }
void* DRUsb20Simple::UsbOpen(int id){ return (void*)-1; }
bool DRUsb20Simple::UsbClose(void*& h){ return false; }
void DRUsb20Simple::WriteVoltage(int ch, float v){}
void DRUsb20Simple::WriteDigit(int ch, int v){}
void DRUsb20Simple::WriteCount(int ch, long c){}
long DRUsb20Simple::ReadCount(int ch){return -1;}
void DRUsb20Simple::WritePio(int ch, bool level){}
bool DRUsb20Simple::ReadPio(int ch) {return false;}
int DRUsb20Simple::ReadRotarySwitch(){ return -1;}
void DRUsb20Simple::Update(){}
void DRUsb20Simple::Reset(){}
void DRUsb20Simple::UsbReset(){}
void DRUsb20Simple::UsbCounterClear(){}
void DRUsb20Simple::UsbDaSet(){}
void DRUsb20Simple::UsbCounterGet(){}
unsigned DRUsb20Simple::UsbVidPid(void* h){ return 0;}
} //namespace Spr
#endif

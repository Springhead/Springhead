/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <SprDefs.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/HumanInterface/DRADAPIO.cpp"
#else
#include "DRAdapio.h"
namespace Spr {	
DRAdapio::DRAdapio(int i){}
DRAdapio::~DRAdapio(){}
bool DRAdapio::Init(){ return false; }
void DRAdapio::DAVoltage(int ch, float v){}
void DRAdapio::DADigit(int ch, int dat){}
float DRAdapio::ADVoltage(int ch){return - FLT_MAX;}
int DRAdapio::ADDigit(int ch){return -1;}
void DRAdapio::CloseDevice(){}
}	//	namespace Spr
#endif USE_CLOSED_SRC

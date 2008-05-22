/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DVDABASE_H
#define VR_DVDABASE_H

#include <HumanInterface/HIVirtualDevice.h>

namespace Spr {

//----------------------------------------------------------------------------
//	DVDaBase

///	D/Aの1チャンネルを表す．
class SPR_DLL DVDaBase:public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVDaBase);
	DVDaBase();
	///	出力するデジタル値の設定
	virtual void Digit(int d)=0;
	///	出力電圧の設定
	virtual void Voltage(float volt)=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "D/A"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DVADBASE_H
#define VR_DVADBASE_H

#include <HumanInterface/HIVirtualDevice.h>

namespace Spr {

//----------------------------------------------------------------------------
//	DVAdBase

///	A/Dの1チャンネルを表す．
class DVAdBase:public HIVirtualDevice{
public:
	SPR_OBJECTDEF_ABST(DVAdBase);
	///	入力デジタル値の取得
	virtual int Digit()=0;
	///	入力電圧の取得
	virtual float Voltage()=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "A/D"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif

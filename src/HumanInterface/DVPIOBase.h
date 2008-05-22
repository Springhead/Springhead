/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef VR_DVPIOBASE_H
#define VR_DVPIOBASE_H

#include <HumanInterface/HIVirtualDevice.h>
namespace Spr {;
//----------------------------------------------------------------------------
//	DVPioBase

///	入力ポートの1端子をあらわす．
class SPR_DLL DVInputPortBase:public HIVirtualDevice, public DVPortEnum{
public:
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual int Get()=0;

	///	デバイスの種類
	static const char* TypeS() { return "input port"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

///	出力ポートの1端子をあらわす．
class SPR_DLL DVOutputPortBase:public HIVirtualDevice, public DVPortEnum{
public:
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	入力ポートの電圧レベル(Hi:1, Lo:0) を返す．
	virtual void Set(int l)=0;

	///	デバイスの種類
	static const char* TypeS() { return "output port"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};


///	PIOの1端子を表す．
class SPR_DLL DVPioBase:public HIVirtualDevice, public DVPortEnum{
public:
	SPR_OBJECTDEF_ABST(DVPioBase);
	///	ポートのロジックレベルの入力。Hi:true Lo:false
	virtual int Get()=0;
	///	ポートのロジックレベルの出力。Hi:true Lo:false
	virtual void Set(int l)=0;
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "PIO"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr
#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HIVIRTUALDEVICE_H
#define HIVIRTUALDEVICE_H

#include <Foundation/Object.h>
#include <SprHumanInterface.h>
#include <HumanInterface/IfStubHumanInterface.h>

namespace Spr {

//----------------------------------------------------------------------------
//	HIVirtualDevice

class HIRealDevice;

///	仮想デバイスの基本クラス．DA,カウンタなどの1チャンネルに相当する.
class SPR_DLL HIVirtualDevice:public Object, public HIVirtualDeviceIfInit{
public:
	OBJECTDEF_ABST(HIVirtualDevice, Object);

	bool used;
	virtual bool GetUsed(){return used;}
	virtual void SetUsed(bool b){used = b;}
	friend class HIVirtualDevicePool;
public:
	HIVirtualDevice(){ used = false; }
	virtual ~HIVirtualDevice(){}
	///	デバイスの名前
	virtual const char* Name() const { return ""; }
	///	デバイスの種類
	virtual const char* Type() const { return ""; }
	///	実デバイスへのポインタ
	virtual HIRealDeviceIf* RealDevice(){ return NULL; }
	///	ダンプ出力
	virtual void Print(std::ostream& o) const;
	///
	virtual void Update(){}
};
inline std::ostream& operator << (std::ostream& o, const HIVirtualDevice& d){
	d.Print(o);
	return o;
}
}	//	namespace Spr

#endif

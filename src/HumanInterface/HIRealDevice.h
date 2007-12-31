/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HIREALDEVICEBASE_H
#define HIREALDEVICEBASE_H

#include <SprHumanInterface.h>
#include <Foundation/Object.h>
#include <HumanInterface/IfStubHumanInterface.h>

namespace Spr {;
class HIVirtualDevicePool;

///	実デバイスの基本クラス．ISAカード1枚などに相当する.
class SPR_DLL HIRealDevice:public Object, public HIRealDeviceIfInit{
public:
	OBJECTDEF_ABST(HIRealDevice, Object);
	HIRealDevice(){}
	virtual ~HIRealDevice(){}
	///	デバイスの名前
	virtual const char* Name() const { return ""; }
	///	初期化
	virtual bool Init(){ return false; }
	///	仮想デバイスの登録
	virtual void Register(HISdkIf* sdk){}
	///	状態の更新
	virtual void Update(){}
	///	ダンプ出力
	virtual void Print(std::ostream& o) const;
};
}	//	namespace Spr

#endif

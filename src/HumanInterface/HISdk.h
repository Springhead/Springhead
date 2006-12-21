/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HISDK_H
#define HISDK_H
#include <Foundation/Foundation.h>
#include <HumanInterface/SprHISdk.h>
#include "IfStubHumanInterface.h"

namespace Spr {;

class HISdkFactory : public FactoryBase {
public:
	const IfInfo* GetIfInfo() const {
		return HISdkIf::GetIfInfoStatic();
	}
	ObjectIf* Create(const void* desc, ObjectIf* parent){
		return HISdkIf::CreateSdk();
	}
};

class SPR_DLL HISdk:public Sdk, HISdkIfInit{
public:
	OBJECTDEF(HISdk, Sdk);
	HISdk(const HISdkDesc& = HISdkDesc());
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	///	ヒューマンインタフェースの作成
	virtual HIBaseIf* CreateHumanInterface(const char* name);
	///	実デバイス(ボードやUSBインタフェースなどのRawデバイス)の作成&登録
	virtual void RegisterDevice(const char* name);
};

}
#endif

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HISDK_H
#define HISDK_H
#include <Foundation/Foundation.h>
#include <HumanInterface/SprHISdk.h>

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

class HIVirtualDevicePool;
class HIRealDevicePool;

class SPR_DLL HISdk:public Sdk{
protected:
	UTRef<HIVirtualDevicePool> vpool;
	UTRef<HIRealDevicePool> rpool;
public:
	SPR_OBJECTDEF(HISdk);
	HISdk(const HISdkDesc& = HISdkDesc());
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);

	///	SDKの初期化
	virtual void Init();
	///	作成・登録したRealDevice/VirtualDevice/HumanInterfaceをすべて削除
	virtual void Clear();
	///	Rent virtual device.
	virtual HIVirtualDeviceIf* RentVirtualDevice(const char* type, const char* name=NULL);
	///	Return virutal device.
	virtual bool ReturnVirtualDevice(HIVirtualDeviceIf* dev);
	
	///	Register real device.
	virtual void RegisterRealDevice(HIRealDeviceIf* dev);	
	///	Get real device.
	virtual HIRealDeviceIf* FindRealDevice(const char* name=NULL);
	///	Register virutal device.
	virtual void RegisterVirtualDevice(HIVirtualDeviceIf* dev);
	///	ヒューマンインタフェースの作成
	virtual UTRef<HIBaseIf> CreateHumanInterface(const IfInfo* keyInfo);
	virtual UTRef<HIBaseIf> CreateHumanInterface(const char* name);
	///	Create and register real device.
	virtual bool AddRealDevice(const IfInfo* keyInfo, const void* desc=NULL);

	//	SDK生成、登録関数
	///	HISdkのインスタンスを作成
	static HISdkIf* SPR_CDECL CreateSdk();
	///	HISdkをファイルローダーなどに登録
	static void SPR_CDECL RegisterSdk();
	///	中身の表示
	void Print(std::ostream& o) const;
};

}
#endif

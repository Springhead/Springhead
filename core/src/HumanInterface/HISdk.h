/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HISDK_H
#define HISDK_H

#include <Foundation/Scene.h>
#include <HumanInterface/SprHISdk.h>
#include <HumanInterface/HIBase.h>

#include <vector>

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

class SPR_DLL HISdk: public Sdk{
public:
typedef	std::vector< UTRef<HIRealDeviceIf> >	DRPool;
typedef std::vector< UTRef<HIBaseIf> >			HIPool;

protected:
	DRPool					drPool;		///< 実デバイスのプール
	HIPool					hiPool;		///< ヒューマンインタフェースのプール

public:
	SPR_OBJECTDEF(HISdk);
	HISdk(const HISdkDesc& = HISdkDesc());

	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual void Clear();

	/// インタフェースの実装
	HIVirtualDeviceIf* RentVirtualDevice(const IfInfo* ii, const char* name=NULL, int portNum = -1);
	HIVirtualDeviceIf* RentVirtualDevice(const char* type, const char* name=NULL, int portNum = -1);
	bool ReturnVirtualDevice(HIVirtualDeviceIf* dev);
	
	void			RegisterRealDevice(HIRealDeviceIf* dev);	
	HIRealDeviceIf* FindRealDevice(const char* name);
	HIRealDeviceIf*	FindRealDevice(const IfInfo* ii);
	void			RegisterVirtualDevice(HIVirtualDeviceIf* dev);
	HIBaseIf*		CreateHumanInterface(const IfInfo* keyInfo);
	HIBaseIf*		CreateHumanInterface(const char* name);
	HIRealDeviceIf* AddRealDevice(const IfInfo* keyInfo, const void* desc=NULL);

	static HISdkIf* SPR_CDECL CreateSdk();
};

}
#endif

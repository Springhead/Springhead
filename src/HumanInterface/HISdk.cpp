/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HISdk.h"
#include "HIRealDevicePool.h"
#include "HIVirtualDevicePool.h"
#include "DRKeyMouseWin32.h"
#include "HIMouse6D.h"

namespace Spr {;

HISdkIf* SPR_CDECL HISdkIf::CreateSdk(){
	HISdk* rv = DBG_NEW HISdk;
	return rv->Cast();
}

IF_OBJECT_IMP(HISdk, Sdk);
HISdk::HISdk(const HISdkDesc& desc){
	vpool = new HIVirtualDevicePool;
	rpool = new HIRealDevicePool;


	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpNoDesc(DRKeyMouseWin32));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpNoDesc(HIMouse6D));

	HIMouse6DIf::GetIfInfoStatic();

}
size_t HISdk::NChildObject() const { 
	return 0; 
}
ObjectIf* HISdk::GetChildObject(size_t i){
	return NULL;
}
bool HISdk::AddChildObject(ObjectIf* o){
	return false;
}
UTRef<HIBaseIf> HISdk::CreateHumanInterface(const IfInfo* keyInfo, const void* desc){
	UTRef<ObjectIf> obj = CreateObject(keyInfo, desc);
	HIBaseIf* hi = obj->Cast();
	if (hi->Init(Cast(), desc)) return hi;
	return NULL;
}
UTRef<HIBaseIf> HISdk::CreateHumanInterface(const char* name, const char* desc){
	IfInfo* info = IfInfo::Find(name);
	//	hase TODO descのパーサを用意して，Desc構造体を作る
	if (info) return CreateHumanInterface(info, NULL);
	return NULL;
}
void HISdk::Init(){
	rpool->Init(Cast());
}

void HISdk::Clear(){
	vpool->Clear();
	rpool->Clear();
}

HIVirtualDeviceIf* HISdk::RentVirtualDevice(const char* type, const char* name){
	return vpool->Rent(type, name)->Cast();
}
bool HISdk::ReturnVirtualDevice(HIVirtualDeviceIf* dev){
	return vpool->Return(dev->Cast());
}
bool HISdk::AddRealDevice(const IfInfo* keyInfo, const void* desc){
	UTRef<ObjectIf> obj = CreateObject(keyInfo, desc);
	HIRealDeviceIf* dev = obj->Cast();
	if (dev){
		RegisterRealDevice(dev);
		return true;
	}
	return false;
}
HIRealDeviceIf* HISdk::FindRealDevice(const char* name){
	for(unsigned i=0; i<rpool->size(); ++i){
		if (strcmp(rpool->at(i)->Name(), name) == 0){
			return rpool->at(i)->Cast();
		}
	}
	return NULL;
}

void HISdk::RegisterRealDevice(HIRealDeviceIf* dev){
	rpool->Register(dev->Cast());
}
void HISdk::RegisterVirtualDevice(HIVirtualDeviceIf* dev){
	vpool->Register(dev->Cast());
}
void HISdk::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<HISdk>" << std::endl;
	o.width(w+2);
	o << *vpool;
	o << *rpool;
	o << UTPadding(w) << "</HISdk>" << std::endl;
	o.width(w);
}


}

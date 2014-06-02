/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
//#include <Springhead.h>
#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseWin32.h>
#include <HumanInterface/DRKeyMouseGLUT.h>
#include <HumanInterface/DRUsb20Simple.h>
#include <HumanInterface/DRUsb20Sh4.h>
#include <HumanInterface/DRCyUsb20Sh4.h>
#include <HumanInterface/HISpidar.h>
#include <HumanInterface/HINovintFalcon.h>
#include <HumanInterface/HISpaceNavigator.h>
#include <HumanInterface/HITrackball.h>
#include <HumanInterface/HIXbox360Controller.h>
#include <HumanInterface/HILeap.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

HISdkIf* SPR_CDECL HISdkIf::CreateSdk(){
	HISdk* rv = DBG_NEW HISdk;
	return rv->Cast();
}

extern void SPR_CDECL HIRegisterTypeDescs();
void SPR_CDECL HISdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;
	Sdk::RegisterFactory(DBG_NEW HISdkFactory());
	HIRegisterTypeDescs();

	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRKeyMouseWin32));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRKeyMouseGLUT));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRUsb20Simple));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRUsb20Sh4));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRCyUsb20Sh4));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpidar4));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpidar4D));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpidarG));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HINovintFalcon));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpaceNavigator));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HITrackball));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HIXbox360Controller));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HILeap));

}

HISdk::HISdk(const HISdkDesc& desc){
	HISdkIf::RegisterSdk();
}

size_t HISdk::NChildObject() const { 
	return drPool.size()/* + dvPool.size()*/ + hiPool.size(); 
}

ObjectIf* HISdk::GetChildObject(size_t i){
	if(i < drPool.size())
		return drPool[i]->Cast();
	i -= drPool.size();
	/*if(i < dvPool.size())
		return dvPool[i]->Cast();
	i -= dvPool.size();*/
	if(i < hiPool.size())
		return hiPool[i]->Cast();

	return NULL;
}

bool HISdk::AddChildObject(ObjectIf* o){
	HIRealDeviceIf* dr = o->Cast();
	if(dr){
		drPool.push_back(dr);
		return true;
	}
	/*HIVirtualDevice* dv = o->Cast();
	if(dv){
		dvPool.push_back(dv);
		return true;
	}*/
	HIBaseIf* hi = o->Cast();
	if(hi){
		hiPool.push_back(hi);
		return true;
	}
	return false;
}

HIBaseIf* HISdk::CreateHumanInterface(const IfInfo* keyInfo){
	return DCAST(HIBaseIf, CreateObject(keyInfo, NULL));
}

HIBaseIf* HISdk::CreateHumanInterface(const char* name){
	IfInfo* info = IfInfo::Find(name);
	if(info)
		return CreateHumanInterface(info);
	return NULL;
}

//void HISdk::Init(){
//	drPool.Init(Cast());
//}

void HISdk::Clear(){
	//dvPool.Clear();
	drPool.clear();
	hiPool.clear();
}

HIVirtualDeviceIf* HISdk::RentVirtualDevice(const IfInfo* ii, const char* name, int port){
	HIVirtualDeviceIf* dev;
	for(DRPool::iterator it = drPool.begin(); it != drPool.end(); it++){
		dev = (*it)->Rent(ii, name, port);
		if(dev)
			return dev->Cast();
	}
	return 0;
}
HIVirtualDeviceIf* HISdk::RentVirtualDevice(const char* type, const char* name, int port){
	IfInfo* info = IfInfo::Find(type);
	return RentVirtualDevice(info, name, port);
}
bool HISdk::ReturnVirtualDevice(HIVirtualDeviceIf* dev){
	return dev->GetRealDevice()->Return(dev);
}

HIRealDeviceIf* HISdk::AddRealDevice(const IfInfo* keyInfo, const void* desc){
	HIRealDeviceIf* dev = DCAST(HIRealDeviceIf, CreateObject(keyInfo, desc));
	if (dev){
		// デバイスの初期化と仮想デバイスの登録
		dev->Init();
		return dev;
	}
	return 0;
}

HIRealDeviceIf* HISdk::FindRealDevice(const char* name){
	for(unsigned i=0; i < drPool.size(); ++i){
		if (strcmp(drPool[i]->GetName(), name) == 0){
			return drPool[i]->Cast();
		}
	}
	return NULL;
}
HIRealDeviceIf*	HISdk::FindRealDevice(const IfInfo* ii){
	for(unsigned i=0; i < drPool.size(); ++i){
		if (drPool[i]->GetIfInfo() == ii){
			return drPool[i]->Cast();
		}
	}
	return NULL;
}

}

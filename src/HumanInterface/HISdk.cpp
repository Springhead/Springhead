/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseWin32.h>
#include <HumanInterface/DRKeyMouseGLUT.h>
#include <HumanInterface/DRUsb20Simple.h>
#include <HumanInterface/DRUsb20Sh4.h>
#include <HumanInterface/HISpidar.h>
#include <HumanInterface/HINovintFalcon.h>
#include <HumanInterface/HIMouse6D.h>
#include <HumanInterface/HITrackball.h>
#include <HumanInterface/HIDragger.h>

namespace Spr {;

HISdkIf* SPR_CDECL HISdkIf::CreateSdk(){
	HISdk* rv = DBG_NEW HISdk;
	return rv->Cast();
}
void SPR_CDECL HISdkIf::RegisterSdk(){
	static bool bFirst = true;
	if (!bFirst) return;
	bFirst=false;

	Sdk::RegisterFactory(DBG_NEW HISdkFactory());
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRKeyMouseWin32));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRKeyMouseGLUT));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRUsb20Simple));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(DRUsb20Sh4));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpidar4));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpidar4D));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HISpidarG));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HIMouse6D));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HINovintFalcon));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HITrackball));
	HISdkIf::GetIfInfoStatic()->RegisterFactory(new FactoryImpOwned(HIDragger));
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

HIVirtualDeviceIf* HISdk::RentVirtualDevice(const IfInfo* ii, const char* name, int portNo){
	// 既存の仮想デバイスプールを探す
	//  初期化時に仮想デバイスを登録するデバイスはこちらになる
	HIVirtualDeviceIf* dev;
	//dev = dvPool.Rent(type, name, portNo);
	//if(dev)
	//	return dev->Cast();
	// なければ実デバイスに問い合わせて動的に作成
	//  マウスやジョイスティックなど，ウィンドウ生成に合わせて仮想デバイスが作成されるものはこちら
	for(DRPool::iterator it = drPool.begin(); it != drPool.end(); it++){
		dev = (*it)->Rent(ii, name, portNo);
		if(dev)
			return dev->Cast();
	}
	return 0;
}
/*HIVirtualDeviceIf* HISdk::RentVirtualDeviceNo(const char* type, int No, const char* name){
	return dvPool.RentNo(type, No)->Cast();
}*/
bool HISdk::ReturnVirtualDevice(HIVirtualDeviceIf* dev){
	return dev->GetRealDevice()->Return(dev);
//	return dvPool.Return(dev->Cast());
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

//void HISdk::RegisterRealDevice(HIRealDeviceIf* dev){
//	drPool.Register(dev->Cast());
//}
//void HISdk::RegisterVirtualDevice(HIVirtualDeviceIf* dev){
//	dvPool.Register(dev->Cast());
//}
/*void HISdk::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<HISdk>" << std::endl;
	o.width(w+2);
	//o << dvPool;
	o << drPool;
	o << UTPadding(w) << "</HISdk>" << std::endl;
	o.width(w);
}*/

}

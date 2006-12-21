/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HISdk.h"

namespace Spr {;

HISdkIf* SPR_CDECL HISdkIf::CreateSdk(){
	HISdk* rv = DBG_NEW HISdk;
	return rv->Cast();
}

IF_OBJECT_IMP(HISdk, Sdk);
HISdk::HISdk(const HISdkDesc& desc){
}
ObjectIf* HISdk::CreateObject(const IfInfo* info, const void* desc){
	return NULL;
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
HIBaseIf* HISdk::CreateHumanInterface(const char* name){
	return NULL;
}
void HISdk::RegisterDevice(const char* name){
}


}

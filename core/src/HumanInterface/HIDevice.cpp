/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HIDevice.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

using namespace std;

namespace Spr {;

//-------------------------------------------------------------------------------------------------

/*void HIRealDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DRDevice name:\"" << GetDeviceName() << "\"/>" << std::endl;
	o.width(w);
}*/

HIVirtualDeviceIf* HIRealDevice::Rent(const IfInfo* ii, const char* n, int portNo){
	if(n && strcmp(n, "") && name != n)
		return 0;
	
	for(int i = 0; i < (int)NChildObject(); i++){
		HIVirtualDeviceIf* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetIfInfo() == ii && (portNo==-1 || portNo == dv->GetPortNo()) && !dv->IsUsed()){
			DCAST(HIVirtualDevice, dv)->SetUsed(true);
			return dv;
		}
	}
	return 0;
}	

bool HIRealDevice::Return(HIVirtualDeviceIf* dv){
	DCAST(HIVirtualDevice, dv)->SetUsed(false);
	return true;
}

bool HIRealDevice::AddChildObject(ObjectIf* o){
	HIVirtualDevice* dv = o->Cast();
	if(dv){
		dvPool.push_back(dv);
		return true;
	}
	return false;
}

ObjectIf* HIRealDevice::GetChildObject(size_t i){
	if(i >= dvPool.size())
		return 0;
	return dvPool[i]->Cast();
}

size_t HIRealDevice::NChildObject() const{
	return dvPool.size();
}

//-------------------------------------------------------------------------------------------------

HIVirtualDevice::HIVirtualDevice(HIRealDevice* dev, int ch){
	portNo		= ch;
	realDevice	= dev;
	used		= false;
}

/*void HIVirtualDevice::SetDeviceName(){
	stringstream ss;
	ss << realDevice->GetDeviceName() << ' ' << GetDeviceType();
	if(portNo != -1)
		ss << ' ' << portNo;
	devName = ss.str();
}*/

/*void HIVirtualDevice::Print(std::ostream& o) const{
	int w = o.width();
	o.width(0);
	o << UTPadding(w) << "<DVDevice type:\"" << GetDeviceType() << "\" name:\"" << GetDeviceName() << "\"/>" << std::endl;
	o.width(w);
}*/

//-------------------------------------------------------------------------------------------------

void DVKeyMouse::OnMouse(int button, int state, int x, int y){
	for(Callbacks::iterator it = callbacks.begin(); it != callbacks.end(); it++)
		(*it)->OnMouse(button, state, x, y);
}

void DVKeyMouse::OnDoubleClick(int button, int x, int y){
	for(Callbacks::iterator it = callbacks.begin(); it != callbacks.end(); it++)
		(*it)->OnDoubleClick(button, x, y);
}

void DVKeyMouse::OnMouseMove(int button, int x, int y, int zdelta){
	for(Callbacks::iterator it = callbacks.begin(); it != callbacks.end(); it++)
		(*it)->OnMouseMove(button, x, y, zdelta);
}

void DVKeyMouse::OnKey(int state, int key, int x, int y){
	for(Callbacks::iterator it = callbacks.begin(); it != callbacks.end(); it++)
		(*it)->OnKey(state, key, x, y);
}

//-------------------------------------------------------------------------------------------------

void DVJoyStick::OnUpdate(int buttonMask, int x, int y, int z){
	for(Callbacks::iterator it = callbacks.begin(); it != callbacks.end(); it++)
		(*it)->OnUpdate(buttonMask, x, y, z);
}

}	//	namespace Spr

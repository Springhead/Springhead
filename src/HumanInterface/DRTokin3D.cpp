/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Device.h"
#pragma hdrstop

//#include "DRTokin3D.h"
//#include "TKINPUT.h"
//#include "TKINPUT_I.C"

namespace Spr {;

DRTokin3D::DVInputPort::DVInputPort(DRTokin3D* r, int c):ch(c), realDevice(r){
	sprintf(name, "%s input port ch %d", realDevice->Name(), ch);
}

DRTokin3D::DRTokin3D(){
	sprintf(name, "Tokin 3D motion sensor");
	intf = NULL;
	CoInitialize(NULL);
}
DRTokin3D::~DRTokin3D(){
	if (intf){
		long retinfo;
		((ITk3dInput*)intf)->CloseDevice(&retinfo);
		((ITk3dInput*)intf)->Release();
	}
	CoUninitialize();
}
bool DRTokin3D::Init(){
	if ((ITk3dInput*)intf) return true;
	HRESULT hr = CoCreateInstance(CLSID_Tk3dInput, NULL, CLSCTX_INPROC, IID_ITk3dInput, &intf);
	if ( FAILED( hr )){
		DSTR.setf(std::ios::hex, std::ios::basefield);
		DSTR << "Failed to create ITk3dInput interface. result:" << hr << std::endl;
		return false;
	}
	// Access the IMath interface
	long retinfo;
	((ITk3dInput*)intf)->CreateDevice(&retinfo);
//	if (!retinfo) return false;
	//	DSTR << "CreateDevice :retinfo = 0x" << std::ios::hex << retinfo << std::endl;
	((ITk3dInput*)intf)->Poll( 0, &retinfo );
	//	DSTR << "Poll(0):retinfo = 0x" << std::ios::hex << retinfo << std::endl;
	Sleep(100);
	return true;
}
void DRTokin3D::Register(HIVirtualDevicePool& vpool){
	vpool.Register(new DVOrientation(this));
	for(int i=0; i<5; i++){
		vpool.Register(new DVInputPort(this, i));
	}
}
void DRTokin3D::GetMatrix(Matrix3f& ori){
	static Matrix3f oriOld;
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	if (retinfo==S_OK){
		Quaternionf q;
		q = Quaternionf::Rot((float)tk3dIState.Alfa, 'y') * Quaternionf::Rot((float)tk3dIState.Beta, 'z') * Quaternionf::Rot((float)tk3dIState.Gamma, 'x');
		oriOld = Matrix3f::Rot(q);
	}
	ori = oriOld;
}
Vec3f DRTokin3D::GetEuler(){
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	Vec3f euler;
	euler.Z() = (float)tk3dIState.Alfa;
	euler.Y() = (float)tk3dIState.Beta;
	euler.X() = (float)tk3dIState.Gamma;
	return euler;
}
int DRTokin3D::GetPort(int ch){
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	int button = tk3dIState.Button;
	if (ch==0) return (button&0x01)!=0;
	if (ch==1) return (button&0x02)!=0;
	if (ch==2) return (button&0x04)!=0;
	if (ch==3) return (button&0x08)!=0;
	if (ch==4) return (button&0x10)!=0;
	return 0;
}
}	//	namespace Spr

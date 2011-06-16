/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/DRTokin3D.h>

// NEC Tokin 3Dを使用するためのコードのインクルード
#ifdef SPR_USE_TOKIN3D
# include "Tkinput.h"
# include "Tkinputdef.h"
# include "Tuadapio.h"
# include "TKINPUT_I.C"
#endif

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

DRTokin3D::DRTokin3D(){
#ifdef SPR_USE_TOKIN3D
	intf = NULL;
	CoInitialize(NULL);
#endif
}

DRTokin3D::~DRTokin3D(){
#ifdef SPR_USE_TOKIN3D
	if (intf){
		long retinfo;
		((ITk3dInput*)intf)->CloseDevice(&retinfo);
		((ITk3dInput*)intf)->Release();
	}
	CoUninitialize();
#endif
}

bool DRTokin3D::Init(){
	SetName("Tokin 3D motion sensor");
#ifdef SPR_USE_TOKIN3D
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

	AddChildObject((DBG_NEW DVOri(this))->Cast());
	for(int i=0; i<5; i++)
		AddChildObject((DBG_NEW DVIn(this, i))->Cast()s);

	return true;
#else
	return false;
#endif
}
/*void DRTokin3D::Register(HIVirtualDevicePool& vpool){
	vpool.Register(new DVOrientation(this));
	for(int i=0; i<5; i++){
		vpool.Register(new DVInputPort(this, i));
	}
}*/
void DRTokin3D::GetMatrix(Matrix3f& ori){
#ifdef SPR_USE_TOKIN3D
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
#endif
}

Vec3f DRTokin3D::GetEuler(){
	Vec3f euler;
#ifdef SPR_USE_TOKIN3D
	TK3DISTATE tk3dIState;
	ZeroMemory( &tk3dIState, sizeof(TK3DISTATE) );
	long retinfo;
	((ITk3dInput*)intf)->GetDeviceData( 0, &tk3dIState, &retinfo);
	euler.Z() = (float)tk3dIState.Alfa;
	euler.Y() = (float)tk3dIState.Beta;
	euler.X() = (float)tk3dIState.Gamma;
#endif
	return euler;
}

int DRTokin3D::GetPort(int ch){
#ifdef SPR_USE_TOKIN3D
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
#endif
	return 0;
}

}	//	namespace Spr

/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWJoyStickGLUT.h>
#include <HumanInterface/HISdk.h>

#include <GL/glut.h>

#ifndef GLUTCALLBACK
#  define GLUTCALLBACK
#endif

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

FWJoyStickGLUT* FWJoyStickGLUT::instance = 0;

FWJoyStickGLUT::FWJoyStickGLUT(const FWJoyStickGLUTDesc& desc){
}

bool FWJoyStickGLUT::Init(){
	SetName("JoyStickGLUT");
	return true;
}

HIVirtualDeviceIf* FWJoyStickGLUT::Rent(const IfInfo* ii, const char* n, int portNo){
	HIVirtualDeviceIf* dv = HIRealDevice::Rent(ii, n, portNo);

	// 既存の仮想デバイスがなければ作成
	if(ii == DVJoyStickIf::GetIfInfoStatic() && !dv){
		DVJoyStick* js = DBG_NEW DV(this, portNo);
		AddChildObject(js->Cast());
		SetPollInterval(js);
		dv = js->Cast();
	}

	return dv;
}

static void GLUTCALLBACK OnUpdateStatic(unsigned int buttonMask, int x, int y, int z) {
	FWJoyStickGLUT::instance->OnUpdate(buttonMask, x, y, z);
}
void FWJoyStickGLUT::SetPollInterval(DVJoyStick* dv){
	int wid = dv->GetPortNo();
	int widCur = glutGetWindow();
	glutSetWindow(wid);
#ifdef FREEGLUT
	glutJoystickFunc(OnUpdateStatic, dv->pollInterval);
#endif
	glutSetWindow(widCur);
}

void FWJoyStickGLUT::OnUpdate(unsigned int buttonMask, int x, int y, int z){
	int wid = glutGetWindow();
	for(int i = 0; i < (int)NChildObject(); i++){
		DVJoyStick* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnUpdate(buttonMask, x, y, z);
	}
}


}	//	namespace Spr

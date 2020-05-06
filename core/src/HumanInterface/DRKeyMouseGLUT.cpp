/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseGLUT.h>
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

//---------------------------------------------------------------------

void DRKeyMouseGLUT::DV::GetMousePosition(int& x, int& y, int& time, int count){
	if(count < (int)mousePoints.size()){
		x = mousePoints[count].x;
		y = mousePoints[count].y;
	}
	else x = y = 0;
	time = 0;
}

void DRKeyMouseGLUT::DV::OnMouse(int button, int state, int x, int y){
	mousePoints.push_front(Vec2i(x, y));
	mousePoints.pop_back();

	DVKeyMouse::OnMouse(button, state, x, y);
}

void DRKeyMouseGLUT::DV::OnMouseMove(int button, int x, int y, int zdelta){
	mousePoints.push_front(Vec2i(x, y));
	mousePoints.pop_back();

	DVKeyMouse::OnMouseMove(button, x, y, zdelta);
}

//---------------------------------------------------------------------
//	DRKeyMouseGLUT
//

int	DRKeyMouseGLUT::ConvertKeyCode(int key, bool spr_to_glut, bool special){
	if(spr_to_glut)
		return (key < 256 ? key : key - 256);
	return (special ? key + 256 : key);
}

DRKeyMouseGLUT::DRKeyMouseGLUT(const DRKeyMouseGLUTDesc& desc){
	buttonState = 0;
}

bool DRKeyMouseGLUT::Init(){
	SetName("KeyMouseGLUT");
	return true;
}

HIVirtualDeviceIf* DRKeyMouseGLUT::Rent(const IfInfo* ii, const char* n, int portNo){
	HIVirtualDeviceIf* dv = HIRealDevice::Rent(ii, n, portNo);

	// 既存の仮想デバイスがなければ作成
	if(!dv && DVKeyMouseIf::GetIfInfoStatic()->Inherit(ii)){
		DVKeyMouse* km = DBG_NEW DV(this, portNo);
		AddChildObject(km->Cast());
		dv = km->Cast();
	}
	return dv;
}

void DRKeyMouseGLUT::OnMouse(int button, int state, int x, int y){
	int st;
	if(state == GLUT_DOWN)
		 st = DVButtonSt::DOWN;
	else st = DVButtonSt::UP;

	if(button == GLUT_LEFT_BUTTON)
		 buttonState = DVButtonMask::LBUTTON;
	else if(button == GLUT_MIDDLE_BUTTON)
		 buttonState = DVButtonMask::MBUTTON;
	else buttonState = DVButtonMask::RBUTTON;

	int mod = glutGetModifiers();
	if(mod & GLUT_ACTIVE_SHIFT)
		buttonState |= DVButtonMask::SHIFT;
	if(mod & GLUT_ACTIVE_CTRL)
		buttonState |= DVButtonMask::CONTROL;
	if(mod & GLUT_ACTIVE_ALT)
		buttonState |= DVButtonMask::ALT;

	// カレントウィンドウに対応する仮想デバイスのコールバックを呼ぶ
	int wid = glutGetWindow();
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnMouse(buttonState, st, x, y);
	}

	if(st == DVButtonSt::UP)
		buttonState = 0;
}
void DRKeyMouseGLUT::OnMotion(int x, int y){
	int wid = glutGetWindow();
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnMouseMove(buttonState, x, y, 0);
	}
}
void DRKeyMouseGLUT::OnPassiveMotion(int x, int y){
	int wid = glutGetWindow();
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnMouseMove(buttonState, x, y, 0);
	}
}
void DRKeyMouseGLUT::OnKey(unsigned char ch, int x, int y){
	int wid = glutGetWindow();
	int key = ConvertKeyCode(ch, false, false);
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnKey(DVKeySt::PRESSED, key, x, y);
	}
}
void DRKeyMouseGLUT::OnSpecialKey(int ch, int x, int y){
	int wid = glutGetWindow();
	int key = ConvertKeyCode(ch, false, true);
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnKey(DVKeySt::PRESSED, key, x, y);
	}
}

void DRKeyMouseGLUT::OnMouseWheel(int wheel, int direction, int x, int y){
	int wid = glutGetWindow();
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnMouseMove(buttonState, x, y, direction);
	}
}

}	//	namespace Spr

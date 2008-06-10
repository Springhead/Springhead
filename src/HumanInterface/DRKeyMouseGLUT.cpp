/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Device.h"
#pragma hdrstop
#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseGLUT.h>
#include <Windows.h>
#include <iomanip>

namespace Spr {;

//	DRKeyMouseGLUT

DRKeyMouseGLUT* DRKeyMouseGLUT::This;

DRKeyMouseGLUT::KeyMouse::KeyMouse(DRKeyMouseGLUT* w):win32(w){
	strcpy(name, Type());
	strcat(name, "Win32");
}
void DRKeyMouseGLUT::KeyMouse::SetHandler(DVKeyMouseHandler* h){
	DRKeyMouseGLUT* kmglut = RealDevice()->Cast();
	kmglut->SetHandler(h);
}
int DRKeyMouseGLUT::KeyMouse::GetKeyState(int key){
	int rv = DVKeyMouseIf::NONE;
	short got = ::GetKeyState(key);
	if (got & 0x1) rv |= DVKeyMouseIf::TOGGLE_ON;
	if (got & 0x8000) rv |= DVKeyMouseIf::PRESS;
	return rv;
}
///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
DVKeyMouseIf::DVMousePosition DRKeyMouseGLUT::KeyMouse::GetMousePosition(int count){
	MOUSEMOVEPOINT points[64];
	int nPoint = GetMouseMovePointsEx(sizeof(MOUSEMOVEPOINT), NULL, points, 64, GMMP_USE_HIGH_RESOLUTION_POINTS);
	DVKeyMouseIf::DVMousePosition rv;
	if (count < nPoint){
		rv.time = points[count].time;
		rv.x = points[count].x;
		rv.y = points[count].y;
	}else{
		rv.time = 0;
		rv.x = 0;
		rv.y = 0;
	}
	return rv;
};

//---------------------------------------------------------------------
//	DRKeyMouseGLUT
//

DRKeyMouseGLUT::DRKeyMouseGLUT(): handler(NULL){
	strcpy(name, Type());
	strcat(name, "Win32");
}
bool DRKeyMouseGLUT::Init(){
	return true;
}
void DRKeyMouseGLUT::Register(HISdkIf* intf){
	HISdk* sdk = intf->Cast();
	sdk->RegisterVirtualDevice((new DRKeyMouseGLUT::KeyMouse(this))->Cast());
}
inline int ConvertKeyState(unsigned fwKeys){
	int keyState=0;
	if (fwKeys & MK_LBUTTON) keyState |= DVKeyMouseHandler::LBUTTON;
	if (fwKeys & MK_RBUTTON) keyState |= DVKeyMouseHandler::RBUTTON;
	if (fwKeys & MK_SHIFT) keyState |= DVKeyMouseHandler::SHIFT;
	if (fwKeys & MK_CONTROL) keyState |= DVKeyMouseHandler::CONTROL;
	if (fwKeys & MK_MBUTTON) keyState |= DVKeyMouseHandler::MBUTTON;
	return keyState;
}
void GLUTCALLBACK DRKeyMouseGLUT::OnMouse(int button, int state, int x, int y){
	if (!This->handler) return;
	bool bKeyDown=false;
	if (state == GLUT_DOWN){
		bKeyDown = true;
	}
	int vk;
	switch(button){
		case GLUT_LEFT_BUTTON:
			vk = VK_LBUTTON;
			break;
		case GLUT_MIDDLE_BUTTON:
			vk = VK_MBUTTON;
			break;
		case GLUT_RIGHT_BUTTON:
			vk = VK_RBUTTON;
			break;
	}
	This->handler->OnKey(bKeyDown, vk, state, x, y);
}
void GLUTCALLBACK DRKeyMouseGLUT::OnMotion(int x, int y){
	This->handler->OnMouseMove(0, x, y, 0);
}
void GLUTCALLBACK DRKeyMouseGLUT::OnPassiveMotion(int x, int y){
	This->handler->OnMouseMove(0, x, y, 0);
}

}	//	namespace Spr

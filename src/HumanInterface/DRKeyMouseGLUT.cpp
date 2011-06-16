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

DRKeyMouseGLUT* DRKeyMouseGLUT::instance = 0;

int	DRKeyMouseGLUT::ConvertKeyCode(int key, bool spr_to_glut, bool special){
	if(spr_to_glut)
		return (key < 256 ? key : key - 256);
	return (special ? key + 256 : key);
}

DRKeyMouseGLUT::DRKeyMouseGLUT(const DRKeyMouseGLUTDesc& desc){
	instance = this;
	buttonState = 0;
}

bool DRKeyMouseGLUT::Init(){
	SetName("KeyMouseGLUT");
	//RegisterCallback();

	// カレントウィンドウのIDを取得して対応付け
	//keyMouseMap[glutGetWindow()] = this;

	return true;
}

/*void DRKeyMouseGLUT::RegisterCallback(){
	// カレントウィンドウにコールバックを登録
	glutMouseFunc(OnMouseStatic);
	glutMotionFunc(OnMotionStatic);
	glutPassiveMotionFunc(OnPassiveMotionStatic);
	glutKeyboardFunc(OnKeyStatic);
	glutSpecialFunc(OnSpecialKeyStatic);
}*/

/*void DRKeyMouseGLUT::Register(HISdkIf* intf){
	HISdk* sdk = intf->Cast();
	dvKeyMouse = DBG_NEW DRKeyMouseGLUT::DV(this);
	sdk->RegisterVirtualDevice(dvKeyMouse->Cast());
}*/

HIVirtualDeviceIf* DRKeyMouseGLUT::Rent(const IfInfo* ii, const char* n, int portNo){
	HIVirtualDeviceIf* dv = HIRealDevice::Rent(ii, n, portNo);

	// 既存の仮想デバイスがなければ作成
	if(!dv){
		DVKeyMouse* km = (DBG_NEW DV(this, portNo))->Cast();
		AddChildObject(km->Cast());
		
		// カレントウィンドウにコールバックを登録
		glutMouseFunc(OnMouseStatic);
		glutMotionFunc(OnMotionStatic);
		glutPassiveMotionFunc(OnPassiveMotionStatic);
		glutKeyboardFunc(OnKeyStatic);
		glutSpecialFunc(OnSpecialKeyStatic);

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
			dv->OnKey(true, key, x, y);
	}
}
void DRKeyMouseGLUT::OnSpecialKey(int ch, int x, int y){
	int wid = glutGetWindow();
	int key = ConvertKeyCode(ch, false, true);
	for(int i = 0; i < (int)NChildObject(); i++){
		DVKeyMouse* dv = GetChildObject(i)->Cast();
		if(dv && dv->GetPortNo() == wid)
			dv->OnKey(true, key, x, y);
	}
}

void GLUTCALLBACK DRKeyMouseGLUT::OnMouseStatic(int button, int state, int x, int y){
	/*int wid = glutGetWindow();
	for(KeyMouseMap::iterator it = keyMouseMap.begin(); it != keyMouseMap.end(); it++){
		if(it->first == wid)
			it->second->OnMouse(button, state, x, y);
	}*/
	instance->OnMouse(button, state, x, y);
}
void GLUTCALLBACK DRKeyMouseGLUT::OnMotionStatic(int x, int y){
	/*int wid = glutGetWindow();
	for(KeyMouseMap::iterator it = keyMouseMap.begin(); it != keyMouseMap.end(); it++){
		if(it->first == wid)
			it->second->OnMotion(x, y);
	}*/
	instance->OnMotion(x, y);
}
void GLUTCALLBACK DRKeyMouseGLUT::OnPassiveMotionStatic(int x, int y){
	/*int wid = glutGetWindow();
	for(KeyMouseMap::iterator it = keyMouseMap.begin(); it != keyMouseMap.end(); it++){
		if(it->first == wid)
			it->second->OnPassiveMotion(x, y);
	}*/
	instance->OnPassiveMotion(x, y);
}
void GLUTCALLBACK DRKeyMouseGLUT::OnKeyStatic(unsigned char ch, int x, int y){
	/*int wid = glutGetWindow();
	for(KeyMouseMap::iterator it = keyMouseMap.begin(); it != keyMouseMap.end(); it++){
		if(it->first == wid)
			it->second->OnKey(ch, x, y);
	}*/
	instance->OnKey(ch, x, y);
}
void GLUTCALLBACK DRKeyMouseGLUT::OnSpecialKeyStatic(int ch, int x, int y){
	/*int wid = glutGetWindow();
	for(KeyMouseMap::iterator it = keyMouseMap.begin(); it != keyMouseMap.end(); it++){
		if(it->first == wid)
			it->second->OnSpecialKey(ch, x, y);
	}*/
	instance->OnSpecialKey(ch, x, y);
}

}	//	namespace Spr

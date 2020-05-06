/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef _WIN32_WINNT
 #define _WIN32_WINNT 0x0500
#endif

#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseWin32.h>
#ifdef _WIN32
# include <windows.h>
#endif
#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

//---------------------------------------------------------------------
//	DRKeyMouseWin32
//

#ifdef _WIN32

const int keyMapping[][2] = {
	{DVKeyCode::F1,			VK_F1},
	{DVKeyCode::F2,			VK_F2},
	{DVKeyCode::F3,			VK_F3},
	{DVKeyCode::F4,			VK_F4},
	{DVKeyCode::F5,			VK_F5},
	{DVKeyCode::F6,			VK_F6},
	{DVKeyCode::F7,			VK_F7},
	{DVKeyCode::F8,			VK_F8},
	{DVKeyCode::F9,			VK_F9},
	{DVKeyCode::F10,		VK_F10},
	{DVKeyCode::F11,		VK_F11},
	{DVKeyCode::F12,		VK_F12},
	{DVKeyCode::LEFT,		VK_LEFT},
	{DVKeyCode::UP,			VK_UP},
	{DVKeyCode::RIGHT,		VK_RIGHT},
	{DVKeyCode::DOWN,		VK_DOWN},
	{DVKeyCode::PAGE_UP,	VK_PRIOR},
	{DVKeyCode::PAGE_DOWN,	VK_NEXT},
	{DVKeyCode::HOME,		VK_HOME},
	{DVKeyCode::END,		VK_END},
	{DVKeyCode::INSERT,		VK_INSERT}
};

int	DRKeyMouseWin32::ConvertKeyCode(int key, bool spr_to_win32){
	int n = sizeof(keyMapping)/sizeof(keyMapping[0]);
	if(spr_to_win32){
		// アルファベット小文字は大文字へ
		if('a' <= key && key <= 'z'){
			key += 'A' - 'a';
			return key;
		}
		for(int i = 0; i < n; i++)
			if(keyMapping[i][0] == key)
				return keyMapping[i][1];
	}
	else{
		for(int i = 0; i < n; i++)
			if(keyMapping[i][1] == key)
				return keyMapping[i][0];
	}
	// デフォルトでは変換せずにそのまま返す
	return key;
}	

DRKeyMouseWin32::DRKeyMouseWin32(const DRKeyMouseWin32Desc& desc){
}

bool DRKeyMouseWin32::Init(){
	SetName("KeyMouseWin32");
	return true;
}

HIVirtualDeviceIf* DRKeyMouseWin32::Rent(const IfInfo* ii, const char* n, int portNo){
	HIVirtualDeviceIf* dv = HIRealDevice::Rent(ii, n, portNo);

	// 既存の仮想デバイスがなければ作成
	if(!dv){
		DVKeyMouse* km = (DBG_NEW DV(this, portNo))->Cast();
		AddChildObject(km->Cast());
		dv = km->Cast();
	}

	return dv;
}


int ConvertKeyState(unsigned fwKeys){
	int button = 0;
	if (fwKeys & MK_LBUTTON) button |= DVButtonMask::LBUTTON;
	if (fwKeys & MK_MBUTTON) button |= DVButtonMask::MBUTTON;
	if (fwKeys & MK_RBUTTON) button |= DVButtonMask::RBUTTON;
	if (fwKeys & MK_SHIFT)	 button |= DVButtonMask::SHIFT;
	if (fwKeys & MK_CONTROL) button |= DVButtonMask::CONTROL;
	return button;
}

//	キーの取得
int DRKeyMouseWin32::GetKeyState(int key){
	int rv = 0;
	short got = ::GetKeyState(ConvertKeyCode(key, true));
	if (got & 0x1)
		rv |= DVKeySt::TOGGLE_ON;
	if (got & 0x8000)
		rv |= DVKeySt::PRESSED;
	return rv;
}
void DRKeyMouseWin32::GetMousePosition(int& xo, int& yo, int& timeo, int count){
	if (count > NHISTORY-1) count = NHISTORY-1;
	xo = history[count].x;
	yo = history[count].y;
	timeo = history[count].time;
};
void DRKeyMouseWin32::UpdateMousePos(int x, int y){
	for(int i=NHISTORY-1; i>0; --i){
		history[i] = history[i-1];
	}
	history[0].x = x;
	history[0].y = y;
	history[0].time = GetTickCount();
}

bool DRKeyMouseWin32::PreviewMessage(void* m){
	MSG* msg = (MSG*)m;

	bool btnState = DVButtonSt::UP;
	bool keyState = 0;
	
#pragma warning (push)
#pragma warning (disable : 4302 4311)
	switch (msg->message){
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		btnState = DVButtonSt::DOWN;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:{
		if (btnState == DVButtonSt::UP)
			ReleaseCapture();

		int button = ConvertKeyState(msg->wParam);  // key flags
		int x = (short)LOWORD(msg->lParam);  // horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);  // vertical position of cursor 
		UpdateMousePos(x,y);
		if (btnState == DVButtonSt::DOWN)
			SetCapture(msg->hwnd);

		for(int i = 0; i < (int)NChildObject(); i++){
			DVKeyMouse* dv = GetChildObject(i)->Cast();
			if(dv && dv->GetPortNo() == (int)msg->hwnd)
				dv->OnMouse(button, btnState, x, y);
		}
		return true;
		}
	case WM_MOUSEMOVE:{
		unsigned fwKeys = msg->wParam;  // key flags 
		int button = ConvertKeyState(msg->wParam);  // key flags		
		int x = (short)LOWORD(msg->lParam);	// horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);	// vertical position of cursor 
		UpdateMousePos(x, y);
		for(int i = 0; i < (int)NChildObject(); i++){
			DVKeyMouse* dv = GetChildObject(i)->Cast();
			if(dv && dv->GetPortNo() == (int)msg->hwnd)
				dv->OnMouseMove(button, x, y, 0);
		}
		return true;
		}
	case WM_MOUSEWHEEL:{
		unsigned fwKeys = LOWORD(msg->wParam);		
		int button = ConvertKeyState(fwKeys);		// key flags		
		int zDelta = (short) HIWORD(msg->wParam);   // wheel rotation
		int x = (short) LOWORD(msg->lParam);		// horizontal position of pointer
		int y = (short) HIWORD(msg->lParam);		// vertical position of pointer
		UpdateMousePos(x,y);
		for(int i = 0; i < (int)NChildObject(); i++){
			DVKeyMouse* dv = GetChildObject(i)->Cast();
			if(dv && dv->GetPortNo() == (int)msg->hwnd)
				dv->OnMouseMove(button, x, y, zDelta);
		}
		return true;
		}
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:{
		int button = ConvertKeyState(msg->wParam);        // key flags 
		int x = (short)LOWORD(msg->lParam);  // horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);  // vertical position of cursor 
		UpdateMousePos(x,y);
		for(int i = 0; i < (int)NChildObject(); i++){
			DVKeyMouse* dv = GetChildObject(i)->Cast();
			if(dv && dv->GetPortNo() == (int)msg->hwnd)
				dv->OnDoubleClick(button, x, y);
		}
		return true;
		}
	case WM_KEYDOWN:
		keyState = DVKeySt::PRESSED;
	case WM_KEYUP:{
		int key = ConvertKeyCode((int) msg->wParam, false);
		POINT pt;
		GetCursorPos(&pt);
		for(int i = 0; i < (int)NChildObject(); i++){
			DVKeyMouse* dv = GetChildObject(i)->Cast();
			if(dv && dv->GetPortNo() == (int)msg->hwnd)
				dv->OnKey(keyState, key, pt.x, pt.y);
		}
		return true;
		}
	default:;
	}
#pragma warning (pop)
	return false;
}

#else

DRKeyMouseWin32::DRKeyMouseWin32(const DRKeyMouseWin32Desc& desc){}
bool DRKeyMouseWin32::Init(){ return false; }
HIVirtualDeviceIf* DRKeyMouseWin32::Rent(const IfInfo* ii, const char* name, int portNo){ return 0; }
int  DRKeyMouseWin32::ConvertKeyCode(int key, bool spr_to_win32){ return 0; }
int  DRKeyMouseWin32::GetKeyState(int key){ return 0;}
void DRKeyMouseWin32::GetMousePosition(int& x, int& y, int& time, int count){}
bool DRKeyMouseWin32::PreviewMessage(void* m){ return false; }
void DRKeyMouseWin32::UpdateMousePos(int x, int y){}

#endif  // _WIN32

}	//	namespace Spr

#include "Device.h"
#pragma hdrstop
#include <HumanInterface/HISdk.h>
#include <HumanInterface/DRKeyMouseWin32.h>
#include <Windows.h>
#include <iomanip>

namespace Spr {;

//	DRKeyMouseWin32

IF_OBJECT_IMP(DRKeyMouseWin32, HIRealDevice);

DRKeyMouseWin32::KeyMouse::KeyMouse(DRKeyMouseWin32* w):win32(w){
	strcpy(name, Type());
	strcat(name, "Win32");
}
void DRKeyMouseWin32::KeyMouse::SetHandler(DVKeyMouseHandler* h){
	DRKeyMouseWin32* kmw32 = RealDevice()->Cast();
	kmw32->SetHandler(h);
}
int DRKeyMouseWin32::KeyMouse::GetKeyState(int key){
	int rv = DVKeyMouseIf::NONE;
	short got = ::GetKeyState(key);
	if (got & 0x1) rv |= DVKeyMouseIf::TOGGLE_ON;
	if (got & 0x8000) rv |= DVKeyMouseIf::PRESS;
	return rv;
}
///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
DVKeyMouseIf::DVMousePosition DRKeyMouseWin32::KeyMouse::GetMousePosition(int count){
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
//	DRKeyMouseWin32
//

DRKeyMouseWin32::DRKeyMouseWin32(): handler(NULL){
	strcpy(name, Type());
	strcat(name, "Win32");
}
bool DRKeyMouseWin32::Init(){
	return true;
}
void DRKeyMouseWin32::Register(HISdkIf* intf){
	HISdk* sdk = intf->Cast();
	sdk->RegisterVirtualDevice((new DRKeyMouseWin32::KeyMouse(this))->Cast());
}

}	//	namespace Spr

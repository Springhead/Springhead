#ifndef DVKEYMOUSE_H
#define DVKEYMOUSE_H

#include <HumanInterface/HIVirtualDevice.h>
namespace Spr {;

///	DVKeyMouse
class SPR_DLL DVKeyMouse:public HIVirtualDevice, public DVKeyMouseIfInit{
public:
	OBJECTDEF_ABST(DVKeyMouse, HIVirtualDevice);
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	デバイスの種類
	static const char* TypeS() { return "KeyMouse"; }
	///	デバイスの種類
	virtual const char* Type() const{ return TypeS(); }
	///	ハンドラの設定
	virtual void SetHandler(DVKeyMouseHandler* h)=0;
	///	マウスボタン・キーボード状態取得
	virtual int GetKeyState(int key)=0;
	///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
	virtual DVKeyMouseIf::DVMousePosition GetMousePosition(int count=0){
		return DVKeyMouseIf::DVMousePosition();
	}
};

}	//	namespace Spr
#endif

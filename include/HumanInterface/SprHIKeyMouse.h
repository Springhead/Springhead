#ifndef SPR_HIKEYMOUSE_H
#define SPR_HIKEYMOUSE_H

#include <HumanInterface/SprHIBase.h>
namespace Spr {;

struct DVKeyMouseHandler{
	/// 押されているキー，ボタンの状態を示すフラグ
	enum ButtonState{
		NONE,
		LBUTTON = 0x01,
		RBUTTON = 0x02,
		SHIFT   = 0x04,
		CONTROL = 0x08,
		MBUTTON = 0x10,		
	};
	/** マウスが動いたときの処理
		@param keyState 押されているキー，ボタンの状態
	*/
	virtual void OnMouseMove(int keyState, int x, int y){}
	///	キー，マウスボタン処理
	virtual bool OnKey(bool isDown, int vkey, int keyState, int x, int y){
		return false;
	}
};


///	DVKeyMouse
struct DVKeyMouseIf:public HIVirtualDeviceIf{
	///	マウスの位置を返すための構造体
	struct DVMousePosition{
		int x;
		int y;
		unsigned time;
	};
	///	キーの状態
	enum DVKeyState{
		NONE	= 0,
		PRESS	= 1,
		TOGGLE_ON = 2,
		BOTH	= PRESS|TOGGLE_ON,
	};
	IF_DEF(DVKeyMouse);
	///	デバイスの名前
	virtual const char* Name() const =0;
	///	ハンドラの設定
	virtual void SetHandler(DVKeyMouseHandler* h)=0;
	///	マウスボタン・キーボード状態取得
	virtual int GetKeyState(int key) = 0;
	///	マウス位置取得関数	0が最新，1以上は過去の履歴を取得
	virtual DVKeyMouseIf::DVMousePosition GetMousePosition(int count=0)=0;
};

///	DRKeyMouseWin32
struct DRKeyMouseWin32If:public HIRealDeviceIf{
	IF_DEF(DRKeyMouseWin32);
};

}	//	namespace Spr
#endif

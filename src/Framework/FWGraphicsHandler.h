#ifndef FW_GRAPHICSHANDLER_H
#define FW_GRAPHICSHANDLER_H

/** FWGraphicsHandler
	処理系に依存したウィンドウの作成・管理機能
	＊機能を考えるとFWWindowManagerの方が合う気がする tazz
 */

#include <Foundation/Object.h>

namespace Spr{;

class FWWinBase;
class FWWin;
class FWDialog;
class FWControl;
class FWPanel;
class FWButton;
class FWTextBox;
class FWRotationControl;
class FWTranslationControl;
class FWListBox;
struct GRDeviceIf;

class FWGraphicsHandler : public Object{
public:
	SPR_OBJECTDEF_ABST_NOIF(FWGraphicsHandler);

	static UTRef<FWGraphicsHandler>	instance;

	GRDeviceIf*		grDevice;		///< グラフィクスデバイスへの参照
public:
	FWGraphicsHandler():grDevice(NULL){}
	///	初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc = 0, char** argv = NULL){}	
	/// メインループを開始
	virtual void StartMainLoop()=0;
	/// Idle callbackの有効化/無効化
	virtual void EnableIdleFunc(bool on = true)=0;
	/// フルスクリーンモードへの切り替え．API名は暫定的にGLUTに準拠
	virtual void EnterGameMode()=0;
	virtual void LeaveGameMode()=0;

	/** ウィンドウ */
	///	ウィンドウを作成し、ウィンドウ IDを返す
	virtual FWWinIf* CreateWin(const FWWinDesc& d, FWWinIf* parent){ return NULL; }
	///	ウィンドウを破棄する
	virtual void DestroyWin(FWWinIf* w){}
	///	カレントウィンドウを設定する
	virtual void SetCurrentWin(FWWinIf* w){}
	///	カレントウィンドウを返す。
	virtual int GetCurrentWin(){ return 0; }
	/// カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	virtual void PostRedisplay(){}
	/// Shift,Ctrl,Altのステートを返す
	virtual int GetModifiers(){ return 0 /*NULL*/; }
	/// 再描画時のビューポート計算
	virtual void CalcViewport(int& l, int& t, int& w, int& h){}

	/// ダイアログ作成
	virtual FWDialog*	CreateDialog(FWWin* owner, const FWDialogDesc& desc){ return 0; }
	/// GUIアイテム作成
	virtual FWControl*	CreateControl(FWDialog* owner, const IfInfo* ii, const FWControlDesc& desc, FWPanel* parent){ return 0; }
	virtual void	CreateColumn	 (FWDialog* owner, bool sep, FWPanel* panel){}
	virtual void	CreateSeparator	 (FWDialog* owner, FWPanel* panel){}
	/// 属性操作
	virtual Vec2i	GetPosition		(FWWinBase* win){ return Vec2i(); }
	virtual void	SetPosition		(FWWinBase* win, int left, int top){}
	virtual Vec2i	GetSize			(FWWinBase* win){ return Vec2i(); }
	virtual void	SetSize			(FWWinBase* win, int width, int height){}
	virtual void	SetTitle		(FWWinBase* win, UTString title){}
	virtual void	SetFullScreen	(FWWin* win){}
	virtual void	SetLabel		(FWControl* ctrl, UTString label){}
	virtual void	SetAlign		(FWControl* ctrl, int align){}
	virtual void	SetStyle		(FWControl* ctrl, int style){}
	virtual int		GetInt			(FWControl* ctrl){ return 0; }
	virtual void	SetInt			(FWControl* ctrl, int val){}
	virtual float	GetFloat		(FWControl* ctrl){ return 0.0f; }
	virtual void	SetFloat		(FWControl* ctrl, float val){}
	virtual void	SetChecked		(FWButton*	btn,  bool on){}
	virtual bool	IsChecked		(FWButton*	btn){ return false; }
	virtual void	SetIntRange		(FWTextBox* text, int rmin, int rmax){}
	virtual void	SetFloatRange	(FWTextBox* text, float rmin, float rmax){}
	virtual const char*	GetString	(FWTextBox* text){ return 0; }
	virtual void	SetString		(FWTextBox* text, char* str){}
	virtual Matrix3f GetRotation	(FWRotationControl* rotCtrl){ return Matrix3f(); }
	virtual void	SetRotation		(FWRotationControl* rotCtrl, const Matrix3f& rot){}
	virtual void	SetDamping		(FWRotationControl* rotCtrl, float d){}
	virtual void	Reset			(FWRotationControl* rotCtrl){}
	virtual	Vec3f	GetTranslation	(FWTranslationControl* trnCtrl){ return Vec3f(); }
	virtual	void	SetTranslation	(FWTranslationControl* trnCtrl, Vec3f p){}
	virtual void	SetSpeed		(FWTranslationControl* trnCtrl, float sp){}
	virtual void	AddItem			(FWListBox*	listBox, UTString label){}
	
	/// グラフィクスデバイスの取得
	GRDeviceIf* GetGRDevice(){ return grDevice; }

};

}

#endif

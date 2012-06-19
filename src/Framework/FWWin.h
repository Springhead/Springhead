#ifndef FW_WINDOW_H
#define FW_WINDOW_H

#include <Framework/SprFWWin.h>
#include <Framework/FrameworkDecl.hpp>
#include <Foundation/Object.h>
#include <HumanInterface/SprHITrackball.h>
#include <vector>

namespace Spr{;

struct FWSceneIf;

class FWWinBase : public Object{
public:
	SPR_OBJECTDEF(FWWinBase);
	SPR_DECLMEMBEROF_FWWinBaseDesc;

	int			id;			///< ウィンドウID
	void*		handle;		///< 内部オブジェクトのハンドル
	
	int			GetID()			{ return id; }
	
	UTString	GetTitle()		{ return title; }
	void		SetTitle(UTString title);
	
	Vec2i			GetPosition();
	virtual void	SetPosition(int left, int top);
	
	Vec2i			GetSize();	
	virtual void	SetSize(int width, int height);
	
};

class FWWin : public FWWinBase, public HITrackballCallback/*, public HIDraggerCallback*/{
public:
	SPR_OBJECTDEF(FWWin);
	SPR_DECLMEMBEROF_FWWinDesc;

	GRRenderIf*			render;			///< レンダラ
	FWSceneIf*			scene;			///< シーン

	DVKeyMouseIf*		keyMouse;		///< キーボード・マウス
	DVJoyStickIf*		joyStick;		///< ジョイスティック
	HITrackballIf*		trackball;		///< トラックボール
	//HIDraggerIf*		dragger;		///< ドラッガー

	std::vector<FWDialogIf*>	childDlg;

public:
	// ダイアログの作成
	FWDialogIf*		CreateDialog(const FWDialogDesc& desc);
	// ビューポート計算
	void	CalcViewport(int& left, int& top, int& width, int& height);
	// フルスクリーン
	void	SetFullScreen();
	bool	GetFullScreen(){ return fullscreen; }
	//
	virtual void	SetPosition(int left, int top);
	virtual void	SetSize(int width, int height);
	
	// レンダラとシーンの割当てと取得
	GRRenderIf*	GetRender(){ return render; }
	void		SetRender(GRRenderIf* data){render = data;}
	FWSceneIf*  GetScene(){ return scene; }
	void		SetScene(FWSceneIf* s){ scene = s; }

	// HumanInterfaceデバイスの登録と取得
	DVKeyMouseIf*	GetKeyMouse(){ return keyMouse; }
	void			SetKeyMouse(DVKeyMouseIf* dv){ keyMouse = dv; }
	DVJoyStickIf*	GetJoyStick(){ return joyStick; }
	void			SetJoyStick(DVJoyStickIf* dv){ joyStick = dv; }
	HITrackballIf*	GetTrackball(){ return trackball; }
	void			SetTrackball(HITrackballIf* dv){
		trackball = dv;
		trackball->SetKeyMouse(keyMouse);
		trackball->SetCallback(this);
	}
	/*HIDraggerIf*	GetDragger(){ return dragger; }
	void			SetDragger(HIDraggerIf* dv){
		dragger = dv;
		dragger->SetKeyMouse(keyMouse);
		dragger->SetCallback(this);
	}*/

	// 描画モードの設定と取得
	void	SetDebugMode(bool ph_or_gr){ debugMode = ph_or_gr; }
	bool	GetDebugMode(){ return debugMode; }
	// 描画
	void	Display();
	
	// Objectの関数
	bool	AddChildObject(ObjectIf* o);
	size_t	NChildObject() const;

	// HITrackballCallbackの関数
	virtual void OnUpdatePose(HITrackballIf*);

	FWWin(){
		render		= 0;
		scene		= 0;
		keyMouse	= 0;
		joyStick	= 0;
		trackball	= 0;
		//dragger		= 0;
	}
	virtual ~FWWin(){}
};

class FWDialog : public FWWin{
public:
	SPR_OBJECTDEF(FWDialog);
	SPR_DECLMEMBEROF_FWDialogDesc;

	FWControlIf*			CreateControl			(const IfInfo* ii, const FWControlDesc& desc, FWPanelIf* parent);
	FWButtonIf*				CreatePushButton		(UTString label, FWPanelIf* parent);
	FWButtonIf*				CreateCheckButton		(UTString label, bool checked, FWPanelIf* parent);
	FWButtonIf*				CreateRadioButton		(UTString label, FWPanelIf* parent);
	FWStaticTextIf*			CreateStaticText		(UTString text,  FWPanelIf* parent);
	FWTextBoxIf*			CreateTextBox			(UTString label, UTString text, int style, FWPanelIf* parent);
	FWPanelIf*				CreatePanel				(UTString label, int style, FWPanelIf* parent);
	FWPanelIf*				CreateRadioGroup		(FWPanelIf* parent);
	FWRotationControlIf*	CreateRotationControl	(UTString label, FWPanelIf* parent);
	FWTranslationControlIf*	CreateTranslationControl(UTString label, int style, FWPanelIf* parent);
	FWListBoxIf*			CreateListBox			(UTString label, FWPanelIf* parent);
	void					CreateColumn			(bool sep, FWPanelIf* parent);
	void					CreateSeparator			(FWPanelIf* parent);	
};

class FWControl : public FWWinBase{
public:
	SPR_OBJECTDEF(FWControl);
	SPR_DECLMEMBEROF_FWControlDesc;

	int		GetStyle(){ return style; }
	
	UTString	GetLabel(){ return label; }
	void		SetLabel(UTString l);

	void	SetAlign(int align);
	void	SetStyle(int style);

	int		GetInt();
	void	SetInt(int val);
	float	GetFloat();
	void	SetFloat(float val);

};

class FWPanel : public FWControl{
public:
	SPR_OBJECTDEF(FWPanel);
	SPR_DECLMEMBEROF_FWPanelDesc;
};

class FWButton : public FWControl{
public:
	SPR_OBJECTDEF(FWButton);
	SPR_DECLMEMBEROF_FWButtonDesc;

	void	SetChecked(bool on = true);
	bool	IsChecked();

	void		SetRadioGroup(FWPanelIf*);
	FWPanelIf*	GetRadioGroup();
};

class FWStaticText : public FWControl{
public:
	SPR_OBJECTDEF(FWStaticText);
	SPR_DECLMEMBEROF_FWStaticTextDesc;
};

class FWTextBox : public FWControl{
public:
	SPR_OBJECTDEF(FWTextBox);
	SPR_DECLMEMBEROF_FWTextBoxDesc;

	void	GetIntRange(int& rmin, int& rmax){
		rmin = intMin;
		rmax = intMax;
	}
	void	GetFloatRange(float& rmin, float& rmax){
		rmin = floatMin;
		rmax = floatMax;
	}

	void	SetIntRange(int rmin, int rmax);
	void	SetFloatRange(float rmin, float rmax);

	const char*	GetString();
	void		SetString(char* str);
	
};

class FWListBox : public FWControl{
	SPR_OBJECTDEF(FWListBox);
	SPR_DECLMEMBEROF_FWListBoxDesc;

	std::vector<UTString>	items;

	void	AddItem(UTString label);
};

class FWRotationControl : public FWControl{
	SPR_OBJECTDEF(FWRotationControl);
	SPR_DECLMEMBEROF_FWRotationControlDesc;

	Matrix3f	GetRotation();
	void		SetRotation(const Matrix3f& rot);
	float		GetDamping(){ return damping; }
	void		SetDamping(float d);
	void		Reset();
};

class FWTranslationControl : public FWControl{
	SPR_OBJECTDEF(FWTranslationControl);
	SPR_DECLMEMBEROF_FWTranslationControlDesc;

	Vec3f	GetTranslation();
	void	SetTranslation(Vec3f p);
	float	GetSpeed(){ return speed; }
	void	SetSpeed(float sp);

};

}
#endif 
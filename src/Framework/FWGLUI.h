/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWGLUI_H
#define FWGLUI_H

#include <Framework/FWGLUT.h>

#include <GL/glui.h>

/** @brief GLUIを用いるアプリケーションクラス
		   基本的にFWAppGLUIを自分のアプリケーションのクラスに継承させ，
		   DesignGUIを必ずオーバーライドして使用する.
		   CreateGUIでFWAppGLUIDescの情報を基にGLUI*型の変数を取得する．
		   GLUI*型の変数からツリー状にUIのパーツとコールバックを定義する（DesignGUIの実装）
		   glutTimerFuncをGLUI_Master.set_glutTimerFuncにしないと固まる．
		   あとはglutMainLoop();
		   GLUI*がNULLだとglutと同じ動きになるのでDesignGUIを呼ばないのならCreateGUIを呼ぶ必要も無い．
		   必要に応じてDesignGUIから派生するコールバック関数をアプリケーションクラスに作成する必要がある．
		   自分のアプリケーションクラスに作成するコールバックは，呼び出し規約に注意して作成すること．
		   (クラス内の関数定義は必ず__thiscallになる)   
	*/

namespace Spr{;

class FWGLUI : public FWGLUT{
public:
	SPR_OBJECTDEF_NOIF(FWGLUI);
protected:
	// コールバック処理を行うコントロールの配列
	// ボタン，テキストボックス
	std::vector<FWControl*>	ctrls;

	/// インスタンス取得
	static FWGLUI* GetInstance();

	/** コールバック関数*/
	static void SPR_CDECL GLUIUpdateFunc(int id);
	
	virtual void RegisterCallbacks();
public:
	virtual void EnableIdleFunc(bool on = true);
	virtual void CalcViewport(int& l, int& t, int& w, int& h);

	virtual FWDialog*	CreateDialog	(FWWin* owner, const FWDialogDesc& desc);
	virtual FWControl*	CreateControl	(FWDialog* owner, const IfInfo* ii, const FWControlDesc& desc, FWPanel* parent);
	virtual void		CreateColumn	(FWDialog* owner, bool sep, FWPanel* panel);
	virtual void		CreateSeparator	(FWDialog* owner, FWPanel* panel);
	virtual void		SetSize			(FWWinBase* win, int width, int height);
	virtual void		SetLabel		(FWControl* win, UTString label);
	virtual void		SetAlign		(FWControl* ctrl, int align);
	virtual int			GetInt			(FWControl* ctrl);
	virtual void		SetInt			(FWControl* ctrl, int val);
	virtual float		GetFloat		(FWControl* ctrl);
	virtual void		SetFloat		(FWControl* ctrl, float val);
	virtual void		SetChecked		(FWButton*	btn,  bool on);
	virtual bool		IsChecked		(FWButton*	btn);
	virtual void		SetIntRange		(FWTextBox* text, int rmin, int rmax);
	virtual void		SetFloatRange	(FWTextBox* text, float rmin, float rmax);
	virtual const char*	GetString		(FWTextBox* text);
	virtual void		SetString		(FWTextBox* text, const char* str);
	virtual Matrix3f	GetRotation		(FWRotationControl* rotCtrl);
	virtual void		SetRotation		(FWRotationControl* rotCtrl, const Matrix3f& rot);
	virtual void		SetDamping		(FWRotationControl* rotCtrl, float d);
	virtual void		Reset			(FWRotationControl* rotCtrl);
	virtual	Vec3f		GetTranslation	(FWTranslationControl* trnCtrl);
	virtual	void		SetTranslation	(FWTranslationControl* trnCtrl, Vec3f p);
	virtual void		SetSpeed		(FWTranslationControl* trnCtrl, float sp);
	virtual void		AddItem			(FWListBox*	listBox, UTString label);

	FWGLUI(){}
	~FWGLUI();
};
/*
class FWWinGLUI : public FWWinGLUT{
public:
	virtual FWDialogIf*		CreateDialog(const FWDialogDesc& desc);
};

class FWDialogGLUI : public FWDialog{
public:
	virtual FWControlIf*	CreateControl(const FWControlDesc& desc);
};

class FWControlGLUI : public FWControl{
public:
	virtual UTString	GetLabel();
	virtual void		SetLabel(UTString label);
	virtual void		SetAlign(int align);
	virtual int			GetStyle();
	virtual void		SetStyle(int style);
};

class FWPanelGLUI : public FWPanel{
public:
	virtual void	Open(bool on = true);
	virtual void	CreateColumn(bool sep = true);
	virtual void	CreateSeparator();
};

class FWButtonGLUI : public FWButton{
public:
	virtual void	SetChecked(bool on = true);
	virtual bool	IsChecked();

	virtual	void		SetRadioGroup(FWPanelIf*);
	virtual	FWPanelIf*	GetRadioGroup();
};

class FWStaticTextGLUI : public FWStaticText{
public:

};

class FWTextBoxGLUI : public FWTextBox{
public:
	virtual void	SetIntRange(int rmin, int rmax);
	virtual void	GetIntRange(int& rmin, int& rmax);
	virtual void	SetFloatRange(int rmin, int rmax);
	virtual void	GetFloatRange(int& rmin, int& rmax);

};

class FWListBoxGLUI : public FWListBox{
public:

};

class FWRotationControlGLUI : public FWRotationControl{
public:
};

class FWTranslationControlGLUI : public FWTranslationControl{
public:
};
*/
}

#endif
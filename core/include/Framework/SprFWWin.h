#ifndef SPR_FW_WINDOW_H
#define SPR_FW_WINDOW_H

/** SprFWWin.h
	- ウィンドウとGUIの提供
	- 実際に利用可能な機能は選択されているグラフィクスシステムに依存する

	- GUIコントロールの仕様はGLUIに基づいている．
		- コントロールの配置はFWColumn(横配置)とFWPanel(縦配置)を利用する
 **/

#include <Foundation/SprObject.h>

namespace Spr{;

struct GRRenderIf;
struct DVKeyMouseIf;
struct DVJoyStickIf;
struct HITrackballIf;
//struct HIDraggerIf;

/** \addtogroup gpFramework */
//@{

struct FWSceneIf;
struct FWPanelIf;
struct FWStaticTextIf;
struct FWButtonIf;
struct FWTextBoxIf;
struct FWRotationControlIf;
struct FWTranslationControlIf;
struct FWListBoxIf;

/** @ウィンドウの基底クラス
 */
struct FWWinBaseIf : ObjectIf{
	SPR_IFDEF(FWWinBase);

	/// IDを取得する
	int		GetID();

	/// 位置を取得する
	Vec2i	GetPosition();
	/// 位置を変更する
	void	SetPosition(int left, int top);
	
	/// サイズを取得する
	Vec2i	GetSize();
	/// サイズを変更する
	void	SetSize(int width, int height);
	
	/// タイトルを取得する
	UTString GetTitle();
	/// タイトルを変更する
	void	SetTitle(UTString title);
	
};
struct FWWinBaseDesc{
	SPR_DESCDEF(FWWinBase);

	int			width;			///<	幅
	int			height;			///<	高さ
	int			left;			///<	左端の座標
	int			top;			///<	上端の座標
	UTString	title;			///<	ウィンドウのタイトル文字列(トップレベルウィンドウのみ)

	FWWinBaseDesc(){
		width = 640;
		height = 480;
		left = 100;
		top = 100;
		title = "Springhead2";
	};
};

/** GUIアイテム
 */
struct FWControlIf : FWWinBaseIf{
	SPR_IFDEF(FWControl);

	/// ラベルの取得
	UTString	GetLabel();
	/// ラベルの設定
	void		SetLabel(UTString l);

	/// 配置の設定
	void	SetAlign(int align);

	/// スタイルの取得
	int		GetStyle();
	/// スタイルの設定
	void	SetStyle(int style);

	/// int値を取得
	int		GetInt();
	/// int値を設定
	void	SetInt(int val);
	/// float値を取得
	float	GetFloat();
	/// float値を設定
	void	SetFloat(float val);

};
struct FWControlDesc{
	SPR_DESCDEF(FWControl);

	enum {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
	};
	UTString	label;
	int			align;
	int			style;

	FWControlDesc(){
		align = ALIGN_LEFT;
		style = 0;
	}
};


/// パネル
struct FWPanelIf : FWControlIf{
	SPR_IFDEF(FWPanel);

};
struct FWPanelDesc : FWControlDesc{
	SPR_DESCDEF(FWPanel);

	enum{
		NONE,
		RAISED,
		EMBOSSED,
		ROLLOUT,
		RADIOGROUP,
	};
	bool	open;		///< 作成直後に開いているかどうか（ROLLOUTスタイル限定）
};

/// ボタン
struct FWButtonIf : FWControlIf{
	SPR_IFDEF(FWButton);

	/// チェック状態の設定
	void	SetChecked(bool on = true);
	/// チェック状態の取得
	bool	IsChecked();
};
struct FWButtonDesc : FWControlDesc{
	SPR_DESCDEF(FWButton);

	enum{
		PUSH_BUTTON,
		CHECK_BUTTON,
		RADIO_BUTTON,
	};
	int		style;
	bool	checked;

	FWButtonDesc(){
		style = PUSH_BUTTON;
		checked = false;
	}
};

/// 静的テキスト
struct FWStaticTextIf : FWControlIf{
	SPR_IFDEF(FWStaticText);
};
struct FWStaticTextDesc : FWControlDesc{
	SPR_DESCDEF(FWStaticText);
};

/// テキストボックス
struct FWTextBoxIf : FWControlIf{
	SPR_IFDEF(FWTextBox);

	/// int範囲設定
	void	SetIntRange(int rmin, int rmax);
	/// int範囲取得
	void	GetIntRange(int& rmin, int& rmax);
	/// float範囲設定
	void	SetFloatRange(float rmin, float rmax);
	/// float範囲取得
	void	GetFloatRange(float& rmin, float& rmax);

	/// 文字列を取得
	const char*	GetString();
	/// 文字列を設定
	void		SetString(char* str);

};
struct FWTextBoxDesc : FWControlDesc{
	SPR_DESCDEF(FWTextBox);

	enum{
		TEXT,
		INT,
		FLOAT,
	};

	UTString	text;
	int			intMin;
	int			intMax;
	float		floatMin;
	float		floatMax;

	bool	spinner;

	FWTextBoxDesc(){
		style = TEXT;
		intMin = 0;
		intMax = 0;
		floatMin = 0.0f;
		floatMax = 0.0f;
		spinner = false;
	}
};

/** リストボックス
	 選択されているアイテムの取得はGetInt, アイテムの選択はSetIntで行う．
	 選択アイテムが変化するとOnControlUpdateが呼ばれる
 **/
struct FWListBoxIf : FWControlIf{
	SPR_IFDEF(FWListBox);

	/// アイテムを追加
	void	AddItem(UTString label);
};
struct FWListBoxDesc : FWControlDesc{
	SPR_DESCDEF(FWListBox);
};

/// 回転コントロール
struct FWRotationControlIf : FWControlIf{
	SPR_IFDEF(FWRotationControl);

	/// 回転の取得
	Matrix3f	GetRotation();
	/// 回転の設定
	void		SetRotation(const Matrix3f& rot);
	/// ダンピング係数の取得
	float		GetDamping();
	/** @brief ダンピング係数の設定
		@param d ダンピング係数
		マウスボタンをリリースした後の回転の減衰係数を設定する．
		dが0.0だと即座に停止し，dが1.0だと回転しつづける．
	 **/
	void		SetDamping(float d);
	/// 回転のリセット
	void		Reset();
};
struct FWRotationControlDesc : FWControlDesc{
	SPR_DESCDEF(FWRotationControl);

	Matrix3f	rot;
	float		damping;

	FWRotationControlDesc(){
		damping = 0.0f;
	}
};

/// 平行移動コントロール
struct FWTranslationControlIf : FWControlIf{
	SPR_IFDEF(FWTranslationControl);

	/// 変位の取得
	Vec3f	GetTranslation();
	/// 変位の設定
	void	SetTranslation(Vec3f p);
	/// スピードの取得
	float	GetSpeed();
	/** @brief	スピードの設定
		@param	sp
		マウス移動量から変位の変化量にかかる係数を設定する
	 */
	void	SetSpeed(float sp);
};
struct FWTranslationControlDesc : FWControlDesc{
	SPR_DESCDEF(FWTranslationControl);

	enum{
		TRANSLATION_XY,
		TRANSLATION_X,
		TRANSLATION_Y,
		TRANSLATION_Z,
	};

	Vec3f	pos;
	float	speed;

	FWTranslationControlDesc(){
		style = TRANSLATION_XY;
		speed = 1.0f;
	}
};

/**
	ダイアログボックス(GUIコンテナ)
 */
struct FWDialogIf : FWWinBaseIf{
	SPR_IFDEF(FWDialog);

	/** @brief GUIアイテムを作成
		@param	desc	ディスクリプタ
		@param	parent	親パネル
		作成するGUIアイテムの種類はディスクリプタの型で判定される．
		ラジオボタンを作成する場合はparentがラジオグループでなければならない．
	 **/
	FWControlIf*		CreateControl(const IfInfo* ii, const FWControlDesc& desc, FWPanelIf* parent = 0);
	/** 以下短縮形API **/
	/// プッシュボタン
	FWButtonIf*			CreatePushButton(UTString label, FWPanelIf* parent = 0);
	/// チェックボタン
	FWButtonIf*			CreateCheckButton(UTString label, bool checked = false, FWPanelIf* parent = 0);
	/// ラジオボタン
	FWButtonIf*			CreateRadioButton(UTString label, FWPanelIf* parent = 0);
	/// 静的テキスト
	FWStaticTextIf*		CreateStaticText(UTString text, FWPanelIf* parent = 0);
	/// テキストボックス
	FWTextBoxIf*		CreateTextBox(UTString label, UTString text = "", int style = FWTextBoxDesc::TEXT, FWPanelIf* parent = 0);
	/// パネル
	FWPanelIf*			CreatePanel(UTString label, int style = FWPanelDesc::RAISED, FWPanelIf* parent = 0);
	/// ラジオボタングループ
	FWPanelIf*			CreateRadioGroup(FWPanelIf* parent = 0);
	/// 回転コントロール
	FWRotationControlIf*	CreateRotationControl(UTString label, FWPanelIf* parent = 0);
	/// 並進コントロール
	FWTranslationControlIf*	CreateTranslationControl(UTString label, int style = FWTranslationControlDesc::TRANSLATION_XY, FWPanelIf* parent = 0);
	/// リストボックス
	FWListBoxIf*			CreateListBox(UTString label, FWPanelIf* parent = 0);
	/// 新しい列を作成
	void	CreateColumn(bool sep = true, FWPanelIf* parent = 0);
	/// 水平セパレータを作成
	void	CreateSeparator(FWPanelIf* parent = 0);
	
};
struct FWDialogDesc : FWWinBaseDesc{
	SPR_DESCDEF(FWDialog);

	enum{
		DOCK_LEFT,
		DOCK_RIGHT,
		DOCK_TOP,
		DOCK_BOTTOM,
	};

	/// ドッキングタイプかどうか
	bool	dock;

	/// ドッキングタイプの場合の位置
	int		dockPos;

	FWDialogDesc(){
		dock	= false;
		dockPos = DOCK_TOP;
	}

};

/**	@brief ウィンドウ
	window ID, scene, renderを持つ．
 */
struct FWWinIf : FWWinBaseIf{
	SPR_IFDEF(FWWin);

	/// フルスクリーンにする
	void	SetFullScreen();
	/// フルスクリーンかどうか
	bool	GetFullScreen();

	/// 登録されているレンダラを取得する
	GRRenderIf*	GetRender();
	/// レンダラを登録する
	void		SetRender(GRRenderIf* data);
	/// 登録されているシーンを取得する
	FWSceneIf*  GetScene();
	/// シーンを登録する
	void		SetScene(FWSceneIf* s);

	/// キーボード・マウスデバイス
	DVKeyMouseIf*	GetKeyMouse();
	void			SetKeyMouse(DVKeyMouseIf* dv);
	/// ジョイスティックデバイス
	DVJoyStickIf*	GetJoyStick();
	void			SetJoyStick(DVJoyStickIf* dv);
	/// トラックボール
	HITrackballIf*	GetTrackball();
	void			SetTrackball(HITrackballIf* dv);
	/// ドラッガー
	//HIDraggerIf*	GetDragger();
	//void			SetDragger(HIDraggerIf* dv);

	/// 描画モード設定
	void	SetDebugMode(bool ph_or_gr);
	bool	GetDebugMode();
	/// 描画
	void	Display();
	
	/** @brief ダイアログの作成
		@param desc	ディスクリプタ
		このウィンドウの子としてダイアログを作成する
	 **/
	FWDialogIf*		CreateDialog(const FWDialogDesc& desc = FWDialogDesc());

	/** @brief	ビューポートの計算
		@param	left
		@param	top
		@param	width
		@param	height
		ドッキングタイプのダイアログがある場合，それを除いた残りの領域を計算する．
		カレントウィンドウである必要がある
	 */
	void	CalcViewport(int& left, int& top, int& width, int& height);
};
struct FWWinDesc : FWWinBaseDesc{
	SPR_DESCDEF(FWWin);

	bool		debugMode;		///<	デバッグ描画モードか
	bool		fullscreen;		///<	フルスクリーンにするかどうか
	bool		useKeyMouse;	///<	キーボード・マウスデバイスを使用するか
	bool		useJoyStick;	///<	ジョイスティックデバイスを使用するか
	int			joyStickPollInterval;
	bool		useTrackball;	///<	トラックボールインタフェースを使用するか
	//bool		useDragger;		///<	ドラッガーインタフェースを使用するか

	FWWinDesc(){
		debugMode		= true;
		fullscreen		= false;
		useKeyMouse		= true;
		useJoyStick		= false;
		joyStickPollInterval = 10;
		useTrackball	= true;
		//useDragger		= true;
	}
};
//@}
}
#endif 

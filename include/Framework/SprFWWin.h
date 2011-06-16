#ifndef SPR_FW_WINDOW_H
#define SPR_FW_WINDOW_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct GRRenderIf;
struct DVKeyMouseIf;
struct DVJoyStickIf;
struct HITrackballIf;
struct HIDraggerIf;
struct FWSceneIf;

struct FWWinDesc{
	int			width;			///<	幅
	int			height;			///<	高さ
	int			left;			///<	左端の座標
	int			top;			///<	上端の座標
	int			parentWindow;	///<	子ウィンドウを作る場合は、親のID、そうでなければ0
	UTString	title;			///<	ウィンドウのタイトル文字列(トップレベルウィンドウのみ)
	bool		fullscreen;		///<	フルスクリーンにするかどうか

	bool		useKeyMouse;	///<	キーボード・マウスデバイスを使用するか
	bool		useJoyStick;	///<	ジョイスティックデバイスを使用するか
	bool		useTrackball;	///<	トラックボールインタフェースを使用するか
	bool		useDragger;		///<	ドラッガーインタフェースを使用するか

	FWWinDesc(int w=640, int h=480, int l=-1, int t=-1, int p=0, bool f=false):
		width(w), height(h), left(l), top(t), parentWindow(p), fullscreen(f){
		title			= "Springhead2";
		useKeyMouse		= true;
		useJoyStick		= false;
		useTrackball	= true;
		useDragger		= true;
	}
};

/**	@brief ウィンドウ
	window ID, scene, renderを持つ．
 */
struct FWWinIf : ObjectIf{
	SPR_IFDEF(FWWin);

	/// 位置を変更する
	void	SetPosition(int left, int top);
	/// サイズを変更する
	void	SetSize(int width, int height);
	/// タイトルを変更する
	void	SetTitle(UTString title);
	/// タイトルを取得する
	UTString GetTitle();
	/// フルスクリーンにする
	void	SetFullScreen(bool full = true);
	/// フルスクリーンかどうか
	bool	GetFullScreen();
	/// 位置を取得する
	Vec2i	GetPosition();
	/// 幅を取得する
	int		GetWidth();
	/// 高さを取得する
	int		GetHeight();
	/// IDを取得する
	int		GetID();
	/// IDを設定する
	void	SetID(int newID);

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
	HIDraggerIf*	GetDragger();
	void			SetDragger(HIDraggerIf* dv);

	/// 描画モード設定
	void	SetDebugMode(bool ph_or_gr);
	bool	GetDebugMode();
	/// 描画
	void	Display();
	/// サイズ変更時の処理
	void	Reshape(int w, int h);
};

}
#endif 

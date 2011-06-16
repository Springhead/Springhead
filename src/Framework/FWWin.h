#ifndef FW_WINDOW_H
#define FW_WINDOW_H

#include <Framework/SprFWWin.h>
#include <Foundation/Object.h>
#include <HumanInterface/SprHITrackball.h>
#include <HumanInterface/SprHIDragger.h>

namespace Spr{;

struct FWSceneIf;

class FWWin : public Object, public FWWinDesc, public HITrackballCallback, public HIDraggerCallback{
public:
	SPR_OBJECTDEF(FWWin);

	int					id;				///< ウィンドウID
	bool				debugMode;		///< デバッグ描画モード
	
	GRRenderIf*			render;			///< レンダラ
	FWSceneIf*			scene;			///< シーン

	DVKeyMouseIf*		keyMouse;		///< キーボード・マウス
	DVJoyStickIf*		joyStick;		///< ジョイスティック
	HITrackballIf*		trackball;		///< トラックボール
	HIDraggerIf*		dragger;		///< ドラッガー

public:
	// 属性変更API（派生クラスが実装）
	virtual void SetPosition(int left, int top){}
	virtual void SetSize(int width, int height){}
	virtual void SetTitle(UTString title){}
	// 属性取得API
	UTString	GetTitle(){ return title; }
	void		SetFullScreen(bool full = true){ fullscreen = full;}
	bool		GetFullScreen(){ return fullscreen; }
	Vec2i		GetPosition(){ return Vec2i(left, top); }
	int			GetWidth(){ return width; }
	int			GetHeight(){ return height; }
	int			GetID() const{ return id; }
	void		SetID(int newID){id = newID;}
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
	HIDraggerIf*	GetDragger(){ return dragger; }
	void			SetDragger(HIDraggerIf* dv){
		dragger = dv;
		dragger->SetKeyMouse(keyMouse);
		dragger->SetCallback(this);
	}
	// 描画モードの設定と取得
	void	SetDebugMode(bool ph_or_gr){ debugMode = ph_or_gr; }
	bool	GetDebugMode(){ return debugMode; }
	// 描画
	void	Display();
	// サイズ変更時処理
	void	Reshape(int w, int h);

	// HITrackballCallbackの関数
	virtual void OnUpdatePose(HITrackballIf*, bool dir);

	FWWin(int wid = 0, const FWWinDesc& desc = FWWinDesc());
	virtual ~FWWin(){}
};

}
#endif 
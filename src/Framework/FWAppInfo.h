#ifndef FWAPPINFO_H
#define FWAPPINFO_H
#include <Springhead.h>
#include "FWGraphicsAdaptee.h"

namespace Spr{;

class FWVFuncBridge : public UTRefCount{
public:
	virtual void Link(void* pObj) = 0;
	virtual bool Display() = 0;
	virtual bool Reshape(int w, int h) = 0;
	virtual bool Keyboard(int key, int x, int y) = 0;
	virtual bool MouseButton(int button, int state, int x, int y) = 0;
	virtual bool MouseMove(int x, int y) = 0;
	virtual bool Step() = 0;
	virtual bool Idle() = 0;
	virtual bool Joystick(unsigned int buttonMask, int x, int y, int z) = 0;
	virtual void AtExit() = 0;
	virtual ~FWVFuncBridge(){}
};


//	hase	TypeDescができないようにクラスにしてある。TypeDesc側での対応が望ましい。
class FWWinDesc{
public:
	int width;			///<	幅
	int height;			///<	高さ
	int left;			///<	左端の座標
	int top;			///<	上端の座標
	int parentWindow;	///<	子ウィンドウを作る場合は、親のID、そうでなければ0
	UTString title;		///<	ウィンドウのタイトル文字列(トップレベルウィンドウのみ)
	bool fullscreen;	///<	フルスクリーンにするかどうか

	FWWinDesc(int w=640, int h=480, int l=-1, int t=-1, int p=0, bool f=false):
		width(w), height(h), left(l), top(t), parentWindow(p), fullscreen(f){
	}
};

/**	@brief ウィンドウ
	window ID, scene, renderを持つ．
	実装されるのはFWWinGlutなので，FWAppGlutでの使用が前提．
 */
class FWWin : public UTRefCount, public FWWinDesc{
protected:
	int id;
public:	// protectedでよいが互換性のため一時的に
	UTRef<GRRenderIf>	render;
	UTRef<FWSceneIf>	scene;

	FWWin(int wid, const FWWinDesc& d, GRRenderIf* r):FWWinDesc(d), id(wid), render(r){}
public:
	virtual void Position(int left, int top){}
	virtual void Reshape(int width, int height){}
	virtual void SetTitle(UTString title){}
	virtual UTString GetTitle(){ return UTString(); }
	virtual void SetFullScreen(bool full = true){ fullscreen = full; }
	virtual bool GetFullScreen(){ return fullscreen; }
	virtual int  GetWidth(){ return width; }
	virtual int  GetHeight(){ return height; }

	int			GetID() const{ return id; }
	void		SetID(int newID){id = newID;}
	GRRenderIf*	GetRender(){ return render; }
	void		SetRender(GRRenderIf* data){render = data;}
	FWSceneIf*  GetScene(){ return scene; }
	void		SetScene(FWSceneIf* s){ scene = s; }

	virtual ~FWWin(){}
};
typedef std::vector< UTRef<FWWin> > Wins;

struct MouseInfo{
	TVec2<int> lastPos;			/// 前回のカーソル座標
	bool left, middle, right;	/// ボタン押し下げ
	bool shift, ctrl, alt;		/// コントロールキー状態
	bool first;
	MouseInfo():left(false), middle(false), right(false), first(false){}
};

// 以下の定数はGLUTに合わせてある
enum MouseButtonType{
	LEFT_BUTTON = 0,
	MIDDLE_BUTTON = 1,
	RIGHT_BUTTON = 2,
};
enum MouseButtonState{
	BUTTON_DOWN = 0,
	BUTTON_UP = 1,
};
enum ModifierMask{
	ACTIVE_SHIFT = 1,
	ACTIVE_CTRL = 2,
	ACTIVE_ALT = 4,
};

/// カメラ情報
/// 本来はscene毎に個別に保持すべき。要修正
struct CameraInfo{
	Vec3f target;		/// 中心点
	Vec2f rot;			/// 経度角，緯度角
	float zoom;			/// 拡大率（対象からの距離）
	Vec2f rotRangeX, rotRangeY;
	Vec2f zoomRange;
	Affinef view;
	CameraInfo();
	void UpdateView();
};

/// 剛体ドラッグ機能.
struct DragInfo{
	PHRayIf* ray;			/// カーソル下の剛体を特定するためのPHRay
	PHSolidIf* cursor;		/// カーソル剛体
	PHSpringIf* spring;
	float	depth;
	DragInfo():ray(NULL), cursor(NULL), spring(NULL), depth(0.0f){}
};


/// タイマ．
class  GTimer : public UTRefCount{
public:
	static void SPR_CDECL GTimerFunc(int id);	///<	タイマーコールバック関数
	GTimer(int id);
private:
	unsigned id;								///<	タイマーID
	unsigned interval;							///<	タイマーの時間間隔[ms]
	//GTimerFunc* func;							///<	時間が来るたびに呼ばれるコールバック関数．							///<	コールバック関数に渡す引数
	//FWApp*				app;					///<	FWAppインスタンスへの参照
	//FWGraphicsAdaptee*	adaptee;
	//int adapteeNo;
public:
	void SetInterval(unsigned ms);				///<	タイマーの時間間隔の設定
	unsigned GetInterval(){return interval;}	///<	タイマーの時間間隔の取得
	//void SetCallback(GTimerFunc* f);
	void Create();
	void Loop();
};
}
#endif 
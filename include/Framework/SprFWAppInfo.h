#ifndef FWAPPINFO_H
#define FWAPPINFO_H
#include <Springhead.h>
#include <Framework/SprFWGraphicsAdaptee.h>
#include <Foundation/UTMMTimer.h>

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
	virtual UTString GetTitle(){ return title; }
	virtual void SetFullScreen(bool full = true){ fullscreen = full;}
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

struct FWMouseInfo{
	TVec2<int>	pos;				///< 現在のカーソル座標
	TVec2<int>	lastPos;			///< 前回のカーソル座標
	bool	left, middle, right;	///< ボタン押し下げ
	bool	shift, ctrl, alt;		///< コントロールキー状態
	bool	first;					///< ボタン押し下げイベントの直後かどうか

	FWMouseInfo();
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

/** ユーザインタフェースつきカメラ
	カーソル移動によって注視点からの距離，緯度，経度を操作できる
 */
class FWUICamera{
public:
	/// 状態
	Vec2f rot;						/// 経度角，緯度角
	float zoom;						/// 拡大率（対象からの距離）
	Vec3f target;					/// 注視点
	Affinef view;					/// 視点行列

	/// パラメータ
	Vec2f rotRangeX, rotRangeY;		/// 角度範囲
	Vec2f zoomRange;				/// 対象からの距離範囲
	
	float rotGain;					/// カーソル移動量から角度変化量（経度・緯度）へのゲイン
	float posGain;					/// カーソル移動量から注視点移動量へのゲイン
	float zoomGain;					/// カーソル移動量から距離変化率へのゲイン
	
public:
	/// カメラ位置情報をもとにビュー変換行列を設定
	void UpdateView();

	/// 注視点上にある半径radiusの球がちょうど視野に収まるようにzoomを設定する
	void Fit(const GRCameraDesc& cam, float radius);

	/**	@brief	視点の回転
		@param xrel	スクリーン上移動量（経度分）
		@param yrel	スクリーン上移動量（緯度分）
		@param px	1ピクセルの長さ
		カーソル移動量に対して緯度・経度を変化させる．
		pxは(スクリーンサイズ/ビューポートサイズ)
	 */
	void Rotate(int xrel, int yrel, Vec2f px);

	/** @brief	視点の移動
		@param xrel
		@param yrel
		@param px
	 */
	void Translate(int xrel, int yrel, Vec2f px);

	/** @brief	ズーム（注視点からの距離の操作）
		@param yrel	スクリーン上移動量
	 */
	void Zoom(int yrel);

	FWUICamera();
};

/** カーソルによる剛体のドラッグ機能
 */
class FWDragger{
public:
	PHSceneIf*	scene;
	Affinef		view;		///	
	GRRenderIf*	render;		/// 

	PHRayIf*	ray;		/// カーソル下の剛体を特定するためのPHRay
	PHSolidIf*	cursor;		/// カーソル剛体
	PHSpringIf* spring;		/// ドラッグするためのバネ
	float		depth;		/// ドラッグする剛体のカメラから見たz座標

public:
	/**	@brief	初期化
		@param	scene
		@param	view
		@param	render

		sceneを用いて内部のレイ，バネ，カーソル剛体が作られる．
		ビュー変換viewはドラッグの最中に変化しないと想定される．
		renderを用いてスクリーン・カメラ座標変換を行う．
	 */
	void Init(PHSceneIf* phScene, const Affinef& view, GRRenderIf* render);

	/**	@brief	剛体をつかむ
		@param	x		カーソルx座標
		@param	y		カーソルy座標
		@param	view	ビュー変換行列
		@param	render	レンダラ
		カメラからカーソル位置へ投射した直線が最初に交差する剛体をドラッグ対象に設定する．

	 */
	void Grab(int x, int y);

	/**	@brief	剛体をひっぱる
		@param	x
		@param	y
		@param	view
		@param	render
		カーソル位置に向かってバネの力で剛体をひっぱる．
		この際カメラの奥行方向には力は加わらない．
	 */
	void Drag(int x, int y);

	/**	@brief	剛体を離す
	 */
	void Release();

	FWDragger():ray(NULL), cursor(NULL), spring(NULL), depth(0.0f){}
};

/// タイマ．

class  FWTimer : public UTRefCount{
public:
	enum TimerType{
		GLUT = 0,
		MM = 1,
	};
	static void SPR_CDECL GLUTTimerFunc(int id);		///<	タイマーコールバック関数
	static void SPR_CDECL MultiMediaTimerFunc(int id);	///<	タイマーコールバック関数
	FWTimer(int id, TimerType t = GLUT);

private:
	UTMMTimer* mtimer;								///<　	マルチメディアタイマー
	TimerType timerType;							///<	タイマーのタイプ(GLUTタイマー、マルチメディアタイマー)
	unsigned id;									///<	タイマーID
	unsigned interval;								///<	タイマーの時間間隔[ms]
	unsigned resolution;							///<	タイマーの分解能[ms](マルチメディアタイマーのみ）

	//void SetTimerType(TimerType t){timerType = t;}	///<	タイマーのタイプの設定
	//TimerType GetTimerType(){return timerType;}		///<	タイマーのタイプの取得
public:
	int GetTimerId(){return id;}					///<	タイマーのid取得
	void SetInterval(unsigned ms);					///<	タイマーの時間間隔の設定
	unsigned GetInterval(){return interval;}		///<	タイマーの時間間隔の取得
	void SetResolution(unsigned r);					///<	タイマーの分解能の設定
	unsigned GetResolution(){return resolution;}	///<	タイマーの分解能の取得
	void Create();								///<	タイマーの作成
	void Clear();
	void Recreate();
	void Release();

};
}
#endif 
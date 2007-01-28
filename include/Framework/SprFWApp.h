#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <Framework/SprFWSdk.h>
#include <Framework/SprFWScene.h>

namespace Spr{;

class FWVFuncBridge : public UTRefCount{
public:
	virtual void Link(void* pObj) = 0;
	virtual bool Display() = 0;
	virtual bool Reshape(int w, int h) = 0;
	virtual bool Keyboard(unsigned char key, int x, int y) = 0;
	virtual bool MouseButton(int button, int state, int x, int y) = 0;
	virtual bool MouseMove(int x, int y) = 0;
	virtual bool Step() = 0;
	virtual void AtExit() = 0;
};

class FWApp{
protected:
	UTRef<FWSdkIf> fwSdk;
	UTRef<FWSceneIf> fwScene;
	UTRef<GRDebugRenderIf> grRender;
	UTRef<GRDeviceIf> grDevice;
	bool isRunning;

	void CallDisplay(){
		if(!vfBridge || !vfBridge->Display())
			Display();
	}
	void CallReshape(int w, int h){
		if(!vfBridge || !vfBridge->Reshape(w, h))
			Reshape(w, h);
	}
	void CallKeyboard(unsigned char key, int x, int y){
		if(!vfBridge || !vfBridge->Keyboard(key, x, y))
			Keyboard(key, x, y);
	}
	void CallMouseButton(int button, int state, int x, int y){
		if(!vfBridge || !vfBridge->MouseButton(button, state, x, y))
			MouseButton(button, state, x, y);
	}
	void CallMouseMove(int x, int y){
		if(!vfBridge || !vfBridge->MouseMove(x, y))
			MouseMove(x, y);
	}
	void CallStep(){
		if(!vfBridge || !vfBridge->Step())
			Step();
	}
public:

	UTRef<FWVFuncBridge>	vfBridge;

	enum DebugMode{
		DM_NONE,
		DM_DEBUG,
	} debugMode;
	FWApp();

	/// レンダラを取得
	GRRenderIf* GetRender(){return grRender;}
	/// シーンを取得
	FWSceneIf* GetScene(){return fwScene;}

	virtual ~FWApp();

	/** @brief 初期化
		FWAppオブジェクトの初期化を行う．最初に必ず呼ぶ．
	 */
	virtual void Init(int argc, char* argv[]);

	/** @brief コマンドライン引数の処理
		アプリケーションに渡されたコマンドライン引数を処理したい場合にオーバライドする
	 */
	virtual void ProcessArguments(int argc, char* argv[]);

	/** @brief シーンをファイルからロードする
		@param filename ファイル名
		指定されたファイルからシーンをロードする
	 */
	virtual void LoadScene(UTString filename);

	/** @brief シーンを描画する
		シーンが表示されるときに呼ばれる．
		描画処理をカスタマイズしたい場合にオーバライドする．
	 */
	virtual void Display();

	/** @brief 描画領域変更時の処理
		@param w 描画領域の横幅
		@param h 描画領域の縦幅
		ユーザによってウィンドウサイズが変更されたときなどに呼ばれる．
	 */
	virtual void Reshape(int w, int h);

	/** @brief キーボードイベントのハンドラ

	 */
	virtual void Keyboard(unsigned char key, int x, int y);

	/** @brief マウスイベントのハンドラ

	 */
	virtual void MouseButton(int button, int state, int x, int y);

	/** @brief マウスイベントのハンドラ

	 */
	virtual void MouseMove(int x, int y);

	/** @brief シミュレーションの実行

	 */
	virtual void Step();

	/** @brief デバッグモードの取得

	 */
	DebugMode GetDebugMode(){ return debugMode; }

	/** @brief デバッグモードの設定

	 */
	void SetDebugMode(DebugMode m){ debugMode = m; }

	FWSceneIf* GetFWScene(){ return fwScene; }
};

}
#endif

#ifndef FWAPPSAMPLE_H								// ヘッダファイルを作る時のおまじない
#define FWAPPSAMPLE_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWApp.h>					// Frameworkクラスが宣言されているヘッダファイル
#include <Framework/SprFWGLUI.h>					// Frameworkクラスが宣言されているヘッダファイル
#include <GL/glui.h>

using namespace Spr;								// Springheadの名前空間

class FWAppSample : public FWApp{
private:
	GLUI*			glui;
	GLUI_Panel*		panel;
	GLUI_Button*	button1;
public:
	FWAppSample();									// コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	static FWAppSample* ins;

	void Init(int argc, char* argv[]);			// GLUTの初期化
	virtual void Timer();
	void InitCameraView();							// カメラ座標		
	void BuildObject();								// 物体を作成
	void Step();										// シミュレーションを1ステップ進める
	void Display();									// 描画
	void Reset();										// シーンのリセット
	void Keyboard(int key, int x, int y);		// キーボード関数
	bool bDrawInfo;									// 接触力などを表示するか
	static void _cdecl TimerFunc(int id);
	void SPR_CDECL DesignGUI();
	void CallStep();
	static void SPR_CDECL CallDisplay();

private:
	static void _cdecl CallButton1(int control);
	void		Button1(int control);
}; 



#endif
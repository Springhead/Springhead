#ifndef FWAPPSAMPLE_H								// ヘッダファイルを作る時のおまじない
#define FWAPPSAMPLE_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWApp.h>					// Frameworkクラスが宣言されているヘッダファイル
#include <Framework/FWGLUI.h>					// Frameworkクラスが宣言されているヘッダファイル
#include <GL/glui.h>

using namespace Spr;								// Springheadの名前空間

class FWGLUISample : public FWApp{
private:
	GLUI*			glui;
	GLUI_Panel*		panel;
	GLUI_Button*	button1;
public:
	FWGLUISample();									// コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	
	virtual void Init(int argc, char* argv[]);		// GLUTの初期化
	virtual void TimerFunc(int id);
	virtual void Display();							// 描画
	virtual void Keyboard(int key, int x, int y);	// キーボード関数
	
	void InitCameraView();							// カメラ座標		
	void CreateObjects();							// 物体を作成
	void Reset();									// シーンのリセット
	bool bDrawInfo;									// 接触力などを表示するか
	void SPR_CDECL DesignGUI();
	
private:
	static void _cdecl CallButton1(int control);
	void		Button1(int control);
}; 



#endif
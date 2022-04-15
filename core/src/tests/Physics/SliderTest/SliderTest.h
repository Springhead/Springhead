#ifndef FWAPPSAMPLE_H								// ヘッダファイルを作る時のおまじない
#define FWAPPSAMPLE_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWApp.h>						// Frameworkクラスが宣言されているヘッダファイル

using namespace Spr;								// Springheadの名前空間

class SliderTest : public FWApp{
public:
	SliderTest();									// コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	virtual void Init(int argc, char* argv[]);		// GLUTの初期化
	virtual void TimerFunc(int id);
	virtual void Display();							// 描画
	virtual void Keyboard(int key, int x, int y);	// キーボード関数
	
	void InitCameraView();							// カメラ座標		
	void CreateObjects();							// 物体を作成
	void Reset();									// シーンのリセット
	bool bDrawInfo;									// 接触力などを表示するか
}; 

#endif
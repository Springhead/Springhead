#ifndef FWAPPSAMPLE_H								// ヘッダファイルを作る時のおまじない
#define FWAPPSAMPLE_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWAppGLUT.h>					// Frameworkクラスが宣言されているヘッダファイル

using namespace Spr;								// Springheadの名前空間

class FWAppSample : public FWAppGLUT, public UTRefCount{
public:
	FWAppSample();									// コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	void Init(int argc, char* argv[]);				// GLUTの初期化
	void InitCameraView();							// カメラ座標				
	void BuildObject();								// 物体を作成
	void Step();									// シミュレーションを1ステップ進める
	void Display();									// 描画
	void Keyboard(int key, int x, int y);			// キーボード関数
	bool bDrawInfo;									// 接触力などを表示するか
}; 

#endif
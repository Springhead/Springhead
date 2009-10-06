#ifndef FWXFILE_LOADER_H							// ヘッダファイルを作る時のおまじない
#define FWXFILE_LOADER_H							// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWApp.h>						// Frameworkクラスが宣言されているヘッダファイル

using namespace Spr;								// Springheadの名前空間

class FWXfileLoader : public FWApp{
public:
	std::string fileName;
	bool bDebug;									///< デバックモードの切り替え
	FWXfileLoader();								///< コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	virtual void Init(int argc, char* argv[]);		///< GLUTの初期化
	virtual void TimerFunc(int id);
	virtual void IdleFunc();
	virtual void Display();							///< 描画
	virtual void Keyboard(int key, int x, int y);	///< キーボード関数

	void InitCameraView();							///< カメラ座標				
	void Reset();
	void Step();									///< シミュレーションを1ステップ進める
}; 

#endif
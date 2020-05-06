#ifndef FWVRMLFILE_LOADER_H							// ヘッダファイルを作る時のおまじない
#define FWVRMLFILE_LOADER_H							// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWApp.h>						// Frameworkクラスが宣言されているヘッダファイル

using namespace Spr;								// Springheadの名前空間

class FWVrmlfileLoader : public FWApp{
public:
	std::string fileName;
	bool bDebug;									///< デバックモードの切り替え
	FWVrmlfileLoader();								///< コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	virtual void Init(int argc, char* argv[]);		///< GLUTの初期化
	virtual void Keyboard(int key, int x, int y);	///< キーボード関数

	void Reset();
	void Step();									///< シミュレーションを1ステップ進める
}; 

#endif
#ifndef FWAPPSAMPLE_H								// ヘッダファイルを作る時のおまじない
#define FWAPPSAMPLE_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWAppSimple.h>					// Frameworkクラスが宣言されているヘッダファイル

using namespace Spr;								// Springheadの名前空間
class FWAppSample;
class FWAppSample : public FWAppSimple{
public:	
	virtual void BuildObject();								// 物体を作成
	virtual void Keyboard(int key, int x, int y);		// キーボード関数
}; 

#endif
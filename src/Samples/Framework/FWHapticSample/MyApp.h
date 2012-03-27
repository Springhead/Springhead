#ifndef MYAPP_H
#define MYAPP_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class MyApp : public FWApp{
public:
	PHSceneIf* phscene;			// PHSceneへのポインタ
	PHHapticPointerIf* pointer; // 力覚ポインタへのポインタ
	int physicsTimerID;			// 物理スレッドのタイマ
	int hapticTimerID;			// 力覚スレッドのタイマ
	UTRef<HIBaseIf> spg;		// 力覚インタフェースへのポインタ

	void InitInterface();						// 力覚インタフェースの初期化
	void Init(int argc = 0, char* argv[] = 0);	// アプリケーションの初期化
	void Keyboard(int key, int x, int y);		// キーボード関数
	virtual void TimerFunc(int id);				// 各タイマが呼ぶコールバック関数
};


#endif
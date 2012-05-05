#ifndef FWHAPTIC_SAMPLE_H
#define FWHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWHapticSample : public FWApp{
public:
	enum HumanInterface{
		SPIDAR,
		XBOX,
		FALCON,
	} humanInterface;
	enum EngineType{
		SINGLE = 0,
		MULTI,
		LD,
	} engineType;
	PHSceneIf* phscene;			// PHSceneへのポインタ
	PHHapticPointerIf* pointer; // 力覚ポインタへのポインタ
	float pdt;
	float hdt;
	int physicsTimerID;			// 物理スレッドのタイマ
	int hapticTimerID;			// 力覚スレッドのタイマ
	UTRef<HIBaseIf> spg;		// 力覚インタフェースへのポインタ

	FWHapticSample();
	void InitInterface();						// 力覚インタフェースの初期化
	void Init(int argc = 0, char* argv[] = 0);	// アプリケーションの初期化
	virtual void BuildScene();							// オブジェクトの作成
	virtual void Keyboard(int key, int x, int y);		// キーボード関数
	void TimerFunc(int id);				// 各タイマが呼ぶコールバック関数
};


#endif
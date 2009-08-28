#ifndef FWLDHAPTIC_SAMPLE_H
#define FWLDHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWLDHapticSample : public FWApp{
public:
	FWLDHapticSample();									
	void Init(int argc, char* argv[]);			///< アプリケーションの初期化
	void InitCameraView();						///< カメラ行列の初期化
	void InitHumanInterface();					///< HapticInterfaceの初期化
	void Start();								///< アプリケーション開始
	void IdleFunc();							
	static void CallBackHapticLoop(void* arg);	///< hapticloopのコールバック
	void Display();								///< 描画関数	
	void BuildScene();							///< 剛体の作成									
	void Reset();								///< リセット(末実装)
	void Keyboard(int key, int x, int y);		///< キーボード関数

	/// オプション
	bool bDrawInfo;								///< デバック表示するかどうか									
	void DisplayContactPlane();					///< 接触面の描画
	void DisplayLineToNearestPoint();			///< ポインタの近傍点から剛体の近傍点への線
}; 

#endif
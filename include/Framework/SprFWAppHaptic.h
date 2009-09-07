#ifndef FWAPPHAPTIC_H
#define FWAPPHAPTIC_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class FWAppHaptic : public FWApp{
public:
	FWAppHaptic();									
	virtual void Init(int argc, char* argv[]);			///< アプリケーションの初期化
	virtual void InitCameraView();						///< カメラ行列の初期化
	virtual void InitHumanInterface();					///< HapticInterfaceの初期化
	virtual void Start();								///< アプリケーション開始
	virtual void IdleFunc();							
	virtual void Display();								///< 描画関数	
	virtual void BuildScene();							///< 剛体の作成
	virtual void BuildPointer();							///< 剛体の作成
	virtual void Reset(){};								///< リセット(末実装)
	virtual void Keyboard(int key, int x, int y);		///< キーボード関数

	static void CallBackHapticLoop(void* arg);			///< hapticloopのコールバック

	/// オプション
	bool bStep;									///< シミュレーションを進めるかどうか
	bool bOneStep;								///< ステップ実行
	bool bDrawInfo;								///< デバック表示するかどうか									
	void DisplayContactPlane();					///< 接触面の描画
	void DisplayLineToNearestPoint();			///< ポインタの近傍点から剛体の近傍点への線
}; 

#endif
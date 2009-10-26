#ifndef FWLDHAPTIC_SAMPLE_H
#define FWLDHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <vector>

using namespace Spr;

class FWLDHapticSample : public FWApp{
public:
	FWLDHapticSample();
	virtual void Init(int argc, char* argv[]);			///< アプリケーションの初期化
	virtual void InitCameraView();						///< カメラ行列の初期化
	virtual void InitHumanInterface();					///< HapticInterfaceの初期化
	virtual void Reset(){};
	virtual void IdleFunc();							///< Physicのコールバック
	virtual void TimerFunc(int id);						///< hapticloopのコールバック
	virtual void Display();								///< 描画関数	
	virtual void BuildScene(){};							///< 剛体の作成
	virtual void Keyboard(int key, int x, int y);		///< キーボード関数
	virtual void BuildPointer();						///< 剛体の作成	

	void TwoPointerCalib();								///< 2点のポインタの座標系を一致させる
	void TwoPointerSet();								///< 2点のポインタの座標系を一致させる
	std::vector<Vec3d> mainPosition;
	std::vector<Vec3d> subPosition;
	Posed tPos;

	/// オプション
	bool bStep;									///< シミュレーションを進めるかどうか
	bool bOneStep;								///< ステップ実行
	bool bDrawInfo;								///< デバック表示するかどうか									
	void DisplayContactPlane();					///< 接触面の描画
	void DisplayLineToNearestPoint();			///< ポインタの近傍点から剛体の近傍点への線
	PH3ElementBallJointIf* Balljoint;
}; 

#endif
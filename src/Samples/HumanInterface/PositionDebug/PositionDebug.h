#ifndef FWLDHAPTIC_SAMPLE_H
#define FWLDHAPTIC_SAMPLE_H

#include <Springhead.h>
#include <Framework/SprFWAppHaptic.h>
#include <vector>

using namespace Spr;

class FWLDHapticSample : public FWAppHaptic{
public:
	FWLDHapticSample();
	virtual void Init(int argc, char* argv[]);			///< アプリケーションの初期化
	virtual void InitCameraView();						///< カメラ行列の初期化
	virtual void InitHumanInterface();					///< HapticInterfaceの初期化
	virtual void IdleFunc();							
	virtual void BuildScene(){};						///< 剛体の作成	
	virtual void BuildPointer();						///< 剛体の作成	
	virtual void Display();								///< 描画関数	
	void Keyboard(int key, int x, int y);				///< キーボード関数

	void TwoPointerCalib();								///< 2点のポインタの座標系を一致させる
	void TwoPointerSet();								///< 2点のポインタの座標系を一致させる
	std::vector<Vec3d> mainPosition;
	std::vector<Vec3d> subPosition;


	PH3ElementBallJointIf* Balljoint;
}; 

#endif
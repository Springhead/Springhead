#ifndef HAPTIC_PROCESS_H
#define HAPTIC_PROCESS_H

#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIDRUsb.h>

#include "Synchronization.h"

using namespace Spr;
using namespace std;

class HapticProcess{
public:
	// SPIDAR,Deviceに使う変数
//	DVDeviceManager devMan;
	UTRef<HISpidarGIf>  spidarG6;
	// LocalDynamicに使う変数
	double dt;
	PHSolid hpointer; 
	vector<ExpandedObject> expandedObjects;
	//　力提示に使う変数
	bool bDisplayforce;	///< 力覚を提示するかどうか
	Vec3d displayforce;		///< ユーザへの提示力
	Vec3d displaytorque;
	double K;				///< バネ係数
	double D;				///< ダンパ係数
	float posScale;		///<スパイダーの移動スケール
	// 同期に使う変数
	int stepcount;

	HapticProcess();

	void Init();
	void InitDevice();
	void Step();
	void UpdateSpidar();
	void HapticRendering();
	void LocalDynamics();
	void Keyboard(unsigned char key);
};
extern HapticProcess hprocess;

#endif
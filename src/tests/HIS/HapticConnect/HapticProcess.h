#ifndef HAPTIC_PROCESS_H
#define HAPTIC_PROCESS_H

#include "Synchronization.h"
#include <Springhead.h>
#include <Physics/PHSolid.h>
#include <Physics/PHContactPoint.h>
#include <HIS/HISpidarG6X.h>
#include <HIS/HISpidarG6X3.h>
#ifdef _WIN32		//	Win32版(普通はこっち)
 #include <Device/DRUsb20Simple.h>
 #include <Device/DRUsb20Sh4.h>
 #include <conio.h>
#endif

using namespace Spr;
using namespace std;

class HapticProcess{
public:
	// SPIDAR,Deviceに使う変数
	HISpidarG6X3 spidarG6;
	DVDeviceManager devMan;
	// LocalDynamicに使う変数
	double dt;
	PHSolid hpointer; 
	vector<PHNeighborObject> neighborObjects;
	//　力提示に使う変数
	bool bDisplayforce;	///< 力覚を提示するかどうか
	Vec3d addforce;		///< ユーザへの提示力
	double K;				///< バネ係数
	double D;				///< ダンパ係数
	
	HapticProcess(){
		dt = 0.001f;
		K = 30;
		D = 0;
		bDisplayforce = false;
		hpointer.SetDynamical(false);
		hpointer.SetFrozen(true);
	};

	void Init();
	void InitDevice();
	void Step();
	void UpdateSpidar();
	void HapticRendering();
	void CalcForce(Vec3d dis);
	void LocalDynamics();
	void ClearForce();
	void GenerateForce();
	void Integrate();	
	double CalcDistance(Vec3d a2, Vec3d b);
	void Keyboard(unsigned char key);

};



#endif
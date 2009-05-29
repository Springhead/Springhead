#ifndef FW_MULTIRATE_HAPTIC_APP_H
#define FW_MULTIRATE_HAPTIC_APP_H

#include <Springhead.h>
#include <Framework/FWAppImp.h>
#include <Framework/FWHapticProcessBase.h>
//#include <Framework/FWExpandedPHSolid.h>

namespace Spr{;

class FWMultiRateHapticApp : public FWAppImp{
protected:
	FWHapticProcessBase* hapticProcess;
	double hdt;
	PHSolidIf* hapticPointer;

	volatile int hapticcount;
	volatile bool bSync;
	bool bCalcPhys;
public:
	double localRange;
	FWExpandedPHSolids expandedPHSolids;

	FWMultiRateHapticApp();
	void SetHapticProcess(FWHapticProcessBase* process);
	FWHapticProcessBase* GetHapticProcess();
	void SetPhysicTimeStep(double dt);
	double GetPhysicTimeStep();
	void SetHapticTimeStep(double dt);
	double GetHapticTimeStep();
	void SetHapticPointer(PHSolidIf* pointer);
	PHSolidIf* GetHapticPointer();
	int GetNExpandedPHSolids();
	FWExpandedPHSolid** GetFWExpandedPHSolids();

	virtual void Init();													
	virtual void ResetScene();
	virtual void Idle();
	FWExpandedPHSolid** ExpandPHSolidInfo();
	void FindNearestObjectFromHapticPointer(PHSolidIf* hPointer);
	void UpdateHapticPointer();
	void SyncHapticProcess();

//	void DisplayContactPlane();
//	void DisplayLineToNearestPoint();

};
}
#endif
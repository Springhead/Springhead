#ifndef FWOPHAPTICHANDLER_H
#define FWOPHAPTICHANDLER_H



//#include <Framework/FWObject.h>

#include <Framework/SprFWHapticPointer.h>
#include <Foundation\Object.h>
#include <HumanInterface\SprHISpidar.h>
#include <HumanInterface\SprHIDRUsb.h>
#include <HumanInterface\SprHIKeyMouse.h>
#include <HumanInterface\SprHISdk.h>
#include <Physics\PHOpHapticController.h>
#include <Framework/SprFWOpHapticHandler.h>

namespace Spr {
	;

	class FWOpHapticHandler : public SceneObject{
		SPR_OBJECTDEF(FWOpHapticHandler);

	public:
		FWOpHapticHandler()

		{
			hcReady = false;
			hapticTimeInterval = 0.001f;
		}

		//UTRef<HISpidarGIf> currSpg;
		HIBaseIf* humanInterface;
		SpatialVector hapticForce;
		//PHOpHapticControllerIf* opHcIf;
	//	PHOpEngineIf* opEngine;
		bool hcReady;
		float hapticTimeInterval;

		/*void initialize(PHOpEngineIf* myopEngine, PHOpHapticControllerIf* ophapticcontroller)
		{
			opHcIf = ophapticcontroller;
			opEngine = myopEngine;
		}*/

	//	bool SetHapticFunction(bool enable, HISdkIf* hisdk);
		void SetHapticflag(bool flag);
		bool IsHapticEnabled();
		void SetHapticTimeInterval(float dt);
		float GetHapticTimeInterval();
		//bool initDevice(HISdkIf* hiSdk);
		bool doCalibration(float dt);
		void UpdateHumanInterface(PHOpHapticController* opHC, float dt);

		void SetHumanInterface(HIBaseIf* hi){ humanInterface = hi; }
		HIBaseIf* GetHumanInterface(){ return humanInterface; }

		Vec3d GetHapticForce(){ return hapticForce.v(); }
		Vec3d GetHapticTorque(){ return hapticForce.w(); }
	};


}//namespace

#endif
#ifndef SPRFWOPHAPTICHANDLER_H
#define SPRFWOPHAPTICHANDLER_H


#include <Foundation/SprObject.h>
#include <HumanInterface\SprHISdk.h>
#include <Physics\SprPHOpObj.h>


namespace Spr{;

struct FWOpHapticHandlerIf :SceneObjectIf{
	SPR_IFDEF(FWOpHapticHandler);

	void SetHapticflag(bool flag);
	bool IsHapticEnabled();

	bool doCalibration(float dt);
	// void UpdateHumanInterface(PHOpHapticController* opHC, float dt);
	void SetHumanInterface(HIBaseIf* hi);
	HIBaseIf* GetHumanInterface();
	void SetHapticTimeInterval(float dt);
	float GetHapticTimeInterval();
};


}//namespace



#endif
#ifndef HAPTIC_HDSPIDAR8_H
#define HAPTIC_HDSPIDAR8_H

#include "HISpidar3.h"

namespace Spr{

class SPR_DLL HISpidar8{
public:
	HISpidar3 sp3[8];
	void Init(DVDeviceManager& devMan);
	static void RegisterDevice(DVDeviceManager& devMan);
	void Update(float dt);
};

}	//	namespace Haptic
#endif

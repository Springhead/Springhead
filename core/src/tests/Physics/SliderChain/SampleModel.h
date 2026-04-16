#ifndef SAMPLE_MODEL_H
#define SAMPLE_MODEL_H

#include <Springhead.h>

using namespace Spr;

PHSolidIf* CreateBox(FWSdkIf* fwSdk);
PHSolidIf* CreateSphere(FWSdkIf* fwSdk);
PHSolidIf* CreateCapsule(FWSdkIf* fwSdk);
PHSolidIf* CreateRoundCone(FWSdkIf* fwSdk);

#endif 
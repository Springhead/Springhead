#ifndef SAMPLE_MODEL_H
#define SAMPLE_MODEL_H

#include <Springhead.h>

using namespace Spr;

CDShapeIf* CreateShapeBox(FWSdkIf* fwSdk);
PHSolidIf* CreateBox(FWSdkIf* fwSdk);
PHSolidIf* CreateSphere(FWSdkIf* fwSdk);
PHSolidIf* CreateCapsule(FWSdkIf* fwSdk);
PHSolidIf* CreateRoundCone(FWSdkIf* fwSdk);
PHSolidIf* CreatePolyhedron(FWSdkIf* fwSdk);
PHSolidIf* CreateLumpBox(FWSdkIf* fwSdk);
PHJointIf* CreateJointBox(FWSdkIf* fwSdk);
void CreateWall(FWSdkIf* fwSdk);
void CreateTower(FWSdkIf* fwSdk);


#endif
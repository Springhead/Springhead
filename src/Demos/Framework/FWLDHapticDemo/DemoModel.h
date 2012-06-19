#ifndef SAMPLE_MODEL_H
#define SAMPLE_MODEL_H

#include <Springhead.h>

using namespace Spr;

const double modelScale = 2.0;
CDShapeIf* CreateShapeBox(PHSceneIf* phScene);
CDShapeIf* CreateShapeSphere(PHSceneIf* phScene);
PHSolidIf* CreateBox(PHSceneIf* phScene);
PHSolidIf* CreateSphere(PHSceneIf* phScene);
PHSolidIf* CreateCapsule(PHSceneIf* phScene);
PHSolidIf* CreateRoundCone(PHSceneIf* phScene);
PHSolidIf* CreatePolyhedron(PHSceneIf* phScene);
PHSolidIf* CreateLumpBox(PHSceneIf* phScene);
void CreateWall(PHSceneIf* phScene);
void CreateTower(PHSceneIf* phScene);
void CreateJointBox(PHSceneIf* phScene);
//void Create3ElementJointBox(PHSceneIf* phScene);

#endif
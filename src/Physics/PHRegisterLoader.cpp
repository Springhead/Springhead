#include <FileIO/FIDocScene.h>
#include "PHWaterContact.h"
#pragma hdrstop

namespace Spr{;

void RegisterLoaderForPhysics(FILoadScene* l){
	REGISTER_LOADER(l, PHSolid);
	REGISTER_LOADER(l, PHSolidContainer);
	REGISTER_LOADER(l, PHPenalty);
	REGISTER_LOADER(l, PHPenaltyEngine);
	REGISTER_LOADER(l, PHContactEngine);
	REGISTER_LOADER(l, PHContactInactive);
	REGISTER_LOADER(l, PHGravityEngine);
	REGISTER_LOADER(l, PHJoint);
	REGISTER_LOADER(l, PHJointHinge);
	REGISTER_LOADER(l, PHJointSlider);
	REGISTER_LOADER(l, PHJointUniversal);
	REGISTER_LOADER(l, PHJointBall);
	REGISTER_LOADER(l, PHConeLimit);
	REGISTER_LOADER(l, PHJointEngine);
	REGISTER_LOADER(l, PHJointPid);
	REGISTER_LOADER(l, PHJointBallPid);
	REGISTER_LOADER(l, PHJointControl);
	REGISTER_LOADER(l, PHJointControlEngine);
	REGISTER_LOADER(l, PHStick);
	REGISTER_LOADER(l, PHStickEngine);
	REGISTER_LOADER(l, PHWarp);
	REGISTER_LOADER(l, PHWarpEngine);
	REGISTER_LOADER(l, PHChangeObjectCollision);
	REGISTER_LOADER(l, PHChangeObjectOrientation);
	REGISTER_LOADER(l, PHChangeObjectContainer);
	REGISTER_LOADER(l, PHSpring);
	REGISTER_LOADER(l, PHMagneticEngine);
	REGISTER_LOADER(l, PHMagnet);
	REGISTER_LOADER(l, PHWaterEngine);
	REGISTER_LOADER(l, PHWater);
	REGISTER_LOADER(l, PHWaterContactEngine);
	REGISTER_LOADER(l, PHWaterTrackTarget);
#ifdef USE_FRM
	REGISTER_LOADER(l, PHWaterResistanceMap);
#endif
}
void RegisterSaverForPhysics(FISaveScene* l){
	REGISTER_SAVER(l, PHSolid);
	REGISTER_SAVER(l, PHSolidContainer);
	REGISTER_SAVER(l, PHPenalty);
	REGISTER_SAVER(l, PHPenaltyEngine);
	REGISTER_SAVER(l, PHContactEngine);
	REGISTER_SAVER(l, PHGravityEngine);
	REGISTER_SAVER(l, PHJointHinge);
	REGISTER_SAVER(l, PHJointSlider);
	REGISTER_SAVER(l, PHJointUniversal);
	REGISTER_SAVER(l, PHJointBall);
	REGISTER_SAVER(l, PHJointEngine);
	REGISTER_SAVER(l, PHJointPid);
	REGISTER_SAVER(l, PHJointBallPid);
	REGISTER_SAVER(l, PHJointControl);
	REGISTER_SAVER(l, PHJointControlEngine);
	REGISTER_SAVER(l, PHWarp);
	REGISTER_SAVER(l, PHWarpEngine);
	REGISTER_SAVER(l, PHChangeObjectCollision);
	REGISTER_SAVER(l, PHChangeObjectOrientation);
	REGISTER_SAVER(l, PHChangeObjectContainer);
	REGISTER_SAVER(l, PHSpring);
	REGISTER_SAVER(l, PHMagneticEngine);
	REGISTER_SAVER(l, PHMagnet);
	REGISTER_SAVER(l, PHWaterEngine);
	REGISTER_SAVER(l, PHWater);
	REGISTER_SAVER(l, PHWaterContactEngine);
	REGISTER_SAVER(l, PHWaterTrackTarget);
#ifdef USE_FRM
	REGISTER_SAVER(l, PHWaterResistanceMap);
#endif
}
}

void RegisterLoaderForPhysics(Spr::FILoadScene* l){
	Spr::RegisterLoaderForPhysics(l);
}
void SPR_DLL RegisterSaverForPhysics(Spr::FISaveScene* s){
	Spr::RegisterSaverForPhysics(s);
}

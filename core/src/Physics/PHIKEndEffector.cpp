/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHIKEndEffector.h>
#include <Physics/PHIKActuator.h>
#include <Physics/PHSolid.h>

using namespace std;
namespace Spr{;

//static std::ofstream *dlog;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKEndEffectorDesc

PHIKEndEffectorDesc::PHIKEndEffectorDesc() {
	bEnabled = true;

	bPosition		= true;
	bOrientation	= false;
	bForce			= false;
	bTorque			= false;

	oriCtlMode		= PHIKEndEffectorDesc::MODE_QUATERNION;

	positionPriority	= 1.0;
	orientationPriority	= 1.0;
	forcePriority		= 1.0;
	torquePriority		= 1.0;

	targetPosition			= Vec3d();
	targetVelocity			= Vec3d();
	targetLocalPosition		= Vec3d();
	targetLocalDirection	= Vec3d(0,0,1);
	targetOrientation		= Quaterniond();
	targetDirection			= Vec3d(0,0,1);
	targetLookat			= Vec3d(0,0,0);
	targetAngVel			= Vec3d();
	targetForce				= Vec3d();
	targetForceWorkingPoint	= Vec3d();
	targetTorque			= Vec3d();
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// IKEndEffector

bool PHIKEndEffector::AddChildObject(ObjectIf* o){
	PHSolidIf* so = o->Cast();
	if (so) { this->solid = so; return true; }
	return false;
}

ObjectIf* PHIKEndEffector::GetChildObject(size_t pos){
	if (pos == 0 && this->solid != NULL) { return this->solid; }
	return NULL;
}

size_t PHIKEndEffector::NChildObject() const {
	if (this->solid != NULL) { return 1; }
	return 0;
}

void PHIKEndEffector::Enable(bool enable){
	this->bEnabled = enable;
}

void PHIKEndEffector::GetTempTarget(PTM::VVector<double> &v){
	v.resize(ndof);

	PHSceneIf*    phScene = DCAST(PHSceneIf,GetScene());
	PHIKEngineIf* engine  = phScene->GetIKEngine();
	double dt = DCAST(PHSceneIf,GetScene())->GetTimeStep();

	if (bPosition) {
		Vec3d currPos = solidTempPose*targetLocalPosition;
		Vec3d dir     = (targetPosition - currPos);

		double maxmove = engine->GetMaxVelocity() * phScene->GetTimeStep();
		if (dir.norm() > maxmove) {
			dir = dir.unit() * maxmove;
		}
		dir *= positionPriority;

		for (int i=0; i<3; ++i) { v[i] = dir[i]; }
	}

	if (bOrientation) {
		Quaterniond qG;
		if (oriCtlMode == PHIKEndEffectorDesc::MODE_QUATERNION) {
			Quaterniond qS = solidTempPose.Ori();
			qG = (targetOrientation * qS.Inv());

		} else if (oriCtlMode == PHIKEndEffectorDesc::MODE_DIRECTION || oriCtlMode == PHIKEndEffectorDesc::MODE_LOOKAT)  {
			Vec3d dirS = solidTempPose.Ori() * targetLocalDirection;

			Vec3d dirG;
			if (oriCtlMode == PHIKEndEffectorDesc::MODE_DIRECTION) {
				dirG = targetDirection;
			} else {
				dirG = targetLookat - (solidTempPose * targetLocalPosition);
			}
			if (dirG.norm()>1e-5) { dirG.unitize(); }

			Vec3d rot  = dirS % dirG;

			if (rot.norm() < 1e-5) {
				qG = Quaterniond();
			} else {
				qG = Quaterniond::Rot(rot.norm(), rot.unit());
			}
		}

		double maxmove = engine->GetMaxAngularVelocity() * phScene->GetTimeStep();
		Vec3d v_o;
		if (qG.Theta() < maxmove) {
			v_o = (qG.Axis() * qG.Theta());
		} else {
			v_o = (qG.Axis() * maxmove);
		}
		v_o *= orientationPriority;

		int stride = (bPosition ? 3 : 0);
		for (int i=0; i<3; ++i) { v[i+stride] = v_o[i]; }
	}
}

void PHIKEndEffector::GetTempVelocity(PTM::VVector<double> &v){
	v.resize(ndof);

	if (bPosition) {
		Vec3d dir = GetTargetVelocity();
		for (int i=0; i<3; ++i) { v[i] = dir[i]; }
	}

	if (bOrientation) {
		Vec3d v_o = GetTargetAngularVelocity();
		int stride = (bPosition ? 3 : 0);
		for (int i=0; i<3; ++i) { v[i+stride] = v_o[i]; }
	}
}
}


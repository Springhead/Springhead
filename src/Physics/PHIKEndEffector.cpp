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

	positionPriority	= 1.0;
	orientationPriority	= 1.0;
	forcePriority		= 1.0;
	torquePriority		= 1.0;

	targetPosition			= Vec3d();
	targetLocalPosition		= Vec3d();
	targetOrientation		= Quaterniond();
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

	PHIKEngineIf* engine = DCAST(PHSceneIf,GetScene())->GetIKEngine();
	double dt = DCAST(PHSceneIf,GetScene())->GetTimeStep();

	if (bPosition) {
		Vec3d currPos = solidTempPose*targetLocalPosition;
		Vec3d dir     = (targetPosition - currPos);

		double maxmove = engine->GetLinearDistance();
		if (dir.norm() > maxmove) {
			dir = dir.unit() * maxmove;
		}

		for (int i=0; i<3; ++i) { v[i] = dir[i]; }

		/*
		Vec3d solidPos = solid->GetPose()*targetLocalPosition;
		Vec3d dir = (targetPosition - solidPos) * positionPriority;
		double epsilon = 1.0 * positionPriority;

		Vec3d v_p;
		if (dir.norm() > epsilon) {
			dir.unitize();
			dir *= epsilon;
		}
		v_p = dir;

		for (int i=0; i<3; ++i) { v[i] = v_p[i]; }
		*/
	}

	if (bOrientation) {
		Quaterniond qS = solidTempPose.Ori();
		Quaterniond qG = (targetOrientation * qS.Inv());
		double maxmove = Rad(10.0);

		Vec3d v_o;
		if (qG.Theta() < maxmove) {
			v_o = (qG.Axis() * qG.Theta());
		} else {
			v_o = (qG.Axis() * maxmove);
		}

		int stride = (bPosition ? 3 : 0);
		for (int i=0; i<3; ++i) { v[i+stride] = v_o[i]; }
	}
}

}


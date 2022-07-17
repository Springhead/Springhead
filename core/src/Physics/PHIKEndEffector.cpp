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
	DSTR <<"***name:"<< this->GetName() <<"baseSolid:"<<baseSolid<< std::endl;

	v.resize(ndof);

	PHSceneIf*    phScene = DCAST(PHSceneIf,GetScene());
	PHIKEngineIf* engine  = phScene->GetIKEngine();
	//double dt = DCAST(PHSceneIf,GetScene())->GetTimeStep();

	if (bPosition) {
		Vec3d currPos = solidTempPose*targetLocalPosition;
		//Vec3d dir     = (targetPosition - currPos);//HERE!! 
		
		Vec3d dir;
		if (baseSolid != NULL) {
			//dir = (targetPosition - baseSolid->GetPose().Inv() * currPos);
			dir = (baseSolid->GetPose()*targetPosition) - currPos;
			
			DSTR<<this->GetName()<<",dir&&&:"<< dir<<std::endl;
			DSTR << this->GetName() <<",targetPosition:"<< targetPosition << std::endl;
			DSTR << this->GetName() << ",baseSolid->GetPose()*targetPosition:" << baseSolid->GetPose()*targetPosition << std::endl;
			DSTR << this->GetName() << ",currPose:" << currPos << std::endl;
			DSTR << this->GetName() << "0717bodyPose:" << baseSolid->GetPose() << std::endl;
		}
		else
		{
			dir = (targetPosition - currPos);
		}
		
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
			if (baseSolid != NULL) {
				qS = qS*baseSolid->GetOrientation().Inv();//To relative orientation
			}

			qG = (targetOrientation * qS.Inv());

			DSTR << this->GetName() << "0717,qG:" << qS.ToEuler() << std::endl;

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
		Vec3d v_o = qG.RotationHalf();
		if (v_o.norm() > maxmove) {
			v_o = v_o.unit() * maxmove;
		}
		v_o *= orientationPriority;

		int stride = (bPosition ? 3 : 0);
		for (int i=0; i<3; ++i) { v[i+stride] = v_o[i]; }
	}
}

void PHIKEndEffector::GetTempTarget(PTM::VVector<double> &v, PTM::VVector<double> &w) {
	GetTempTarget(v);
	if (bPosition) {
		for (int i = 0; i < 3; i++) { w[i] = positionPriority; v[i] += lagMulP[i]; lagMulP[i] = v[i]; }
	}/*
	int stride = (bPosition ? 3 : 0);
	if (bOrientation) {
		// <!!> クオータニオン経由でラグランジェ乗数を各軸ベクトルに変換しているが、多分経由しちゃダメ
		Vec3d l = Vec3d(v[0 + stride], v[1 + stride], v[2 + stride]);
		Quaterniond q = Quaterniond::Rot(l);
		Vec3d le = q.ToEuler();
		lagMulR += le;
		q.FromEuler(lagMulR);
		Vec3d r = q.RotationHalf();
		for (int i = 0; i < 3; i++) { w[i + stride] = orientationPriority; v[i + stride] = r[i]; }
	}*/
}

void PHIKEndEffector::UpdateLagrangeMultiplier(PTM::VVector<double> &l) {
	if (bPosition) {
		for (int i = 0; i < 3; i++) { lagMulP[i] += l[i]; }
	}
	int stride = (bPosition ? 3 : 0);
	if (bOrientation) {
		// <!!> クオータニオン経由でラグランジェ乗数を各軸ベクトルに変換しているが、多分経由しちゃダメ
		Vec3d v = Vec3d(l[0 + stride], l[1 + stride], l[2 + stride]);
		Quaterniond q = Quaterniond::Rot(v);
		Vec3d r = q.ToEuler();
		for (int i = 0; i < 3; i++) { lagMulR[i] += r[i]; }
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
void PHIKEndEffector::SetBaseSolid(PHSolidIf* bsolid){
	baseSolid = bsolid;
	DSTR << baseSolid << std::endl;
}

}


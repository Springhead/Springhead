/*
*  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef CRMINJERKTRAJECTORY_H
#define CRMINJERKTRAJECTORY_H

#include <Foundation/SprObject.h>
#include <Base/Spatial.h>

namespace Spr { ;

template<class TP, class TV>
class CRMinimumJerkTrajectory {
public:
	float startTime;
	float goalTime;
	
	virtual void SetStartPosition(TP sp) {}
	virtual void SetStartVelocity(TV sv) {}
	virtual void SetStartAcceralation(TV sa) {}
	virtual void SetStartTime(float st) {}

	virtual void SetGoalPosition(TP gp) {}
	virtual void SetGoalVelocity(TV gv) {}
	virtual void SetGoalAcceralation(TV ga) {}
	virtual void SetGoalTime(float gt) {}

	virtual TP GetPosition(float t) { return TP(); }
	virtual TV GetVelocity(float t) { return TV(); }
	virtual TP GetDeltaPosition(float t) { return TP(); }
};

template<class T>
class CRNDMinimumJerkTrajectory : public CRMinimumJerkTrajectory<T, T>{
protected:
public:
	T startPosition; 
	T startVelocity;
	T startAcceralation;
	T goalPosition;
	T goalVelocity;
	T goalAcceralation;

	PTM::TVector<6, T> coefficient;

	// VecNdÇ≈Ç†ÇÍÇŒåWêîåàÇﬂë≈ÇøÇ≈çsÇØÇÈÇÃÇ≈GetÇ∑ÇÈÇ∆Ç´ÇÃåvéZÇ≈çsÇØÇÈÇÕÇ∏
	CRNDMinimumJerkTrajectory() {};

	virtual void SetStartPosition(T sp) { this->startPosition = sp; }
	virtual void SetStartVelocity(T sv) { this->startVelocity = sv; }
	virtual void SetStartAcceralation(T sa) { this->startAcceralation = sa; }
	virtual void SetStartTime(float st) { this->startTime = st; }

	virtual void SetGoalPosition(T gp) { this->goalPosition = gp; }
	virtual void SetGoalVelocity(T gv) { this->goalVelocity = gv; }
	virtual void SetGoalAcceralation(T ga) { this->goalAcceralation = ga; }
	virtual void SetGoalTime(float gt) { this->goalTime = gt; }

	virtual T GetPosition(float t);
	virtual T GetVelocity(float t);
	virtual T GetDeltaPosition(float t);
};

typedef CRNDMinimumJerkTrajectory<double> CRAngleMinimumJerkTrajectory;

typedef CRNDMinimumJerkTrajectory<Vec3d> CRPositionMinimumJerkTrajectory;
typedef CRNDMinimumJerkTrajectory<Vec3d> CREulerAngleMinimumJerkTrajectory;

class CRQuaternionMinimumJerkTrajectory : public CRMinimumJerkTrajectory<Quaterniond, Vec3d>{
protected:
	// Quaternionê¨ï™ópÇÃåWêî
	CRNDMinimumJerkTrajectory<double> positionCoefficient;
	// Ç«Ç§Ç¢Ç§àµÇ¢Ç…Ç∑ÇÈÇ©çlÇ¶íÜ
	CREulerAngleMinimumJerkTrajectory velocityTrajectory;
public:
	Quaterniond startPosition, goalPosition;

	virtual void SetStartPosition(Quaterniond sp) { this->startPosition = sp; }
	virtual void SetStartVelocity(Vec3d sv) { this->velocityTrajectory.SetStartVelocity(sv); }
	virtual void SetStartAcceralation(Vec3d sa) { this->velocityTrajectory.SetStartAcceralation(sa); }
	virtual void SetStartTime(float st) { this->startTime; this->velocityTrajectory.SetStartTime(st); }

	virtual void SetGoalPosition(Quaterniond gp) { this->goalPosition = gp; }
	virtual void SetGoalVelocity(Vec3d gv) { this->velocityTrajectory.SetGoalVelocity(gv); }
	virtual void SetGoalAcceralation(Vec3d ga) { this->velocityTrajectory.SetGoalAcceralation(ga); }
	virtual void SetGoalTime(float gt) { this->goalTime = gt; this->velocityTrajectory.SetGoalTime(gt); }

	virtual Quaterniond GetPosition(float t);
	virtual Vec3d GetVelocity(float t);
	virtual Quaterniond GetDeltaPosition(float t);
};

class CRPoseMinimumJerkTrajectory : public CRMinimumJerkTrajectory<Posed, SpatialVector>{
protected:
	CRPositionMinimumJerkTrajectory positionTrajectory;
	CRQuaternionMinimumJerkTrajectory orientationTrajectory;
public:
	virtual void SetStartPosition(Posed sp) { this->positionTrajectory.SetStartPosition(sp.Pos()); this->orientationTrajectory.SetStartPosition(sp.Ori()); }
	virtual void SetStartVelocity(SpatialVector sv) { this->positionTrajectory.SetStartVelocity(sv.v()); this->orientationTrajectory.SetGoalVelocity(sv.w()); }
	virtual void SetStartAcceralation(SpatialVector sa) { this->positionTrajectory.SetStartAcceralation(sa.v()); this->orientationTrajectory.SetStartAcceralation(sa.w()); }
	virtual void SetStartTime(float st) { this->positionTrajectory.SetStartTime(st); this->orientationTrajectory.SetGoalTime(st); }

	virtual void SetGoalPosition(Posed gp) { this->positionTrajectory.SetGoalPosition(gp.Pos()); this->orientationTrajectory.SetGoalPosition(gp.Ori()); }
	virtual void SetGoalVelocity(SpatialVector gv) { this->positionTrajectory.SetGoalVelocity(gv.v()); this->orientationTrajectory.SetGoalVelocity(gv.w()); }
	virtual void SetGoalAcceralation(SpatialVector ga) { this->positionTrajectory.SetStartAcceralation(ga.v()); this->orientationTrajectory.SetGoalAcceralation(ga.w()); }
	virtual void SetGoalTime(float gt) { this->positionTrajectory.SetGoalTime(gt); this->orientationTrajectory.SetGoalTime(gt); }

	virtual Posed GetPosition(float t);
	virtual SpatialVector GetVelocity(float t);
	virtual Posed GetDeltaPosition(float t);
};

}

#endif
/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

#include <Creature/CRMinimumJerkTrajectory.h>

namespace Spr { ; 
template<class T>
T CRNDMinimumJerkTrajectory<T>::GetPosition(float t){
	if (t < startTime) {
		return startPosition;
	}
	else if (goalTime < t) {
		return goalPosition;
	}
	double r = t - startTime;
	double s = (t - startTime) / (goalTime - startTime);

	double duration = goalTime - startTime;

	coefficient[0] = startPosition;
	coefficient[1] = startVelocity;
	coefficient[2] = startAcceralation / 2;
	coefficient[3] = ((-3 * startAcceralation + goalAcceralation) * pow(duration, 2) / 2
		              - (6 * startVelocity + 4 * goalVelocity) * duration
		              - 10 * (startPosition - goalPosition)) / pow(duration, 3);
	coefficient[4] = ((3 * startAcceralation - 2 * goalAcceralation) * pow(duration, 2) / 2
		              + (8 * startVelocity + 7 * goalVelocity) * duration
		              + 15 * (startPosition - goalPosition)) / pow(duration, 4);
	coefficient[5] = ((-startAcceralation + goalAcceralation) * pow(duration, 2) / 2
		              - 3 * (startVelocity + goalVelocity) * duration
		              - 6 * (startPosition - goalPosition)) / pow(duration, 5);

	return coefficient * Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5));
}

template<class T>
T CRNDMinimumJerkTrajectory<T>::GetVelocity(float t) {
	return T();
}

template<class T>
T CRNDMinimumJerkTrajectory<T>::GetDeltaPosition(float t) {
	return T();
}

Quaterniond CRQuaternionMinimumJerkTrajectory::GetPosition(float t) {
	if (t < startTime) {
		return startPosition;
	}
	else if (goalTime < t) {
		return goalPosition;
	}
	double s = (double)(t - startTime) / (goalTime - startTime);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	Quaterniond pos = interpolate(sr, startPosition, goalPosition);

	Vec3d compVel = velocityTrajectory.GetPosition(t);
	Quaterniond compQuaVel;
	compQuaVel.FromEuler(compVel);

	return pos * compQuaVel;
}

Vec3d CRQuaternionMinimumJerkTrajectory::GetVelocity(float t) {
	return Vec3d();
}

Quaterniond CRQuaternionMinimumJerkTrajectory::GetDeltaPosition(float t) {
	return this->GetPosition(t) * startPosition.Inv();
}

Posed CRPoseMinimumJerkTrajectory::GetPosition(float t) {
	return Posed(positionTrajectory.GetPosition(t), orientationTrajectory.GetPosition(t));
}

SpatialVector CRPoseMinimumJerkTrajectory::GetVelocity(float t) {
	return SpatialVector();
}

Posed CRPoseMinimumJerkTrajectory::GetDeltaPosition(float t) {
	return Posed(positionTrajectory.GetDeltaPosition(t), orientationTrajectory.GetDeltaPosition(t));
}

// explicit instantiation
template class CRMinimumJerkTrajectory<double, double>;
template class CRMinimumJerkTrajectory<Vec3d, Vec3d>;
template class CRMinimumJerkTrajectory<Posed, SpatialVector>;
template class CRMinimumJerkTrajectory<Quaterniond, Vec3d>;

template class CRNDMinimumJerkTrajectory<double>;
template class CRNDMinimumJerkTrajectory<Vec3d>;

}
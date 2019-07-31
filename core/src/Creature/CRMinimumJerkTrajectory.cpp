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
void CRNDMinimumJerkTrajectory<T>::CompCoefficient() {
	double duration = this->goalTime - this->startTime;

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
}
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

	Vec6d val = Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5));
	T pos = T();
	for (int i = 0; i < 6; i++) {
		pos += coefficient[i] * val[i];
	}
	return pos;
}

template<class T>
T CRNDMinimumJerkTrajectory<T>::GetVelocity(float t) {
	if (t < startTime) {
		return startPosition;
	}
	else if (goalTime < t) {
		return goalPosition;
	}
	double r = t - startTime;
	double s = (t - startTime) / (goalTime - startTime);

	Vec6d val = Vec6d(0, 1, 2 * r, 3 * pow(r, 2), 4 * pow(r, 3), 5 * pow(r, 4));
	T vel = T();
	for (int i = 0; i < 6; i++) {
		vel += coefficient[i] * val[i];
	}
	return vel;
}

template<class T>
T CRNDMinimumJerkTrajectory<T>::GetDeltaPosition(float t) {
	return this->GetPosition(t) - startPosition;
}

Quaterniond CRQuaternionMinimumJerkTrajectory::GetPosition(float t) {
	if (t < startTime) {
		return startPosition;
	}
	else if (goalTime < t) {
		return goalPosition;
	}
	double s = (double)(t - startTime) / (goalTime - startTime);
	double sr = positionCoefficient.GetPosition(s);
	DSTR << sr << s << std::endl;
	Quaterniond pos = interpolate(sr, startPosition, goalPosition);
	
	Vec3d compVel = velocityTrajectory.GetPosition(t);
	Quaterniond compQuaVel;
	compQuaVel.FromEuler(compVel);
	
	return pos * compQuaVel;
}

Vec3d CRQuaternionMinimumJerkTrajectory::GetVelocity(float t) {
	Quaterniond delta = GetPosition(t + 0.01) * GetPosition(t - 0.01).Inv();
	Vec3d vel = Vec3d();
	delta.ToEuler(vel);
	return Vec3d(vel.z, vel.x, vel.y) * 50;
}

Quaterniond CRQuaternionMinimumJerkTrajectory::GetDeltaPosition(float t) {
	return this->GetPosition(t) * startPosition.Inv();
}

Posed CRPoseMinimumJerkTrajectory::GetPosition(float t) {
	return Posed(positionTrajectory.GetPosition(t), orientationTrajectory.GetPosition(t));
}

SpatialVector CRPoseMinimumJerkTrajectory::GetVelocity(float t) {
	return SpatialVector(positionTrajectory.GetVelocity(t), orientationTrajectory.GetVelocity(t));
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

void CRPoseViaPointsMinimumJerkTrajectory::Recalc() {
	/*
	sPoint = spoint;
	fPoint = fpoint;
	vPoint = vpoint;

	//開始と終点を０としたときの経由点の相対位置相対位置
	
	double internalRatio = (double)(vtime - stime) / (ftime - stime);
	Vec3d relvpos = vpoint.pose.Pos() - ((1 - internalRatio) * spoint.pose.Pos() + internalRatio * fpoint.pose.Pos());
	double tvs = vtime - stime;
	double tfv = ftime - vtime;
	Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * vpoint.pose.Pos();
	Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * vpoint.pose.Pos();
	
	//Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * relvpos;
	//Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * relvpos;

	double stime = sPoint.time;
	double ftime = fPoint.time;
	double vtime = vPoint.time;
	if (vtime > stime && vtime < ftime) {
		for (int i = 0; i < 3; ++i) {
			PTM::TMatrixRow<6, 6, double> A;
			PTM::TMatrixRow<6, 1, double> b, x;
			A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
			A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sPoint.vel[i];
			A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = sPoint.acc[i];
			A.row(3) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[3][0] = vPoint.pose.Pos()[i];
			A.row(4) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[4][0] = vPoint.vel[i];
			A.row(5) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[5][0] = vPoint.acc[i];
			x = A.inv() * b;

			for (int n = 0; n < 6; ++n) { coeffToV[n][i] = x[n][0]; }
		}
		for (int i = 0; i < 3; ++i) {
			PTM::TMatrixRow<6, 6, double> A;
			PTM::TMatrixRow<6, 1, double> b, x;
			A.row(0) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[0][0] = vPoint.pose.Pos()[i];
			A.row(1) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[1][0] = vPoint.vel[i];
			A.row(2) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[2][0] = vPoint.acc[i];
			A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
			A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fPoint.vel[i];
			A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = fPoint.acc[i];
			x = A.inv() * b;

			for (int n = 0; n < 6; ++n) { coeffToF[n][i] = x[n][0]; }
		}
	}
	else {
		for (int i = 0; i < 3; ++i) {
			PTM::TMatrixRow<6, 6, double> A;
			PTM::TMatrixRow<6, 1, double> b, x;
			A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
			A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = 0;
			A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
			A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
			A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = 0;
			A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
			x = A.inv() * b;

			for (int n = 0; n < 6; ++n) { coeffToF[n][i] = x[n][0]; }
		}
	}
	*/
}

}
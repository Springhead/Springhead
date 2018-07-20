#include <Framework/FWTrajectoryPlanner.h>
#define DEBUG_INFORMATION 1
#define INITAL_INTERPOLATION 0
#define USE_MINJERK_TARGET_POS_VEL 0

namespace Spr {
	;
// 散らかってるのは後で整理します

//----- ----- ----- -----

MinJerkTrajectory::MinJerkTrajectory() {
	sPoint = ControlPoint(); fPoint = ControlPoint();
}

MinJerkTrajectory::MinJerkTrajectory(ControlPoint spoint, ControlPoint fpoint) {
	sPoint = spoint;
	fPoint = fpoint;
	vPoint = ControlPoint();

	double stime = sPoint.time;
	double ftime = fPoint.time;
	for (int i = 0; i < 3; ++i) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sPoint.vel[i];
		A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = sPoint.acc[i];
		A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fPoint.vel[i];
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = fPoint.acc[i];
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToF[n][i] = x[n][0]; }
	}
}

MinJerkTrajectory::MinJerkTrajectory(ControlPoint spoint, ControlPoint fpoint, ControlPoint vpoint) {
	sPoint = spoint;
	fPoint = fpoint;
	vPoint = vpoint;
	
	//開始と終点を０としたときの経由点の相対位置相対位置
	/*
	double internalRatio = (double)(vtime - stime) / (ftime - stime);
	Vec3d relvpos = vpoint.pose.Pos() - ((1 - internalRatio) * spoint.pose.Pos() + internalRatio * fpoint.pose.Pos());
	double tvs = vtime - stime;
	double tfv = ftime - vtime;
	Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * vpoint.pose.Pos();
	Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * vpoint.pose.Pos();
	*/
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
}

Posed MinJerkTrajectory::GetCurrentPose(double t) {
	if (t <= sPoint.time) return sPoint.pose;
	if (fPoint.time <= t) return fPoint.pose;

	double st = sPoint.time, ft = fPoint.time;
	double vtime = vPoint.time;
	/*
	if (vtime > 0) {
		int st = (vtime > t) ? stime : vtime;
		int ft = (vtime > t) ? vtime : ftime;
	}
	*/
	double s = (double)(t - st) / (ft - st);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	double r = t;// -stime;
	//DSTR << s << " " << r << std::endl;

	Posed curPose;
	//curPose.Pos() = sPose.Pos() + (fPose.Pos() - sPose.Pos()) * r;
	for (int i = 0; i < 3; i++) {
		curPose.Pos()[i] = (r < vtime ? coeffToV.col(i) : coeffToF.col(i)) * Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5));
	}
	curPose.Ori() = interpolate(sr, sPoint.pose.Ori(), fPoint.pose.Ori());
	return  curPose;
}

Posed MinJerkTrajectory::GetDeltaPose(double t) {
	if (t <= sPoint.time) return Posed();
	if (fPoint.time <= t) return fPoint.pose * sPoint.pose.Inv();

	double st = sPoint.time, ft = fPoint.time;
	double vtime = vPoint.time;

	if (vtime > 0) {
		st = (vtime > t) ? sPoint.time : vtime;
		ft = (vtime > t) ? vtime : fPoint.time;
	}
	
	double s = (double)(t - st) / (ft - st);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	int r = t;// -stime;

	Posed deltaPose;
	for (int i = 0; i < 3; i++) {
		deltaPose.Pos()[i] = (r < vtime ? coeffToV.col(i) : coeffToF.col(i)) * Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5)) - sPoint.pose.Pos()[i];
	}
	//deltaPose.Pos() = (fPose.Pos() - sPose.Pos()) * r;
	deltaPose.Ori() = interpolate(sr, Quaterniond(), fPoint.pose.Ori() * sPoint.pose.Ori().Inv());
	return deltaPose;
}

Vec6d MinJerkTrajectory::GetCurrentVelocity(double t) {
	Posed curr = GetCurrentPose(t);
	Posed pre = GetCurrentPose(t);
	Vec3d v =  curr.Pos() - pre.Pos();
	return Vec6d(v.x, v.y, v.z, 0, 0, 0);
}

double MinJerkTrajectory::GetCurrentActiveness(double t) {
	double s = (double)(fPoint.time - t) / (fPoint.time - sPoint.time);
	double r = 30 * pow(s, 2) - 60 * pow(s, 3) + 30 * pow(s, 4);
	return r;
}

QuaMinJerkTrajectory::QuaMinJerkTrajectory() {

}

QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, double time) {
	sQua = squa; fQua = fqua;
	stime = 0; ftime = time;
	//sVel *= per;
	//fVel *= per;
	Vec3d sEuler, fEuler;
	sQua.ToEuler(sEuler);
	fQua.ToEuler(fEuler);
	Vec3d sv = Vec3d(sVel.y, sVel.z, sVel.x);
	Vec3d fv = Vec3d(fVel.y, fVel.z, fVel.x);
	amjt = new AngleMinJerkTrajectory[3];
	for (int i = 0; i < 3; i++) {
		//amjt[i] = AngleMinJerkTrajectory(sEuler[i], fEuler[i], sv[i], fv[i], 0, 0, ftime);
		amjt[i] = AngleMinJerkTrajectory(0, 0, sv[i], fv[i], 0, 0, ftime);
	}
	velToQua.resize(ftime + 1);
	for (int i = 1; i <= ftime; i++) {
		Vec3d curvel = Vec3d();
		for (int j = 0; j < 3; j++) {
			curvel[j] = amjt[j].GetCurrentAngle(i) - amjt[j].GetCurrentAngle(i - 1);
		}
		velToQua[i].w = (-curvel[0] * velToQua[i - 1].x - curvel[1] * velToQua[i - 1].y - curvel[2] * velToQua[i - 1].z) / 2;
		velToQua[i].x = (curvel[2] * velToQua[i - 1].y - curvel[1] * velToQua[i - 1].z + curvel[0] * velToQua[i - 1].w) / 2;
		velToQua[i].y = (-curvel[2] * velToQua[i - 1].x + curvel[0] * velToQua[i - 1].z + curvel[1] * velToQua[i - 1].w) / 2;
		velToQua[i].z = (curvel[1] * velToQua[i - 1].x - curvel[0] * velToQua[i - 1].y + curvel[2] * velToQua[i - 1].w) / 2;
		//DSTR << velToQua << std::endl;
	}
}

QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, Vec3d sAcc, Vec3d fAcc, double time) {
	sQua = squa; fQua = fqua;
	stime = 0; ftime = time;
	//sVel *= per;
	//fVel *= per;
	//sAcc *= per * per;
	//fAcc *= per * per;
	Vec3d sEuler, fEuler;
	sQua.ToEuler(sEuler);
	fQua.ToEuler(fEuler);
	Vec3d sv = Vec3d(sVel.y, sVel.z, sVel.x);
	Vec3d fv = Vec3d(fVel.y, fVel.z, fVel.x);
	Vec3d sa = Vec3d(sAcc.y, sAcc.z, sAcc.x);
	Vec3d fa = Vec3d(fAcc.y, fAcc.z, fAcc.x);
	amjt = new AngleMinJerkTrajectory[3];
	for (int i = 0; i < 3; i++) {
		//amjt[i] = AngleMinJerkTrajectory(sEuler[i], fEuler[i], sv[i], fv[i], sa[i], fa[i], ftime);
		amjt[i] = AngleMinJerkTrajectory(0, 0, sv[i], fv[i], sa[i], fa[i], ftime);
	}
	velToQua.resize(ftime + 1);
	for (int i = 1; i <= ftime; i++) {
		Vec3d curvel = Vec3d();
		for (int j = 0; j < 3; j++) {
			curvel[j] = amjt[j].GetCurrentVelocity(i - 1);
		}
		Quaterniond delta = Quaterniond();
		//DSTR << curvel << std::endl;
		delta.w = (-curvel[0] * velToQua[i - 1].x - curvel[1] * velToQua[i - 1].y - curvel[2] * velToQua[i - 1].z) / 2;
		delta.x = (curvel[2] * velToQua[i - 1].y - curvel[1] * velToQua[i - 1].z + curvel[0] * velToQua[i - 1].w) / 2;
		delta.y = (-curvel[2] * velToQua[i - 1].x + curvel[0] * velToQua[i - 1].z + curvel[1] * velToQua[i - 1].w) / 2;
		delta.z = (curvel[1] * velToQua[i - 1].x - curvel[0] * velToQua[i - 1].y + curvel[2] * velToQua[i - 1].w) / 2;
		velToQua[i] = velToQua[i - 1] * delta;
		//DSTR << velToQua << std::endl;
	}
}

QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond vqua, double time, double vtime) {
	sQua = Quaterniond(); fQua = Quaterniond();
	stime = 0; ftime = time;
	vQua = vqua; this->vtime = vtime;
	//sVel *= per;
	//fVel *= per;
	//sAcc *= per * per;
	//fAcc *= per * per;
	Vec3d vEuler;
	vQua.ToEuler(vEuler);
	amjt = new AngleMinJerkTrajectory[3];
	for (int i = 0; i < 3; i++) {
		amjt[i] = AngleMinJerkTrajectory(vEuler[i], ftime, vtime);
	}
}

QuaMinJerkTrajectory::~QuaMinJerkTrajectory() {
	delete amjt;
}

Quaterniond QuaMinJerkTrajectory::GetCurrentQuaternion(double t) {
	if (t <= stime) {
		return sQua;
	}if (t >= ftime) {
		return fQua;
	}
	/*
	double s = (double)(t - stime) / (ftime - stime);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	double d = dot(sQua, fQua);
	/*/
	double s = (double)(t - stime) / (ftime - stime);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	Quaterniond inter = interpolate(sr, sQua, fQua);
	Vec3d current = Vec3d();
	for (int i = 0; i < 3; i++) {
		current[i] = amjt[i].GetCurrentAngle(t);
	}
	Quaterniond cq;
	cq.FromEuler(current);
	//DSTR << "Comp (qua):" << inter << " (euler):" << cq << std::endl;
	return interpolate(sr, sQua, fQua) * cq;
}

Quaterniond QuaMinJerkTrajectory::GetDeltaQuaternion(double t) {
	if (t <= stime) {
		return Quaterniond();
	}if (t > ftime) {
		return fQua * sQua.Inv();
	}
	/*
	double s = (double)(t - stime) / (ftime - stime);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	double d = dot(Quaterniond(), fQua * sQua.Inv());
	if (d > 0) {
	return interpolate(sr, Quaterniond(), fQua * sQua.Inv());
	}
	else {
	return interpolate(sr, Quaterniond(), -fQua * sQua.Inv());
	}
	/*/
	double s = (double)(t - stime) / (ftime - stime);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	Quaterniond inter = interpolate(sr, Quaterniond(), fQua * sQua.Inv());
	Vec3d current = Vec3d();
	for (int i = 0; i < 3; i++) {
		current[i] = amjt[i].GetDeltaAngle(t);
	}
	Quaterniond cq;
	cq.FromEuler(current);
	//DSTR << "Comp (qua):" << inter << " (euler):" << cq << std::endl;
	return interpolate(sr, Quaterniond(), fQua * sQua.Inv()) * cq;
}

Vec3d QuaMinJerkTrajectory::GetCurrentVelocity(double t) {
	Quaterniond delta = GetCurrentQuaternion(t + 0.01) * GetCurrentQuaternion(t - 0.01).Inv();
	Vec3d vel = Vec3d();
	delta.ToEuler(vel);
	return Vec3d(vel.z, vel.x, vel.y) * 50;
}

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double sangle, double fangle, double sVel, double fVel, double sAcc, double fAcc, double time) {
	sAngle = sangle; fAngle = fangle;
	stime = 0; ftime = time;
	vAngle = sangle; vtime = 0;
	
	PTM::TMatrixRow<6, 6, double> A;
	PTM::TMatrixRow<6, 1, double> b, x;
	A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sAngle;
	A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel;
	A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = sAcc;
	A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fAngle;
	A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel;
	A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = fAcc;
	x = A.inv() * b;

	for (int n = 0; n < 6; ++n) { coeffToF[n] = x[n][0]; }
	/*/
	coeff[0] = sAngle;
	coeff[1] = sVel;
	coeff[2] = sAcc / 2;
	coeff[3] = ((-3 * sAcc + fAcc) * pow(ftime, 2) / 2 - (6 * sVel + 4 * fVel) * ftime - 10 * (sAngle - fAngle)) / pow(ftime, 3);
	coeff[4] = ((3 * sAcc -2 * fAcc) * pow(ftime, 2) / 2 + (8 * sVel + 7 * fVel) * ftime + 15 * (sAngle - fAngle)) / pow(ftime, 4);
	coeff[5] = ((-sAcc + fAcc) * pow(ftime, 2) / 2 - 3 * (sVel + fVel) * ftime - 6 * (sAngle - fAngle)) / pow(ftime, 5);
	*/
}

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double sangle, double fangle, double sVel, double fVel, double vangle, double vVel, double time, double vtime) {
	sAngle = sangle; fAngle = fangle;
	stime = 0; ftime = time;
	vAngle = vangle; this->vtime = vtime;

	PTM::TMatrixRow<6, 6, double> A;
	PTM::TMatrixRow<6, 1, double> b, x;
	if (vtime > stime && vtime < ftime) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sangle;
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel;
		A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
		A.row(3) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[3][0] = vangle;
		A.row(4) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[4][0] = vVel;
		A.row(5) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[5][0] = 0;
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToV[n] = x[n][0]; }

		A.row(0) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[0][0] = vangle;
		A.row(1) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[1][0] = vVel;
		A.row(2) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[2][0] = 0;
		A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fangle;
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel;
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToF[n] = x[n][0]; }

	}
	else {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sangle;
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel;
		A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
		A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fAngle;
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel;
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToF[n] = x[n][0]; }

	}
	/*/
	coeff[0] = sAngle;
	coeff[1] = sVel;
	coeff[2] = sAcc / 2;
	coeff[3] = ((-3 * sAcc + fAcc) * pow(ftime, 2) / 2 - (6 * sVel + 4 * fVel) * ftime - 10 * (sAngle - fAngle)) / pow(ftime, 3);
	coeff[4] = ((3 * sAcc -2 * fAcc) * pow(ftime, 2) / 2 + (8 * sVel + 7 * fVel) * ftime + 15 * (sAngle - fAngle)) / pow(ftime, 4);
	coeff[5] = ((-sAcc + fAcc) * pow(ftime, 2) / 2 - 3 * (sVel + fVel) * ftime - 6 * (sAngle - fAngle)) / pow(ftime, 5);
	*/
}

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double vangle, double time, double vtime) {
	sAngle = 0; fAngle = 0;
	stime = 0; ftime = time;
	vAngle = vangle; this->vtime = vtime;

	double internalRatio = (double)(vtime - stime) / (ftime - stime);
	double tvs = vtime - stime;
	double tfv = ftime - vtime;
	double relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * vangle;
	double relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * vangle;

	PTM::TMatrixRow<6, 6, double> A;
	PTM::TMatrixRow<6, 1, double> b, x;
	if (vtime > stime && vtime < ftime) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = 0;
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = 0;
		A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
		A.row(3) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[3][0] = vangle;
		A.row(4) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[4][0] = relvvel;
		A.row(5) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[5][0] = relvacc;
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToV[n] = x[n][0]; }

		A.row(0) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[0][0] = vangle;
		A.row(1) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[1][0] = relvvel;
		A.row(2) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[2][0] = relvacc;
		A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = 0;
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = 0;
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToF[n] = x[n][0]; }

	}
	else {
		for (int n = 0; n < 6; ++n) { coeffToF[n] = 0; }

	}
	/*/
	coeff[0] = sAngle;
	coeff[1] = sVel;
	coeff[2] = sAcc / 2;
	coeff[3] = ((-3 * sAcc + fAcc) * pow(ftime, 2) / 2 - (6 * sVel + 4 * fVel) * ftime - 10 * (sAngle - fAngle)) / pow(ftime, 3);
	coeff[4] = ((3 * sAcc -2 * fAcc) * pow(ftime, 2) / 2 + (8 * sVel + 7 * fVel) * ftime + 15 * (sAngle - fAngle)) / pow(ftime, 4);
	coeff[5] = ((-sAcc + fAcc) * pow(ftime, 2) / 2 - 3 * (sVel + fVel) * ftime - 6 * (sAngle - fAngle)) / pow(ftime, 5);
	*/
}


double AngleMinJerkTrajectory::GetCurrentAngle(double t) {
	if (t <= stime) {
		return sAngle;
	}if (t >= ftime) {
		return fAngle;
	}
	double s = (double)(t - stime) / (ftime - stime);
	double r = t - stime;
	return (r < vtime ?  coeffToV : coeffToF) * Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5));
}

double AngleMinJerkTrajectory::GetDeltaAngle(double t) {
	if (t <= stime) {
		return 0;
	}if (t >= ftime) {
		return fAngle - sAngle;
	}
	//double s = (double)(t - stime) / (ftime - stime);
	//double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	//return (fAngle - sAngle) * sr;
	return GetCurrentAngle(t) - sAngle;
}

double AngleMinJerkTrajectory::GetCurrentVelocity(double t) {
	if (t < stime) {
		return 0;
	}if (t > ftime) {
		return 0;
	}
	double s = (double)(t - stime) / (ftime - stime);
	double r = t - stime;
	return (r < vtime ? coeffToV : coeffToF) * Vec6d(0, 1, 2 * r, 3 * pow(r, 2), 4 * pow(r, 3), 5 * pow(r, 4));
}

ControlPoint::ControlPoint() {
	pose = Posed();
	vel = SpatialVector();
	acc = SpatialVector();
	time = 0;
	velControl = false;
	accControl = false;
	timeControl = false;
}

ControlPoint::ControlPoint(Posed p, double t) {
	pose = p;
	vel = SpatialVector();
	acc = SpatialVector();
	time = t;
	velControl = false;
}
ControlPoint::ControlPoint(Posed p, SpatialVector v, SpatialVector a, double t) {
	pose = p;
	vel = v;
	acc = a;
	time = t;
	velControl = true;
}

FWTrajectoryPlanner::HingeJoint::HingeJoint(PHIKHingeActuatorIf* hinge, std::string path, bool oe) {
	this->hinge = hinge;
	outputEnable = oe;
	torGraph = new std::ofstream(path + "Torque.csv");
	torChangeGraph = new std::ofstream(path + "TorqueChange.csv");
}
FWTrajectoryPlanner::HingeJoint::~HingeJoint(){
	CloseFile();
	delete torGraph;
	delete torChangeGraph;
}
void FWTrajectoryPlanner::HingeJoint::Initialize(int iterate, double mtime, int nVia, double rate, bool vCorr) {
	// 
	hinge->Enable(true);
	this->iterate = iterate;
	this->mtime = mtime;
	this->movetime = mtime * hinge->GetJoint()->GetScene()->GetTimeStepInv();
	originalSpring = hinge->GetJoint()->GetSpring();
	originalDamper = hinge->GetJoint()->GetDamper();
	this->rateLPF = std::min(1.0, std::max(0.0, rate));

	// データのリサイズ
	torque.resize(movetime, 0);
	torqueLPF.resize(movetime, 0);
	angle.resize(iterate + 1, movetime);
	angleLPF.resize(iterate + 1, movetime);
	angleVels.resize(iterate + 1, movetime);
	angleVelsLPF.resize(iterate + 1, movetime);
	tChanges.resize(nVia + 1, 0);

	torqueChange.resize(iterate + 1, INFINITY);
	torqueChangeLPF.resize(iterate + 1, INFINITY);

	// 初期姿勢時のデータを保存
	initialTorque = hinge->GetJoint()->GetMotorForceN(0);
	initialAngle = hinge->GetJoint()->GetPosition();
	initialVel = hinge->GetJoint()->GetVelocity();

	// 経由点関係の初期化
	viaAngles.resize(nVia + 1, 0);
	viaVels.resize(nVia + 1, 0);
	viatimes.resize(nVia + 1, 0);
	viaCorrect = vCorr;

	CorrTraj.resize(iterate, mtime);

	// プラグ剛体の慣性テンソルを質量から計算
	// なお、密度均一として
	double volume = 0;
	PHSolidIf* so = hinge->GetJoint()->GetPlugSolid();
	int nShape = so->NShape();
	for (int i = 0; i < nShape; i++) {
		volume += so->GetShape(i)->CalcVolume();
	}
	if (volume == 0) return;
	double density = so->GetMass() / volume;
	for (int i = 0; i < nShape; i++) {
		so->GetShape(i)->SetDensity(density);
	}
	so->CompInertia();
}
void FWTrajectoryPlanner::HingeJoint::MakeJointMinjerk(int cnt) {
	double dt = hinge->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = hinge->GetJoint()->GetScene()->GetTimeStepInv();
	AngleMinJerkTrajectory hmjt = AngleMinJerkTrajectory(initialAngle, targetAngle, initialVel, targetVel, 0, 0, mtime);
	for (int j = 0; j < movetime; j++) {
		angle[cnt][j] = hmjt.GetCurrentAngle((j + 1) * dt);
		angleVels[cnt][j] = hmjt.GetCurrentVelocity((j + 1) * dt);
	}
}
void FWTrajectoryPlanner::HingeJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void FWTrajectoryPlanner::HingeJoint::SaveTorque(int n) {
	//試しにLimitForce引いてみる
	torque[n] = hinge->GetJoint()->GetMotorForceN(0);
	torque[n] += hinge->GetJoint()->GetLimitForce();
	//torque[n] += hinge->GetJoint()->GetMotorForceN(1);
	torqueLPF[n] = torque[n];
}
void FWTrajectoryPlanner::HingeJoint::SaveTarget() {
	targetAngle = hinge->GetJoint()->GetPosition();
	targetVel = hinge->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::HingeJoint::SetTarget(int k, int n) {
	//後で整理
	if (k >= 0 && k <= iterate) {
		hinge->GetJoint()->SetTargetPosition(angle[k][n]);
#if INITIAL_INTERPOLATION == 1
		int s = movetime * 0.1;
		if (k == 0 && n < s) {
			double r = (double)n / s;
			hinge->GetJoint()->SetTargetPosition(r * angle[0][n] + (1 - r) * initialAngle);
		}
#endif
	}
}

void FWTrajectoryPlanner::HingeJoint::SetTargetVelocity(int k, int n) {
	if (k >= 0 && k <= iterate) {
		hinge->GetJoint()->SetTargetVelocity(angleVels[k][n]);
		//hinge->GetJoint()->SetTargetVelocity(hinge->GetJoint()->GetVelocity());
	}
}

void FWTrajectoryPlanner::HingeJoint::SetTargetInitial() {
	hinge->GetJoint()->SetTargetPosition(initialAngle);
	hinge->GetJoint()->SetTargetVelocity(initialVel);
}
void FWTrajectoryPlanner::HingeJoint::SetOffsetFromLPF(int n) {
	hinge->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
}
void FWTrajectoryPlanner::HingeJoint::ResetOffset(double o) {
	int nMotors = hinge->GetJoint()->NMotors();
	for (int i = 0; i < nMotors; i++) {
		hinge->GetJoint()->SetOffsetForceN(i, o);
	}
}
void FWTrajectoryPlanner::HingeJoint::SavePosition(int k, int n) {
	angle[k][n] = hinge->GetJoint()->GetPosition();
}
void FWTrajectoryPlanner::HingeJoint::SaveVelocity(int k, int n) {
	angleVels[k][n] = hinge->GetJoint()->GetVelocity();
}

void FWTrajectoryPlanner::HingeJoint::SaveViaPoint(int v, int t) {
	viaAngles[v] = hinge->GetJoint()->GetPosition();
	viaVels[v] = hinge->GetJoint()->GetVelocity();
	viatimes[v] = t;
}
void FWTrajectoryPlanner::HingeJoint::SavePositionFromLPF(int k, int n) {
	angleLPF[k][n] = hinge->GetJoint()->GetPosition();
	angle[k][n] = hinge->GetJoint()->GetPosition();
}
void FWTrajectoryPlanner::HingeJoint::SaveVelocityFromLPF(int k, int n) {
	angleVelsLPF[k][n] = hinge->GetJoint()->GetVelocity();
	angleVels[k][n] = hinge->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::HingeJoint::TrajectoryCorrection(int k, bool s) {
	double dt = hinge->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = hinge->GetJoint()->GetScene()->GetTimeStepInv();

	//終端の到達および停止保証部
	int stime = (int)(movetime * 0.0);

	for (int i = 0; i < angleLPF.width(); i++) {
		DSTR << k << " " << i << " " << angleLPF[k][i] << std::endl;
	}
	double end = angleLPF[k][angleLPF.width() - 1];
	double endVel = angleVelsLPF[k][angleVelsLPF.width() - 1];
	double endAcc = (angleVelsLPF[k][angleVelsLPF.width() - 1] - angleVelsLPF[k][angleVelsLPF.width() - 2]) * dtInv;

	double time = mtime;

	AngleMinJerkTrajectory delta = AngleMinJerkTrajectory(0, targetAngle - end, 0, -endVel, 0, -endAcc, time - stime);
	DSTR << "target:" << targetAngle << " actual:" << delta.GetCurrentAngle(mtime) << std::endl;
	for (int i = 0; i < movetime - stime; i++) {
		angle[k][i + stime] = delta.GetDeltaAngle((i + 1) * dt) + angleLPF[k][i + stime]; //k+1でいいのか？
		angleLPF[k][i + stime] += delta.GetDeltaAngle((i + 1) * dt);
		//angleVels[k][i + stime] = delta.GetCurrentVelocity((i + 1) * dt) * perInv + angleVelsLPF[k][i + stime];
		angleVels[k][i + stime] = (angle[k][i + stime] - (i + stime > 0 ? angle[k][i + stime - 1] : initialAngle)) * dtInv;
		angleVelsLPF[k][i + stime] += delta.GetCurrentVelocity((i + 1) * dt) * dtInv;
		//CorrTraj[k - 1][i + stime] = delta.GetCurrentVelocity(i * dt);
	}
}
void FWTrajectoryPlanner::HingeJoint::ApplyLPF(int count) {
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torqueLPF, 3, 1, initialTorque);
	}
	if (count > 0) {
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = rateLPF * torqueLPF[i] + (1 - rateLPF) * initialTorque;
		}
	}
}
void FWTrajectoryPlanner::HingeJoint::Soften() {
	hinge->GetJoint()->SetSpring(0);
	hinge->GetJoint()->SetDamper(0);
}
void FWTrajectoryPlanner::HingeJoint::Harden() {
	if (mul) {
		hinge->GetJoint()->SetSpring(originalSpring * hardenSpring);
		hinge->GetJoint()->SetDamper(originalDamper * hardenDamper);
	}
	else {
		hinge->GetJoint()->SetSpring(hardenSpring);
		hinge->GetJoint()->SetDamper(hardenDamper);
	}
}
void FWTrajectoryPlanner::HingeJoint::ResetPD() {
	hinge->GetJoint()->SetSpring(originalSpring * 1e-5);
	hinge->GetJoint()->SetDamper(originalDamper * 1e-5);
}
double FWTrajectoryPlanner::HingeJoint::CalcTotalTorqueChange() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		total += pow(abs(torque[i] - (((i - 1) < 0) ? initialTorque : torque[i - 1])), 2);
	}
	DSTR << total << std::endl;
	return total;
}

double FWTrajectoryPlanner::HingeJoint::CalcTotalTorqueChangeLPF() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		total += pow(abs(torqueLPF[i] - (((i - 1) < 0) ? initialTorque : torqueLPF[i - 1])), 2);
	}
	DSTR << total << std::endl;
	return total;
}
double FWTrajectoryPlanner::HingeJoint::CalcTorqueChangeInSection(int n) {
	//n = 0, ..., nVia - 1(経由点としては最後), nVia(目標点だけど経由点同様に扱う)
	double total = 0;
	if (n >= 0 && n < viatimes.size()) {
		int start = (n == 0) ? 0 : viatimes[n - 1];
		int end = viatimes[n];
		for (int i = start; i < end; i++) {
			total += pow(abs(torque[i] - (((i - 1) < 0) ? initialTorque : torque[i - 1])), 2);
		}
	}
	DSTR << total << std::endl;
	return total;
}
double FWTrajectoryPlanner::HingeJoint::GetBestTorqueChangeInSection(int n) {
	if (n < 0 || n >= viatimes.size()) return 0;
	return tChanges[n];
}
void FWTrajectoryPlanner::HingeJoint::SetBestTorqueChange() {
	for (int n = 0; n < viatimes.size(); n++) {
		tChanges[n] = CalcTorqueChangeInSection(n);
	}
}
void FWTrajectoryPlanner::HingeJoint::ShowInfo() {
	DSTR << "Pose:" << hinge->GetJoint()->GetPosition() << " Target:" << hinge->GetJoint()->GetTargetPosition() << " Vel:" << hinge->GetJoint()->GetVelocity()  << " TarVel:" << hinge->GetJoint()->GetTargetVelocity() << std::endl;
	Posed plugPose, socketPose;
	hinge->GetJoint()->GetPlugPose(plugPose);
	hinge->GetJoint()->GetSocketPose(socketPose);
	//DSTR << "plug:" << hinge->GetJoint()->GetPlugSolid()->GetPose() * plugPose << " socket:" << hinge->GetJoint()->GetSocketSolid()->GetPose() * socketPose << std::endl;
	PH1DJointLimitIf* limit = hinge->GetJoint()->GetLimit();
	if (limit) {
		if (limit->IsOnLimit()) {
			DSTR << hinge->GetJoint()->GetName() << " is over the limit" << std::endl;
		}
	}
}
void FWTrajectoryPlanner::HingeJoint::SetTargetCurrent() {
	hinge->GetJoint()->SetTargetPosition(hinge->GetJoint()->GetPosition());
}
void FWTrajectoryPlanner::HingeJoint::SetPD(double s, double d, bool mul) {
	this->hardenSpring = s;
	this->hardenDamper = d;
	this->mul = mul;
}
void FWTrajectoryPlanner::HingeJoint::SetTargetFromLPF(int k, int n) {
	if (k >= 0 && k <= iterate) {
		hinge->GetJoint()->SetTargetPosition(angleLPF[k][n]);
	}
}
void FWTrajectoryPlanner::HingeJoint::UpdateIKParam(double b, double p) {
	//hinge->SetBias(b);
	hinge->SetPullbackRate(p);
}

void FWTrajectoryPlanner::HingeJoint::OutputTorque() {
		*torGraph << initialTorque << ",";
		for (int i = 0; i < movetime; i++) {
			*torGraph << torque[i] << ",";
		}
		*torGraph << std::endl;
		*torChangeGraph << abs(torque[0] - initialTorque) << ",";
		for (int i = 0; i < movetime - 1; i++) {
			*torChangeGraph << abs(torque[i + 1] - torque[i]) << ",";
		}
		*torChangeGraph << std::endl;
}

FWTrajectoryPlanner::BallJoint::BallJoint(PHIKBallActuatorIf* ball, std::string path, bool oe) {
	this->ball = ball;
	outputEnable = oe;
	torGraph = new std::ofstream(path + "Torque.csv");
	torChangeGraph = new std::ofstream(path + "TorqueChange.csv");
}
FWTrajectoryPlanner::BallJoint::~BallJoint() { 
	CloseFile();
	delete torGraph;
	delete torChangeGraph;
}
void FWTrajectoryPlanner::BallJoint::Initialize(int iterate, double mtime, int nVia, double rate, bool vCorr) {
	// 
	ball->Enable(true);
	this->iterate = iterate;
	this->mtime = mtime;
	this->movetime = mtime * ball->GetJoint()->GetScene()->GetTimeStepInv();
	this->rateLPF = std::min(1.0, std::max(0.0, rate));
	originalSpring = ball->GetJoint()->GetSpring();
	originalDamper = ball->GetJoint()->GetDamper();

	// データのリサイズ
	torque.resize(movetime, Vec3d());
	torqueLPF.resize(movetime, Vec3d());
	ori.resize(iterate + 1, movetime);
	oriLPF.resize(iterate + 1, movetime);
	oriVels.resize(iterate + 1, movetime);
	oriVelsLPF.resize(iterate + 1, movetime);

	torqueChange.resize(iterate + 1, INFINITY);
	torqueChangeLPF.resize(iterate + 1, INFINITY);

	// 初期姿勢時のデータを保存
	initialTorque = ball->GetJoint()->GetMotorForceN(0);
	initialOri = ball->GetJoint()->GetPosition();
	initialVel = ball->GetJoint()->GetVelocity();

	// 経由点関係の初期化
	viaOris.resize(nVia + 1, Quaterniond());
	viaVels.resize(nVia + 1, Vec3d());
	viatimes.resize(nVia + 1, 0);
	tChanges.resize(nVia + 1, 0);
	viaCorrect = vCorr;

	// プラグ剛体の慣性テンソルを質量から計算
	// なお、密度は均一として
	double volume = 0;
	PHSolidIf* so = ball->GetJoint()->GetPlugSolid();
	int nShape = so->NShape();
	for (int i = 0; i < nShape; i++) {
		volume += so->GetShape(i)->CalcVolume();
	}
	if (volume == 0) return;
	double density = so->GetMass() / volume;
	for (int i = 0; i < nShape; i++) {
		so->GetShape(i)->SetDensity(density);
	}
	so->CompInertia();
	DSTR << "inertia" << std::endl << so->GetInertia() << std::endl;
	return;
}
void FWTrajectoryPlanner::BallJoint::MakeJointMinjerk(int cnt) {
	double dt = ball->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = ball->GetJoint()->GetScene()->GetTimeStepInv();
	QuaMinJerkTrajectory qmjt = QuaMinJerkTrajectory(initialOri, targetOri, initialVel, targetVel, Vec3d(), Vec3d(), mtime);

	for (int j = 0; j < movetime; j++) {
		ori[cnt][j] = qmjt.GetCurrentQuaternion((j + 1) * dt);
		oriVels[cnt][j] = qmjt.GetCurrentVelocity((j + 1) * dt);
	}
}
void FWTrajectoryPlanner::BallJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void FWTrajectoryPlanner::BallJoint::SaveTorque(int n) {
	this->torque[n] = ball->GetJoint()->GetMotorForceN(0);
	//torque[n] += ball->GetJoint()->GetLimitForce();
	//this->torque[n] += ball->GetJoint()->GetMotorForceN(1);
	this->torqueLPF[n] = torque[n];
}
void FWTrajectoryPlanner::BallJoint::SaveTarget() {
	targetOri = ball->GetJoint()->GetPosition();
	targetVel = ball->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::BallJoint::SetTarget(int k, int n) {
	if (k >= 0 && k <= iterate) {
		ball->GetJoint()->SetTargetPosition(ori[k][n]);
#if INITIAL_INTERPOLATION == 1
		int s = movetime * 0.1;
		double r = (double)n / s;
		if (k == 0 && n < s) {
			ball->GetJoint()->SetTargetPosition(interpolate(r, initialOri, ori[0][n]));
		}
#endif
	}
}
void FWTrajectoryPlanner::BallJoint::SetTargetVelocity(int k, int n) {
	if (k >= 0 && k <= iterate) {
		ball->GetJoint()->SetTargetVelocity(oriVels[k][n]);
		//ball->GetJoint()->SetTargetVelocity(ball->GetJoint()->GetVelocity());
	}
}
void FWTrajectoryPlanner::BallJoint::SetTargetInitial() {
	ball->GetJoint()->SetTargetPosition(initialOri);
	ball->GetJoint()->SetTargetVelocity(initialVel);
}
void FWTrajectoryPlanner::BallJoint::SetOffsetFromLPF(int n) {
	ball->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
	DSTR << torqueLPF[n] << " ";
	//ball->GetJoint()->SetOffsetForceN(0, Vec3d(1, 0, 0));
}
void FWTrajectoryPlanner::BallJoint::ResetOffset(double o) {
	int nMotors = ball->GetJoint()->NMotors();
	for (int i = 0; i < nMotors; i++) {
		ball->GetJoint()->SetOffsetForceN(i, Vec3d(o, o, o));
	}
}
void FWTrajectoryPlanner::BallJoint::SavePosition(int k, int n) {
	ori[k][n] = ball->GetJoint()->GetPosition();
}
void FWTrajectoryPlanner::BallJoint::SaveVelocity(int k, int n) {
	oriVels[k][n] = ball->GetJoint()->GetVelocity();
	if (k == 0) {
		Vec3d calced = ori[k][n].AngularVelocity(ori[k][n] - (n > 0 ? ori[k][n - 1] : initialOri));
		DSTR << "comp vel rec:" << oriVels[k][n] << " calced:" << calced << std::endl;
	}
}

void FWTrajectoryPlanner::BallJoint::SaveViaPoint(int v, int t) {
	viaOris[v] = ball->GetJoint()->GetPosition();
	viaVels[v] = ball->GetJoint()->GetVelocity();
	viatimes[v] = t;
}
void FWTrajectoryPlanner::BallJoint::SavePositionFromLPF(int k, int n) {
	oriLPF[k][n] = ball->GetJoint()->GetPosition();
	ori[k][n] = ball->GetJoint()->GetPosition();
}
void FWTrajectoryPlanner::BallJoint::SaveVelocityFromLPF(int k, int n) {
	oriVelsLPF[k][n] = ball->GetJoint()->GetVelocity();
	oriVels[k][n] = ball->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::BallJoint::TrajectoryCorrection(int k, bool s) {
	double dt = ball->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = ball->GetJoint()->GetScene()->GetTimeStepInv();

	// 終端条件保証
	double stime = (int)(mtime * 0.0);

	Quaterniond end = oriLPF[k][oriLPF.width() - 1];
	Vec3d endVel = oriVelsLPF[k][oriVels.width() - 1];
	Vec3d endAcc = (oriVelsLPF[k][oriVelsLPF.width() - 1] - oriVelsLPF[k][oriVelsLPF.width() - 2]) * dtInv;

	double time = mtime;

	QuaMinJerkTrajectory delta = QuaMinJerkTrajectory(Quaterniond(), targetOri * end.Inv(), Vec3d(), -endVel, Vec3d(), -endAcc, time - stime);
	
	DSTR << "target:" << targetOri * end.Inv() << " actual:" << delta.GetCurrentQuaternion(mtime) << std::endl;

	// <!!> stime が doubleです
	for (int i = 0; i < movetime - stime; i++) {
		ori[k][i + stime] = delta.GetCurrentQuaternion((i + 1) * dt) * oriLPF[k][i + stime];
		oriVels[k][i + stime] = ori[k][i + stime].AngularVelocity(ori[k][i + stime] - (i + stime > 0 ? ori[k][i + stime - 1] : initialOri)) * dtInv;

		oriLPF[k][i + stime] = ori[k][i + stime];
		oriVelsLPF[k][i + stime] = oriVels[k][i + stime];
	}
}
void FWTrajectoryPlanner::BallJoint::ApplyLPF(int count) {
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torqueLPF, 3, 1, initialTorque);
	}
	if (count > 0) {
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = rateLPF * torqueLPF[i] + (1 - rateLPF) * initialTorque;
		}
	}
}
void FWTrajectoryPlanner::BallJoint::Soften() {
	ball->GetJoint()->SetSpring(0);
	ball->GetJoint()->SetDamper(0);
}
void FWTrajectoryPlanner::BallJoint::Harden() {
	if (mul) {
		ball->GetJoint()->SetSpring(originalSpring * hardenSpring);
		ball->GetJoint()->SetDamper(originalDamper * hardenDamper);
	}
	else {
		ball->GetJoint()->SetSpring(hardenSpring);
		ball->GetJoint()->SetDamper(hardenDamper);
	}
}
void FWTrajectoryPlanner::BallJoint::ResetPD() {
	ball->GetJoint()->SetSpring(originalSpring * 1e-5);
	ball->GetJoint()->SetDamper(originalDamper * 1e-5);
}
double FWTrajectoryPlanner::BallJoint::CalcTotalTorqueChange() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		for (int j = 0; j < 3; j++) {
			total += pow(abs(torque[i][j] - (((i - 1) < 0) ? initialTorque[j] : torque[i - 1][j])), 2);
			//DSTR << torque[i][j] << " " << (((i - 1) < 0) ? initialTorque[j] : torque[i - 1][j]) << std::endl;
		}
	}
	DSTR << total << std::endl;
	return total * weight;
}
double FWTrajectoryPlanner::BallJoint::CalcTotalTorqueChangeLPF() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		for (int j = 0; j < 3; j++) {
			total += pow(abs(torqueLPF[i][j] - (((i - 1) < 0) ? initialTorque[j] : torqueLPF[i - 1][j])), 2);
			DSTR << torqueLPF[i][j] << " " << (((i - 1) < 0) ? initialTorque[j] : torqueLPF[i - 1][j]) << std::endl;
		}
	}
	DSTR << total << std::endl;
	return total * weight;
}
double FWTrajectoryPlanner::BallJoint::CalcTorqueChangeInSection(int n) {
	double total = 0;
	if (n >= 0 && n < viatimes.size()) {
		int start = (n == 0) ? 0 : viatimes[n - 1];
		int end = viatimes[n];
		for (int i = start; i < end; i++) {
			for (int j = 0; j < 3; j++) {
				total += pow(abs(torque[i][j] - (((i - 1) < 0) ? initialTorque[j] : torque[i - 1][j])), 2);
				DSTR << torque[i][j] << " " << (((i - 1) < 0) ? initialTorque[j] : torque[i - 1][j]) << std::endl;
			}
		}
	}
	DSTR << total << std::endl;
	return total * weight;
}
double FWTrajectoryPlanner::BallJoint::GetBestTorqueChangeInSection(int n) {
	if (n < 0 || n >= viatimes.size()) return 0;
	return tChanges[n];
}
void FWTrajectoryPlanner::BallJoint::SetBestTorqueChange() {
	for(int n = 0; n < viatimes.size(); n++) {
		tChanges[n] = CalcTorqueChangeInSection(n);
	}
}
void FWTrajectoryPlanner::BallJoint::ShowInfo() {
	DSTR << "Pose:" << ball->GetJoint()->GetPosition() << " Target:" << ball->GetJoint()->GetTargetPosition() << " Vel:" << ball->GetJoint()->GetVelocity() << " TarVel:" << ball->GetJoint()->GetTargetVelocity() << std::endl;
	Posed plugPose, socketPose;
	ball->GetJoint()->GetPlugPose(plugPose);
	ball->GetJoint()->GetSocketPose(socketPose);
	//DSTR << "plug:" << ball->GetJoint()->GetPlugSolid()->GetPose() * plugPose << " socket:" << ball->GetJoint()->GetSocketSolid()->GetPose() * socketPose << std::endl;
	PHBallJointLimitIf* limit = ball->GetJoint()->GetLimit();
	if (limit) {
		if (limit->IsOnLimit()) {
			DSTR << ball->GetJoint()->GetName() << " is over the limit" << std::endl;
		}
	}
}
void FWTrajectoryPlanner::BallJoint::SetTargetCurrent() {
	ball->GetJoint()->SetTargetPosition(ball->GetJoint()->GetPosition());
}
void FWTrajectoryPlanner::BallJoint::SetPD(double s, double d, bool mul) {
	this->hardenSpring = s;
	this->hardenDamper = d;
	this->mul = mul;
}
void FWTrajectoryPlanner::BallJoint::SetTargetFromLPF(int k, int n) {
	if (k >= 0 && k <= iterate) {
		ball->GetJoint()->SetTargetPosition(oriLPF[k][n]);
	}
}

void FWTrajectoryPlanner::BallJoint::UpdateIKParam(double b, double p) {
	//ball->SetBias(b);
	ball->SetPullbackRate(p);
}

void FWTrajectoryPlanner::BallJoint::OutputTorque() {
	*torGraph << initialTorque.norm() << ",";
	for (int i = 0; i < movetime; i++) {
		*torGraph << torque[i].norm() << ",";
	}
	*torGraph << std::endl;
	*torChangeGraph << abs(torque[0] - initialTorque).norm() << ",";
	for (int i = 0; i < movetime - 1; i++) {
		*torChangeGraph << abs(torque[i + 1] - torque[i]).norm() << ",";
	}
	*torChangeGraph << std::endl;
}

FWTrajectoryPlanner::Joints::Joints() { joints.clear(); joints.shrink_to_fit(); }
FWTrajectoryPlanner::Joints::~Joints() { RemoveAll();  std::vector<Joint*>().swap(joints); }
void FWTrajectoryPlanner::Joints::RemoveAll() {
	// 各要素でnewしているためこう？
	for (int i = 0; i < joints.size(); i++) {
		delete joints[i];
	}
	joints.clear();  joints.shrink_to_fit();
}
void FWTrajectoryPlanner::Joints::Add(PHIKActuatorIf* j, std::string path, bool oe) {
	if (DCAST(PHIKBallActuatorIf, j)) {
		BallJoint* b = new BallJoint(DCAST(PHIKBallActuatorIf, j), path + "Ball" + std::to_string((int)joints.size()), oe);
		joints.push_back(b);
	}
	if (DCAST(PHIKHingeActuatorIf, j)) {
		HingeJoint* h = new HingeJoint(DCAST(PHIKHingeActuatorIf, j), path + "Hinge" + std::to_string((int)joints.size()), oe);
		joints.push_back(h);
	}
}
void FWTrajectoryPlanner::Joints::Initialize(int iterate, double movetime, int nVia, double rate, bool vCorr) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->Initialize(iterate, movetime, nVia, rate, vCorr);
	}
}
void FWTrajectoryPlanner::Joints::MakeJointMinjerk(int cnt) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->MakeJointMinjerk(cnt);
	}
}

void FWTrajectoryPlanner::Joints::CloseFile() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->CloseFile();
	}
}

void FWTrajectoryPlanner::Joints::SetTarget(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTarget(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SetTargetVelocity(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetVelocity(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SetTargetInitial() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetInitial();
	}
}

void FWTrajectoryPlanner::Joints::SaveTorque(int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveTorque(n);
	}
}

void FWTrajectoryPlanner::Joints::SaveTarget() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveTarget();
	}
}

void FWTrajectoryPlanner::Joints::SetOffsetFromLPF(int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetOffsetFromLPF(n);
	}
	DSTR << std::endl;
}

void FWTrajectoryPlanner::Joints::ResetOffset(double o) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ResetOffset(o);
	}
}

void FWTrajectoryPlanner::Joints::SavePosition(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SavePosition(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SaveVelocity(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveVelocity(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SaveViaPoint(int v, int t) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveViaPoint(v, t);
	}
}

void FWTrajectoryPlanner::Joints::SavePositionFromLPF(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SavePositionFromLPF(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SaveVelocityFromLPF(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveVelocityFromLPF(k, n);
	}
}

void FWTrajectoryPlanner::Joints::TrajectoryCorrection(int k, bool s) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->TrajectoryCorrection(k, s);
	}
}

void FWTrajectoryPlanner::Joints::ApplyLPF(int count) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ApplyLPF(count);
	}
}
void FWTrajectoryPlanner::Joints::Soften() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->Soften();
	}
}
void FWTrajectoryPlanner::Joints::Harden() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->Harden();
	}
}
void FWTrajectoryPlanner::Joints::ResetPD() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ResetPD();
	}
}
double FWTrajectoryPlanner::Joints::CalcTotalTorqueChange() {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTotalTorqueChange();
	}
	return total;
}
double FWTrajectoryPlanner::Joints::CalcTotalTorqueChange(std::ofstream& o) {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTotalTorqueChange();
		o << joints[i]->CalcTotalTorqueChange() << ",";
	}
	return total;
}

double FWTrajectoryPlanner::Joints::CalcTotalTorqueChangeLPF() {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTotalTorqueChangeLPF();
	}
	return total;
}
double FWTrajectoryPlanner::Joints::CalcTorqueChangeInSection(int n) {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTorqueChangeInSection(n);
	}
	return total;
}
void FWTrajectoryPlanner::Joints::SetBestTorqueChange() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetBestTorqueChange();
	}
}
double FWTrajectoryPlanner::Joints::GetBestTorqueChangeInSection(int n) {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->GetBestTorqueChangeInSection(n);
	}
	DSTR << std::endl;
	return total;
}
void FWTrajectoryPlanner::Joints::ShowInfo() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ShowInfo();
	}
}
void FWTrajectoryPlanner::Joints::SetTargetCurrent() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetCurrent();
	}
}
void FWTrajectoryPlanner::Joints::SetWeight() {
	double min = INFINITY;
	for (size_t i = 0; i < joints.size(); i++) {
		if (joints[i]->GetMaxForce() < min) {
			min = joints[i]->GetMaxForce();
		}
	}
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetWeight(1.0);
	}
}

void FWTrajectoryPlanner::Joints::SetPD(double s, double d, bool mul) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetPD(s, d, mul);
	}
}

void FWTrajectoryPlanner::Joints::SetTargetFromLPF(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetFromLPF(k, n);
	}
}

void FWTrajectoryPlanner::Joints::UpdateIKParam(double b, double p) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->UpdateIKParam(b, p);
	}
}

void FWTrajectoryPlanner::Joints::OutputTorque() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->OutputTorque();
	}
}

//N-Simple Moving Average LPF
template<class T>
#ifdef _MSC_VER
static PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
#else
PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
#endif
	PTM::VMatrixRow<T> output;
	//DSTR << input.height() << " " << input.width() << std::endl;
	output.resize(input.height(), input.width());
	output.clear();
	DSTR << n << std::endl;
	//std::ofstream filterRecord("C:/Users/hirohitosatoh/Desktop/logs/filter.csv");

	if (input.width() < (size_t)n) {
		return input;
	}
	else {
		for (size_t i = 0; i < input.height(); i++) {
			size_t j = 0;
			T sum = s[i] * n;
			DSTR << s[i] << std::endl;
			for (; j < (size_t)n; j++) {
				sum = sum + input[i][j] - s[i];
				output[i][j] = sum / n;
			}
			for (; j < input.width(); j++) {
				sum = sum + input[i][j] - input[i][j - n];
				output[i][j] = mag * sum / n;
			}
		}
		/*
		for (size_t i = 0; i < input.width(); i++) {
		for (size_t j = 0; j < input.height(); j++) {
		filterRecord << input[j][i] << "," << output[j][i] << ",";
		}
		filterRecord << std::endl;
		}
		*/
	}
	return output;
}

template<class T>
#ifdef _MSC_VER
static PTM::VVector<T> FWTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
#else
PTM::VVector<T> FWTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
#endif
	PTM::VVector<T> output;
	output.resize(input.size());

	if ((n % 2) == 0) n++;
	int half = n / 2;

	size_t j = 0;
	T sum = initial * (half + 1);
	for (int k = 0; k < half; k++) {
		sum += input[k];
	}
	DSTR << initial << std::endl;
	for (; j < (size_t)(half + 1); j++) {  //初期トルクが残ってる部分のループ
		sum = sum + input[j + half] - initial;
		output[j] = mag * sum / n;
	}
	for (; j < input.size() - half; j++) {
		sum = sum + input[j + half] - input[j - half - 1];
		output[j] = mag * sum / n;
	}
	for (; j < input.size(); j++) {
		sum = sum + input[input.size() - 1] - input[j - half - 1];
		output[j] = mag * sum / n;
	}
	return output;
}

template<class T>
#ifdef _MSC_VER
static PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
#else
PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
#endif
	PTM::VMatrixRow<T> output;
	//DSTR << input.height() << " " << input.width() << std::endl;
	output.resize(input.height(), input.width());
	output.clear();

	if (input.width() < 2) {
		return input;
	}
	for (size_t i = 0; i < input.height(); i++) {
		double weight = 0.3 * w[i];
		output[i][0] = input[i][0] + weight * (s[i] + input[i][1] - 2 * input[i][0]);
		for (size_t j = 1; j < input.width() - 1; j++) {
			output[i][j] = input[i][j] + weight * (input[i][j - 1] + input[i][j + 1] - 2 * input[i][j]);
		}
		output[i][input.width() - 1] = input[i][input.width() - 1] + weight * (input[i][input.width() - 2] - input[i][input.width() - 1]);
	}
	return output;
}

template <class T>
#ifdef _MSC_VER
static PTM::VVector<T> FWTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
#else
PTM::VVector<T> FWTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
#endif
	PTM::VVector<T> output;
	output.resize(input.size());

	if (input.size() < 2) {
		return input;
	}
	double weight = 0.3 * w * r;
	//double weight = 0.3 * r;
	output[0] = input[0] + weight * (initial + input[1] - 2 * input[0]);
	for (int j = 1; j < (int)input.size() - 1; j++) {
		output[j] = input[j] + weight * (input[j - 1] + input[j + 1] - 2 * input[j]);
	}
	output[input.size() - 1] = input[input.size() - 1] + weight * (input[input.size() - 2] - input[input.size() - 1]);
	return output;
}

void FWTrajectoryPlanner::Init() {

	//総移動ステップ数設定
	ControlPoint tpoint = viaPoints.back();
	mtime = tpoint.time;
	movtime = tpoint.time * scene->GetTimeStepInv();

	auto start = std::chrono::system_clock::now();

	// ターゲットと経由点のバネ準備
	PrepareSprings();
	
	CheckAndSetJoints();

	//jointsの初期化
	joints.Initialize(maxIterate, mtime, (int)viaPoints.size(), LPFRate, bViaCorrection);
	joints.SetWeight();
	joints.SetPD(springRate, damperRate, bMultiplePD);

	//手先軌道リサイズ
	trajData.resize(maxIterate + 1, movtime);
	trajDataNotCorrected.resize(maxIterate + 1, movtime);
	trajVel.resize(maxIterate + 1, movtime + 1);
	trajVelNotCorrected.resize(maxIterate + 1, movtime + 1);

	//stateの保存
	scene->GetIKEngine()->ApplyExactState();
	states = ObjectStatesIf::Create();
	tmpStates.clear();
	for (int i = 0; i < (int)viaPoints.size(); i++) {
		tmpStates.push_back(ObjectStatesIf::Create());
	}
	for (int i = 0; i < maxIterate; i++) {
		corStates.push_back(ObjectStatesIf::Create());
		beforeCorStates.push_back(ObjectStatesIf::Create());
	}
	DSTR << scene->GetCount() << std::endl;
	DSTR << "Saved" << std::endl;
	DisplayDebugInfo();
	states->SaveState(scene);

	// 計算前フラグ
	isMoving = false;
	isCalculated = false;
	// 途中停止用のフラグを無効化
	isDiverged = false;

	best = 0;
	bestTorque = INFINITY;
}

void FWTrajectoryPlanner::CheckAndSetJoints() {

	PHIKActuatorIf* ika;
	ika = ikEndEffector->GetParentActuator();
	ikEndEffector->Enable(true);

	// depthがオーバーしてないかチェック
	for (int i = 0; i < depth; i++) {
		if (ika->GetParent()) {
			ika = ika->GetParent();
		}
		else {
			depth = i + 1;
			break;
		}
	}
	ika = ikEndEffector->GetParentActuator();

	// 関節をjointsに投げ込む
	joints.RemoveAll();
	for (int i = 0; i < depth; i++) {
		joints.Add(ika, outputPath);
		ika->Enable(true);
		ika = ika->GetParent();
	}
	while (ika) {
		if (DCAST(PHIKBallActuatorIf, ika)) {
			DCAST(PHIKBallActuatorIf, ika)->GetJoint()->GetPlugSolid()->SetDynamical(false);
			DCAST(PHIKBallActuatorIf, ika)->GetJoint()->GetPlugSolid()->SetStationary(true);
		}
		if (DCAST(PHIKHingeActuatorIf, ika)) {
			DCAST(PHIKHingeActuatorIf, ika)->GetJoint()->GetPlugSolid()->SetDynamical(false);
			DCAST(PHIKHingeActuatorIf, ika)->GetJoint()->GetPlugSolid()->SetStationary(true);
		}
		ika = ika->GetParent();
	}
}

void FWTrajectoryPlanner::RecalcFromIterationN(int n) {

}

void FWTrajectoryPlanner::DisplayDebugInfo() { 
//#ifdef _DEBUG
	// Debug用にいろいろ表示する
	DSTR << "----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----" << std::endl;
	int nsolids = scene->NSolids();
	for (int i = 0; i < nsolids; i++) {
		PHSolidIf* so = scene->GetSolids()[i];
		DSTR << "Solid" << i << " mass:" << so->GetMass() << ": pose" << so->GetPose() << " velocity" << so->GetVelocity() << " force" << so->GetForce() << " torque" << so->GetTorque() << std::endl;
	}
	joints.ShowInfo();
	DSTR << "EndeffectorPosition:" << ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition() << std::endl;
	DSTR << "EndeffectorRotation:" << ikEndEffector->GetSolid()->GetPose().Ori() << std::endl;
	DSTR << "EndEffectorVelocity:" << ikEndEffector->GetSolid()->GetVelocity() << std::endl;
	DSTR << "EndEffectorTarget:" << ikEndEffector->GetTargetPosition() << std::endl;
	DSTR << "Gravity:" << scene->GetGravity() << std::endl << std::endl;
//#endif
}

void FWTrajectoryPlanner::OutputTrajectory(std::string filename) {
	// 手先軌道をファイル出力出力
	std::ofstream outfile(filename + ".csv");
	for (int i = 0; i < maxIterate + 1; i++) {
		for (int j = 0; j < trajData.width(); j++) {
			outfile << trajData[i][j].Pos().x << "," << trajData[i][j].Pos().y << "," << trajData[i][j].Pos().z << "," << std::endl;
		}
	}
}

void FWTrajectoryPlanner::OutputNotCorrectedTrajectory(std::string filename) {
	// 修正前の手先軌道をファイル出力
	std::ofstream outfile(filename + "NotCorrected.csv");
	for (int i = 0; i < maxIterate; i++) {
		for (int j = 0; j < movtime; j++) {
			outfile << trajDataNotCorrected[i][j].Pos().x << "," << trajDataNotCorrected[i][j].Pos().y << "," << trajDataNotCorrected[i][j].Pos().z << "," << std::endl;
		}
	}
}

void FWTrajectoryPlanner::OutputVelocity(std::string filename) {
	Vec3d data;
	// 手先剛体の速度をファイル出力
	std::ofstream outfile(filename + "Velocity.csv");
	for (int i = 0; i < maxIterate + 1; i++) {
		for (int j = 0; j < movtime + 1; j++) {
			data = trajVel[i][j].v();
			outfile << data.x << "," << data.y << "," << data.z << "," << data.norm() << ",";
			outfile << std::endl;
		}
	}
	std::ofstream outfile2(filename + "VelocityNotCorrected.csv");
	for (int i = 0; i < maxIterate; i++) {
		for (int j = 0; j < movtime + 1; j++) {
			data = trajVelNotCorrected[i][j].v();
			outfile2 << data.x << "," << data.y << "," << data.z << "," << data.norm() << "," << std::endl;
		}
	}
}

void FWTrajectoryPlanner::Replay(int ite, bool noncorrected) {
	if (!isCalculated) return;
	if (ite < 0 || ite > maxIterate) return; // 多分変える

	// 要求された再生番号を設定
	this->ite = ite;
	noncorrectedReplay = noncorrected;
	isMoving = true;
	joints.SetTargetInitial(); 
	LoadInitialState();
	repCount = -50;
}

void FWTrajectoryPlanner::JointTrajStep(bool step) {
	if (isMoving) {
		if (repCount >= 0 && repCount < (int)trajData.width()) {
			if (!noncorrectedReplay) {
				joints.SetTarget(ite, repCount);
			}
			else {
				joints.SetTargetFromLPF(ite, repCount);
			}
			repCount++;
		}
		else if (repCount >= movtime) {
			isMoving = false;
			if (!noncorrectedReplay) {
				joints.SetTarget(ite, movtime - 1);
			}
			else {
				joints.SetTargetFromLPF(ite, movtime - 1);
			}
		}
		else {
			joints.SetTargetInitial();
			repCount++;
		}
		DisplayDebugInfo();
	}
}

void FWTrajectoryPlanner::CalcTrajectory() {
	// OneStep実行を規定回走らせる
	for (; currentIterate <= maxIterate; ) {
		CalcOneStep();
	}
}

void FWTrajectoryPlanner::CalcOneStep() {
	// フェイズ管理して1ステップずつ実行
	switch (phase) {
	case INIT:
		Init();
		MakeMinJerk();
		phase = Phase::INVERSE;
		break;
	case FORWARD:
	    {
		double level = 1 + (double)(maxIterate - currentIterate) / maxIterate;
	    joints.ApplyLPF(maxLPF * level); 
	    }
		CompForwardDynamics(currentIterate);
		phase = Phase::INVERSE;
		break;
	case INVERSE:
		if (currentIterate != 0) {
			TrajectoryCorrection(currentIterate);
		}
		CompInverseDynamics(currentIterate);
		if (currentIterate == maxIterate) {
			PostProcessing();
		}
		currentIterate++;
		phase = Phase::FORWARD;
		break;
	}
}

void FWTrajectoryPlanner::ForwardInverseRelaxation() {
	// --- デバッグ用の出力ファイル準備 ---
	/*
	//躍度最小軌道の生成とトルクの保存
	MakeMinJerk();

	// ----- 軌道計算の繰り返し計算 -----

	for (int k = 1; k < maxIterate && !isDiverged; k++) {
		
		//トルクにLPFをかける
		double level = 1 + (double)(maxIterate - k) / maxIterate;
		joints.ApplyLPF(maxLPF * level);

		//フォワード(トルクー＞軌道)
		CompForwardDynamics(k);

		//軌道の修正
		TrajectoryCorrection(k);
		
		//インバース(軌道ー＞トルク)
		CompInverseDynamics(k);
	}

	// --- 計算後であることのフラグ有効化 ---
	isMoving = true;
	isCalculated = true;
	ite = best;
	repCount = 0;

	//joints.ResetPD();

	//いろいろとファイル出力
	OutputTrajectory(outputPath);
	OutputNotCorrectedTrajectory(outputPath);
	OutputVelocity(outputPath);

	LoadInitialState();
	*/
}

void FWTrajectoryPlanner::MakeMinJerk() {
	// シーンのステップ幅など
	double dt = scene->GetTimeStep();
	double dtInv = 1 / dt;

	// 開始点の設定
	ControlPoint startPoint = ControlPoint();
	startPoint.pose = Posed(ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition(), ikEndEffector->GetSolid()->GetPose().Ori());
	startPoint.vel = GetEndEffectorVelocity();
	startPoint.acc = SpatialVector();
	startPoint.time = 0;
	// 最終目標点の設定
	ControlPoint targetPoint = viaPoints.back();

	// 開始点と最終目標点のみを満たす軌道生成
	MinJerkTrajectory minjerk = MinJerkTrajectory(startPoint, targetPoint);
	PTM::VVector<Posed> minjerkPoses;
	minjerkPoses.resize(movtime);
	PTM::VVector<SpatialVector> minjerkVels;
	minjerkVels.resize(movtime);
	for (int i = 0; i < movtime; i++) {
		DSTR << (i + 1) * dt << std::endl;
		minjerkPoses[i] = minjerk.GetCurrentPose((i + 1) * dt);
		//minjerkVels[i] = minjerk.GetCurrentVelocity(i + 1);
	}
	
	//各点を単純な躍度最小軌道で接続
	for (int i = 0; i < (int)viaPoints.size() + 1; i++) {
		ControlPoint s = (i == 0) ? startPoint : viaPoints[i - 1];
		ControlPoint f = (i == viaPoints.size()) ? targetPoint : viaPoints[i];
		MinJerkTrajectory mjt = MinJerkTrajectory(s, f);
		for (int j = s.time * dtInv; j < f.time * dtInv; j++) {
			minjerkPoses[j] = mjt.GetCurrentPose((j + 1) * dt);
		}
	}
	/*/
	//境界条件満足の部分軌道を足し合わせて経由点を通る軌道実現
	for (int i = 0; i < (int)viaPoints.size(); i++) {
		ControlPoint s = ControlPoint(Posed(), SpatialVector(), SpatialVector(), (i == 0) ? startPoint.time : viaPoints[i - 1].time);
		ControlPoint f = ControlPoint(Posed(), SpatialVector(), SpatialVector(), targetPoint.time);
		ControlPoint c = viaPoints[i];
		c.pose.Pos() -= minjerkPoses[c.time * dtInv - 1].Pos();
		c.pose.Ori() = c.pose.Ori() * minjerkPoses[c.time * dtInv - 1].Ori().Inv();
		double internalRatio = (double)(c.time - s.time) / (f.time - s.time);
		Vec3d relvpos = c.pose.Pos() - ((1 - internalRatio) * s.pose.Pos() + internalRatio * f.pose.Pos());
		double tvs = c.time - s.time;
		double tfv = f.time - c.time;
		Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * c.pose.Pos();
		Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * c.pose.Pos();
		c.vel = SpatialVector(relvvel, Vec3d()) - minjerkVels[c.time * dtInv - 1];
		c.acc = SpatialVector(relvacc, Vec3d()) - (minjerkVels[c.time * dtInv - 1] - (c.time * dtInv > 1 ? minjerkVels[c.time * dtInv - 2] : SpatialVector()));
		MinJerkTrajectory mjt = MinJerkTrajectory(s, f, c);
		for (int j = s.time * dtInv; j < f.time* dtInv; j++) {
			Posed delta = mjt.GetCurrentPose((j + 1) * dt);
			minjerkPoses[j].Pos() += delta.Pos();
			minjerkPoses[j].Ori() = minjerkPoses[j].Ori() * delta.Ori();
			minjerkVels[j] += mjt.GetCurrentVelocity((j + 1) * dt);
		}
	}
	*/
	joints.Harden();
	
	LoadInitialState();
	
	scene->GetIKEngine()->Enable(true);
	DisplayDebugInfo();

	SpatialVector eefVel = GetEndEffectorVelocity();
	for (int i = 0; i < maxIterate + 1; i++) {
		trajVel[i][0] = eefVel;
	}
	for (int i = 0; i < maxIterate; i++) {
		trajVelNotCorrected[i][0] = eefVel;
	}

	int reach = 0;
	int count = 0;
	Posed minjerkTarget;
	// 空間躍度最小軌道を再生して関節角を保存
	for (; reach < movtime; reach++) {
		 minjerkTarget = minjerkPoses[reach];
		ikEndEffector->SetTargetPosition(minjerkTarget.Pos());
		ikEndEffector->SetTargetOrientation(minjerkTarget.Ori());
		//ikEndEffector->SetTargetVelocity((minjerkPoses[reach].Pos() - (reach == 0 ? startPoint.pose.Pos() : minjerkPoses[reach - 1].Pos())) * scene->GetTimeStepInv());
		for (int i = 0; i < 1; i++) {
			scene->Step();
		}
		if (count < (int)viaPoints.size()) {
			if (reach == (int)(viaPoints[count].time * dtInv - 1)) {
				joints.SaveViaPoint(count, viaPoints[count].time);
				count++;
			}
		}
		DisplayDebugInfo();

		joints.SaveTorque(reach);
		for (int i = 0; i < maxIterate + 1; i++) {
			// 今はtargetpos/vel保存
			joints.SavePosition(i, reach);
			joints.SaveVelocity(i, reach);
		}

		trajData[0][reach].Pos() = ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition();
		trajData[0][reach].Ori() = ikEndEffector->GetSolid()->GetPose().Ori();
		eefVel = GetEndEffectorVelocity();
		trajVel[0][reach + 1] = eefVel;
	}

	joints.SaveViaPoint(count, mtime);

	joints.SaveTarget();

	// 関節次元躍度最小軌道の生成
	if (bUseJointMJTInitial) {
		joints.MakeJointMinjerk(0);
	}else{
		joints.MakeJointMinjerk(maxIterate);
	}
	
	LoadInitialState();
}

void FWTrajectoryPlanner::CompForwardDynamics(int k) {
	// 関節オフセットトルクで動かすためにIKをオフ
	scene->GetIKEngine()->Enable(false);
	joints.Soften();

	// 関節オフセットトルクでの駆動 関節角を保存
	Vec3d eefVel;
	for (int i = 0; i < movtime; i++) {
		joints.SetOffsetFromLPF(i);
		scene->Step();
		if (ikEndEffector->GetSolid()->GetVelocity().norm() > 1e3) {
			isDiverged = true;
		}
		joints.SaveTorque(i);
		joints.SavePositionFromLPF(k, i);
		joints.SaveVelocityFromLPF(k, i);

		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Pos() = ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition();
		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Ori() = ikEndEffector->GetSolid()->GetPose().Ori();

		eefVel = ikEndEffector->GetSolid()->GetVelocity();
		trajVelNotCorrected[k > 0 ? k - 1 : 0][i + 1] = GetEndEffectorVelocity();
	}

	joints.ResetOffset(0.0);
	joints.OutputTorque();

	DSTR << "in forward end" << std::endl;
	DisplayDebugInfo();

	//spring,damperをもとの値に
	joints.Harden();
}

void FWTrajectoryPlanner::CompInverseDynamics(int k) {
	// シーンのステップ幅など
	double dtInv = scene->GetTimeStepInv();

	scene->GetIKEngine()->Enable(false);
	joints.Harden();

	// 軌道再生と各関節トルクの保存
	int count = 0;
	for (int i = 0; i < movtime; i++) {

		// jointTargetを設定
		joints.SetTarget(k, i);
		joints.SetTargetVelocity(k, i);

		// Stepで軌道再生
		scene->Step();

		if (ikEndEffector->GetSolid()->GetVelocity().norm() > 1e3) {
			isDiverged = true;
		}

		if (count < (int)viaPoints.size()) {
			if (i == (int)(viaPoints[count].time * dtInv - 1)) {
				joints.SaveViaPoint(count, (viaPoints[count].time * dtInv));
				tmpStates[count++]->SaveState(scene);
			}
		}
		trajData[k][i].Pos() = ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition();
		trajData[k][i].Ori() = ikEndEffector->GetSolid()->GetPose().Ori();
		//DisplayDebugInfo();

		Vec3d eefVel = ikEndEffector->GetSolid()->GetVelocity();
		trajVel[k][i + 1] = GetEndEffectorVelocity();

		//トルクの記録
		joints.SaveTorque(i);
	}
	DSTR << "in inverse end" << std::endl;
	DisplayDebugInfo();

	// トルク変化量の確認
	double torqueChange = joints.CalcTotalTorqueChange();
	if (torqueChange < bestTorque) {
		best = k;
		bestTorque = torqueChange;
		joints.SetBestTorqueChange();
	}

	joints.OutputTorque();

	//ロードしてIK周りの調整
	LoadInitialState();
	DisplayDebugInfo();
}

void FWTrajectoryPlanner::TrajectoryCorrection(int k) {
	// シーンのステップ幅など
	double dtInv = scene->GetTimeStepInv();

	// この前のFDMの終了時には目標到達時間における状態で終了しているはず(多分未到達だが)
	// なので、ここで到達位置や速度を
	beforeCorStates[k - 1]->SaveState(scene);

	if (bCorrection) {
		int count;
		scene->GetIKEngine()->Enable(true);

		//IKを用いて目標に到達させ、jointsに状態保存
		if (!bStaticTarget) {
			ControlPoint targetPoint = viaPoints.back();
			ikEndEffector->SetTargetPosition(targetPoint.pose.Pos());
			ikEndEffector->SetTargetOrientation(targetPoint.pose.Ori());
			count = 0;
			if (true) {
				double dist = (ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
				while (count++ < 50 && dist > 0.01) {
					scene->Step();
					dist = (ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
					DisplayDebugInfo();
				}
			}

			DisplayDebugInfo();
			joints.SaveTarget();
			corStates[k - 1]->SaveState(scene);
		}

		//以降では経由点の通過保証修正を行う
		for (int n = 0; n < (int)viaPoints.size(); n++) {
			tmpStates[n]->LoadState(scene);
			scene->GetIKEngine()->ApplyExactState();
			ikEndEffector->SetTargetPosition(viaPoints[n].pose.Pos());
			ikEndEffector->SetTargetOrientation(viaPoints[n].pose.Ori());
			count = 0;
			double dist = (ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition() - viaPoints[n].pose.Pos()).norm();
			while (count++ < 50 && dist > 0.05) {
				scene->Step();
				dist = (ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition() - viaPoints[n].pose.Pos()).norm();
			}
			DisplayDebugInfo();
			joints.SaveViaPoint(n, viaPoints[n].time);
		}

		// 上で位置修正した姿勢をもとに軌道を修正
		joints.TrajectoryCorrection(k, bStaticTarget);
		scene->GetIKEngine()->Enable(false);
	}

	LoadInitialState();

	// IK後の全関節ターゲットリセット
	for (int i = 0; i < scene->NJoints(); i++) {
		PHJointIf* jo = scene->GetJoint(i);
		if (DCAST(PHBallJointIf, jo)) {
			DCAST(PHBallJointIf, jo)->SetTargetPosition(DCAST(PHBallJointIf, jo)->GetPosition());
			DCAST(PHBallJointIf, jo)->SetTargetVelocity(Vec3d());
		}
		if (DCAST(PHHingeJointIf, jo)) {
			DCAST(PHHingeJointIf, jo)->SetTargetPosition(DCAST(PHHingeJointIf, jo)->GetPosition());
			DCAST(PHHingeJointIf, jo)->SetTargetVelocity(0);
		}
	}
	DisplayDebugInfo();
}

void FWTrajectoryPlanner::PostProcessing() {
	// --- 計算後であることのフラグ有効化 ---
	isMoving = true;
	isCalculated = true;
	ite = best;
	repCount = 0;

	//joints.ResetPD();

	//いろいろとファイル出力
	OutputTrajectory(outputPath);
	OutputNotCorrectedTrajectory(outputPath);
	OutputVelocity(outputPath);

	LoadInitialState();

	joints.CloseFile();
}

bool FWTrajectoryPlanner::AdjustViatime() {
	if (viaPoints.empty()) return false;
	double dtinv = scene->GetTimeStepInv();

	// トルクの総変化量と各セクションでの変化量を取得
	int nVia = (int)viaPoints.size();
	double* tChange = new double[nVia + 1];
	for (int i = 0; i < nVia + 1; i++) {
		tChange[i] = joints.GetBestTorqueChangeInSection(i);
		DSTR << tChange[i] << std::endl;
	}
	DSTR << std::endl;

	// 現在の各区間の時間を求める
	double* ti = new double[nVia + 1];
	for (int i = 0; i < nVia + 1; i++) {
		ti[i] = (i == nVia ? movtime * scene->GetTimeStep() : viaPoints[i].time) - (i == 0 ? 0 : viaPoints[i - 1].time);
		DSTR << ti[i] << std::endl;
	}

	// deltatを求める
	double sumdeltat = 0;
	for(int i = 0; i < nVia + 1; i++){
		sumdeltat += tChange[i] / ti[i];
	}
	double sigma = (movtime * scene->GetTimeStep() * viaAdjustRate) / sumdeltat;
	double* deltat = new double[nVia + 1];
	for (int i = 0; i < nVia + 1; i++) {
		deltat[i] = sigma * tChange[i] / ti[i];
		DSTR << "delta_t " << i << " : " << deltat[i] << std::endl;
	}

	//時間の総変化量を計算
	sumdeltat = 0;
	for (int i = 0; i < nVia + 1; i++) {
		sumdeltat += deltat[i];
	}
	DSTR << "whole sum of delta_t : " << sumdeltat << std::endl;

	// 各経由点到達時間を更新
	double sumtime = 0;
	bool cont = true;
	for (int i = 0; i < nVia; i++) {
		sumtime += deltat[i];
		double newtime = (viaPoints[i].time + sumtime) * movtime * scene->GetTimeStep() / (movtime * scene->GetTimeStep() + sumdeltat);
		int newstep = TimeToStep(newtime);
		cont &= (newstep == (int)(viaPoints[i].time * dtinv));
		viaPoints[i].time = newtime;
		DSTR << "new viatime " << i << " : " << viaPoints[i].time << std::endl;
	}

	delete tChange;
	delete ti;
	delete deltat;

	return !cont;
}

void FWTrajectoryPlanner::InitializeViatime() {
	// 各経由点の通過時間を等間隔に初期化
	int nVia = (int)viaPoints.size();
	for (int i = 0; i < nVia; i++) {
		viaPoints[i].time = mtime * (i + 1) / (nVia + 1);
	}
}

void FWTrajectoryPlanner::PrepareSprings() {
	viaSolids.clear();
	viaSprings.clear();

	// ターゲットの剛体とバネの設定
	PHSolidDesc tDesc = PHSolidDesc();
	tDesc.pose = viaPoints.back().pose;
	tDesc.mass = 1.0;
	tDesc.inertia = Matrix3d::Unit() * tDesc.mass;
	tDesc.dynamical = false;
	targetSolid = scene->CreateSolid(tDesc);
	
	PHSpringDesc tspDesc = PHSpringDesc();
	tspDesc.bEnabled = false;
	tspDesc.spring = Vec3d(1e10, 1e10, 1e10);
	tspDesc.damper = Vec3d(1e2, 1e2, 1e2);
	if (ikEndEffector->IsOrientationControlEnabled()) {
		tspDesc.springOri = 1e8;
		tspDesc.damperOri = 1e2;
	}
	else {
		tspDesc.springOri = 0;
		tspDesc.damperOri = 1e2;
	}
	targetSpring = (PHSpringIf*)scene->CreateJoint(targetSolid, ikEndEffector->GetSolid(), tspDesc);
	targetSpring->SetPlugPose(Posed(ikEndEffector->GetTargetLocalPosition(), Quaterniond()));
	scene->SetContactMode(targetSolid, ikEndEffector->GetSolid(), PHSceneDesc::ContactMode::MODE_NONE);
	
	// 経由点の剛体とバネの設定
	for (size_t i = 0; i < (int)viaPoints.size(); i++) {
		PHSolidDesc vDesc = PHSolidDesc();
		vDesc.pose = viaPoints[i].pose;
		vDesc.mass = 1.0;
		vDesc.inertia = Matrix3d::Unit() * vDesc.mass;
		vDesc.dynamical = false;
		viaSolids.push_back(scene->CreateSolid(vDesc));

		PHSpringDesc vspDesc = PHSpringDesc();
		vspDesc.bEnabled = false;
		vspDesc.spring = Vec3d(1e10, 1e10, 1e10);
		vspDesc.damper = Vec3d(100, 100, 100); 
		if (ikEndEffector->IsOrientationControlEnabled()) {
			vspDesc.springOri = 1e8;
			vspDesc.damperOri = 1e2;
		}
		else {
			vspDesc.springOri = 0;
			vspDesc.damperOri = 1e2;
		}
		viaSprings.push_back((PHSpringIf*)scene->CreateJoint(viaSolids[i], ikEndEffector->GetSolid(), vspDesc));
		viaSprings[i]->SetPlugPose(Posed(ikEndEffector->GetTargetLocalPosition(), Quaterniond()));
		scene->SetContactMode(viaSolids[i], ikEndEffector->GetSolid(), PHSceneDesc::ContactMode::MODE_NONE);
	}
}

void FWTrajectoryPlanner::ReloadCorrected(int k, bool nc) {
	if (k < 0 || k >= maxIterate) return;
	if (nc) {
		beforeCorStates[k]->LoadState(scene);
	}
	else {
		corStates[k]->LoadState(scene);
	}
	int nJoints = scene->NJoints();
	for (int i = 0; i < nJoints; i++) {
		PHJointIf* jo = scene->GetJoint(i);
		if (DCAST(PHBallJointIf, jo)) {
			DCAST(PHBallJointIf, jo)->SetTargetPosition(DCAST(PHBallJointIf, jo)->GetPosition());
			DCAST(PHBallJointIf, jo)->SetTargetVelocity(Vec3d());
		}
		if (DCAST(PHHingeJointIf, jo)) {
			DCAST(PHHingeJointIf, jo)->SetTargetPosition(DCAST(PHHingeJointIf, jo)->GetPosition());
			DCAST(PHHingeJointIf, jo)->SetTargetVelocity(0);
		}
	}
}

}

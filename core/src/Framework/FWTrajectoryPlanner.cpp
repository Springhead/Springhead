#include <Framework/FWTrajectoryPlanner.h>

namespace Spr {
	;
// 散らかってるのは後で整理します

//----- ----- ----- -----

MinJerkTrajectory::MinJerkTrajectory() {
	sPoint = ControlPoint(); fPoint = ControlPoint();
	stime = 0; ftime = 1;
}

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, int time) {
	sPoint = ControlPoint(spose, 0, 0); fPoint = ControlPoint(fpose, time, 0); 
	stime = 0; ftime = time;
	Vec3d diff = fPoint.pose.Pos() - sPoint.pose.Pos();

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

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, Posed vpose, int time, int vtime) {
	stime = 0; ftime = time;
	sPoint = ControlPoint(spose, 0, 0); fPoint = ControlPoint(fpose, ftime, 0);
	vPoint = ControlPoint(vpose, vtime, 0);
	this->vtime = vtime;
	Vec3d diff = fPoint.pose.Pos() - sPoint.pose.Pos();
	//開始と終点を０としたときの経由点の相対位置
	double internalRatio = (double)(vtime - stime) / (ftime - stime);
	Vec3d relvpos = vpose.Pos() - ((1 - internalRatio) * spose.Pos() + internalRatio * fpose.Pos());
	double tvs = vtime - stime;
	double tfv = ftime - vtime;
	Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * relvpos;
	Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * relvpos;
	if (vtime > 0 && vtime < ftime) {
		for (int i = 0; i < 3; ++i) {
			PTM::TMatrixRow<6, 6, double> A;
			PTM::TMatrixRow<6, 1, double> b, x;
			A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
			A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = 0;
			A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
			A.row(3) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[3][0] = vPoint.pose.Pos()[i];
			A.row(4) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[4][0] = relvvel[i];
			A.row(5) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[5][0] = relvacc[i];
			x = A.inv() * b;

			for (int n = 0; n < 6; ++n) { coeffToV[n][i] = x[n][0]; }
		}
		for (int i = 0; i < 3; ++i) {
			PTM::TMatrixRow<6, 6, double> A;
			PTM::TMatrixRow<6, 1, double> b, x;
			A.row(0) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[0][0] = vPoint.pose.Pos()[i];
			A.row(1) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[1][0] = relvvel[i];
			A.row(2) = Vec6d(0, 0, 2, 6 * vtime, 12 * pow(vtime, 2), 20 * pow(vtime, 3)); b[2][0] = relvacc[i];
			A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
			A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = 0;
			A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
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

			for (int n = 0; n < 6; ++n) { coeffToF[n][i] = x[n][0]; coeffToV[n][i] = x[n][0];}
		}
	}
}

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, Vec3d sVel, Vec3d fVel, Vec3d sAcc, Vec3d fAcc, int time, double per) {
	sVel *= per;
	fVel *= per;
	sAcc *= per * per;
	fAcc *= per * per;
	sPoint = ControlPoint(spose, Vec6d(sVel.x, sVel.y, sVel.z, 0, 0, 0), Vec6d(sAcc.x, sAcc.y, sAcc.z, 0, 0, 0), 0, 0);
	fPoint = ControlPoint(fpose, Vec6d(fVel.x, fVel.y, fVel.z, 0, 0, 0), Vec6d(fAcc.x, fAcc.y, fAcc.z, 0, 0, 0), time, 0);
	stime = 0; ftime = time;
	vPoint = ControlPoint(); vtime = 0;
	for (int i = 0; i < 3; ++i) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel[i];
		A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = sAcc[i];
		A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel[i];
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = fAcc[i];
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToF[n][i] = x[n][0]; }
	}
}


MinJerkTrajectory::MinJerkTrajectory(ControlPoint spoint, ControlPoint fpoint, double per) {
	sPoint = ControlPoint(spoint.pose, spoint.vel * per, spoint.acc * per * per, spoint.step, spoint.time);
	fPoint = ControlPoint(fpoint.pose, fpoint.vel * per, fpoint.acc * per * per, fpoint.step, fpoint.time);
	stime = sPoint.step; ftime = fpoint.step;
	vPoint = ControlPoint();  vtime = 0;

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

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, Vec3d sVel, Vec3d fVel, Posed vPose, int vtime, int time, double per) {
	sVel *= per;
	fVel *= per;
	sPoint = ControlPoint(spose, Vec6d(sVel.x, sVel.y, sVel.z, 0, 0, 0), Vec6d(), 0, 0);
	fPoint = ControlPoint(fpose, Vec6d(fVel.x, fVel.y, fVel.z, 0, 0, 0), Vec6d(), time + vtime, 0);
	stime = 0; ftime = time + vtime;
	this->vPoint = ControlPoint(vPose, vtime, 0); this->vtime = vtime;
	for (int i = 0; i < 3; ++i) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel[i];
		if (vtime > 0) {
			A.row(2) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[2][0] = vPoint.pose.Pos()[i];
		}
		else {
			A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
		}
		A.row(3) = Vec6d(1, ftime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel[i];
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;

		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeffToF[n][i] = x[n][0]; }
	}
}

MinJerkTrajectory::MinJerkTrajectory(ControlPoint spoint, ControlPoint fpoint, ControlPoint vpoint, double per) {
	sPoint = ControlPoint(spoint.pose, spoint.vel * per, spoint.acc * per * per, spoint.step, spoint.time);
	fPoint = ControlPoint(fpoint.pose, fpoint.vel * per, fpoint.acc * per * per, fpoint.step, fpoint.time);
	stime = sPoint.step; ftime = fpoint.step;
	this->vPoint = ControlPoint(vpoint.pose, vpoint.vel * per, vpoint.acc * per * per, vpoint.step, vpoint.time); this->vtime = vPoint.step;
	
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
	/*
	for (int i = 0; i < 3; ++i) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPoint.pose.Pos()[i];
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sPoint.vel[i];
		if (vtime > 0) {
			A.row(2) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[2][0] = vPoint.pose.Pos()[i];
		}
		else {
			A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
		}
		A.row(3) = Vec6d(1, stime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPoint.pose.Pos()[i];
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fPoint.vel[i];
		if (vpoint.velControl && vtime > 0) {
			A.row(5) = Vec6d(0, 1, 2 * vtime, 3 * pow(vtime, 2), 4 * pow(vtime, 3), 5 * pow(vtime, 4)); b[5][0] = vPoint.vel[i];
		}
		else {
			A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
		}
		x = A.inv() * b;

		for (int n = 0; n < 6; ++n) { coeff[1][n][i] = x[n][0]; }
	}
	*/
}

Posed MinJerkTrajectory::GetCurrentPose(int t) {
	if (t <= stime) return sPoint.pose;
	if (ftime <= t) return fPoint.pose;

	int st = stime, ft = ftime;
	/*
	if (vtime > 0) {
		int st = (vtime > t) ? stime : vtime;
		int ft = (vtime > t) ? vtime : ftime;
	}
	*/
	double s = (double)(t - st) / (ft - st);
	double sr = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	int r = t;// -stime;
	//DSTR << s << " " << r << std::endl;

	Posed curPose;
	//curPose.Pos() = sPose.Pos() + (fPose.Pos() - sPose.Pos()) * r;
	for (int i = 0; i < 3; i++) {
		curPose.Pos()[i] = (r < vtime ? coeffToV.col(i) : coeffToF.col(i)) * Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5));
	}
	curPose.Ori() = interpolate(sr, sPoint.pose.Ori(), fPoint.pose.Ori());
	return  curPose;
}

Posed MinJerkTrajectory::GetDeltaPose(int t) {
	if (t <= stime) return Posed();
	if (ftime <= t) return fPoint.pose * sPoint.pose.Inv();

	int st = stime, ft = ftime;

	if (vtime > 0) {
		st = (vtime > t) ? stime : vtime;
		ft = (vtime > t) ? vtime : ftime;
	}
	
	double s = (double)(t - stime) / (ftime - stime);
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

Vec6d MinJerkTrajectory::GetCurrentVelocity(int t) {
	Posed curr = GetCurrentPose(t);
	Posed pre = GetCurrentPose(t - 1);
	Vec3d v =  curr.Pos() - pre.Pos();
	return Vec6d(v.x, v.y, v.z, 0, 0, 0);
}

double MinJerkTrajectory::GetCurrentActiveness(int t) {
	double s = (double)(ftime - t) / (ftime - stime);
	double r = 30 * pow(s, 2) - 60 * pow(s, 3) + 30 * pow(s, 4);
	return r;
}

QuaMinJerkTrajectory::QuaMinJerkTrajectory() {

}

QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, int time, double per) {
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
		//amjt[i] = AngleMinJerkTrajectory(sEuler[i], fEuler[i], sv[i], fv[i], 0, 0, ftime, per);
		amjt[i] = AngleMinJerkTrajectory(0, 0, sv[i], fv[i], 0, 0, ftime, per);
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

QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, Vec3d sAcc, Vec3d fAcc, int time, double per) {
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
		//amjt[i] = AngleMinJerkTrajectory(sEuler[i], fEuler[i], sv[i], fv[i], sa[i], fa[i], ftime, per);
		amjt[i] = AngleMinJerkTrajectory(0, 0, sv[i], fv[i], sa[i], fa[i], ftime, per);
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

QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond vqua, int time, int vtime, double per) {
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
		amjt[i] = AngleMinJerkTrajectory(vEuler[i], ftime, vtime, per);
	}
}

QuaMinJerkTrajectory::~QuaMinJerkTrajectory() {
	delete amjt;
}

Quaterniond QuaMinJerkTrajectory::GetCurrentQuaternion(int t) {
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

Quaterniond QuaMinJerkTrajectory::GetDeltaQuaternion(int t) {
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

Vec3d QuaMinJerkTrajectory::GetCurrentVelocity(int t) {
	Quaterniond delta = GetCurrentQuaternion(t + 0.01) * GetCurrentQuaternion(t - 0.01).Inv();
	Vec3d vel = Vec3d();
	delta.ToEuler(vel);
	return Vec3d(vel.z, vel.x, vel.y) * 50;
}
/*
QuaMinJerkTrajectory::QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, int time) {
Quaterniond delta = fqua * squa.Inv();
axis = delta.Axis();
angle = delta.Theta();
sQua = squa; fQua = fqua;
stime = 0; ftime = time;

PTM::TMatrixRow<6, 6, double> A;
PTM::TMatrixRow<6, 1, double> b, x;
A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = 0;
A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = 0;
A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = 0;
A.row(3) = Vec6d(1, stime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = angle;
A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = 0;
A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
x = A.inv() * b;

for (int n = 0; n < 6; ++n) { coeff[n][0] = x[n][0]; }
}

Quaterniond QuaMinJerkTrajectory::GetCurrentQuaternion(int t) {
if (t < stime) {
return sQua;
}if (t > ftime) {
return fQua;
}
double s = (double)(t - stime) / (ftime - stime);
double sr = 0;
for (int i = 0; i < 6; i++) { sr += coeff[i][0] * pow(s, i); }
return sQua * Quaterniond::Rot(sr * angle, axis);
}

Quaterniond QuaMinJerkTrajectory::GetDeltaQuaternion(int t) {
if (t < stime) {
return Quaterniond();
}if (t > ftime) {
return fQua * sQua.Inv();
}
double s = (double)(t - stime) / (ftime - stime);
double sr = 0;
for (int i = 0; i < 6; i++) { sr += coeff[i][0] * pow(s, i); }
return Quaterniond::Rot(sr * angle, axis);
}
*/
AngleMinJerkTrajectory::AngleMinJerkTrajectory() {

}

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double sangle, double fangle, int time) {
	sAngle = sangle; fAngle = fangle;
	stime = 0; ftime = time;
}

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double sangle, double fangle, double sVel, double fVel, double sAcc, double fAcc, int time, double per) {
	sAngle = sangle; fAngle = fangle;
	stime = 0; ftime = time;
	vAngle = sangle; vtime = 0;
	sVel *= per;
	fVel *= per;
	sAcc *= per * per;
	fAcc *= per * per;
	
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

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double sangle, double fangle, double sVel, double fVel, double vangle, double vVel, int time, int vtime, double per) {
	sAngle = sangle; fAngle = fangle;
	stime = 0; ftime = time;
	vAngle = vangle; this->vtime = vtime;
	sVel *= per;
	fVel *= per;
	vVel *= per;

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

AngleMinJerkTrajectory::AngleMinJerkTrajectory(double vangle, int time, int vtime, double per) {
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


double AngleMinJerkTrajectory::GetCurrentAngle(int t) {
	if (t <= stime) {
		return sAngle;
	}if (t >= ftime) {
		return fAngle;
	}
	double s = (double)(t - stime) / (ftime - stime);
	double r = t - stime;
	return (r < vtime ?  coeffToV : coeffToF) * Vec6d(1, r, pow(r, 2), pow(r, 3), pow(r, 4), pow(r, 5));
}

double AngleMinJerkTrajectory::GetDeltaAngle(int t) {
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

double AngleMinJerkTrajectory::GetCurrentVelocity(int t) {
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
	vel = Vec6d();
	time = 0;
	step = 0;
}

ControlPoint::ControlPoint(Posed p, int s, double t) {
	pose = p;
	vel = Vec6d();
	time = t;
	step = s;
	velControl = false;
}
ControlPoint::ControlPoint(Posed p, Vec6d v, Vec6d a, int s, double t) {
	pose = p;
	vel = v;
	acc = a;
	time = t;
	step = s;
	velControl = true;
}
/*
ControlPoint::ControlPoint(ControlPoint& c) {
	this->pose = c.pose;
	this->vel = c.pose;
	this->time = c.time;
	this->velControl = c.velControl;
}
*/
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
void FWTrajectoryPlanner::HingeJoint::Initialize(int iterate, int mtime, int nVia, double rate, bool vCorr) {
	hinge->Enable(true);
	this->iterate = iterate;
	this->movetime = mtime;
	originalSpring = hinge->GetJoint()->GetSpring();
	originalDamper = hinge->GetJoint()->GetDamper();
	torque.resize(mtime, 0);
	torqueLPF.resize(mtime, 0);
	angle.resize(iterate + 1, mtime);
	angleLPF.resize(iterate + 1, mtime);
	angleVels.resize(iterate + 1, mtime);
	angleVelsLPF.resize(iterate + 1, mtime);
	this->rateLPF = rate;
	initialTorque = hinge->GetJoint()->GetMotorForceN(0);
	initialAngle = hinge->GetJoint()->GetPosition();
	initialVel = hinge->GetJoint()->GetVelocity();
	viaAngles.resize(nVia + 1, 0);
	viaVels.resize(nVia + 1, 0);
	viatimes.resize(nVia + 1, 0);
	tChanges.resize(nVia + 1, 0);
	viaCorrect = vCorr;
	CorrTraj.resize(iterate, mtime);
}
void FWTrajectoryPlanner::HingeJoint::MakeJointMinjerk(int cnt) {
	double per = hinge->GetJoint()->GetScene()->GetTimeStep();
	double perInv = hinge->GetJoint()->GetScene()->GetTimeStepInv();
	AngleMinJerkTrajectory hmjt = AngleMinJerkTrajectory(initialAngle, targetAngle, initialVel, targetVel, 0, 0, movetime, per);
	for (int j = 0; j < movetime; j++) {
		angle[cnt][j] = hmjt.GetCurrentAngle(j + 1);
		angleVels[cnt][j] = hmjt.GetCurrentVelocity(j + 1) * perInv;
	}
	for (int j = 0; j < viatimes.size() - 1; j++) {
		int st = (j == 0) ? 0 : viatimes[j - 1];
		double v = viaAngles[j] - angle[0][viatimes[j] - 1];
		int t = movetime - st;
		int vt = viatimes[j] - st;
		AngleMinJerkTrajectory dhmjt = AngleMinJerkTrajectory(v, t, vt, per);
		for (int k = 0; k < t; k++) {
			angle[cnt][st + k] += dhmjt.GetCurrentAngle(k + 1);
			//dhmjt.GetCurrentVelocity(k + 1) * perInv;
		}
	}
	/*/
	for (int j = 0; j < (int)viaPoints.size() + 1; j++) {
	double s = (j == 0) ? hj->initialAngle : hj->viaAngles[j - 1];
	double vs = (j == 0) ? hj->initialVel : hj->viaVels[j - 1];
	double f = hj->viaAngles[j];
	double vf = hj->viaVels[j];
	//double f = (j == (int)viaPoints.size()) ? hj->targetAngle : hj->viaAngles[j];
	//double vf = (j == (int)viaPoints.size()) ? hj->targetVel : hj->viaVels[j];
	int st = (j == 0 ? 0 : viaPoints[j - 1].step);
	int time = (j == (int)viaPoints.size() ? movtime : viaPoints[j].step) - st;
	DSTR << s << " " << vs << " " << f << " " << vf << " " << st << " " << time << std::endl;
	AngleMinJerkTrajectory mjt = AngleMinJerkTrajectory(s, f, vs, vf, 0, 0, time, scene->GetTimeStep());
	for (int k = 0; k < time; k++) {
	hj->angle[0][k + st] = mjt.GetCurrentAngle(k + 1);
	DSTR << k + st << std::endl;
	}
	}
	*/
}
void FWTrajectoryPlanner::HingeJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void FWTrajectoryPlanner::HingeJoint::SaveTorque(int n) {
	/*
	//limitにかかってたらsavetorqueを0に
	PH1DJointLimitIf* limit = hinge->GetJoint()->GetLimit();
	if (limit) {
		if (limit->IsOnLimit()) {
			torque[n] = 0;
			torqueLPF[n] = 0;
			return;
		}
	}
	*/
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
		if (hinge->GetJoint()->GetLimit()) {
			Vec2d range;
			hinge->GetJoint()->GetLimit()->GetRange(range);
			if (range[0] < range[1] && (range[0] > angle[k][n] || range[1] < angle[k][n])) {
				if (range[0] > angle[k][n]) {
					hinge->GetJoint()->SetTargetPosition(range[0]);
				}
				else {
					hinge->GetJoint()->SetTargetPosition(range[1]);
				}
			}
			else {
				hinge->GetJoint()->SetTargetPosition(angle[k][n]);
			}
		}
		else {
			hinge->GetJoint()->SetTargetPosition(angle[k][n]);
		}
	}
	int s = movetime * 0.1;
	if (k == 0 && n < s) {
		double r = (double)n / s;
		//hinge->GetJoint()->SetTargetPosition(r * angle[0][n] + (1 - r) * initialAngle);
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
void FWTrajectoryPlanner::HingeJoint::ResetOffset() {
	int nMotors = hinge->GetJoint()->NMotors();
	for (int i = 0; i < nMotors; i++) {
		hinge->GetJoint()->SetOffsetForceN(i, 0);
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
void FWTrajectoryPlanner::HingeJoint::SaveVelocityForwardEnd() {
	velBeforeEnd = hinge->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::HingeJoint::TrajectoryCorrection(int k, bool s) {
	double end;
	int time;
	double per = hinge->GetJoint()->GetScene()->GetTimeStep();
	double perInv = hinge->GetJoint()->GetScene()->GetTimeStepInv();

	//終端の到達および停止保証部
	int stime = (int)(movetime * 0.0);
	//end = angleLPF[k][stime];
	end = angleLPF[k][angleLPF.width() - 1];
	double acc = (angleVelsLPF[k][angleVelsLPF.width() - 1] - angleVelsLPF[k][angleVelsLPF.width() - 2]) * perInv;
	time = movetime;
	AngleMinJerkTrajectory delta = AngleMinJerkTrajectory(0, targetAngle - end, 0, -velBeforeEnd, 0, -acc, time - stime, per);
	DSTR << "target:" << targetAngle << " actual:" << delta.GetCurrentAngle(movetime) << std::endl;
	for (int i = 0; i < time - stime; i++) {
		angle[k][i + stime] = delta.GetDeltaAngle(i + 1) + angleLPF[k][i + stime]; //k+1でいいのか？
		angleLPF[k][i + stime] += delta.GetDeltaAngle(i + 1);
		//angleVels[k][i + stime] = delta.GetCurrentVelocity(i + 1) * perInv + angleVelsLPF[k][i + stime];
		angleVels[k][i + stime] = (angle[k][i + stime] - (i + stime > 0 ? angle[k][i + stime - 1] : initialAngle)) * perInv;
		angleVelsLPF[k][i + stime] += delta.GetCurrentVelocity(i + 1) * perInv;
		//CorrTraj[k - 1][i + stime] = delta.GetCurrentVelocity(i);
	}
	
	//各経由点の通過保証
	double start = 0;
	int last = 0;
	if (viaCorrect) {
		/*
		for (int i = 0; i < viaAngles.size(); i++) {
			end = viaAngles[i] - angleLPF[k][viatimes[i] - 1];
			int starttime = ((i == 0) ? 0 : viatimes[i - 1]);
			time = viatimes[i] - starttime;
			AngleMinJerkTrajectory d = AngleMinJerkTrajectory(start, end, 0, 0, 0, 0, time, per);
			for (int j = 0; j < time; j++) {
				angle[k][starttime + j] = d.GetCurrentAngle(j + 1) + angleLPF[k][starttime + j]; //d.GetCurrentAngle(j?)
				CorrTraj[k - 1][starttime + j] += d.GetCurrentVelocity(j);
			}
			start = end;
			last = viatimes[i];
		}
		/*/
		for (int i = 0; i < viaAngles.size() - 1; i++) {
			int st = (i == 0) ? 0 : viatimes[i - 1];
			double v = viaAngles[i] - angleLPF[k][viatimes[i] - 1];
			int t = movetime - st;
			int vt = viatimes[i] - st;
			AngleMinJerkTrajectory dhmjt = AngleMinJerkTrajectory(v, t, vt, per);
			for (int j = 0; j < t; j++) {
				angle[k][st + j] += dhmjt.GetCurrentAngle(j + 1);
			}
		}
		
	}
	/*
	end = targetAngle - angleLPF[k][angleLPF.width() - 1];
	time = movetime - last;
	AngleMinJerkTrajectory delta = AngleMinJerkTrajectory(start, end, 0, -vel, 0, 0, time, per);
	DSTR << "target:" << targetAngle << " actual:" << delta.GetCurrentAngle(movetime) << std::endl;
	for (int i = 0; i < time; i++) {
		angle[k][i] = delta.GetCurrentAngle(i + 1) + angleLPF[k][last + i]; //k+1でいいのか？
	}
	*/
	
}
void FWTrajectoryPlanner::HingeJoint::ApplyLPF(int lpf, int count) {
	//torqueLPF = LPF::weightedv(torque, initialTorque, weight, rateLPF);
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torqueLPF, 3, 1, initialTorque);
	}
	if (count > 0) {
		double s = 1.0;
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = s * torqueLPF[i] + (1 - s) * initialTorque;
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
	DSTR << "BallJoint Class Object is destroyed" << std::endl;
	CloseFile();
	delete torGraph;
	delete torChangeGraph;
}
void FWTrajectoryPlanner::BallJoint::Initialize(int iterate, int mtime, int nVia, double rate, bool vCorr) {
	ball->Enable(true);
	this->iterate = iterate;
	this->movetime = mtime;
	torque.resize(mtime, Vec3d());
	torqueLPF.resize(mtime, Vec3d());
	ori.resize(iterate + 1, mtime);
	oriLPF.resize(iterate + 1, mtime);
	oriVels.resize(iterate + 1, mtime);
	oriVelsLPF.resize(iterate + 1, mtime);
	this->rateLPF = rate;
	initialTorque = ball->GetJoint()->GetMotorForceN(0);
	initialOri = ball->GetJoint()->GetPosition();
	initialVel = ball->GetJoint()->GetVelocity();
	originalSpring = ball->GetJoint()->GetSpring();
	originalDamper = ball->GetJoint()->GetDamper();
	viaOris.resize(nVia + 1, Quaterniond());
	viaVels.resize(nVia + 1, Vec3d());
	viatimes.resize(nVia + 1, 0);
	tChanges.resize(nVia + 1, 0);
	viaCorrect = vCorr;
}
void FWTrajectoryPlanner::BallJoint::MakeJointMinjerk(int cnt) {
	double per = ball->GetJoint()->GetScene()->GetTimeStep();
	double perInv = ball->GetJoint()->GetScene()->GetTimeStepInv();
	QuaMinJerkTrajectory qmjt = QuaMinJerkTrajectory(initialOri, targetOri, initialVel, targetVel, Vec3d(), Vec3d(), movetime, per);
	for (int j = 0; j < movetime; j++) {
		ori[cnt][j] = qmjt.GetCurrentQuaternion(j + 1);
		oriVels[cnt][j] = qmjt.GetCurrentVelocity(j + 1) * perInv;
	}
	for (int j = 0; j < viaOris.size() - 1; j++) {
		int st = (j == 0) ? 0 : viatimes[j - 1];
		Quaterniond v = viaOris[j] * ori[0][viatimes[j] - 1].Inv();
		int t = movetime - st;
		int vt = viatimes[j] - st;
		QuaMinJerkTrajectory dqmjt = QuaMinJerkTrajectory(v, t, vt, per);
		for (int k = 0; k < t; k++) {
			ori[cnt][st + k] = ori[0][st + k] * dqmjt.GetCurrentQuaternion(k + 1);
			//dqmjt.GetCurrerntVelocity(k + 1) * perInv;
		}
	}
	/*/
	for (int j = 0; j < viaPoints.size() + 1; j++) {
	Quaterniond s = (j == 0) ? bj->initialOri : bj->viaOris[j - 1];
	Vec3d vs = (j == 0) ? bj->initialVel : bj->viaVels[j - 1];
	Quaterniond f = bj->viaOris[j];
	Vec3d vf = bj->viaVels[j];
	//Quaterniond f = (j == viaPoints.size()) ? bj->targetOri : bj->viaOris[j];
	//Quaterniond vf = (j == viaPoints.size()) ? bj->targetVel : bj->viaVels[j];
	int st = (j == 0 ? 0 : viaPoints[j - 1].step);
	int time = (j == viaPoints.size() ? movtime : viaPoints[j].step) - st;
	DSTR << s << " " << vs << " " << f << " " << vf << " " << st << " " << time << std::endl;
	QuaMinJerkTrajectory qmjt = QuaMinJerkTrajectory(s, f, vs, vf, Vec3d(), Vec3d(), time, scene->GetTimeStep());
	for (int k = 0; k < time; k++) {
	bj->ori[0][k + st] = qmjt.GetCurrentQuaternion(k + 1);
	DSTR << k + st << std::endl;
	}
	}
	*/
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
		int s = movetime * 0.1;
		double r = (double)n / s;
		if (k == 0 && n < s) {
			//ball->GetJoint()->SetTargetPosition(interpolate(r, initialOri, ori[0][n]));
		}
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
void FWTrajectoryPlanner::BallJoint::ResetOffset() {
	int nMotors = ball->GetJoint()->NMotors();
	for (int i = 0; i < nMotors; i++) {
		ball->GetJoint()->SetOffsetForceN(i, Vec3d(0, 0, 0));
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
void FWTrajectoryPlanner::BallJoint::SaveVelocityForwardEnd() {
	velBeforeEnd = ball->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::BallJoint::TrajectoryCorrection(int k, bool s) {
	//終端条件保証
	Quaterniond end;
	int time;
	double per = ball->GetJoint()->GetScene()->GetTimeStep();
	double perInv = ball->GetJoint()->GetScene()->GetTimeStepInv();
	int stime = (int)(movetime * 0.0);
	end = oriLPF[k][oriLPF.width() - 1];
	Vec3d acc = (oriVelsLPF[k][oriVelsLPF.width() - 1] - oriVelsLPF[k][oriVelsLPF.width() - 2]) * perInv;
	Quaterniond half = oriLPF[k][stime];
	time = movetime;
	QuaMinJerkTrajectory delta = QuaMinJerkTrajectory(Quaterniond(), targetOri * end.Inv(), Vec3d(), -velBeforeEnd, Vec3d(), -acc, time - stime, per);
	
	DSTR << "target:" << targetOri * end.Inv() << " actual:" << delta.GetCurrentQuaternion(movetime) << std::endl;
	PHBallJointConeLimitIf* limit = DCAST(PHBallJointIf, ball->GetJoint())->GetLimit()->Cast();
	Vec2d range;
	Vec3d limitDir;
	Vec3d   dir;
	Vec3d  axis;
	if (limit) {
		limit->GetSwingRange(range);
		limitDir = limit->GetLimitDir();
	}
	for (int i = 0; i < movetime - stime; i++) {
		ori[k][i + stime] = delta.GetCurrentQuaternion(i + 1) * oriLPF[k][i + stime];
		//oriVels[k][i + stime] = delta.GetCurrentVelocity(i + 1) * perInv + oriVelsLPF[k][i + stime];
		//oriVels[k][i + stime] = (ori[k][i + stime] * (i + stime > 0 ? ori[k][i + stime - 1] : initialOri).Inv()).ToEuler() * perInv;
		oriVels[k][i + stime] = ori[k][i + stime].AngularVelocity(ori[k][i + stime] - (i + stime > 0 ? ori[k][i + stime - 1] : initialOri)) * perInv;
		/*
		//limit補正
		if (limit) {
			dir = (ori[k][i + stime] * Vec3d(0, 0, 1)).unit();
			axis = PTM::cross(limitDir, dir);
			if (axis.norm() > 1e-5) {
				axis.unitize();
				double angle = acos(PTM::dot(limitDir, dir)) + 0.05;

				if (range[1] <= angle) {
					Quaterniond pullback = Quaterniond::Rot(axis * (range[1] - angle));
					ori[k][i + stime] = pullback * ori[k][i + stime];
					oriVels[k][i + stime] = Vec3d();
				}
			}
		}
		*/
		oriLPF[k][i + stime] = ori[k][i + stime];
		oriVelsLPF[k][i + stime] = oriVels[k][i + stime];
	}
	//通過点保証
	Quaterniond start = Quaterniond();
	int last = 0;
	if (viaCorrect) {
		/*
		for (int i = 0; i < viaOris.size(); i++) {
			end = viaOris[i] * oriLPF[k][viatimes[i] - 1].Inv();
			int starttime = ((i == 0) ? 0 : viatimes[i - 1]);
			time = viatimes[i] - starttime;
			QuaMinJerkTrajectory d = QuaMinJerkTrajectory(start, end, Vec3d(), Vec3d(), Vec3d(), Vec3d(), time, per);
			for (int j = 0; j < time; j++) {
				ori[k][starttime + j] = oriLPF[k][starttime + j] * d.GetCurrentQuaternion(j + 1);
			}
			start = end;
			last = viatimes[i];
		}
		/*/
		for (int i = 0; i < viaOris.size() - 1; i++) {
			int st = (i == 0) ? 0 : viatimes[i - 1];
			Quaterniond v = viaOris[i] * oriLPF[k][viatimes[i] - 1].Inv();
			int t = movetime - st;
			int vt = viatimes[i] - st;
			QuaMinJerkTrajectory dhmjt = QuaMinJerkTrajectory(v, t, vt, per);
			for (int j = 0; j < t; j++) {
				ori[k][st + j] = dhmjt.GetCurrentQuaternion(j + 1) * oriLPF[k][st + j];
			}
		}

	}
	/*
	for (int i = 0; i < viatimes.size(); i++) {
		ori[k][viatimes[i] - 1] = viaOris[i];
	}
	*/
	/*
	end = targetOri - oriLPF[k][oriLPF.width() - 1];
	time = movetime - last;
	QuaMinJerkTrajectory delta = QuaMinJerkTrajectory(start, end, 0, -vel, 0, 0, time, per);
	DSTR << "target:" << targetOri << " actual:" << delta.GetCurrentQuaternion(movetime) << std::endl;
	for (int i = 0; i < time; i++) {
	ori[k][i] = oriLPF[k][last + i] * delta.GetCurrentQuaternion(i + 1);
	}
	*/
}
void FWTrajectoryPlanner::BallJoint::ApplyLPF(int lpf, int count) {
	//torqueLPF = LPF::weightedv(torque, initialTorque, weight, rateLPF);
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torqueLPF, 3, 1, initialTorque);
	}
	if (count > 0) {
		double s = 1.0;
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = s * torqueLPF[i] + (1 - s) * initialTorque;
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
		ball->GetJoint()->SetDamper(originalDamper * hardenDamper * 0);
	}
	else {
		ball->GetJoint()->SetSpring(hardenSpring);
		ball->GetJoint()->SetDamper(hardenDamper * 0);
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
FWTrajectoryPlanner::Joints::~Joints() { std::vector<Joint*>().swap(joints); }
void FWTrajectoryPlanner::Joints::RemoveAll() {
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
void FWTrajectoryPlanner::Joints::Initialize(int iterate, int movetime, int nVia, double rate, bool vCorr) {
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

void FWTrajectoryPlanner::Joints::ResetOffset() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ResetOffset();
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
void FWTrajectoryPlanner::Joints::SaveVelocityForwardEnd() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveVelocityForwardEnd();
	}
}

void FWTrajectoryPlanner::Joints::TrajectoryCorrection(int k, bool s) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->TrajectoryCorrection(k, s);
	}
}

void FWTrajectoryPlanner::Joints::ApplyLPF(int lpf, int count) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ApplyLPF(lpf, count);
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
static PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
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
static PTM::VVector<T> FWTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
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
static PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
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
static PTM::VVector<T> FWTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
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

FWTrajectoryPlanner::FWTrajectoryPlanner(int d, int i, int iv, double f, double q, int n, double mg, int c, bool wf, bool snc, double r, double vRate, bool sc) {
	this->depth = d;
	this->iterate = i;
	this->iterateViaAdjust = iv;
	this->freq = f;
	this->qvalue = q;
	this->nsma = n;
	this->mag = mg;
	this->correction = c;
	this->rate = r;
	this->viaAdjustRate = vRate;
	this->waitFlag = wf;
	this->saveNotCorrected = snc;
	this->springCor = sc;
}

void FWTrajectoryPlanner::Init() {

	//とりあえず全エンドエフェクタを無効化
	//計算に使うのだけはCheckAndSetJoints内で有効化
	int nEEF = scene->NIKEndEffectors();
	for (int i = 0; i < nEEF; i++) {
		//scene->GetIKEndEffector(i)->Enable(false);
	}
	/*
	int nJoints = scene->NJoints();
	for (int i = 0; i < nJoints; i++) {
		if (DCAST(PHHingeJointIf, scene->GetJoint(i))) {
			DCAST(PHHingeJointIf, scene->GetJoint(i))->SetSpring(spring);
			DCAST(PHHingeJointIf, scene->GetJoint(i))->SetDamper(1e20);
		}
		if (DCAST(PHBallJointIf, scene->GetJoint(i))) {
			DCAST(PHBallJointIf, scene->GetJoint(i))->SetSpring(spring);
			DCAST(PHBallJointIf, scene->GetJoint(i))->SetDamper(1e20);
		}
	}
	 */
	int nAct = scene->NIKActuators();
	for (int i = 0; i < nAct; i++) {
		//scene->GetIKActuator(i)->Enable(false);
	}

	CheckAndSetJoints();

	eef->SetTargetPosition(eef->GetSolid()->GetPose().Pos());
	eef->SetTargetVelocity(Vec3d());
	/*
	for (int i = 0; i < 10; i++) {
		scene->Step();
	}
	*/
	//jointsの初期化
	joints.Initialize(iterate, movtime, (int)viaPoints.size(), rate, viaCorrect);
	joints.SetWeight();
	/*
	int cnt = 0;
	for (int i = 0; i < (int)joints.balls.size(); i++) {
		joints.balls[i].weight = weights[cnt];
		cnt++;
	}
	for (int i = 0; i < (int)joints.hinges.size(); i++) {
		joints.hinges[i].weight = weights[cnt];
		cnt++;
	}
	*/
	joints.SetPD(spring, damper, mul);

	//手先軌道リサイズ
	trajData.resize(iterate + 1, movtime);
	trajDataNotCorrected.resize(iterate, movtime);
	trajVel.resize(iterate + 1, movtime + 1);
	trajVelNotCorrected.resize(iterate, movtime + 1);

	//stateの保存
	states = ObjectStatesIf::Create();
	cstates = ObjectStatesIf::Create();
	tmpstates.clear();
	for (int i = 0; i < (int)viaPoints.size(); i++) {
		tmpstates.push_back(ObjectStatesIf::Create());
	}
	for (int i = 0; i < iterate; i++) {
		corstates.push_back(ObjectStatesIf::Create());
		beforecorstates.push_back(ObjectStatesIf::Create());
	}
	DSTR << scene->GetCount() << std::endl;
	DSTR << "Saved" << std::endl;
	Debug();
	states->SaveState(scene);

	//計算前フラグ
	moving = false;
	calced = false;
}

void FWTrajectoryPlanner::Init(int d, int i, int iv, int n, double mg, int c, bool wf, bool snc, double r, double vRate, bool vCorr, bool sc) {
	Reset(d, i, iv, n, mg, c, wf, snc, r, vRate, vCorr, sc);
	Init();
}

void FWTrajectoryPlanner::CheckAndSetJoints() {
	PHIKActuatorIf* ika;
	ika = eef->GetParentActuator();
	eef->Enable(true);
	//depthがオーバーしてないかチェック
	for (int i = 0; i < depth; i++) {
		if (ika->GetParent()) {
			ika = ika->GetParent();
		}
		else {
			depth = i + 1;
			break;
		}
	}
	ika = eef->GetParentActuator();

	//関節をjointsに投げ込む
	joints.RemoveAll();
	for (int i = 0; i < depth; i++) {
		joints.Add(ika, path);
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

void FWTrajectoryPlanner::SetControlTarget(PHIKEndEffectorIf* e) { this->eef = e; };

void FWTrajectoryPlanner::SetScene(PHSceneIf* s) { this->scene = s; };

void FWTrajectoryPlanner::AddControlPoint(ControlPoint c) { viaPoints.push_back(c); };

void JointRecalcFromIterateN(Posed tPose, double mt, int LPFmode, int smoothCount, std::string output, bool bChange = false, bool pChange = false, bool staticTarget = false, bool jmjt = false) {

}

void FWTrajectoryPlanner::RecalcFromIterationN(int n) {

}

void FWTrajectoryPlanner::JointTrajStep(bool step) {
	if (moving) {   //軌道再生中
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
			moving = false;
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
		Debug();
	}
	DSTR << std::endl;
}

void FWTrajectoryPlanner::Debug() {   //Debug用にいろいろ表示する
	int nsolids = scene->NSolids();
	for (int i = 0; i < nsolids; i++) {
		PHSolidIf* so = scene->GetSolids()[i];
		DSTR << "Solid" << i << " mass:" << so->GetMass() << ": pose" << so->GetPose() << " velocity" << so->GetVelocity() << " force" << so->GetForce() << " torque" << so->GetTorque() << std::endl;
	}
	joints.ShowInfo();
	DSTR << "EndeffectorPosition:" << eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() << std::endl;
	DSTR << "EndeffectorRotation:" << eef->GetSolid()->GetPose().Ori() << std::endl;
	DSTR << "EndEffectorVelocity:" << eef->GetSolid()->GetVelocity() << std::endl;
	DSTR << "EndEffectorTarget:" << eef->GetTargetPosition() << std::endl;
	DSTR << "Gravity:" << scene->GetGravity() << std::endl << std::endl;
}

void FWTrajectoryPlanner::PrepareOutputFilename(std::string& filename) {
	//日時からファイル名用の文字列を生成
	time_t t = time(NULL);
	struct tm *pnow = localtime(&t);
	std::stringstream ss;
	ss << pnow->tm_year + 1900;

	if ((pnow->tm_mon + 1) < 10) {
		ss << "0" << pnow->tm_mon + 1;
	}
	else {
		ss << pnow->tm_mon + 1;
	}

	if (pnow->tm_mday < 10) {
		ss << "0" << pnow->tm_mday;
	}
	else {
		ss << pnow->tm_mday;
	}

	if (pnow->tm_hour < 10) {
		ss << "0" << pnow->tm_hour;
	}
	else {
		ss << pnow->tm_hour;
	}

	if (pnow->tm_min < 10) {
		ss << "0" << pnow->tm_min;
	}
	else {
		ss << pnow->tm_min;
	}

	if (pnow->tm_sec < 10) {
		ss << "0" << pnow->tm_sec;
	}
	else {
		ss << pnow->tm_sec;
	}
	ss >> filename;
}

void FWTrajectoryPlanner::OutputTrajectory(std::string filename) {
	//とりあえず可視化しやすい手先軌道を出力して軌道可視化に使う
	std::ofstream outfile(filename + ".csv");
	for (int i = 0; i < iterate + 1; i++) {
		for (int j = 0; j < trajData.width(); j++) {
			outfile << trajData[i][j].Pos().x << "," << trajData[i][j].Pos().y << "," << trajData[i][j].Pos().z << "," << std::endl;
		}
	}
}

void FWTrajectoryPlanner::OutputNotCorrectedTrajectory(std::string filename) {
	//とりあえず可視化しやすい手先軌道を出力して軌道可視化に使う
	std::ofstream outfile(filename + "NotCorrected.csv");
	for (int i = 0; i < iterate; i++) {
		for (int j = 0; j < movtime; j++) {
			outfile << trajDataNotCorrected[i][j].Pos().x << "," << trajDataNotCorrected[i][j].Pos().y << "," << trajDataNotCorrected[i][j].Pos().z << "," << std::endl;
		}
	}
}

void FWTrajectoryPlanner::OutputVelocity(std::string filename) {
	//手先剛体の速度のcsv出力
	std::ofstream outfile(filename + "Velocity.csv");
	for (int i = 0; i < iterate + 1; i++) {
		for (int j = 0; j < movtime + 1; j++) {
			outfile << trajVel[i][j].x << "," << trajVel[i][j].y << "," << trajVel[i][j].z << "," << trajVel[i][j].w << ",";
			outfile << std::endl;
		}
	}
	std::ofstream outfile2(filename + "VelocityNotCorrected.csv");
	for (int i = 0; i < iterate; i++) {
		for (int j = 0; j < movtime + 1; j++) {
			outfile2 << trajVelNotCorrected[i][j].x << "," << trajVelNotCorrected[i][j].y << "," << trajVelNotCorrected[i][j].z << "," << trajVelNotCorrected[i][j].w << "," << std::endl;
		}
	}
}

void FWTrajectoryPlanner::LoadScene() {
	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	DSTR << "Loaded" << std::endl;
	Debug();
	eef->SetTargetPosition(eef->GetSolid()->GetPose().Pos());
	for (int i = 0; i < 10; i++) {
		scene->Step();
		DSTR << "after" << i + 1 << "step" << std::endl;
		Debug();
	}
}

void FWTrajectoryPlanner::SetPD(double s, double d, bool mul) {
	this->spring = s;
	this->damper = d;
	this->mul = mul;
}

void FWTrajectoryPlanner::Replay(int ite, bool noncorrected) {
	if (!calced) return;
	if (ite < 0 || ite > iterate) return;

	//要求された再生番号を設定
	this->ite = ite;
	noncorrectedReplay = noncorrected;
	moving = true;
	torquechange.clear();
	joints.SetTargetInitial();
	DSTR << "Before replay load" << std::endl;
	Debug();
	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	DSTR << "After replay load" << std::endl;
	Debug();
	repCount = -50;
}
void FWTrajectoryPlanner::FIRM(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt) {
	// --- デバッグ用の出力ファイル準備 ---
	std::ofstream torqueChangeOutput(output + "TorqueChange.csv");

	//途中停止用のフラグを無効化
	stop = false;
	
	//躍度最小軌道の生成とトルクの保存
	MakeMinJerkAll();
	besttorque = joints.CalcTotalTorqueChange(torqueChangeOutput);
	torqueChangeOutput << besttorque << std::endl;
	joints.OutputTorque();

	//IKのバイアスとプルバックを弱く
	//joints.UpdateIKParam(1, 0);
	int nIK = scene->NIKActuators();
	if (bChange) {
		for (int i = 0; i < nIK; i++) {
			scene->GetIKActuator(i)->SetBias(1);
		}
	}
	if (pChange) {
		for (int i = 0; i < nIK; i++) {
			scene->GetIKActuator(i)->SetPullbackRate(0);
		}
	}

	// --- 軌道計算の繰り返し計算 ---

	scene->GetIKEngine()->Enable(false);

	double torqueChange;
	double torqueChangeLPF;

	int max = jointMJT ? iterate + 1 : iterate;
	for (int k = 1; k < max && !stop; k++) {
		
			//トルクにLPFをかける
			double level = 1 + (double)(iterate - k) / iterate;
			joints.ApplyLPF(LPFmode, smoothCount * level);

			//フォワード(トルクー＞軌道)
			Forward(k);

			torqueChangeLPF = joints.CalcTotalTorqueChangeLPF();

			//軌道の修正
			if (correction == 1) {
				Correction(k);
			}
		
		//インバース(軌道ー＞トルク)
		Inverse(k);

		torqueChange = joints.CalcTotalTorqueChange(torqueChangeOutput);
		torqueChangeOutput << torqueChange << "," << torqueChangeLPF << std::endl;
		joints.OutputTorque();
		//springdamper小さくする
	}

	if (!jointMJT) {
		//インバース(軌道ー＞トルク)
		Inverse(iterate);

		torqueChange = joints.CalcTotalTorqueChange(torqueChangeOutput);
		torqueChangeOutput << torqueChange << "," << torqueChangeLPF << std::endl;
		joints.OutputTorque();
		//springdamper小さくする
	}

	//scene->GetIKEngine()->Enable(true);

	// --- 計算後であることのフラグ有効化 ---
	moving = true;
	calced = true;
	ite = best;
	repCount = 0;

	//joints.ResetPD();

	//いろいろとファイル出力
	OutputTrajectory(output);
	if (saveNotCorrected) {
		OutputNotCorrectedTrajectory(output);
	}
	OutputVelocity(output);

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();

	int nJoints = scene->NJoints();
	for (int i = 0; i < nJoints; i++) {
		PHJointIf* jo = scene->GetJoint(i);
		if (DCAST(PHBallJointIf, jo)) {
			DCAST(PHBallJointIf, jo)->SetTargetVelocity(Vec3d());
		}
		if (DCAST(PHHingeJointIf, jo)) {
			DCAST(PHHingeJointIf, jo)->SetTargetVelocity(0);
		}
	}
}

bool FWTrajectoryPlanner::ViatimeAdjustment() {
	if (viaPoints.empty()) return false;
	//トルクの総変化量と各セクションでの変化量を取得
	int nVia = (int)viaPoints.size();
	double* tChange = new double[nVia + 1];
	for (int i = 0; i < nVia + 1; i++) {
		tChange[i] = joints.GetBestTorqueChangeInSection(i);
		DSTR << tChange[i] << std::endl;
	}
	DSTR << std::endl;

	//現在の各区間の時間を求める
	double* ti = new double[nVia + 1];
	for (int i = 0; i < nVia + 1; i++) {
		ti[i] = (i == nVia ? movtime * scene->GetTimeStep() : viaPoints[i].time) - (i == 0 ? 0 : viaPoints[i - 1].time);
		DSTR << ti[i] << std::endl;
	}

	//deltatを求める
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

	//各経由点到達時間を更新
	double sumtime = 0;
	bool cont = true;
	for (int i = 0; i < nVia; i++) {
		sumtime += deltat[i];
		double newtime = (viaPoints[i].time + sumtime) * movtime * scene->GetTimeStep() / (movtime * scene->GetTimeStep() + sumdeltat);
		int newstep = TimeToStep(newtime);
		cont &= (newstep == viaPoints[i].step);
		viaPoints[i].time = newtime;
		viaPoints[i].step = newstep;
		DSTR << "new viatime " << i << " : " << viaPoints[i].time << std::endl;
	}

	delete tChange;
	delete ti;
	delete deltat;

	return !cont;
}

void FWTrajectoryPlanner::ViatimeInitialize() {
	//各経由点の通過時間を等間隔に初期化
	int nVia = (int)viaPoints.size();
	for (int i = 0; i < nVia; i++) {
		viaPoints[i].time = movtime * scene->GetTimeStep() * (i + 1) / (nVia + 1);
		viaPoints[i].step = TimeToStep(viaPoints[i].time);
	}
}

void FWTrajectoryPlanner::MakeMinJerkAll() {
	//開始点と目標点から空間躍度最小軌道を作成
	MinJerkTrajectory minjerk = MinJerkTrajectory(startPoint, targetPoint, scene->GetTimeStep());
	PTM::VVector<Posed> minjerkPoses;
	minjerkPoses.resize(movtime);
	PTM::VVector<Vec6d> minjerkVels;
	minjerkVels.resize(movtime);
	for (int i = 0; i < movtime; i++) {
		minjerkPoses[i] = minjerk.GetCurrentPose(i + 1);
		//minjerkVels[i] = minjerk.GetCurrentVelocity(i + 1);
	}
	/*
	//各点を単純な躍度最小軌道で接続
	for (int i = 0; i < (int)viaPoints.size() + 1; i++) {
		ControlPoint s = (i == 0) ? startPoint : viaPoints[i - 1];
		ControlPoint f = (i == viaPoints.size()) ? targetPoint : viaPoints[i];
		MinJerkTrajectory mjt = MinJerkTrajectory(s, f, scene->GetTimeStep());
		for (int j = s.step; j < f.step; j++) {
			minjerkPoses[j] = mjt.GetCurrentPose(j + 1);
		}
	}
	/*/
	//境界条件満足の部分軌道を足し合わせて経由点を通る軌道実現
	for (int i = 0; i < (int)viaPoints.size(); i++) {
		ControlPoint s = ControlPoint(Posed(), Vec6d(), Vec6d(), (i == 0) ? startPoint.step : viaPoints[i - 1].step, (i == 0) ? startPoint.time : viaPoints[i - 1].time);
		ControlPoint f = ControlPoint(Posed(), Vec6d(), Vec6d(), movtime, targetPoint.time);
		ControlPoint c = viaPoints[i];
		c.pose.Pos() -= minjerkPoses[c.step - 1].Pos();
		c.pose.Ori() = c.pose.Ori() * minjerkPoses[c.step - 1].Ori().Inv();
		double internalRatio = (double)(c.time - s.time) / (f.time - s.time);
		Vec3d relvpos = c.pose.Pos() - ((1 - internalRatio) * s.pose.Pos() + internalRatio * f.pose.Pos());
		double tvs = c.time - s.time;
		double tfv = f.time - c.time;
		Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * c.pose.Pos();
		Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * c.pose.Pos();
		c.vel = Vec6d(relvvel.x, relvvel.y, relvvel.z, 0, 0, 0) - minjerkVels[c.step - 1] * scene->GetTimeStepInv();
		c.acc = Vec6d(relvacc.x, relvacc.y, relvacc.z, 0, 0, 0) - (minjerkVels[c.step - 1] - (c.step > 1 ? minjerkVels[c.step - 2] : Vec6d())) * scene->GetTimeStepInv();
		MinJerkTrajectory mjt = MinJerkTrajectory(s, f, c, scene->GetTimeStep());
		for (int j = s.step; j < f.step; j++) {
			Posed delta = mjt.GetCurrentPose(j + 1);
			minjerkPoses[j].Pos() += delta.Pos();
			minjerkPoses[j].Ori() = minjerkPoses[j].Ori() * delta.Ori();
			minjerkVels[j] += mjt.GetCurrentVelocity(j + 1);
		}
	}
	
	joints.Harden();

	//states->LoadState(scene);
	scene->GetIKEngine()->Enable(true);
	//eef->Enable(true);
	//scene->GetIKEngine()->ApplyExactState();
	Debug();

	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	for (int i = 0; i < iterate + 1; i++) {
		trajVel[i][0] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}
	for (int i = 0; i < iterate; i++) {
		trajVelNotCorrected[i][0] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}

	int reach = 0;
	int count = 0;
	Posed minjerkTarget;
	//空間躍度最小軌道を再生して関節角を保存
	for (; reach < movtime; reach++) {
		 minjerkTarget = minjerkPoses[reach];
		eef->SetTargetPosition(minjerkTarget.Pos());
		//eef->SetTargetOrientation(minjerkTarget.Ori());
		eef->SetTargetVelocity((minjerkPoses[reach].Pos() - (reach == 0 ? startPoint.pose.Pos() : minjerkPoses[reach - 1].Pos())) * scene->GetTimeStepInv());
		for (int i = 0; i < 1; i++) {
			scene->Step();
		}
		if (count < (int)viaPoints.size()) {
			if (reach == (viaPoints[count].step - 1)) {
				joints.SaveViaPoint(count, (viaPoints[count].step));
				count++;
			}
		}
		Debug();
		for (int i = 0; i < iterate + 1; i++) {
			joints.SavePosition(i, reach);
			joints.SaveVelocity(i, reach);
		}
		trajData[0][reach] = Posed();
		//trajData[0][reach].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		//trajData[0][reach].Ori() = eef->GetSolid()->GetPose().Ori();
		eefVel = eef->GetSolid()->GetVelocity();
		trajVel[0][reach + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
		//trajVel[0][reach + 1] = Vec4d(minjerkTarget.Pos().x, minjerkTarget.Pos().y, minjerkTarget.Pos().z, minjerkPoses[reach].Pos().y);
	} //多分他の方法を使うべき

	joints.SaveViaPoint(count, movtime);

	joints.SaveTarget();

	//関節次元躍度最小軌道の生成
	//２パターン用意してるけどどっちがいいかは未検討
	double per = scene->GetTimeStep();
	if (jointMJT) {
		joints.MakeJointMinjerk(0);
	}else{
		joints.MakeJointMinjerk(iterate);
	}

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	/*
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].ball->GetJoint()->SetTargetVelocity(Vec3d());
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].hinge->GetJoint()->SetTargetVelocity(0);
	}
	Debug();
	*/

	//joints.Harden();

	scene->GetIKEngine()->Enable(false);

	count = 0;
	for (int i = 0; i < movtime; i++) {
		//跳ね対策(SetTarget内にて)
		joints.SetTarget(0, i);
		joints.SetTargetVelocity(0, i);
		//joints.SetTargetCurrent();
		DSTR << "in minjerk " << i << "step" << std::endl;
		Debug();
		scene->Step();
		if (count < (int)viaPoints.size()) {
			if (i == (viaPoints[count].step - 1)) {
				joints.SaveViaPoint(count, (viaPoints[count].step));
				cstates->SaveState(scene);
				count++;
			}
		}
		Debug();
		joints.SaveTorque(i);
		trajData[0][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajData[0][i].Ori() = eef->GetSolid()->GetPose().Ori();
		eefVel = eef->GetSolid()->GetVelocity();
		trajVel[0][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}

	best = 0;
	besttorque = joints.CalcTotalTorqueChange();
	joints.SetBestTorqueChange();

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	DSTR << "loaded scene after minjerk" << std::endl;
	Debug();
}

void FWTrajectoryPlanner::Forward(int k) {
	scene->GetIKEngine()->Enable(false);
	joints.Soften();
	/*
	for (int i = 0; i < nBall; i++)
	{
		DSTR << joints.balls[i].initialTorque << " ";
	}
	for (int i = 0; i < nHinge; i++)
	{
		DSTR << joints.hinges[i].initialTorque << " ";
	}
	DSTR << std::endl;
	*/
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	trajVelNotCorrected[k > 0 ? k - 1 : 0][0] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	Debug();
	for (int i = 0; i < movtime; i++) {
		joints.SetOffsetFromLPF(i);
		scene->Step();
		if (eef->GetSolid()->GetVelocity().norm() > 1e3) {
			stop = true;
		}
		if (i == 0) {
			Debug();
			DSTR << "in forward " << k << " " << i << std::endl;
			DSTR << std::endl;
		}
		joints.SavePositionFromLPF(k, i);
		joints.SaveVelocityFromLPF(k, i);

		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Ori() = eef->GetSolid()->GetPose().Ori();

		eefVel = eef->GetSolid()->GetVelocity();
		trajVelNotCorrected[k > 0 ? k - 1 : 0][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}
	joints.ResetOffset();
	DSTR << "in forward end" << std::endl;
	Debug();

	//Correction用にこの時の関節角速度を保存
	joints.SaveVelocityForwardEnd();

	//spring,damperをもとの値に
	joints.Harden();
}

void FWTrajectoryPlanner::Inverse(int k) {
	torquechange.clear();
	totalchange = 0;
	scene->GetIKEngine()->Enable(false);
	joints.Harden();
	//軌道再生と各関節トルクの保存
	int count = 0;
	for (int i = 0; i < movtime; i++) {

		//jointTargetを設定
		joints.SetTarget(k, i);
		joints.SetTargetVelocity(k, i);

		//Stepで軌道再生
		scene->Step();

		if (eef->GetSolid()->GetVelocity().norm() > 1e3) {
			stop = true;
		}
		if (i == 0) {
			Debug();
			DSTR << "in inverse " << k << " " << i << std::endl;
			DSTR << std::endl;
		}

		if (count < (int)viaPoints.size()) {
			if (i == (viaPoints[count].step - 1)) {
				tmpstates[count++]->SaveState(scene);
			}
		}
		trajData[k][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajData[k][i].Ori() = eef->GetSolid()->GetPose().Ori();
		//Debug();

		Vec3d eefVel = eef->GetSolid()->GetVelocity();
		trajVel[k][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());

		//トルクの記録
		joints.SaveTorque(i);
	}
	DSTR << "in inverse end" << std::endl;
	Debug();

	double torqueChange = joints.CalcTotalTorqueChange();
	if (torqueChange < besttorque) {
		best = k;
		besttorque = torqueChange;
		joints.SetBestTorqueChange();
	}

	//ロードしてIK周りの調整
	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();   //IKの一時的目標角?のリセット
	Debug();
}

void FWTrajectoryPlanner::Correction(int k) {
	//この前のFDMの終了時には目標到達時間における状態で終了しているはず(多分未到達だが)
	//なので、ここで到達位置や速度を
	beforecorstates[k - 1]->SaveState(scene);

	double dist = 0;
	int count;
	if (!springCor) {
		scene->GetIKEngine()->Enable(true);

		//IKを用いて目標に到達させ、jointsに状態保存
		if (!staticTarget) {

			eef->SetTargetPosition(targetPoint.pose.Pos());
			eef->SetTargetOrientation(targetPoint.pose.Ori());
			count = 0;
			if (true) {
				double dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
				while (count++ < 50 && dist > 0.01) {
					scene->Step();
					dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
					DSTR << count << " step" << std::endl;
					Debug();
				}
			}
			else {
				MinJerkTrajectory mjt = MinJerkTrajectory(eef->GetSolid()->GetPose(), targetPoint.pose, 10);
				for (int i = 0; i < 10; i++) {
					Posed target = mjt.GetCurrentPose(i + 1);
					eef->SetTargetPosition(target.Pos());
					eef->SetTargetOrientation(target.Ori());
					scene->Step();
				}
			}

			DSTR << "in correction IK step after " << count << " steps" << std::endl;
			Debug();
			DSTR << "dist:" << dist << std::endl;
			joints.SaveTarget();
			corstates[k - 1]->SaveState(scene);
		}

		//以降では経由点の通過保証修正を行う
		for (int n = 0; n < (int)viaPoints.size(); n++) {
			tmpstates[n]->LoadState(scene);
			scene->GetIKEngine()->ApplyExactState();
			eef->SetTargetPosition(viaPoints[n].pose.Pos());
			eef->SetTargetOrientation(viaPoints[n].pose.Ori());
			count = 0;
			dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoints[n].pose.Pos()).norm();
			while (count++ < 50 && dist > 0.05) {
				scene->Step();
				dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoints[n].pose.Pos()).norm();
			}
			DSTR << "ViaPoint " << n << " in correction IK step after " << count << " steps" << std::endl;
			Debug();
			joints.SaveViaPoint(n, viaPoints[n].step);
		}
	}
	else {
		joints.ResetPD();
		//joints.Soften();

		if (!staticTarget) {
			joints.SetTargetCurrent();
			targetSpring->Enable(true);
			count = 0;
			double dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
			while (count++ < 50) {
				scene->Step();
				dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
				DSTR << count << " step" << std::endl;
				DSTR << "dist:" << dist << std::endl;
				Debug();
				DSTR << "SpringForce:" << targetSpring->GetMotorForce() << std::endl;
			}

			DSTR << "in correction IK step after " << count << " steps" << std::endl;
			Debug();
			DSTR << "dist:" << dist << std::endl;
			joints.SaveTarget();
			targetSpring->Enable(false);
			corstates[k - 1]->SaveState(scene);
		}

		//以降では経由点の通過保証修正を行う
		for (int n = 0; n < (int)viaPoints.size(); n++) {
			tmpstates[n]->LoadState(scene);
			scene->GetIKEngine()->ApplyExactState();
			joints.SetTargetCurrent();
			viaSprings[n]->Enable(true);
			count = 0;
			dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoints[n].pose.Pos()).norm();
			while (count++ < 50) {
				scene->Step();
				dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoints[n].pose.Pos()).norm();
			}
			DSTR << "ViaPoint " << n << " in correction IK step after " << count << " steps" << std::endl;
			Debug();
			joints.SaveViaPoint(n, viaPoints[n].step);
			viaSprings[n]->Enable(false);
		}
	}

	joints.TrajectoryCorrection(k, staticTarget);
	scene->GetIKEngine()->Enable(false);

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();

	//IK後の全関節ターゲットリセット
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
	Debug();
}

void FWTrajectoryPlanner::PrepareSprings() {
	viaSolids.clear();
	viaSprings.clear();

	//ターゲットの剛体とバネの設定
	PHSolidDesc tDesc = PHSolidDesc();
	tDesc.pose = targetPoint.pose;
	tDesc.mass = 1.0;
	tDesc.inertia = Matrix3d::Unit() * tDesc.mass;
	tDesc.dynamical = false;
	targetSolid = scene->CreateSolid(tDesc);
	
	PHSpringDesc tspDesc = PHSpringDesc();
	tspDesc.bEnabled = false;
	tspDesc.spring = Vec3d(1e10, 1e10, 1e10);
	tspDesc.damper = Vec3d(1e2, 1e2, 1e2);
	if (eef->IsOrientationControlEnabled()) {
		tspDesc.springOri = 1e8;
		tspDesc.damperOri = 1e2;
	}
	else {
		tspDesc.springOri = 0;
		tspDesc.damperOri = 1e2;
	}
	targetSpring = (PHSpringIf*)scene->CreateJoint(targetSolid, eef->GetSolid(), tspDesc);
	targetSpring->SetPlugPose(Posed(eef->GetTargetLocalPosition(), Quaterniond()));
	scene->SetContactMode(targetSolid, eef->GetSolid(), PHSceneDesc::ContactMode::MODE_NONE);
	
	//経由点の剛体とバネの設定
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
		if (eef->IsOrientationControlEnabled()) {
			vspDesc.springOri = 1e8;
			vspDesc.damperOri = 1e2;
		}
		else {
			vspDesc.springOri = 0;
			vspDesc.damperOri = 1e2;
		}
		viaSprings.push_back((PHSpringIf*)scene->CreateJoint(viaSolids[i], eef->GetSolid(), vspDesc));
		viaSprings[i]->SetPlugPose(Posed(eef->GetTargetLocalPosition(), Quaterniond()));
		scene->SetContactMode(viaSolids[i], eef->GetSolid(), PHSceneDesc::ContactMode::MODE_NONE);
	}
}

void FWTrajectoryPlanner::ReloadCorrected(int k, bool nc) {
	if (k < 0 || k >= iterate) return;
	if (nc) {
		beforecorstates[k]->LoadState(scene);
	}
	else {
		corstates[k]->LoadState(scene);
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

void FWTrajectoryPlanner::CalcTrajectory(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt) {

	//総移動ステップ数設定
	movtime = tpoint.step;

	//path set
	path = output;

	DSTR << "stapose:" << startPoint.pose << " tarvel:" << startPoint.vel << " time:" << startPoint.step << std::endl;
	DSTR << "tarpose:" << targetPoint.pose << " tarvel:" << targetPoint.vel << " time:" << targetPoint.step << std::endl;

	this->staticTarget = staticTarget;
	this->jointMJT = jmjt;

	auto start = std::chrono::system_clock::now();

	//ターゲットと経由点のバネ準備
	PrepareSprings();

	Init();

	//経由点の通過時間の初期化
	ViatimeInitialize();

	//開始点の設定
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	startPoint.pose = Posed(eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition(), eef->GetSolid()->GetPose().Ori());
	//startPoint.vel = Vec6d(eefVel.x, eefVel.y, eefVel.z, 0, 0, 0);
	startPoint.vel = Vec6d();
	startPoint.acc = Vec6d();
	startPoint.step = 0;
	startPoint.time = 0;

	//目標点の設定(引数とってくるだけ)
	targetPoint = tpoint;

	//経由点通過時間推移記録ファイル
	std::ofstream outfile(output + "Viatimes.csv");
	std::ofstream outfile2(output + "Bests.csv");

	bool cont = true;
	for (int i = 0; i < (iterateViaAdjust + 1) && cont; i++) {
		//現在の経由点通過時間を記録
		for (int j = 0; j < (int)viaPoints.size(); j++) {
			outfile << viaPoints[j].step << ",";
		}
		outfile << std::endl;
		//FIRMにより近似最適解の導出
		FIRM(tpoint, LPFmode, smoothCount, output, bChange, pChange, staticTarget, jmjt);
		outfile2 << besttorque << std::endl;
		//経由点の通過時間の調整をして継続か判断
		cont = ViatimeAdjustment();
	}

	joints.CloseFile();
	outfile.close();
	outfile2.close();

	viaPoints.clear();
	viaPoints.shrink_to_fit();

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	//outfile3 << msec << std::endl;
}

}
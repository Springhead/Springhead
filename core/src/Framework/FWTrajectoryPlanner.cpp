#include <Framework/FWTrajectoryPlanner.h>

namespace Spr { ;


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
	sPoint = ControlPoint(spose, Vec6d(sVel.x, sVel.y, sVel.z, 0, 0, 0), 0, 0); 
	fPoint = ControlPoint(fpose, Vec6d(fVel.x, fVel.y, fVel.z, 0, 0, 0), time, 0);
	stime = 0; ftime = time;
	vPoint = ControlPoint(); vtime = 0;
	sAcc *= per * per;
	fAcc *= per * per;
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
	sPoint = spoint; fPoint = fpoint;
	stime = sPoint.step; ftime = fpoint.step;
	vPoint = ControlPoint();  vtime = 0;

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

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, Vec3d sVel, Vec3d fVel, Posed vPose, int vtime, int time, double per) {
	sVel *= per;
	fVel *= per;
	sPoint = ControlPoint(spose, Vec6d(sVel.x, sVel.y, sVel.z, 0, 0, 0), 0, 0);
	fPoint = ControlPoint(fpose, Vec6d(fVel.x, fVel.y, fVel.z, 0, 0, 0), time + vtime, 0);
	stime = 0; ftime = time + vtime;
	this->vPoint = ControlPoint(vPose, vtime, 0); this->vtime = vtime;
	sVel *= per;
	fVel *= per;
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
	sPoint = ControlPoint(spoint.pose, spoint.vel * per, spoint.step, spoint.time);
	fPoint = ControlPoint(fpoint.pose, fpoint.vel * per, fpoint.step, fpoint.time);
	stime = sPoint.step; ftime = fpoint.step;
	this->vPoint = ControlPoint(vpoint.pose, vpoint.vel * per, vpoint.step, vpoint.time); this->vtime = vPoint.step;
	
	//開始と終点を０としたときの経由点の相対位置相対位置
	double internalRatio = (double)(vtime - stime) / (ftime - stime);
	Vec3d relvpos = vpoint.pose.Pos() - ((1 - internalRatio) * spoint.pose.Pos() + internalRatio * fpoint.pose.Pos());
	double tvs = vtime - stime;
	double tfv = ftime - vtime;
	Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * vpoint.pose.Pos();
	Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * vpoint.pose.Pos();
	//Vec3d relvvel = -2.5 * ((tvs - tfv) / (tvs * tfv)) * relvpos;
	//Vec3d relvacc = (10 * (tvs * tvs - 4 * tvs * tfv + tfv * tfv) / (3 * tvs * tvs * tfv * tfv)) * relvpos;
	
	if (vtime > stime && vtime < ftime) {
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
		DSTR << velToQua << std::endl;
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
		DSTR << curvel << std::endl;
		delta.w = (-curvel[0] * velToQua[i - 1].x - curvel[1] * velToQua[i - 1].y - curvel[2] * velToQua[i - 1].z) / 2;
		delta.x = (curvel[2] * velToQua[i - 1].y - curvel[1] * velToQua[i - 1].z + curvel[0] * velToQua[i - 1].w) / 2;
		delta.y = (-curvel[2] * velToQua[i - 1].x + curvel[0] * velToQua[i - 1].z + curvel[1] * velToQua[i - 1].w) / 2;
		delta.z = (curvel[1] * velToQua[i - 1].x - curvel[0] * velToQua[i - 1].y + curvel[2] * velToQua[i - 1].w) / 2;
		velToQua[i] = velToQua[i - 1] * delta;
		DSTR << velToQua << std::endl;
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
	double d = dot(sQua, fQua);
	Quaterniond inter = (d > 0.0) ? interpolate(sr, sQua, fQua) : interpolate(sr, sQua, -fQua);
	Vec3d current = Vec3d();
	for (int i = 0; i < 3; i++) {
		current[i] = amjt[i].GetCurrentAngle(t);
	}
	Quaterniond cq;
	cq.FromEuler(current);
	DSTR << "Comp (qua):" << inter << " (euler):" << cq << std::endl;
	if (d > 0.0) {
		return interpolate(sr, sQua, fQua) * cq;
	}
	else {
		return interpolate(sr, sQua, -fQua) * cq;
	}
}

Quaterniond QuaMinJerkTrajectory::GetDeltaQuaternion(int t) {
	if (t <= stime) {
		return Quaterniond();
	}if (t >= ftime) {
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
	double d = dot(sQua, fQua);
	Quaterniond inter = (d > 0.0) ? interpolate(sr, Quaterniond(), fQua * sQua.Inv()) : interpolate(sr, Quaterniond(), -fQua * sQua.Inv());
	Vec3d current = Vec3d();
	for (int i = 0; i < 3; i++) {
		current[i] = amjt[i].GetDeltaAngle(t);
	}
	Quaterniond cq;
	cq.FromEuler(current);
	DSTR << "Comp (qua):" << inter << " (euler):" << cq << std::endl;
	if (d > 0) {
		return interpolate(sr, Quaterniond(), fQua * sQua.Inv()) * cq;
	}
	else {
		return interpolate(sr, Quaterniond(), -fQua * sQua.Inv()) * cq;
	}
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
ControlPoint::ControlPoint(Posed p, Vec6d v, int s, double t) {
	pose = p;
	vel = v;
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
FWTrajectoryPlanner::HingeJoint::HingeJoint(PHIKHingeActuatorIf* hinge) {
	this->hinge = hinge;
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
	//angle.clear();
	angleLPF.resize(iterate + 1, mtime);
	//angle.clear();
	//weight = 100 / hinge->GetJoint()->GetSpring();
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
void FWTrajectoryPlanner::HingeJoint::SaveTorque(int n) {
	torque[n] = hinge->GetJoint()->GetMotorForceN(0);
	torqueLPF[n] = hinge->GetJoint()->GetMotorForceN(0);
}
void FWTrajectoryPlanner::HingeJoint::SaveTarget() {
	targetAngle = hinge->GetJoint()->GetPosition();
	targetVel = hinge->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::HingeJoint::SetTarget(int k, int n) {
	if (k >= 0 && k <= iterate) {
		hinge->GetJoint()->SetTargetPosition(angle[k][n]);
	}
	DSTR << "SetHingeTarget : " << angle[k][n] << " current : " << hinge->GetJoint()->GetPosition() << std::endl;
}
void FWTrajectoryPlanner::HingeJoint::SetOffsetFromLPF(int n) {
	hinge->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
	DSTR << torqueLPF[n] << " ";
}
void FWTrajectoryPlanner::HingeJoint::ResetOffset() {
	hinge->GetJoint()->SetOffsetForceN(0, 0);
}
void FWTrajectoryPlanner::HingeJoint::SavePosition(int k, int n) {
	angle[k][n] = hinge->GetJoint()->GetPosition();
	DSTR << "Saved : " << angle[k][n] << std::endl;
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
void FWTrajectoryPlanner::HingeJoint::TrajectoryCorrection(int k, bool s) {
	double end;
	int time;
	double per = hinge->GetJoint()->GetScene()->GetTimeStep();

	//終端の到達および停止保証部
	end = angleLPF[k][angleLPF.width() - 1];
	time = movetime;
	AngleMinJerkTrajectory delta = AngleMinJerkTrajectory(end, targetAngle, 0, -vel, 0, 0, time, per);
	DSTR << "target:" << targetAngle << " actual:" << delta.GetCurrentAngle(movetime) << std::endl;
	for (int i = 0; i < time; i++) {
		angle[k][i] = delta.GetDeltaAngle(i + 1) + angleLPF[k][i]; //k+1でいいのか？
		angleLPF[k][i] += delta.GetDeltaAngle(i + 1);
		CorrTraj[k - 1][i] = delta.GetCurrentVelocity(i);
	}
	
	//各経由点の通過保証
	double start = 0;
	int last = 0;
	if (viaCorrect) {
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
void FWTrajectoryPlanner::HingeJoint::TrajectoryCorrectionWithVia(int k, bool s) {
	double end = angleLPF[k][angleLPF.width() - 1];
	double via = angleLPF[k][viatimes[0]];
	double per = hinge->GetJoint()->GetScene()->GetTimeStep();
	AngleMinJerkTrajectory delta = AngleMinJerkTrajectory(0, targetAngle - end, 0, -vel, deltaViaAngle - via, 0, movetime - viatimes[0], viatimes[0], per);
	DSTR << "target:" << targetAngle << " actual:" << delta.GetCurrentAngle(movetime) << std::endl;
	for (int i = 0; i < movetime; i++) {
		angle[k][i] = delta.GetDeltaAngle(i + 1) + angleLPF[k][i]; //k+1でいいのか？
	}
}
void FWTrajectoryPlanner::HingeJoint::ApplyLPF(int lpf, int count) {
	torqueLPF = LPF::weightedv(torque, initialTorque, weight, rateLPF);
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
	hinge->GetJoint()->SetSpring(originalSpring);
	hinge->GetJoint()->SetDamper(originalDamper);
}
double FWTrajectoryPlanner::HingeJoint::CalcTotalTorqueChange() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		total += pow(abs(torque[i] - (((i - 1) < 0) ? initialTorque : torque[i - 1])), 2);
	}
	DSTR << total << std::endl;
	return total * weight;
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
	return total * weight;
}
void FWTrajectoryPlanner::HingeJoint::SetBestTorqueChange() {
	for (int n = 0; n < viatimes.size(); n++) {
		tChanges[n] = CalcTorqueChangeInSection(n);
	}
}
void FWTrajectoryPlanner::HingeJoint::ShowInfo() {
	DSTR << "Pose:" << hinge->GetJoint()->GetPosition() << " Target:" << hinge->GetJoint()->GetTargetPosition() << " Vel:" << hinge->GetJoint()->GetVelocity() << std::endl;
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

FWTrajectoryPlanner::BallJoint::BallJoint(PHIKBallActuatorIf* ball) {
	this->ball = ball;
}
FWTrajectoryPlanner::BallJoint::~BallJoint() { DSTR << "BallJoint Class Object is destroyed" << std::endl; }
void FWTrajectoryPlanner::BallJoint::Initialize(int iterate, int mtime, int nVia, double rate, bool vCorr) {
	ball->Enable(true);
	this->iterate = iterate;
	this->movetime = mtime;
	torque.resize(mtime, Vec3d());
	torqueLPF.resize(mtime, Vec3d());
	ori.resize(iterate + 1, mtime);
	//ori.clear();
	oriLPF.resize(iterate + 1, mtime);
	//oriLPF.clear();
	//weight = 100 / ball->GetJoint()->GetSpring();
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
void FWTrajectoryPlanner::BallJoint::SaveTorque(int n) {
	this->torque[n] = ball->GetJoint()->GetMotorForceN(0);
	this->torqueLPF[n] = ball->GetJoint()->GetMotorForceN(0);
}
void FWTrajectoryPlanner::BallJoint::SaveTarget() {
	targetOri = ball->GetJoint()->GetPosition();
	targetVel = ball->GetJoint()->GetVelocity();
}
void FWTrajectoryPlanner::BallJoint::SetTarget(int k, int n) {
	if (k >= 0 && k <= iterate) {
		DSTR << "SetBallTarget : " << ori[k][n] << " current : " << ball->GetJoint()->GetPosition() << std::endl;
		ball->GetJoint()->SetTargetPosition(ori[k][n]);
	}
}
void FWTrajectoryPlanner::BallJoint::SetOffsetFromLPF(int n) {
	ball->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
	DSTR << torqueLPF[n] << " ";
	//ball->GetJoint()->SetOffsetForceN(0, Vec3d(1, 0, 0));
}
void FWTrajectoryPlanner::BallJoint::ResetOffset() {
	ball->GetJoint()->SetOffsetForceN(0, Vec3d(0, 0, 0));
}
void FWTrajectoryPlanner::BallJoint::SavePosition(int k, int n) {
	ori[k][n] = ball->GetJoint()->GetPosition();
	DSTR << "Saved : " << ori[k][n] << std::endl;
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
void FWTrajectoryPlanner::BallJoint::TrajectoryCorrection(int k, bool s) {
	//終端条件保証
	Quaterniond end;
	int time;
	double per = ball->GetJoint()->GetScene()->GetTimeStep();

	end = oriLPF[k][oriLPF.width() - 1];
	time = movetime;
	QuaMinJerkTrajectory delta = QuaMinJerkTrajectory(end, targetOri, Vec3d(), -vel, Vec3d(), Vec3d(), time, per);
	DSTR << "target:" << targetOri << " actual:" << delta.GetCurrentQuaternion(movetime) << std::endl;
	for (int i = 0; i < time; i++) {
		ori[k][i] = delta.GetDeltaQuaternion(i + 1) * oriLPF[k][i];
		oriLPF[k][i] = ori[k][i];
	}
	//通過点保証
	Quaterniond start = Quaterniond();
	int last = 0;
	if (viaCorrect) {
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
//修正予定
void FWTrajectoryPlanner::BallJoint::TrajectoryCorrectionWithVia(int k, bool s) {
	Quaterniond end = oriLPF[k][oriLPF.width() - 1];
	Quaterniond target = s ? targetOri : ball->GetJoint()->GetPosition();
	double per = ball->GetJoint()->GetScene()->GetTimeStep();
	QuaMinJerkTrajectory delta = QuaMinJerkTrajectory(end, target, Vec3d(), Vec3d(), movetime, per);
	for (int i = 0; i < movetime; i++) {
		ori[k][i] = delta.GetDeltaQuaternion(i + 1) *oriLPF[k][i]; //k+1でいいのか？
		DSTR << delta.GetDeltaQuaternion(i + 1) << std::endl;
	}
}
void FWTrajectoryPlanner::BallJoint::ApplyLPF(int lpf, int count) {
	torqueLPF = LPF::weightedv(torque, initialTorque, weight, rateLPF);
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
	ball->GetJoint()->SetSpring(originalSpring);
	ball->GetJoint()->SetDamper(originalDamper);
}
double FWTrajectoryPlanner::BallJoint::CalcTotalTorqueChange() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		for (int j = 0; j < 3; j++) {
			total += pow(abs(torque[i][j] - (((i - 1) < 0) ? initialTorque[j] : torque[i - 1][j])), 2);
			DSTR << torque[i][j] << " " << (((i - 1) < 0) ? initialTorque[j] : torque[i - 1][j]) << std::endl;
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
void FWTrajectoryPlanner::BallJoint::SetBestTorqueChange() {
	for(int n = 0; n < viatimes.size(); n++) {
		tChanges[n] = CalcTorqueChangeInSection(n);
	}
}
void FWTrajectoryPlanner::BallJoint::ShowInfo() {
	DSTR << "Pose:" << ball->GetJoint()->GetPosition() << " Target:" << ball->GetJoint()->GetTargetPosition() << std::endl;
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

FWTrajectoryPlanner::Joints::Joints() { joints.clear(); joints.shrink_to_fit(); }
FWTrajectoryPlanner::Joints::~Joints() { std::vector<Joint*>().swap(joints); }
void FWTrajectoryPlanner::Joints::RemoveAll() {
	joints.clear();  joints.shrink_to_fit();
	balls.clear();   balls.shrink_to_fit();
	hinges.clear();  hinges.shrink_to_fit();
}
void FWTrajectoryPlanner::Joints::Add(PHIKActuatorIf* j) {
	if (DCAST(PHIKBallActuatorIf, j)) {
		BallJoint b = BallJoint(DCAST(PHIKBallActuatorIf, j));
		balls.push_back(b);
		joints.push_back(&balls[balls.size() - 1]);
	}
	if (DCAST(PHIKHingeActuatorIf, j)) {
		HingeJoint h = HingeJoint(DCAST(PHIKHingeActuatorIf, j));
		hinges.push_back(h);
		joints.push_back(&hinges[hinges.size() - 1]);
	}
}
void FWTrajectoryPlanner::Joints::initialize(int iterate, int movetime, int nVia, double rate, bool vCorr) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->Initialize(iterate, movetime);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].Initialize(iterate, movetime, nVia, rate, vCorr);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].Initialize(iterate, movetime, nVia, rate, vCorr);
	}
}

void FWTrajectoryPlanner::Joints::SetTarget(int k, int n) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SetTarget(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SetTarget(k, n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SetTarget(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SaveTorque(int n) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SaveTorque(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SaveTorque(n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SaveTorque(n);
	}
}

void FWTrajectoryPlanner::Joints::SaveTarget() {
	for (int i = 0; i < balls.size(); i++)
	{
		balls[i].SaveTarget();
	}
	for (int i = 0; i < hinges.size(); i++)
	{
		hinges[i].SaveTarget();
	}
}

void FWTrajectoryPlanner::Joints::SetOffsetFromLPF(int n) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SetOffsetFromLPF(n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SetOffsetFromLPF(n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SetOffsetFromLPF(n);
	}
	DSTR << std::endl;
}

void FWTrajectoryPlanner::Joints::ResetOffset() {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SetOffsetFromLPF(n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].ResetOffset();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].ResetOffset();
	}
}

void FWTrajectoryPlanner::Joints::SavePosition(int k, int n) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SavePosition(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SavePosition(k, n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SavePosition(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SaveViaPoint(int v, int t) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SavePosition(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SaveViaPoint(v, t);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SaveViaPoint(v, t);
	}
}

void FWTrajectoryPlanner::Joints::SavePositionFromLPF(int k, int n) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SavePositionFromLPF(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SavePositionFromLPF(k, n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SavePositionFromLPF(k, n);
	}
}

void FWTrajectoryPlanner::Joints::TrajectoryCorrection(int k, bool s) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->TrajectoryCorrection(k);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].TrajectoryCorrection(k, s);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].TrajectoryCorrection(k, s);
	}
}

void FWTrajectoryPlanner::Joints::TrajectoryCorrectionWithVia(int k, bool s) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->TrajectoryCorrection(k);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].TrajectoryCorrectionWithVia(k, s);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].TrajectoryCorrectionWithVia(k, s);
	}
}

void FWTrajectoryPlanner::Joints::ApplyLPF(int lpf, int count) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->ApplyLPF(lpf, count);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].ApplyLPF(lpf, count);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].ApplyLPF(lpf, count);
	}
}
void FWTrajectoryPlanner::Joints::Soften() {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->Soften();
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].Soften();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].Soften();
	}
}
void FWTrajectoryPlanner::Joints::Harden() {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->Harden();
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].Harden();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].Harden();
	}
}
void FWTrajectoryPlanner::Joints::ResetPD() {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->ResetPD();
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].ResetPD();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].ResetPD();
	}
}
double FWTrajectoryPlanner::Joints::CalcTotalTorqueChange() {
	double total = 0;
	for (int i = 0; i < (int)balls.size(); i++) {
		total += balls[i].CalcTotalTorqueChange();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		total += hinges[i].CalcTotalTorqueChange();
	}
	return total;
}
double FWTrajectoryPlanner::Joints::CalcTorqueChangeInSection(int n) {
	double total = 0;
	for (int i = 0; i < (int)balls.size(); i++) {
		total += balls[i].CalcTorqueChangeInSection(n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		total += hinges[i].CalcTorqueChangeInSection(n);
	}
	return total;
}
void FWTrajectoryPlanner::Joints::SetBestTorqueChange() {
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SetBestTorqueChange();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SetBestTorqueChange();
	}
}
double FWTrajectoryPlanner::Joints::GetBestTorqueChangeInSection(int n) {
	double total = 0;
	for (int i = 0; i < (int)balls.size(); i++) {
		total += balls[i].tChanges[n];
		DSTR << balls[i].tChanges[n] << " ";
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		total += hinges[i].tChanges[n];
		DSTR << hinges[i].tChanges[n] << " ";
	}
	DSTR << std::endl;
	return total;
}
void FWTrajectoryPlanner::Joints::ShowInfo() {
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].ShowInfo();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].ShowInfo();
	}
}
void FWTrajectoryPlanner::Joints::SetTargetCurrent() {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SetTarget(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SetTargetCurrent();
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SetTargetCurrent();
	}
}
void FWTrajectoryPlanner::Joints::SetWeight() {
	double minSp = INFINITY;
	for (int i = 0; i < (int)balls.size(); i++) {
		if (balls[i].originalSpring < minSp) {
			minSp = balls[i].originalSpring;
		}
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		if (hinges[i].originalSpring < minSp) {
			minSp = hinges[i].originalSpring;
		}
	}
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].weight = minSp / balls[i].originalSpring;
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].weight = minSp / hinges[i].originalSpring;
	}
}

void FWTrajectoryPlanner::Joints::SetPD(double s, double d, bool mul) {
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SetPD(s, d, mul);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SetPD(s, d, mul);
	}
}

void FWTrajectoryPlanner::Joints::SetTargetFromLPF(int k, int n) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SetTarget(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].SetTargetFromLPF(k, n);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].SetTargetFromLPF(k, n);
	}
}

void FWTrajectoryPlanner::Joints::UpdateIKParam(double b, double p) {
	/*for (int i = 0; i < joints.size(); i++) {
	joints[i]->SetTarget(k, n);
	}*/
	for (int i = 0; i < (int)balls.size(); i++) {
		balls[i].UpdateIKParam(b, p);
	}
	for (int i = 0; i < (int)hinges.size(); i++) {
		hinges[i].UpdateIKParam(b, p);
	}
}

//BiQuad LPF
//荒ぶる
PTM::VMatrixRow<double> FWTrajectoryPlanner::LPF::BiQuad(PTM::VMatrixRow<double> input, double samplerate, double freq, double q) {
	PTM::VMatrixRow<double> output;
	//DSTR << input.height() << " " << input.width() << std::endl;
	output.resize(input.height(), input.width());
	output.clear();

	double omega = 2.0 * M_PI * freq / samplerate;
	double alpha = sin(omega) / (2.0 * q);

	double a0 = 1.0 + alpha;
	double a1 = -2.0 * cos(omega);
	double a2 = 1.0 - alpha;
	double b0 = (1.0 - cos(omega)) / 2.0;
	double b1 = 1.0 - cos(omega);
	double b2 = (1.0 - cos(omega)) / 2.0;

	for (size_t i = 0; i < input.height(); i++) {
		output[i][0] = b0 / a0 * input[i][0];
		output[i][1] = b0 / a0 * input[i][1] + b1 / a0 * input[i][0] - a1 / a0 * output[i][0];
		for (size_t j = 0; j < input.width(); j++) {
			output[i][j] = b0 / a0 * input[i][j] + b1 / a0 * input[i][j - 1] + b2 / a0 * input[i][j - 2]
				- a1 / a0 * output[i][j - 1] - a2 / a0 * output[i][j - 2];
		}
	}
	return output;
}
//N-Simple Moving Average LPF
template<class T>
PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
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
				sum = sum + input[i][j] - input[i][j - n];   //このやり方だと累積誤差出ます
															 /* 累積誤差チェック用
															 double check_sum = 0;
															 for (int k = 0; k < n; k++){
															 check_sum += input[i][j - k];
															 }
															 DSTR << sum - check_sum << std::endl;
															 */
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
 PTM::VVector<T> FWTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
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
PTM::VMatrixRow<T> FWTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
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
PTM::VVector<T> FWTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
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

FWTrajectoryPlanner::FWTrajectoryPlanner(int d, int i, int iv, double f, double q, int n, double mg, int c, bool wf, bool snc, double r, double vRate) {
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
}

void FWTrajectoryPlanner::Init() {

	CheckAndSetJoints();

	//jointsの初期化
	joints.initialize(iterate, movtime, viaPoints.size(), rate, viaCorrect);
	joints.SetWeight();
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
	for (int i = 0; i < viaPoints.size(); i++) {
		tmpstates.push_back(ObjectStatesIf::Create());
	}
	DSTR << scene->GetCount() << std::endl;
	DSTR << "Saved" << std::endl;
	Debug();
	states->SaveState(scene);

	//計算前フラグ
	moving = false;
	calced = false;
}

void FWTrajectoryPlanner::Init(int d, int i, int iv, int n, double mg, int c, bool wf, bool snc, double r, double vRate, bool vCorr) {
	this->depth = d;
	this->iterate = i;
	this->iterateViaAdjust = iv;
	this->nsma = n;
	this->mag = mg;
	this->correction = c;
	this->rate = r;
	this->viaAdjustRate = vRate;
	this->waitFlag = wf;
	this->saveNotCorrected = snc;
	this->viaCorrect = vCorr;
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
		joints.Add(ika);
		ika = ika->GetParent();
	}
}

void FWTrajectoryPlanner::SetControlTarget(PHIKEndEffectorIf* e) { this->eef = e; };

void FWTrajectoryPlanner::SetScene(PHSceneIf* s) { this->scene = s; };

void FWTrajectoryPlanner::CalcTrajectoryWithViaPoint(ControlPoint tpoint, ControlPoint vpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt) {
	// --- デバッグ用の出力ファイル準備 ---
	//ss << ".csv";
	std::string filename;
	PrepareOutputFilename(filename);
	DSTR << filename << std::endl;
	std::ofstream torqueChangeOutput(output + "TorqueChange.csv");

	auto start = std::chrono::system_clock::now();

	// --- 計算に際してのいろいろな初期化 ---
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	Vec6d startVel = Vec6d(eefVel.x, eefVel.y, eefVel.z, 0, 0, 0);
	startPoint = ControlPoint(Posed(eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition(), eef->GetSolid()->GetPose().Ori()), startVel, 0, 0);
	targetPoint = tpoint;
	this->viaPoint = vpoint;
	movtime = viaPoint.step + targetPoint.step;
	DSTR << "stapose:" << startPoint.pose << " tarvel:" << startPoint.vel << " time:" << startPoint.step << std::endl;
	DSTR << "tarpose:" << targetPoint.pose << " tarvel:" << targetPoint.vel << " time:" << targetPoint.step << std::endl;
	DSTR << "viapose:" << viaPoint.pose << " viavel:" << viaPoint.vel << " time:" << viaPoint.step << std::endl;
	this->staticTarget = staticTarget;
	this->jointMJT = jmjt;
	Init();

	MakeMinJerkAndSaveWithViaPoint();
	torqueChangeOutput << besttorque << std::endl;

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

	Debug();

	// --- 軌道計算の繰り返し計算 ---

	scene->GetIKEngine()->Enable(false);

	double torqueChange;

	for (int k = 1; k < iterate + 1; k++) {

		//トルクにLPFをかける
		for (int i = 0; i < smoothCount; i++) {
			joints.ApplyLPF(LPFmode, smoothCount);
		}
		JointForward(k, true);

		JointInverse(k, true);

		torqueChange = joints.CalcTotalTorqueChange();
		torqueChangeOutput << torqueChange << std::endl;
		//springdamper小さくする
	}

	//scene->GetIKEngine()->Enable(true);

	// --- 計算後であることのフラグ有効化 ---
	moving = true;
	calced = true;
	ite = best;
	repCount = 0;

	//joints.ResetPD();

	OutputTrajectory(output);
	if (saveNotCorrected) {
		OutputNotCorrectedTrajectory(output);
	}
	OutputVelocity(output);

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();   //IKの一時的目標角?のリセット

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	//outfile3 << msec << std::endl;
}

void FWTrajectoryPlanner::CalcContinuousTrajectory(int LPFmode, int smoothCount, std::string filename, bool bChange, bool pChange, bool staticTarget, bool jmjt) {
	initialstates = ObjectStatesIf::Create();
	initialstates->SaveState(scene);
	int totalTime = 0;
	for (size_t i = 0; i < viaPoints.size(); i++) {
		totalTime += viaPoints[i].step;
	}
	ContinuousTrajData.resize(iterate + 1,totalTime);
	totalTime = 0;
	int movesteps = 0;
	/*
	while (!viaPoints.empty()) {
		//ControlPoint vPoint = ControlPoint(viaPoints[0]);
		JointCalcTrajectory(viaPoints[0].pose, viaPoints[0].step, LPFmode, smoothCount, filename, bChange, pChange, staticTarget, jmjt);
		movesteps = viaPoints[0].step;
		viaPoints.erase(viaPoints.begin());
		for (int j = 0; j < iterate + 1; j++) {
			for (int i = 0; i < movesteps; i++) {
				ContinuousTrajData[j][totalTime + i] = trajData[j][i];
				DSTR << trajData[j][i] << std::endl;
			}
		}
		totalTime += movesteps;
		cstates->LoadState(scene);
		//cstates->ReleaseState(scene);
		scene->GetIKEngine()->ApplyExactState();
		moving = false;
		calced = false;
	}
	/*/

	while (viaPoints.size() > 1) {
		ControlPoint vPoint;
		vPoint = viaPoints[0];
		DSTR << "tarpose:" << viaPoints[1].pose << " tarvel:" << viaPoints[1].vel << " time:" << viaPoints[1].step << std::endl;
		DSTR << "viapose:" << viaPoints[0].pose << " viavel:" << viaPoints[0].vel << " time:" << viaPoints[0].step << std::endl;
		CalcTrajectoryWithViaPoint(viaPoints[1], viaPoints[0], LPFmode, smoothCount, filename, bChange, pChange, staticTarget, jmjt);
		movesteps = viaPoints[0].step;
		viaPoints.erase(viaPoints.begin());
		for (int j = 0; j < iterate + 1; j++) {
			for (int i = 0; i < movesteps; i++) {
				ContinuousTrajData[j][totalTime + i] = trajData[j][i];
				DSTR << trajData[j][i] << std::endl;
			}
		}
		totalTime += movesteps;
		cstates->LoadState(scene);
		Debug();
		//cstates->ReleaseState(scene);
		scene->GetIKEngine()->ApplyExactState();
		moving = false;
		calced = false;
	}
	ControlPoint vPoint = ControlPoint(viaPoints[0]);
	viaPoints.erase(viaPoints.begin());
	JointCalcTrajectory(vPoint.pose, vPoint.step * scene->GetTimeStep(), LPFmode, smoothCount, filename, bChange, pChange, staticTarget, jmjt);
	movesteps = vPoint.step;
	for (int j = 0; j < iterate + 1; j++) {
		for (int i = 0; i < movesteps; i++) {
			ContinuousTrajData[j][totalTime + i] = trajData[j][i];
			DSTR << trajData[j][i] << std::endl;
		}
	}
	totalTime += movesteps;
	Debug();
	//cstates->ReleaseState(scene);
	scene->GetIKEngine()->ApplyExactState();
	
	trajData.resize(iterate + 1, totalTime);
	for (int j = 0; j < iterate + 1; j++) {
		for (int i = 0; i < totalTime; i++) {
			trajData[j][i] = ContinuousTrajData[j][i];
		}
	}
	initialstates->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	OutputTrajectory(filename);
	ite = 0;
	best = 0;
}

void FWTrajectoryPlanner::AddControlPoint(ControlPoint c) { viaPoints.push_back(c); };

void FWTrajectoryPlanner::JointCalcTrajectory(Posed tPose, double mt, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt) {

	// --- デバッグ用の出力ファイル準備 ---
	//ss << ".csv";
	std::string filename;
	PrepareOutputFilename(filename);
	DSTR << filename << std::endl;
	std::ofstream torqueChangeOutput(output + "TorqueChange.csv");

	auto start = std::chrono::system_clock::now();

	// --- 計算に際してのいろいろな初期化 ---
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	startPoint.pose = Posed(eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition(), eef->GetSolid()->GetPose().Ori());
	startPoint.vel = Vec6d(eefVel.x, eefVel.y, eefVel.z, 0, 0, 0);
	startPoint.step = 0;
	targetPoint.pose = tPose;
	movtime = (int)(mt / scene->GetTimeStep());
	DSTR << "stapose:" << startPoint.pose << " tarvel:" << startPoint.vel << " time:" << startPoint.step << std::endl;
	DSTR << "tarpose:" << targetPoint.pose << " tarvel:" << targetPoint.vel << " time:" << targetPoint.step << std::endl;
	DSTR << mt << std::endl;
	this->staticTarget = staticTarget;
	this->jointMJT = jmjt;
	Init();

	MakeMinJerkAndSave();
	torqueChangeOutput << besttorque << std::endl;

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

	Debug();

	// --- 軌道計算の繰り返し計算 ---

	scene->GetIKEngine()->Enable(false);

	double torqueChange;

	for (int k = 1; k < iterate + 1; k++) {

		//トルクにLPFをかける
		for (int i = 0; i < smoothCount; i++) {
			joints.ApplyLPF(LPFmode, smoothCount);
		}
		JointForward(k);

		JointInverse(k);

		torqueChange = joints.CalcTotalTorqueChange();
		torqueChangeOutput << torqueChange << std::endl;
		//springdamper小さくする
	}

	//scene->GetIKEngine()->Enable(true);

	// --- 計算後であることのフラグ有効化 ---
	moving = true;
	calced = true;
	ite = best;
	repCount = 0;

	//joints.ResetPD();

	OutputTrajectory(output);
	if (saveNotCorrected) {
		OutputNotCorrectedTrajectory(output);
	}
	OutputVelocity(output);

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();   //IKの一時的目標角?のリセット

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	//outfile3 << msec << std::endl;
}

void JointRecalcFromIterateN(Posed tPose, double mt, int LPFmode, int smoothCount, std::string output, bool bChange = false, bool pChange = false, bool staticTarget = false, bool jmjt = false) {

}

void FWTrajectoryPlanner::RecalcFromIterationN(int n) {

}

void FWTrajectoryPlanner::MakeMinJerkAndSave() {
	//std::ofstream outfile("C:/Users/hirohitosatoh/Desktop/traj.csv", std::ios::out | std::ios::trunc);
	//開始状況とターゲットからMinJerkを作り軌道の初期化
	MinJerkTrajectory minjerk = MinJerkTrajectory(startPoint.pose, targetPoint.pose, movtime);
	/*
	for (i = 0; i < movtime; i++) {
	for (int k = 0; k < iterate + 1; k++) {
	trajData[k][i] = minjerk.GetCurrentPose(i + 1);
	}
	for (int k = 0; k < iterate; k++) {
	trajDataNotCorrected[k][i] = minjerk.GetCurrentPose(i + 1);
	}
	//outfile << trajData[0][i].px << "," << trajData[0][i].py << "," << trajData[0][i].pz << "," << trajData[0][i].w << "," << trajData[0][i].x << "," << trajData[0][i].y << "," << trajData[0][i].z << std::endl;
	}
	*/

	joints.Harden();

	states->LoadState(scene);
	Debug();
	scene->GetIKEngine()->Enable(true);
	//eef->Enable(true);
	scene->GetIKEngine()->ApplyExactState();
	Debug();

	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	for (int i = 0; i < iterate + 1; i++) {
		trajVel[i][0] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}

	int reach = 0;
	//空間躍度最小軌道を再生して関節角を保存
	for (; reach < movtime; reach++) {
		Posed minjerkTarget = minjerk.GetCurrentPose(reach + 1);
		eef->SetTargetPosition(minjerkTarget.Pos());
		eef->SetTargetOrientation(minjerkTarget.Ori());
		for (int i = 0; i < 1; i++) {
			scene->Step();
		}
		Debug();
		for (int i = 0; i < iterate + 1; i++) {
			joints.SavePosition(i, reach);
		}
		trajData[0][reach] = minjerk.GetCurrentPose(reach + 1);
		//trajData[0][reach].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		//trajData[0][reach].Ori() = eef->GetSolid()->GetPose().Ori();
	} //多分他の方法を使うべき

	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].targetOri = joints.balls[i].ball->GetJoint()->GetPosition();
		joints.balls[i].targetVel = joints.balls[i].ball->GetJoint()->GetVelocity();
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].targetAngle = joints.hinges[i].hinge->GetJoint()->GetPosition();
		joints.hinges[i].targetVel = joints.hinges[i].hinge->GetJoint()->GetVelocity();
	}

	double per = scene->GetTimeStep();
	if (jointMJT) {
		for (int i = 0; i < nBall; i++)
		{
			BallJoint* bj = &joints.balls[i];
			QuaMinJerkTrajectory qt = QuaMinJerkTrajectory(bj->initialOri, bj->ball->GetJoint()->GetPosition(), Vec3d(), Vec3d(), movtime, per);
			for (int j = 0; j < movtime; j++) {
				bj->ori[0][j] = qt.GetCurrentQuaternion(j + 1);
			}
		}
		for (int i = 0; i < nHinge; i++)
		{
			HingeJoint* hj = &joints.hinges[i];
			AngleMinJerkTrajectory at = AngleMinJerkTrajectory(hj->initialAngle, hj->hinge->GetJoint()->GetPosition(), 0, 0, 0, 0, movtime, per);
			for (int j = 0; j < movtime; j++) {
				hj->angle[0][j] = at.GetCurrentAngle(j + 1);
			}
		}
	}

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	Debug();
	/*/
	LoadScene();
	*/

	//joints.Harden();

	scene->GetIKEngine()->Enable(false);

	for (int i = 0; i < movtime; i++) {
		joints.SetTarget(0, i);
		//joints.SetTargetCurrent();
		Debug();
		scene->Step();
		Debug();
		joints.SaveTorque(i);
		trajData[0][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajData[0][i].Ori() = eef->GetSolid()->GetPose().Ori();
		eefVel = eef->GetSolid()->GetVelocity();
		trajVel[0][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}

	Debug();

	best = 0;
	besttorque = joints.CalcTotalTorqueChange();

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
}

void FWTrajectoryPlanner::MakeMinJerkAndSaveWithViaPoint() {
	//std::ofstream outfile("C:/Users/hirohitosatoh/Desktop/traj.csv", std::ios::out | std::ios::trunc);
	//開始状況とターゲットからMinJerkを作り軌道の初期化
	MinJerkTrajectory minjerk = MinJerkTrajectory(startPoint, targetPoint, viaPoint, scene->GetTimeStep());
	/*
	for (i = 0; i < movtime; i++) {
	for (int k = 0; k < iterate + 1; k++) {
	trajData[k][i] = minjerk.GetCurrentPose(i + 1);
	}
	for (int k = 0; k < iterate; k++) {
	trajDataNotCorrected[k][i] = minjerk.GetCurrentPose(i + 1);
	}
	//outfile << trajData[0][i].px << "," << trajData[0][i].py << "," << trajData[0][i].pz << "," << trajData[0][i].w << "," << trajData[0][i].x << "," << trajData[0][i].y << "," << trajData[0][i].z << std::endl;
	}
	*/

	int viatime = viaPoint.step - 1;

	joints.Harden();

	states->LoadState(scene);
	Debug();
	scene->GetIKEngine()->Enable(true);
	//eef->Enable(true);
	scene->GetIKEngine()->ApplyExactState();
	Debug();

	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	for (int i = 0; i < iterate + 1; i++) {
		trajVel[i][0] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}

	int reach = 0;
	//空間躍度最小軌道を再生して関節角を保存
	for (; reach < movtime; reach++) {
		Posed minjerkTarget = minjerk.GetCurrentPose(reach + 1);
		eef->SetTargetPosition(minjerkTarget.Pos());
		eef->SetTargetOrientation(minjerkTarget.Ori());
		for (int i = 0; i < 1; i++) {
			scene->Step();
		}
		if (reach == viatime) {
			joints.SaveViaPoint(0, viatime);
		}
		Debug();
		for (int i = 0; i < iterate + 1; i++) {
			joints.SavePosition(i, reach);
		}
		trajData[0][reach] = minjerk.GetCurrentPose(reach + 1);
		//trajData[0][reach].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		//trajData[0][reach].Ori() = eef->GetSolid()->GetPose().Ori();
	} //多分他の方法を使うべき

	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].targetOri = joints.balls[i].ball->GetJoint()->GetPosition();
		joints.balls[i].targetVel = joints.balls[i].ball->GetJoint()->GetVelocity();
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].targetAngle = joints.hinges[i].hinge->GetJoint()->GetPosition();
		joints.hinges[i].targetVel = joints.hinges[i].hinge->GetJoint()->GetVelocity();
	}

	double per = scene->GetTimeStep();
	if (jointMJT) {
		for (int i = 0; i < nBall; i++)
		{
			BallJoint* bj = &joints.balls[i];
			QuaMinJerkTrajectory qt = QuaMinJerkTrajectory(bj->initialOri, bj->ball->GetJoint()->GetPosition(), Vec3d(), Vec3d(), movtime, per);
			for (int j = 0; j < movtime; j++) {
				bj->ori[0][j] = qt.GetCurrentQuaternion(j + 1);
			}
		}
		for (int i = 0; i < nHinge; i++)
		{
			HingeJoint* hj = &joints.hinges[i];
			AngleMinJerkTrajectory at = AngleMinJerkTrajectory(hj->initialAngle, hj->targetAngle, hj->initialVel, hj->targetVel, hj->viaAngles[0], hj->viaVels[0], movtime - viatime, viatime, per);
			for (int j = 0; j < movtime; j++) {
				hj->angle[0][j] = at.GetCurrentAngle(j + 1);
			}
		}
	}

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	Debug();
	/*/
	LoadScene();
	*/

	//joints.Harden();

	scene->GetIKEngine()->Enable(false);

	for (int i = 0; i < movtime; i++) {
		joints.SetTarget(0, i);
		//joints.SetTargetCurrent();
		Debug();
		scene->Step();
		if (i == viatime) {
			joints.SaveViaPoint(0, viatime);
			cstates->SaveState(scene);
		}
		Debug();
		joints.SaveTorque(i);
		//trajData[0][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		//trajData[0][i].Ori() = eef->GetSolid()->GetPose().Ori();
		eefVel = eef->GetSolid()->GetVelocity();
		trajVel[0][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}

	Debug();

	best = 0;
	besttorque = joints.CalcTotalTorqueChange();

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
}

void FWTrajectoryPlanner::JointForward(int k, bool via) {
	//std::ofstream outfile("C:/Users/hirohitosatoh/Desktop/traj.csv", std::ios::out | std::ios::app);
	scene->GetIKEngine()->Enable(false);
	joints.Soften();
	for (int i = 0; i < movtime; i++) {
		joints.SetOffsetFromLPF(i);
		scene->Step();
		joints.SavePositionFromLPF(k, i);

		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Ori() = eef->GetSolid()->GetPose().Ori();
	}
	joints.ResetOffset();
	DSTR << "in forward end" << std::endl;
	Debug();

	//spring,damperをもとの値に
	joints.Harden();

	//目標位置への補正
	if (correction == 1) {
		if (via) {
			JointTrajCorrectionWithViaPoint(k);
		}
		else {
			JointTrajCorrection(k);
		}
	}

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
}

void FWTrajectoryPlanner::JointInverse(int k, bool via) {
	torquechange.clear();
	totalchange = 0;
	int viatime = viaPoint.step - 1;
	scene->GetIKEngine()->Enable(false);
	joints.Harden();
	//軌道再生と各関節トルクの保存
	for (int i = 0; i < movtime; i++) {

		//jointTargetを設定
		joints.SetTarget(k, i);

		//Stepで軌道再生
		scene->Step();

		if (i == viatime) {
			tmpstates[0]->SaveState(scene);
		}
		trajData[k][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajData[k][i].Ori() = eef->GetSolid()->GetPose().Ori();

		Vec3d eefVel = eef->GetSolid()->GetVelocity();
		trajVel[k][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());

		//outfile << trajData[k][i].px << "," << trajData[k][i].py << "," << trajData[k][i].pz << std::endl;

		//Step前とのトルク差を取得
		DSTR << torquechange << std::endl;

		//トルクの記録(ignoreMotorsによる分岐)
		joints.SaveTorque(i);
	}
	DSTR << "in inverse end" << std::endl;
	Debug();

	double torqueChange = joints.CalcTotalTorqueChange();
	if (torqueChange < besttorque) {
		best = k;
		besttorque = torqueChange;
		if (via) {
			tmpstates[0]->LoadState(scene);
			Debug();
		}
		cstates->SaveState(scene);
	}

	//ロードしてIK周りの調整
	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();   //IKの一時的目標角?のリセット
	Debug();
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
			int nBall = joints.balls.size();
			int nHinge = joints.hinges.size();
			for (int i = 0; i < nBall; i++)
			{
				BallJoint* bj = &joints.balls[i];
				bj->ball->GetJoint()->SetTargetPosition(joints.balls[i].initialOri);
			}
			for (int i = 0; i < nHinge; i++)
			{
				HingeJoint* hj = &joints.hinges[i];
				hj->hinge->GetJoint()->SetTargetPosition(joints.hinges[i].initialAngle);
			}
			repCount++;
		}
	}
	//DSTR << now << eef->GetTargetPosition() << eef->GetSolid()->GetPose() << std::endl;
}

void FWTrajectoryPlanner::JointTrajCorrection(int k) {   //目標と暫定終端間でMinJerkを作り修正
	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].vel = joints.balls[i].ball->GetJoint()->GetVelocity();
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].vel = joints.hinges[i].hinge->GetJoint()->GetVelocity();
	}
	int i = 0;
	if (!staticTarget) {
		scene->GetIKEngine()->Enable(true);
		eef->SetTargetPosition(targetPoint.pose.Pos());
		eef->SetTargetOrientation(targetPoint.pose.Ori());
		double dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
		while (i++ < 50 && dist > 0.005) {
			scene->Step();
			dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
		}
		DSTR << "in correction IK step after " << i << " steps" << std::endl;
		Debug();  //到達はしている
		DSTR << "dist:" << dist << std::endl;
		for (int i = 0; i < nBall; i++)
		{
			joints.balls[i].targetOri = joints.balls[i].ball->GetJoint()->GetPosition();
		}
		for (int i = 0; i < nHinge; i++)
		{
			joints.hinges[i].targetAngle = joints.hinges[i].hinge->GetJoint()->GetPosition();
		}
	}
}

void FWTrajectoryPlanner::JointTrajCorrectionWithViaPoint(int k) {
	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].vel = joints.balls[i].ball->GetJoint()->GetVelocity();
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].vel = joints.hinges[i].hinge->GetJoint()->GetVelocity();
	}
	int i = 0;
	if (!staticTarget) {
		scene->GetIKEngine()->Enable(true);
		eef->SetTargetPosition(targetPoint.pose.Pos());
		eef->SetTargetOrientation(targetPoint.pose.Ori());
		double dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
		while (i++ < 50 && dist > 0.005) {
			scene->Step();
			dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
		}
		DSTR << "in correction IK step after " << i << " steps" << std::endl;
		Debug();  //到達はしている
		DSTR << "dist:" << dist << std::endl;
		for (int i = 0; i < nBall; i++)
		{
			joints.balls[i].targetOri = joints.balls[i].ball->GetJoint()->GetPosition();
		}
		for (int i = 0; i < nHinge; i++)
		{
			joints.hinges[i].targetAngle = joints.hinges[i].hinge->GetJoint()->GetPosition();
		}
	}
	tmpstates[0]->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	eef->SetTargetPosition(viaPoint.pose.Pos());
	eef->SetTargetOrientation(viaPoint.pose.Ori());
	double dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoint.pose.Pos()).norm();
	i = 0;
	while (i++ < 50 && dist > 0.005) {
		scene->Step();
		dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoint.pose.Pos()).norm();
	}
	for (int j = 0; j < nBall; j++)
	{
		joints.balls[j].deltaViaOri = joints.balls[j].ball->GetJoint()->GetPosition();
	}
	for (int j = 0; j < nHinge; j++)
	{
		joints.hinges[j].deltaViaAngle = joints.hinges[j].hinge->GetJoint()->GetPosition();
	}
	joints.TrajectoryCorrectionWithVia(k, staticTarget);
	scene->GetIKEngine()->Enable(false);
}

void FWTrajectoryPlanner::Debug() {   //Debug用にいろいろ表示する
	int nsolids = scene->NSolids();
	for (int i = 0; i < nsolids; i++) {
		PHSolidIf* so = scene->GetSolids()[i];
		DSTR << "Solid" << i << ": pose" << so->GetPose() << " velocity" << so->GetVelocity() << " force" << so->GetForce() << " torque" << so->GetTorque() << std::endl;
	}
	joints.ShowInfo();
	DSTR << "EndeffectorPosition:" << eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() << std::endl;
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
			outfile << trajVel[i][j].x << "," << trajVel[i][j].y << "," << trajVel[i][j].z << "," << trajVel[i][j].w <<"," << std::endl;
		}
	}
	std::ofstream outfile2(filename + "VelocityNotCorrected.csv");
	for (int i = 0; i < iterate; i++) {
		for (int j = 0; j < movtime + 1; j++) {
			outfile2 << trajVelNotCorrected[i][j].x << "," << trajVelNotCorrected[i][j].y << "," << trajVelNotCorrected[i][j].z << "," << trajVelNotCorrected[i][j].w << "," << std::endl;
		}
	}
	std::ofstream outfile3(filename + "VelocityDelta.csv");
	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < iterate; i++) {
		for (int j = 0; j < movtime; j++) {
			for (int k = 0; k < nBall; k++)
			{
				
			}
			for (int k = 0; k < nHinge; k++)
			{
				outfile3 << joints.hinges[k].CorrTraj[i][j] << ",";
			}
			outfile3 << std::endl;
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
	this->ite = ite;
	if (moving) return;
	moving = true;
	noncorrectedReplay = noncorrected;
	torquechange.clear();
	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	repCount = -50;
}

double FWTrajectoryPlanner::GetTotalChange() { return totalchange; }

int FWTrajectoryPlanner::GetBest() { return best; }

void FWTrajectoryPlanner::FIRM(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt) {
	// --- デバッグ用の出力ファイル準備 ---
	//ss << ".csv";
	std::string filename;
	PrepareOutputFilename(filename);
	DSTR << filename << std::endl;
	std::ofstream torqueChangeOutput(output + "TorqueChange.csv");

	// --- 計算に際してのいろいろな初期化 ---
	// CalcTrajectory内で一度同じことしてるからどちらを残すか考える
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	startPoint.pose = Posed(eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition(), eef->GetSolid()->GetPose().Ori());
	startPoint.vel = Vec6d(eefVel.x, eefVel.y, eefVel.z, 0, 0, 0);
	startPoint.step = 0;
	targetPoint = tpoint;
	movtime = targetPoint.step;
	DSTR << "stapose:" << startPoint.pose << " tarvel:" << startPoint.vel << " time:" << startPoint.step << std::endl;
	DSTR << "tarpose:" << targetPoint.pose << " tarvel:" << targetPoint.vel << " time:" << targetPoint.step << std::endl;
	this->staticTarget = staticTarget;
	this->jointMJT = jmjt;
	Init();

	MakeMinJerkAll();
	torqueChangeOutput << besttorque << std::endl;

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

	Debug();

	// --- 軌道計算の繰り返し計算 ---

	scene->GetIKEngine()->Enable(false);

	double torqueChange;

	for (int k = 1; k < iterate + 1; k++) {

		//トルクにLPFをかける
		for (int i = 0; i < smoothCount; i++) {
			joints.ApplyLPF(LPFmode, smoothCount);
		}

		//フォワード(トルクー＞軌道)
		Forward(k);

		//軌道の修正
		if (correction == 1) {
			Correction(k);
		}

		states->LoadState(scene);
		scene->GetIKEngine()->ApplyExactState();

		//インバース(軌道ー＞トルク)
		Inverse(k);

		torqueChange = joints.CalcTotalTorqueChange();
		torqueChangeOutput << torqueChange << std::endl;
		//springdamper小さくする
	}

	//scene->GetIKEngine()->Enable(true);

	// --- 計算後であることのフラグ有効化 ---
	moving = true;
	calced = true;
	ite = best;
	repCount = 0;

	//joints.ResetPD();

	OutputTrajectory(output);
	if (saveNotCorrected) {
		OutputNotCorrectedTrajectory(output);
	}
	OutputVelocity(output);

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
}

bool FWTrajectoryPlanner::ViatimeAdjustment() {
	//トルクの総変化量と各セクションでの変化量を取得
	//目標点前のセクションだけ別にするのは変かもしれない
	int nVia = viaPoints.size();
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

	return !cont;
}

void FWTrajectoryPlanner::ViatimeInitialize() {
	//各経由点の通過時間を等間隔に初期化
	int nVia = viaPoints.size();
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
	for (int i = 0; i < movtime; i++) {
		minjerkPoses[i] = minjerk.GetCurrentPose(i + 1);
	}
	/*
	//各点を単純な躍度最小軌道で接続
	for (int i = 0; i < viaPoints.size() + 1; i++) {
		ControlPoint s = (i == 0) ? startPoint : viaPoints[i - 1];
		ControlPoint f = (i == viaPoints.size()) ? targetPoint : viaPoints[i];
		MinJerkTrajectory mjt = MinJerkTrajectory(s, f, scene->GetTimeStep());
		for (int j = s.step; j < f.step; j++) {
			minjerkPoses[j] = mjt.GetCurrentPose(j + 1);
		}
	}
	/*/
	//境界条件満足の部分軌道を足し合わせて経由点を通る軌道実現
	for (int i = 0; i < viaPoints.size(); i++) {
		ControlPoint s = ControlPoint(Posed(), Vec6d(), (i == 0) ? startPoint.step : viaPoints[i - 1].step, (i == 0) ? startPoint.time : viaPoints[i - 1].time);
		ControlPoint f = ControlPoint(Posed(), Vec6d(), movtime, targetPoint.time);
		ControlPoint c = viaPoints[i];
		c.pose.Pos() -= minjerkPoses[c.step - 1].Pos();
		c.pose.Ori() = c.pose.Ori() * minjerkPoses[c.step - 1].Ori().Inv();
		MinJerkTrajectory mjt = MinJerkTrajectory(s, f, c, scene->GetTimeStep());
		for (int j = s.step; j < f.step; j++) {
			Posed delta = mjt.GetCurrentPose(j + 1);
			minjerkPoses[j].Pos() += delta.Pos();
			minjerkPoses[j].Ori() = minjerkPoses[j].Ori() * delta.Ori();
		}
	}
	
	joints.Harden();

	states->LoadState(scene);
	Debug();
	scene->GetIKEngine()->Enable(true);
	//eef->Enable(true);
	scene->GetIKEngine()->ApplyExactState();
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
	//空間躍度最小軌道を再生して関節角を保存
	for (; reach < movtime; reach++) {
		Posed minjerkTarget = minjerkPoses[reach];
		eef->SetTargetPosition(minjerkTarget.Pos());
		eef->SetTargetOrientation(minjerkTarget.Ori());
		for (int i = 0; i < 1; i++) {
			scene->Step();
		}
		if (count < viaPoints.size()) {
			if (reach == (viaPoints[count].step - 1)) {
				joints.SaveViaPoint(count, (viaPoints[count].step));
				count++;
			}
		}
		Debug();
		for (int i = 0; i < iterate + 1; i++) {
			joints.SavePosition(i, reach);
		}
		trajData[0][reach] = minjerkPoses[reach];
		//trajData[0][reach].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		//trajData[0][reach].Ori() = eef->GetSolid()->GetPose().Ori();
	} //多分他の方法を使うべき

	joints.SaveViaPoint(count, movtime);

	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	/*
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].targetOri = joints.balls[i].ball->GetJoint()->GetPosition();
		joints.balls[i].targetVel = joints.balls[i].ball->GetJoint()->GetVelocity();
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].targetAngle = joints.hinges[i].hinge->GetJoint()->GetPosition();
		joints.hinges[i].targetVel = joints.hinges[i].hinge->GetJoint()->GetVelocity();
	}
	*/

	joints.SaveTarget();

	//関節次元躍度最小軌道の生成
	//２パターン用意してるけどどっちがいいかは未検討
	double per = scene->GetTimeStep();
	if (jointMJT) {
		for (int i = 0; i < nBall; i++)
		{
			BallJoint* bj = &joints.balls[i];
			
			QuaMinJerkTrajectory qmjt = QuaMinJerkTrajectory(bj->initialOri, bj->targetOri, bj->initialVel, bj->targetVel, Vec3d(), Vec3d(), movtime, scene->GetTimeStep());
			for (int j = 0; j < movtime; j++) {
				bj->ori[0][j] = qmjt.GetCurrentQuaternion(j + 1);
			}
			for (int j = 0; j < viaPoints.size(); j++) {
				int st = (j == 0) ? startPoint.step : viaPoints[j - 1].step;
				Quaterniond v = bj->viaOris[j] * bj->ori[0][viaPoints[j].step - 1].Inv();
				int t = movtime - st;
				int vt = viaPoints[j].step - st;
				QuaMinJerkTrajectory dqmjt = QuaMinJerkTrajectory(v, t, vt, scene->GetTimeStep());
				for (int k = 0; k < t; k++) {
					bj->ori[0][st + k] = bj->ori[0][st + k] * dqmjt.GetCurrentQuaternion(k + 1);
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
		for (int i = 0; i < nHinge; i++)
		{
			HingeJoint* hj = &joints.hinges[i];
			
			AngleMinJerkTrajectory hmjt = AngleMinJerkTrajectory(hj->initialAngle, hj->targetAngle, hj->initialVel, hj->targetVel, 0, 0, movtime, scene->GetTimeStep());
			for (int j = 0; j < movtime; j++) {
				hj->angle[0][j] = hmjt.GetCurrentAngle(j + 1);
			}
			for (int j = 0; j < viaPoints.size(); j++) {
				int st = (j == 0) ? startPoint.step : viaPoints[j - 1].step;
				double v = hj->viaAngles[j] - hj->angle[0][viaPoints[j].step - 1];
				int t = movtime - st;
				int vt = viaPoints[j].step - st;
				AngleMinJerkTrajectory dhmjt = AngleMinJerkTrajectory(v, t, vt, scene->GetTimeStep());
				for (int k = 0; k < t; k++) {
					hj->angle[0][st + k] += dhmjt.GetCurrentAngle(k + 1);
				}
			}
			/*/
			for (int j = 0; j < viaPoints.size() + 1; j++) {
				double s = (j == 0) ? hj->initialAngle : hj->viaAngles[j - 1];
				double vs = (j == 0) ? hj->initialVel : hj->viaVels[j - 1];
				double f = hj->viaAngles[j];
				double vf = hj->viaVels[j];
				//double f = (j == viaPoints.size()) ? hj->targetAngle : hj->viaAngles[j];
				//double vf = (j == viaPoints.size()) ? hj->targetVel : hj->viaVels[j];
				int st = (j == 0 ? 0 : viaPoints[j - 1].step);
				int time = (j == viaPoints.size() ? movtime : viaPoints[j].step) - st;
				DSTR << s << " " << vs << " " << f << " " << vf << " " << st << " " << time << std::endl;
				AngleMinJerkTrajectory mjt = AngleMinJerkTrajectory(s, f, vs, vf, 0, 0, time, scene->GetTimeStep());
				for (int k = 0; k < time; k++) {
					hj->angle[0][k + st] = mjt.GetCurrentAngle(k + 1);
					DSTR << k + st << std::endl;
				}
			}
			*/
		}
	}

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
	Debug();
	/*/
	LoadScene();
	*/

	//joints.Harden();

	scene->GetIKEngine()->Enable(false);

	count = 0;
	for (int i = 0; i < movtime; i++) {
		joints.SetTarget(0, i);
		//joints.SetTargetCurrent();
		Debug();
		scene->Step();
		if (count < viaPoints.size()) {
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

	Debug();

	best = 0;
	besttorque = joints.CalcTotalTorqueChange();
	joints.SetBestTorqueChange();

	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();
}

void FWTrajectoryPlanner::Forward(int k) {
	scene->GetIKEngine()->Enable(false);
	joints.Soften();
	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < nBall; i++)
	{
		DSTR << joints.balls[i].initialTorque << " ";
	}
	for (int i = 0; i < nHinge; i++)
	{
		DSTR << joints.hinges[i].initialTorque << " ";
	}
	DSTR << std::endl;
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	trajVelNotCorrected[k > 0 ? k - 1 : 0][0] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	for (int i = 0; i < movtime; i++) {
		joints.SetOffsetFromLPF(i);
		scene->Step();
		joints.SavePositionFromLPF(k, i);

		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Ori() = eef->GetSolid()->GetPose().Ori();

		eefVel = eef->GetSolid()->GetVelocity();
		trajVelNotCorrected[k > 0 ? k - 1 : 0][i + 1] = Vec4d(eefVel.x, eefVel.y, eefVel.z, eefVel.norm());
	}
	joints.ResetOffset();
	DSTR << "in forward end" << std::endl;
	Debug();

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

		//Stepで軌道再生
		scene->Step();

		if (count < viaPoints.size()) {
			if (i == (viaPoints[count].step - 1)) {
				tmpstates[count++]->SaveState(scene);
			}
		}
		trajData[k][i].Pos() = eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition();
		trajData[k][i].Ori() = eef->GetSolid()->GetPose().Ori();

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
	//なので、ここで到達位置や速度を保存しておく
	int nBall = joints.balls.size();
	int nHinge = joints.hinges.size();
	for (int i = 0; i < nBall; i++)
	{
		joints.balls[i].vel = joints.balls[i].ball->GetJoint()->GetVelocity();
		DSTR << joints.balls[i].ball->GetJoint()->GetVelocity() << " " << joints.balls[i].vel << std::endl;
	}
	for (int i = 0; i < nHinge; i++)
	{
		joints.hinges[i].vel = joints.hinges[i].hinge->GetJoint()->GetVelocity();
	}

	double dist;
	int count;
	scene->GetIKEngine()->Enable(true);

	//IKを用いて目標に到達させ、jointsに状態保存
	if (!staticTarget) {
		eef->SetTargetPosition(targetPoint.pose.Pos());
		eef->SetTargetOrientation(targetPoint.pose.Ori());
		count = 0;
		double dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
		while (count++ < 50 && dist > 0.005) {
			scene->Step();
			dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - targetPoint.pose.Pos()).norm();
		}
		DSTR << "in correction IK step after " << count << " steps" << std::endl;
		Debug();
		DSTR << "dist:" << dist << std::endl;
		/*
		for (int i = 0; i < nBall; i++)
		{
			joints.balls[i].targetOri = joints.balls[i].ball->GetJoint()->GetPosition();
			joints.balls[i].viaOris[viaPoints.size()] = joints.balls[i].ball->GetJoint()->GetPosition();
		}
		for (int i = 0; i < nHinge; i++)
		{
			joints.hinges[i].targetAngle = joints.hinges[i].hinge->GetJoint()->GetPosition();
			joints.hinges[i].viaAngles[viaPoints.size()] = joints.hinges[i].hinge->GetJoint()->GetPosition();
		}
		*/
		joints.SaveTarget();
	}

	//以降では経由点の通過保証修正を行う
	for (int n = 0; n < viaPoints.size(); n++) {
		tmpstates[n]->LoadState(scene);
		scene->GetIKEngine()->ApplyExactState();
		eef->SetTargetPosition(viaPoints[n].pose.Pos());
		eef->SetTargetOrientation(viaPoints[n].pose.Ori());
		count = 0;
		dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoint.pose.Pos()).norm();
		while (count++ < 50 && dist > 0.005) {
			scene->Step();
			dist = (eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition() - viaPoint.pose.Pos()).norm();
		}
		DSTR << "ViaPoint " << n << " in correction IK step after " << count << " steps" << std::endl;
		Debug();
		for (int i = 0; i < nBall; i++)
		{
			joints.balls[i].viaOris[n] = joints.balls[i].ball->GetJoint()->GetPosition();
		}
		for (int i = 0; i < nHinge; i++)
		{
			joints.hinges[i].viaAngles[n] = joints.hinges[i].hinge->GetJoint()->GetPosition();
		}
	}
	joints.TrajectoryCorrection(k, staticTarget);
	scene->GetIKEngine()->Enable(false);
}

void FWTrajectoryPlanner::CalcTrajectory(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt) {

	//開始点の設定
	Vec3d eefVel = eef->GetSolid()->GetVelocity();
	startPoint.pose = Posed(eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition(), eef->GetSolid()->GetPose().Ori());
	startPoint.vel = Vec6d(eefVel.x, eefVel.y, eefVel.z, 0, 0, 0);
	startPoint.step = 0;
	startPoint.time = 0;
	
	//目標点の設定(引数とってくるだけ)
	targetPoint = tpoint;

	//総移動ステップ数設定
	movtime = targetPoint.step;

	DSTR << "stapose:" << startPoint.pose << " tarvel:" << startPoint.vel << " time:" << startPoint.step << std::endl;
	DSTR << "tarpose:" << targetPoint.pose << " tarvel:" << targetPoint.vel << " time:" << targetPoint.step << std::endl;

	this->staticTarget = staticTarget;
	this->jointMJT = jmjt;

	auto start = std::chrono::system_clock::now();

	//経由点の通過時間の初期化
	ViatimeInitialize();

	//経由点通過時間推移記録ファイル
	std::ofstream outfile(output + "Viatimes.csv");
	std::ofstream outfile2(output + "Bests.csv");

	bool cont = true;
	for (int i = 0; i < (iterateViaAdjust + 1) && cont; i++) {
		//現在の経由点通過時間を記録
		for (int j = 0; j < viaPoints.size(); j++) {
			outfile << viaPoints[j].step << ",";
		}
		outfile << std::endl;
		//FIRMにより近似最適解の導出
		FIRM(tpoint, LPFmode, smoothCount, output, bChange, pChange, staticTarget, jmjt);
		outfile2 << besttorque << std::endl;
		//経由点の通過時間の調整をして継続か判断
		cont = ViatimeAdjustment();
	}

	//joints.ResetPD();
	/*
	OutputTrajectory(output);
	if (saveNotCorrected) {
		OutputNotCorrectedTrajectory(output);
	}
	OutputVelocity(output);
	*/
	states->LoadState(scene);
	scene->GetIKEngine()->ApplyExactState();   //IKの一時的目標角?のリセット

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	//outfile3 << msec << std::endl;
}

}
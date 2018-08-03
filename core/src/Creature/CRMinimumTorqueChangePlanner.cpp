/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

#include <Creature/CRMinimumTorqueChangePlanner.h>
#include <Creature/CRMinimumJerkTrajectory.h>

namespace Spr {;

ViaPoint::ViaPoint() {}

ViaPoint::ViaPoint(Posed p, double t) {}

ViaPoint::ViaPoint(Posed p, SpatialVector v, SpatialVector a, double t) {}

CRTrajectoryPlanner::HingeJoint::HingeJoint(PHIKHingeActuatorIf* hinge, std::string path, bool oe) {
	this->hinge = hinge;
	outputEnable = oe;
	torGraph = new std::ofstream(path + "Torque.csv");
	torChangeGraph = new std::ofstream(path + "TorqueChange.csv");
}
CRTrajectoryPlanner::HingeJoint::~HingeJoint() {
	CloseFile();
	delete torGraph;
	delete torChangeGraph;
}
void CRTrajectoryPlanner::HingeJoint::Initialize(int iterate, double mtime, int nVia, double rate, bool vCorr) {
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
void CRTrajectoryPlanner::HingeJoint::MakeJointMinjerk(int cnt) {
	double dt = hinge->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = hinge->GetJoint()->GetScene()->GetTimeStepInv();
	CRAngleMinimumJerkTrajectory hmjt = CRAngleMinimumJerkTrajectory();
	for (int j = 0; j < movetime; j++) {
		angle[cnt][j] = hmjt.GetPosition((j + 1) * dt);
		angleVels[cnt][j] = hmjt.GetVelocity((j + 1) * dt);
	}
}
void CRTrajectoryPlanner::HingeJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void CRTrajectoryPlanner::HingeJoint::SaveTorque(int n) {
	//試しにLimitForce引いてみる
	torque[n] = hinge->GetJoint()->GetMotorForceN(0);
	torque[n] += hinge->GetJoint()->GetLimitForce();
	//torque[n] += hinge->GetJoint()->GetMotorForceN(1);
	torqueLPF[n] = torque[n];
}
void CRTrajectoryPlanner::HingeJoint::SaveTarget() {
	targetAngle = hinge->GetJoint()->GetPosition();
	targetVel = hinge->GetJoint()->GetVelocity();
}
void CRTrajectoryPlanner::HingeJoint::SetTarget(int k, int n) {
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

void CRTrajectoryPlanner::HingeJoint::SetTargetVelocity(int k, int n) {
	if (k >= 0 && k <= iterate) {
		hinge->GetJoint()->SetTargetVelocity(angleVels[k][n]);
		//hinge->GetJoint()->SetTargetVelocity(hinge->GetJoint()->GetVelocity());
	}
}

void CRTrajectoryPlanner::HingeJoint::SetTargetInitial() {
	hinge->GetJoint()->SetTargetPosition(initialAngle);
	hinge->GetJoint()->SetTargetVelocity(initialVel);
}
void CRTrajectoryPlanner::HingeJoint::SetOffsetFromLPF(int n) {
	hinge->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
}
void CRTrajectoryPlanner::HingeJoint::ResetOffset(double o) {
	int nMotors = hinge->GetJoint()->NMotors();
	for (int i = 0; i < nMotors; i++) {
		hinge->GetJoint()->SetOffsetForceN(i, o);
	}
}
void CRTrajectoryPlanner::HingeJoint::SavePosition(int k, int n) {
	angle[k][n] = hinge->GetJoint()->GetPosition();
}
void CRTrajectoryPlanner::HingeJoint::SaveVelocity(int k, int n) {
	angleVels[k][n] = hinge->GetJoint()->GetVelocity();
}

void CRTrajectoryPlanner::HingeJoint::SaveViaPoint(int v, int t) {
	viaAngles[v] = hinge->GetJoint()->GetPosition();
	viaVels[v] = hinge->GetJoint()->GetVelocity();
	viatimes[v] = t;
}
void CRTrajectoryPlanner::HingeJoint::SavePositionFromLPF(int k, int n) {
	angleLPF[k][n] = hinge->GetJoint()->GetPosition();
	angle[k][n] = hinge->GetJoint()->GetPosition();
}
void CRTrajectoryPlanner::HingeJoint::SaveVelocityFromLPF(int k, int n) {
	angleVelsLPF[k][n] = hinge->GetJoint()->GetVelocity();
	angleVels[k][n] = hinge->GetJoint()->GetVelocity();
}
void CRTrajectoryPlanner::HingeJoint::TrajectoryCorrection(int k, bool s) {
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

	CRAngleMinimumJerkTrajectory delta = CRAngleMinimumJerkTrajectory();
	DSTR << "target:" << targetAngle << " actual:" << delta.GetPosition(mtime) << std::endl;
	for (int i = 0; i < movetime - stime; i++) {
		angle[k][i + stime] = delta.GetDeltaPosition((i + 1) * dt) + angleLPF[k][i + stime]; //k+1でいいのか？
		angleLPF[k][i + stime] += delta.GetDeltaPosition((i + 1) * dt);
		//angleVels[k][i + stime] = delta.GetCurrentVelocity((i + 1) * dt) * perInv + angleVelsLPF[k][i + stime];
		angleVels[k][i + stime] = (angle[k][i + stime] - (i + stime > 0 ? angle[k][i + stime - 1] : initialAngle)) * dtInv;
		angleVelsLPF[k][i + stime] += delta.GetVelocity((i + 1) * dt) * dtInv;
		//CorrTraj[k - 1][i + stime] = delta.GetCurrentVelocity(i * dt);
	}
}
void CRTrajectoryPlanner::HingeJoint::ApplyLPF(int count) {
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torqueLPF, 3, 1, initialTorque);
	}
	if (count > 0) {
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = rateLPF * torqueLPF[i] + (1 - rateLPF) * initialTorque;
		}
	}
}
void CRTrajectoryPlanner::HingeJoint::Soften() {
	hinge->GetJoint()->SetSpring(0);
	hinge->GetJoint()->SetDamper(0);
}
void CRTrajectoryPlanner::HingeJoint::Harden() {
	if (mul) {
		hinge->GetJoint()->SetSpring(originalSpring * hardenSpring);
		hinge->GetJoint()->SetDamper(originalDamper * hardenDamper);
	}
	else {
		hinge->GetJoint()->SetSpring(hardenSpring);
		hinge->GetJoint()->SetDamper(hardenDamper);
	}
}
void CRTrajectoryPlanner::HingeJoint::ResetPD() {
	hinge->GetJoint()->SetSpring(originalSpring * 1e-5);
	hinge->GetJoint()->SetDamper(originalDamper * 1e-5);
}
double CRTrajectoryPlanner::HingeJoint::CalcTotalTorqueChange() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		total += pow(abs(torque[i] - (((i - 1) < 0) ? initialTorque : torque[i - 1])), 2);
	}
	DSTR << total << std::endl;
	return total;
}

double CRTrajectoryPlanner::HingeJoint::CalcTotalTorqueChangeLPF() {
	double total = 0;
	for (int i = 0; i < movetime; i++) {
		total += pow(abs(torqueLPF[i] - (((i - 1) < 0) ? initialTorque : torqueLPF[i - 1])), 2);
	}
	DSTR << total << std::endl;
	return total;
}
double CRTrajectoryPlanner::HingeJoint::CalcTorqueChangeInSection(int n) {
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
double CRTrajectoryPlanner::HingeJoint::GetBestTorqueChangeInSection(int n) {
	if (n < 0 || n >= viatimes.size()) return 0;
	return tChanges[n];
}
void CRTrajectoryPlanner::HingeJoint::SetBestTorqueChange() {
	for (int n = 0; n < viatimes.size(); n++) {
		tChanges[n] = CalcTorqueChangeInSection(n);
	}
}
void CRTrajectoryPlanner::HingeJoint::ShowInfo() {
	DSTR << "Pose:" << hinge->GetJoint()->GetPosition() << " Target:" << hinge->GetJoint()->GetTargetPosition() << " Vel:" << hinge->GetJoint()->GetVelocity() << " TarVel:" << hinge->GetJoint()->GetTargetVelocity() << std::endl;
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
void CRTrajectoryPlanner::HingeJoint::SetTargetCurrent() {
	hinge->GetJoint()->SetTargetPosition(hinge->GetJoint()->GetPosition());
}
void CRTrajectoryPlanner::HingeJoint::SetPD(double s, double d, bool mul) {
	this->hardenSpring = s;
	this->hardenDamper = d;
	this->mul = mul;
}
void CRTrajectoryPlanner::HingeJoint::SetTargetFromLPF(int k, int n) {
	if (k >= 0 && k <= iterate) {
		hinge->GetJoint()->SetTargetPosition(angleLPF[k][n]);
	}
}
void CRTrajectoryPlanner::HingeJoint::UpdateIKParam(double b, double p) {
	//hinge->SetBias(b);
	hinge->SetPullbackRate(p);
}

void CRTrajectoryPlanner::HingeJoint::OutputTorque() {
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

CRTrajectoryPlanner::BallJoint::BallJoint(PHIKBallActuatorIf* ball, std::string path, bool oe) {
	this->ball = ball;
	outputEnable = oe;
	torGraph = new std::ofstream(path + "Torque.csv");
	torChangeGraph = new std::ofstream(path + "TorqueChange.csv");
}
CRTrajectoryPlanner::BallJoint::~BallJoint() {
	CloseFile();
	delete torGraph;
	delete torChangeGraph;
}
void CRTrajectoryPlanner::BallJoint::Initialize(int iterate, double mtime, int nVia, double rate, bool vCorr) {
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
void CRTrajectoryPlanner::BallJoint::MakeJointMinjerk(int cnt) {
	double dt = ball->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = ball->GetJoint()->GetScene()->GetTimeStepInv();
	CRQuaternionMinimumJerkTrajectory qmjt = CRQuaternionMinimumJerkTrajectory();

	for (int j = 0; j < movetime; j++) {
		ori[cnt][j] = qmjt.GetPosition((j + 1) * dt);
		oriVels[cnt][j] = qmjt.GetVelocity((j + 1) * dt);
	}
}
void CRTrajectoryPlanner::BallJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void CRTrajectoryPlanner::BallJoint::SaveTorque(int n) {
	this->torque[n] = ball->GetJoint()->GetMotorForceN(0);
	//torque[n] += ball->GetJoint()->GetLimitForce();
	//this->torque[n] += ball->GetJoint()->GetMotorForceN(1);
	this->torqueLPF[n] = torque[n];
}
void CRTrajectoryPlanner::BallJoint::SaveTarget() {
	targetOri = ball->GetJoint()->GetPosition();
	targetVel = ball->GetJoint()->GetVelocity();
}
void CRTrajectoryPlanner::BallJoint::SetTarget(int k, int n) {
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
void CRTrajectoryPlanner::BallJoint::SetTargetVelocity(int k, int n) {
	if (k >= 0 && k <= iterate) {
		ball->GetJoint()->SetTargetVelocity(oriVels[k][n]);
		//ball->GetJoint()->SetTargetVelocity(ball->GetJoint()->GetVelocity());
	}
}
void CRTrajectoryPlanner::BallJoint::SetTargetInitial() {
	ball->GetJoint()->SetTargetPosition(initialOri);
	ball->GetJoint()->SetTargetVelocity(initialVel);
}
void CRTrajectoryPlanner::BallJoint::SetOffsetFromLPF(int n) {
	ball->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
	DSTR << torqueLPF[n] << " ";
	//ball->GetJoint()->SetOffsetForceN(0, Vec3d(1, 0, 0));
}
void CRTrajectoryPlanner::BallJoint::ResetOffset(double o) {
	int nMotors = ball->GetJoint()->NMotors();
	for (int i = 0; i < nMotors; i++) {
		ball->GetJoint()->SetOffsetForceN(i, Vec3d(o, o, o));
	}
}
void CRTrajectoryPlanner::BallJoint::SavePosition(int k, int n) {
	ori[k][n] = ball->GetJoint()->GetPosition();
}
void CRTrajectoryPlanner::BallJoint::SaveVelocity(int k, int n) {
	oriVels[k][n] = ball->GetJoint()->GetVelocity();
	if (k == 0) {
		Vec3d calced = ori[k][n].AngularVelocity(ori[k][n] - (n > 0 ? ori[k][n - 1] : initialOri));
		DSTR << "comp vel rec:" << oriVels[k][n] << " calced:" << calced << std::endl;
	}
}

void CRTrajectoryPlanner::BallJoint::SaveViaPoint(int v, int t) {
	viaOris[v] = ball->GetJoint()->GetPosition();
	viaVels[v] = ball->GetJoint()->GetVelocity();
	viatimes[v] = t;
}
void CRTrajectoryPlanner::BallJoint::SavePositionFromLPF(int k, int n) {
	oriLPF[k][n] = ball->GetJoint()->GetPosition();
	ori[k][n] = ball->GetJoint()->GetPosition();
}
void CRTrajectoryPlanner::BallJoint::SaveVelocityFromLPF(int k, int n) {
	oriVelsLPF[k][n] = ball->GetJoint()->GetVelocity();
	oriVels[k][n] = ball->GetJoint()->GetVelocity();
}
void CRTrajectoryPlanner::BallJoint::TrajectoryCorrection(int k, bool s) {
	double dt = ball->GetJoint()->GetScene()->GetTimeStep();
	double dtInv = ball->GetJoint()->GetScene()->GetTimeStepInv();

	// 終端条件保証
	double stime = (int)(mtime * 0.0);

	Quaterniond end = oriLPF[k][oriLPF.width() - 1];
	Vec3d endVel = oriVelsLPF[k][oriVels.width() - 1];
	Vec3d endAcc = (oriVelsLPF[k][oriVelsLPF.width() - 1] - oriVelsLPF[k][oriVelsLPF.width() - 2]) * dtInv;

	double time = mtime;

	CRQuaternionMinimumJerkTrajectory delta = CRQuaternionMinimumJerkTrajectory();

	// <!!> stime が doubleです
	for (int i = 0; i < movetime - stime; i++) {
		ori[k][i + stime] = delta.GetPosition((i + 1) * dt) * oriLPF[k][i + stime];
		oriVels[k][i + stime] = ori[k][i + stime].AngularVelocity(ori[k][i + stime] - (i + stime > 0 ? ori[k][i + stime - 1] : initialOri)) * dtInv;

		oriLPF[k][i + stime] = ori[k][i + stime];
		oriVelsLPF[k][i + stime] = oriVels[k][i + stime];
	}
}
void CRTrajectoryPlanner::BallJoint::ApplyLPF(int count) {
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torqueLPF, 3, 1, initialTorque);
	}
	if (count > 0) {
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = rateLPF * torqueLPF[i] + (1 - rateLPF) * initialTorque;
		}
	}
}
void CRTrajectoryPlanner::BallJoint::Soften() {
	ball->GetJoint()->SetSpring(0);
	ball->GetJoint()->SetDamper(0);
}
void CRTrajectoryPlanner::BallJoint::Harden() {
	if (mul) {
		ball->GetJoint()->SetSpring(originalSpring * hardenSpring);
		ball->GetJoint()->SetDamper(originalDamper * hardenDamper);
	}
	else {
		ball->GetJoint()->SetSpring(hardenSpring);
		ball->GetJoint()->SetDamper(hardenDamper);
	}
}
void CRTrajectoryPlanner::BallJoint::ResetPD() {
	ball->GetJoint()->SetSpring(originalSpring * 1e-5);
	ball->GetJoint()->SetDamper(originalDamper * 1e-5);
}
double CRTrajectoryPlanner::BallJoint::CalcTotalTorqueChange() {
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
double CRTrajectoryPlanner::BallJoint::CalcTotalTorqueChangeLPF() {
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
double CRTrajectoryPlanner::BallJoint::CalcTorqueChangeInSection(int n) {
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
double CRTrajectoryPlanner::BallJoint::GetBestTorqueChangeInSection(int n) {
	if (n < 0 || n >= viatimes.size()) return 0;
	return tChanges[n];
}
void CRTrajectoryPlanner::BallJoint::SetBestTorqueChange() {
	for (int n = 0; n < viatimes.size(); n++) {
		tChanges[n] = CalcTorqueChangeInSection(n);
	}
}
void CRTrajectoryPlanner::BallJoint::ShowInfo() {
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
void CRTrajectoryPlanner::BallJoint::SetTargetCurrent() {
	ball->GetJoint()->SetTargetPosition(ball->GetJoint()->GetPosition());
}
void CRTrajectoryPlanner::BallJoint::SetPD(double s, double d, bool mul) {
	this->hardenSpring = s;
	this->hardenDamper = d;
	this->mul = mul;
}
void CRTrajectoryPlanner::BallJoint::SetTargetFromLPF(int k, int n) {
	if (k >= 0 && k <= iterate) {
		ball->GetJoint()->SetTargetPosition(oriLPF[k][n]);
	}
}

void CRTrajectoryPlanner::BallJoint::UpdateIKParam(double b, double p) {
	//ball->SetBias(b);
	ball->SetPullbackRate(p);
}

void CRTrajectoryPlanner::BallJoint::OutputTorque() {
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

CRTrajectoryPlanner::Joints::Joints() { joints.clear(); joints.shrink_to_fit(); }
CRTrajectoryPlanner::Joints::~Joints() { RemoveAll();  std::vector<Joint*>().swap(joints); }
void CRTrajectoryPlanner::Joints::RemoveAll() {
	// 各要素でnewしているためこう？
	for (int i = 0; i < joints.size(); i++) {
		delete joints[i];
	}
	joints.clear();  joints.shrink_to_fit();
}
void CRTrajectoryPlanner::Joints::Add(PHIKActuatorIf* j, std::string path, bool oe) {
	if (DCAST(PHIKBallActuatorIf, j)) {
		BallJoint* b = new BallJoint(DCAST(PHIKBallActuatorIf, j), path + "Ball" + std::to_string((int)joints.size()), oe);
		joints.push_back(b);
	}
	if (DCAST(PHIKHingeActuatorIf, j)) {
		HingeJoint* h = new HingeJoint(DCAST(PHIKHingeActuatorIf, j), path + "Hinge" + std::to_string((int)joints.size()), oe);
		joints.push_back(h);
	}
}
void CRTrajectoryPlanner::Joints::Initialize(int iterate, double movetime, int nVia, double rate, bool vCorr) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->Initialize(iterate, movetime, nVia, rate, vCorr);
	}
}
void CRTrajectoryPlanner::Joints::MakeJointMinjerk(int cnt) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->MakeJointMinjerk(cnt);
	}
}

void CRTrajectoryPlanner::Joints::CloseFile() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->CloseFile();
	}
}

void CRTrajectoryPlanner::Joints::SetTarget(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTarget(k, n);
	}
}

void CRTrajectoryPlanner::Joints::SetTargetVelocity(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetVelocity(k, n);
	}
}

void CRTrajectoryPlanner::Joints::SetTargetInitial() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetInitial();
	}
}

void CRTrajectoryPlanner::Joints::SaveTorque(int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveTorque(n);
	}
}

void CRTrajectoryPlanner::Joints::SaveTarget() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveTarget();
	}
}

void CRTrajectoryPlanner::Joints::SetOffsetFromLPF(int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetOffsetFromLPF(n);
	}
	DSTR << std::endl;
}

void CRTrajectoryPlanner::Joints::ResetOffset(double o) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ResetOffset(o);
	}
}

void CRTrajectoryPlanner::Joints::SavePosition(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SavePosition(k, n);
	}
}

void CRTrajectoryPlanner::Joints::SaveVelocity(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveVelocity(k, n);
	}
}

void CRTrajectoryPlanner::Joints::SaveViaPoint(int v, int t) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveViaPoint(v, t);
	}
}

void CRTrajectoryPlanner::Joints::SavePositionFromLPF(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SavePositionFromLPF(k, n);
	}
}

void CRTrajectoryPlanner::Joints::SaveVelocityFromLPF(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SaveVelocityFromLPF(k, n);
	}
}

void CRTrajectoryPlanner::Joints::TrajectoryCorrection(int k, bool s) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->TrajectoryCorrection(k, s);
	}
}

void CRTrajectoryPlanner::Joints::ApplyLPF(int count) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ApplyLPF(count);
	}
}
void CRTrajectoryPlanner::Joints::Soften() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->Soften();
	}
}
void CRTrajectoryPlanner::Joints::Harden() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->Harden();
	}
}
void CRTrajectoryPlanner::Joints::ResetPD() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ResetPD();
	}
}
double CRTrajectoryPlanner::Joints::CalcTotalTorqueChange() {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTotalTorqueChange();
	}
	return total;
}
double CRTrajectoryPlanner::Joints::CalcTotalTorqueChange(std::ofstream& o) {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTotalTorqueChange();
		o << joints[i]->CalcTotalTorqueChange() << ",";
	}
	return total;
}

double CRTrajectoryPlanner::Joints::CalcTotalTorqueChangeLPF() {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTotalTorqueChangeLPF();
	}
	return total;
}
double CRTrajectoryPlanner::Joints::CalcTorqueChangeInSection(int n) {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->CalcTorqueChangeInSection(n);
	}
	return total;
}
void CRTrajectoryPlanner::Joints::SetBestTorqueChange() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetBestTorqueChange();
	}
}
double CRTrajectoryPlanner::Joints::GetBestTorqueChangeInSection(int n) {
	double total = 0;
	for (size_t i = 0; i < joints.size(); i++) {
		total += joints[i]->GetBestTorqueChangeInSection(n);
	}
	DSTR << std::endl;
	return total;
}
void CRTrajectoryPlanner::Joints::ShowInfo() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->ShowInfo();
	}
}
void CRTrajectoryPlanner::Joints::SetTargetCurrent() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetCurrent();
	}
}
void CRTrajectoryPlanner::Joints::SetWeight() {
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

void CRTrajectoryPlanner::Joints::SetPD(double s, double d, bool mul) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetPD(s, d, mul);
	}
}

void CRTrajectoryPlanner::Joints::SetTargetFromLPF(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetTargetFromLPF(k, n);
	}
}

void CRTrajectoryPlanner::Joints::UpdateIKParam(double b, double p) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->UpdateIKParam(b, p);
	}
}

void CRTrajectoryPlanner::Joints::OutputTorque() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->OutputTorque();
	}
}

//N-Simple Moving Average LPF
template<class T>
#ifdef _MSC_VER
static PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
#else
PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
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
static PTM::VVector<T> CRTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
#else
PTM::VVector<T> CRTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
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
static PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
#else
PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
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
static PTM::VVector<T> CRTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
#else
PTM::VVector<T> CRTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
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

void CRTrajectoryPlanner::Init() {

	//総移動ステップ数設定
	ViaPoint tpoint = viaPoints.back();
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

void CRTrajectoryPlanner::CheckAndSetJoints() {

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

void CRTrajectoryPlanner::RecalcFromIterationN(int n) {

}

void CRTrajectoryPlanner::DisplayDebugInfo() {
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

void CRTrajectoryPlanner::OutputTrajectory(std::string filename) {
	// 手先軌道をファイル出力出力
	std::ofstream outfile(filename + ".csv");
	for (int i = 0; i < maxIterate + 1; i++) {
		for (int j = 0; j < trajData.width(); j++) {
			outfile << trajData[i][j].Pos().x << "," << trajData[i][j].Pos().y << "," << trajData[i][j].Pos().z << "," << std::endl;
		}
	}
}

void CRTrajectoryPlanner::OutputNotCorrectedTrajectory(std::string filename) {
	// 修正前の手先軌道をファイル出力
	std::ofstream outfile(filename + "NotCorrected.csv");
	for (int i = 0; i < maxIterate; i++) {
		for (int j = 0; j < movtime; j++) {
			outfile << trajDataNotCorrected[i][j].Pos().x << "," << trajDataNotCorrected[i][j].Pos().y << "," << trajDataNotCorrected[i][j].Pos().z << "," << std::endl;
		}
	}
}

void CRTrajectoryPlanner::OutputVelocity(std::string filename) {
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

void CRTrajectoryPlanner::Replay(int ite, bool noncorrected) {
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

void CRTrajectoryPlanner::JointTrajStep(bool step) {
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

void CRTrajectoryPlanner::CalcTrajectory() {
	// OneStep実行を規定回走らせる
	for (; currentIterate <= maxIterate; ) {
		CalcOneStep();
	}
}

void CRTrajectoryPlanner::CalcOneStep() {
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

void CRTrajectoryPlanner::ForwardInverseRelaxation() {
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

void CRTrajectoryPlanner::MakeMinJerk() {
	// シーンのステップ幅など
	double dt = scene->GetTimeStep();
	double dtInv = 1 / dt;

	// 開始点の設定
	ViaPoint startPoint = ViaPoint();
	startPoint.pose = Posed(ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition(), ikEndEffector->GetSolid()->GetPose().Ori());
	startPoint.vel = GetEndEffectorVelocity();
	startPoint.acc = SpatialVector();
	startPoint.time = 0;
	// 最終目標点の設定
	ViaPoint targetPoint = viaPoints.back();

	// 開始点と最終目標点のみを満たす軌道生成
	CRPoseMinimumJerkTrajectory minjerk = CRPoseMinimumJerkTrajectory();
	PTM::VVector<Posed> minjerkPoses;
	minjerkPoses.resize(movtime);
	PTM::VVector<SpatialVector> minjerkVels;
	minjerkVels.resize(movtime);
	for (int i = 0; i < movtime; i++) {
		DSTR << (i + 1) * dt << std::endl;
		minjerkPoses[i] = minjerk.GetPosition((i + 1) * dt);
		//minjerkVels[i] = minjerk.GetCurrentVelocity(i + 1);
	}

	//各点を単純な躍度最小軌道で接続
	for (int i = 0; i < (int)viaPoints.size() + 1; i++) {
		ViaPoint s = (i == 0) ? startPoint : viaPoints[i - 1];
		ViaPoint f = (i == viaPoints.size()) ? targetPoint : viaPoints[i];
		CRPoseMinimumJerkTrajectory mjt = CRPoseMinimumJerkTrajectory();
		for (int j = s.time * dtInv; j < f.time * dtInv; j++) {
			minjerkPoses[j] = mjt.GetPosition((j + 1) * dt);
		}
	}
	/*/
	//境界条件満足の部分軌道を足し合わせて経由点を通る軌道実現
	for (int i = 0; i < (int)viaPoints.size(); i++) {
	ViaPoint s = ViaPoint(Posed(), SpatialVector(), SpatialVector(), (i == 0) ? startPoint.time : viaPoints[i - 1].time);
	ViaPoint f = ViaPoint(Posed(), SpatialVector(), SpatialVector(), targetPoint.time);
	ViaPoint c = viaPoints[i];
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
	}
	else {
		joints.MakeJointMinjerk(maxIterate);
	}

	LoadInitialState();
}

void CRTrajectoryPlanner::CompForwardDynamics(int k) {
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

void CRTrajectoryPlanner::CompInverseDynamics(int k) {
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

void CRTrajectoryPlanner::TrajectoryCorrection(int k) {
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
			ViaPoint targetPoint = viaPoints.back();
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

void CRTrajectoryPlanner::PostProcessing() {
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

bool CRTrajectoryPlanner::AdjustViatime() {
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
	for (int i = 0; i < nVia + 1; i++) {
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

void CRTrajectoryPlanner::InitializeViatime() {
	// 各経由点の通過時間を等間隔に初期化
	int nVia = (int)viaPoints.size();
	for (int i = 0; i < nVia; i++) {
		viaPoints[i].time = mtime * (i + 1) / (nVia + 1);
	}
}

void CRTrajectoryPlanner::PrepareSprings() {
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

void CRTrajectoryPlanner::ReloadCorrected(int k, bool nc) {
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
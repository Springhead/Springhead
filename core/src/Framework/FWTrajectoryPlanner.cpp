#include <Framework/FWTrajectoryPlanner.h>
#define DEBUG_INFORMATION 1
#define INITAL_INTERPOLATION 0
#define USE_MINJERK_TARGET_POS_VEL 0

namespace Spr {
	;
// 散らかってるのは後で整理します

//----- ----- ----- -----


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
	initialPullbackTarget = hinge->GetPullbackTarget();

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

	CRAngleMinimumJerkTrajectory hmjt = CRAngleMinimumJerkTrajectory();
	hmjt.SetStart(initialAngle, initialVel, 0, 0);
	hmjt.SetGoal(targetAngle, targetVel, 0, mtime);

	for (int j = 0; j < movetime; j++) {
		angle[cnt][j] = hmjt.GetPosition((j + 1) * dt);
		angleVels[cnt][j] = hmjt.GetVelocity((j + 1) * dt);
		//angle[cnt][j] = hmjt.GetCurrentAngle((j + 1) * dt);
		//angleVels[cnt][j] = hmjt.GetCurrentVelocity((j + 1) * dt);
	}
}
void FWTrajectoryPlanner::HingeJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void FWTrajectoryPlanner::HingeJoint::SaveTorque(int n) {
	torque[n] = hinge->GetJoint()->GetMotorForceN(0);
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

	CRAngleMinimumJerkTrajectory delta = CRAngleMinimumJerkTrajectory();
	delta.SetStart(0, 0, 0, 0);
	delta.SetGoal(targetAngle - end, -endVel, -endAcc, time - stime);

	for (int i = 0; i < movetime - stime; i++) {
		angle[k][i + stime] = angleLPF[k][i + stime] = delta.GetPosition((i + 1) * dt) + angleLPF[k][i + stime];
		angleVels[k][i + stime] = angleVelsLPF[k][i + stime] = (angle[k][i + stime] - (i + stime > 0 ? angle[k][i + stime - 1] : initialAngle)) * dtInv;
	}
}
void FWTrajectoryPlanner::HingeJoint::ApplyLPF(int count) {
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torque, 3, 1, initialTorque);
	}
	if (count > 0) {
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = rateLPF * torqueLPF[i] + (1 - rateLPF) * torque[i];
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
	hinge->GetJoint()->SetSpring(originalSpring);
	hinge->GetJoint()->SetDamper(originalDamper);
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
	DSTR << "Pose:" << hinge->GetJoint()->GetPosition() << " Target:" << hinge->GetJoint()->GetTargetPosition() << " Vel:" << hinge->GetJoint()->GetVelocity()  << " TarVel:" << hinge->GetJoint()->GetTargetVelocity() 
		<< " PD:" << hinge->GetJoint()->GetSpring() << "," << hinge->GetJoint()->GetDamper() << std::endl;
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

void FWTrajectoryPlanner::HingeJoint::SetPullbackTarget(int k, int n) {
	if (k >= 0 && k <= iterate) {
		hinge->SetPullbackTarget(angle[k][n]);
	}
}

void FWTrajectoryPlanner::HingeJoint::SetPullbackTargetFromInitial() {
	hinge->SetPullbackTarget(initialPullbackTarget);
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
	initialPullbackTarget = ball->GetPullbackTarget();

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

	CRQuaternionMinimumJerkTrajectory qmjt = CRQuaternionMinimumJerkTrajectory();
	qmjt.SetStart(initialOri, initialVel, Vec3d(), 0);
	qmjt.SetGoal(targetOri, targetVel, Vec3d(), mtime);

	for (int j = 0; j < movetime; j++) {
		ori[cnt][j] = qmjt.GetPosition((j + 1) * dt);
		oriVels[cnt][j] = qmjt.GetVelocity((j + 1) * dt);
	}
}
void FWTrajectoryPlanner::BallJoint::CloseFile() {
	torGraph->close();
	torChangeGraph->close();
}
void FWTrajectoryPlanner::BallJoint::SaveTorque(int n) {
	this->torque[n] = ball->GetJoint()->GetMotorForceN(0);
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
	}
}
void FWTrajectoryPlanner::BallJoint::SetTargetInitial() {
	ball->GetJoint()->SetTargetPosition(initialOri);
	ball->GetJoint()->SetTargetVelocity(initialVel);
}
void FWTrajectoryPlanner::BallJoint::SetOffsetFromLPF(int n) {
	ball->GetJoint()->SetOffsetForceN(0, torqueLPF[n]);
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

	Quaterniond end = oriLPF[k][oriLPF.width() - 1];
	Vec3d endVel = oriVelsLPF[k][oriVels.width() - 1];
	Vec3d endAcc = (oriVelsLPF[k][oriVelsLPF.width() - 1] - oriVelsLPF[k][oriVelsLPF.width() - 2]) * dtInv;

	double time = mtime;

	//QuaMinJerkTrajectory delta = QuaMinJerkTrajectory(Quaterniond(), targetOri * end.Inv(), Vec3d(), -endVel, Vec3d(), -endAcc, time - stime);
	CRQuaternionMinimumJerkTrajectory delta = CRQuaternionMinimumJerkTrajectory();
	delta.SetStart(Quaterniond(), Vec3d(), Vec3d(), 0);
	delta.SetGoal(targetOri * end.Inv(), -endVel, -endAcc, time);

	// <!!> oriVelsの更新がなんか変かも
	for (int i = 0; i < movetime ; i++) {
		ori[k][i] = delta.GetPosition((i + 1) * dt) * oriLPF[k][i];
		DSTR << delta.GetPosition((i + 1) * dt) << std::endl;
		oriVels[k][i] = ori[k][i].AngularVelocity(ori[k][i] * (i > 0 ? ori[k][i - 1] : initialOri).Inv());

		oriLPF[k][i] = ori[k][i];
		oriVelsLPF[k][i] = oriVels[k][i];
	}
}
void FWTrajectoryPlanner::BallJoint::ApplyLPF(int count) {
	for (int i = 0; i < count; i++) {
		torqueLPF = LPF::centerNSMAv(torque, 3, 1, initialTorque);
	}
	if (count > 0) {
		for (int i = 0; i < torqueLPF.size(); i++) {
			torqueLPF[i] = rateLPF * torqueLPF[i] + (1 - rateLPF) * torque[i];
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
	ball->GetJoint()->SetSpring(originalSpring);
	ball->GetJoint()->SetDamper(originalDamper);
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
	DSTR << "Pose:" << ball->GetJoint()->GetPosition() << " Target:" << ball->GetJoint()->GetTargetPosition() << " Vel:" << ball->GetJoint()->GetVelocity() << " TarVel:" << ball->GetJoint()->GetTargetVelocity()
		 << " PD:" << ball->GetJoint()->GetSpring() << "," << ball->GetJoint()->GetDamper() << std::endl;
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

void FWTrajectoryPlanner::BallJoint::SetPullbackTarget(int k, int n) {
	if (k >= 0 && k <= iterate) {
		ball->SetPullbackTarget(ori[k][n]);
	}
}

void FWTrajectoryPlanner::BallJoint::SetPullbackTargetFromInitial() {
	ball->SetPullbackTarget(initialPullbackTarget);
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

void FWTrajectoryPlanner::Joints::SetPullbackTarget(int k, int n) {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetPullbackTarget(k, n);
	}
}

void FWTrajectoryPlanner::Joints::SetPullbackTargetFromInitial() {
	for (size_t i = 0; i < joints.size(); i++) {
		joints[i]->SetPullbackTargetFromInitial();
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

	initialTargetPose = Posed(ikEndEffector->GetTargetPosition(), ikEndEffector->GetTargetOrientation());

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
	std::ofstream outfile2(filename + "TorqueChange.csv");
	int l = torqueChangeRecord.size();
	for (int i = 0; i < l; i++) {
		outfile2 << torqueChangeRecord[i] << std::endl;
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
	//joints.SetTargetInitial();
	LoadInitialState();
	joints.SetPullbackTargetFromInitial();
	ikEndEffector->SetTargetPosition(initialTargetPose.Pos());
	ikEndEffector->SetTargetOrientation(initialTargetPose.Ori());
	repCount = 0;
}

void FWTrajectoryPlanner::JointTrajStep(bool step) {
	if (isMoving) {
		/*
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
		*/
		Posed endEffectorTarget;
		if (repCount >= 0 && repCount < (int)trajData.width()) {
			if (!noncorrectedReplay) {
				joints.SetPullbackTarget(ite, repCount);
				endEffectorTarget = trajData[ite][repCount];
				ikEndEffector->SetTargetPosition(endEffectorTarget.Pos());
				ikEndEffector->SetTargetOrientation(endEffectorTarget.Ori());
			}
			else {
				//joints.SetTargetFromLPF(ite, repCount);
			}
			repCount++;
		}
		else if (repCount >= movtime) {
			isMoving = false;
			if (!noncorrectedReplay) {
				joints.SetPullbackTarget(ite, movtime - 1);
				endEffectorTarget = trajData[ite][movtime - 1];
				ikEndEffector->SetTargetPosition(endEffectorTarget.Pos());
				ikEndEffector->SetTargetOrientation(endEffectorTarget.Ori());
			}
			else {
				//joints.SetTargetFromLPF(ite, movtime - 1);
			}
		}
		else {
			//joints.SetPullbackTargetFromInitial();
			repCount++;
		}
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
	//startPoint.pose = Posed(ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition(), ikEndEffector->GetSolid()->GetPose().Ori());
	startPoint.pose = Posed(ikEndEffector->GetTargetPosition(), ikEndEffector->GetTargetOrientation());
	//startPoint.vel = GetEndEffectorVelocity();
	startPoint.vel = SpatialVector(ikEndEffector->GetTargetVelocity(), ikEndEffector->GetTargetAngularVelocity());
	startPoint.acc = SpatialVector();
	startPoint.time = 0;
	// 最終目標点の設定
	ControlPoint targetPoint = viaPoints.back();

	// 開始点と最終目標点のみを満たす軌道生成
	//MinJerkTrajectory minjerk = MinJerkTrajectory(startPoint, targetPoint);
	CRPoseMinimumJerkTrajectory minjerk = CRPoseMinimumJerkTrajectory();
	minjerk.SetStart(startPoint.pose, startPoint.vel, startPoint.acc, startPoint.time);
	minjerk.SetGoal(targetPoint.pose, targetPoint.vel, targetPoint.acc, targetPoint.time);
	PTM::VVector<Posed> minjerkPoses;
	minjerkPoses.resize(movtime);
	PTM::VVector<SpatialVector> minjerkVels;
	minjerkVels.resize(movtime);
	for (int i = 0; i < movtime; i++) {
		DSTR << minjerk.GetPosition((i + 1) * dt) << std::endl;
		minjerkPoses[i] = minjerk.GetPosition((i + 1) * dt);
		minjerkVels[i] = minjerk.GetVelocity((i + 1) * dt);
		//minjerkPoses[i] = minjerk.GetCurrentPose((i + 1) * dt);
	    //minjerkVels[i] = minjerk.GetCurrentVelocity(i + 1);
	}
	
	LoadInitialState();
	joints.Harden();
	
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
		 ikEndEffector->SetTargetVelocity(minjerkVels[reach].v());
		 ikEndEffector->SetTargetAngularVelocity(minjerkVels[reach].w());
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

		//trajData[0][reach].Pos() = minjerkTarget.Pos();
		trajData[0][reach].Pos() = ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition();
		trajData[0][reach].Ori() = ikEndEffector->GetSolid()->GetPose().Ori();
		trajVel[0][reach + 1] = GetEndEffectorVelocity();
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

		//trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Pos() = ikEndEffector->GetSolid()->GetPose() * ikEndEffector->GetTargetLocalPosition();
		//trajDataNotCorrected[k > 0 ? k - 1 : 0][i].Ori() = ikEndEffector->GetSolid()->GetPose().Ori();

		eefVel = ikEndEffector->GetSolid()->GetVelocity();
		//trajVelNotCorrected[k > 0 ? k - 1 : 0][i + 1] = GetEndEffectorVelocity();
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
	torqueChangeRecord.push_back(torqueChange);
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

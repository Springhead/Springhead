/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
//#include <Springhead.h>
#include <Framework/SprFWOptimizer.h>
#include <Springhead.h>

#include <Physics/PHJointMotor.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
double Add(double a){
	return a + 5.0;
}

double ccw(Vec3f p1, Vec3f p2, Vec3f p3){
	double c = (p2.x - p1.x) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.x - p1.x);
	//DSTR << p1 << " " << p2 << " " << p3 << " " << c << std::endl;
	return c;
};
/*
//Calculate Graham Convex hull from vertices
//Y value is ignored
void GrahamConvexHull(std::vector<Vec3f>& vertices){
	int n = (int)vertices.size();
	//DSTR << n << std::endl;
	if (n < 3) return;
	struct Sort{
		static float getPolar(Vec3f v1, Vec3f v2){
			Vec3f dv = v2 - v1;
			return dv.norm() == 0 ? 0 : -dv.x / dv.norm();
		}
		struct tang{
			double slope;
			bool same_x = false;
		};
		static tang getTang(Vec3f v1, Vec3f v2){
			tang t;
			if (v1.x == v2.x){
				t.slope = 0;
				t.same_x = true;
			}
			else{
				t.slope = (v1.z - v2.z) / (v1.x - v2.x);
			}
			return t;
		}
		static int part(std::vector<Vec3f> ver, int i, int j, int p){
			int l = i, r = j;

			while (l <= r){
				while (l <= j && ccw(ver[0], ver[l], ver[p]) < 0) l++;
				while (i <= r && ccw(ver[0], ver[r], ver[p]) > 0) r--;
				if (l >= r) break;
				Vec3f v = ver[l];
				ver[l] = ver[r];
				ver[r] = v;
				l++;
				r--;
			}
			return l;
		}
		static void quicksort(std::vector<Vec3f>& ver, int i, int j){
			//DSTR << i << " " << j << std::endl;
			if (i >= j) return; //DSTR << "side error" << std::endl;
			int p = (i + j) / 2;
			int k = part(ver, i, j, p);
			//DSTR << "p:" << p << " k:" << k << std::endl;
			int l = i, r = j;
			while (l <= r){
				while (l <= j && ccw(ver[0], ver[l], ver[p]) < 0) l++;
				while (i <= r && ccw(ver[0], ver[r], ver[p]) > 0) r--;
				if (l >= r) break;
				Vec3f v = ver[l];
				ver[l] = ver[r];
				ver[r] = v;
				//DSTR << "swap(" << l << "," << r << ")" << std::endl;
				l++;
				r--;
			}
			quicksort(ver, i, l-1);
			quicksort(ver, r+1, j);
		}
	};
	int xmax = 0;
	for (int i = 0; i < n; i++){
		if (vertices[xmax].x < vertices[i].x){
			xmax = i;
		}
		else if (vertices[xmax].x == vertices[i].x && vertices[xmax].z < vertices[i].z){
			xmax = i;
		}
	}
	Vec3f tmp = vertices[0];
	vertices[0] = vertices[xmax];
	vertices[xmax] = tmp;
	Sort::quicksort(vertices, 1, n - 1);
	vertices.insert(vertices.begin(), vertices[n - 1]);
	int m = 1;
	std::vector<Vec3f> hull;
	hull.push_back(vertices[0]); hull.push_back(vertices[1]);
	for (int i = 2; i < n + 1; i++){
		while (ccw(hull[m - 1], hull[m], vertices[i]) > 0){
			Vec3f removed = hull[m];
			hull.pop_back();
			//DSTR << "removed:" << removed << std::endl;
			m--;
			if (m == 0){
				break;
			}
		}
		hull.push_back(vertices[i]);
		//DSTR << "add:" << vertices[i] << std::endl;
		m++;
	}
	vertices.swap(hull);
}

//Y value is ignored completely
//Maybe some error happen in this 
bool InsideGrahamConvexHull(std::vector<Vec3f>& vertices, Vec3f gp){
	bool inside = false;
	GrahamConvexHull(vertices);
	int l = vertices.size() - 1;
	if (l < 3) return false;
	for (int i = 1; i < l - 1; i++){
		double ccw1 = ccw(vertices[0], vertices[i], gp);
		double ccw2 = ccw(vertices[i], vertices[i + 1], gp);
		double ccw3 = ccw(vertices[i + 1], vertices[0], gp);
		if (ccw1 * ccw2 > 0 && ccw1 * ccw3 > 0){
			inside = true;
			DSTR << "inside!" << std::endl;
			break;
		}
	}
	return inside;
}

double EvalCenterOfGravity(std::vector<Vec3f>& vertices, Vec3f gp){
	for (int i = 0; i < vertices.size(); i++)
		DSTR << "vertices[" << i << "]:" << vertices[i] << std::endl;
	Vec3f hullCenter;
	GrahamConvexHull(vertices);
	for (int i = 0; i < vertices.size(); i++)
		DSTR << "vertices(hull)[" << i << "]:" << vertices[i] << std::endl;
	int l = vertices.size() - 1;
	for (int i = 0; i < l; i++){
		hullCenter = hullCenter + (vertices[i]) / l;
	}
	DSTR << "center of hull:" << hullCenter << std::endl;
	double dist = (gp - hullCenter).norm();
	return InsideGrahamConvexHull(vertices, gp) ? dist : 1000 * dist;
}
*/
GrahamConvexHull::GrahamConvexHull(){
	center = Vec3f(0.0, 0.0, 0.0);
	normal = Vec3d(0.0, 1.0, 0.0);
}
/*
GrahamConvexHull::GrahamConvexHull(std::vector<Vec3f> vertices){
	int n = (int)vertices.size();
	if (n > 2){
		int xmax = 0;
		for (int i = 0; i < n; i++){
			if (vertices[xmax].x < vertices[i].x){
				xmax = i;
			}
			else if (vertices[xmax].x == vertices[i].x && vertices[xmax].z < vertices[i].z){
				xmax = i;
			}
		}
		Vec3f tmp = vertices[0];
		vertices[0] = vertices[xmax];
		vertices[xmax] = tmp;
		Sort::quicksort(vertices, 1, n - 1);
		vertices.insert(vertices.begin(), vertices[n - 1]);
		int m = 1;
		hull.push_back(vertices[0]); hull.push_back(vertices[1]);
		for (int i = 2; i < n + 1; i++){
			while (ccw(hull[m - 1], hull[m], vertices[i]) > 0){
				Vec3f removed = hull[m];
				hull.pop_back();
				m--;
				if (m == 0){
					break;
				}
			}
			hull.push_back(vertices[i]);
			m++;
		}
		int l = hull.size() - 1;
		center = Vec3f(0.0, 0.0, 0.0);
		for (int i = 0; i < l; i++){
			center = center + (vertices[i]) / l;
		}
	}
}
*/
GrahamConvexHull::~GrahamConvexHull(){
	std::vector<Vec3f>().swap(hull);
}

bool GrahamConvexHull::InsideGrahamConvexHull(Vec3f v){
	bool inside = false;
	int l = hull.size() - 1;
	if (l < 3) return false;
	for (int i = 1; i < l - 1; i++){
		double ccw1 = ccw(hull[0], hull[i], v);
		double ccw2 = ccw(hull[i], hull[i + 1], v);
		double ccw3 = ccw(hull[i + 1], hull[0], v);
		if (ccw1 * ccw2 > 0 && ccw1 * ccw3 > 0){
			inside = true;
			DSTR << "inside!" << std::endl;
			break;
		}
	}
	return inside;
}

void GrahamConvexHull::Recalc(std::vector<Vec3f> vertices){
	int n = (int)vertices.size();
	if (n > 2){
		int xmax = 0;
		for (int i = 0; i < n; i++){
			if (vertices[xmax].x < vertices[i].x){
				xmax = i;
			}
			else if (vertices[xmax].x == vertices[i].x && vertices[xmax].z < vertices[i].z){
				xmax = i;
			}
			vertices[i].y = 0;
		}
		Vec3f tmp = vertices[0];
		vertices[0] = vertices[xmax];
		vertices[xmax] = tmp;
		Sort::quicksort(vertices, 1, n - 1);
		vertices.insert(vertices.begin(), vertices[n - 1]);
		int m = 1;
		hull.clear();
		hull.push_back(vertices[0]); hull.push_back(vertices[1]);
		for (int i = 2; i < n + 1; i++){
			while (ccw(hull[m - 1], hull[m], vertices[i]) > 0){
				Vec3f removed = hull[m];
				hull.pop_back();
				m--;
				if (m == 0){
					break;
				}
			}
			hull.push_back(vertices[i]);
			m++;
		}
		int l = hull.size() - 1;
		center = Vec3f(0.0, 0.0, 0.0);
		for (int i = 0; i < l; i++){
			center = center + (vertices[i]) / l;
		}
	}
}

double GrahamConvexHull::Eval(Vec3f v){
	int nHull = hull.size();
	double ccw1;
	double min = INFINITY;;
	for (int i = 0; i < nHull; i++)
		DSTR << "vertices[" << i << "]:" << hull[i] << std::endl;
	DSTR << "center of hull:" << center << std::endl;
	/*double ccw1, ccw2, ccw3;
	for (int i = 0; i < nHull - 1; i++){
		ccw1 = ccw(hull[i], hull[i + 1], v);
		ccw2 = ccw(hull[i + 1], center, v);
		ccw3 = ccw(center, hull[i], v);
		if (ccw2 * ccw3 >= 0){
			ccw1 /= (hull[i + 1] - hull[i]).norm();
			if ((ccw1 * ccw2 > 0 && ccw1 * ccw3 > 0) || (ccw1 * ccw2 * ccw3) == 0){
				DSTR << "inside!" << ccw1 << " " << ccw2 << " " << ccw3 << std::endl;
				if (ccw1 > 0) ccw1 *= -1;
				break;
			}
			else{
				DSTR << "outside!" << ccw1 << " " << ccw2 << " " << ccw3 << std::endl;
				if (ccw1 < 0) ccw1 *= -1;
				break;
			}
		}
	}*/
	InsideGrahamConvexHull(v);
	for (int i = 0; i < nHull; i++){
		Vec3f edge = hull[i + 1] - hull[i]; 
		if (edge.norm() != 0){
			ccw1 = ccw(hull[i], v, hull[i + 1]) / edge.norm();
			DSTR << ccw1 << std::endl;
			if (min > ccw1){
				min = ccw1;
			}
		}
	}
	if (min >= 0) DSTR << "inside" << std::endl;
	return exp(10 * (-ccw1 + 0.0));
}

Vec3f* GrahamConvexHull::GetVertices(){
	return &*hull.begin();
}

MinJerkTrajectory::MinJerkTrajectory(){
	sPose = Posed(); fPose = Posed();
	stime = 0; ftime = 1;
}

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, int time){
	sPose = spose; fPose = fpose;
	stime = 0; ftime = time;
	Vec3d diff = fPose.Pos() - sPose.Pos();
	/*for (int i = 0; i < 3; i++){
		coeff[0][i] = sPose.Pos()[i]; coeff[1][i] = 0;
		coeff[2][i] = 0;              coeff[3][i] = 10 * diff[i];
		coeff[4][i] = -15 * diff[i];  coeff[5][i] = 6 * diff[i];
	}*/
	DSTR << sPose << fPose << ftime << std::endl;
}
/*
MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, Vec6d sVel, Vec6d fVel, Vec6d sAcc, Vec6d fAcc, int time){
	sPose = spose; fPose = fpose;
	stime = 0; ftime = time;
	for (int i = 0; i<3; ++i) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPose.Pos()[i];
		A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel[i];
		A.row(2) = Vec6d(0, 0, 2, 6 * stime, 12 * pow(stime, 2), 20 * pow(stime, 3)); b[2][0] = sAcc[i];
		A.row(3) = Vec6d(1, stime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPose.Pos()[i];
		A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel[i];
		A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = fAcc[i];
		x = A.inv() * b;

		for (int n = 0; n<6; ++n){ coeff[n][i] = x[n][0]; }
	}
}

MinJerkTrajectory::MinJerkTrajectory(Posed spose, Posed fpose, Vec6d sVel, Vec6d fVel, Posed vPose, int vtime, int time){
	sPose = spose; fPose = fpose;
	stime = 0; ftime = time;
	for (int i = 0; i<3; ++i) {
		PTM::TMatrixRow<6, 6, double> A;
		PTM::TMatrixRow<6, 1, double> b, x;
		if (vtime > 0){
			A.row(0) = Vec6d(1, stime, pow(stime, 2), pow(stime, 3), pow(stime, 4), pow(stime, 5)); b[0][0] = sPose.Pos()[i];
			A.row(1) = Vec6d(0, 1, 2 * stime, 3 * pow(stime, 2), 4 * pow(stime, 3), 5 * pow(stime, 4)); b[1][0] = sVel[i];
			A.row(2) = Vec6d(1, vtime, pow(vtime, 2), pow(vtime, 3), pow(vtime, 4), pow(vtime, 5)); b[2][0] = vPose.Pos()[i];
			A.row(3) = Vec6d(1, stime, pow(ftime, 2), pow(ftime, 3), pow(ftime, 4), pow(ftime, 5)); b[3][0] = fPose.Pos()[i];
			A.row(4) = Vec6d(0, 1, 2 * ftime, 3 * pow(ftime, 2), 4 * pow(ftime, 3), 5 * pow(ftime, 4)); b[4][0] = fVel[i];
			A.row(5) = Vec6d(0, 0, 2, 6 * ftime, 12 * pow(ftime, 2), 20 * pow(ftime, 3)); b[5][0] = 0;
		}
		x = A.inv() * b;

		for (int n = 0; n<6; ++n){ coeff[n][i] = x[n][0]; }
	}
}
*/
Posed MinJerkTrajectory::GetCurrentPose(int t){
	if (t < stime) return sPose;
	if (ftime < t) return fPose;
	double s = (double)(t - stime) / (ftime - stime);
	double r = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);
	//DSTR << s << " " << r << std::endl;

	Posed curPose;
	curPose.Pos() = sPose.Pos() + (fPose.Pos() - sPose.Pos()) * r;
	curPose.Ori() = interpolate(s, sPose.Ori(), fPose.Ori());
	return  curPose;
}

Posed MinJerkTrajectory::GetDeltaPose(int t){
	double s = (ftime - t) / (ftime - stime);
	double r = 10 * pow(s, 3) - 15 * pow(s, 4) + 6 * pow(s, 5);

	Posed deltaPose;
	deltaPose.Pos() = (fPose.Pos() - sPose.Pos()) * r;
	deltaPose.Ori() = interpolate(s, Quaterniond(), fPose.Ori() * sPose.Ori().Inv());
	return deltaPose;
}

double MinJerkTrajectory::GetCurrentActiveness(int t){
	double s = (ftime - t) / (ftime - stime);
	double r = 30 * pow(s, 2) - 60 * pow(s, 3) + 30 * pow(s, 4);
	return r;
}

void SPR_CDECL FWOptimizer::FWOptimizer_ThreadCallback(int id, void* arg) {
	FWOptimizer* instance = (FWOptimizer*)arg;
	instance->Optimize();
}

FWOptimizer::FWOptimizer() {
	thread = UTTimerIf::Create();
	thread->SetMode(UTTimerIf::THREAD);
	thread->SetCallback(FWOptimizer_ThreadCallback, this);
	thread->Stop();

	bRunning = false;
	bInitialized = false;
	xstart = NULL; stddev = NULL; xfinal = NULL; xprovisional = NULL;
#ifdef USE_CLOSED_SRC
	parameters = NULL; evo = NULL;
#endif
}

FWOptimizer::~FWOptimizer() {
	thread->Clear();
	if (xstart)			{ delete xstart; }
	if (stddev)			{ delete stddev; }
#ifdef USE_CLOSED_SRC
	if (parameters)		{ delete parameters; }
	if (evo)			{ delete evo; }
#endif
	if (xfinal)			{ delete xfinal; }
	if (xprovisional)	{ delete xprovisional; }
}

void FWOptimizer::CopyScene(PHSceneIf* phSceneInput) {
	// ファイル名は一考の余地あり
	/*
	fwSdk = fwSceneInput->GetSdk();
	fwSdk->SaveScene("temp.spr");
	fwSdk->LoadScene("temp.spr");
	
	fwScene = fwSdk->GetScene(fwSdk->NScene() - 1);

	// 本当はContactも複製する必要あり
	for (int i = 0; i < fwSdk->NScene(); ++i) {
		PHSceneIf* phScene = fwSdk->GetScene(i)->GetPHScene();
		if (phScene) {
			phScene->SetContactMode(PHSceneDesc::MODE_NONE); // 現状ではとりあえず切る
		}
	}
	*/
}

void FWOptimizer::Init(int dimension) {
#ifdef USE_CLOSED_SRC
	this->dimension = dimension;

	if (xstart)			{ delete xstart; }
	if (stddev)			{ delete stddev; }
	if (parameters)		{ delete parameters; }
	if (evo)			{ delete evo; }
	if (xfinal)			{ delete xfinal; }
	if (xprovisional)	{ delete xprovisional; }

	xstart = new double[dimension];
	stddev = new double[dimension];
	evo = new CMAES<double>();
	parameters = new Parameters<double>();

	for (int i = 0; i < dimension; ++i) {
		xstart[i] = ixstart;
		stddev[i] = istddev;
	}

	parameters->stopTolFun = iTolFun;
	parameters->lambda = ilambda;
	parameters->stopMaxIter = iMaxIter;
	parameters->init(dimension, xstart, stddev);
	arFunvals = evo->init(*parameters);

	states = ObjectStatesIf::Create();
	DSTR << "Saved scene" << std::endl;
	
	/*
	for (int i = 0; i < phScene->NSolids(); i++){
		DSTR << "Solid[" << i << "]:" << phScene->GetSolids()[i]->GetName() << "(" << phScene->GetSolids()[i] << ")" << " : " << phScene->GetSolids()[i]->GetPose() << std::endl;
	}
	*/
	states->SaveState(phScene);

	bInitialized = true;
#endif
}

void FWOptimizer::Start() {
	bRunning = true;
	thread->Start();
}

void FWOptimizer::Abort() {
	bRunning = false;
}

bool FWOptimizer::IsRunning() {
	return bRunning;
}

void FWOptimizer::Optimize() {
#ifdef USE_CLOSED_SRC
	bRunning = true;
	DSTR << "Optimize Thread Running" << std::endl;
	if (bInitialized) {
		int cnt = 0;
		while (bRunning && !evo->testForTermination()) {
			Iterate();
			cnt++;
			if (cnt % 10 == 0) {
				DSTR << "Generation : " << cnt << " (Fitness = " << evo->get(CMAES<double>::Fitness) << ")" << std::endl;
				double *lastxprov = xprovisional;
				xprovisional = evo->getNew(CMAES<double>::XMean);
				if (lastxprov) { delete lastxprov; }
			}
		}
		double *lastxprov = xprovisional;
		xprovisional = evo->getNew(CMAES<double>::XMean);
		if (lastxprov) { delete lastxprov; }

		xfinal = evo->getNew(CMAES<double>::XMean);
		bRunning = false;
		DSTR << "Stop:" << std::endl << evo->getStopMessage();
		thread->Stop();
	}
#endif
}

void FWOptimizer::Iterate() {
#ifdef USE_CLOSED_SRC
	// Generate lambda new search points, sample population
	pop = evo->samplePopulation();

	// evaluate the new search points using objective function
	for (int i = 0; i < evo->get(CMAES<double>::Lambda); ++i) {
		states->LoadState(phScene);
		DSTR << "Loadedd scene" << std::endl;
		/*
		for (int j = 0; j < phScene->NSolids(); j++){
			DSTR << "Solid[" << j << "]:" << phScene->GetSolids()[j]->GetName() << "(" << phScene->GetSolids()[j] << ")" << " : " << phScene->GetSolids()[j]->GetPose() << std::endl;
		}
		for (int j = 0; j < phScene->NIKActuators(); j++){
			DSTR << "Joint[" << j << "]:" << phScene->GetIKActuator(j) << std::endl;
		}
		*/
		arFunvals[i] = Objective(pop[i], (int)evo->get(CMAES<double>::Dimension));
	}

	// update the search distribution used for sampleDistribution()
	evo->updateDistribution(arFunvals);
#endif
}

double FWOptimizer::Objective(double const *x, int n) {
	// 1. Apply x to Scene
	for (int i = 0; i < phScene->NJoints(); ++i) {
		Vec3d f, t;
		PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
		jo->SetSpring(1000 * abs(x[2 * i]));
		jo->SetDamper(1000 * abs(x[2 * i + 1]));
	}

	double obj = 0;
	for (int i = 0; i < 100; ++i) {
		// 2. Do Simulation Step
		phScene->Step();

		// 3. Calc Criterion and Sum up
		for (int j = 0; j < phScene->NJoints(); ++j) {
			double torque = DCAST(PHHingeJointIf, phScene->GetJoint(j))->GetMotorForce();
			obj += abs(torque);
		}
	}

	// 4. Calc Criterion for Final State
	PHIKEndEffectorIf* eef = phScene->GetIKEndEffector(0);
	double error = ((eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition()) - eef->GetTargetPosition()).norm();

	obj += 1e+4 * abs(error);

	obj += 1e+4 * eef->GetSolid()->GetVelocity().norm();
	obj += 1e+4 * eef->GetSolid()->GetAngularVelocity().norm();

	return obj;
}

double FWOptimizer::ApplyPop(PHSceneIf* phScene, double const *x, int n) {
	return 0;
}

int FWOptimizer::NResults() {
	return dimension;
}

double FWOptimizer::GetResult(int i) {
	if (i < dimension && xfinal) {
		return xfinal[i];
	} else {
		return 0;
	}
}

double* FWOptimizer::GetResults() {
	return xfinal;
}

double* FWOptimizer::GetProvisionalResults() {
	return xprovisional;
}

void FWOptimizer::SetESParameters(double xs, double st, double tf, double la, double mi){
	ixstart = xs;
	istddev = st;
	iTolFun = tf;
	ilambda = la;
	iMaxIter = mi;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void FWStaticTorqueOptimizer::Init() {
	// Save Initial Root Pos
	PHIKActuatorIf* root = phScene->GetIKActuator(0);
	while (root->GetParent()) { root = root->GetParent(); }
	if (DCAST(PHIKBallActuatorIf, root)) {
		initialRootPos = DCAST(PHIKBallActuatorIf, root)->GetJoint()->GetSocketSolid()->GetPose().Pos();
	}
	if (DCAST(PHIKHingeActuatorIf, root)) {
		initialRootPos = DCAST(PHIKHingeActuatorIf, root)->GetJoint()->GetSocketSolid()->GetPose().Pos();
	}

	// Save Initial Pos and Determine DOF
	int nJoints = phScene->NIKActuators();
	int dof = 3;
	initialPos.resize(nJoints);
	for (int i = 0; i < nJoints; ++i) {
		if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))) {
			PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint();
			if (jo->GetSpring() == 0){
				jo->SetSpring(1e+30);
			}
			if (jo->GetDamper() == 0){
				jo->SetDamper(1e+30);
			}
			initialPos[i].ori = jo->GetPosition();
			DSTR << "InitialPos[" << i << "]:" << initialPos[i].ori << std::endl;
			initialPos[i].ori = unityInitPos[i].ori;
			DSTR << "InitialPos[" << i << "]:" << initialPos[i].ori << std::endl;
			dof += 3;
		}
		if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))) {
			PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint();
			if (jo->GetSpring() == 0){
				jo->SetSpring(1e+30);
			}
			if (jo->GetDamper() == 0){
				jo->SetDamper(1e+30);
			}
			initialPos[i].angle = jo->GetPosition();
			DSTR << "InitialPos[" << i << "]:" << initialPos[i].angle << std::endl;
			initialPos[i].angle = unityInitPos[i].angle;
			dof += 1;
		}
	}
	for (int i = 0; i < groundConst.size(); i++){
		groundConst[i].Init();
	}
	for (int i = 0; i < ungroundedConst.size(); i++){
		ungroundedConst[i].Init();
	}
	for (int i = jointWeights.size(); i < nJoints; i++){
		jointWeights.push_back(1.0);
	}
	/*  graham動作確認
	srand((unsigned int)time(NULL));
	std::vector<Vec3f> randv;
	int boun = 20;
	for (int i = 0; i < 10; i++){
		randv.push_back(Vec3f(rand() % boun, 0, rand() % boun));
		DSTR << randv[i] << std::endl;
	}
	GrahamConvexHull(randv);
	*/
	int nSolids = phScene->NSolids();
	for (int i = 0; i < nSolids; i++){
		PHSolidIf* so = phScene->GetSolids()[i];
		if (so->NShape() > 0){
			for (int j = 0; j < so->NShape(); j++){
				DSTR << "CD(" << so->GetName() << "):" << so->GetShape(j)->GetName() << std::endl;
			}
		}
	}
	DSTR << errorWeight << stabilityWeight << torqueWeight << std::endl;
	//DSTR << sizeof(tor) << std::endl;
	//tor.resize(nJoints);
	tor = new TorqueResult[nJoints];
	//DSTR << sizeof(tor) << " " << nJoints << std::endl;
	FWOptimizer::Init(dof);
	double* initObj = new double[dof];
	Objective(initObj, dof);
}

double FWStaticTorqueOptimizer::ApplyPop(PHSceneIf* phScene, double const *x, int n) {
	int nJoints = phScene->NIKActuators();
	int cnt = 0;
	double obj = 0;

	double scale = 0.1;

	// Root Solid
	PHIKActuatorIf* root = phScene->GetIKActuator(0);
	while (root->GetParent()) { root = root->GetParent(); }
	Vec3d pos = Vec3d(x[cnt + 0], x[cnt + 1], x[cnt + 2]) * scale;
	if (DCAST(PHIKBallActuatorIf, root)) {
		DCAST(PHIKBallActuatorIf, root)->GetJoint()->GetSocketSolid()->SetFramePosition(pos + initialRootPos);
	}
	if (DCAST(PHIKHingeActuatorIf, root)) {
		DCAST(PHIKHingeActuatorIf, root)->GetJoint()->GetSocketSolid()->SetFramePosition(pos + initialRootPos);
	}
	obj += pos.norm();
	cnt += 3;

	// Joint
	for (int i = 0; i < nJoints; ++i) {
		if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))) {
			Vec3d rot = Vec3d(x[cnt + 0], x[cnt + 1], x[cnt + 2]) * scale;
			Quaterniond ori = Quaterniond::Rot(rot) * initialPos[i].ori;
			DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->SetJointTempOri(ori);
			DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->SetTargetPosition(ori);
			obj += pow(rot.norm(), 4);
			cnt += 3;
		}
		if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))) {
			double relAngle = x[cnt] * scale;
			double angle = relAngle + initialPos[i].angle;
			DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->SetJointTempAngle(angle);
			DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->SetTargetPosition(angle);
			obj += pow(relAngle, 4);
			cnt += 1;
		}
	}

	phScene->GetIKEngine()->FK();
	phScene->GetIKEngine()->ApplyExactState(true);

	// Find Lowest Solid
	/*
	double y = 1e+10; PHSolidIf* lowest = NULL;
	for (int i = 0; i < phScene->NSolids(); ++i) {
		PHSolidIf* so = phScene->GetSolids()[i];
		if (so->IsDynamical() && so->GetFramePosition().y < y) {
			y = so->GetFramePosition().y;
			lowest = so;
		}
	}
	if (lowest != NULL) {
		for (int i = 0; i < phScene->NSolids(); ++i) {
			PHSolidIf* so = phScene->GetSolids()[i];
			so->SetFramePosition(so->GetFramePosition() - Vec3d(0, y, 0));
		}
	}
	*/

	return obj;
}

double FWStaticTorqueOptimizer::Objective(double const *x, int n) {
	double obj = 0;
	int orcount = 0;
	int acount = 0;
	double tmp = 0;

	std::vector<Vec3f> groundVertices;

	// 1. Apply x to Scene
	obj += ApplyPop(phScene, x, n);

	// 2. Do Simulation Step And Calc Criterion
	int nJoints = phScene->NIKActuators();
	int nContacts = phScene->NContacts();

	//Contact Detect
	//ゆくゆくは接触してる点だけで支持多角形作りたい
	DSTR << "constacts:" << nContacts << std::endl;
	/*Posed conpose;
	for (int i = 0; i < nContacts; i++){
		phScene->GetContact(i)->GetSocketPose(conpose);
		DSTR << "contacts[" << i << "]:(socket)->" << phScene->GetContact(i)->GetSocketSolid()->GetName() << " (plug):" << phScene->GetContact(i)->GetPlugSolid()->GetName() << " pos->" << conpose << std::endl;
		DSTR << phScene->GetContact(i)->GetSocketSolid()->GetPose() << std::endl;
		conpose = phScene->GetContact(i)->GetSocketSolid()->GetPose() * conpose;
		DSTR << conpose << std::endl;
		groundVertices.push_back(conpose);
	}*/
	
	// b. Calc Error Criterion
	tmp = obj;
	for (int i = 0; i < phScene->NIKEndEffectors(); ++i) {
		PHIKEndEffectorIf* eef = phScene->GetIKEndEffector(i);
		if (eef->IsPositionControlEnabled()) {
			Vec3d diff = ((eef->GetSolid()->GetPose() * eef->GetTargetLocalPosition()) - eef->GetTargetPosition());
			obj += errorWeight * pow(diff.norm(), 2);
		}
		if (eef->IsOrientationControlEnabled()) {
			obj += errorWeight * ((eef->GetSolid()->GetOrientation() * Vec3d(1, 0, 0)) - (eef->GetTargetOrientation() * Vec3d(1, 0, 0))).norm();
			obj += errorWeight * ((eef->GetSolid()->GetOrientation() * Vec3d(0, 1, 0)) - (eef->GetTargetOrientation() * Vec3d(0, 1, 0))).norm();
		}
	}
	val.errorvalue = obj - tmp;
	/*for (int i = 0; i < phScene->NSolids(); i++){
		DSTR << "Solid[" << i << "]:" << phScene->GetSolids()[i]->GetName() << "(" << phScene->GetSolids()[i] << ")" << " : " << phScene->GetSolids()[i]->GetPose() << std::endl;
	}*/
	
	//gronded solids evaluation
	tmp = obj;
	for (int i = 0; i < groundConst.size(); i++){
		DSTR << groundConst[i].GetSolid()->GetName() << " " << groundConst[i].GetWeight() << " " << groundConst[i].GetInitialPose() << std::endl;
		DSTR << groundConst[i].GetSolid()->GetPose() << std::endl;
		double e = groundConst[i].CalcEvalFunc();
		DSTR << e << std::endl;
		int ns = groundConst[i].GetSolid()->NShape();
		
		//make supporting polygon from all vertices of body grounded collider
		for (int j = 0; j < ns; j++){
		    Vec3f* vertices;
			Vec3f v;
			CDShapeIf* shape = groundConst[i].GetSolid()->GetShape(0);
			if (DCAST(CDBoxIf, shape)){
				vertices = DCAST(CDBoxIf, shape)->GetVertices();
				for (int k = 0; k < 8; k++){
					v = groundConst[i].GetSolid()->GetPose() * vertices[k];
					//DSTR << "CDBoxVer[" << k << "]:" << v << std::endl;
					groundVertices.push_back(v);
				}
			}
			else if (DCAST(CDCapsuleIf, shape)){
				//
			}
		}
		
		obj += constWeight * e;
	}
	val.groundvalue = obj - tmp;

	//position constraint evaluation
	tmp = obj;
	for (int i = 0; i < ungroundedConst.size(); i++){
		DSTR << ungroundedConst[i].GetSolid()->GetName() << " " << ungroundedConst[i].GetWeight() << " " << ungroundedConst[i].GetInitialPose() << std::endl;
		DSTR << ungroundedConst[i].GetSolid()->GetPose() << std::endl;
		double e = ungroundedConst[i].CalcEvalFunc();
		DSTR << e << std::endl;
		obj += constWeight * e;
	}
	val.ungroundedvalue = obj - tmp;

	//center of gravity evaluation
	tmp = obj;
	if(!bodyIndex.empty()){
		int bSize = bodyIndex.size();
		Vec3d totalBodyGravPos = Vec3d();
		double totalBodyMass = 0;
		//Calculate position of cog
		for (int i = 0; i < bSize; i++){
			//DSTR << bodyIndex[i] << " " << phScene->GetSolids()[bodyIndex[i]]->GetName() << std::endl;
			Vec3d bodyGravPos = phScene->GetSolids()[bodyIndex[i]]->GetCenterPosition();
			double bodyMass = phScene->GetSolids()[bodyIndex[i]]->GetMass();
			//DSTR << bodyGravPos << totalBodyMass << std::endl;
			totalBodyGravPos = (bodyMass / (totalBodyMass + bodyMass)) * bodyGravPos + (totalBodyMass / (totalBodyMass + bodyMass)) * totalBodyGravPos;
			totalBodyMass += bodyMass;
		}
		DSTR << "Center of Gravity:" << totalBodyGravPos << " TotalMass:" << totalBodyMass << std::endl;
		DSTR << "Gravity:" << phScene->GetGravity() << std::endl;
		cog = totalBodyGravPos;
		//Create convex hull and calculate cog evaluation
		GrahamConvexHull gh = GrahamConvexHull();
		gh.Recalc(groundVertices);
		supportPolygon.resize(gh.hull.size());
		std::copy(gh.hull.begin(), gh.hull.end(), supportPolygon.begin());
		double e = gh.Eval(totalBodyGravPos);
		//double e = EvalCenterOfGravity(groundVertices, totalBodyGravPos);
		DSTR << "centerofgravity eval " << e << std::endl;
		obj += gravcenterWeight * totalBodyMass * e;
	}
	val.centervalue = obj - tmp;
	/*
	if (!bodyIndex.empty()){
		int bSize = bodyIndex.size();
		for (int i = 0; i < bSize; i++){
			Vec3d position = phScene->GetSolids()[bodyIndex[i]]->GetPose().Pos;
		}
	}
	*/


	for (int i = 0; i < 1; ++i) {
		phScene->Step();
	}
	//evaluate difference from initial joint ori
	tmp = obj;
	for (int i = 0; i < nJoints; i++){
		if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))){
			Quaterniond jQuaternion = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetPosition();
			Vec3d euler;
			jQuaternion.ToEuler(euler);
			Vec3d euler0;
			Quaterniond jQuaternion0 = initialPos[i].ori;
			jQuaternion0.ToEuler(euler0);
			DSTR << euler << " " << euler0 << std::endl;
			euler0 -= euler;
			DSTR << euler0 << " " << differentialWeight * (euler0.norm() * euler0.norm()) << std::endl;
			obj += differentialWeight * (euler0.norm() * euler0.norm());
		}
		else if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))){
			double angle = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetPosition() - initialPos[i].angle;
			DSTR << angle << " " << differentialWeight * angle * angle << std::endl;
			obj += differentialWeight * angle * angle;
		}
	}
	val.initialorivalue = obj - tmp;

	// c. Calc Torque Criterion
	//int cnt = 0;
	tmp = obj;
	if (!specificFlag){
		for (int i = 0; i < nJoints; ++i) {
			if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))) {
				Vec3d torque = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetMotorForce();
				DSTR << "torque[" << i << "](normal):" << torque << std::endl;
				Quaterniond jQuaternion = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetPosition();
				Vec3d euler;
				Posed socketPose;
				DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetSocketPose(socketPose);
				jQuaternion.ToEuler(euler);
				Vec3d resistTorque;
				for (int j = 0; j < 3; j++){
					if (jointspecs[i].specs[j].eulerDir){
						euler[j] *= -1;
						torque[j] *= -1;
					}
					//DSTR << "Balljoint:" << DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetName() << std::endl;
					//DSTR << "resist[" << i << "][" << cnt << "]:" << resist[cnt].k_1 << " " << resist[cnt].k_2 << " " << resist[cnt].k_3 << " " << resist[cnt].k_4 << " " << resist[cnt].min << " " << resist[cnt].max << std::endl;
					//DSTR << "euler angle:" << euler << std::endl;
					resistTorque[j] = exp(jointspecs[i].specs[j].k_1 * (euler[j] - jointspecs[i].specs[j].k_2)) - exp(jointspecs[i].specs[j].k_3 * (jointspecs[i].specs[j].k_4 - euler[j]));
					double AttentionAngle = 0.1*(jointspecs[i].specs[j].max - jointspecs[i].specs[j].min);
					if (euler[j] < jointspecs[i].specs[j].min || euler[j] > jointspecs[i].specs[j].max){
						DSTR << "OutOfRange[" << i << "][" << j << "]:" << euler[j] << std::endl;
						orcount++;
					}
					else if (euler[j] < jointspecs[i].specs[j].min + AttentionAngle || euler[j] > jointspecs[i].specs[j].max - AttentionAngle){
						DSTR << "Attention[" << i << "][" << j << "]:" << euler[j] << std::endl;
						acount++;
					}
					//DSTR << "torque:" << torque[j] << "resistTorque[" << i << "][" << j << "]:" << resistTorque[j] << std::endl;
					//DSTR << "resist torque:" << resistTorque << std::endl;
					//cnt++;
					//DSTR << "euler angle:" << euler << std::endl;
				}
				tor[i] = TorqueResult(torque, resistTorque);
				torque += resistWeight * resistTorque;
				DSTR << "torque[" << i << "](after):" << torque << std::endl;
				DSTR << "weight:" << jointspecs[i].weight << std::endl;
				obj += torqueWeight * jointspecs[i].weight * (torque.norm() * torque.norm());
			}
			if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))) {
				double torque = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetMotorForce();
				DSTR << "torque[" << i << "](normal):" << torque << std::endl;
				double euler = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetPosition();
				if (jointspecs[i].specs[2].eulerDir){
					euler *= -1;
					torque *= -1;
				}
				//DSTR << "Hingejoint:" << DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetName() << std::endl;
				//DSTR << "resist[" << i << "][" << cnt << "]:" << resist[cnt].k_1 << " " << resist[cnt].k_2 << " " << resist[cnt].k_3 << " " << resist[cnt].k_4 << " " << std::endl;
				//DSTR << "euler angle:" << euler << std::endl;
				double resistTorque = exp(jointspecs[i].specs[2].k_1 * (euler - jointspecs[i].specs[2].k_2)) - exp(jointspecs[i].specs[2].k_3 * (jointspecs[i].specs[2].k_4 - euler));
				double AttentionAngle = 0.1*(jointspecs[i].specs[2].max - jointspecs[i].specs[2].min);
				if (euler < jointspecs[i].specs[2].min || euler > jointspecs[i].specs[2].max){
					DSTR << "OutOfRange[" << i << "]" << euler << std::endl;
					orcount++;
				}
				else if (euler < jointspecs[i].specs[2].min + AttentionAngle || euler > jointspecs[i].specs[2].max - AttentionAngle){
					DSTR << "Attention[" << i << "]" << euler << std::endl;
					acount++;
				}
				//DSTR << "torque[" << i << "]:" << torque << " resistTorque[" << i << "]:" << resistTorque << std::endl;
				tor[i] = TorqueResult(Vec3d(0, 0, torque), Vec3d(0, 0, resistTorque));
				torque += resistWeight * resistTorque;
				//DSTR << "resist torque:" << resistTorque << std::endl;
				//cnt++;
				DSTR << "torque[" << i << "](after):" << torque << std::endl;
				obj += torqueWeight * jointspecs[i].weight * (torque * torque);
			}
			/*
			if (i == 0){
			cnt = 0;
			}
			if (i == 5){
			cnt = 6;
			}
			if (i == 11){
			cnt = 13;
			}
			*/
		}
	}
	else{
		double mag = nJoints / specificJoints.size();
		for (int i = 0; i < specificJoints.size(); ++i) {
			if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(specificJoints[i]))) {
				Vec3d torque = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetMotorForce();
				DSTR << "torque[" << specificJoints[i] << "](normal):" << torque << std::endl;
				Quaterniond jQuaternion = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetPosition();
				Vec3d euler;
				Posed socketPose;
				DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetSocketPose(socketPose);
				jQuaternion.ToEuler(euler);
				for (int j = 0; j < 3; j++){
					if (jointspecs[i].specs[j].eulerDir){
						euler[j] *= -1;
					}
					DSTR << "Balljoint:" << DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetName() << std::endl;
					DSTR << "euler angle:" << euler << std::endl;
					double resistTorque = exp(jointspecs[i].specs[j].k_1 * (euler[j] - jointspecs[i].specs[j].k_2)) - exp(jointspecs[i].specs[j].k_3 * (jointspecs[i].specs[j].k_4 - euler[j]));
					DSTR << "resistTorque[" << specificJoints[i] << "][" << j << "]:" << resistTorque << std::endl;
					torque[j] += resistWeight * resistTorque;
					//DSTR << "resist torque:" << resistTorque << std::endl;
					if (euler[j] < jointspecs[i].specs[j].min || euler[j] > jointspecs[i].specs[j].max){
						DSTR << "OutOfRange[" << specificJoints[i] << "][" << j << "]:" << euler[j] << std::endl;
						orcount++;
					}
					//cnt++;
				}
				DSTR << "torque[" << specificJoints[i] << "](after):" << torque << std::endl;
				obj += mag * torqueWeight * jointWeights[specificJoints[i]] * (torque.norm() * torque.norm());
			}
			if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(specificJoints[i]))) {
				double torque = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetMotorForce();
				DSTR << "torque[" << specificJoints[i] << "](normal):" << torque << std::endl;
				double euler = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetPosition();
				if (jointspecs[i].specs[2].eulerDir){
					euler *= -1;
				}
				DSTR << "Hingejoint:" << DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(specificJoints[i]))->GetJoint()->GetName() << std::endl;
				DSTR << "euler angle:" << euler << std::endl;
				double resistTorque = exp(jointspecs[i].specs[2].k_1 * (euler - jointspecs[i].specs[2].k_2)) - exp(jointspecs[i].specs[2].k_3 * (jointspecs[i].specs[2].k_4 - euler));
				DSTR << "resistTorque[" << specificJoints[i] << "]:" << resistTorque << std::endl;
				torque += resistWeight * resistTorque;
				//DSTR << "resist torque:" << resistTorque << std::endl;
				if (euler < jointspecs[i].specs[2].min || euler > jointspecs[i].specs[2].max){
					DSTR << "OutOfRange[" << specificJoints[i] << "]" << euler << std::endl;
					orcount++;
				}
				//cnt++;
				obj += mag * torqueWeight * jointWeights[specificJoints[i]] * (torque * torque);
			}
		}
	}
	val.torquevalue = obj - tmp;

	// d. Calc Stability Criterion

	//Evaluate from body solids velocity and angular velocity
	tmp = obj;
	for (int j = 0; j < phScene->NSolids(); ++j) {
		obj += stabilityWeight * phScene->GetSolids()[j]->GetVelocity().norm();
		obj += stabilityWeight * phScene->GetSolids()[j]->GetAngularVelocity().norm();
	}
	val.stabilityvalue = obj - tmp;
	
	DSTR << "orcount:" << orcount << std::endl;
	DSTR << "acount:" << acount << std::endl;
	DSTR << "error:" << val.errorvalue << " torque:" << val.torquevalue << " stability:" << val.stabilityvalue << std::endl;
	DSTR << "grounded:" << val.groundvalue << " ungrounded:" << val.ungroundedvalue << " center:" << val.centervalue << " iniori:" << val.initialorivalue << std::endl;
	DSTR << "obj:" << obj << std::endl;

	/*
	Vec3d force  = Vec3d();
	Vec3d torque = Vec3d();
	Vec3d CoM    = Vec3d();

	for (int i = 0; i < phScene->NSolids(); ++i) {
		PHSolidIf* so = phScene->GetSolids()[i];
		Vec3d pos = so->GetPose() * so->GetCenterOfMass();
		pos += CoM;

		force += so->GetForce();
	}
	CoM *= (1.0 / phScene->NSolids());

	for (int i = 0; i < phScene->NContacts(); ++i) {
		PHContactPointIf* contact = phScene->GetContact(i);
		PHSolidIf* so; Vec3d contactPos;

		Vec3d f, t; contact->GetConstraintForce(f, t);
		force += f;

		if (contact->GetSocketSolid()->IsDynamical()) {
			so = contact->GetSocketSolid();
			Posed pose; contact->GetSocketPose(pose);
			contactPos = pose * so->GetPose().Pos();
		} else {
			so = contact->GetPlugSolid();
			Posed pose; contact->GetPlugPose(pose);
			contactPos = pose * so->GetPose().Pos();
		}

		torque += ((contactPos - CoM) % f);
	}

	obj += stabilityWeight * torque.norm();
	obj += stabilityWeight * force.norm();
	*/
	
	return obj;
}

Vec3d FWStaticTorqueOptimizer::CalcTorqueN(int n){
	if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(n))) {
		Vec3d torque = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(n))->GetJoint()->GetMotorForce();
		//DSTR << "torque[" << n << "](normal):" << torque << std::endl;
		Quaterniond jQuaternion = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(n))->GetJoint()->GetPosition();
		Vec3d euler;
		Posed socketPose;
		DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(n))->GetJoint()->GetSocketPose(socketPose);
		jQuaternion.ToEuler(euler);
		Vec3d resistTorque;
		for (int j = 0; j < 3; j++){
			if (jointspecs[n].specs[j].eulerDir){
				euler[j] *= -1;
				torque[j] *= -1;
			}
			//DSTR << "Balljoint:" << DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetName() << std::endl;
			//DSTR << "resist[" << i << "][" << cnt << "]:" << resist[cnt].k_1 << " " << resist[cnt].k_2 << " " << resist[cnt].k_3 << " " << resist[cnt].k_4 << " " << resist[cnt].min << " " << resist[cnt].max << std::endl;
			//DSTR << "euler angle:" << euler << std::endl;
			resistTorque[j] = exp(jointspecs[n].specs[j].k_1 * (euler[j] - jointspecs[n].specs[j].k_2)) - exp(jointspecs[n].specs[j].k_3 * (jointspecs[n].specs[j].k_4 - euler[j]));
			double AttentionAngle = 0.1*(jointspecs[n].specs[j].max - jointspecs[n].specs[j].min);
			//DSTR << "resistTorque[" << i << "][" << j << "]:" << resistTorque << std::endl;
			//DSTR << "resist torque:" << resistTorque << std::endl;
		}
		//tor[n] = TorqueResult(torque, resistTorque);
		torque += resistWeight * resistTorque;
		return torque;
	}
	if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(n))) {
		double torque = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(n))->GetJoint()->GetMotorForce();
		DSTR << "torque[" << n << "](normal):" << torque << std::endl;
		double euler = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(n))->GetJoint()->GetPosition();
		if (jointspecs[n].specs[2].eulerDir){
			euler *= -1;
			torque *= -1;
		}
		//DSTR << "Hingejoint:" << DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetName() << std::endl;
		//DSTR << "resist[" << i << "][" << cnt << "]:" << resist[cnt].k_1 << " " << resist[cnt].k_2 << " " << resist[cnt].k_3 << " " << resist[cnt].k_4 << " " << std::endl;
		//DSTR << "euler angle:" << euler << std::endl;
		double resistTorque = exp(jointspecs[n].specs[2].k_1 * (euler - jointspecs[n].specs[2].k_2)) - exp(jointspecs[n].specs[2].k_3 * (jointspecs[n].specs[2].k_4 - euler));
		double AttentionAngle = 0.1*(jointspecs[n].specs[2].max - jointspecs[n].specs[2].min);
		//DSTR << "resistTorque[" << i << "]:" << resistTorque << std::endl;
		//tor[n] = TorqueResult(Vec3d(0, 0, torque), Vec3d(0, 0, resistTorque));
		torque += resistWeight * resistTorque;
		//DSTR << "resist torque:" << resistTorque << std::endl;
		DSTR << "torque[" << n << "](after):" << torque << std::endl;
		return Vec3d(0.0, 0.0, torque);
	}
	return Vec3d(0.0, 0.0, 0.0);
}


TrajectoryPlanner::TrajectoryPlanner(){

}

TrajectoryPlanner::TrajectoryPlanner(int d, int i, double f, double q, int n, double mg, bool c, bool ik, bool jc, bool im, bool wf){
	this->depth = d;
	this->iterate = i;
	this->freq = f;
	this->qvalue = q;
	this->nsma = n;
	this->mag = mg;
	this->corFlag = c;
	this->IKFlag = ik;
	this->jointCorFlag = jc;
	this->ignoreMotors = im;
	this->waitFlag = wf;
}

void TrajectoryPlanner::Init(){
	PHIKActuatorIf* act;
	act = eef->GetParentActuator();
	eef->Enable(true);
	//depthがオーバーしてないかチェック
	for (int i = 0; i < depth; i++){
		if (act->GetParent()){
			act = act->GetParent();
		}
		else{
			depth = i + 1;
			break;
		}
	}
	moving = false;
	calced = false;
}

void TrajectoryPlanner::Init(int d, int i, double f, double q, int n, double mg, bool c, bool ik, bool jc, bool im, bool wf){
	this->depth = d;
	this->iterate = i;
	this->freq = f;
	this->qvalue = q;
	this->nsma = n;
	this->mag = mg;
	this->corFlag = c;
	this->IKFlag = ik;
	this->jointCorFlag = jc;
	this->ignoreMotors = im;
	this->waitFlag = wf;
	Init();
}

void TrajectoryPlanner::CalcTrajectory(Posed tPose, double mt, int LPFmode){
	
	//デバッグ用の出力ファイル準備
	time_t t = time(NULL);
	std::string filename;
	struct tm *pnow = localtime(&t);
	std::stringstream ss;
	ss << pnow->tm_year + 1900;

	if ((pnow->tm_mon + 1) < 10){
	ss << "0" << pnow->tm_mon + 1;
	}
	else{
	ss << pnow->tm_mon + 1;
	}

	if (pnow->tm_mday < 10){
	ss << "0" << pnow->tm_mday;
	}
	else{
	ss << pnow->tm_mday;
	}

	if (pnow->tm_hour < 10){
	ss << "0" << pnow->tm_hour;
	}
	else{
	ss << pnow->tm_hour;
	}

	if (pnow->tm_min < 10){
	ss << "0" << pnow->tm_min;
	}
	else{
	ss << pnow->tm_min;
	}

	if (pnow->tm_sec < 10){
	ss << "0" << pnow->tm_sec;
	}
	else{
	ss << pnow->tm_sec;
	}
	//ss << ".csv";

	ss >> filename;
	DSTR << filename << std::endl;
	//std::ofstream outfile("C:/Users/hirohitosatoh/Desktop/logs/" + filename + ".csv");
	std::ofstream outfile2("C:/Users/hirohitosatoh/Desktop/logs/traj" + filename + ".csv");
	std::ofstream outfile3("C:/Users/hirohitosatoh/Desktop/logs/torquechange" + filename + ".csv");
	std::ofstream outfile4("C:/Users/hirohitosatoh/Desktop/traj.csv", std::ios::out | std::ios::trunc);

	auto start = std::chrono::system_clock::now();

	//いろいろ初期化して準備
	PHIKActuatorIf* ika = eef->GetParentActuator();
	startPose = eef->GetSolid()->GetPose();
	targetPose = tPose;
	startStep = scene->GetCount();
	movtime = (int)(mt / scene->GetTimeStep());
	motorDOF = 0;
	for (int i = 0; i < depth; i++){
		if (DCAST(PHIKBallActuatorIf, ika)){
			motorDOF += 3 * DCAST(PHIKBallActuatorIf, ika)->GetJoint()->NMotors();
		}
		if (DCAST(PHIKHingeActuatorIf, ika)){
			motorDOF += DCAST(PHIKHingeActuatorIf, ika)->GetJoint()->NMotors();
		}
		ika = ika->GetParent();
	}
	ika = eef->GetParentActuator();
	sTorque.resize(motorDOF);
	torque.resize(motorDOF, movtime);
	int cnt = 0;
	/*trajJointData.resize(depth, movtime);
	trajData.resize(movtime);
	MinJerkTrajectory minjerk = MinJerkTrajectory(startPose, targetPose, movtime);
	for (int i = 0; i < movtime; i++){
		trajData[i] = minjerk.GetCurrentPose(i);
	}
	for (int i = 0; i < movtime; i++){
		for (int j = 0; j < depth; j++){
			trajJointData[j][i] = JointPos();
		}
	}*/
	trajJointData.resize(iterate + 1);
	trajData.resize(iterate + 1, movtime);
	MinJerkTrajectory minjerk = MinJerkTrajectory(startPose, targetPose, movtime);
	for (int i = 0; i < iterate + 1; i++){
		trajJointData[i] = TrajJoint(depth, movtime);
	}
	for (int i = 0; i < movtime; i++){
	    for (int k = 0; k < iterate + 1; k++){
		    trajData[k][i] = minjerk.GetCurrentPose(i);
	    }
		outfile4 << trajData[0][i].px << "," << trajData[0][i].py << "," << trajData[0][i].pz << std::endl;
		outfile2 << trajData[0][i].px << "," << trajData[0][i].py << "," << trajData[0][i].pz << std::endl;
	}
	torquechange.resize(motorDOF);
	best = 0;
	besttorque = INFINITY;

	//stateの保存
	states = ObjectStatesIf::Create();
	DSTR << scene->GetCount() << std::endl;
	DSTR << "Saved" << std::endl;
	Debug();
	states->SaveState(scene);

	//軌道計算の繰り返し計算
	//初期姿勢維持トルクの保存
	if (ignoreMotors){
		for (int i = 0; i < depth; i++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
					Vec3d t = jo->GetMotorForce();
					sTorque[cnt] = t.x;
					sTorque[cnt + 1] = t.y;
					sTorque[cnt + 2] = t.z;
					cnt += 3;
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
					sTorque[cnt++] = jo->GetMotorForce();
			}
			ika = ika->GetParent();
		}
	}
	else{
		for (int i = 0; i < depth; i++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				for (int j = 0; j < jo->NMotors(); j++){
					Vec3d t = jo->GetMotorForceN(j);
					sTorque[cnt] = t.x;
					sTorque[cnt + 1] = t.y;
					sTorque[cnt + 2] = t.z;
					cnt += 3;
				}
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				for (int j = 0; j < jo->NMotors(); j++){
					sTorque[cnt++] = jo->GetMotorForceN(j);
				}
			}
			ika = ika->GetParent();
		}
	}
	ika = eef->GetParentActuator();
	cnt = 0;

	for (int k = 0; k < iterate; k++){
	torquechange.clear();
	totalchange = 0;
		//軌道再生と各関節トルクの保存
		for (int i = 0; i < movtime; i++){
			eef->SetTargetPosition(trajData[k][i].Pos());
			if (jointCorFlag){
				for (int j = 0; j < depth; j++){
					if (DCAST(PHIKBallActuatorIf, ika)){
						DCAST(PHIKBallActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].ori);
					}
					if (DCAST(PHIKHingeActuatorIf, ika)){
						DCAST(PHIKHingeActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].angle);
					}
					ika = ika->GetParent();  //error
				}
				ika = eef->GetParentActuator();
			}

			VVector<double> tor;
			tor.resize(motorDOF);
			for (int j = 0; j < depth; j++){
				if (DCAST(PHIKBallActuatorIf, ika)){
					PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
					Vec3d t = jo->GetMotorForce();
					tor[cnt] = t.x;
					tor[cnt + 1] = t.y;
					tor[cnt + 2] = t.z;
					cnt += 3;
					//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
				}
				if (DCAST(PHIKHingeActuatorIf, ika)){
					PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
					tor[cnt++] = jo->GetMotorForce();
					//outfile3 << "0,0," << tm << ",";
				}
				//outfile << torque[3 * j][i] << "," << torque[3 * j + 1][i] << "," << torque[3 * j + 2][i] << ",";
				ika = ika->GetParent();  //error
			}
			ika = eef->GetParentActuator();
			DSTR << tor << std::endl;
			cnt = 0;
			scene->Step();
			for (int j = 0; j < depth; j++){
				if (DCAST(PHIKBallActuatorIf, ika)){
					PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
					Vec3d t = jo->GetMotorForce();
					DSTR << t << std::endl;
					torquechange[cnt] += std::pow(tor[cnt] - t.x, 2);
					torquechange[cnt + 1] += std::pow(tor[cnt + 1] - t.y, 2);
					torquechange[cnt + 2] += std::pow(tor[cnt + 2] - t.z, 2);
					cnt += 3;
					//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
				}
				if (DCAST(PHIKHingeActuatorIf, ika)){
					PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
					DSTR << jo->GetMotorForce() << std::endl;
					torquechange[cnt++] += std::pow((tor[cnt] - jo->GetMotorForce()), 2);
					//outfile3 << "0,0," << tm << ",";
				}
				//outfile << torque[3 * j][i] << "," << torque[3 * j + 1][i] << "," << torque[3 * j + 2][i] << ",";
				ika = ika->GetParent();  //error
			}
			ika = eef->GetParentActuator();
			cnt = 0;
			DSTR << torquechange << std::endl;

			//Posed eefpose = eef->GetSolid()->GetPose();
			//Vec3d eefVel = eef->GetSolid()->GetVelocity();
			//outfile << trajData[i].px << "," << trajData[i].py << "," << trajData[i].pz << ",";
			//outfile << eefpose.px << "," << eefpose.py << "," << eefpose.pz << ",";
			//outfile << eefVel.x << "," << eefVel.y << "," << eefVel.z << ",";
			if (ignoreMotors){
				for (int j = 0; j < depth; j++){
					if (DCAST(PHIKBallActuatorIf, ika)){
						PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
						Vec3d t = jo->GetMotorForce();
						torque[cnt][i] = t.x;
						torque[cnt + 1][i] = t.y;
						torque[cnt + 2][i] = t.z;
						cnt += 3;
						//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
					}
					if (DCAST(PHIKHingeActuatorIf, ika)){
						PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
							torque[cnt++][i] = jo->GetMotorForce();
							//outfile3 << "0,0," << tm << ",";
					}
					//outfile << torque[3 * j][i] << "," << torque[3 * j + 1][i] << "," << torque[3 * j + 2][i] << ",";
					ika = ika->GetParent();  //error
				}
			}
			else{
				for (int j = 0; j < depth; j++){
					if (DCAST(PHIKBallActuatorIf, ika)){
						PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
						for (int l = 0; l < jo->NMotors(); l++){
							Vec3d t = jo->GetMotorForceN(l);
							torque[cnt][i] = t.x;
							torque[cnt + 1][i] = t.y;
							torque[cnt + 2][i] = t.z;
							cnt += 3;
							//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
						}
					}
					if (DCAST(PHIKHingeActuatorIf, ika)){
						PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
						for (int l = 0; l < jo->NMotors(); l++){
							torque[cnt++][i] = jo->GetMotorForceN(l);
							//outfile3 << "0,0," << tm << ",";
						}
					}
					//outfile << torque[3 * j][i] << "," << torque[3 * j + 1][i] << "," << torque[3 * j + 2][i] << ",";
					ika = ika->GetParent();  //error
				}
			}
			ika = eef->GetParentActuator();
			cnt = 0;
			//outfile << std::endl;
			//outfile3 << std::endl;
		}
		//Debug();
		outfile3 << k << ",";
		for (int i = 0; i < motorDOF; i++){
			outfile3 << torquechange[i] << ",";
			totalchange += torquechange[i];
		}
		outfile3 << totalchange << std::endl;
		if (besttorque > totalchange){
			best = k;
			besttorque = totalchange;
		}
		//springdamper小さくしてからload
		double** sd = new double*[2];
		sd[0] = new double[depth];
		sd[1] = new double[depth];
		for (int i = 0; i < depth; i++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				sd[0][i] = jo->GetSpring();
				sd[1][i] = jo->GetDamper();
				jo->SetSpring(spring);
				jo->SetDamper(damper);
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				sd[0][i] = jo->GetSpring();
				sd[1][i] = jo->GetDamper();
				jo->SetSpring(spring);
				jo->SetDamper(damper);
			}
			ika = ika->GetParent();
		}
		ika = eef->GetParentActuator();
		//ロードしてIK周りの調整
		states->LoadState(scene);
		scene->GetIKEngine()->ApplyExactState();   //IKの一時的目標角?のリセット
		Debug();
		/*for (int i = 0; i < depth; i++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				jo->SetTargetPosition(jo->GetPosition());
				jo->SetTargetVelocity(jo->GetVelocity());
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				jo->SetTargetPosition(jo->GetPosition());
				jo->SetTargetVelocity(jo->GetVelocity());
			}
			ika = ika->GetParent();
		}
		ika = eef->GetParentActuator();*/
		//LoadScene();

		//トルクにLPFをかける
		PTM::VMatrixRow<double> torqueLPF;
		if (LPFmode == 0){
			torqueLPF = LPF::BiQuad(torque, scene->GetTimeStep(), freq, qvalue);
		}
		else if(LPFmode == 1){
			torqueLPF = LPF::NSMA(torque, nsma, mag, sTorque);
		}
		else{
			torqueLPF = torque;
		}

		//LPF後トルクデータで動かし軌道保存
		if (!IKFlag){
			scene->GetIKEngine()->Enable(false);
			eef->Enable(false);
		}
		for (int i = 0; i < movtime; i++){
			if (ignoreMotors){
				for (int j = 0; j < depth; j++){
					Vec3d delta;
					if (DCAST(PHIKBallActuatorIf, ika)){
						PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
						//オフセットを設定
						jo->SetOffsetForce(Vec3d(torqueLPF[cnt][i], torqueLPF[cnt + 1][i], torqueLPF[cnt + 2][i]));
						cnt += 3;
						DCAST(PHIKBallActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].ori);
						//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
					}
					if (DCAST(PHIKHingeActuatorIf, ika)){
						PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
						//オフセットを設定
						jo->SetOffsetForce(torqueLPF[cnt++][i] * scene->GetTimeStepInv());
						DCAST(PHIKHingeActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].angle);
						//outfile3 << "0,0," << tm << ",";
					}
					//outfile2 << torqueLPF[3 * j][i] << "," << torqueLPF[3 * j + 1][i] << "," << torqueLPF[3 * j + 2][i] << "," ;
					ika = ika->GetParent();
				}
			}
			else{
				for (int j = 0; j < depth; j++){
					Vec3d delta;
					if (DCAST(PHIKBallActuatorIf, ika)){
						PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
						//オフセットを設定
						for (int l = 0; l < jo->NMotors(); l++){
							jo->SetOffsetForceN(l, Vec3d(torqueLPF[cnt][i], torqueLPF[cnt + 1][i], torqueLPF[cnt + 2][i]));
							cnt += 3;
						}
						DCAST(PHIKBallActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].ori);
						//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
					}
					if (DCAST(PHIKHingeActuatorIf, ika)){
						PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
						//オフセットを設定
						for (int l = 0; l < jo->NMotors(); l++){
							jo->SetOffsetForceN(l, torqueLPF[cnt++][i] * scene->GetTimeStepInv());
						}
						DCAST(PHIKHingeActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].angle);
						//outfile3 << "0,0," << tm << ",";
					}
					//outfile2 << torqueLPF[3 * j][i] << "," << torqueLPF[3 * j + 1][i] << "," << torqueLPF[3 * j + 2][i] << "," ;
					ika = ika->GetParent();
				}
			}
			ika = eef->GetParentActuator();
			cnt = 0;
			if (IKFlag){
				eef->SetTargetPosition(trajData[k+1][i].Pos());
			}
			/*for (int j = 0; j < depth; j++){
				if (DCAST(PHIKBallActuatorIf, ika)){
					DCAST(PHIKBallActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].ori);
				}
				if (DCAST(PHIKHingeActuatorIf, ika)){
					DCAST(PHIKHingeActuatorIf, ika)->SetPullbackTarget(trajJointData[k].data[j][i].angle);
				}
				ika = ika->GetParent();
			}
			ika = eef->GetParentActuator();*/
			scene->Step();
			//Debug();
			trajData[k + 1][i] = eef->GetSolid()->GetPose();
			for (int j = 0; j < depth; j++){
				if (DCAST(PHIKBallActuatorIf, ika)){
					trajJointData[k + 1].data[j][i].ori = DCAST(PHIKBallActuatorIf, ika)->GetJoint()->GetPosition();
				}
				if (DCAST(PHIKHingeActuatorIf, ika)){
					trajJointData[k + 1].data[j][i].angle = DCAST(PHIKHingeActuatorIf, ika)->GetJoint()->GetPosition();
				}
				ika = ika->GetParent();
			}
			ika = eef->GetParentActuator();
			//Debug();
			//outfile2 << trajData[k + 1][i].px << "," << trajData[k + 1][i].py << "," << trajData[k + 1][i].pz << std::endl;
			//outfile3 << std::endl;
		}
		//無効化していたIKの最有効化
		if (!IKFlag){
			scene->GetIKEngine()->Enable(true);
			eef->Enable(true);
		}
		//目標位置への補正
		if (corFlag){
			TrajCorrection(k + 1);
		}
		for (int i = 0; i < movtime; i++){
			outfile4 << trajData[k + 1][i].px << "," << trajData[k + 1][i].py << "," << trajData[k + 1][i].pz << std::endl;
			outfile2 << trajData[k + 1][i].px << "," << trajData[k + 1][i].py << "," << trajData[k + 1][i].pz << std::endl;
		}
		states->LoadState(scene);
		/*for (int i = 0; i < depth; i++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				jo->SetTargetPosition(jo->GetPosition());
				jo->SetTargetVelocity(jo->GetVelocity());
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				jo->SetTargetPosition(jo->GetPosition());
				jo->SetTargetVelocity(jo->GetVelocity());
			}
			ika = ika->GetParent();
		}
		ika = eef->GetParentActuator();*/
		//LoadScene();
		//spring,damperをもとの値に
		for (int i = 0; i < depth; i++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				jo->SetSpring(sd[0][i]);
				jo->SetDamper(sd[1][i]);
				jo->SetOffsetForce(Vec3d(0, 0, 0));
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				jo->SetSpring(sd[0][i]);
				jo->SetDamper(sd[1][i]);
				jo->SetOffsetForce(0);
			}
			ika = ika->GetParent();
		}
		ika = eef->GetParentActuator();
		//Debug();
	}
	moving = true;
	calced = true;
	ite = best;

	auto end = std::chrono::system_clock::now();
	auto dur = end - start;
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	outfile3 << msec << std::endl;

	/*  //LPFのテスト
	std::ofstream outfile3("C:/Users/hirohitosatoh/Desktop/logs/LPStest.csv");
	PTM::VMatrixRow<double> test;
	test.resize(1, 400);
	for (int i = 0; i < 400; i++){
	int j = i % 10;
	if (j > 5){ test[0][i] = 0; }
	else{ test[0][i] = 1.0; }
	}
	PTM::VMatrixRow<double> testOutput = LPF::BiQuad(test, scene->GetTimeStep(), freq, qvalue);
	for (int i = 0; i < 400; i++){
	outfile3 << test[0][i] << "," << testOutput[0][i] << std::endl;
	}
	*/
}

void TrajectoryPlanner::TrajStep(bool step){
	int now = scene->GetCount() - startStep;
	DSTR << scene->GetCount() << " " << startStep << " " << trajData.width() << std::endl;
	//Posed nowTarget = minjerk.GetCurrentPose(now);
	//eef->SetTargetPosition(nowTarget.Pos());
	if (moving){
		if (now >= 0 && now < trajData.width()){
			eef->SetTargetPosition(trajData[ite][now].Pos());
			eef->SetTargetOrientation(trajData[ite][now].Ori());
			if (jointCorFlag){
				PHIKActuatorIf* ika = eef->GetParentActuator();
				for (int j = 0; j < depth; j++){
					if (DCAST(PHIKBallActuatorIf, ika)){
						DCAST(PHIKBallActuatorIf, ika)->SetPullbackTarget(trajJointData[ite].data[j][now].ori);
					}
					if (DCAST(PHIKHingeActuatorIf, ika)){
						DCAST(PHIKHingeActuatorIf, ika)->SetPullbackTarget(trajJointData[ite].data[j][now].angle);
					}
					ika = ika->GetParent();
				}
				ika = eef->GetParentActuator();
			}
		}
		else {
			double diff = (eef->GetSolid()->GetPose().Pos() - trajData[ite][movtime - 1].Pos()).norm();
			DSTR << diff << std::endl;
			if (diff < 0.1 || !waitFlag){
				moving = false;
				if (step){
					totalchange = 0;
					for (int i = 0; i < motorDOF; i++){
						totalchange += torquechange[i];
					}
					DSTR << torquechange << " " << totalchange << std::endl;
				}
			}
			else{
				eef->SetTargetPosition(trajData[ite][movtime - 1].Pos());
				eef->SetTargetOrientation(trajData[ite][movtime - 1].Ori());
				if (jointCorFlag){
					PHIKActuatorIf* ika = eef->GetParentActuator();
					for (int j = 0; j < depth; j++){
						if (DCAST(PHIKBallActuatorIf, ika)){
							DCAST(PHIKBallActuatorIf, ika)->SetPullbackTarget(trajJointData[ite].data[j][movtime - 1].ori);
						}
						if (DCAST(PHIKHingeActuatorIf, ika)){
							DCAST(PHIKHingeActuatorIf, ika)->SetPullbackTarget(trajJointData[ite].data[j][movtime - 1].angle);
						}
						ika = ika->GetParent();
					}
					ika = eef->GetParentActuator();
				}
			}
		}
	}
	if (step){
		PHIKActuatorIf* ika = eef->GetParentActuator();
		int cnt = 0;
		VVector<double> tor;
		tor.resize(motorDOF);
		for (int j = 0; j < depth; j++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				Vec3d t = jo->GetMotorForce();
				tor[cnt] = t.x;
				tor[cnt + 1] = t.y;
				tor[cnt + 2] = t.z;
				cnt += 3;
				//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				tor[cnt++] = jo->GetMotorForce();
				//outfile3 << "0,0," << tm << ",";
			}
			//outfile << torque[3 * j][i] << "," << torque[3 * j + 1][i] << "," << torque[3 * j + 2][i] << ",";
			ika = ika->GetParent();  //error
		}
		ika = eef->GetParentActuator();
		DSTR << tor << std::endl;
		cnt = 0;
		scene->Step();
		for (int j = 0; j < depth; j++){
			if (DCAST(PHIKBallActuatorIf, ika)){
				PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
				Vec3d t = jo->GetMotorForce();
				torquechange[cnt] += abs(tor[cnt] - t.x);
				torquechange[cnt + 1] += abs(tor[cnt + 1] - t.y);
				torquechange[cnt + 2] += abs(tor[cnt + 2] - t.z);
				cnt += 3;
				//outfile3 << tm.x << "," << tm.y << "," << tm.z << ",";
			}
			if (DCAST(PHIKHingeActuatorIf, ika)){
				PH1DJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
				torquechange[cnt++] += abs(tor[cnt] - jo->GetMotorForce());
				//outfile3 << "0,0," << tm << ",";
			}
			//outfile << torque[3 * j][i] << "," << torque[3 * j + 1][i] << "," << torque[3 * j + 2][i] << ",";
			ika = ika->GetParent();  //error
		}
		DSTR << torquechange << std::endl;
	}
	DSTR << now << eef->GetTargetPosition() << eef->GetSolid()->GetPose() << std::endl;
}

void TrajectoryPlanner::TrajCorrection(int k){
	std::ofstream outfile("C:/Users/hirohitosatoh/Desktop/logs/trajCorrectedData.csv");
	Posed end = trajData[k][movtime - 1];
	Vec3d v1 = targetPose.Pos() - startPose.Pos();
	Vec3d v2 = end.Pos() - startPose.Pos();
	Quaterniond rot;
	rot.RotationArc(v2, v1);
	double scale = v1.norm() / v2.norm();

	Quaterniond dq = end.Ori().Inv() * targetPose.Ori();    //Invって前からなのか?Quaternionの乗算の向きがわからない?

	for (int i = 0; i < movtime; i++){
		trajData[k][i].Pos() = scale * (rot * (trajData[k][i].Pos() - startPose.Pos())) + startPose.Pos();
		trajData[k][i].Ori() = interpolate(((double)(i + 1) / movtime), Quaterniond(), dq) * trajData[k][i].Ori(); 
		outfile << trajData[k][i].Pos().x << "," << trajData[k][i].Pos().y << "," << trajData[k][i].Pos().z << "," << std::endl;
	}
}

void TrajectoryPlanner::Debug(){
	int nsolids = scene->NSolids();
	for (int i = 0; i < nsolids; i++){
		PHSolidIf* so = scene->GetSolids()[i];
		DSTR << "Solid" << i << ": pose" << so->GetPose() << " velocity" << so->GetVelocity() << " force" << so->GetForce() << " torque" << so->GetTorque() << std::endl;
	}
	PHIKActuatorIf* ika = eef->GetParentActuator();
	for (int i = 0; i < depth; i++){
		Vec3d delta;
		Vec3d torque;
		Vec3d target;
		Vec3d targetVel;
		Vec3d vel;
		double sp, da;
		Vec3d offset;
		if (DCAST(PHIKBallActuatorIf, ika)){
			PHBallJointIf* jo = DCAST(PHIKBallActuatorIf, ika)->GetJoint();
			Quaterniond q = jo->GetPosition();
			q.ToEuler(delta);
			torque = jo->GetMotorForce();
			Quaterniond tq = jo->GetTargetPosition();
			tq.ToEuler(target);
			targetVel = jo->GetTargetVelocity();
			vel = jo->GetVelocity();
			sp = jo->GetSpring();
			da = jo->GetDamper();
			offset = jo->GetOffsetForce();
		}
		if (DCAST(PHIKHingeActuatorIf, ika)){
			PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, ika)->GetJoint();
			double q = jo->GetPosition();
			delta = Vec3d(0, 0, q);
			torque = Vec3d(0, 0,jo->GetMotorForce());
			double tq = jo->GetTargetPosition();
			target = Vec3d(0, 0, tq);
			targetVel = Vec3d(0, 0, jo->GetTargetVelocity());
			vel = Vec3d(0, 0, jo->GetVelocity());
			sp = jo->GetSpring();
			da = jo->GetDamper();
			offset = Vec3d(0, 0, jo->GetOffsetForce());
		}
		DSTR << "depth" << i << ": delta" << delta << " velocity" << vel << " torque" << torque << " targetPos" << target << " targeVel" << targetVel << sp << da << offset <<std::endl;
		ika = ika->GetParent();
	}
	ika = eef->GetParentActuator();
	DSTR << "EndeffectorPosition:" << eef->GetSolid()->GetPose() << std::endl;
	DSTR << "EndEffectorVelocity:" << eef->GetSolid()->GetVelocity() << std::endl;
	DSTR << "EndEffectorTarget:" << eef->GetTargetPosition() << std::endl;
	DSTR << "Gravity:" << scene->GetGravity() << std::endl << std::endl;
}

void TrajectoryPlanner::LoadScene(){
	states->LoadState(scene);
	DSTR << "Loaded" << std::endl;
	Debug();
	eef->SetTargetPosition(eef->GetSolid()->GetPose().Pos());
	//scene->SetGravity(Vec3d(0, 0, 0));
	for (int i = 0; i < 10; i++){
		scene->Step();
		DSTR << "after" << i + 1 << "step" << std::endl;
		Debug();
	}
	//scene->SetGravity(Vec3d(0, -9.8, 0));
	states->LoadState(scene);
	//Debug();
}

void TrajectoryPlanner::Replay(int ite){
	if (!calced) return;
	if (ite < 0 || ite > iterate) return;
	this->ite = ite;
	if (moving) return;
	moving = true;
	torquechange.clear();
	states->LoadState(scene);
}

}
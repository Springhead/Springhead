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

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

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
		xstart[i] = 0.5;
		stddev[i] = 0.3;
	}

	parameters->stopTolFun = 1e-2;
	parameters->lambda = 30;
	parameters->init(dimension, xstart, stddev);
	arFunvals = evo->init(*parameters);

	states = ObjectStatesIf::Create();
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
			jo->SetSpring(1e+30);
			jo->SetDamper(1e+30);
			initialPos[i].ori = jo->GetPosition();
			dof += 3;
		}
		if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))) {
			PHHingeJointIf* jo = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint();
			jo->SetSpring(1e+30);
			jo->SetDamper(1e+30);
			initialPos[i].angle = jo->GetPosition();
			dof += 1;
		}
	}
	FWOptimizer::Init(dof);
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

	// 1. Apply x to Scene
	obj += ApplyPop(phScene, x, n);

	// 2. Do Simulation Step And Calc Criterion
	int nJoints = phScene->NIKActuators();

	// b. Calc Error Criterion
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

	for (int i = 0; i < 5; ++i) {
		phScene->Step();
	}

	// c. Calc Torque Criterion
	for (int i = 0; i < nJoints; ++i) {
		if (DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))) {
			Vec3d torque = DCAST(PHIKBallActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetMotorForce();
			obj += torqueWeight * (torque.norm() * torque.norm());
		}
		if (DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))) {
			double torque = DCAST(PHIKHingeActuatorIf, phScene->GetIKActuator(i))->GetJoint()->GetMotorForce();
			obj += torqueWeight * (torque * torque);
		}
	}

	// d. Calc Stability Criterion
	for (int j = 0; j < phScene->NSolids(); ++j) {
		obj += stabilityWeight * phScene->GetSolids()[j]->GetVelocity().norm();
		obj += stabilityWeight * phScene->GetSolids()[j]->GetAngularVelocity().norm();
	}

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

}


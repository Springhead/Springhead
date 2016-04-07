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

void FWOptimizer::CopyScene(FWSceneIf* fwSceneInput) {
	// ファイル名は一考の余地あり
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

	parameters->init(dimension, xstart, stddev);
	arFunvals = evo->init(*parameters);

	states = ObjectStatesIf::Create();
	states->SaveState(fwScene);

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
				DSTR << "Generation : " << cnt << std::endl;
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
		states->LoadState(fwScene);
		arFunvals[i] = Objective(pop[i], (int)evo->get(CMAES<double>::Dimension));
	}

	// update the search distribution used for sampleDistribution()
	evo->updateDistribution(arFunvals);
#endif
}

double FWOptimizer::Objective(double const *x, int n) {
	// 1. Apply x to Scene
	PHSceneIf* phScene = fwScene->GetPHScene();
	for (int i = 0; i < phScene->NJoints(); ++i) {
		Vec3d f, t;
		PHHingeJointIf* jo = phScene->GetJoint(i)->Cast();
		jo->SetSpring(1000 * abs(x[2 * i]));
		jo->SetDamper(1000 * abs(x[2 * i + 1]));
	}

	double obj = 0;
	for (int i = 0; i < 100; ++i) {
		// 2. Do Simulation Step
		fwScene->Step();

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

}

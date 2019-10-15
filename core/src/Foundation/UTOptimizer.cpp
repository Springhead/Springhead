/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
//#include <Springhead.h>
#include <Foundation/UTOptimizer.h>
#include <Springhead.h>
#include <limits>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;
	UTCMAESOptimizerIf* UTCMAESOptimizerIf::Create() {
		return UTCMAESOptimizerIf::Create(UTCMAESOptimizerDesc());
	}
	UTCMAESOptimizerIf* UTCMAESOptimizerIf::Create(const UTCMAESOptimizerDesc& desc) {
		return (DBG_NEW UTCMAESOptimizer(desc))->Cast();
	}


	UTCMAESOptimizerDesc::UTCMAESOptimizerDesc() :
		stopMaxFunEvals(-1),
		facmaxeval(1.0),
		stopMaxIter(-1.0),
		stopTolFun(1e-12),
		stopTolFunHist(1e-13),
		stopTolX(0),
		stopTolUpXFactor(1e3),
		lambda(-1),
		mu(-1),
		mucov(-1),
		mueff(-1),
		weights(0),
		damps(-1),
		cs(-1),
		ccumcov(-1),
		ccov(-1),
		facupdateCmode(1),
		weightMode(UTCMA_UNINITIALIZED_WEIGHTS)
	{
		stStopFitness.flg = false;
		stStopFitness.val = -std::numeric_limits<double>::max();
		updateCmode.modulo = -1;
		updateCmode.maxtime = -1;
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	UTCMAESOptimizer::UTCMAESOptimizer(const UTCMAESOptimizerDesc& desc) {
		SetDesc(&desc);

		initialValue = NULL;
		initialStdDev = NULL;

#ifdef USE_CLOSED_SRC
		parameters = NULL;
		cmaes = NULL;
#endif
	}

	UTCMAESOptimizer::~UTCMAESOptimizer() {
		Clear();
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	void UTCMAESOptimizer::SetDimension(int dimension) {
		this->dimension = dimension;

		Clear();

		initialValue = new double[dimension];
		initialStdDev = new double[dimension];

#ifdef USE_CLOSED_SRC
		cmaes = new CMAES<double>();
		parameters = new Parameters<double>();
#endif
	}

	void UTCMAESOptimizer::Initialize() {
#ifdef USE_CLOSED_SRC
		// Copy Parameters from Descriptor to CMA-ESpp Parameters
		parameters->stopMaxFunEvals = stopMaxFunEvals;
		parameters->facmaxeval = facmaxeval;
		parameters->stopMaxIter = stopMaxIter;
		parameters->stopTolFun = stopTolFun;
		parameters->stopTolFunHist = stopTolFunHist;
		parameters->stopTolX = stopTolX;
		parameters->stopTolUpXFactor = stopTolUpXFactor;
		parameters->lambda = lambda;
		parameters->mu = mu;
		parameters->mucov = mucov;
		parameters->mueff = mueff;
		parameters->weights = weights;
		parameters->damps = damps;
		parameters->cs = cs;
		parameters->ccumcov = ccumcov;
		parameters->ccov = ccov;
		parameters->facupdateCmode = facupdateCmode;
		parameters->weightMode = (Parameters<double>::Weights)weightMode;
		parameters->stStopFitness.flg = stStopFitness.flg;
		parameters->stStopFitness.val = stStopFitness.val;
		parameters->updateCmode.modulo = updateCmode.modulo;
		parameters->updateCmode.maxtime = updateCmode.maxtime;

		// -----

		parameters->init(dimension, initialValue, initialStdDev);
		objectiveFunctionValues = cmaes->init(*parameters);

		// -----

		// Set First Population of First Generation
		optimizationFinished = cmaes->testForTermination();
		population = cmaes->samplePopulation();
		currGenerationNum = 0; currPopulationNum = 0;
#endif
	}

	void UTCMAESOptimizer::Next() {
		if (optimizationFinished) { return; }

		currPopulationNum++;

#ifdef USE_CLOSED_SRC
		if (currPopulationNum >= cmaes->get(CMAES<double>::Lambda)) {
			// update the search distribution used for sampleDistribution()
			cmaes->updateDistribution(objectiveFunctionValues);

			// check that the optimization is finished or not
			optimizationFinished = cmaes->testForTermination();

			if (optimizationFinished) {
				// Get Final Result
				finalValue = cmaes->getNew(CMAES<double>::XMean);

			} else {
				// Get Current Fitness
				currentFitness = cmaes->get(CMAES<double>::Fitness);

				// Generate lambda new search points, sample population
				population = cmaes->samplePopulation();

				currPopulationNum = 0;
				currGenerationNum++;
			}
		}
#endif
	}

}

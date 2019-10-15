#ifndef SPR_UTOPTIMIZER_H
#define SPR_UTOPTIMIZER_H

#include <Springhead.h>
#include <Foundation/SprObject.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/include/cmaes/cmaes.h"
#endif

namespace Spr{;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// 最適化計算
struct UTOptimizerIf : public ObjectIf {
	SPR_IFDEF(UTOptimizer);

	/// Set Number of Parameters
	void SetDimension(int dimension);

	/// Get Number of Parameters
	int GetDimension();

	/// Set Initial Value
	void SetInitialValue(double const *initialValue);

	/// Initialize Optimizer
	void Initialize();

	/// Get Parameter Value to Compute Objective Function
	double* GetPopulation();

	/// Set Objective Function Value Computation Result
	void SetObjectiveFunctionValue(double value);

	/// Proceed Optimize Process Step
	void Next();

	/// Check Optimization is Finished or Not
	bool IsFinished();

	/// Return Results
	double* GetResult();

	/// Return Fitness
	double GetFitness();
};


// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

struct UTCMAESOptimizerDesc;
/// CMA-ESによる最適化計算
struct UTCMAESOptimizerIf : public UTOptimizerIf {
	SPR_IFDEF(UTCMAESOptimizer);

	/// Set Initial Standard Deviation
	void SetInitialStdDev(double const *initialStdDev);

	/// Get Current Generation Number
	int GetCurrentGeneration();

	/// Get Current Population Number
	int GetCurrentPopulation();

	///	Create an instance
	static UTCMAESOptimizerIf* Create(const UTCMAESOptimizerDesc& desc);
	static UTCMAESOptimizerIf* Create();
};

/// CMAES最適化のデスクリプタ
struct UTCMAESOptimizerDesc {
	SPR_DESCDEF(UTCMAESOptimizer);

	/* Termination parameters. */
	//! Maximal number of objective function evaluations.
	double stopMaxFunEvals;
	double facmaxeval;
	//! Maximal number of iterations.
	double stopMaxIter;
	//! Minimal fitness value. Only activated if flg is true.
	struct { bool flg; double val; } stStopFitness;
	//! Minimal value difference.
	double stopTolFun;
	//! Minimal history value difference.
	double stopTolFunHist;
	//! Minimal search space step size.
	double stopTolX;
	//! Defines the maximal condition number.
	double stopTolUpXFactor;

	/* internal evolution strategy parameters */
	/**
	* Population size. Number of samples per iteration, at least two,
	* generally > 4.
	*/
	int lambda;
	/**
	* Number of individuals used to recompute the mean.
	*/
	int mu;
	double mucov;
	/**
	* Variance effective selection mass, should be lambda/4.
	*/
	double mueff;
	/**
	* Weights used to recombinate the mean sum up to one.
	*/
	double* weights;
	/**
	* Damping parameter for step-size adaption, d = inifinity or 0 means adaption
	* is turned off, usually close to one.
	*/
	double damps;
	/**
	* cs^-1 (approx. n/3) is the backward time horizon for the evolution path
	* ps and larger than one.
	*/
	double cs;
	double ccumcov;
	/**
	* ccov^-1 (approx. n/4) is the backward time horizon for the evolution path
	* pc and larger than one.
	*/
	double ccov;
	double diagonalCov;
	struct { double modulo; double maxtime; } updateCmode;
	double facupdateCmode;

	/**
	* Determines the method used to initialize the weights.
	*/
	enum Weights
	{
		UTCMA_UNINITIALIZED_WEIGHTS, UTCMA_LINEAR_WEIGHTS, UTCMA_EQUAL_WEIGHTS, UTCMA_LOG_WEIGHTS
	} weightMode;

	UTCMAESOptimizerDesc();
};

}

#endif //SPR_FWOPTIMIZER_H


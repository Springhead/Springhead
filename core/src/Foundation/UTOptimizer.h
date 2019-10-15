#ifndef UTOPTIMIZER_H
#define UTOPTIMIZER_H

#include <Foundation/SprUTOptimizer.h>
#include <Foundation/Object.h>

namespace Spr { ;

class UTOptimizer : public Object {
protected:
	int dimension;

	double *initialValue;

	double currentFitness;

public:
	SPR_OBJECTDEF(UTOptimizer);

	/// Constructor
	UTOptimizer() { }

	/// Destructor
	~UTOptimizer() { }

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	/// Set Number of Parameters
	void SetDimension(int dimension) { this->dimension = dimension; }

	/// Get Number of Parameters
	int GetDimension() { return dimension; }

	/// Set Initial Value
	void SetInitialValue(double const *initialValue) {
		for (int i = 0; i < dimension; i++) { this->initialValue[i] = initialValue[i]; }
	}

	/// Initialize Optimizer
	void Initialize() {}

	/// Get Parameter Value to Compute Objective Function
	double* GetPopulation() { return NULL; }

	/// Set Objective Function Value Computation Result
	void SetObjectiveFunctionValue(double value) {}

	/// Proceed Optimize Process Step
	void Next() {}

	/// Check Optimization is Finished or Not
	bool IsFinished() { return true; }

	/// Return Results
	double* GetResult() { return NULL; }

	/// Return Fitness
	double GetFitness() { return currentFitness; }
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

class UTCMAESOptimizer : public UTOptimizer, public UTCMAESOptimizerDesc {
protected:
	double *initialStdDev;

	double *objectiveFunctionValues;

	double *finalValue;

	double *const*population;

	int currGenerationNum;
	int currPopulationNum;
	bool optimizationFinished;

#ifdef USE_CLOSED_SRC
	Parameters<double>		*parameters;
	CMAES<double>			*cmaes;
#endif

public:
	SPR_OBJECTDEF(UTCMAESOptimizer);

	/// Constructor
	UTCMAESOptimizer(const UTCMAESOptimizerDesc& desc = UTCMAESOptimizerDesc());

	/// Destructor
	~UTCMAESOptimizer();

	/// Destruct Values
	void Clear() {
		if (initialValue) { delete initialValue; }
		if (initialStdDev) { delete initialStdDev; }

#ifdef USE_CLOSED_SRC
		if (parameters) { delete parameters; }
		if (cmaes) { delete cmaes; }
#endif
	}

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	/// Set Number of Parameters
	void SetDimension(int dimension);

	/// Initialize Optimizer
	void Initialize();

	/// Get Parameter Value to Compute Objective Function
	double* GetPopulation() { return population[currPopulationNum]; }

	/// Set Objective Function Value Computation Result
	void SetObjectiveFunctionValue(double value) { objectiveFunctionValues[currPopulationNum] = value; }

	/// Proceed Optimize Process Step
	void Next();

	/// Check Optimization is Finished or Not
	bool IsFinished() { return optimizationFinished; }

	/// Return Results
	double* GetResult() { return finalValue; }

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	/// Set Initial Standard Deviation
	void SetInitialStdDev(double const *initialStdDev) {
		for (int i = 0; i < dimension; i++) { this->initialStdDev[i] = initialStdDev[i]; }
	}

	/// Get Current Generation Number
	int GetCurrentGeneration() { return currGenerationNum; }

	/// Get Current Population Number
	int GetCurrentPopulation() { return currPopulationNum; }
};

}

#endif

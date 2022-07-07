#ifndef UTOPTIMIZER_H
#define UTOPTIMIZER_H

#include <Foundation/SprUTOptimizer.h>
#include <Foundation/Object.h>

namespace Spr {
	;

	class UTOptimizer : public Object {
	protected:
		int dimension;

		double* initialValue;

		double currentFitness;

	public:
		SPR_OBJECTDEF(UTOptimizer);

		/// Constructor
		UTOptimizer() : dimension(0), initialValue(NULL), currentFitness(DBL_MAX) { }

		/// Destructor
		virtual ~UTOptimizer() { }

		// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

		/// Set Number of Parameters
		virtual void SetDimension(int dimension) { this->dimension = dimension; }

		/// Get Number of Parameters
		virtual int GetDimension() { return dimension; }

		/// Set Initial Value
		virtual void SetInitialValue(const double* initialValue) {
			for (int i = 0; i < dimension; i++) { this->initialValue[i] = initialValue[i]; }
		}

		/// Initialize Optimizer
		virtual void Initialize() {}

		/// Get Parameter Value to Compute Objective Function
		virtual double* GetPopulation() { return NULL; }
		virtual double* GetPopulation(int index) { return NULL; }
		virtual int GetPopulationSize() = 0;

		/// Set Objective Function Value Computation Result
		virtual void SetObjectiveFunctionValue(double value) {}
		virtual void SetObjectiveFunctionValue(double value, int index) {}

		/// Proceed Optimize Process Step
		virtual void Next() {}
		bool NextGeneration() {}

		/// Check Optimization is Finished or Not
		virtual bool IsFinished() { return true; }

		/// Return Results
		virtual double* GetResult() { return NULL; }

		/// Return Fitness
		virtual double GetFitness() { return currentFitness; }

	};

	// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

	class UTCMAESOptimizer : public UTOptimizer, public UTCMAESOptimizerDesc {
	protected:
		double* initialStdDev;

		double* objectiveFunctionValues;

		double* finalValue;

		double* const* population;

		int currGenerationNum;
		int currPopulationNum;
		bool optimizationFinished;

#ifdef USE_CLOSED_SRC
		Parameters<double>* parameters;
		CMAES<double>* cmaes;
#endif

	public:
		SPR_OBJECTDEF(UTCMAESOptimizer);
		ACCESS_DESC(UTCMAESOptimizer);

		/// Constructor
		UTCMAESOptimizer(const UTCMAESOptimizerDesc& desc = UTCMAESOptimizerDesc());

		/// Destructor
		~UTCMAESOptimizer();

		/// Destruct Values
		void Clear() {
			if (initialValue) { delete initialValue; initialValue = NULL; }
			if (initialStdDev) { delete initialStdDev; initialStdDev = NULL; }
			if (finalValue) { delete finalValue;  finalValue = NULL; }

#ifdef USE_CLOSED_SRC
			if (parameters) { delete parameters; parameters = NULL; }
			if (cmaes) { delete cmaes; cmaes = NULL; }
#endif
		}

		// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

		/// Set Number of Parameters
		void SetDimension(int dimension);

		/// Initialize Optimizer
		void Initialize();

		/// Get Parameter Value to Compute Objective Function
		double* GetPopulation() { return population[currPopulationNum]; }
		int GetPopulationSize();

		double* GetPopulation(int index) { return population[index]; }

		/// Set Objective Function Value Computation Result
		void SetObjectiveFunctionValue(double value) { objectiveFunctionValues[currPopulationNum] = value; }
		void SetObjectiveFunctionValue(double value, int index) { objectiveFunctionValues[index] = value; }

		/// Proceed Optimize Process Step
		void Next();
		///	Generate population for next generation. When finished it returns false.
		bool NextGeneration();

		/// Check Optimization is Finished or Not
		bool IsFinished() { return optimizationFinished; }

		/// Return Results
		double* GetResult() { return finalValue; }

		// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

		/// Set Initial Standard Deviation
		void SetInitialStdDev(const double* initialStdDev) {
			for (int i = 0; i < dimension; i++) {
				this->initialStdDev[i] = initialStdDev[i];
			}
		}

		/// Get Current Generation Number
		int GetCurrentGeneration() { return currGenerationNum; }

		/// Get Current Population Number
		int GetCurrentPopulation() { return currPopulationNum; }

		double GetCs() { return cs; }
		double GetLambda() { return lambda; }
	};

}
#endif

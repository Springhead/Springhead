#ifndef FWOPTIMIZER_H
#define FWOPTIMIZER_H

#include <Framework/SprFWOptimizer.h>
#include <Framework/FrameworkDecl.hpp>
#include <Foundation/Object.h>

namespace Spr { ;

class FWOptimizer : public Object {
protected:
	static FWOptimizer*		instance;
	UTRef<PHSceneIf>		phScene;
	UTRef<UTTimerIf>		thread;
	UTRef<ObjectStatesIf>	states;
	bool					bRunning;
	bool					bInitialized;

	int						dimension;
	double					*arFunvals, *xfinal, *xstart, *stddev, *xprovisional;
	double					*const*pop;
	double                  ixstart = 0.3, istddev = 0.3;

#ifdef USE_CLOSED_SRC
	Parameters<double>		*parameters;
	CMAES<double>			*evo;
	double                  iTolFun = 10, ilambda = 30, iMaxIter = 500;
#endif

	static void SPR_CDECL FWOptimizer_ThreadCallback(int id, void* arg);

public:
	SPR_OBJECTDEF(FWOptimizer);
	SPR_DECLMEMBEROF_FWOptimizerDesc;
	/// Constructor
	FWOptimizer(const FWOptimizerDesc& desc = FWOptimizerDesc());

	/// Destructor
	~FWOptimizer();

	/// Copy PHScene from passed scene
	void CopyScene(PHSceneIf* phSceneInput);

	/// Get FWScene
	PHSceneIf* GetScene();

	/// Initialize Optimizer
	void Init(int dimension);

	/// Start Optimization
	void Start();

	/// Abort Optimization
	void Abort();

	/// Check if Running
	bool IsRunning();

	/// Optimization Thread
	void Optimize();

	/// Optimization Iteration Step
	void Iterate();

	/// Apply Poplulation to Scene
	double ApplyPop(PHSceneIf* phScene, double const *x, int n);

	/// Objective Function to Minimize
	double Objective(double const *x, int n);

	/// Return Dimension of Result
	int NResults();

	/// Return Result
	double GetResult(int i);

	/// Return Results
	double* GetResults();

	/// Return Provisional Results
	double* GetProvisionalResults();

	void SetESParameters(double xs, double st, double tf, double la, double mi);
};

}

#endif

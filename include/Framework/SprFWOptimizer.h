#ifndef SPR_FWOPTIMIZER_H
#define SPR_FWOPTIMIZER_H

#include <Springhead.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/include/cmaes/cmaes.h"
#endif

namespace Spr{;

/*
  物理シミュレーションを使った最適化計算の基本クラス
  FWAppのようにこのクラスを継承して独自の最適化アプリケーションを作成することを想定する  
*/
class FWOptimizer : public UTRefCount {
protected:
	static FWOptimizer*		instance;
	UTRef<FWSdkIf>			fwSdk;
	UTRef<FWSceneIf>		fwScene;
	UTRef<UTTimerIf>		thread;
	UTRef<ObjectStatesIf>	states;
	bool					bRunning;
	bool					bInitialized;

	int						dimension;
	double					*arFunvals, *xfinal, *xstart, *stddev, *xprovisional;
	double					*const*pop;

#ifdef USE_CLOSED_SRC
	Parameters<double>		*parameters;
	CMAES<double>			*evo;
#endif

	static void FWOptimizer_ThreadCallback(int id, void* arg);

public:
	/// Constructor
	FWOptimizer();

	/// Destructor
	~FWOptimizer();

	/// Copy PHScene from passed scene
	void CopyScene(FWSceneIf* fwSceneInput);

	/// Get FWScene
	FWSceneIf* GetScene() { return fwScene; }

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

	/// Objective Function to Minimize
	virtual double Objective(double const *x, int n);

	/// Return Dimension of Result
	int NResults();

	/// Return Result
	double GetResult(int i);

	/// Return Results
	double* GetResults();

	/// Return Provisional Results
	double* GetProvisionalResults();
};

}

#endif //SPR_FWOPTIMIZER_H


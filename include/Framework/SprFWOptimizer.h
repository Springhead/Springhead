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
	UTRef<PHSceneIf>		phScene;
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

	static void SPR_CDECL FWOptimizer_ThreadCallback(int id, void* arg);

public:
	/// Constructor
	FWOptimizer();

	/// Destructor
	~FWOptimizer();

	/// Copy PHScene from passed scene
	void CopyScene(PHSceneIf* phSceneInput);

	/// Get FWScene
	PHSceneIf* GetScene() { return phScene; }

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
	virtual double ApplyPop(PHSceneIf* phScene, double const *x, int n);

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

class FWStaticTorqueOptimizer : public FWOptimizer {
	struct JointPos {
		Quaterniond ori;
		double angle;
		JointPos() { ori = Quaterniond(); angle = 0; }
	};
	std::vector<JointPos> initialPos;
	Vec3d initialRootPos;

	double errorWeight, stabilityWeight, torqueWeight;

public:
	virtual void Init();

	virtual void Iterate() { FWOptimizer::Iterate(); }

	virtual void ApplyResult(PHSceneIf* phScene) { FWOptimizer::ApplyPop(phScene, GetResults(), NResults()); }

	virtual double ApplyPop(PHSceneIf* phScene, double const *x, int n);

	virtual double Objective(double const *x, int n);

	void SetScene(PHSceneIf* phSceneInput) { phScene = phSceneInput; }

	void Optimize() { FWOptimizer::Optimize(); }

#ifdef USE_CLOSED_SRC
	bool TestForTermination() { return evo->testForTermination(); }

	void TakeFinalValue() { xfinal = evo->getNew(CMAES<double>::XMean); }
#endif

	void SetErrorWeight(double v) { errorWeight = v; }

	void SetStabilityWeight(double v) { stabilityWeight = v; }

	void SetTorqueWeight(double v) { torqueWeight = v; }
};

}

#endif //SPR_FWOPTIMIZER_H


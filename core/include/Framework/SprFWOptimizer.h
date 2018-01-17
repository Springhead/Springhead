#ifndef SPR_FWOPTIMIZER_H
#define SPR_FWOPTIMIZER_H

#include <Springhead.h>
#include <Framework/SprFWObject.h>
#include <Foundation/SprObject.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/include/cmaes/cmaes.h"
#endif

#include<chrono> //trajectoryPlannerへ
#include<queue>

namespace Spr{;

/*
  物理シミュレーションを使った最適化計算の基本クラス
  FWAppのようにこのクラスを継承して独自の最適化アプリケーションを作成することを想定する  


*/

class FWGroundConstraint{
public:
	Vec3d cNormal;
	PHSolidIf* cSolid;
	Posed initialPose;
	double cWeight;
	Vec3d contactForce;
	Vec3d contactPoint;
public:
	FWGroundConstraint();
	FWGroundConstraint(double w, Vec3d n);
	double CalcEvalFunc();
	void Init();
};

class FWUngroundedConstraint{
public:
	Vec3i cAxis;
	PHSolidIf* cSolid;
	Posed initialPose;
	double cWeight;
public:
	FWUngroundedConstraint();
	FWUngroundedConstraint(double w, Vec3i a);
	double CalcEvalFunc();
	void Init();
};

struct JointPos {
	Quaterniond ori;
	double angle;
	JointPos();
	JointPos(Quaterniond q);
	JointPos(double a);
};

//Unityで評価値の内訳を表示するための転送用構造体
struct FWObjectiveValues{
	double errorvalue = 0;
	double torquevalue = 0;
	double stabilityvalue = 0;
	double groundvalue = 0;
	double ungroundedvalue = 0;
	double centervalue = 0;
	double initialorivalue = 0;
};

struct FWOptimizerIf : public ObjectIf {
	SPR_IFDEF(FWOptimizer);

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

struct FWOptimizerDesc{
	SPR_DESCDEF(FWOptimizer);

	double ixstart;
	double istddev;
	double iTolFun;
	double ilambda;
	double iMaxIter;

	FWOptimizerDesc() {
		ixstart = 0.3;
		istddev = 0.3;
		iTolFun = 10;
		ilambda = 30;
		iMaxIter = 500;
	}
};

struct FWStaticTorqueOptimizerIf : public FWOptimizerIf {
	SPR_IFDEF(FWStaticTorqueOptimizer);
	
	void Init();

	void Iterate();

	void ApplyResult(PHSceneIf* phScene);

	double ApplyPop(PHSceneIf* phScene, double const *x, int n);

	double Objective(double const *x, int n);

	void SetScene(PHSceneIf* phSceneInput);

	void Optimize();

	bool TestForTermination();

	void TakeFinalValue();
	
	void SetErrorWeight(double v);
	double GetErrorWeight();

	void SetStabilityWeight(double v);
	double GetStabilityWeight();

	void SetTorqueWeight(double v);
	double GetTorqueWeight();

	void SetResistWeight(double v);
	double GetResistWeight();

	void SetConstWeight(double v);
	double GetConstWeight();

	void SetGravcenterWeight(double v);
	double GetGravcenterWeight();

	void SetDifferentialWeight(double v);
	double GetDifferentialWeight();

	//構造体の配列を外部から取れないので１要素ずつpush
	void AddPositionConst(FWGroundConstraint* f);
	FWGroundConstraint GetGroundConst(int n);
	void ClearGroundConst();
	void AddPositionConst(FWUngroundedConstraint* f);
	FWUngroundedConstraint GetUngroundConst(int n);
	void ClearUngroundedConst();

	void SetESParameters(double xs, double st, double tf, double la, double mi);

	FWObjectiveValues GetObjectiveValues();
	
	Vec3f GetCenterOfGravity();

	int NSupportPolygonVertices();

	Vec3f GetSupportPolygonVerticesN(int n);
};

struct FWStaticTorqueOptimizerDesc : public FWOptimizerDesc{
	SPR_DESCDEF(FWStaticTorqueOptimizer);

	FWStaticTorqueOptimizerDesc() {

	}
};

//複数点指定用に必要そうなデータ
struct ControlPoint{
	Posed pose;
	Vec6d vel;
	Vec6d acc;
	int step;
	double time;
	bool velControl;
	bool accControl;
	bool timeControl;
	ControlPoint();
	ControlPoint(Posed p, int s, double t);
	ControlPoint(Posed p, Vec6d v, Vec6d a, int s, double t);
	//ControlPoint(ControlPoint& c);
};

struct FWTrajectoryPlannerIf : public ObjectIf{
	SPR_IFDEF(FWTrajectoryPlanner);

	//初期化系
	void Reset(int d, int i, int iv, int n, double mg, int c, bool wf, bool snc = false, double r = 1.0, double vRate = 0.65, bool vCorr = true, bool sc = false);
	void Init();
	void Init(int d, int i, int iv, int n, double mg, int c, bool wf, bool snc, double r = 1.0, double vRate = 0.65, bool sc = false);
	//jointの深さのチェックと投げ込み
	//void CheckAndSetJoints();
	//エンドエフェクタ設定
	void SetControlTarget(PHIKEndEffectorIf* e);
	//シーン設定
	void SetScene(PHSceneIf* s);
	void AddControlPoint(ControlPoint c); 
	//関節角度次元軌道計算
	void CalcTrajectory(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt);
	//N回目の繰り返しから再計算
	void RecalcFromIterationN(int n);
	//生成された軌道を実際適用
	void JointTrajStep(bool step);
	//
	bool Moving();
	//spring, damper set
	void SetPD(double s = 1e10, double d = 1e10, bool mul = true);
	//
	void SetWeights(std::vector<double> w);
	//replay
	void Replay(int ite, bool noncorrected = false);
	//return totalChange
	double GetTotalChange();
	//return best
	int GetBest();
	//
	void ReloadCorrected(int k, bool nc = false);
};

struct FWTrajectoryPlannerDesc {
	SPR_DESCDEF(FWTrajectoryPlanner);

	FWTrajectoryPlannerDesc() {

	}
};

}

#endif //SPR_FWOPTIMIZER_H


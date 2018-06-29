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

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// FWStaticTorqueOptimizerに使う拘束クラス

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
	Vec3d normal;
	PHSolidIf* cSolid;
	Posed initialPose;
	double cWeight;
public:
	FWUngroundedConstraint();
	FWUngroundedConstraint(double w, Vec3i a, Vec3d n = Vec3d());
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

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
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

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// Frameworkの最適化計算の基底クラス
// インタフェース
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

// デスクリプタ
struct FWOptimizerDesc{
	SPR_DESCDEF(FWOptimizer);

	/// Initial search space vector
	double ixstart;
	/// Inital standard deviation of the samples
	double istddev;
	/// Minimal value difference
	double iTolFun;
	/// Population size
	double ilambda;
	/// Max iteration
	double iMaxIter;

	FWOptimizerDesc() {
		ixstart = 0.3;
		istddev = 0.3;
		iTolFun = 0.1;
		ilambda = 30;
		iMaxIter = 500;
	}
};


// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// 姿勢最適化計算のインタフェース
struct FWStaticTorqueOptimizerIf : public FWOptimizerIf {
	SPR_IFDEF(FWStaticTorqueOptimizer);
	
	/// Init
	void Init();

	/// 
	void Iterate();

	/// 
	void ApplyResult(PHSceneIf* phScene);

	/// 
	double ApplyPop(PHSceneIf* phScene, double const *x, int n);

	/// 
	double Objective(double const *x, int n);

	/// 
	void SetScene(PHSceneIf* phSceneInput);

	/// 
	void Optimize();

	/// 
	bool TestForTermination();

	/// 
	void TakeFinalValue();
	
	// Set/Get errorWeight
	void SetErrorWeight(double v);
	double GetErrorWeight();

	// Set/Get stabilityWeight
	void SetStabilityWeight(double v);
	double GetStabilityWeight();

	// Set/Get torqueWeight
	void SetTorqueWeight(double v);
	double GetTorqueWeight();

	// Set/Get  constWeight
	void SetConstWeight(double v);
	double GetConstWeight();

	// Set/Get gravcenterWeight
	void SetGravcenterWeight(double v);
	double GetGravcenterWeight();

	// Set/Get  differentialWeight
	void SetDifferentialWeight(double v);
	double GetDifferentialWeight();

	// GroundConstraintの追加、取得、全消去
	void AddPositionConst(FWGroundConstraint* f);
	FWGroundConstraint GetGroundConst(int n);
	void ClearGroundConst();

	// UngroundedConstraintの追加、取得、全消去
	void AddPositionConst(FWUngroundedConstraint* f);
	FWUngroundedConstraint GetUngroundConst(int n);
	void ClearUngroundedConst();

	// トルク評価時の関節ウェイトの設定
	void SetJointWeight(PHJointIf* jo, double w);

	/// CMAESのパラメータの設定
	void SetESParameters(double xs, double st, double tf, double la, double mi);

	/// 評価値を取得
	FWObjectiveValues GetObjectiveValues();
	
	/// 重心位置取得
	Vec3f GetCenterOfGravity();

	/// 支持多角形に使われている点の数
	int NSupportPolygonVertices();

	/// 支持多角形に使われている点の取得
	Vec3f GetSupportPolygonVerticesN(int n);
};

// 姿勢最適化計算のデスクリプタ
struct FWStaticTorqueOptimizerDesc : public FWOptimizerDesc{
	SPR_DESCDEF(FWStaticTorqueOptimizer);

	FWStaticTorqueOptimizerDesc() {

	}
};


// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// トルク変化最小化計算

// 空間内の経由点指定用データ
struct ControlPoint{
	Posed pose;
	SpatialVector vel;
	SpatialVector acc;
	double time;
	bool velControl;
	bool accControl;
	bool timeControl;
	ControlPoint();
	ControlPoint(Posed p, double t);
	ControlPoint(Posed p, SpatialVector v, SpatialVector a, double t);
	//ControlPoint(ControlPoint& c);
};


// トルク変化最小化計算のインタフェース
struct FWTrajectoryPlannerIf : public ObjectIf{
	SPR_IFDEF(FWTrajectoryPlanner);

	void SetDepth(int d);
	int GetDepth();
	void SetMaxIterate(int i);
	int GetMaxIterate();
	void SetMaxLPF(int l);
	int GetMaxLPF();
	void SetLPFRate(double r);
	double GetLPFRate();
	void EnableCorrection(bool e);
	bool IsEnabledCorrection();
	void EnableStaticTarget(bool e);
	bool IsEnabledStaticTarget();
	void EnableSpringCorrection(bool e);
	bool IsEbabledSpringCorrection();
	void EnableJointMJTInitial(bool e);
	bool IsEbabledJointMJTInitial();

	void EnableViaCorrection(bool e);
	bool IsEnabledViaCorrection();
	void SetMaxIterateViaAdjust(int m);
	int GetMaxIterateViaAdjust();
	void SetViaAdjustRate(double r);
	double GetViaAdjustRate();

	void SetSpringRate(double s);
	double GetSpringRate();
	void SetDamperRate(double d);
	double GetDamperRate();
	void EnableMultiplePD(bool e);
	bool IsEnabledMultiplePD();

	void EnableChangeBias(bool e);
	bool IsEnabledChangeBias();
	void EnableChangePullback(bool e);
	bool IsEnabledChangePullback();

	//初期化系
	void ResetParameters(int d, int i, int iv, bool c, double r = 1.0, double vRate = 0.65, bool vCorr = true, bool sc = false);
	void Init();
	void Init(int d, int i, int iv, bool c, double r = 1.0, double vRate = 0.65, bool sc = false);
	//エンドエフェクタ設定
	void SetControlTarget(PHIKEndEffectorIf* e);
	//シーン設定
	void SetScene(PHSceneIf* s);
	void AddViaPoint(ControlPoint c); 
	//関節角度次元軌道計算
	void CalcTrajectory(ControlPoint tpoint, std::string output);
	//N回目の繰り返しから再計算
	void RecalcFromIterationN(int n);
	//生成された軌道を実際適用
	void JointTrajStep(bool step);
	//
	bool Moving();
	//spring, damper set
	void SetSpringDamper(double s = 1e5, double d = 1e5, bool mul = true);
	//replay
	void Replay(int ite, bool noncorrected = false);
	//return totalChange
	double GetTotalChange();
	//return best
	int GetBest();
	//
	void ReloadCorrected(int k, bool nc = false);
};

// トルク変化最小化計算のデスクリプタ
struct FWTrajectoryPlannerDesc {
	SPR_DESCDEF(FWTrajectoryPlanner);
	// Descにいろいろ移したい

	double depth;
	int maxIterate;
	int maxLPF;
	double LPFRate;
	bool bCorrection;
	bool bStaticTarget;
	bool bUseSpringCorrection;
	bool bUseJointMJTInitial;

	bool bViaCorrection;
	int maxIterateViaAdjust;
	double viaAdjustRate;

	double springRate;
	double damperRate;
	bool bMultiplePD;

	bool bChangeBias;
	bool bChangePullback;

	FWTrajectoryPlannerDesc() {
		depth = 3;
		maxIterate = 100;
		maxLPF = 10;
		LPFRate = 1.0;
		bCorrection = true;
		bStaticTarget = false;
		bUseSpringCorrection = false;
		bUseJointMJTInitial = false;

		bViaCorrection = false;
		maxIterateViaAdjust = 0;
		viaAdjustRate = 0.0;

		springRate = 1e5;
		damperRate = 1e5;
		bMultiplePD = true;

		bChangeBias = true;
		bChangePullback = false;
	}
};

}

#endif //SPR_FWOPTIMIZER_H


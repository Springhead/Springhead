#ifndef SPR_CRTRAJECTORYPLANNER_H
#define SPR_CRTRAJECTORYPLANNER_H

#include <Springhead.h>
#include <Foundation/SprObject.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/include/cmaes/cmaes.h"
#endif

namespace Spr { ;

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
// トルク変化最小化計算

// 空間内の経由点指定用データ
struct ControlPoint { // 改名 ViaPoint
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
struct CRTrajectoryPlannerIf : public ObjectIf {
	SPR_IFDEF(CRTrajectoryPlanner);

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

	//初期化
	void Init();
	//エンドエフェクタ設定
	void SetControlTarget(PHIKEndEffectorIf* e);
	//シーン設定
	void SetScene(PHSceneIf* s);
	void AddViaPoint(ControlPoint c);
	//関節角度次元軌道計算
	void CalcTrajectory();
	void CalcOneStep();
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
	//return best
	int GetBest();
	//
	void ReloadCorrected(int k, bool nc = false);

	Posed GetTrajctoryData(int k, int n);
	Posed GetNotCorrectedTrajctoryData(int k, int n);
	SpatialVector GetVeclocityData(int k, int n);
	SpatialVector GetNotCorrectedVelocityData(int k, int n);
};

// トルク変化最小化計算のデスクリプタ
struct CRTrajectoryPlannerDesc {
	SPR_DESCDEF(CRTrajectoryPlanner);
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

	std::string outputPath;

	CRTrajectoryPlannerDesc() {
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

		outputPath = "";
	}
};

}

#endif
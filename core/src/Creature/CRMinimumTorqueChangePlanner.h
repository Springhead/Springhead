/*
*  Copyright (c) 2003-2010, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef CRMINIMUMTORQUECHANGE_H
#define CRMINIMUMTORQUECHANGE_H

#include <Foundation/SprObject.h>
#include <Creature/SprCRMinimumTorqueChangePlanner.h>
#include <Creature/SprCRTimeSeries.h>
#include <Creature/SprCRCreature.h>
#include <Foundation/Object.h>
#include <Creature/CreatureDecl.hpp>

namespace Spr {;

/**  CRMinimumTorqueChangePlanner
     トルク変化最小軌道を用いて関節系をコントロールするための計算クラス
	 順逆緩和法による反復計算による近似で求める
	 参考論文
	  Title : Trajectory Formation of Arm Movement by a Neural Network with Forward and Inverse Dynamics Models
	  Authors : Yasuhiro WADA and Mitsuo KAWATO
	  Outline : They got approximate solution of Minimum Torque Change Model by using iterative method named FIRM(Forward Inverse Relaxation Model).

	 現在の問題：
	  動き初めに謎の跳ねが生じる
	  - 追従時にPDゲインを大きくしているせいかとも思ったが、1倍にしても発生する
	  - 初期軌道の開始姿勢に現在の位置を指定していたのが原因、PD制御なので今の姿勢を目標に設定してしまうとずれが生じる
	  繰り返し回数が多く必要なため計算に時間がかかる
	  - 1ステップ分の実時間経過で現状のものをとりあえず出力？
	  - シーンが複雑化するとそれだけシミュレーションに時間がかかるのも難点(シーンを分ける？)
	  うっかりしていたが、使用しない関節の剛体をstationaryにするだけだとIKの計算が狂う
	  - つまりはIKActuatorそのものを無効化する必要多分あり
	  TimeSeries構造体をベースにした実装に変更中
*/
class CRTrajectoryPlanner : public Object {
public:
	//Joint系の管理クラス(PHJointとは別)
	class Joint {
	public:
		virtual void Initialize(int iterate, double movetime, int nVia, double rate = 1.0, bool vCorr = true) = 0;
		virtual void MakeJointMinjerk(int cnt) = 0;
		virtual void CloseFile() = 0;
		virtual void SaveTorque(int n) = 0;
		virtual void SaveTarget() = 0;
		virtual void SetTarget(int k, int n) = 0;
		virtual void SetTargetVelocity(int k, int n) = 0;
		virtual void SetTargetInitial() = 0;
		virtual void SetOffsetFromLPF(int n) = 0;
		virtual void ResetOffset(double o) = 0;
		virtual void SavePosition(int k, int n) = 0;
		virtual void SaveVelocity(int k, int n) = 0;
		virtual void SaveViaPoint(int v, int t) = 0;
		virtual void SavePositionFromLPF(int k, int n) = 0;
		virtual void SaveVelocityFromLPF(int k, int n) = 0;
		virtual void TrajectoryCorrection(int k, bool s) = 0;
		virtual void ApplyLPF(int count) = 0;
		virtual void Soften() = 0;
		virtual void Harden() = 0;
		virtual void ResetPD() = 0;
		virtual double CalcTotalTorqueChange() = 0;
		virtual double CalcTotalTorqueChangeLPF() = 0;
		virtual double CalcTorqueChangeInSection(int n) = 0;
		virtual double GetBestTorqueChangeInSection(int n) = 0;
		virtual void SetBestTorqueChange() = 0;
		virtual void ShowInfo() = 0;
		virtual void SetTargetCurrent() = 0;
		virtual void SetTargetFromLPF(int k, int n) = 0;
		virtual void OutputTorque() = 0;
		virtual void SetPD(double s, double d, bool mul) = 0;
		virtual void UpdateIKParam(double b, double p) = 0;
		virtual double GetMaxForce() = 0;
		virtual void SetWeight(double w = 1.0) = 0;
	};

	struct LPF {
		// N-Simple Moving Average LPF
		template<class T>
		static PTM::VMatrixRow<T> NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s);

		template<class T>
		static PTM::VVector<T> centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial);

		template<class T>
		static PTM::VMatrixRow<T> weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w);

		template <class T>
		static PTM::VVector<T> weightedv(PTM::VVector<T> input, T initial, double w, double r = 1.0);
	};

private:
	// 発散したときに止める
	bool isDiverged;

	// ----- 関節の状態を保持するJointsTimeSeries -----
	std::vector<CRJointsTimeSeries> trajectorySatisfyingCondition;
	std::vector<CRJointsTimeSeries> trajectorySmoothed;

	// ----- トルクから生成した軌道データ群 -----
	std::vector<CRSolidsTimeSeries> solidsTrajectorySatisfyingCondition;
	std::vector<CRSolidsTimeSeries> solidsTrajectorySmoothed;

	// ----- Sceneと保存用のStates -----
	PHSceneIf* scene;
	UTRef<ObjectStatesIf> states;
	UTRef<ObjectStatesIf> initialStates;
	UTRefArray<ObjectStatesIf> tmpStates;
	UTRefArray<ObjectStatesIf> corStates;
	UTRefArray<ObjectStatesIf> beforeCorStates;


	// 計算フェイズ
	enum Phase {
		INIT,
		FORWARD,
		INVERSE,
		POST_PROCESS,
	};
	Phase phase;
	int currentIterate;
	bool bFinished;

	// フェイズシフト
	void PhaseShift() {
		switch (phase) {
		case Phase::FORWARD:
		case Phase::INIT:
			phase = Phase::INVERSE;
			break;
		case Phase::INVERSE:
			phase = Phase::FORWARD;
			break;
		}
	}

public:
	SPR_OBJECTDEF(CRTrajectoryPlanner);
	SPR_DECLMEMBEROF_CRTrajectoryPlannerDesc;
	//コンストラクタ
	CRTrajectoryPlanner(const CRTrajectoryPlannerDesc& desc = CRTrajectoryPlannerDesc()) {
		SetDesc(&desc);
	}

	// -----インタフェースの実装-----

	// ----- ディスクリプタ由来の各種パラメータのSetter/Getter -----
	void SetDepth(int d) { depth = d; }
	int GetDepth() { return depth; }
	void SetMaxIterate(int i) { maxIterate = i; }
	int GetMaxIterate() { return maxIterate; }
	void SetMaxLPF(int l) { maxLPF = l; }
	int GetMaxLPF() { return maxLPF; }
	void SetLPFRate(double r) { LPFRate = r; }
	double GetLPFRate() { return LPFRate; }
	void EnableCorrection(bool e) { bCorrection = e; }
	bool IsEnabledCorrection() { return bCorrection; }
	void EnableStaticTarget(bool e) { bStaticTarget = e; }
	bool IsEnabledStaticTarget() { return bStaticTarget; }
	void EnableSpringCorrection(bool e) { bUseSpringCorrection = e; }
	bool IsEbabledSpringCorrection() { return bUseSpringCorrection; }
	void EnableJointMJTInitial(bool e) { bUseJointMJTInitial = e; }
	bool IsEbabledJointMJTInitial() { return bUseJointMJTInitial; }

	void EnableViaCorrection(bool e) { bViaCorrection = e; }
	bool IsEnabledViaCorrection() { return bViaCorrection; }
	void SetMaxIterateViaAdjust(int m) { maxIterateViaAdjust = m; }
	int GetMaxIterateViaAdjust() { return maxIterateViaAdjust; }
	void SetViaAdjustRate(double r) { viaAdjustRate = r; }
	double GetViaAdjustRate() { return viaAdjustRate; }

	void SetSpringRate(double s) { springRate = s; }
	double GetSpringRate() { return springRate; }
	void SetDamperRate(double d) { damperRate = d; }
	double GetDamperRate() { return damperRate; }
	void EnableMultiplePD(bool e) { bMultiplePD = e; }
	bool IsEnabledMultiplePD() { return bMultiplePD; }

	void EnableChangeBias(bool e) { bChangeBias = e; }
	bool IsEnabledChangeBias() { return bChangeBias; }
	void EnableChangePullback(bool e) { bChangePullback = e; }
	bool IsEnabledChangePullback() { return bChangePullback; }

	// ----- それ以外のSetter -----

	// 初期化処理
	void Init();

	// 計算実行
	void CalcTrajectory() {};
	// 
	void CalcOneStep() {};

	// N回目の繰り返しから再計算
	void RecalcFromIterationN(int n) {};

	// 生成された軌道を実際適用
	void JointTrajStep(bool step) {};

	// spring, damper set
	void SetSpringDamper(double s = 1e10, double d = 1e10, bool mul = true) {
		this->springRate = s;
		this->damperRate = d;
		this->bMultiplePD = mul;
	}
	// replay
	void Replay(int ite, bool noncorrected = false) {};

	void ReloadCorrected(int k, bool nc = false) {};

	// getters for trajectory
	Posed GetTrajctoryData(int k, int n) { return Posed(); }
	Posed GetNotCorrectedTrajctoryData(int k, int n) { return Posed(); }
	SpatialVector GetVeclocityData(int k, int n) { return SpatialVector(); }
	SpatialVector GetNotCorrectedVelocityData(int k, int n) { return SpatialVector(); }
};

}

#endif
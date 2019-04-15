#ifndef CRTRAJECTORYPLANNER_H
#define CRTRAJECTORYPLANNER_H


#include <Creature/CRMinimumJerkTrajectory.h>
#include <Creature/SprCRTrajectoryPlanner.h>
#include <Foundation/Object.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHIK.h>
#include <Physics/SprPHScene.h>

#include <vector>

#include <Creature/CreatureDecl.hpp>


namespace Spr {
	;

	/**  FWTrajectoryPlanner
	トルク変化最小軌道を用いて関節系をコントロールするための計算クラス
	参考論文
	Title : Trajectory Formation of Arm Movement by a Neural Network with Forward and Inverse Dynamics Models
	Authors : Yasuhiro WADA and Mitsuo KAWATO
	Outline : They got approximate solution of Minimum Torque Change Model by using iterative method named FIRM(Forward Inverse Relaxation Model).

	バグについて
	解決済のバグ
	・動かし初めにジャンプが発生する
	-> 初期解の手先軌道生成時の開始手先位置に現在の手先ターゲットを指定することで解消
	残っているバグ
	　・ローパスをかける回数を0にして、修正も切っているのに軌道が変わっていく
	 -> IKを切り忘れていたり、PDゲインが適切じゃなかったり、といった原因と推測
	 特にPDゲインについては何かとシビア

	 問題
	 ・フォワードは忠実だと思うが、インバースの修正がうまくいかない
	 ・

	 */

	class CRTrajectoryPlanner : public Object {
	public:
		//Joint系の管理クラス(PHJointとは別)
		class Joint {
		public:
			//
			virtual void Initialize(int iterate, double movetime, int nVia, double rate = 1.0, bool vCorr = true) = 0;
			//
			virtual void MakeJointMinjerk(int cnt) = 0;
			//
			virtual void CloseFile() = 0;
			//
			virtual void SaveTorque(int n) = 0;
			//
			virtual void SaveTarget() = 0;
			virtual void SetTarget(int k, int n) = 0;
			//
			virtual void SetTargetVelocity(int k, int n) = 0;
			//
			virtual void SetTargetInitial() = 0;
			//
			virtual void SetOffsetFromLPF(int n) = 0;
			//
			virtual void ResetOffset(double o) = 0;
			//
			virtual void SavePosition(int k, int n) = 0;
			virtual void SaveVelocity(int k, int n) = 0;
			//
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
			virtual void SetPullbackTarget(int k, int n) = 0;
			virtual void SetPullbackTargetFromInitial() = 0;
		};
		class HingeJoint : public Joint {
		public:
			PHIKHingeActuatorIf * hinge;          //アクチュエータ
			PHSceneIf* scene;

			PTM::VVector<double> torque;         //Inverse時に記録したトルク
			PTM::VVector<double> torqueLPF;      //LPF後トルク
			PTM::VMatrixRow<double> angle;       //Inverseで動かす用角度(修正込み)
			PTM::VMatrixRow<double> angleLPF;    //Forward時に記録する角度
			PTM::VMatrixRow<double> angleVels;
			PTM::VMatrixRow<double> angleVelsLPF;

			double targetAngle;                  //ターゲットとなる関節角  finalAngle
			double targetVel;                    //ターゲットとなる角速度(通常は0)  finalAngularVelocity
			PTM::VVector<double> viaAngles;      //経由点における角度
			PTM::VVector<double> viaVels;        //経由点における角速度
			PTM::VVector<double> viatimes;       //経由点の通過時間

			double initialTorque;                //開始時の発揮トルク  torque[0]
			double initialAngle;                 //開始時の関節角度  angle[i][0]
			double initialVel;                   //開始時の角速度   angleVel[i][0]
			double initialPullbackTarget;   // 開始時のIKプルバックターゲット

			double weight = 1.0;                 //評価ウェイト
			double rateLPF = 1.0;                //LPFのレート
			double originalSpring;               //元のばね定数
			double originalDamper;               //元のダンパ定数
			double hardenSpring = 1e30;  //
			double hardenDamper = 1e8;
			bool mul = true;

			int iterate;                         //繰り返し回数  *
			double mtime;                        //所要時間(s)   *
			int movetime;                        //所要ステップ  *
			bool viaCorrect;                     // *
			PTM::VMatrixRow<double> CorrTraj;    // 使ってなかった

			PTM::VVector<double> torqueChange;
			PTM::VVector<double> torqueChangeLPF;
			PTM::VVector<double> tChanges;

		private:
			std::ofstream* torGraph;
			std::ofstream* torChangeGraph;
			bool outputEnable = false;

		public:
			HingeJoint(PHIKHingeActuatorIf* hinge, std::string path, bool oe);
			~HingeJoint();
			void Initialize(int iterate, double mtime, int nVia, double rate = 1.0, bool vCorr = true);
			void MakeJointMinjerk(int cnt);
			void CloseFile();
			void SaveTorque(int n);
			void SaveTarget();
			void SetTarget(int k, int n);
			void SetTargetVelocity(int k, int n);
			void SetTargetInitial();
			void SetOffsetFromLPF(int n);
			void ResetOffset(double o);
			void SavePosition(int k, int n);
			void SaveVelocity(int k, int n);
			void SavePositionFromLPF(int k, int n);
			void SaveVelocityFromLPF(int k, int n);
			void SaveViaPoint(int v, int t);
			void TrajectoryCorrection(int k, bool s);
			void ApplyLPF(int count);
			void Soften();
			void Harden();
			void ResetPD();
			double CalcTotalTorqueChange();
			double CalcTotalTorqueChangeLPF();
			double CalcTorqueChangeInSection(int n);
			double GetBestTorqueChangeInSection(int n);
			void SetBestTorqueChange();
			void ShowInfo();
			void SetTargetCurrent();
			void SetPD(double s, double d, bool mul);
			void SetTargetFromLPF(int k, int n);
			void UpdateIKParam(double b, double p);
			void OutputTorque();
			double GetMaxForce() { return hinge->GetJoint()->GetMaxForce(); }
			void SetWeight(double w) { weight = w; }
			void SetPullbackTarget(int k, int n);
			void SetPullbackTargetFromInitial();
		};
		class BallJoint : public Joint {
		public:
			PHIKBallActuatorIf * ball;            // アクチュエータ
			PHSceneIf* scene;

			/// 
			PTM::VVector<Vec3d> torque;          // Inverse時に記録したトルク
			PTM::VVector<Vec3d> torqueLPF;       // LPF後トルク
			PTM::VMatrixRow<Quaterniond> ori;    // Inverseで動かす用角度(修正込み)
			PTM::VMatrixRow<Quaterniond> oriLPF; // Forward時に記録する角度
			PTM::VMatrixRow<Vec3d> oriVels;
			PTM::VMatrixRow<Vec3d> oriVelsLPF;

			Quaterniond targetOri;               // ターゲットとなる関節角
			Vec3d targetVel;                     // ターゲットとなる角速度(通常は0)
			PTM::VVector<Quaterniond> viaOris;   // 経由点における角度
			PTM::VVector<Vec3d> viaVels;         // 経由点における角速度
			PTM::VVector<double> viatimes;          // 経由点の通過時間

			Vec3d initialTorque;                 // 開始時の発揮トルク
			Quaterniond initialOri;              // 開始時の関節角度
			Vec3d initialVel;                    // 開始時の角速度
			Quaterniond initialPullbackTarget;   // 開始時のIKプルバックターゲット

			int iterate;                         // 繰り返し回数
			double mtime;                        // 所要時間(s)
			int movetime;                        // 所要ステップ
			double weight = 1.0;                 // 評価ウェイト
			double rateLPF = 1.0;                // LPFのレート
			double originalSpring;               // 元のばね定数
			double originalDamper;               // 元のダンパ定数
			double hardenSpring = 1e30;
			double hardenDamper = 1e8;
			bool mul = true;
			PTM::VVector<double> tChanges;
			bool viaCorrect;
			PTM::VMatrixRow<Quaterniond> CorrTraj; // 経由点修正に使う何か(今はコメントアウトされている)

			PTM::VVector<double> torqueChange;
			PTM::VVector<double> torqueChangeLPF;

		private:
			std::ofstream* torGraph;
			std::ofstream* torChangeGraph;
			bool outputEnable = false;

		public:
			BallJoint(PHIKBallActuatorIf* ball, std::string path, bool oe);
			~BallJoint();
			void Initialize(int iterate, double mtime, int nVia, double rate = 1.0, bool vCorr = true);
			void MakeJointMinjerk(int cnt);
			void CloseFile();
			void SaveTorque(int n);
			void SaveTarget();
			void SetTarget(int k, int n);
			void SetTargetVelocity(int k, int n);
			void SetTargetInitial();
			void SetOffsetFromLPF(int n);
			void ResetOffset(double o);
			void SavePosition(int k, int n);
			void SaveVelocity(int k, int n);
			void SaveViaPoint(int v, int t);
			void SavePositionFromLPF(int k, int n);
			void SaveVelocityFromLPF(int k, int n);
			void TrajectoryCorrection(int k, bool s);
			void SetBestTorqueChange();
			void ApplyLPF(int count);
			void Soften();
			void Harden();
			void ResetPD();
			double CalcTotalTorqueChange();
			double CalcTotalTorqueChangeLPF();
			double CalcTorqueChangeInSection(int n);
			double GetBestTorqueChangeInSection(int n);
			void ShowInfo();
			void SetTargetCurrent();
			void SetPD(double s, double d, bool mul);
			void SetTargetFromLPF(int k, int n);
			void UpdateIKParam(double b, double p);
			void OutputTorque();
			double GetMaxForce() { return ball->GetJoint()->GetMaxForce(); }
			void SetWeight(double w) { weight = w; }
			void SetPullbackTarget(int k, int n);
			void SetPullbackTargetFromInitial();
		};
		class Joints {
			friend class Joint;
		public:
			std::vector<Joint*> joints;
			CRTrajectoryPlannerIf* fwPlanner;
		public:
			Joints();
			~Joints();
			void RemoveAll();
			void Add(PHIKActuatorIf* j, std::string path, bool oe = true);
			void Initialize(int iterate, double movetime, int nVia, double rate = 1.0, bool vCorr = true);
			void MakeJointMinjerk(int cnt);
			void CloseFile();
			void SetTarget(int k, int n);
			void SetTargetVelocity(int k, int n);
			void SetTargetInitial();
			void SaveTorque(int n);
			void SaveTarget();
			void SetOffsetFromLPF(int n);
			void ResetOffset(double o);
			void SavePosition(int k, int n);
			void SaveVelocity(int k, int n);
			void SaveViaPoint(int v, int t);
			void SavePositionFromLPF(int k, int n);
			void SaveVelocityFromLPF(int k, int n);
			void TrajectoryCorrection(int k, bool s);
			void SetBestTorqueChange();
			double GetBestTorqueChangeInSection(int n);
			void ApplyLPF(int count);
			void Soften();
			void Harden();
			void ResetPD();
			double CalcTotalTorqueChange();
			double CalcTotalTorqueChange(std::ofstream& o);
			double CalcTotalTorqueChangeLPF();
			double CalcTorqueChangeInSection(int n);
			void ShowInfo();
			void SetTargetCurrent();
			void SetWeight();
			void SetPD(double s, double d, bool mul);
			void SetTargetFromLPF(int k, int n);
			void UpdateIKParam(double b, double p);
			void OutputTorque();
			void SetPullbackTarget(int k, int n);
			void SetPullbackTargetFromInitial();
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
		// ----- 計算にかかわるもの -----
		// 操作対象となるエンドエフェクタ
		PHIKEndEffectorIf * ikEndEffector;
		// 開始姿勢
		//ControlPoint startPoint = ControlPoint();
		Posed initialTargetPose;
		SpatialVector initialTargetVel;
		SpatialVector initialTargetAcc;
		// 目標姿勢
		PHSolidIf* targetSolid;
		PHSpringIf* targetSpring;
		// 途中通過点
		std::vector<ControlPoint> viaPoints;
		std::vector<PHSolidIf*> viaSolids;
		std::vector<PHSpringIf*> viaSprings;
		// 移動時間を秒で表したもの
		double mtime;
		// 移動時間をStep数にしたもの(mtime要らない？)
		int movtime;
		// 考慮するActuator
		Joints joints;
		// 発散したときに止める
		bool isDiverged;

		// ----- Sceneと保存用のStates -----
		PHSceneIf* scene;
		UTRef<ObjectStatesIf> states;
		UTRef<ObjectStatesIf> initialStates;
		UTRefArray<ObjectStatesIf> tmpStates;
		UTRefArray<ObjectStatesIf> corStates;
		UTRefArray<ObjectStatesIf> beforeCorStates;

		// ----- トルクから生成した軌道データ群 -----
		PTM::VMatrixRow<Posed> trajData;
		PTM::VMatrixRow<Posed> trajDataNotCorrected;
		PTM::VMatrixRow<SpatialVector> trajVel;
		PTM::VMatrixRow<SpatialVector> trajVelNotCorrected;

		// ----- フラグ系 -----
		// 再生時にまだ移動中かどうか
		bool isMoving;
		// 計算済みかのフラグ
		bool isCalculated;

		// ----- リプレイ関係 -----
		// (replay時の)再生軌道index、通常は最後のもの
		int ite;
		// リプレイ時の適用ステップ
		int repCount;
		// 極小値をとった軌道の番号
		int best;

		// ----- Unityからどこまで適用するかのフラグ -----
		// 修正前軌道を再生するかのフラグ
		bool noncorrectedReplay;

		// トルク変化
		double bestTorque;
		std::vector<double> torqueChangeRecord;

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

		// ----- 計算用関数 -----

		// 適用関節を確認し、計算用構造体ベクトルに保存
		void CheckAndSetJoints();
		// デバッグ用情報表示
		void DisplayDebugInfo();
		// 軌道データの出力
		void OutputTrajectory(std::string filename);
		void OutputNotCorrectedTrajectory(std::string filename);
		void OutputVelocity(std::string filename);

		// Forward Inverse Relaxation Model
		void ForwardInverseRelaxation();
		// スタックした経由点からMJTを作成
		void MakeMinJerk();

		// Forward model(torque -> pos)
		void CompForwardDynamics(int k);
		// Inverse model(pos -> torque)
		void CompInverseDynamics(int k);
		//
		void TrajectoryCorrection(int k);
		//
		void PostProcessing();
		// viatime adjustment
		bool AdjustViatime();
		// viatime initialize
		void InitializeViatime();
		// Prepare solids and springs for correction
		void PrepareSprings();

		// 
		//
		int TimeToStep(double t) {
			t *= scene->GetTimeStepInv();
			return std::round(t);
		}
		void LoadInitialState() {
			states->LoadState(scene);
			//scene->GetIKEngine()->ApplyExactState();
		}
		SpatialVector GetEndEffectorVelocity() {
			Vec3d vel = ikEndEffector->GetSolid()->GetVelocity();
			Vec3d angularVel = ikEndEffector->GetSolid()->GetAngularVelocity();
			Vec3d localVel = vel + PTM::cross(angularVel, ikEndEffector->GetSolid()->GetPose().Ori() * ikEndEffector->GetTargetLocalPosition());
			Vec3d localAngularVel = angularVel;
			return SpatialVector(localVel, localAngularVel);
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

		// エンドエフェクタ設定
		void SetControlTarget(PHIKEndEffectorIf* e) { this->ikEndEffector = e; }
		// シーン設定
		void SetScene(PHSceneIf* s) { this->scene = s; }

		// 初期化処理
		void Init();

		// 経由点の追加
		void AddViaPoint(ControlPoint c) { viaPoints.push_back(c); }

		// 計算実行
		void CalcTrajectory();
		// 
		void CalcOneStep();

		// N回目の繰り返しから再計算
		void RecalcFromIterationN(int n);

		// 生成された軌道を実際適用
		void JointTrajStep(bool step);

		// moving?
		bool Moving() { return isMoving; }
		// spring, damper set
		void SetSpringDamper(double s = 1e10, double d = 1e10, bool mul = true) {
			this->springRate = s;
			this->damperRate = d;
			this->bMultiplePD = mul;
		}
		// replay
		void Replay(int ite, bool noncorrected = false);
		// return best
		int GetBest() { return best; }
		void ReloadCorrected(int k, bool nc = false);

		// getters for trajectory
		Posed GetTrajctoryData(int k, int n) { return Posed(); }
		Posed GetNotCorrectedTrajctoryData(int k, int n) { return Posed(); }
		SpatialVector GetVeclocityData(int k, int n) { return SpatialVector(); }
		SpatialVector GetNotCorrectedVelocityData(int k, int n) { return SpatialVector(); }
	};
}

#endif

#ifndef FWTRAJECTORYPLANNER_H
#define FWTRAJECTORYPLANNER_H

#include <Framework/SprFWOptimizer.h>
#include <Foundation/Object.h>
#include <Framework/FrameworkDecl.hpp>

namespace Spr {
	; 
	class MinJerkTrajectory {
	private:
		ControlPoint sPoint;
		ControlPoint fPoint;
		int stime;
		int ftime;
		ControlPoint vPoint;
		int vtime;
		PTM::TMatrixRow<6, 3, double> coeffToV;
		PTM::TMatrixRow<6, 3, double> coeffToF;
	public:
		//コンストラクタ(多項式の係数を決める)
		//default
		MinJerkTrajectory();
		//開始位置と終端位置のみ
		MinJerkTrajectory(Posed spose, Posed fpose, int time);
		//経由点込み(経由点での速度と加速度は内部で計算)
		MinJerkTrajectory(Posed spose, Posed fpose, Posed vpose, int time, int vtime);
		//開始と終端の位置、速度、加速度指定
		MinJerkTrajectory(Posed spose, Posed fpose, Vec3d sVel, Vec3d fVel, Vec3d sAcc, Vec3d fAcc, int time, double per);
		MinJerkTrajectory(ControlPoint spoint, ControlPoint fpoint, double per);
		//開始と終点の位置、速度と通過点の位置、通過時間指定
		MinJerkTrajectory(Posed spose, Posed fpose, Vec3d sVel, Vec3d fVel, Posed vPose, int vtime, int time, double per);
		MinJerkTrajectory(ControlPoint spoint, ControlPoint fpoint, ControlPoint vpoint, double per);
		Posed GetCurrentPose(int t);
		Posed GetDeltaPose(int t);
		double GetCurrentActiveness(int t);
	};

	class AngleMinJerkTrajectory {
	private:
		double sAngle;
		double fAngle;
		int stime;
		int ftime;
		double vAngle;
		int vtime;
		PTM::TVector<6, double> coeffToV;
		PTM::TVector<6, double> coeffToF;
	public:
		//コンストラクタ(多項式の係数を決める)
		//default
		AngleMinJerkTrajectory();
		//開始位置と終端位置のみ
		AngleMinJerkTrajectory(double sangle, double fangle, int time);
		AngleMinJerkTrajectory(double sangle, double fangle, double sVel, double fVel, double sAcc, double fAcc, int time, double per);
        AngleMinJerkTrajectory(double sangle, double fangle, double sVel, double fVel, double vangle, double vVel, int time, int vtime, double per);
		AngleMinJerkTrajectory(double vAngle, int time, int vtime, double per);
		double GetCurrentAngle(int t);
		double GetDeltaAngle(int t);
		double GetCurrentVelocity(int t);
	};

	class QuaMinJerkTrajectory {
	private:
		Quaterniond sQua;
		Quaterniond fQua;
		int stime;
		int ftime;
		Quaterniond vQua;
		int vtime;
		Vec3d axis;
		double angle;
		AngleMinJerkTrajectory* amjt;
		PTM::VVector<Quaterniond> velToQua;
		PTM::TMatrixRow<6, 1, double> coeffToV;
		PTM::TMatrixRow<6, 1, double> coeffToF;
	public:
		//コンストラクタ(多項式の係数を決める)
		//default
		QuaMinJerkTrajectory();
		//開始位置と終端位置のみ
		QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, int time, double per);
		QuaMinJerkTrajectory(Quaterniond squa, Quaterniond fqua, Vec3d sVel, Vec3d fVel, Vec3d sAcc, Vec3d fAcc, int time, double per);
		QuaMinJerkTrajectory(Quaterniond vqua, int time, int vtime, double per);
		Quaterniond GetCurrentQuaternion(int t);
		Quaterniond GetDeltaQuaternion(int t);
	};

	class FWTrajectoryPlanner : public Object{

		//Joint系の管理クラス
		class Joint {
		public:
			virtual void Initialize(int iterate, int movetime) {};
			virtual void SaveTorque(int n) = 0;
			virtual void SaveTarget() = 0;
			virtual void SetTarget(int k, int n) = 0;
			virtual void SetOffsetFromLPF(int n) = 0;
			virtual void ResetOffset() = 0;
			virtual void SavePosition(int k, int n) = 0;
			virtual void SaveViaPoint(int v, int t) = 0;
			virtual void SavePositionFromLPF(int k, int n) = 0;
			virtual void TrajectoryCorrection(int k, bool s) = 0;
			virtual void TrajectoryCorrectionWithVia(int k, bool s) = 0;
			virtual void ApplyLPF(int lpf, int count) = 0;
			virtual void Soften() = 0;
			virtual void Harden() = 0;
			virtual void ResetPD() = 0;
			virtual double CalcTotalTorqueChange() = 0;
			virtual double CalcTorqueChangeInSection(int n) = 0;
			virtual void SetBestTorqueChange() = 0;
			virtual void ShowInfo() = 0;
			virtual void SetTargetCurrent() = 0;
			virtual void SetTargetFromLPF(int k, int n) = 0;
		};
		class HingeJoint : public Joint {
		public:
			PHIKHingeActuatorIf* hinge;
			PTM::VVector<double> torque;
			PTM::VVector<double> torqueLPF;
			PTM::VMatrixRow<double> angle;
			PTM::VMatrixRow<double> angleLPF;
			double targetAngle;
			double targetVel;
			PTM::VVector<double> viaAngles;
			PTM::VVector<double> viaVels;
			PTM::VVector<int> viatimes;
			double deltaViaAngle;
			double initialTorque;
			double initialAngle;
			double initialVel;
			int iterate;
			int movetime;
			double weight = 1.0;
			double rateLPF = 1.0;
			double originalSpring;
			double originalDamper;
			double hardenSpring = 1e10;
			double hardenDamper = 1e8;
			bool mul = true;
			double vel = 0;
			PTM::VVector<double> tChanges;
			bool viaCorrect;
			PTM::VMatrixRow<double> CorrTraj;
		public:
			HingeJoint(PHIKHingeActuatorIf* hinge);
			//~HingeJoint();
			void Initialize(int iterate, int mtime, int nVia, double rate = 1.0, bool vCorr = true);
			void SaveTorque(int n);
			void SaveTarget();
			void SetTarget(int k, int n);
			void SetOffsetFromLPF(int n);
			void ResetOffset();
			void SavePosition(int k, int n);
			void SavePositionFromLPF(int k, int n);
			void SaveViaPoint(int v, int t);
			void TrajectoryCorrection(int k, bool s);
			void TrajectoryCorrectionWithVia(int k, bool s);
			void ApplyLPF(int lpf, int count);
			void Soften();
			void Harden();
			void ResetPD();
			double CalcTotalTorqueChange();
			double CalcTorqueChangeInSection(int n);
			void SetBestTorqueChange();
			void ShowInfo();
			void SetTargetCurrent();
			void SetPD(double s, double d, bool mul);
			void SetTargetFromLPF(int k, int n);
			void UpdateIKParam(double b, double p);
		};
		class BallJoint : public Joint {
		public:
			PHIKBallActuatorIf* ball;
			PTM::VVector<Vec3d> torque;
			PTM::VVector<Vec3d> torqueLPF;
			PTM::VMatrixRow<Quaterniond> ori;
			PTM::VMatrixRow<Quaterniond> oriLPF;
			Quaterniond targetOri;
			Vec3d targetVel;
			Quaterniond deltaEndOri;
			PTM::VVector<Quaterniond> viaOris;
			PTM::VVector<Vec3d> viaVels;
			PTM::VVector<int> viatimes;
			Quaterniond deltaViaOri;
			int viatime;
			Vec3d initialTorque;
			Quaterniond initialOri;
			Vec3d initialVel;
			int iterate;
			int movetime;
			double weight = 1.0;
			double rateLPF = 1.0;
			double originalSpring;
			double originalDamper;
			double hardenSpring = 1e10;
			double hardenDamper = 1e8;
			bool mul = true;
			Vec3d vel = Vec3d();
			PTM::VVector<double> tChanges;
			bool viaCorrect;
		public:
			BallJoint(PHIKBallActuatorIf* ball);
			~BallJoint();
			void Initialize(int iterate, int mtime, int nVia, double rate = 1.0, bool vCorr = true);
			void SaveTorque(int n);
			void SaveTarget();
			void SetTarget(int k, int n);
			void SetOffsetFromLPF(int n);
			void ResetOffset();
			void SavePosition(int k, int n);
			void SaveViaPoint(int v, int t);
			void SavePositionFromLPF(int k, int n);
			void TrajectoryCorrection(int k, bool s);
			void TrajectoryCorrectionWithVia(int k, bool s);
			void SetBestTorqueChange();
			void ApplyLPF(int lpf, int count);
			void Soften();
			void Harden();
			void ResetPD();
			double CalcTotalTorqueChange();
			double CalcTorqueChangeInSection(int n);
			void ShowInfo();
			void SetTargetCurrent();
			void SetPD(double s, double d, bool mul);
			void SetTargetFromLPF(int k, int n);
			void UpdateIKParam(double b, double p);
		};
		class Joints {
			friend class Joint;
		public:
			std::vector<Joint*> joints;
			std::vector<BallJoint> balls;
			std::vector<HingeJoint> hinges;
		public:
			Joints();
			~Joints();
			void RemoveAll();
			void Add(PHIKActuatorIf* j);
			void initialize(int iterate, int movetime, int nVia, double rate = 1.0, bool vCorr = true);
			void SetTarget(int k, int n);
			void SaveTorque(int n);
			void SaveTarget();
			void SetOffsetFromLPF(int n);
			void ResetOffset();
			void SavePosition(int k, int n);
			void SaveViaPoint(int v, int t);
			void SavePositionFromLPF(int k, int n);
			void TrajectoryCorrection(int k, bool s);
			void TrajectoryCorrectionWithVia(int k, bool s);
			void SetBestTorqueChange();
			double GetBestTorqueChangeInSection(int n);
			void ApplyLPF(int lpf, int count);
			void Soften();
			void Harden();
			void ResetPD();
			double CalcTotalTorqueChange();
			double CalcTorqueChangeInSection(int n);
			void ShowInfo();
			void SetTargetCurrent();
			void SetWeight();
			void SetPD(double s, double d, bool mul);
			void SetTargetFromLPF(int k, int n);
			void UpdateIKParam(double b, double p);
		};

		struct LPF {
			//BiQuad LPF
			static PTM::VMatrixRow<double> BiQuad(PTM::VMatrixRow<double> input, double samplerate, double freq, double q);
			//N-Simple Moving Average LPF
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
		//----- 計算にかかわるもの -----
		//操作対象となるエンドエフェクタ
		PHIKEndEffectorIf* eef;
		//開始姿勢
		ControlPoint startPoint = ControlPoint();
		//目標姿勢
		ControlPoint targetPoint = ControlPoint();
		//途中通過点
		ControlPoint viaPoint = ControlPoint();
		std::vector<ControlPoint> viaPoints;
		//移動時間を秒で表したもの
		double mtime;
		//移動時間をStep数にしたもの(mtime要らない？)
		int movtime;
		//考慮する関節の深さ
		int depth;
		//考慮するActuator
		Joints joints;
		//繰り返し回数
		int iterate;
		//経由時間補正最大回数
		int iterateViaAdjust;
		//経由時間補正率
		double viaAdjustRate;

		//----- Sceneと保存用のStates -----
		PHSceneIf* scene;
		UTRef<ObjectStatesIf> states;
		UTRef<ObjectStatesIf> cstates;
		UTRef<ObjectStatesIf> initialstates;
		UTRefArray<ObjectStatesIf> tmpstates;

		//----- トルクから生成した軌道データ群 -----
		PTM::VMatrixRow<Posed> trajData;
		PTM::VMatrixRow<Posed> trajDataNotCorrected;
		PTM::VMatrixRow<Vec4d> trajVel;
		PTM::VMatrixRow<Vec4d> trajVelNotCorrected;
		PTM::VMatrixRow<Posed> ContinuousTrajData;

		//再生時にまだ移動中かどうか
		bool moving;
		//(replay時の)再生軌道index、通常は最後のもの
		int ite;
		//計算済みかのフラグ
		bool calced;
		//リプレイ時の適用ステップ
		int repCount;
		//極小値をとった軌道の番号
		int best;

		//----- ローパス関係 -----
		//BiQuadLPS用の遮断周波数とQ値
		double freq;
		double qvalue;
		//N点平均
		int nsma;
		double mag;
		//ウェイト
		PTM::VVector<double> weight;
		//ローパスの掛け具合
		double rate = 1.0;

		//----- 計算中のPD値に関する変数 -----
		//トルク->軌道生成時のspring&damper
		double spring = 1e10;
		double damper = 1e10;
		bool mul = true;

		//----- Unityからどこまで適用するかのフラグ -----
		//correctionを適用するかのフラグ
		int correction;
		//修正前の軌道をSaveするか
		bool saveNotCorrected;
		//到達まで待つ
		bool waitFlag;
		//修正前軌道を再生するかのフラグ
		bool noncorrectedReplay;
		//到達目標の姿勢を固定するかのフラグ
		bool staticTarget;
		//関節次元躍度最小軌道を初期軌道とするかのフラグ
		bool jointMJT;
		//ローパスウェイトを動的に変化させるかのフラグ
		bool dynamicalWeight;
		//
		bool viaCorrect;

		//トルク変化
		PTM::VVector<double> torquechange;
		double totalchange;
		double besttorque;

	public:
		SPR_OBJECTDEF(FWTrajectoryPlanner);
		SPR_DECLMEMBEROF_FWTrajectoryPlannerDesc;
		//コンストラクタ
		FWTrajectoryPlanner(const FWTrajectoryPlannerDesc& desc = FWTrajectoryPlannerDesc()) {
			SetDesc(&desc);
		}
		FWTrajectoryPlanner(int d, int i, int iv, double f, double q, int n, double mg, int c, bool wf, bool snc = false, double r = 1.0, double vRate = 0.65);

		//----- 計算用関数 -----
		//jointの深さのチェックと投げ込み
		void CheckAndSetJoints();
		//躍度最小軌道生成
		void MakeMinJerkAndSave();
		void MakeMinJerkAndSaveWithViaPoint();
		//Forward 
		void JointForward(int k, bool via = false);
		//Inverse
		void JointInverse(int k, bool via = false);
		//補正
		void JointTrajCorrection(int k);
		void JointTrajCorrectionWithViaPoint(int k);
		//デバッグ用情報表示
		void Debug();
		//デバッグ用アウトプットファイルの大本生成
		void PrepareOutputFilename(std::string& filename);
		//軌道データの出力
		void OutputTrajectory(std::string filename);
		void OutputNotCorrectedTrajectory(std::string filename);
		void OutputVelocity(std::string filename);
		//ロード
		void LoadScene();

		//実装中
		//Forward Inver Relaxation Model
		void FIRM(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt);
		//viatime adjustment
		bool ViatimeAdjustment();
		//viatime initialize
		void ViatimeInitialize();
		//MakeMinJerk複数点版
		void MakeMinJerkAll();
		//
		void Forward(int k);
		void Inverse(int k);
		void Correction(int k);
		void CalcTrajectory(ControlPoint tpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt);
		int TimeToStep(double t) {
			t *= scene->GetTimeStepInv();
			return ((t - (int)t) < 0.5) ? (int)t : (int)(t + 1);
		}

		//-----インタフェースの実装-----

		//初期化系
		void Reset(int d, int i, int iv, int n, double mg, int c, bool wf, bool snc = false, double r = 1.0, double vRate = 0.65, bool vCorr = true) {
			this->depth = d;
			this->iterate = i;
			this->iterateViaAdjust = iv;
			this->nsma = n;
			this->mag = mg;
			this->correction = c;
			this->rate = r;
			this->viaAdjustRate = vRate;
			this->waitFlag = wf;
			this->saveNotCorrected = snc;
			this->viaCorrect = vCorr;
		}

		void Init();

		void Init(int d, int i, int iv, int n, double mg, int c, bool wf, bool snc, double r = 1.0, double vRate = 0.65, bool vCorr = true);
		
		//エンドエフェクタ設定
		void SetControlTarget(PHIKEndEffectorIf* e);

		//シーン設定
		void SetScene(PHSceneIf* s);

		//指定点通過軌道計算
		void CalcTrajectoryWithViaPoint(ControlPoint tpoint, ControlPoint vpoint, int LPFmode, int smoothCount, std::string output, bool bChange, bool pChange, bool staticTarget, bool jmjt);

		//連続軌道計算
		void CalcContinuousTrajectory(int LPFmode, int smoothCount, std::string filename, bool bChange, bool pChange, bool staticTarget, bool jmjt);
		void AddControlPoint(ControlPoint c);

		//関節角度次元軌道計算
		void JointCalcTrajectory(Posed tPose, double mt, int LPFmode, int smoothCount, std::string output, bool bChange = false, bool pChange = false, bool staticTarget = false, bool jmjt = false);
		void JointRecalcFromIterateN(Posed tPose, double mt, int LPFmode, int smoothCount, std::string output, bool bChange = false, bool pChange = false, bool staticTarget = false, bool jmjt = false);

		//N回目の繰り返しから再計算
		void RecalcFromIterationN(int n);

		//生成された軌道を実際適用
		void JointTrajStep(bool step);

		//moving?
		bool Moving() { return moving; }
		//spring, damper set
		void SetPD(double s = 1e10, double d = 1e10, bool mul = true);
		//replay
		void Replay(int ite, bool noncorrected = false);
		//return totalChange
		double GetTotalChange();
		//return best
		int GetBest();
	};

}

#endif

#ifndef SPR_FWOPTIMIZER_H
#define SPR_FWOPTIMIZER_H

#include <Springhead.h>
#ifdef USE_CLOSED_SRC
#include "../../closed/include/cmaes/cmaes.h"
#endif

#include<chrono>

namespace Spr{;

/*
  物理シミュレーションを使った最適化計算の基本クラス
  FWAppのようにこのクラスを継承して独自の最適化アプリケーションを作成することを想定する  


*/

struct FWJointSpecInfo{
public:
	double k_1;
	double k_2;
	double k_3;
	double k_4;
	double min;
	double max;
	bool eulerDir = false;
	FWJointSpecInfo(){
		k_1 = 0.0;
		k_2 = 0.0;
		k_3 = 0.0;
		k_4 = 0.0;
		min = -M_PI;
		max = M_PI;
	}
	FWJointSpecInfo(double a, double b, double c, double d){
		k_1 = a;
		k_2 = b;
		k_3 = c;
		k_4 = d;
		min = -M_PI;
		max = M_PI;
	}
	FWJointSpecInfo(double a, double b, double c, double d, int mi, int ma){
		k_1 = a;
		k_2 = b;
		k_3 = c;
		k_4 = d;
		min = Rad((double)mi);
		max = Rad((double)ma);
	}
	FWJointSpecInfo(double a, double b, double c, double d, int mi, int ma, bool e){
		k_1 = a;
		k_2 = b;
		k_3 = c;
		k_4 = d;
		min = Rad((double)mi);
		max = Rad((double)ma);
		eulerDir = e;
	}
	double Add(double a){
		DSTR << k_1 << " " << k_2 << std::endl;
		return a + 10;
	}

	double Calc(double d){
		return exp(k_1 * (d - k_2)) - exp(k_3 * (k_4 - d));
	}
};

class FW3DJointSpecInfo{
public:
	FWJointSpecInfo specs[3];
	double weight;
	bool lr;
public:
	FW3DJointSpecInfo(){ specs[0] = FWJointSpecInfo(); specs[1] = FWJointSpecInfo(); specs[2] = FWJointSpecInfo(); weight = 0; lr = false; }
	FW3DJointSpecInfo(FWJointSpecInfo zs, double w){ specs[0] = FWJointSpecInfo(); specs[1] = FWJointSpecInfo(); specs[2] = zs, weight = w; lr = false; }
	FW3DJointSpecInfo(FWJointSpecInfo xs, FWJointSpecInfo ys, FWJointSpecInfo zs, double w){ specs[0] = xs; specs[1] = ys; specs[2] = zs, weight = w; lr = false; }
	/*FW3DJointSpecInfo(FWJointSpecInfo s[], double w, bool r){
		int l = sizeof(s[0]) == 0 ? 0 : sizeof(s) / sizeof(s[0]);
		switch (l){
		case 1:
			zspec = s[0];
			break;
		case 3:
			xspec = s[0];
			yspec = s[1];
			zspec = s[2];
			break;
		default:
			break;
		}
		weight = w;
		lr = l;
	}*/
};

struct HingeJointSpecInfo{
	FWJointSpecInfo spec;
	double weight;
	bool lr;
};

class FWConstraint{
protected:
	PHSolidIf* cSolid;
	Posed initialPose;
	double cWeight;
public:
	void SetSolid(PHSolidIf* solid) { this->cSolid = solid; }
	PHSolidIf* GetSolid() { return this->cSolid; }
	void SetInitialPose(Posed pose){ this->initialPose = pose; }
	Posed GetInitialPose(){ return this->initialPose; }
	void SetWeight(double w){ this->cWeight = w; }
	double GetWeight(){ return this->cWeight; }
	virtual double CalcEvalFunc(){ return 0; }
	void Init(){
		if (cSolid != NULL){
			this->initialPose = cSolid->GetPose();
		}
	}
};

class FWGroundConstraint{
private:
	Vec3d cNormal;
	PHSolidIf* cSolid;
	Posed initialPose;
	double cWeight;
public:
	FWGroundConstraint(){ this->cWeight = 1.0; this->cNormal = Vec3d(0, 0, 0); }
	FWGroundConstraint(double w, Vec3d n){ this->cWeight = w;  this->cNormal = n.unit(); }
	void SetSolid(PHSolidIf* solid) { this->cSolid = solid; }
	PHSolidIf* GetSolid() { return this->cSolid; }
	void SetInitialPose(Posed pose){ this->initialPose = pose; }
	Posed GetInitialPose(){ return this->initialPose; }
	void SetWeight(double w){ this->cWeight = w; }
	double GetWeight(){ return this->cWeight; }
	void SetNormal(Vec3d n){ this->cNormal = n.unit(); }
	double CalcEvalFunc(){ 
		Vec3d dPosition = cSolid->GetPose().Pos() - initialPose.Pos();
		Quaterniond dQuaternion = initialPose.Ori().Inv() * cSolid->GetPose().Ori();
		Vec3d n1 = dQuaternion * cNormal;
		double gq = 1.0 - n1 * cNormal;
		double gp = abs(dPosition * cNormal);
		DSTR << "rotationValue:" << gq << " positionValue:" << gp << std::endl;
		return cWeight * (gq + gp);
	}
	void Init(){
		if (cSolid != NULL){
			this->initialPose = cSolid->GetPose();
		}
	}
};

class FWUngroundedConstraint{
private:
	Vec3i cAxis;
	PHSolidIf* cSolid;
	Posed initialPose;
	double cWeight;
public:
	FWUngroundedConstraint(){ this->cWeight = 1.0; this->cAxis = Vec3i(0, 0, 0); }
	FWUngroundedConstraint(double w, Vec3i a){ this->cWeight = w; this->cAxis = a; }
	void SetSolid(PHSolidIf* solid) { this->cSolid = solid; }
	PHSolidIf* GetSolid() { return this->cSolid; }
	void SetInitialPose(Posed pose){ this->initialPose = pose; }
	Posed GetInitialPose(){ return this->initialPose; }
	void SetWeight(double w){ this->cWeight = w; }
	double GetWeight(){ return this->cWeight; }
	void SetAxis(Vec3i a){ this->cAxis = a; }
	double CalcEvalFunc(){
		Vec3d dPosition = cSolid->GetPose().Pos() - initialPose.Pos();
		return cWeight * abs(dPosition * cAxis);
	}
	void Init(){
		if (cSolid != NULL){
			this->initialPose = cSolid->GetPose();
		}
	}
};

struct JointPos {
	Quaterniond ori;
	double angle;
	JointPos() { ori = Quaterniond(); angle = 0; }
	JointPos(Quaterniond q){ ori = q; }
	JointPos(double a){ angle = a; }
};

double ccw(Vec3f p1, Vec3f p2, Vec3f p3);
/*
void GrahamConvexHull(std::vector<Vec3f>& vertices);
bool InsideGrahamConvexHull(std::vector<Vec3f>& vertices, Vec3f gp);
double EvalCenterOfGravity(std::vector<Vec3f>& vertices, Vec3f gp);
*/
class GrahamConvexHull : public UTRefCount{
	struct Sort{
		static float getPolar(Vec3f v1, Vec3f v2){
			Vec3f dv = v2 - v1;
			return dv.norm() == 0 ? 0 : -dv.x / dv.norm();
		}
		struct tang{
			double slope;
			bool same_x = false;
		};
		static tang getTang(Vec3f v1, Vec3f v2){
			tang t;
			if (v1.x == v2.x){
				t.slope = 0;
				t.same_x = true;
			}
			else{
				t.slope = (v1.z - v2.z) / (v1.x - v2.x);
			}
			return t;
		}
		static int part(std::vector<Vec3f> ver, int i, int j, int p){
			int l = i, r = j;

			while (l <= r){
				while (l <= j && ccw(ver[0], ver[l], ver[p]) < 0) l++;
				while (i <= r && ccw(ver[0], ver[r], ver[p]) > 0) r--;
				if (l >= r) break;
				Vec3f v = ver[l];
				ver[l] = ver[r];
				ver[r] = v;
				l++;
				r--;
			}
			return l;
		}
		static void quicksort(std::vector<Vec3f>& ver, int i, int j){
			//DSTR << i << " " << j << std::endl;
			if (i >= j) return; //DSTR << "side error" << std::endl;
			int p = (i + j) / 2;
			int k = part(ver, i, j, p);
			//DSTR << "p:" << p << " k:" << k << std::endl;
			int l = i, r = j;
			while (l <= r){
				while (l <= j && ccw(ver[0], ver[l], ver[p]) < 0) l++;
				while (i <= r && ccw(ver[0], ver[r], ver[p]) > 0) r--;
				if (l >= r) break;
				Vec3f v = ver[l];
				ver[l] = ver[r];
				ver[r] = v;
				//DSTR << "swap(" << l << "," << r << ")" << std::endl;
				l++;
				r--;
			}
			quicksort(ver, i, l - 1);
			quicksort(ver, r + 1, j);
		}
	};
public:
	std::vector<Vec3f> hull;
	Vec3f center;
	Vec3d normal;
public:
	GrahamConvexHull();
	//GrahamConvexHull(std::vector<Vec3f> vertices);
	~GrahamConvexHull();
	void Recalc(std::vector<Vec3f> vertices);
	bool InsideGrahamConvexHull(Vec3f v);
	double Eval(Vec3f v);
	Vec3f* GetVertices();
};

struct FWObjectiveValues{
	double errorvalue = 0;
	double torquevalue = 0;
	double stabilityvalue = 0;
	double groundvalue = 0;
	double ungroundedvalue = 0;
	double centervalue = 0;
	double initialorivalue = 0;
};

struct TorqueResult{
	Vec3d torque;
	Vec3d resist;
	TorqueResult(){ torque = Vec3d(); resist = Vec3d(); }
	TorqueResult(Vec3d t, Vec3d r){ torque = t; resist = r; }
};

class MinJerkTrajectory{
private:
	Posed sPose;
	Posed fPose;
	int stime;
	int ftime;
	//Vec6d coeff[3];
public:
	//コンストラクタ(多項式の係数を決める)
	//default
	MinJerkTrajectory();
	//開始位置と終端位置のみ
	MinJerkTrajectory(Posed spose, Posed fpose, int time);
	//開始と終端の位置、速度、加速度指定
	//MinJerkTrajectory(Posed spose, Posed fpose, Vec6d sVel, Vec6d fVel, Vec6d sAcc, Vec6d fAcc, int time);
	//開始と終点の位置、速度と通過点の位置、通過時間指定
	//MinJerkTrajectory(Posed spose, Posed fpose, Vec6d sVel, Vec6d fVel, Posed vPose, int vtime, int time);
	Posed GetCurrentPose(int t);
	Posed GetDeltaPose(int t);
	double GetCurrentActiveness(int t);
};

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
	double                  ixstart = 0.3, istddev = 0.3;

#ifdef USE_CLOSED_SRC
	Parameters<double>		*parameters;
	CMAES<double>			*evo;
	double                  iTolFun = 10, ilambda = 30, iMaxIter = 500;
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

	void SetESParameters(double xs, double st, double tf, double la, double mi);
};

class FWStaticTorqueOptimizer : public FWOptimizer {
	std::vector<JointPos> initialPos;
	Vec3d initialRootPos;

	double errorWeight, stabilityWeight, torqueWeight, resistWeight, constWeight, gravcenterWeight, differentialWeight;    //各評価値Weight

	std::vector<FWJointSpecInfo> resist;     //jointの各軸方向の抵抗特性係数(Ballは３要素、Hingeは１要素使う)、将来は改良します
	std::vector<FW3DJointSpecInfo> jointspecs;

	std::vector<JointPos> unityInitPos;      //Unityの初期姿勢(各関節のEuler角)

	std::vector<double> jointWeights;        //関節ごとのWeight、将来は関節情報としてresist共々まとめるつもり

	bool specificFlag = false;             //特定の関節のみにトルク計算を適用するフラグ
	std::vector<int> specificJoints;        //上フラグ有効時の適用関節番号

	std::vector<FWGroundConstraint> groundConst;               //接地拘束集合、とりあえず書きやすかったのでvector
	std::vector<FWUngroundedConstraint> ungroundedConst;       //非接地拘束

	//int* bodyIndex;
	std::vector<int> bodyIndex;            //身体構成剛体のindex

	//データ送信用
	FWObjectiveValues val;                 //評価値内訳保存構造体
	TorqueResult* tor;                     //トルク計算結果保存構造体集合
	Vec3d cog;                             //重心座標
	std::vector<Vec3f> supportPolygon;     //支持多角形形成頂点集合

public:
	virtual void Init();

	virtual void Iterate() { FWOptimizer::Iterate(); }

	virtual void ApplyResult(PHSceneIf* phScene) { FWOptimizer::ApplyPop(phScene, GetResults(), NResults()); }

	virtual double ApplyPop(PHSceneIf* phScene, double const *x, int n);

	virtual double Objective(double const *x, int n);

	void SetScene(PHSceneIf* phSceneInput) { phScene = phSceneInput; }

	void Optimize() { FWOptimizer::Optimize(); }

	bool TestForTermination() {
#ifdef USE_CLOSED_SRC
		return evo->testForTermination();
#else
		return true;
#endif
	}

	void TakeFinalValue() {
#ifdef USE_CLOSED_SRC
		xfinal = evo->getNew(CMAES<double>::XMean);
		Objective(xfinal, (int)evo->get(CMAES<double>::Dimension));
#endif
	}

	Vec3d CalcTorqueN(int n);

	void SetErrorWeight(double v) { errorWeight = v; }
	double GetErrorWeight() { return errorWeight; }

	void SetStabilityWeight(double v) { stabilityWeight = v; }
	double GetStabilityWeight() { return stabilityWeight; }

	void SetTorqueWeight(double v) { torqueWeight = v; }
	double GetTorqueWeight() { return torqueWeight; }

	void SetResistWeight(double v){ resistWeight = v; }
	double GetResistWeight(){ return resistWeight; }

	void SetConstWeight(double v){ constWeight = v; }
	double GetConstWeight(){ return constWeight; }

	void SetGravcenterWeight(double v){ gravcenterWeight = v; }
	double GetGravcenterWeight(){ return gravcenterWeight; }

	void SetDifferentialWeight(double v){ differentialWeight = v; }
	double GetDifferentialWeight(){ return differentialWeight; }
	
	void SetResistanceTorqueN(FWJointSpecInfo h){
		resist.push_back(h);
	}

	void SetResistanceTorque(FWJointSpecInfo h[]){
		int l = sizeof(h) / sizeof(h[0]);
		for (int i = 0; i < l; i++){
			resist.push_back(h[i]);
		}
	}
	
	void SetResistanceTorque(double* h){
		int l = sizeof(h) / (4 * sizeof(h[0]));
		int k = 0;
		for (int i = 0; i < l; i++){
			resist[i].k_1 = h[4*i];
			resist[i].k_2 = h[4 * i + 1];
			resist[i].k_3 = h[4 * i + 2];
			resist[i].k_4 = h[4 * i + 3];
		}
	}

	void AddJointSpecs(FW3DJointSpecInfo j){
		jointspecs.push_back(j);
	}

	void SetUnityInitPos(JointPos j){
		unityInitPos.push_back(j);
	}

	void SetSpecificFlag(bool f){
		this->specificFlag = f;
	}

	void AddSpecificJoints(int n){
		specificJoints.push_back(n);
	}

	void SetSpecificJoints(int n[]){
		int l = sizeof(n) / sizeof(n[0]);
		for (int i = 0; i < l; i++){
			specificJoints.push_back(n[i]);
		}
	}

	/*
	void AddPositionConst(FWConstraint f){
		positionConst.push_back(f);
	}
	*/

	//構造体の配列を外部から取れないので１要素ずつpush
	void AddPositionConst(FWGroundConstraint f){
		groundConst.push_back(f);
	}
	void AddPositionConst(FWUngroundedConstraint f){
		ungroundedConst.push_back(f);
	}

	void SetBodyIndex(int s[]){
		int l = sizeof(s) / sizeof(s[0]);
		for (int i = 0; i < l; i++){
			bodyIndex.push_back(s[i]);
		}
	}
	
	void AddBodyIndex(int s){
	    bodyIndex.push_back(s);
	}

	/*void SetBodyIndex(int s[]){
		int l = sizeof(s) / sizeof(s[0]);
		bodyIndex = new int[l];
		DSTR << l << " " << sizeof(s) << " " << sizeof(s[0]) << std::endl;
		for (int i = 0; i < l; i++){
			bodyIndex[i] = s[i];
		}
	}

	void SetBodyIndex(int s[], int l){
		bodyIndex = new int[l];
		DSTR << l << " " << sizeof(s) << " " << sizeof(s[0]) << std::endl;
		for (int i = 0; i < l; i++){
			bodyIndex[i] = s[i];
		}
	}*/

	void SetESParameters(double xs, double st, double tf, double la, double mi){ FWOptimizer::SetESParameters(xs, st, tf, la, mi); }

	void AddJointWeights(double w){
		jointWeights.push_back(w);
	}

	FWObjectiveValues GetObjectiveValues(){
		return this->val;
	}
	
	int NTorque(){
		//return sizeof(tor[0]) == 0 ? 0 : sizeof(tor) / sizeof(tor[0]);   //newで領域確保したのでこれではsizeof(tor)が機能しない
		return phScene->NIKActuators();
	}

	TorqueResult GetTorqueN(int n){
		return tor[n];
	}
	
	Vec3f GetCenterOfGravity(){
		return cog;
	}

	int NSupportPolygonVertices(){
		return  supportPolygon.size();
	}

	Vec3f GetSupportPolygonVerticesN(int n){
		return supportPolygon[n];
	}
};

class TrajectoryPlanner : public UTRefCount{
	struct LPF{
		//BiQuad LPF
		//荒ぶる
		static PTM::VMatrixRow<double> BiQuad(PTM::VMatrixRow<double> input, double samplerate, double freq, double q){
			PTM::VMatrixRow<double> output;
			//DSTR << input.height() << " " << input.width() << std::endl;
			output.resize(input.height(), input.width());
			output.clear();

			double omega = 2.0 * M_PI * freq / samplerate;
			double alpha = sin(omega) / (2.0 * q);

			double a0 = 1.0 + alpha;
			double a1 = -2.0 * cos(omega);
			double a2 = 1.0 - alpha;
			double b0 = (1.0 - cos(omega)) / 2.0;
			double b1 = 1.0 - cos(omega);
			double b2 = (1.0 - cos(omega)) / 2.0;

			for (int i = 0; i < input.height(); i++){
				output[i][0] = b0 / a0 * input[i][0];
				output[i][1] = b0 / a0 * input[i][1] + b1 / a0 * input[i][0] - a1 / a0 * output[i][0];
				for (int j = 0; j < input.width(); j++){
					output[i][j] = b0 / a0 * input[i][j] + b1 / a0 * input[i][j - 1] + b2 / a0 * input[i][j - 2]
						- a1 / a0 * output[i][j - 1] - a2 / a0 * output[i][j - 2];
				}
			}
			return output;
		}
		//N-Simple Moving Average LPF 
		//グラフ見た限りどこかうまくいってないっぽい
		//位相ずれが起きている？
		static PTM::VMatrixRow<double> NSMA(PTM::VMatrixRow<double> input, int n, double mag, PTM::VVector<double> s){
			PTM::VMatrixRow<double> output;
			//DSTR << input.height() << " " << input.width() << std::endl;
			output.resize(input.height(), input.width());
			output.clear();
			DSTR << n << std::endl;

			if (input.width() < n){
				return input;
			}
			else{
				for (int i = 0; i < input.height(); i++){
					int j = 0;
					double sum = s[i] * n;
					DSTR << s[i] << std::endl;
					for (; j < n; j++){
						sum = sum + input[i][j] - s[i];
						output[i][j] = sum / n;
					}
					for (; j < input.width(); j++){
						sum = sum + input[i][j] - input[i][j - n];   //このやり方だと累積誤差出ます
						/* 累積誤差チェック用
						double check_sum = 0;
						for (int k = 0; k < n; k++){
							check_sum += input[i][j - k];
						}
						DSTR << sum - check_sum << std::endl;
						*/
						output[i][j] = mag * sum / n;
					}
				}
			}
			return output;
		}
	};
private:
	//操作対象となるエンドエフェクタ
	PHIKEndEffectorIf* eef;
	//開始姿勢
	Posed startPose;
	//目標姿勢
	Posed targetPose;
	//移動時間
	double mtime;
	//移動時間をStep数にしたもの(mtime要らない？)
	int movtime;
	//考慮する関節の深さ
	int depth;
	//関節モータのDOF合計
	int motorDOF;
	//繰り返し回数
	int iterate;
	//計算開始時のStep数
	int startStep;
	//Sceneと保存用のStates
	PHSceneIf* scene;
	UTRef<ObjectStatesIf> states;

	//sTorque, torqueはCalcTrajectory時にしか多分使わないのでそちらに移動すべき
	//軌道から得たトルクデータ
	PTM::VVector<double> sTorque;
	PTM::VMatrixRow<double> torque;

	//関節角も考慮する場合のローパス後の各関節角
	struct TrajJoint{
		PTM::VMatrixRow<JointPos> data;
		TrajJoint(){ data.clear(); }
		TrajJoint(int depth, int time){ 
			data.resize(depth, time); 
			for (int i = 0; i < time; i++){
				for (int j = 0; j < depth; j++){
					data[j][i] = JointPos();
				}
			}
		}
	};
	std::vector<TrajJoint> trajJointData;
	//std::vector<std::vector<Posed>>とstd::vector<PTM::VMatrixRow<double>>がsyntax error in input(3)
	//トルクから生成した軌道データ
	PTM::VMatrixRow<Posed> trajData;
	//動いているか
	bool moving;
	//(replay時の)再生軌道index、通常は最後のもの
	int ite;
	bool calced;
	//BiQuadLPS用の遮断周波数とQ値
	double freq;
	double qvalue;
	//N点平均
	int nsma;
	double mag;
	//トルク->軌道生成時のspring&damper
	double spring;
	double damper;

	//以下Unityからどこまで適用するかのフラグ
	//correctionを適用するかのフラグ
	bool corFlag;
	//トルク->軌跡のときにIKによる制御を入れるかのフラグ
	bool IKFlag;
	//関節のpullbackを有効にするか
	bool jointCorFlag;
	//到達まで待つ
	bool waitFlag;
	//トルク変化
	PTM::VVector<double> torquechange;
	double totalchange;
	int best;
	double besttorque;

	//PTM::VMatrixRow<double> torquechangeAll;
	//PTM::VVector<double> totalchangeAll;
	
	bool ignoreMotors;
public:
	//コンストラクタ
	TrajectoryPlanner();
	TrajectoryPlanner(int d, int i, double f, double q, int n, double mg, bool c, bool ik, bool jc, bool im, bool wf);
	//初期化系
	void Init();
	void Init(int d, int i, double f, double q, int n, double mg, bool c, bool ik, bool jc, bool im, bool wf);
	//エンドエフェクタ設定
	void SetControlTarget(PHIKEndEffectorIf* e) { this->eef = e; };
	//シーン設定
	void SetScene(PHSceneIf* s){ this->scene = s; };
	//軌道計算
	void CalcTrajectory(Posed tPose, double mt, int LPFmode);
	//生成された軌道を実際適用
	void TrajStep(bool step);
	//補正
	void TrajCorrection(int k);
	//
	bool Moving(){ return moving; };
	//デバッグ用情報表示
	void Debug();
	//ロード
	void LoadScene();
	//spring, damper set
	void SetSD(double s, double d){ this->spring = s; this->damper = d; };
	//replay
	void Replay(int ite);
	//return totalChange
	double GetTotalChange(){ return totalchange; }
	//return best
	int GetBest(){ return best; }
};

}

#endif //SPR_FWOPTIMIZER_H


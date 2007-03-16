/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/HIS/HISpidarG6/main.cpp
【概要】
  グラフィックスレンダラークラスの DrawScene APIを使い、シーンを一括でレンダリングする。
  DrawScene API で設定されているマテリアルサンプルを用いて、カラフルな球体および直方体をレンダリングする。　
  また、SPIDARを操作して球体、直方体及び地面と力覚インタラクションを行う。
  
【終了基準】
  ・5000ステップ後に強制終了。 
 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <GL/glut.h>
#include <iostream>
#include <time.h>
#include <stdio.h>

// debug出力用
#include <fstream>
#include "Foundation/UTPreciseTimer.h"

#include "HIS/HIS.h"
#include "Physics/PHConstraintEngine.h"
#include "Physics/PHForceField.h"
#include "Physics/PHConstraint.h"
#include "Physics/PHContactPoint.h"
#include <Physics/PHSpatial.h>

#include "Collision/CDDetectorImp.h"
#include "Collision/CDConvex.h"

#include <mmsystem.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

// 実験にあわせて設定を行うフラグ
// 次のうちから一つ選ぶことで実験用の設定にできる
//#define EXP1
//#define EXP2
//#define EXP3
//#define EXP4
//#define Demo

#define ESC				27			// ESC key
//#define EXIT_TIMER		10000		// 実行ステップ数
#if !defined Demo
//	#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
//	#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define WINSIZE_WIDTH 960
#define WINSIZE_HEIGHT 720
#else
	#define WINSIZE_WIDTH   1280
	#define WINSIZE_HEIGHT  960
#endif

#if defined EXP1
	#define NUM_OBJECTS		2			// object数
#elif defined EXP2
	#define NUM_OBJECTS		2
#elif defined EXP3
	#define NUM_OBJECTS		1
#elif defined EXP4
	#define NUM_OBJECTS		5
#elif defined Demo
	#define NUM_OBJECTS		2
#else
	#define NUM_OBJECTS		2
#endif

#define SPIDAR_SCALE	2			// SPIDARのVE内での動作スケール
#define MASS_SCALE		1000			// 剛体の質量のスケール
									// この二つのスケールは仮想世界と現実世界のスケールをあわせるのに使われる

#define POINTER_RADIUS 0.1f			// ポインタの半径
#define EPSILON 2.0					// ポインタに接触しそうな剛体を予測するためにポインタのBBoxを膨らませて接触判定をするときの膨らませる倍率
									// 式としてはd = (EPSILON - 1) x POINTER_RADIUSだけ先の剛体を検索して接触候補点を作る
									// 大きくするほどたくさんの接触を予想できるが、その分量も増えるので計算が重くなる
									// 1にすると予測なし

#define LIMIT_DEPTH 100				// 予測シミュレーションを行う剛体取得の深さ上限
#define NUM_PREDICT_ITERATE 15		// 予測シミュレーションのイテレート回数

#define NUM_COLLISIONS 20			// ポインタへの許容接触数
#define NUM_COL_SOLIDS 20			// ポインタへの許容接触剛体数　
									// NUM_COLLISIONSと区別するのはプログラムを読みやすくするため。実質的な存在意義はない

#define COLLISION_DENSITY 0.4		// 力覚の計算に使用する接触点の分布量を調節する値
									// 接触を前回から引き継いだ場合接触点はこの値より大きい距離で分布する

#define DISTANCE_LIMIT    0.3		// 接触点が離れたときに接触を解除するかを決定する距離
									// これより距離が大きくなった場合はその接触はなくなったものとみなす
									// 高速回転による誤判定を防ぐ機能

#define PCORRECTIONRATE    0.5		// ポインタに直に接している剛体の周りで発生した接触を予測シミュレーションするときに
									// 設定するめり込み補正パラメータ
									// これを大きくするとめり込みはなくなるが不安定（発振）が起こり
									// 小さくするとめり込みが発生するが安定になるので
									// ポインタに直に接する剛体に関してはこの係数を下げることにする

#ifdef _DEBUG
	int SIMULATION_FREQ	= 60;		// シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		500		// 力覚スレッドの周期Hz
	float Km = 20;//100;					// virtual couplingの係数
	float Bm = 0;					// 並進

	float Kr = 1;					// 回転
	float Br = 0;

#elif _WINDOWS
	int SIMULATION_FREQ = 60;		// シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000	// 力覚スレッドの周期Hz
	float Km = 20;//100;					// virtual couplingの係数
	float Bm = 0;					// 並進

	float Kr = 1;					// 回転
	float Br = 0;
#endif

// シミュレーション周波数の切り替えに使うバッファ
int SIMULATION_FREQ_BUFFER = SIMULATION_FREQ;

// 提示力と剛体に提示する力を直接変化させる定数
double FORCE_COEFF =		1.0;

#ifdef _WIN32		//	Win32版(普通はこっち)
	#include <Device/DRUsb20Simple.h>
	#include <Device/DRUsb20Sh4.h>
	#include <conio.h>
#endif


typedef PTM::TMatrixCol<NUM_COLLISIONS, 6, double> MatrixN6d;
typedef PTM::TMatrixCol<6, NUM_COLLISIONS, double> Matrix6Nd;
typedef PTM::TVector<NUM_COLLISIONS, double> VecNd;

// グラフィック用の変数
UTRef<GRSdkIf> grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

// 物理計算用の変数
UTRef<PHSdkIf> phSdk;
UTRef<PHSceneIf> scene;

// 仮想世界で扱う剛体
vector<PHSolidIf*> soWalls;
vector<PHSolidIf*> soObject; 
PHSolidIf* soPointer;

// SPIDAR用の変数
DVDeviceManager devMan;						//	D/Aやカウンタなどのデバイスの管理
HISpidarG6X3 spidarG6;						//	SPIDARに対応するクラス

// その他の変数
Vec3f spidar_pos = Vec3f();
Quaterniond spidar_ori = Quaterniond();
Matrix3f view_rot;
Matrix3f view_haptic;


// multimedia timerのハンドラ
MMRESULT FTimerId1;


// 提示力を出すかどうか
bool bForce = false;

// 局所的動力学計算
bool bLocalDynamics = true;

// debug出力用fstream
ofstream ofs;
bool bOutput = false;

// シミュレーションをするかどうかをあらわすフラグ
// mmtimerで動くHapticRenderingが時間をカウントして
// 適切な時期にONにして
// idle functionがシミュレーションを行う
bool bSimulation = true;

// 周囲の影響を有効にするかどうかを決めるフラグ
bool bSurroundEffect = true;

// SPIDARの位置を表示するかどうか
bool bDisplayPointer = false;

// 接触点を描画するかどうか
bool bDisplayCollision = false;

// 接触予想を使うかどうか
bool bPredictCollision = false;

// KとBの値どちらの変更を有効にするか
// trueの場合並進
// falseの場合回転
bool bMode = true;

// 接触候補点の情報を格納する構造体
typedef struct{
	int pointer_index;
	PHSolid* solid[2];
	Vec3d closestPoint[2];
	Vec3d normal;
	Spr::PHShapePairForLCP* shapePair;
} CandidateInfo;

// 接触予想の時に使う構造体
struct Edge{
		float edge;				///<	端の位置(グローバル系)
		int	index;				///<	元の solidの位置
		bool bMin;				///<	初端ならtrue
		bool operator < (const Edge& s) const { return edge < s.edge; }
	};
typedef std::vector<Edge> Edges;

// その剛体に関する周囲の影響を格納する構造体
typedef struct{
	SpatialVector constant_effect;
	vector<pair<Matrix3d, Matrix3d> > surround_effect;
} SEData;

// 力覚計算に必要なデータを集めた構造体
// 力覚計算を高速で行えるように
// シミュレーションでデータを格納し、
// HapticRenderingに渡す
typedef struct {
	// collision data
	PHConstraint* constraint[NUM_COLLISIONS];
	
	// 新たな接触を作るために必要なshapePair
	Spr::PHShapePairForLCP* shapePair[NUM_COLLISIONS];

	// 前回から引き継がれた接触かどうか判定するフラグを格納する配列
	bool bPreviousCollide[NUM_COLLISIONS];

	int sign[NUM_COLLISIONS];

	// 自分で作成した接触点の集合を格納する変数
	multimap<double, PHContactPoint *> points;

	// 接触点から、その元になったデータへアクセスするための配列の添え字を格納する変数
	int NewcolToCol[NUM_COLLISIONS];
	int NewsolToSol[NUM_COL_SOLIDS];

	// constraint->solidと同じ
	// constraintがintegrateで消えてしまうので
	// solidだけでほぞんしておいて、
	// 同じ接触を作るときに使うようにする
	PHSolid* solid[NUM_COLLISIONS][2];

	// collision solid data
	PHSolid* nearest_solids[NUM_COL_SOLIDS];

	// map from physical data to rigid body itself
	int ColToSol[NUM_COLLISIONS];
	int SolToCol[NUM_COL_SOLIDS][NUM_COLLISIONS];
	int num_col_per_sol[NUM_COL_SOLIDS];
	int num_solids;

	// physical data
	Vec3d solid_velocity[NUM_COL_SOLIDS];
	Vec3d solid_angular_velocity[NUM_COL_SOLIDS];

	Vec3d solid_center_positions[NUM_COL_SOLIDS];
	Quaterniond solid_orientations[NUM_COL_SOLIDS];

	double solid_massinvs[NUM_COL_SOLIDS];
	Matrix3d solid_inertiainvs[NUM_COL_SOLIDS];

	// collision data
	int num_collisions;
	Vec3d col_positions[NUM_COLLISIONS];
	Vec3d col_normals[NUM_COLLISIONS];
	Vec3d pointer_col_positions[NUM_COLLISIONS];

	// 周囲の影響の結果を格納する変数
	Matrix3d vel_effect[NUM_COL_SOLIDS][NUM_COLLISIONS];
	Vec3d vel_constant[NUM_COLLISIONS];

	Matrix3d ang_effect[NUM_COL_SOLIDS][NUM_COLLISIONS];
	Vec3d ang_constant[NUM_COLLISIONS];

	SpatialVector gravity_effect[NUM_COL_SOLIDS];

	// 接触によって他の接する剛体がどのように動くのかを格納する変数
	// 各軸ごと・定数を保存
	// ErrorCorrectionで使う
	vector<SEData> relative_solids_velocity;
	set<PHSolid *> relative_solids;

	// 加えられた力の総和を格納する変数
	Vec3d ForceSum[NUM_COLLISIONS];

	// 重力が適応されているかどうか
	bool bGravity[NUM_COL_SOLIDS];

	// 実際に接触しているかあらわすフラグ
	// MakeHapticInfoで前回接触していた場合、の判断に使う
	bool bCollide[NUM_COLLISIONS];

	// ワールド座標系から剛体座標系に直す４元数
	// PHConstraint::Xj.Conjugated()と同じ
//	Quaterniond wldTosld[NUM_COLLISIONS];

	// 接触に力を加えるときの侵入量に比例した重み
	double weight[NUM_COLLISIONS];

	Vec3d pointer_pos[NUM_COLLISIONS];
	Quaterniond pointer_ori[NUM_COLLISIONS];

	// 更新時間幅周り
	double HSratio;
	double SHratio;
	double Hdt;
	double Sdt;
} HapticInfo;

// pointer data
Vec3d pointer_pos;
Quaterniond pointer_ori;
Vec3d pointer_vel;
Vec3d pointer_angvel;

// 周囲の影響を計算するときに使う力
// 周囲の影響を計算するときは
// その時点でHapticRenderingで加えた最後の力を使って求める
Vec3d LastVCForce = Vec3d();
Vec3d TestForce = Vec3d(1, 1, 1);

// １と２を用意するのはスレッドで必要な排他アクセスを避け（待ちが発生するため）、
// 各プロセスの更新速度を保つため
HapticInfo info1;
HapticInfo info2;

// 重力が適用されているかどうかを格納するマップ
// 本当はPHSolidにbGravityがあればいいが
// とりあえずsetでやる
// 登録されていれば,それは重力を適用しない剛体
set<PHSolid*> bGravitySet;

// trueのときはinfo１を参照
// falseのときはinfo２を参照
bool current_valid_data = true; 

#if defined EXP1
	// 出力用バッファ
	vector<Vec3d> Buffer;
	int num_data;
	bool bExperiment = false;
	Vec3d pos_array[] = {Vec3d(0.0, 0.0, 0.0), Vec3d(7.0, 0.0, 0.0)};
#elif defined EXP2
	double mass;
	int session;
	bool bTest = false;
#endif

// 関数のプロトタイプ宣言
//////////////////////////////////////////////////////////////////////////////
// 物理情報を作成する処理
void CalcSurroundEffect(HapticInfo*, HapticInfo*);

// 影響のある剛体の集合を取得してくる処理
void GetSolidsCollisionsOnPointer(vector<pair<PHConstraint *, int> >*, multimap<double, PHContactPoint*> ,
						  set<PHConstraint *>*, set<PHSolid *>*);
void GetAllRelativeSolidsCollisions(set<PHConstraint*>*, set<PHSolid*>*, set<PHSolid*>, set<PHConstraint*>*);
void RecursiveSolidsCollisionsRetrieval(multimap<double, pair<PHConstraint *, bool> >*, PHSolid*, 
										set<PHConstraint *>*, set<PHSolid *>*, set<PHSolid*>, set<PHConstraint*>*, int);

// 接触候補点を作成する処理
void CreateCandidateContactPoints(vector<pair<PHConstraint*, int> >, 
								  vector<pair<PHConstraint*, int> >*, 
								  set<PHConstraint *>*, set<PHSolid*>*,
								  map<PHContactPoint*, Vec3d>*, HapticInfo*);
void GetCandidateSolidsByBBox(set<int> *, int *);
void GetWillCollidePoints(int, int, vector<CandidateInfo>*);
void CreateShapePairMMap(vector<pair<PHConstraint *, int> >, multimap<PHSolid*, Spr::PHShapePairForLCP*>*);

// 予測シミュレーションをする処理
void PredictSimulations(vector<pair<PHConstraint*, int> >, vector<pair<PHConstraint *, int> >, vector<pair<PHConstraint *, int> >,
						set<PHSolid*>, set<PHConstraint*>, 
						set<PHSolid*>, set<PHConstraint*>,
						map<PHSolid*, SEData>*,
						vector<SEData>*);
void SetupPredictSimulation(set<PHConstraint*>, set<PHSolid*>, set<PHSolid*>, set<PHConstraint *>, map<PHSolid*, Vec3d>*);
void AddForceToConstraint(int, PHConstraint*, PHSolid*, map<PHSolid *, Vec3d>, Vec3d);
vector<SpatialVector> PredictSimulation(vector<SpatialVector>*,
										set<PHSolid*>, set<PHConstraint *>,
										set<PHConstraint *>, set<PHSolid*>);
vector<SpatialVector> PredictSimulationToCollision(vector<SpatialVector>*,
										PHConstraint*, PHSolid*,
										set<PHConstraint *>, set<PHSolid *>,
										set<PHConstraint *>, set<PHSolid *>,
										Vec3d, map<PHSolid *, Vec3d>, int index = -1);
// 接触点を引き継ぐ処理
void CreateConstraintFromCurrentInfo(HapticInfo*, vector<pair<PHConstraint *, int> >*, map<PHConstraint*, int>*,
									 set<PHSolid*>*, set<PHConstraint *>*, bool);

// 得られた物理情報をすべてまとめる処理
void MakeHapticInfo(HapticInfo*, HapticInfo*, vector<pair<PHConstraint*, int> >, 
					vector<pair<PHConstraint*, int> >, map<PHContactPoint*, Vec3d>,
					vector<pair<PHConstraint*, int> >, 
					map<PHConstraint*, int>, map<PHSolid*, SEData>,
					vector<SEData>, set<PHSolid *>, bool);
PHConstraint* GetSpecificDataFromCollisions(int i, 
					 vector<pair<PHConstraint*, int> >, vector<pair<PHConstraint*, int> >, 
					 map<PHContactPoint*, Vec3d>, vector<pair<PHConstraint*, int> >,  
					 int*, PHSolid**, Vec3d*, bool*, bool*);
void RegisterNewSolid(HapticInfo*, int, HapticInfo*, PHSolid*, map<PHSolid*, SEData>);
void RegisterNewCollision(HapticInfo*, int i, HapticInfo*, PHConstraint*, int, bool, bool, map<PHConstraint*, int>, Vec3d, bool);


////////////////////////////////////////////////////////////////////////////////////
// 力覚計算での処理
#ifdef EXP1
void HapticRendering();
#else 
void CALLBACK HapticRendering(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
#endif
// データを切り替える時に行う処理
void UpdateNewInfoFromCurrent();

// めり込みとSPIDARの位置からポインタの位置を計算する処理
void CheckPenetrations(HapticInfo*, int*, vector<Vec3d>*, vector<double>*, vector<Vec3d>*);
void CalcTTheta(HapticInfo*, Vec3d*, Vec3d*);
void CorrectPenetration(HapticInfo*);
inline void GetParametersOfCollision(double*, double*, double*, double*, double*, double*, double*);
void MakePointerPosMatrix(int, vector<Vec3d>t, vector<double>, vector<Vec3d>, MatrixN6d*, VecNd*);

// 局所的な動力学計算を行う処理
void UpdateVelocityByCollision(HapticInfo*, Vec3d, bool*);
inline void UpdateSolidByCollision(HapticInfo *, Vec3d, bool*);
void UpdateSolids(HapticInfo*);

// 純粋なバーチャルカップリング
void VirtualCoupling(HapticInfo*, Vec3d*, Vec3d*, bool*);

// バネダンパモデルから力を計算する関数
inline void calcForceBySpringDumper(Vec3d*, Vec3d*);

// 接触点に加える力の重みを計算する処理
void CalcForceWeight(HapticInfo*);

// 計画的に力覚計算と物理シミュレーションを行う処理
inline void scheduleSimulationFromHaptic();

////////////////////////////////////////////////////////////////////////////////////
// 中立的な処理
void ErrorCorrection();
void UpdatePointer();
void StepSimulation();


////////////////////////////////////////////////////////////////////////////////////
// 本質的でない処理
void idle();
void display();
void setLight();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void InitScene();
void InitRendering(int*, char**);
void InitPointer(PHSolidIf*);
void SaveCollisionF(set<PHConstraint *>, set<PHConstraint *>);
void SaveSolidDv(set<PHSolid*>, set<PHSolid*>);
void SaveSolidV(set<PHSolid*>);
void SaveSolidF(set<PHSolid*>);
void RestoreParameters(set<PHConstraint*>*, set<PHSolid*>*, set<PHConstraint*>*, set<PHSolid*>*);


////////////////////////////////////////////////////////////////////////////////////
// ユーティリティ
void ShowCollisionInfo();
inline void t_start();
inline int t_end();
void CalcPinv(MatrixN6d, Matrix6Nd*);
void UpdateSpidarData(Vec3d*, Quaterniond*);
void DisplayPointer();
void DisplayCollisions();
inline PHSolid* GetAdjacentSolid(PHConstraint*, PHSolid*, int* sign = NULL);
PHContactPoint* CreateContactPoint(Vec3d, Vec3d, PHSolid*, PHSolid*, Spr::PHShapePairForLCP*);
void ResetOriginalContactPoints(multimap<double, PHContactPoint*>*);
void AddInactiveSolid(PHSolidIf*);
void InitDeviceManager();
void InitHapticInfo(HapticInfo*);
void updatePenetrations(HapticInfo* info);
inline double calcDistance(Vec3d, Vec3d);
inline void updateSimulationFrequency();
inline void calcTimeSpan(HapticInfo* info);

///////////////////////////////////////////////////////////////////////////////////////////////
// 周囲の影響を計算するための処理
inline void SaveParameters(set<PHConstraint*> relative_consts, set<PHSolid*> relative_solids, set<PHConstraint *> nearest_consts, set<PHSolid *> nearest_solids)
{
	SaveCollisionF(relative_consts, nearest_consts);
	SaveSolidDv(relative_solids, nearest_solids);
//	SaveSolidV(relative_solids);
//	SaveSolidF(relative_solids);
}

// 周囲の影響を計算し、引数のHapticInfoに格納する関数
void CalcSurroundEffect(HapticInfo* new_info, HapticInfo* current_info)
{
	// ポインタに関する接触を格納する変数
	vector<pair<PHConstraint *, int> > pointer_consts;

	// 接触予測用のデータを格納する変数
	vector<pair<PHConstraint *, int> > col_candidate_consts;
	map<PHContactPoint*, Vec3d> col_candidate_pointer_pos;

	// 前回の接触を引き継ぐためのデータを格納する変数
	vector<pair<PHConstraint *, int> > current_consts;

	// 新しく作った接触と現在の接触を関連付けるマップ
	map<PHConstraint *, int> NewcolToCol;

	// ポインタと直に関係のある剛体・接触
	set<PHSolid *> nearest_solids;
	set<PHConstraint *> nearest_consts;

	// 直接ではないが関係のある剛体,接触を格納する変数
	set<PHSolid *> relative_solids;
	set<PHConstraint *> relative_consts;

	// 直接と間接でポインタに関係する周囲の影響を格納する変数
	map<PHSolid *, SEData> se_map;
	vector<SEData> relSE;

	// データ製作の途中でフラグが変更されると困るので
	// この時点でのフラグの値を保存しておいて
	// この関数内ではそれを信じることにする
	bool bLocalDynamics_local = bLocalDynamics;
	bool bPredictCollision_local = bPredictCollision;

	// 自作の接触を削除する
	ResetOriginalContactPoints(&(current_info->points));

	// 現在使っている接触のうちで、次も使いそうな接触を取り出す関数
	CreateConstraintFromCurrentInfo(current_info, &current_consts, 
									&NewcolToCol, &nearest_solids,
									&nearest_consts,
									bLocalDynamics_local);

	// ポインタに接触している接触と剛体を取得してくる関数
	GetSolidsCollisionsOnPointer(&pointer_consts, current_info->points,
								&nearest_consts, &nearest_solids);

	// 周囲の剛体による影響の計算
	if(bLocalDynamics_local)
	{
		// 接触候補点を探して作成する処理
		if(bPredictCollision_local) 
			CreateCandidateContactPoints(pointer_consts, 
										&col_candidate_consts, 
										&nearest_consts, &nearest_solids, 
										&col_candidate_pointer_pos, current_info);

		// 周囲の影響を計算するためのデータ作成処理
		// 関係のある剛体と接触を再帰的にすべてとってくる
		GetAllRelativeSolidsCollisions(&relative_consts, &relative_solids, nearest_solids, &nearest_consts);

		// 力を加えてみて動かし、影響を観測する関数
		PredictSimulations(pointer_consts, col_candidate_consts, current_consts, 
							nearest_solids, nearest_consts, 
							relative_solids, relative_consts,
							&se_map, &relSE);
	}

	// 接触の情報を作成する
	MakeHapticInfo(new_info, current_info, 
					pointer_consts,  
					col_candidate_consts, col_candidate_pointer_pos,
					current_consts,  
					NewcolToCol, 
					se_map, relSE,
					relative_solids, bLocalDynamics_local);
}

// 現在の情報から次も使う接触を作成する処理
void CreateConstraintFromCurrentInfo(HapticInfo* current_info, vector<pair<PHConstraint *, int> > *current_consts, map<PHConstraint *, int>* NewcolToCol,
									set<PHSolid *> *nearest_solids, set<PHConstraint *> *nearest_consts, bool bLocalDynamics_local)
{
	// 現在使用しているデータから
	// 実際に接触しているもの
	// 接触していないものを調べ
	// そのデータを次に使うかどうか判定する
	for(int i = 0; i < current_info->num_collisions; ++i)
	{
		// 実際には接触していない接触
 		if(!current_info->bCollide[i]) continue;

		int sol_index = current_info->ColToSol[i];

		// バーチャルカップリングの場合は
		// IsDynamical == falseの場合のみ接触点を継続する
//		if(!bLocalDynamics_local && current_info->nearest_solids[sol_index]->IsDynamical()) continue;

		// 現在使用しているContact Point(=constraint)のほとんどはstepが呼ばれるたびに削除されるので、
		// 毎回新しいContact Pointを作る必要がある
		// ここでは現在の位置と向きを持ったContact Pointを新たに作成する
		// ここで作られたContact Pointは hapticinfo->points に格納され、手動で消す
		// signをもういちどかけて前のステップのsignの乗算を打ち消してcontact pointを作成
//		PHContactPoint* point = CreateContactPoint(current_info->col_normals[i] * current_info->sign[i], (current_info->col_positions[i] + current_info->pointer_col_positions[i]) / 2, current_info->solid[i][0], current_info->solid[i][1], current_info->shapePair[i]);
		PHContactPoint* point = CreateContactPoint(current_info->col_normals[i] * current_info->sign[i], current_info->col_positions[i], current_info->solid[i][0], current_info->solid[i][1], current_info->shapePair[i]);

		// 現在のポインタ側の接触点を格納する
		NewcolToCol->insert(pair<PHConstraint *, int>(point, i));

		int sign;
		PHSolid* so = GetAdjacentSolid(point, DCAST(PHSolid, soPointer), &sign);

		// 今作った接触の登録
		current_consts->push_back(pair<PHConstraint *, int>(point, sign));

		nearest_consts->insert(point);
		nearest_solids->insert(so);

		// 次の回で削除に使うためにリンクを保存しておく
		current_info->points.insert(pair<double, PHContactPoint*>(point->pos.z, point));
	}
}

// ポインタを含む接触とその剛体をすべて取得してくる関数
void GetSolidsCollisionsOnPointer(vector<pair<PHConstraint *, int> >* pointer_consts, multimap<double, PHContactPoint*> points,
						  set<PHConstraint *>* nearest_consts, set<PHSolid *>* nearest_solids)
{
	// 接触点を取得
	PHConstraints cs = scene->GetConstraintEngine()->GetContactPoints();

	// すべての衝突点について調査
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); ++it)
	{
		int sign = 1;

		// ポインタを含む接触を取得
		PHSolid* col_solid = GetAdjacentSolid(*it, DCAST(PHSolid, soPointer), &sign);
			
		if(col_solid)
		{
			nearest_solids->insert(col_solid);

			PHContactPoint* contact = DCAST(PHContactPoint, *it);
			
			// 引き継ぐ接触と新しく作る接触の位置が近すぎる場合は
			// 新しい接触は追加しない
			// すでに引き継ぐ接触がこの代わりとして考えられるので、
			// 他の処理の部分は考える必要はない
			// BoundingBox的な考え方で計算対象を絞る
			bool bAdd = true;
			for(multimap<double, PHContactPoint*>::iterator it2 = points.lower_bound(contact->pos.z - COLLISION_DENSITY); it2 != points.upper_bound(contact->pos.z + COLLISION_DENSITY); ++it2)
			{
				// shapePairが同じなら同じ接触点のグループだと考える
				if(contact->shapePair != (*it2).second->shapePair) continue;

				// 追加しようとする接触点の距離（ベクトルのノルム）が
				// 小さすぎる場合は追加しない
				double norm = (contact->pos - (*it2).second->pos).norm();
				if(norm < COLLISION_DENSITY)
				{
					bAdd = false;
					break;
				}
			}

			if(bAdd)
			{
				pointer_consts->push_back(pair<PHConstraint *, int>(*it, sign));
				nearest_consts->insert(*it);
			}
		}
	}
}

// ポインタと接している剛体からスタートして、関係のある接触と剛体をすべてとってくる関数
void GetAllRelativeSolidsCollisions(set<PHConstraint*> *relative_consts, set<PHSolid*>* relative_solids, 
									set<PHSolid*> nearest_solids, set<PHConstraint*>* nearest_consts)
{
	// 接触点を取得
	PHConstraints cs = scene->GetConstraintEngine()->GetContactPoints();
	multimap<double, pair<PHConstraint *, bool> > csm;

	// 計算時間を短縮するために接触に処理済フラグをつけて保存する
	// 接触数が少ない場合は無駄が多いが、
	// 接触数が多くなったときの伸びはこちらのほうが断然少ないはず
	// さらにz軸のbounding boxで検索範囲を狭める
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); ++it)
	{
		csm.insert(pair<double, pair<PHConstraint *, bool> >(DCAST(PHContactPoint, *it)->pos.z, pair<PHConstraint *, bool>(*it, false)));
	}

	for(set<PHSolid*>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		// この剛体と生じた接触が次の剛体に影響を及ぼす可能性がある場合は、次の剛体にすすむ
		if((*it)->IsDynamical())
			RecursiveSolidsCollisionsRetrieval(&csm, *it, relative_consts, relative_solids, nearest_solids, nearest_consts, 0);
	}
}

// 与えられた剛体に直に接している剛体と、接触を取得する関数
void RecursiveSolidsCollisionsRetrieval(multimap<double, pair<PHConstraint *, bool> >* csm, PHSolid* solid, 
										set<PHConstraint *>* relative_consts, set<PHSolid *>* relative_solids, 
										set<PHSolid*> nearest_solids, set<PHConstraint *>* nearest_consts, int depth)
{
	// z軸に関するbounding boxで検索範囲を狭める
	for(multimap<double, pair<PHConstraint *, bool> >::iterator it = csm->lower_bound(solid->GetFramePosition().z - 1.5); it != csm->upper_bound(solid->GetFramePosition().z + 1.5); ++it)
	{
		// 処理済の場合は次にすすむ
		if((*it).second.second) continue;

		// 与えられた剛体から、その剛体に接触する剛体を取得する
		PHSolid* col_solid = GetAdjacentSolid(it->second.first, solid);

		// もしその剛体が存在した場合
		if(col_solid && col_solid != DCAST(PHSolid, soPointer))
		{
			// 処理済みにする
			it->second.second = true;

			// 接触自体を追加
			PHConstraint* c = it->second.first;
			
			// ポインタと直に接している剛体上に発生した接触は
			// nearest_constsに格納する
			// nearest_constsは予測シミュレーションの時に
			// correctionRateを低い値に設定して行うので
			// ポインタに直に接する剛体は
			// 多少のめり込みを許すが安定に動作できる
			if(depth)relative_consts->insert(c);
			else nearest_consts->insert(c);

			// PHSolidを保存
			// ポインタに直に接している剛体でなければ追加する
			// 重複している場合はsetなので自動的に排除される
			if(nearest_solids.find(c->solid[0]) == nearest_solids.end()) relative_solids->insert(c->solid[0]);
			if(nearest_solids.find(c->solid[1]) == nearest_solids.end()) relative_solids->insert(c->solid[1]);
			
			// 動かない剛体ではなく、かつ取得上限まで達していなかった場合、次の剛体にすすむ
			if(col_solid->IsDynamical() && depth < LIMIT_DEPTH) 
				RecursiveSolidsCollisionsRetrieval(csm, col_solid, relative_consts, relative_solids, nearest_solids, nearest_consts, depth+1);
		}
	}
}

// ポインタに接触しそうな剛体を探し、接触していなければ接触を作成する関数
void CreateCandidateContactPoints(vector<pair<PHConstraint *, int> > pointer_consts, 
								  vector<pair<PHConstraint *, int> >* col_candidate_consts, 
								  set<PHConstraint *> *nearest_consts, set<PHSolid *>* nearest_solids, 
								  map<PHContactPoint*, Vec3d>* col_candidate_pointer_pos, HapticInfo* current_info) 
{
	// すでに得られた接触点からshapePairを作成する処理
	// 接触候補点が登録された接触としてすでに存在するかどうかの判定に使う
	multimap<PHSolid*, Spr::PHShapePairForLCP*> shapePairMMap;
	CreateShapePairMMap(pointer_consts, &shapePairMMap);

	// BBoxを使ってポインタと接触しそうな剛体を取得してくる
	int pointer_index;
	set<int> candidate_solid_indices;
	GetCandidateSolidsByBBox(&candidate_solid_indices, &pointer_index);

	// 接触しそうな剛体がshapeレベルで本当に接触するか調べる
	for(set<int>::iterator it = candidate_solid_indices.begin(); it != candidate_solid_indices.end(); ++it)
	{		
		// 実際に位置関係を計算して接触しそうかどうかを判定
		// 接触しそうな情報を取得してくる
		vector<CandidateInfo> candidates;
		GetWillCollidePoints(*it, pointer_index, &candidates);

		// 接触しそうな情報のうちで
		// すでに接触として処理されているかどうかを判定
		// まだ処理されていなければ新しい接触を作成する
		for(int i = 0; i < (int)candidates.size(); ++i)
		{
			bool bExist = false;
			CandidateInfo cinfo = candidates[i];

			pair<multimap<PHSolid*, Spr::PHShapePairForLCP*>::iterator, multimap<PHSolid*, Spr::PHShapePairForLCP*>::iterator> it 
				= shapePairMMap.equal_range(cinfo.solid[1 - cinfo.pointer_index]);
			for(multimap<PHSolid*, Spr::PHShapePairForLCP*>::iterator it2 = it.first; it2 != it.second; ++it2)
			{
				// 接触しそうな剛体をキーとしてすでに登録された接触を検索する
				// もし両方のshapePairが等しければ
				// so && soPointer　でさらに
				// so shape && pointer shape となるので同じ接触とみなせる
				if((*it2).second == cinfo.shapePair)
				{
					bExist = true;
					break;
				}
			}

			// ポインタとの接触がない剛体か
			// 接触がある剛体だがそのshapePairではまだ接触が作られていないもの
			if(!bExist)
			{
				// 接触の作成
				PHContactPoint* point = CreateContactPoint(cinfo.normal, cinfo.closestPoint[1 - cinfo.pointer_index], cinfo.solid[0], cinfo.solid[1], cinfo.shapePair);
				// 関係のある接触
				nearest_consts->insert(point);
				// 近い剛体
				nearest_solids->insert(cinfo.solid[1 - cinfo.pointer_index]);
				// ポインタ側の接触点保存
				col_candidate_pointer_pos->insert(pair<PHContactPoint*, Vec3d>(point, cinfo.closestPoint[cinfo.pointer_index]));
				// 削除用にリンクを保存
				current_info->points.insert(pair<double, PHContactPoint*>(cinfo.closestPoint[1 - cinfo.pointer_index].z, point));

				int sign;
				GetAdjacentSolid(point, DCAST(PHSolid, soPointer), &sign);

				col_candidate_consts->push_back(pair<PHConstraint*, int>(point, sign));
			}
		}
	}
}

// ポインタに接触しそうな剛体を取得する関数
void GetCandidateSolidsByBBox(set<int> *candidate_solid_indices, int *pointer_index)
{
	// ここ以下はポインタと剛体をBBoxを使って
	// ｚ軸でソートして
	// 接触判定をする
	// BBoxを膨らませてあるので
	// 通常より多めに取得できるはず
	int N =	scene->NSolids();
	Vec3f dir(0, 0, 1);
	Edges edges;
	edges.resize(2 * N);

	PHSolids solids = scene->GetConstraintEngine()->solids;
	Edges::iterator eit = edges.begin();

	for(int i = 0; i < N; ++i){
		// ポインタの添え字を取得
		if(solids[i] == DCAST(PHSolid, soPointer)) *pointer_index = i;
		
		solids[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		Vec3d dPos = solids[i]->GetDeltaPosition();
		float dLen = (float) (dPos * dir);
		if (dLen < 0){
			eit[0].edge += dLen;
		}else{
			eit[1].edge += dLen;
		}
		eit[0].index = i; eit[0].bMin = true;
		eit[1].index = i; eit[1].bMin = false;
		eit += 2;
	}
	// ｚ座標でソート
	std::sort(edges.begin(), edges.end());

	typedef std::set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	bool bCheck = false;
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
			// ポインタ以外の剛体の始まり
			if(it->index != *pointer_index) 
			{
				cur.insert(it->index);
				
				// もしすでにpointerが始まっていればチェック対象になる
				if(bCheck)
				{
					int f1 = *pointer_index;
					candidate_solid_indices->insert(it->index);
				}
			}
			// ポインタの始まり
			else
			{
				// ここから先に出てきたEdgeは全部チェック対象
				bCheck = true;
				int f1 = *pointer_index;

				// 登録されている剛体のペアをすべてチェック
				for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
					candidate_solid_indices->insert(*itf);
				}
			}
		}else{
			// ポインタ以外の剛体の終わり
			if(it->index != *pointer_index){
				cur.erase(it->index);			//	終端なので削除．
			}
			// ポインタのBBoxの終わり
			// ここまでで得られた剛体のindexが接触候補
			else break;
		}
	}
}

// bounding boxの判定を元に
// ポインタと剛体が接触しそうかどうかを判定する処理
// 接触しそうな剛体がある場合は情報を作成してベクトルにつめる
void GetWillCollidePoints(int so_index, int pointer_index, vector<CandidateInfo>* candidates)
{
	Vec3d normal;
	Vec3d closestPoint[2];
	double distance;
	PHSolid* solid[2];

	// solidpairをつくる処理
	// index_small < index_large
	// なので、ここで判定してsolidを用意
	if(so_index > pointer_index)
	{
		solid[0] = DCAST(PHSolid, soPointer);
		solid[1] = scene->GetConstraintEngine()->solids[so_index];
	}
	else 
	{
		solid[0] = scene->GetConstraintEngine()->solids[so_index];
		solid[1] = DCAST(PHSolid, soPointer);
	}
	
	// 必要な情報の準備
	std::vector<Vec3d> deltaPos[2];
	std::vector<Posed> shapePose[2];
	double dt = scene->GetTimeStep();
	for(int i = 0; i < 2; i++){
		deltaPos[i].resize(solid[i]->NShape());
		shapePose[i].resize(solid[i]->NShape());
		for(int j = 0; j < solid[i]->NShape(); j++){
			CDConvex* convex = DCAST(CDConvex, solid[i]->GetShape(j));
			Posed lp = solid[i]->GetShapePose(j);
			Vec3d c = lp * convex->GetCenter();
			if(solid[i] == DCAST(PHSolid, soPointer)) deltaPos[i][j] = (pointer_vel * dt) + (pointer_angvel ^ (c - solid[i]->GetCenterPosition())) * dt;
			else deltaPos[i][j] = solid[i]->GetDeltaPosition(c);
			shapePose[i][j] = solid[i]->GetPose() * lp;
			shapePose[i][j].Pos() -= deltaPos[i][j];
		}
	}

	for(int i = 0; i < solid[0]->NShape(); ++i)
	{
		for(int j = 0; j < solid[1]->NShape(); ++j)
		{
			// 相対移動位置
			Vec3d delta = deltaPos[0][i] - deltaPos[1][j];

			// 接触予想
			// 距離を調べる
			int res = ContFindCommonPoint(DCAST(CDConvex, solid[0]->GetShape(i)), 
										DCAST(CDConvex, solid[1]->GetShape(j)), 
										shapePose[0][i], shapePose[1][j], 
										delta, normal, closestPoint[0], closestPoint[1], distance);

			// もうすぐ接触する可能性がある
			if(res > 0)
			{
				// 距離を調べる
				double toi = distance / (delta * delta);
				if(toi >= 0 && toi <= POINTER_RADIUS * (EPSILON - 1))
				{
					// 最近傍の点を調べる
					FindClosestPoints(DCAST(CDConvex, solid[0]->GetShape(i)), 
									DCAST(CDConvex, solid[1]->GetShape(j)), 
									shapePose[0][i], shapePose[1][j], closestPoint[0], closestPoint[1]);

					// 接触候補の情報を作成
					CandidateInfo cinfo;
					
					// 添え字は （小,大）となっている
					// そのためこの場合は交換する
					if(so_index > pointer_index)
					{
						// pointer
						cinfo.closestPoint[0] = shapePose[1][j] * closestPoint[1];
						// so
						cinfo.closestPoint[1] = shapePose[0][i] * closestPoint[0];
						cinfo.shapePair = scene->GetConstraintEngine()->solidPairs.item(pointer_index, so_index)->shapePairs.item(i, j);
						cinfo.normal = cinfo.closestPoint[1] - cinfo.closestPoint[0]; // - normal;

						cinfo.solid[0] = DCAST(PHSolid, soPointer);
						cinfo.solid[1] = solid[1];
						cinfo.pointer_index = 0;
					}
					else
					{
						// so
						cinfo.closestPoint[0] = shapePose[0][i] * closestPoint[0];
						// pointer
						cinfo.closestPoint[1] = shapePose[1][j] * closestPoint[1];
						cinfo.shapePair = scene->GetConstraintEngine()->solidPairs.item(so_index, pointer_index)->shapePairs.item(i, j);
						cinfo.shapePair->depth = 0;

						// 法線の計算
						cinfo.normal = cinfo.closestPoint[1] - cinfo.closestPoint[0]; // normal;
						cinfo.solid[0] = solid[0];
						cinfo.solid[1] = DCAST(PHSolid, soPointer);
						cinfo.pointer_index = 1;
					}
					// 追加
					candidates->push_back(cinfo);
				}
			}
		}
	}
}

// 与えられた情報からshapePairのマップを作成する関数
void CreateShapePairMMap(vector<pair<PHConstraint *, int> > pointer_consts, multimap<PHSolid*, Spr::PHShapePairForLCP*>* shapePairMMap)
{
	// すでに得られたポインタ周りの接触から
	// 接触対象の剛体とポインタとのshapePairが引けるようにする
	int size = (int)pointer_consts.size();
	for(int i = 0; i < size; ++i)
	{
		PHConstraint *consts;
		consts = pointer_consts.at(i).first;			
		shapePairMMap->insert(pair<PHSolid*, Spr::PHShapePairForLCP*>(GetAdjacentSolid(consts, DCAST(PHSolid, soPointer)), 
																	DCAST(PHContactPoint, consts)->shapePair));
	}
}

// 提案手法のために先送りシミュレーションを計画的に行ってくれる関数
void PredictSimulations(vector<pair<PHConstraint *, int> > pointer_consts, vector<pair<PHConstraint *, int> > col_candidate_consts, vector<pair<PHConstraint *, int> > current_consts, 
						set<PHSolid *> nearest_solids, set<PHConstraint *> nearest_consts,
						set<PHSolid *> relative_solids, set<PHConstraint *> relative_consts,
						map<PHSolid *, SEData>* SEMap, vector<SEData>* relSE)
{
	double dt = scene->GetTimeStep();

	map<PHSolid *, Vec3d> coeff;
	// 予測シミュレーションのセットアップ
	// 必要な情報の準備
	SetupPredictSimulation(relative_consts, relative_solids, nearest_solids, nearest_consts, &coeff);

	// 他の周囲の物体の動きを保存する変数
	vector<SpatialVector> relative_solid_velocity_const;
	// 加える力を０ベクトルとして定数項を取得
	vector<SpatialVector> b = PredictSimulationToCollision(&relative_solid_velocity_const,
															NULL, NULL,
															relative_consts, relative_solids, 
															nearest_consts, nearest_solids,
															Vec3d(), coeff);

	// ポインタに直に接する剛体の、何も力を加えなかった場合の動きを保存する処理
	// 剛体数だけSEDataを宣言してmapに追加する
	int local_counter = 0;
	map<PHSolid*, SEData> se_map;
	for(set<PHSolid*>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		SEData se;
		se.constant_effect = SpatialVector((*it)->GetOrientation() * b[local_counter].v(),
										   (*it)->GetOrientation() * b[local_counter].w());
		se_map.insert(pair<PHSolid*, SEData>((*it), se));
		++local_counter;
	}

	// ポインタに間接的に接触する剛体の何も力を加えなかったときの動きを保存する処理
	// 関係する物体の定数部分をワールド座標系に直して保存
	vector<SEData> relative_se;
	local_counter = 0;
	for(set<PHSolid*>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		SEData se;
		se.constant_effect = SpatialVector((*it)->GetOrientation() * relative_solid_velocity_const[local_counter].v(),
										   (*it)->GetOrientation() * relative_solid_velocity_const[local_counter].w());
		relative_se.push_back(se);
		++local_counter;
	}
	
	bool bCol, bPrevCol;
	PHSolid* solid;
	int sign;
	Vec3d vv;

	// 接触数＝列数だけ、力を加えて動きを観測を繰り返す
	vector<SpatialVector> relative_solid_velocity_x, relative_solid_velocity_y, relative_solid_velocity_z;
	int total_size = (int)pointer_consts.size() + (int)col_candidate_consts.size() + (int)current_consts.size();
	for(int i = 0; i < total_size; ++i)
	{
		// i番目の接触を取り出す
		PHConstraint* constraint = GetSpecificDataFromCollisions(i, 
																pointer_consts, col_candidate_consts, map<PHContactPoint*, Vec3d>(), 
																current_consts, &sign, &solid, &vv, &bCol, &bPrevCol);

		// 周囲の影響を計算すべき剛体
		if(solid->IsDynamical())
		{
			// ある単位ベクトルを加えてその結果をあらわすベクトルを取得
			// 以下の３つの関数である接触に力を加えたときの,すべての剛体の反応がわかる
			vector<SpatialVector> vec_x = PredictSimulationToCollision(&relative_solid_velocity_x,
															constraint, solid,
															relative_consts, relative_solids, 
															nearest_consts, nearest_solids,
															Vec3d(TestForce.x, 0, 0), coeff, i);
			vector<SpatialVector> vec_y = PredictSimulationToCollision(&relative_solid_velocity_y,
															constraint, solid,
															relative_consts, relative_solids, 
															nearest_consts, nearest_solids,
															Vec3d(TestForce.x, TestForce.y, 0), coeff, i);
			vector<SpatialVector> vec_z = PredictSimulationToCollision(&relative_solid_velocity_z,
															constraint, solid,
															relative_consts, relative_solids, 
															nearest_consts, nearest_solids,
															Vec3d(TestForce.x, 0, TestForce.z), coeff, i);

			// このループで剛体soの接触iに関する動きをあらわす行列を計算し格納
			// この接触のポインタに接するすべての剛体に対する影響をこのループで処理する
			// PredictSimulation内でベクトルへの要素の追加はnearest_solidsのiteratorでやっているので、
			// 今回もそのループでまわせば対応するPHSolidがわからなくても検索を避けて追加できる
			local_counter = 0;
			for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
			{
				PHSolid *so = *it;
				SpatialVector sv = b[local_counter];
				SpatialVector sv_x = vec_x[local_counter];
				SpatialVector sv_y = vec_y[local_counter];
				SpatialVector sv_z = vec_z[local_counter];
				++local_counter;

				// 先ほど加えた単位力から、影響を求めて行列を作成する
				Matrix3d v = Matrix3d(so->GetOrientation() * ((sv_x.v() - sv.v())   / TestForce.x), 
									  so->GetOrientation() * ((sv_y.v() - sv_x.v()) / TestForce.y), 
									  so->GetOrientation() * ((sv_z.v() - sv_x.v()) / TestForce.z));
				Matrix3d w = Matrix3d(so->GetOrientation() * ((sv_x.w() - sv.w())   / TestForce.x), 
									  so->GetOrientation() * ((sv_y.w() - sv_x.w()) / TestForce.y), 
									  so->GetOrientation() * ((sv_z.w() - sv_x.w()) / TestForce.z));
				
				// データの格納
				pair<Matrix3d, Matrix3d> p = pair<Matrix3d, Matrix3d>(v, w);

				// 剛体ごとに接触と係数行列のマップを作成する
				// 接触の順番はわかるが剛体の順番はわからないため。
				// 剛体ごとに接触の順番に行列が格納されていく
				se_map.find(so)->second.surround_effect.push_back(p);
			}

			// この接触に力を加えたときの、ポインタとは直に接していない周囲の剛体達の動きを観察して保存する
			local_counter = 0;
			for(set<PHSolid *>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
			{
				PHSolid *so = *it;
				SpatialVector sv = relative_solid_velocity_const[local_counter];
				SpatialVector sv_x = relative_solid_velocity_x[local_counter];
				SpatialVector sv_y = relative_solid_velocity_y[local_counter];
				SpatialVector sv_z = relative_solid_velocity_z[local_counter];

				// 先ほど加えた単位力から影響を求めて行列を作成する
				Matrix3d v = Matrix3d(so->GetOrientation() * ((sv_x.v() - sv.v())   / TestForce.x), 
									  so->GetOrientation() * ((sv_y.v() - sv_x.v()) / TestForce.y), 
									  so->GetOrientation() * ((sv_z.v() - sv_x.v()) / TestForce.z));
				Matrix3d w = Matrix3d(so->GetOrientation() * ((sv_x.w() - sv.w())   / TestForce.x), 
									  so->GetOrientation() * ((sv_y.w() - sv_x.w()) / TestForce.y), 
									  so->GetOrientation() * ((sv_z.w() - sv_x.w()) / TestForce.z));
				
				// データの格納
				pair<Matrix3d, Matrix3d> p = pair<Matrix3d, Matrix3d>(v, w);
				relative_se[local_counter].surround_effect.push_back(p);
				++local_counter;
			}
			relative_solid_velocity_x.clear();
			relative_solid_velocity_y.clear();
			relative_solid_velocity_z.clear();
		}
		// 動かない剛体との接触の場合
		else
		{
			// ポインタと直接接している剛体の動き
			for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
			{
				se_map.find((*it))->second.surround_effect.push_back(pair<Matrix3d, Matrix3d>(Matrix3d(), Matrix3d()));
			}

			// ポインタには直接接していない剛体の動き
			local_counter = 0;
			for(set<PHSolid *>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
			{
				relative_se[local_counter].surround_effect.push_back(pair<Matrix3d, Matrix3d>(Matrix3d(), Matrix3d()));
				++local_counter;
			}
		}
	}
	
	// 結果の格納
	*SEMap = se_map;
	*relSE = relative_se;
}

// 予測シミュレーションをセットアップする
void SetupPredictSimulation(set<PHConstraint *> relative_consts, set<PHSolid *> relative_solids, set<PHSolid *> nearest_solids, set<PHConstraint *> nearest_consts, map<PHSolid *, Vec3d> *coeff)
{
	double dt = scene->GetTimeStep();
	Vec3d gravity = scene->GetGravity();

	// すべての剛体をセットアップ
	for(set<PHSolid *>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		// 重力を加える
		if((*it)->IsDynamical() && bGravitySet.find((*it)) == bGravitySet.end()) (*it)->AddForce(gravity * (*it)->GetMass());
		(*it)->UpdateCacheLCP(dt);
	}
	for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		// 重力を加える
		if((*it)->IsDynamical() && bGravitySet.find((*it)) == bGravitySet.end()) (*it)->AddForce(gravity * (*it)->GetMass());
		(*it)->UpdateCacheLCP(dt);
	}

/*
	for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		PHSolid* solid = *it;

		// 単位力を加える処理のところで使う値(=加えた力をワールド座標から拘束座標に変換するもの)を今のうちに計算しておく
		Vec3d w = solid->GetOrientation().Conjugated() * solid->GetAngularVelocity();
		Vec3d calc = w % (solid->GetInertia() * w);
		coeff->insert(pair<PHSolid *, Vec3d>(solid, calc));
	}
*/

	// すべての接触についてヤコビアンを計算
	// ヤコビアンの計算は一度だけでよい
	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{
		(*it)->CompJacobian();
	}
	for(set<PHConstraint *>::iterator it = nearest_consts.begin(); it != nearest_consts.end(); ++it)
	{
		(*it)->CompJacobian();
	}

	// Setupが終わった時点でのパラメータを保存しておく
	// 次に行われるPredictSimulationでは
	// RestoreParametersを呼ぶことで
	// この時点のパラメータにセットしなおして
	// 繰り返し計算できるようにする
	SaveParameters(relative_consts, relative_solids, nearest_consts, nearest_solids);
}

// ある接触に力を加えて予測シミュレーションする関数
vector<SpatialVector> PredictSimulationToCollision(vector<SpatialVector>* relative_solid_velocity, 
													PHConstraint* constraint, PHSolid* solid,
													set<PHConstraint *> relative_consts, set<PHSolid *> relative_solids,
													set<PHConstraint *> nearest_consts, set<PHSolid *> nearest_solids,
													Vec3d force_vec, map<PHSolid *, Vec3d> coeff, int index)
{
	// ポインタが接する剛体のうちindex番目の接触に単位力を加える
	if(index != -1) AddForceToConstraint(index, constraint, solid, coeff, force_vec);
	return PredictSimulation(relative_solid_velocity, nearest_solids, nearest_consts, relative_consts, relative_solids);
}

// 予測シミュレーションをする関数
vector<SpatialVector> PredictSimulation(vector<SpatialVector>* relative_solid_velocity, 
										set<PHSolid *> nearest_solids, set<PHConstraint *> nearest_consts,
										set<PHConstraint *> relative_consts, set<PHSolid *> relative_solids)
{
	// ポインタとの接触に関しては,correctionRateを下げることで発振を防ぐ
	// こうするとめりこみは起こるが,安定に剛体が運動する
	// そのほかの接触に関してはcorrectionRateを通常の値にして
	// 不安定になるがめり込みが起こらないようにする(開発中)
	double pos_correctionRate = scene->GetConstraintEngine()->posCorrectionRate;

	// シミュレーションに必要な情報をセットアップ
	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{ 
		(*it)->SetupLCP();
	}
	scene->GetConstraintEngine()->posCorrectionRate = PCORRECTIONRATE;
	for(set<PHConstraint *>::iterator it = nearest_consts.begin(); it != nearest_consts.end(); ++it)
	{ 
		(*it)->SetupLCP();
	}
	scene->GetConstraintEngine()->posCorrectionRate = pos_correctionRate;

	for(int i = 0; i < NUM_PREDICT_ITERATE; ++i)
	{
		for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
		{
			// すべての接触をiterate
			(*it)->IterateLCP();
		}
		for(set<PHConstraint *>::iterator it = nearest_consts.begin(); it != nearest_consts.end(); ++it)
		{
			// すべての接触をiterate
			(*it)->IterateLCP();
		}
	}

	// 影響を観測
	// 剛体の速度変化を取得
	vector<SpatialVector> effects;
	for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		effects.push_back((*it)->dv);
	}

	// 周囲の物体の速度変化を格納
	for(set<PHSolid*>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		relative_solid_velocity->push_back((*it)->dv);
	}

	// 上書きしてしまう変数の回復
	RestoreParameters(&relative_consts, &relative_solids, &nearest_consts, &nearest_solids);

	// 影響をreturn
	return effects;
}

// 特定の接触に含まれる剛体に力を加える関数
void AddForceToConstraint(int index, PHConstraint* constraint, PHSolid* solid, map<PHSolid *, Vec3d> coeff, Vec3d force_vec)
{
	double dt = scene->GetTimeStep();
	PHContactPoint* contact = DCAST(PHContactPoint, constraint);
	Vec3d m_accel;
	Vec3d r_accel;

//		force_vec = contact->Xj[0].q * force_vec;
//		map<PHSolid *, Vec3d>::iterator cef = coeff.find(si);

	// ワールド座標の力を剛体座標に直して加える
	force_vec = solid->GetOrientation().Conjugated() * force_vec;
	m_accel = solid->GetMassInv() * force_vec * dt;
	r_accel = solid->GetInertiaInv() * ((solid->GetOrientation().Conjugated() * (contact->pos - solid->GetCenterPosition()) ^ force_vec)) * dt;

	// 力を加えた結果による速度変化を足す
	solid->dv.v() += m_accel;
	solid->dv.w() += r_accel;
}

// 提示力の計算および提案手法に必要な情報を集めて構造体を作成する
void MakeHapticInfo(HapticInfo *new_info, HapticInfo *current_info, 
					vector<pair<PHConstraint *, int> > pointer_consts,  
					vector<pair<PHConstraint *, int> > col_candidate_consts, map<PHContactPoint*, Vec3d> col_candidate_pointer_pos,
					vector<pair<PHConstraint *, int> > current_consts,  
					map<PHConstraint *, int> NewcolToCol, 
					map<PHSolid*, SEData> se_map,
					vector<SEData> relSE, set<PHSolid *> relative_solids, bool bLocalDynamics_local)
{
	// 力覚情報の初期化
	InitHapticInfo(new_info);

	// 力覚レンダリングに必要なデータの作成
	int num_collisions = (int)pointer_consts.size() + (int)col_candidate_consts.size() + (int)current_consts.size();
	for(int i = 0; i < num_collisions; ++i)
	{
		// 許容数を超える場合は打ち切り
		if(i > NUM_COLLISIONS) break;

		PHConstraint* constraint;
		int sign;
		PHSolid* so;
		
		// ポインタ側の接触点と剛体側の接触点が分けられている場合に
		// ポインタ側の接触点を格納する変数
		Vec3d pointer_col_position = Vec3d();

		bool bCollide = true;
		bool bPreviousCollide = false;

		// すべての接触から今回の処理に必要なデータを取ってくる関数
		constraint = GetSpecificDataFromCollisions(i, 
									pointer_consts, col_candidate_consts, col_candidate_pointer_pos, 
									current_consts,	&sign, &so, &pointer_col_position, &bCollide, &bPreviousCollide);

		// 剛体の登録と,その剛体の周囲からの影響の登録の処理
		RegisterNewSolid(new_info, i, current_info, so, se_map);

		// 接触の登録と,その他フラグの登録
		RegisterNewCollision(new_info, i, current_info, constraint, sign, 
			bPreviousCollide, bCollide, NewcolToCol, pointer_col_position, 
			bLocalDynamics_local);
	}

	// 周囲の物体への影響を格納
	if(bLocalDynamics_local)
	{
		new_info->relative_solids_velocity = relSE;
		new_info->relative_solids = relative_solids;
	}

	// 接触数の登録
	new_info->num_collisions = num_collisions;

	// 時間幅周りの計算を行う
	calcTimeSpan(new_info);

	// 接触点が多すぎる場合は打ち切る
	if(new_info->num_collisions >= NUM_COLLISIONS) 
	{
		cout << "number of collisions is overfull: " << num_collisions << endl << "you must change to acceptable numbers" << endl;
		new_info->num_collisions = NUM_COLLISIONS-1;
	}
}

// すべての接触からi番目のデータを取ってくる関数
PHConstraint* GetSpecificDataFromCollisions(int i, 
					 vector<pair<PHConstraint*, int> > pointer_consts, 
					 vector<pair<PHConstraint*, int> > col_candidate_consts,  
					 map<PHContactPoint*, Vec3d> col_candidate_pointer_pos,
					 vector<pair<PHConstraint*, int> > current_consts, 
					 int* sign, PHSolid** so, Vec3d* pointer_col_position, bool* bCollide, bool* bPreviousCollide)
{
	PHConstraint* constraint;
	int pointer_consts_size = (int)pointer_consts.size();
	int col_candidate_consts_size = (int)col_candidate_consts.size();
	int current_consts_size = (int)current_consts.size();

	// 実際に接触し、動く剛体を含む接触
	if(pointer_consts_size > i)
	{
		constraint = pointer_consts[i].first;
		*sign = pointer_consts[i].second;

		if(*sign > 0) *so = pointer_consts[i].first->solid[0];
		else *so = pointer_consts[i].first->solid[1];
	}
	// 前ステップで接触していた
	else if(pointer_consts_size + current_consts_size > i)
	{
		int i_local = i - pointer_consts_size;

		constraint = current_consts[i_local].first;
		*sign = current_consts[i_local].second;

		if(*sign > 0) *so = current_consts[i_local].first->solid[0];
		else *so = current_consts[i_local].first->solid[1];

		*bPreviousCollide = true;
	}
	// ポインタには接触していなく、動く剛体を含む接触
	// ポインタに接触しそうなものたち
	else if(pointer_consts_size + current_consts_size + col_candidate_consts_size > i)
	{
		int i_local = i - (pointer_consts_size + current_consts_size);
		constraint = col_candidate_consts[i_local].first;
		*sign = col_candidate_consts[i_local].second;

		*pointer_col_position = col_candidate_pointer_pos.find(DCAST(PHContactPoint, constraint))->second;

		if(*sign > 0) *so = col_candidate_consts[i_local].first->solid[0];
		else *so = col_candidate_consts[i_local].first->solid[1];

		// この接触は実際には接触していないのでfalse
		*bCollide = false;
	}
	return constraint;
}

// 剛体の登録と,接触と剛体の関連付け
void RegisterNewSolid(HapticInfo* new_info, int i, HapticInfo* current_info, PHSolid* so, map<PHSolid*, SEData> se_map)
{
	int counter = 0;
	// 配列にPHSolidを追加
	// 剛体と接触が１対１ではないため、接触から剛体へ添え字を変換するマップを作成する
	while(1)
	{
		// まだ追加されていない剛体は追加する
		if(!new_info->nearest_solids[counter])
		{
			new_info->nearest_solids[counter] = so;
			
			// 剛体から接触へ
			// また接触から剛体へ時間をかけずに変換できるように
			// 配列に添え字を格納しておく
			new_info->SolToCol[counter][new_info->num_col_per_sol[counter]] = i;
			++new_info->num_col_per_sol[counter];
			new_info->ColToSol[i] = counter;

			// 前回の接触と今回の接触で同じ剛体が含まれていればindexを保存する
			new_info->NewsolToSol[counter] = -1;
			for(int j = 0; j < current_info->num_solids; ++j)
			{
				if(current_info->nearest_solids[j] == so)
				{
					new_info->NewsolToSol[counter] = j;
					break;
				}
			}			

			// 剛体数を更新
			new_info->num_solids = counter+1;

			// 物理情報も剛体ごとに追加
			new_info->solid_velocity[counter] = so->GetVelocity();
			new_info->solid_angular_velocity[counter] = so->GetAngularVelocity();
			new_info->solid_massinvs[counter] = so->GetMassInv();
			new_info->solid_inertiainvs[counter] = so->GetInertiaInv();
			new_info->solid_center_positions[counter] = so->GetCenterPosition();
			new_info->solid_orientations[counter] = so->GetOrientation();

			// 周囲の影響をあらわす変数をコピーする
			// この変数は剛体固有の値なので、
			// 剛体が追加された時点で格納処理を行う
			if(so->IsDynamical())
			{
				map<PHSolid *, SEData>::iterator it = se_map.find(so);

				new_info->vel_constant[counter] = (*it).second.constant_effect.v();
				new_info->ang_constant[counter] = (*it).second.constant_effect.w();
				
				int size = (int)(*it).second.surround_effect.size();
				for(int j = 0; j < size; ++j)
				{
					new_info->vel_effect[counter][j] = (*it).second.surround_effect[j].first;
					new_info->ang_effect[counter][j] = (*it).second.surround_effect[j].second;
				}
			}
			else
			{
				new_info->vel_constant[counter] = Vec3d();
				new_info->ang_constant[counter] = Vec3d();
				new_info->gravity_effect[counter] = SpatialVector();
			}

			// この物体に重力が適応されているか調べる			
			// 登録されていれば,その剛体はbGravity == falseなのでfalseにする
			set<PHSolid*>::iterator it = bGravitySet.find(so);
			if(it != bGravitySet.end()) new_info->bGravity[counter] = false;
			else new_info->bGravity[counter] = true;

			break;
		}
		// もし剛体がすでに追加済みの場合は追加しない
		// 添え字変換表のみ作成
		else if(new_info->nearest_solids[counter] == so)
		{
			new_info->SolToCol[counter][new_info->num_col_per_sol[counter]] = i;
			++new_info->num_col_per_sol[counter];
			new_info->ColToSol[i] = counter;

			break;
		}

		// 次の登録された剛体に進む
		++counter;
	}
}

// 接触の登録を行う
void RegisterNewCollision(HapticInfo* new_info, int i, HapticInfo* current_info, PHConstraint* constraint, int sign, 
						  bool bPreviousCollide, bool bCollide, map<PHConstraint*, int> NewcolToCol, Vec3d pointer_col_position, 
						  bool bLocalDynamics_local)
{
	new_info->constraint[i] = constraint;
	new_info->sign[i] = sign;

	PHContactPoint* contact = DCAST(PHContactPoint, constraint);
	new_info->solid[i][0] = contact->solid[0];
	new_info->solid[i][1] = contact->solid[1];

	new_info->shapePair[i] = contact->shapePair;

	new_info->bPreviousCollide[i] = bPreviousCollide;
	new_info->bCollide[i] = bCollide;

	new_info->pointer_pos[i] = pointer_pos;
	new_info->pointer_ori[i] = pointer_ori;

/*
	// ワールド座標から拘束座標に直す４元数を登録
	// 周囲の影響を使うときに
	// ワールド座標の力を拘束座標に移して係数を使う
	// 周囲の影響は入力が拘束座標で出力がワールド座標
	if(bLocalDynamics_local)
	{
		if(sign > 0) new_info->wldTosld[i] = contact->Xj[0].q.Conjugated();
		else new_info->wldTosld[i] = contact->Xj[1].q.Conjugated();
	}
	// 局所的な動力学計算を行わない場合は
	// 周囲の影響を計算するときに
	// ワールド座標の力をワールド座標のまま
	// 周囲の影響（＝運動方程式）をかけて
	// 運動方程式を作るために
	// 力での拘束座標への変換を打ち消すように剛体座標系を収納する
	else
	{
		if(sign > 0) new_info->wldTosld[i] = contact->solid[0]->GetOrientation();
		else new_info->wldTosld[i] = contact->solid[1]->GetOrientation();
	}
*/

	// 引き継いだ接触
	if(bPreviousCollide)
	{
		// この接触から現在のデータへの引数を取得する
		int index = NewcolToCol.find(constraint)->second;

		// 現在の値を次も引き継ぐ
		new_info->col_positions[i] = current_info->col_positions[index];
		new_info->col_normals[i] = current_info->col_normals[index];
		new_info->pointer_col_positions[i] = current_info->pointer_col_positions[index];
		
		// 添え字の変換表を作成
		new_info->NewcolToCol[i] = index;
	}
	else
	{
		// 現時点で接触している
		if(bCollide)
		{
			// ポインタの接触点は接触の接触点と同じ
			new_info->pointer_col_positions[i] = contact->pos;
			new_info->col_normals[i] = contact->shapePair->normal * sign;
		}
		// 実際には接触していない
		// 接触候補点
		else
		{
			// ポインタの接触点は接触点と別
			new_info->pointer_col_positions[i] = pointer_col_position;
			new_info->col_normals[i] = (pointer_col_position - contact->pos).unit();
		}
		new_info->col_positions[i] = contact->pos;

		// 一応-1で初期化する
		new_info->NewcolToCol[i] = -1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// 力覚計算関係の処理

// 予測シミュレーションに使う単位力を作成する関数
inline void calcTestForce()
{
	// 予測シミュレーションに使う単位力のベクトルの用意
	// 一番最後に加えた力を使う
	// 力を加えなかった場合は、移動量から適当に力を計算
	if(LastVCForce != Vec3d()) TestForce = LastVCForce;
	else TestForce = 100 * (spidar_pos - pointer_pos);

	// ゼロ割を防ぐためある程度以上小さくなったら0.0001に統一する
	if(fabs(TestForce.x) == 0.0)
	{
		TestForce.x = 0.0000001;
	}
	if(fabs(TestForce.y) == 0.0)
	{
		TestForce.y = 0.0000001;
	}
	if(fabs(TestForce.z) == 0.0)
	{
		TestForce.z = 0.0000001;
	}

	if(fabs(TestForce.x) < 0.0001) TestForce.x = 0.0001 * fabs(TestForce.x) / TestForce.x;
	if(fabs(TestForce.y) < 0.0001) TestForce.y = 0.0001 * fabs(TestForce.y) / TestForce.y;
	if(fabs(TestForce.z) < 0.0001) TestForce.z = 0.0001 * fabs(TestForce.z) / TestForce.z;

//	TestForce = Vec3d(1, 1, 1);
}

// 引数の力をhaptic deviceに提示する処理
inline void feedbackForce(bool feedback, Vec3d VCForce, Vec3d VCTorque)
{
	if(feedback && bForce)
	{
		// SPIDARの空間と見ている空間が違うので行列を掛けて射影する
		// virtual couplingの逆向きの力・トルクを発生
		Vec3d f = - view_haptic * VCForce / SPIDAR_SCALE;
		Vec3d t = - view_haptic * VCTorque / SPIDAR_SCALE;

		spidarG6.SetForce(FORCE_COEFF * MASS_SCALE * f, FORCE_COEFF * MASS_SCALE * t);
	}
	else spidarG6.SetForce(Vec3d(), Vec3d());
}

// 物理情報とSPIDARの情報を更新する関数
inline HapticInfo* updateInfo()
{
	////////////////////////////////////////////////////////////////////////////////////////
	// 物理情報の取得
	// シミュレーションから得られる情報を格納する変数
	HapticInfo* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data) info = &info1;
	else info = &info2;

	/////////////////////////////////////////////////////////////////////////////////////////
	// SPIDAR情報を更新する
	Vec3d SPIDARVel;
	Quaterniond SPIDARAngVel;
	UpdateSpidarData(&SPIDARVel, &SPIDARAngVel);

	return info;
}

// バネダンパから提示力を計算する関数
inline void calcForceBySpringDumper(Vec3d* VCForce, Vec3d* VCTorque)
{
	*VCForce = Km * (spidar_pos - pointer_pos);// + Bm * (SPIDARVel - M_vec/dt);
	*VCTorque = (Kr * spidar_ori * pointer_ori.Inv()).Rotation();// + Br * (SPIDARAngVel.Rotation() - R_vec/dt);
}

// 局所的な動力学計算
inline void calcLocalDynamics(HapticInfo* info, Vec3d* VCForce, Vec3d* VCTorque, bool* feedback)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// ６自由度のめり込み解消処理

	// 内部的にpointer_pos, pointer_oriを変更している
	CorrectPenetration(info);

	// 接触剛体の接触力による速度・角速度更新
	calcForceBySpringDumper(VCForce, VCTorque);

	// 接触点のめり込み量から
	// 各接触点での力の重みを計算する関数
	CalcForceWeight(info);

	// ポインタとの接触による剛体の更新
	UpdateSolidByCollision(info, *VCForce, feedback);
}

// 剛体上の複数の接触点のめり込み量から
// 各接触点での力を加える割合を計算する関数
void CalcForceWeight(HapticInfo* info)
{
	for(int i = 0; i < info->num_solids; ++i)
	{
		double sum = 0.0;
		
		for(int j = 0; j < info->num_col_per_sol[i]; ++j)
		{
			int col_index = info->SolToCol[i][j];

			// めり込み量の総量を計算
			sum += info->weight[col_index];
		}
			
		if(sum == 0.0) continue;

		for(int j = 0; j < info->num_col_per_sol[i]; ++j)
		{
			int col_index = info->SolToCol[i][j];

#if defined EXP1
			// 実験１の場合はVCと条件を合わせるためにこっちにする
			info->weight[col_index] = 1.0 / info->num_col_per_sol[i];
#else
			// 総和で割って割合を計算する
			// 触った感じこっちのほうが安定みたい
			// 力の配分は深度に比例
			info->weight[col_index] /= sum;
#endif
		}
	}
}

/**
 brief  	提示力の計算とHaptic Device	へ反映. multimedia timerによって呼び出される
			内部的にはポインタとSPIDARをvirtual couplingでつなぎ、ポインタも更新している
 param	 	なし
 return 	なし
*/
#ifdef EXP1
void HapticRendering()
#else
void CALLBACK HapticRendering(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
#endif
{
	///////////////////////////////////////////////////////////////////////////////////////
	// 物理情報の管理,シミュレーションの管理と実行
	scheduleSimulationFromHaptic();

	// 処理に必要な情報を得る
	HapticInfo* info = updateInfo();

	// 登録された接触があっても、現在本当に接触しているかはわからないのでフラグを用意
	bool feedback = false;

	Vec3d VCForce, VCTorque;

	////////////////////////////////////////////////////////////////////////////////////////
	// 局所的な動力学計算
	if(bLocalDynamics)
	{
		calcLocalDynamics(info, &VCForce, &VCTorque, &feedback);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 純粋なバーチャルカップリング
	// 剛体に接触力を加える
	else
	{
		VirtualCoupling(info, &VCForce, &VCTorque, &feedback);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// ユーザへの力の提示
	feedbackForce(feedback, VCForce, VCTorque);
}

// 接触（めり込み）のチェックをする
void CheckPenetrations(HapticInfo* info, int* num_cols, vector<Vec3d>* r_vectors, vector<double>* c_vectors, vector<Vec3d>* n_vectors)
{
	for(int i = 0; i < info->num_collisions; ++i)
	{
		// めり込み解消処理の準備
		Vec3d col_vector = info->col_positions[i] - info->pointer_col_positions[i];
		double vector_coeff = dot(col_vector, info->col_normals[i]);

		// めり込んでいたら補正計算に使うデータを追加
		if(vector_coeff > 0 && info->bCollide[i])
		{
			r_vectors->push_back(Vec3d(info->pointer_col_positions[i] - pointer_pos));
			c_vectors->push_back(vector_coeff);
			n_vectors->push_back(info->col_normals[i].unit());

			// 重みを格納
			info->weight[i] = vector_coeff;

			// めり込んでた接触の個数
			++(*num_cols);
		}
		else info->weight[i] = 0;
	}
}

// めり込みを補正するのに必要な２つのベクトルを作成する
void CalcTTheta(HapticInfo* info, Vec3d* T, Vec3d* Theta)
{
	int num_cols = 0;
	vector<Vec3d> r_vectors;
	vector<double> c_vectors;
	vector<Vec3d> n_vectors;

	// めり込みをチェックして行列の作成に必要なデータを作成する
	CheckPenetrations(info, &num_cols, &r_vectors, &c_vectors, &n_vectors);

	// めり込みの数の最大値を接触許容数-6にする
	// これは6行分はSPIDARの位置を反映させるのに使われるため
	if(num_cols > NUM_COLLISIONS - 6) num_cols = NUM_COLLISIONS - 6;

	MatrixN6d M = MatrixN6d();
	VecNd C = VecNd();

	// 条件を連立した行列を作成する
	MakePointerPosMatrix(num_cols, r_vectors, c_vectors, n_vectors, &M, &C);

	// 擬似逆行列を計算
	Matrix6Nd Pinv;
	CalcPinv(M, &Pinv);

	// 並進と回転のベクトル
	// 第一～三要素が回転ベクトルのxyz
	// 第四～六要素が並進ベクトルのxyz
	Vec6d Ans = Pinv * C;
	*Theta = Ans.sub_vector(TSubVectorDim<0,3>());
	*T = Ans.sub_vector(TSubVectorDim<3,3>());
}

// めり込みを連立方程式にして行列を作成する関数
void MakePointerPosMatrix(int num_cols, vector<Vec3d> r_vectors, vector<double> c_vectors, vector<Vec3d> n_vectors, MatrixN6d* M, VecNd* C)
{
	// めり込みにかける重み
	double P;

	// SPIDARの位置・姿勢にかける重み
	double MX, MY, MZ;
	double RX, RY, RZ;

	// 接触の状況からパラメータを決める
	GetParametersOfCollision(&MX, &MY, &MZ, &RX, &RY, &RZ, &P);

	// 擬似逆行列で座標計算も兼ねる
	Vec3d t = spidar_pos - pointer_pos;
	Vec3d th = (spidar_ori * pointer_ori.Inv()).Rotation();

	for(int i = 0; i < 6; ++i)
	{
		switch(i)
		{
		case 0:
			C->item(i) = RX * th.x;
			break;
		case 1:
			C->item(i) = RY * th.y;
			break;
		case 2:
			C->item(i) = RZ * th.z;
			break;
		case 3:
			C->item(i) = MX * t.x;
			break;
		case 4:
			C->item(i) = MY * t.y;
			break;
		case 5:
			C->item(i) = MZ * t.z;
			break;
		}

		// 基本的にtとth(SPIDARの位置・姿勢)で決まるようにする
		// 接触がある場合はこの行列にさらに情報を追加し、
		// 擬似逆行列を解くことによってめり込みも考慮した位置を決める
		for(int j = 0; j < 6; ++j)
		{
			if(i == j)
			{
				switch(i)
				{
				case 0:
					M->item(i, j) = RX;
					break;
				case 1:
					M->item(i, j) = RY;
					break;
				case 2:
					M->item(i, j) = RZ;
					break;
				case 3:
					M->item(i, j) = MX;
					break;
				case 4:
					M->item(i, j) = MY;
					break;
				case 5:
					M->item(i, j) = MZ;
					break;
				}
			}
			else M->item(i, j) = 0;
		}
	}

	// 擬似逆行列の計算に必要な
	// 行列とベクトルを作っていく
	for(int k = 6; k < NUM_COLLISIONS; ++k)
	{				
		if(k < num_cols + 6)
		{
			Vec3d n = P * n_vectors[k-6];
			double c = P * c_vectors[k-6];
			Vec3d r = P * r_vectors[k-6];

			C->item(k) = c;

			for(int j = 0; j < 6; ++j)
			{
				switch(j)
				{
				case 0:
					M->item(k, j) = r.y * n.z - r.z * n.y;
					break;
				case 1:
					M->item(k, j) = r.z * n.x - r.x * n.z;
					break;
				case 2:
					M->item(k, j) = r.z * n.y - r.y * n.x;
					break;
				case 3:
					M->item(k, j) = n.x;
					break;
				case 4:
					M->item(k, j) = n.y;
					break;
				case 5:
					M->item(k, j) = n.z;
					break;
				}
			}			
		}
		else
		{
			// データがない部分の初期化
			C->item(k) = 0;

			for(int j = 0; j < 6; ++j)
			{
				M->item(k, j) = 0;
			}
		}
	}
}

// ポインタの位置計算につかう重みを返す関数
inline void GetParametersOfCollision(double* MX, double* MY, double* MZ, double* RX, double* RY, double* RZ, double *P)
{
	// とりあえずこの値で安定
	// MX, MY, MZを小さくしすぎるとダンパーが強くなってしまう
	// 大きくしすぎると重い可動物体にめり込んでいってしまう
#ifdef EXP2
	*MX = *MY = *MZ = 0.0045;
#else
	*MX = *MY = *MZ = 0.008;
#endif
	*RX = *RY = *RZ = 1.0;
	*P = 1.0;
}

// めり込みを補正する関数
void CorrectPenetration(HapticInfo* info)
{
	// ポインタのめり込みに対して擬似逆行列を計算し、
	// 適切な補正量を求めることで、めり込みを補正していく
	// ポインタ自身を並進・回転させてめり込みを解消させるようにする
	Vec3d M_vec, R_vec;
	CalcTTheta(info, &M_vec, &R_vec);

	// ポインタ自身の位置・姿勢を移動
	pointer_pos += M_vec;
	pointer_ori = Quaterniond::Rot(R_vec) * pointer_ori;

	// すべての接触もその方向に移動・回転
	for(int i = 0; i < info->num_collisions; ++i)
	{
		info->pointer_col_positions[i] += M_vec;
		info->pointer_col_positions[i] += R_vec ^ (info->pointer_col_positions[i] - pointer_pos);
	}
}

// 剛体の速度から移動距離を計算して位置を更新する関数
void UpdateSolids(HapticInfo* info)
{
#ifdef EXP1
	bool bWriteData = false;
#endif

	for(int i = 0; i < info->num_solids; ++i)
	{
		if(bSurroundEffect)
		{
			// 周囲の影響のうちで、定数項を徐々に加えていく
			// IsDynamical == falseの場合はそれぞれ初期化してあるので条件分岐は必要ない
			info->solid_velocity[i] += info->vel_constant[i] * info->SHratio;
			info->solid_angular_velocity[i] += info->ang_constant[i] * info->SHratio;
		}
		// 局所的な動力学計算＋周囲の影響なしの場合
		// 重力が適応されている物体には重力を加える
		else if(info->bGravity[i])
		{
			info->solid_velocity[i] += scene->GetGravity() * info->Hdt;
		}

#ifdef EXP1
		// 速度を計算し出力
		if(bOutput) 
		{
			if(info->nearest_solids[i] == DCAST(PHSolid, soObject[0]))
			{
				Buffer[num_data++] = info->solid_velocity[i];
				bWriteData = true;
			}
		}
#endif

		// 剛体の速度による移動・回転
		Vec3d dx_local = info->solid_velocity[i] * info->Hdt;
		info->solid_center_positions[i] += dx_local;
		
		Vec3d dth_local = info->solid_angular_velocity[i] * info->Hdt;
		info->solid_orientations[i] = Quaterniond::Rot(dth_local) * info->solid_orientations[i];

		// 剛体上で起こった接触を検索する		
		for(int j = 0; j < info->num_col_per_sol[i]; ++j)
		{
			// この剛体上にある接触を剛体の移動に伴って更新する
			// 一覧表から接触の添え字を検索
			int col_index = info->SolToCol[i][j];

			info->col_positions[col_index] += dx_local;
			Vec3d dx_rotation = dth_local ^ (info->col_positions[col_index] - info->solid_center_positions[i]);
			info->col_positions[col_index] += dx_rotation;
			info->col_normals[col_index] += dth_local ^ info->col_normals[col_index];
			info->col_normals[col_index] = info->col_normals[col_index].unit();

//			info->wldTosld[col_index] = Quaterniond::Rot(dth_local) * info->wldTosld[col_index];
		}
	}

#ifdef EXP1
	if(!bWriteData && bOutput && !bSimulation) 
	{
		Buffer[num_data++] = soObject[0]->GetVelocity();
	}
#endif
}

// 力積から速度を更新する関数
void UpdateVelocityByCollision(HapticInfo* info, Vec3d VCForce, bool* feedback)
{
	for(int i = 0; i < info->num_collisions; ++i)
	{
		// ２接触点の距離を調べる
		double distance = calcDistance(info->col_positions[i], info->pointer_col_positions[i]);
		// 剛体の添え字
		int sol_index = info->ColToSol[i];

		// 面の法線と、ポインタ上の点から剛体上の点までを結んだベクトルの内積を計算
		// これが０以上なら（ゼロベクトルも含む。ちょうど接している）接触がある
		// また、このままだと仮想壁が裏を向いたときも力を発生させてしまうので、
		// さらに２つの接触点間の距離も導入する
		// 距離がある程度以上大きくなると接触していないことにする
		if(dot(info->col_positions[i] - info->pointer_col_positions[i], info->col_normals[i]) >= 0 && 
			((info->nearest_solids[sol_index]->IsDynamical() && distance < DISTANCE_LIMIT)
			|| !info->nearest_solids[sol_index]->IsDynamical()))
		{
			// とにかく接触しているのでフラグをONにする
			info->bCollide[i] = true;

			// 力を加えてよい対象かチェック
			// 力の向きと剛体の接する側のチェック
			if(dot(info->col_normals[i], -VCForce) >= 0)
			{
				// ユーザに力覚を提示
				*feedback = true;
		
				// 外力で動く剛体であれば速度が変化するはずなので加速度を計算して速度を更新する
				if(info->nearest_solids[sol_index]->IsDynamical())
				{
					// 周囲の影響を計算するときに使うテスト力を更新
					LastVCForce = VCForce;

					// 局所的な動力学計算のみ
					if(!bSurroundEffect)
					{
						// 運動方程式に従って加速度を計算する
						Vec3d addF = FORCE_COEFF * info->weight[i] * VCForce;
						Vec3d accel = info->solid_massinvs[sol_index] * addF;
						Vec3d ang_accel = info->solid_inertiainvs[sol_index] * ((info->col_positions[i] - info->solid_center_positions[sol_index]) ^ addF);

						// 得られた加速度で速度を更新
						info->solid_velocity[sol_index] += accel * info->Hdt;

						// 角速度を更新
						info->solid_angular_velocity[sol_index] += ang_accel * info->Hdt;
					}
					// 局所的な動力学計算＋周囲の影響を考慮
					else
					{
						// 剛体に加える力を計算する
						// ratioをかけるのは、
						// 行列自体がすでにSIMULATION_FREQのdtがかけられており
						// それをHAPTIC_FREQのdtに直すため
						// また、この行列にはすでにmassinv, inertiainvもかけられているので
						// 力をかければすべて計算される
						// 回転に関しては、
						// （解釈の仕方として）トルクの計算式r x Fのうちの
						// 外積が行列とベクトルの積としてあらわされ、
						// その行列がすでに係数行列内に展開されているので、
						// 力のみをかければよいことになる
//						Vec3d q_f = info->wldTosld[i] * info->solid_orientations[sol_index].Conjugated() * /*info->weight[i] */ FORCE_COEFF * VCForce * ratio;
						Vec3d q_f = info->weight[i] * VCForce * info->SHratio;
				
						// 接触点に加えた力の総和を更新
						info->ForceSum[i] += q_f;

						// この力が及ぼす影響をすべての剛体について計算する
						for(int j = 0; j < info->num_solids; ++j)
						{
							if(!info->nearest_solids[j]->IsDynamical()) continue;

							// 行列を用いて加速度を計算して速度を更新
							info->solid_velocity[j] += info->vel_effect[j][i] * q_f;
							info->solid_angular_velocity[j] += info->ang_effect[j][i] * q_f;
						}
					}
				}
			}
		}
		else{ info->bCollide[i] = false; }
	}
}

// 接触による剛体の更新
inline void UpdateSolidByCollision(HapticInfo *info, Vec3d VCForce, bool* feedback)
{
	// 接触力による速度の更新
	UpdateVelocityByCollision(info, VCForce, feedback);

	// ポインタに接しているすべての剛体の情報を更新する
	UpdateSolids(info);
}

// 力覚レンダリングで物理情報を切り替えるときに
// 違和感を生じさせないように誤差を修正する処理をする関数
void UpdateNewInfoFromCurrent()
{
	HapticInfo* info_next = NULL;
	HapticInfo* info_cur = NULL;

	if(current_valid_data)
	{
		info_cur = &info1;
		info_next = &info2;
	}
	else
	{
		info_cur = &info2;
		info_next = &info1;
	}

	// 局所的な動力学計算を行う場合
	// 現在の値(info_cur)を信じる
	if(bLocalDynamics)
	{
		// 剛体の更新
		for(int i = 0; i< info_next->num_solids; ++i)
		{
			// 次回の剛体から今回の剛体の添え字を検索
			int prev_sol_index = info_next->NewsolToSol[i];

			// 見つからなければ次にすすむ
			// 今回新しく剛体が加わった場合はありえる
			if(prev_sol_index == -1) continue;

			info_next->solid_center_positions[i] = info_cur->solid_center_positions[prev_sol_index];
			info_next->solid_orientations[i] = info_cur->solid_orientations[prev_sol_index];
			info_next->solid_velocity[i] = info_cur->solid_velocity[prev_sol_index];
			info_next->solid_angular_velocity[i] = info_cur->solid_angular_velocity[prev_sol_index];
		}

		// 接触の更新
		for(int i = 0; i < info_next->num_collisions; ++i)
		{
			int index = info_next->NewcolToCol[i];

			// 前の接触を引き継ぐ
			// 前の情報を信じる
			if(index != -1)
			{
				info_next->pointer_col_positions[i] = info_cur->pointer_col_positions[index];
				info_next->col_positions[i] = info_cur->col_positions[index];
				info_next->col_normals[i] = info_cur->col_normals[index].unit();
			}
			// 新しい接触
			// ポインタの位置のみ更新する
			else
			{
				Vec3d spidar_pos_error = pointer_pos - info_next->pointer_pos[i];
				Quaterniond spidar_ori_error = pointer_ori * info_next->pointer_ori[i].Inv();

				info_next->pointer_col_positions[i] += spidar_pos_error;
				info_next->pointer_col_positions[i] += spidar_ori_error.Rotation() ^ (info_next->pointer_col_positions[i] - spidar_pos);
			}
		}
	}
	// バーチャルカップリングの場合
	// 最新の情報を信じることにする
	else
	{
		// 接触の更新
		for(int i = 0; i < info_next->num_collisions; ++i)
		{
			int index = info_next->NewcolToCol[i];

			// ポインタは最新の情報を使う
			Vec3d ppos_error = pointer_pos - info_next->pointer_pos[i];
			Quaterniond pori_error = pointer_ori * info_next->pointer_ori[i].Inv();

			info_next->pointer_col_positions[i] += ppos_error;
			info_next->pointer_col_positions[i] += pori_error.Rotation() ^ (info_next->pointer_col_positions[i] - spidar_pos);

			// 新しい接触の場合はこれ以降更新する必要なし
			if(!info_next->bPreviousCollide[i]) continue;

			// 接触の情報を最新の値に書き換える
			int sol_index = info_next->ColToSol[i];
			int prev_sol_index = info_cur->ColToSol[index];

			Vec3d pos_error = info_next->solid_center_positions[sol_index] - info_cur->solid_center_positions[prev_sol_index];
			Quaterniond ori_error = info_next->solid_orientations[sol_index] * info_cur->solid_orientations[prev_sol_index].Inv();

			info_next->col_positions[i] += pos_error;
			info_next->col_positions[i] += ori_error.Rotation() ^ (info_next->col_positions[i] - info_next->solid_center_positions[sol_index]);

			info_next->col_normals[i] += ori_error.Rotation() ^ info_next->col_normals[i];
			info_next->col_normals[i] = info_next->col_normals[i].unit();
		}
	}

	// めり込み状況の更新
	updatePenetrations(info_next);
}

// 普通のバーチャルカップリング
void VirtualCoupling(HapticInfo* info, Vec3d *VCForce, Vec3d *VCTorque, bool* feedback)
{
	// めり込みの補正処理を行う
	CorrectPenetration(info);

	// 接触剛体の接触力による速度・角速度更新
	calcForceBySpringDumper(VCForce, VCTorque);

	Vec3d addF = FORCE_COEFF * (*VCForce) * info->SHratio;
	// 接触があれば接触している剛体に提示力の逆方向のベクトルを加える
	// 剛体の挙動は物理シミュレーションに任せる
	for(int i = 0; i < info->num_collisions; ++i)
	{
		// 力を加える向きと面の法線が90度以上開いてない場合（向き合っていない場合）
		// 力を加えない。こうしないと接触を離す方向に動かしたときに粘っこく感じる
		if(!(dot(*VCForce, info->col_normals[i]) < 0)) continue;

		// 力覚フィードバックON
		*feedback = true;

		// ratioをかける理由は、
		// シミュレーションが加えた力の総量を平均してシミュレーションの周期で力積にするため
		// また、一つの剛体に存在する接触点の数で割って、均等に力を加えるようにする
		int sol_index = info->ColToSol[i];
		info->nearest_solids[sol_index]->AddForce(addF / info->num_col_per_sol[sol_index], info->col_positions[i]);
	}

#ifdef EXP1
	// 速度を計算し出力
	if(bOutput) 
	{
		bool bWriteData = false;
		for(int i = 0; i < info->num_solids; ++i)
		{
			if(info->nearest_solids[i] == DCAST(PHSolid, soObject[0]))
			{
				Buffer[num_data++] = info->solid_velocity[i];
				bWriteData = true;
			}
		}
		if(!bWriteData)
		{
			if(!bSimulation)Buffer[num_data++] = soObject[0]->GetVelocity();
		}
	}
#endif
}

// 力覚レンダリングとシミュレーションを計画的に実行する関数
inline void scheduleSimulationFromHaptic()
{
	// シミュレーションから得られる情報を格納する変数
	HapticInfo* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data) info = &info1;
	else info = &info2;

	// シミュレーションを管理するカウンターを作成
	static int step_counter = 0;	
	// カウンターの値が条件を満たすかチェック
	if(step_counter+1 >= (int)info->HSratio && !bSimulation)
	{
		// 現在使用しているデータから次に使用するデータに切り替える際に
		// 違和感が生じないように値をそろえる処理
		UpdateNewInfoFromCurrent();

		// 周囲の影響を計算するときにつかう単位力の計算
		// 加えた力の平均を求める
		calcTestForce();

		// 使用するデータを変更し、シミュレーションを行う
		step_counter = 0;
		current_valid_data = !current_valid_data;
		bSimulation = true;

#ifdef EXP1
		StepSimulation();
#endif
	}
	else ++step_counter;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//　関連があり中立性な処理

// 提案手法によって生じた剛体の位置の誤差を修正する関数
void ErrorCorrection()
{
	// 局所的な動力学を行わない場合は
	// 誤差が生じないはずなのでそのままリターン
	if(!bLocalDynamics) return;

	// 状態によって参照先を切り替える
	HapticInfo* info = NULL;
	if(!current_valid_data) info = &info1;
	else info = &info2;

//	Vec3d gravity = scene->GetGravity() * info->Sdt;

	// 力覚レンダリングからシミュレーションへの局所的動力学計算の結果反映処理
	// ポインタに接するすべての剛体について行う
	for(int i = 0; i < info->num_solids; ++i)
	{
		// 誤差の分平行移動・回転する
		Vec3d pos_error = info->solid_center_positions[i] - info->nearest_solids[i]->GetCenterPosition();
		Quaterniond ori_error = info->solid_orientations[i] * info->nearest_solids[i]->GetOrientation().Inv();

		info->nearest_solids[i]->SetFramePosition(info->nearest_solids[i]->GetFramePosition() + pos_error);
		info->nearest_solids[i]->SetOrientation(ori_error * info->nearest_solids[i]->GetOrientation());
		info->nearest_solids[i]->SetCenterPosition(info->solid_center_positions[i]);
		info->nearest_solids[i]->SetVelocity(info->solid_velocity[i]);
		info->nearest_solids[i]->SetAngularVelocity(info->solid_angular_velocity[i]);

		// この修正によってシミュレーションされたのでupdateをtrueにする
		info->nearest_solids[i]->SetUpdated(true);

		// もし局所的な動力学計算を行っていた場合
		// HapticRendering内で重力計算を行っている
		// さらにシミュレーションで重力計算を行うと二重で重力を追加してしまうので
		// GravityEngineが重力を加えることを見越して
		// その加える分を先に引いて相殺させる
		// 直接重力をON/OFFさせてもよいが、その操作だけでコストがかかってしまうので
		// とりあえずこのようにした
		/*
		if(info->bGravity[i])
		{
			if(bSurroundEffect)
			{
				info->nearest_solids[i]->SetVelocity(info->nearest_solids[i]->GetVelocity()
													- info->gravity_effect[i].v() * scene->GetTimeStep());
				info->nearest_solids[i]->SetAngularVelocity(info->nearest_solids[i]->GetAngularVelocity()
													- info->gravity_effect[i].w() * scene->GetTimeStep());
			}
			else
			{
				info->nearest_solids[i]->SetVelocity(info->nearest_solids[i]->GetVelocity() 
													- gravity * info->nearest_solids[i]->GetMass());
			}
		}
		*/
	}

	// 周囲の制約を使用している場合は
	// 周囲の剛体も更新する
	if(bSurroundEffect)
	{
		// 直接ポインタに接していないが影響を受ける剛体の更新
		int local_counter = -1;
		for(set<PHSolid *>::iterator it = info->relative_solids.begin(); it != info->relative_solids.end(); ++it)
		{			
			// 次の剛体にすすむ
			// 初期値-1なので最初は0になる
			// ここでincrementしないとcontinueしたときincrementされなくなる
			++local_counter;

			// 動かない剛体は動かない
			if(!(*it)->IsDynamical()) continue;

/*
			bool bNearest = false;
			for(int i = 0; i < info->num_solids; ++i)
			{
				if((*it) == info->nearest_solids[i])
				{
					bNearest = true;
					break;
				}
			}
			// この剛体はポインタに直に接しているので、
			// ここでは更新しない
			if(bNearest) {ofs << "error " << endl; continue;}
*/

			// まずは速度を更新
			// 定数分を加算
			SpatialVector d_accel = info->relative_solids_velocity[local_counter].constant_effect;

			// ポインタと剛体の接触から、間接的に受ける影響を計算
			for(int i = 0; i < info->num_collisions; ++i)
			{
				// 動く剛体を押したときのみ,その剛体を介して周囲の剛体が動く
				if(info->nearest_solids[info->ColToSol[i]]->IsDynamical())
				{
					d_accel += SpatialVector(info->relative_solids_velocity[local_counter].surround_effect[i].first * info->ForceSum[i],
											 info->relative_solids_velocity[local_counter].surround_effect[i].second * info->ForceSum[i]);
				}
			}

			// 元々ForceSumでratioにしているところをHaptic dtにするために
			// ratio = haptic dt / sim dt より ratio * sim dt をする
			// つまり d_accel * sim dt
			d_accel = d_accel * info->Sdt;

			// この加速度による移動量の計算
			Vec3d d_pos = (*it)->GetVelocity()        * info->Sdt + d_accel.v() * info->Sdt / 2.0;
			Vec3d d_ori = (*it)->GetAngularVelocity() * info->Sdt + d_accel.w() * info->Sdt / 2.0;

			// 速度の更新
			(*it)->SetVelocity((*it)->GetVelocity() + d_accel.v());
			(*it)->SetAngularVelocity((*it)->GetAngularVelocity() + d_accel.w());

			// 位置の更新
			(*it)->SetCenterPosition((*it)->GetCenterPosition() + d_pos);
			(*it)->SetOrientation(Quaterniond::Rot(d_ori) * (*it)->GetOrientation());

			// 更新完了
			(*it)->SetUpdated(true);
		}
	}
}

// 物理シミュレーションを一歩すすめる関数
void StepSimulation()
{
#ifdef EXP3
	t_start();
#endif

	scene->ClearForce();

	// ポインタの位置を更新
	UpdatePointer();

	// 局所的動力学計算によって生じた差を修正する
	ErrorCorrection();

	// シミュレーション周波数に変更があれば更新を行う関数
	updateSimulationFrequency();

	// シミュレーションの具体的な処理
	scene->GenerateForce();
	scene->Integrate();

	// 衝突点情報を表示
//	show_collision_info();

	// 衝突点から周囲の影響を計算
	// １を参照中。２を更新 
	if(current_valid_data) CalcSurroundEffect(&info2, &info1);
	// ２を参照中。１を更新
	else CalcSurroundEffect(&info1, &info2);

	// シミュレーションが終わったのでフラグを下げる
	bSimulation = false;

#ifdef EXP3
	int time = t_end();
	if(time > 1000000 / (double)SIMULATION_FREQ)
	{
		cout << "collapsed time = " << (double)time / 1000000 << " num object = " << (int)soObject.size() << endl;
//		ofs << "collapsed time = " << (double)time / 1000000 << " num object = " << (int)soObject.size() << endl;
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 関連はあるが本質ではない処理の関数
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){

	ofs.open("log.txt");
	ofs.precision(15);

#if defined EXP1
	Buffer.resize(10000, Vec3d());
	num_data = 0;
#endif

	// object, solidなどの初期化
	InitScene();

	// rendering関係の初期化
	InitRendering(&argc, argv);

	// device managerの初期化
	InitDeviceManager();

	// deviceの初期化
	spidarG6.Init(devMan, false);			//	SPIDARの初期化，キャリブレーションもする．

	timeBeginPeriod(1);							//分解能の最小値

	DWORD time = 0;

	info1.num_collisions = 0;
	info2.num_collisions = 0;

#ifdef EXP1
	StepSimulation();

	keyboard('2', 0, 0);
	keyboard('u', 0, 0);
	
	while(1)
	{
		HapticRendering();
	}
#else
	// hapticスレッドの生成・開始
	FTimerId1 = timeSetEvent(1000 / HAPTIC_FREQ,    // タイマー間隔[ms]
	                        1,   // 時間分解能
		                    HapticRendering,//コールバック関数
		                    (DWORD)&time,//ユーザー定義の値
		                    TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
							);

	// openGL main loop
	glutMainLoop();
#endif

}
/**
 brief		VR内のプロキシの位置を更新する関数。
			この関数と力覚レンダリングを組み合わせることでポインタの更新をHAPTIC_FREQの周波数で更新できる。
 param		なし
 return		なし
 */
void UpdatePointer()
{
	double dt = scene->GetTimeStep();
	pointer_vel = (soPointer->GetFramePosition() - pointer_pos) / dt;
	pointer_angvel = (soPointer->GetOrientation() * pointer_ori.Inv()).Rotation() / dt;

	soPointer->SetFramePosition(pointer_pos);
	soPointer->SetVelocity(Vec3d());
	soPointer->SetOrientation(pointer_ori);
	soPointer->SetAngularVelocity(Vec3d());
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	if (key == ESC) 
	{
		// HapticRenderingスレッドを削除
		timeKillEvent(FTimerId1);
		timeEndPeriod(1);

		// 自作の接触点を削除
		info1.points.clear();
		info2.points.clear();

		cout << "EXIT from Key Input" << endl;
		exit(0);
	}
	// ポインタのキャリブレーション
	else if(key == 'c')
	{
		spidarG6.Calib();
	}
	// toggle force output
	else if(key == ' ')
	{
		bForce = !bForce;

		// メッセージを出力
		cout << "Feedback Force ";
		if(bForce) cout << "ON" << endl;
		else cout << "OFF" << endl;
	}
	// objectの場所をリセット
	else if(key == 'r')
	{
		for(int i = 0; i < (int)soObject.size(); ++i)
		{
			if(!soObject[i]->IsDynamical()) continue;

			soObject[i]->SetPose(Posed());

#if defined EXP1
			soObject[i]->SetFramePosition(Vec3f(1.55 * (i + 1), 0.0, 0.0));
#elif defined EXP2
			if(!bTest)
			{
				if(i < 2) soObject[i]->SetFramePosition(Vec3f(1.55 * (i + 1), 0.0, 0.0));
				else soObject[i]->SetFramePosition(Vec3f(1.55 * (i - 1), 0.0, 2.0));
			}
			else
			{
				if(i < 2) soObject[i]->SetFramePosition(Vec3f(1.55 * (i + 1), 0.0, 2.0));
				else soObject[i]->SetFramePosition(Vec3f(1.55 * (i - 1), 0.0, 0.0));
			}
#elif defined EXP4
			soObject[i]->SetFramePosition(Vec3f(1.6 * (i+1) - 3.0, 0.0, 0.0));
#elif defined Demo
			if(i < 15) soObject[i]->SetFramePosition(Vec3f(-2.0 + (i % 3) * 2.0, 3.0, (i / 3) * 2.0));
			else if(i < 30)soObject[i]->SetFramePosition(Vec3f(-2.0 + (i % 3) * 2.0, 5.0, (i / 3) * 2.0 - 10));
			else soObject[i]->SetFramePosition(Vec3f(-2.0 + (i % 3) * 2.0, 7.0, (i / 3) * 2.0 - 20));
#else 
			soObject[i]->SetFramePosition(Vec3f(0, 0.5f*(i+1), 0));
#endif

			soObject[i]->SetAngularVelocity(Vec3d());
			soObject[i]->SetVelocity(Vec3d());
		}
	}
	// SPIDARの位置と原点をあらわす球の表示をON/OFFする
	else if(key == 'd')
	{
		bDisplayPointer = !bDisplayPointer;
		cout << "Pointer Display ";
		if(!bDisplayPointer)cout << "OFF " << endl;
		else cout << "ON " << endl;
	}
	// 新しい剛体を作成する
	else if(key == 'g')
	{
		PHSolidDesc desc;
#ifdef EXP4
		desc.mass = 0.2f;
#elif defined Demo
		desc.mass = 0.3f;
#else
		desc.mass = 1.0f;
#endif
		desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);
		soObject.push_back(scene->CreateSolid(desc));
			
		CDBoxIf* box = NULL;
		CDBoxDesc bd;
		bd.boxsize = Vec3f(1.5f, 1.5f, 1.5f);
		box = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soObject[soObject.size() - 1]->AddShape(box);
		soObject[soObject.size() - 1]->SetFramePosition(Vec3d(0, soObject.size() * 1.1, 0));

		cout << "#object = " << (int)soObject.size() << endl;
	}
	else if(key == 'a')
	{
		bDisplayCollision = !bDisplayCollision;

		cout << "Collision Display ";
		if(bDisplayCollision) cout << "ON" << endl;
		else cout << "OFF" << endl;
	}
	// 設定のプレセット
	else if(key == '1')
	{
		// シミュレーションの周波数を60Hzにする
		SIMULATION_FREQ_BUFFER = 60;

		bSurroundEffect = true;
		bLocalDynamics = true;
		bPredictCollision = true;

#if !defined EXP2
		cout << "PRESET 1: Local Dynamics With Surrounding Effect and Predict Collision" << endl;
		keyboard('f', 0, 0);
#else
		cout << "PRESET 1" << endl;
		ofs << "PRESET 1" << endl;
#endif
	}
	else if(key == '2')
	{
		// シミュレーションの周波数を60Hzにする
		SIMULATION_FREQ_BUFFER = 60;

		bSurroundEffect = true;
		bLocalDynamics = true;
		bPredictCollision = false;
#if !defined EXP2
		cout << "PRESET 2: Local Dynamics With Surrounding Effect" << endl;
		keyboard('f', 0, 0);
#else
		cout << "PRESET 2" << endl;
		ofs << "PRESET 2" << endl;
#endif
	}
	else if(key == '3')
	{
		// シミュレーションの周波数を60Hzにする
		SIMULATION_FREQ_BUFFER = 60;

		bLocalDynamics = true;
		bPredictCollision = true;
		bSurroundEffect = false;
#if !defined EXP2
		cout << "PRESET 3: Local Dynamics With Predict Collision" << endl;
		keyboard('f', 0, 0);
#else 
		cout << "PRESET 3" << endl;
		ofs << "PRESET 3" << endl;
#endif
	}
	else if(key == '4')
	{
		// シミュレーションの周波数を60Hzにする
		SIMULATION_FREQ_BUFFER = 60;

		bSurroundEffect = false;
		bLocalDynamics = true;
		bPredictCollision = false;
#if !defined EXP2
		cout << "PRESET 4: Local Dynamics" << endl;
		keyboard('f', 0, 0);
#else
		cout << "PRESET 4" << endl;
		ofs << "PRESET 4" << endl;
#endif
	}
	else if(key == '5')
	{
		// シミュレーションの周波数を60Hzにする
		SIMULATION_FREQ_BUFFER = 60;

		bSurroundEffect = false;
		bLocalDynamics = false;
		bPredictCollision = false;
#if !defined EXP2
		cout << "PRESET 5: Virtual Coupling" << endl;
		keyboard('f', 0, 0);
#else
		cout << "PRESET 5" << endl;
		ofs << "PRESET 5" << endl;
#endif
	}
	else if(key == '6')
	{
		// シミュレーションの周波数を500Hzにする
		SIMULATION_FREQ_BUFFER = 500;

		// 高周波数バーチャルカップリング
		bSurroundEffect = false;
		bLocalDynamics = false;
		bPredictCollision = false;
#if !defined EXP2
		cout << "PRESET 6: High Definition Virtual Coupling" << endl;
		keyboard('f', 0, 0);
#else
		cout << "PRESET 6" << endl;
		ofs << "PRESET 6" << endl;
#endif
	}
#if !defined Demo
	// 周囲の影響を有効にするかどうか
	else if(key == 's')
	{
		bSurroundEffect = !bSurroundEffect;

		// メッセージを出力
		cout << "Surrounding Effect ";
		if(bSurroundEffect)
		{
			cout << "ON" << endl;

			if(!bLocalDynamics)
			{
				bLocalDynamics = true;
				cout << "Local Dynamics ON" << endl;
			}
		}
		else cout << "OFF" << endl;
	}
	// デバッグ出力の許可・不可の切り替え
	else if(key == 'o')
	{
		bOutput = !bOutput;
		cout << "File Output is ";
		if(bOutput)cout << "ENABLED" << endl;
		else cout << "DISABLED" << endl;
	}
	// シミュレーションの更新周波数を変更する
	else if(key == 'i')
	{
		if(SIMULATION_FREQ_BUFFER == 60)
		{
			SIMULATION_FREQ_BUFFER = 500;
			cout << "SIMULATION FREQUENCY is changed to 500 Hz" << endl;
		}
		else
		{
			SIMULATION_FREQ_BUFFER = 60;
			cout << "SIMULATION FREQUENCY is changed to 60 Hz" << endl;
		}
	}
	// 現在のバーチャルカップリングのKとBの値を表示する
	else if(key == 'e')
	{
		cout << "Km = " << Km << " Bm = " << Bm << endl;
		cout << "Kr = " << Kr << " Br = " << Br << endl;
	}
	// バーチャルカップリングの係数のKを1増加して現在の状態を表示する
	else if(key == 'k')
	{
		if(bMode)Km += 1;
		else Kr += 1;
		keyboard('e', 0, 0);
	}
	// バーチャルカップリングの係数のBを1増加して現在の状態を表示する
	else if(key == 'b')
	{
		if(bMode)Bm += 1;
		else Br += 1;
		keyboard('e', 0, 0);
	}
	// バーチャルカップリングの係数のKを1減少して現在の状態を表示する
	else if(key == 'j')
	{
		if(bMode)Km -= 1;
		else Kr -= 1;
		keyboard('e', 0, 0);
	}
	// バーチャルカップリングの係数のBを1減少して現在の状態を表示する
	else if(key == 'v')
	{
		if(bMode)Bm -= 1;
		else Br -= 1;
		keyboard('e', 0, 0);
	}
	// 提示力を調節する値を0.1増加する
	else if(key == 'm')
	{
		FORCE_COEFF += 0.01;
		cout << "coeff = " << FORCE_COEFF << endl;
	}
	// 提示力を調節する値を0.1減少する
	else if(key == 'n')
	{
		FORCE_COEFF -= 0.01;
		cout << "coeff = " << FORCE_COEFF << endl;
	}
	// パラメータ編集対象の切り替え処理
	else if(key == 't')
	{
		bMode = !bMode;
		if(bMode)cout << "MOVEMENT ";
		else cout << "ROTATION ";
		cout << "Edit Mode" << endl;
	}
	// 局所的動力学計算のフラグ変更
	else if(key == 'l')
	{
		bLocalDynamics = !bLocalDynamics;
		cout << "Local Dynamics ";
		if(bLocalDynamics)cout << "ON " << endl;
		else
		{
			// OFFになったときは
			// 局所的動力学計算に伴うほかの機能もOFFにする
			cout << "OFF " << endl;

			bSurroundEffect = false;
			cout << "Surround Effect OFF " << endl;

			bPredictCollision = false;
			cout << "Predict Collision OFF" << endl;
		}
	}
	// 接触予想のフラグ変更
	else if(key == 'h')
	{
		bPredictCollision = !bPredictCollision;
	
		cout << "Predict Collision ";
		if(bPredictCollision) cout << "ON" << endl;
		else cout << "OFF" << endl;
	}
	// すべてのフラグのデバッグ表示
	else if(key == 'f')
	{
		cout << "--- Current Status ---" << endl;
		
		cout << "Haptic Frequency : 1000 Hz" << endl;
		cout << "Simulation Frequency : " << SIMULATION_FREQ_BUFFER << " Hz" << endl;

		keyboard('e', 0, 0);
		cout << "coeff = " << FORCE_COEFF << endl;

		cout << "Feedback Force ";
		if(bForce)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "LocalDynamics ";
		if(bLocalDynamics) cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Surrounding Effect ";
		if(bSurroundEffect)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Predict Collision ";
		if(bPredictCollision) cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Pointer Display ";
		if(bDisplayPointer)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Collision Display ";
		if(bDisplayCollision)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "File Output ";
		if(bOutput)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "----------------------" << endl;
	}
#endif
// 実験ごとの機能
#ifdef EXP1
	else if(key == 'u')
	{
		if(!bExperiment) 
		{
			bOutput = true;
			cout << "Experiment start " << endl;
		}
		else
		{
			bOutput = false;
			cout << "suspend" << endl;
		}
		bExperiment = !bExperiment;
	}
#elif defined EXP2
	// 次のセッションに進む
	else if(key == 'y')
	{
		ofs << "session : " << session+1 << " finished!!" << endl;
		ofs << "--- mass = ---" << mass << endl;

		session++;

		if(session == 3)
		{
			cout << "finish experiment " << endl;
			keyboard(ESC, 0, 0);
		}
		else cout << "go to next session" << endl;

		PHSolidDesc desc;

		for(int i = 0; i < 4; ++i)
		{
			switch(i)
			{
			case 0:
				desc.mass = 1.0f;
				break;
			case 1:
				switch(session)
				{
				case 1:
					desc.mass = 2.0f;
					break;
				case 2:
					desc.mass = 3.0f;
					break;
				}
				break;
			case 2:
				desc.mass = 1.0f;
				break;
			case 3:
				desc.mass = 1.0f;
				mass = 1.0f;
				break;
			}

			ofs << "mass of solid " << i << " = " << desc.mass << endl;

			desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);
				
			soObject[i]->SetMass(desc.mass);
			soObject[i]->SetInertia(desc.inertia);
		}

		ofs << "mass = " << mass << endl;
		bTest = false;
		keyboard('r', 0, 0);
	}
	else if(key == 'q')
	{
		mass = mass + 0.1;
		ofs << "mass + 0.1 :" << mass << endl;

		PHSolidDesc desc;
		desc.mass = mass;
		desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);
		soObject[3]->SetMass(desc.mass);
		soObject[3]->SetInertia(desc.inertia);
		cout << "make box heavier" << endl;
	}
	else if(key == 'z')
	{
		if(mass - 0.1 <= 0.0f)
		{
			cout << "weight is less than zero" << endl;
		}
		else
		{
			mass = mass - 0.1;
			ofs << "mass - 0.1 :" << mass << endl;
			PHSolidDesc desc;
			desc.mass = mass;
			desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);
			soObject[3]->SetMass(desc.mass);
			soObject[3]->SetInertia(desc.inertia);
			cout << "make box lighter" << endl;
		}
	}
	/*
	else if(key == 'w')
	{
		mass = mass + 0.5;
		ofs << "mass + 0.5 :" << mass << endl;
		PHSolidDesc desc;
		desc.mass = mass;
		desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);
		soObject[3]->SetMass(desc.mass);
		soObject[3]->SetInertia(desc.inertia);
		cout << "make box much heavier" << endl;
	}
	else if(key == 'x')
	{
		if(mass - 0.5 <= 0.0f)
		{
			cout << "weight is less than zero" << endl;
		}
		else
		{
			mass = mass - 0.5;
			ofs << "mass - 0.5 :" << mass << endl;
			PHSolidDesc desc;
			desc.mass = mass;
			desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);
			soObject[3]->SetMass(desc.mass);
			soObject[3]->SetInertia(desc.inertia);
			cout << "make box much lighter" << endl;
		}
	}
	*/
	else if(key == 'p')
	{
		bTest = !bTest;
		if(bTest) cout << "Weight Adjust Mode" << endl;
		else cout << "Weight Check Mode" << endl;

		keyboard('r', 0, 0);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 本質ではない処理の関数
vector<SpatialVector> CollisionF;
void SaveCollisionF(set<PHConstraint *> relative_consts, set<PHConstraint *> nearest_consts)
{
	// lambdaを初期化
	CollisionF.clear();

	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{
		// ラムダの値(fv, fw)を保存しておく
		CollisionF.push_back((*it)->f);
	}
	for(set<PHConstraint *>::iterator it = nearest_consts.begin(); it != nearest_consts.end(); ++it)
	{
		// ラムダの値(fv, fw)を保存しておく
		CollisionF.push_back((*it)->f);
	}	
}

vector<SpatialVector> SolidDv;
void SaveSolidDv(set<PHSolid*> relative_solids, set<PHSolid*> nearest_solids)
{
	SolidDv.clear();

	for(set<PHSolid*>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		SolidDv.push_back((*it)->dv);
	}
	for(set<PHSolid*>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		SolidDv.push_back((*it)->dv);
	}
}

map<PHSolid*, SpatialVector> SolidV;
void SaveSolidV(set<PHSolid*> relative_solids)
{
	SolidV.clear();

	for(set<PHSolid*>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		SolidV.insert(pair<PHSolid*, SpatialVector>(*it, (*it)->v));
	}
}

map<PHSolid*, SpatialVector> SolidF;
void SaveSolidF(set<PHSolid*> relative_solids)
{
	SolidF.clear();

	for(set<PHSolid*>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		SolidF.insert(pair<PHSolid*, SpatialVector>(*it, (*it)->f));
	}
}

// 予測シミュレーションで上書きしてしまった変数の値を元に戻す関数
void RestoreParameters(set<PHConstraint*>* relative_consts, set<PHSolid*>* relative_solids, set<PHConstraint*> *nearest_consts, set<PHSolid*> *nearest_solids)
{
	int local_counter = 0;
	// fvとfwの値を元にもどす
	// lambdaの保存の過程でrelative_constsのiteratorでvectorに格納しているはずなので
	// このiteratorでループをまわせば接触がわからなくても検索せずに参照できるはず
	for(set<PHConstraint *>::iterator it = relative_consts->begin(); it != relative_consts->end(); ++it)
	{
		(*it)->f = CollisionF[local_counter++];
	}
	for(set<PHConstraint *>::iterator it = nearest_consts->begin(); it != nearest_consts->end(); ++it)
	{
		(*it)->f = CollisionF[local_counter++];
	}

	local_counter = 0;
	// すべての剛体のdvをもとに戻す
	for(set<PHSolid*>::iterator it = relative_solids->begin(); it != relative_solids->end(); ++it)
	{
		(*it)->dv = SolidDv[local_counter];
/*
		it2 = SolidV.find(*it);
		(*it)->v = (*it2).second;

		it2 = SolidF.find(*it);
		(*it)->f = (*it2).second;
*/
		++local_counter;
	}
	for(set<PHSolid*>::iterator it = nearest_solids->begin(); it != nearest_solids->end(); ++it)
	{
		(*it)->dv = SolidDv[local_counter];
/*
		it2 = SolidV.find(*it);
		(*it)->v = (*it2).second;

		it2 = SolidF.find(*it);
		(*it)->f = (*it2).second;
*/
		++local_counter;
	}
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	// 再描画
	glutPostRedisplay();

#ifndef EXP1
	// simulationを１ステップ進める
	if(bSimulation)StepSimulation();
#endif
}


/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);

	// フラグがONのときSPIDARの軌跡をデバッグ出力
	if(bDisplayPointer) DisplayPointer();

	// 接触点を描画
	if(bDisplayCollision) DisplayCollisions();

	render->EndScene();
	glutSwapBuffers();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, -20.0, 1.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		 なし
 */
void reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(), Vec2f(w,h));
}

// ポインタ固有の初期化処理を行う
void InitPointer(PHSolidIf* pointer)
{
	// pointerの重力を無効にする
	pointer->SetGravity(false);

	// pointerと接触する剛体との解析法による計算をやめ、
	// 力覚で処理するようにする
	AddInactiveSolid(pointer);

	pointer_pos = pointer->GetFramePosition();
	pointer_ori = pointer->GetOrientation();
	pointer_vel = Vec3d();
	pointer_angvel = Vec3d();

	// soPointerのBBoxを膨らませる
	PHBBox* bbox = &(DCAST(PHSolid, pointer)->bbox);
	bbox->SetBBoxCenterExtent(bbox->GetBBoxCenter(), bbox->GetBBoxExtent() * EPSILON);	
}

/**
 brief  	sceneの初期化を行う関数
 param	 	なし
 return 	なし
 */
void InitScene()
{
	phSdk = PHSdkIf::CreateSdk();					// SDKの作成　
	PHSceneDesc sd;
	sd.timeStep = (double)1.0 / (double)SIMULATION_FREQ;
	scene = phSdk->CreateScene(sd);				// シーンの作成
	PHSolidDesc desc;

#ifdef EXP2
	mass = 1.0;
	session = 0;
#endif

	// Solidの作成
	for (unsigned int objectCnt=0; objectCnt<NUM_OBJECTS; ++objectCnt){
		// 質量の設定
		// スケーリングによってMASS_SCALEで割る
		desc.mass = 1.0f / MASS_SCALE;// * (objectCnt + 1);

#ifdef EXP2
//		if(objectCnt == 1) desc.mass = 1.0f;
		if(objectCnt == 3)
		{
//			desc.mass = 1.0f;
			mass = desc.mass;
		}
		ofs << "mass of solid " << objectCnt << " = " << desc.mass << endl;
#elif defined Demo
		desc.mass = 0.3f;
#endif

		// inertiaの計算式
		// 直方体の場合 I = mass * (r1^2 + r2^2) / 12
		// 球の場合 I = mass * r~2 * 2/5
		desc.inertia = Matrix3d(desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0, 0, 0, 0, desc.mass * 1.5 * 1.5 / 6.0);

		soObject.push_back(scene->CreateSolid(desc));		// 剛体をdescに基づいて作成
//		ofs << "solid" << objectCnt << " = " << DCAST(PHSolid, soObject[objectCnt]) << endl;
#if defined EXP1 
		soObject[objectCnt]->SetGravity(false);
		bGravitySet.insert(DCAST(PHSolid, soObject[objectCnt]));
#elif defined EXP2
//		soObject[objectCnt]->SetGravity(false);
//		bGravitySet.insert(DCAST(PHSolid, soObject[objectCnt]));
#endif
	}

	desc.mass = 0.1f;
	desc.inertia = Matrix3d(0.16, 0, 0, 0, 0.16, 0, 0, 0, 0.16);

	for(int i = 0; i < 1; ++i)
	{
		PHSolidIf *soWall = scene->CreateSolid(desc);
		soWall->SetDynamical(false);
		soWalls.push_back(soWall);
		bGravitySet.insert(DCAST(PHSolid, soWalls[i]));
//		ofs << "static solid = " << DCAST(PHSolid, soWall) << endl;
	}
	soPointer = scene->CreateSolid(desc);

	//	形状の作成
	CDBoxIf* floor=NULL;
	CDSphereIf* sphere=NULL;

	{
		/*
		CDSphereDesc sd;
		sd.radius = 1.25f;
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));

		// 球形のポインタ
		soPointer->AddShape(sphere);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));

		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf,phSdk->CreateShape(sd));
		*/

		CDBoxDesc bd;
		for(int i = 0; i < 1; ++i)
		{
			Vec3f position;

			switch(i)
			{
			case 0:
				bd.boxsize = Vec3f(4.0f, 10.0f, 4.0f);
				position = Vec3f(0, -5.2f, 0);
				break;
			case 1:
				bd.boxsize = Vec3f(1.0f, 1.0f, 1.0f);
				position = Vec3f(1.0f, 0.0f, 0.0f);
				break;
			case 2:
				bd.boxsize = Vec3f(1.0f, 1.0f, 1.0f);
				position = Vec3f(-1.0f, 0.0f, 0.0f);
				break;
			case 3:
				bd.boxsize = Vec3f(1.0f, 1.0f, 1.0f);
				position = Vec3f(0.0f, 0.0f, 1.0f);
				break;
			}

			floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
			soWalls[i]->AddShape(floor);
			soWalls[i]->SetFramePosition(position);
//			soFloor->AddShape(floor);
//			soFloor->SetFramePosition(Vec3f(0,-4.0f,0));
		}

		// 四角のポインタ
		bd.boxsize = Vec3f(2 * POINTER_RADIUS, 2 * POINTER_RADIUS, 2 * POINTER_RADIUS);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soPointer->AddShape(floor);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));

		// 通常の剛体用のshape
		bd.boxsize = Vec3f(0.3f, 0.3f, 0.3f);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
	}	

	for (unsigned int sphereCnt=0; sphereCnt<NUM_OBJECTS; ++sphereCnt){
		soObject[sphereCnt]->AddShape(floor);
#if defined EXP1
		soObject[sphereCnt]->SetFramePosition(Vec3f(1.55 * (sphereCnt + 1), 0.0, 0.0));
#elif defined EXP2
		if(sphereCnt < 2)soObject[sphereCnt]->SetFramePosition(Vec3f(1.55 * (sphereCnt + 1), 0.0, 0.0));
		else soObject[sphereCnt]->SetFramePosition(Vec3f(1.55 * (sphereCnt - 1), 0.0, 2.0));
#elif defined EXP4
		soObject[sphereCnt]->SetFramePosition(Vec3f(1.501 * (sphereCnt + 1) - 3.0, 0.0, 0.0));
#else
		soObject[sphereCnt]->SetFramePosition(Vec3f(0, 0.5f*(sphereCnt+1), 2));
#endif
	}
	scene->SetGravity(Vec3f(0,-9.8f * SPIDAR_SCALE, 0));	// 重力を設定

	// ポインタの初期化
	InitPointer(soPointer);
}

/**
 brief  	rendering, windowまわりの初期化を行う関数
 param	 	mainの引数argc, argv
 return 	なし
 */
void InitRendering(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINSIZE_WIDTH, WINSIZE_HEIGHT);

	int window = glutCreateWindow("HapticTest");
	grSdk = GRSdkIf::CreateSdk();
	render = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL();

	// 初期設定
	grDevice->Init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	render->SetDevice(grDevice);	// デバイスの設定

	// 視点を設定する
	Affinef view;
	view.Pos() = Vec3f(0.0, 0.75, -3.0);
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);
	
	setLight();

	/// SPIDAR座標をビュー座標に直す変数
	view_rot = view.inv().Rot() * SPIDAR_SCALE;
	view_haptic = view.Rot();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// ユーティリティ関数

// 与えられた接触で、引数のsolidに接している剛体をしらべ、あれば返す
// また、法線の向きも調べて、１から０に向かうように符号を返す
inline PHSolid* GetAdjacentSolid(PHConstraint* constraint, PHSolid* solid, int* sign)
{
	if(constraint->solid[0] == solid)
	{
		if(sign != NULL) *sign = -1;
		return constraint->solid[1];
	}
	else if(constraint->solid[1] == solid)
	{
		if(sign != NULL) *sign = 1;
		return constraint->solid[0];
	}
	else return NULL;
}

// 与えられた情報から接触を作る関数
PHContactPoint* CreateContactPoint(Vec3d col_normal, Vec3d col_position, PHSolid* solid0, PHSolid* solid1, Spr::PHShapePairForLCP* shapePair)
{
	// Contact Pointを作成する処理
	Matrix3d local;	//	contact coodinate system 接触の座標系
	Vec3d normal = col_normal;
	Vec3d center_pos = col_position;

	Vec3d v0 = solid0->GetPointVelocity(center_pos);
	Vec3d v1 = solid1->GetPointVelocity(center_pos);

	local.Ex() = normal;
	local.Ey() = v1-v0;
	local.Ey() -= local.Ey() * normal * normal;
	if (local.Ey().square() > 1e-6){
		local.Ey().unitize(); 
	}else{
		if (Square(normal.x) < 0.5) local.Ey()= (normal ^ Vec3f(1,0,0)).unit();
		else local.Ey() = (normal ^ Vec3f(0,1,0)).unit();
	}
	local.Ez() =  local.Ex() ^ local.Ey();

	PHContactPoint *point = DBG_NEW PHContactPoint(local, shapePair, col_position, solid0, solid1);
	point->scene = DCAST(PHScene, scene);
	point->engine = scene->GetConstraintEngine();

	if(scene->GetConstraintEngine()->IsInactiveSolid(solid0->Cast())) point->SetInactive(1, false);
	if(scene->GetConstraintEngine()->IsInactiveSolid(solid1->Cast())) point->SetInactive(0, false);

	return point;
}

/**
 brief     	物体の接触点の情報を表示する関数
 param		なし
 return 	なし
*/
void show_collision_info()
{
	// 衝突点を取得
	PHConstraints cp = scene->GetConstraintEngine()->GetContactPoints();//GetContactPoints(scene);

	// 接触点が存在すれば処理する
	if(cp.size() > 0)
	{
		// 衝突を表示するときの個数を数えるカウンター
		static double global_collision_counter = 0;
		int counter = 0;
		cout << "----------NEW COLLISION DATA No." << global_collision_counter++ << "-----------" << endl;
		
		// すべての衝突に対して処理
		for(PHConstraints::iterator it = cp.begin(); it != cp.end(); ++it)
		{
			// ループ内のカウンターを更新
			++counter;
			cout << "COLLISION" << counter << ": " << endl;

			// PHContactPointにdynamic castした処理のチェック
			PHContactPoint* contact = DCAST(PHContactPoint, (*it));
			cout << "collision pos = " << contact->pos << endl;
			cout << endl;

			// PHConstraintの情報の表示
			// solid1
			cout << "solid1.mass = " << (*it)->solid[0]->mass << endl;
			cout << "solid1.inertia = " << endl << (*it)->solid[0]->inertia;
			cout << "solid1.velocity = " << (*it)->solid[0]->GetVelocity()<< endl;
			cout << "solid1.angular_velocity = " << (*it)->solid[0]->GetAngularVelocity() << endl;
			cout << "solid1.position = " << (*it)->solid[0]->GetFramePosition() << endl;
			cout << "vector to working point from center = " << (contact->pos - (*it)->solid[0]->GetCenterPosition()).unit() << endl;

			if(current_valid_data)
			{
				for(int i = 0; i < info1.num_solids; ++i)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = (PHSolidIf *)&(*info1.nearest_solids[i]);
					if(nearest_address == (PHSolidIf *)((*it)->solid[0]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			else 
			{
				for(int i = 0; i < info2.num_solids; ++i)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = (PHSolidIf *)&(*info2.nearest_solids[i]);
					if(nearest_address == (PHSolidIf *)((*it)->solid[0]))cout << " --this solid is nearest object!!" << endl;
				}

			}
			cout << endl;

			// solid2
			cout << "solid2.mass = " << (*it)->solid[1]->mass << endl;
			cout << "solid2.inertia = " << endl << (*it)->solid[1]->inertia;
			cout << "solid2.velocity = " << (*it)->solid[1]->GetVelocity()<< endl;
			cout << "solid2.angular_velocity = " << (*it)->solid[1]->GetAngularVelocity() << endl;
			cout << "solid2.position = " << (*it)->solid[1]->GetFramePosition() << endl;
			cout << "vector to working point from center = " << (contact->pos - (*it)->solid[1]->GetCenterPosition()).unit() << endl;

			if(current_valid_data)
			{
				for(int i = 0; i < info1.num_solids; ++i)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			else
			{
				for(int i = 0; i < info2.num_solids; ++i)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info2.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			cout << endl;
		}
	}
}

// 自作の接触がある場合にそれらを削除する関数
void ResetOriginalContactPoints(multimap<double, PHContactPoint*>* points)
{
	// 2ステップ前に作った接触の削除処理
	for(multimap<double, PHContactPoint*>::iterator it = points->begin(); it != points->end(); ++it)
	{
		PHContactPoint* cp = (*it).second;
		delete cp;
	}
	points->clear();
}

// debug用時間計測タイマー
UTPreciseTimer timer;
static int start;
static int end;

// 処理にかかる時間を計測。一時的なもの
inline void t_start()
{
	start = timer.CountUS();
}

inline int t_end()
{
	// 一周にかかる時間をチェック
	end = timer.CountUS();
	if(end > 0)
	{
		if(bOutput) ofs << "time = " << end << endl;
		return end;
	}
	return 0;
}

// 与えられた行列の擬似逆行列を計算する
void CalcPinv(MatrixN6d M, Matrix6Nd* Pinv)
{
	Matrix6Nd Mt = M.trans();
	Matrix6d MTM = Mt * M;
	Matrix6d m = Matrix6d();

	// 対角要素に値を足してランク落ちを防ぐ
	// 解の精度は落ちるが求まらなくなることはなくなる
	for(int i = 0; i < 6; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			// 値は適当
			if(i == j) m[i][j] = 0.001;
			else m[i][j] = 0;
		}
	}
	MTM += m;

	Matrix6d MTMinv = MTM.inv();

	// 擬似逆行列
	*Pinv = MTMinv * Mt;
}

inline void UpdateSpidarData(Vec3d* SPIDARVel, Quaterniond* SPIDARAngVel)
{
	const double dt = (double)1.0 / HAPTIC_FREQ;

	spidarG6.Update(dt);//	USBの通信を行う．

	// SPIDARの位置の更新
	spidar_pos = view_rot * spidarG6.GetPos();		// SPIDAR座標からワールド座標にする

#ifdef EXP1
	if(bExperiment)
	{
		if(num_data > 1500)
		{
			bExperiment = false;
			cout << "finish" << endl;
			
			// ファイルへのバッファの書き出し
			for(int i = 0; i < num_data; ++i)
			{
				ofs << Buffer[i].x << endl;
			}
			ofs.close();

//			system("notepad C:/home/ohuchi/springhead2/src/tests/HIS/HISpidarG6/Release/log.txt");
			keyboard(ESC, 0, 0);

		}
		spidar_pos = pos_array[0];
		pos_array[0].x += 0.001;
	}
#endif

	// SPIDARの情報を格納する変数
	static Vec3d old_pos = spidar_pos;

	// SPIDARの速度を計算
	// 速度の定義では v = dx / dt だが
	// dtで割らないほうが安定みたいなので
	// 差分だけを使うことにする
	*SPIDARVel = spidar_pos - old_pos;
	old_pos = spidar_pos;

	// SPIDARの姿勢を更新
	Quaterniond qv;
	qv.FromMatrix(view_rot);

	spidar_ori = qv * spidarG6.GetOri();	
	
	static Quaterniond old_ori = spidar_ori;

	// SPIDARの回転速度を計算
	// 前の姿勢から現在の姿勢に変換する４元数を計算
	*SPIDARAngVel = spidar_ori * old_ori.Inv();
	old_ori = spidar_ori;

#if defined EXP1
	spidar_ori = Quaterniond::Rot(45, 'y');
	*SPIDARAngVel = Quaterniond();
#elif defined EXP2
	spidar_ori = Quaterniond::Rot(45, 'y');
	*SPIDARAngVel = Quaterniond();
#endif
}

/**
 brief		解析法の影響を受けない剛体を登録する。この剛体が生む接触は相手に接触力を与えない
 param		登録する剛体
 return		なし
*/
inline void AddInactiveSolid(PHSolidIf* soSolid)
{
	scene->GetConstraintEngine()->AddInactiveSolid(soSolid);
}

// SPIDARの位置と原点をあらわす点を描画する関数
void DisplayPointer()
{
	GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat orange[] = {1.0, 0.2, 0.0, 1.0};
	GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
	GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat yellow[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat *color;

	// 状態によってSPIDARの球の色を変える
	if(bForce)
	{
		if(bSurroundEffect)color = orange;
		else if(!bLocalDynamics) color = green;
		else color = red;
	}
	else 
	{
		if(bSurroundEffect)color = blue;
		else if(!bLocalDynamics) color = yellow;
		else color = white;
	}

	// SPIDARの位置を描画
	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		static Affinef pos_affine;
		Posef::Unit(spidar_pos).ToAffine(pos_affine);
		glMultMatrixf(pos_affine);
	
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, color);

		glutSolidSphere(0.5, 12, 9);
	glPopMatrix();

	// 原点を描画
	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		Vec3f te = Vec3f();
		Affinef af = Affinef();
		Posef::Unit(te).ToAffine(af);
		glMultMatrixf(af);
		glutSolidSphere(0.5, 12, 9);
	glPopMatrix();
}

// 接触点を描画する関数
void DisplayCollisions()
{
	HapticInfo* info;

	if(current_valid_data) info = &info1;
	else info = &info2;

	GLfloat purple[] = {1.0, 0.0, 1.0, 1.0};
	GLfloat color2[] = {0.0, 1.0, 1.0, 1.0};
	GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};

	for(int i = 0; i < info->num_collisions; ++i)
	{
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			Affinef col_affine;
			Posed::Unit(info->col_positions[i]).ToAffine(col_affine);
			glMultMatrixf(col_affine);
			
			if(!info->bPreviousCollide[i])
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, purple);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, purple);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, purple);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, purple);
			}
			else
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, red);
			}
			glutSolidSphere(0.1, 12, 9);
		glPopMatrix();
	}

	for(int i = 0; i < info->num_collisions; ++i)
	{
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			Affinef col_affine;
			Posed::Unit(info->pointer_col_positions[i]).ToAffine(col_affine);
			glMultMatrixf(col_affine);
	
			if(!info->bPreviousCollide[i])
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, color2);
			}
			else
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blue);
			}
			glutSolidSphere(0.1, 12, 9);
		glPopMatrix();
	}
}

/**
 brief  	device managerの初期化を行う関数
 param	 	なし
 return 	なし
 */
void InitDeviceManager()
{
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(1));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(2));		//	Sh4版コントローラ 8モータ

	devMan.Init();								//	デバイスの初期化
	cout << devMan;						//	初期化の結果を表示	
}

// 二点の距離を計算する関数
inline double calcDistance(Vec3d a, Vec3d b)
{
	double x_sq = (a.x - b.x) * (a.x - b.x);
	double y_sq = (a.y - b.y) * (a.y - b.y);
	double z_sq = (a.z - b.z) * (a.z - b.z);

	return sqrt(x_sq + y_sq + z_sq);
}

// 力覚情報を初期化する関数
void InitHapticInfo(HapticInfo* info)
{
	// 初期化処理
	// これ以外のデータは上書きし、さらに個数でアクセスを制限するので
	// 初期化の必要はない
	for(int i = 0; i < info->num_solids; ++i) 
	{
		info->nearest_solids[i] = NULL;
		info->num_col_per_sol[i] = 0;
	}
	for(int i = 0; i < info->num_collisions; ++i)
	{
		info->ForceSum[i] = Vec3d();
	}
	info->num_solids = 0;
	info->num_collisions = 0;
}

// めり込みの状況をチェックしてフラグを更新する関数
void updatePenetrations(HapticInfo* info)
{
	for(int i = 0; i < info->num_collisions; ++i)
	{
		// 接触を更新した瞬間の接触状況をチェックして
		// それにあったフラグを設定する
		double ddot = dot(info->pointer_col_positions[i] - info->col_positions[i], - info->col_normals[i]);			
		double distance = calcDistance(info->pointer_col_positions[i], info->col_positions[i]);

		// 接触していて動かない剛体もしくは距離が離れている接触の場合は接触ありとする
		// 動かない剛体を別扱いにしないと,動かない剛体にポインタがめり込んでいってしまう
		if(ddot >= 0 && (!info->nearest_solids[info->ColToSol[i]]->IsDynamical() || distance <= DISTANCE_LIMIT))info->bCollide[i] = true;
		else info->bCollide[i] = false;
	}
}

inline void updateSimulationFrequency()
{
	SIMULATION_FREQ = SIMULATION_FREQ_BUFFER;
	scene->SetTimeStep((double)1.0 / SIMULATION_FREQ);
}

inline void calcTimeSpan(HapticInfo* info)
{
	info->Hdt = (double)1.0 / (double)HAPTIC_FREQ;
	info->Sdt = (double)1.0 / (double)SIMULATION_FREQ;
	info->HSratio = (double)HAPTIC_FREQ * info->Sdt;
	info->SHratio = (double)SIMULATION_FREQ * info->Hdt;
}

/*
			if(bOutput)
			{
				ofs << "vector (-1, 0, 0) in constraint coordinate is represented in solid coordinate as ";
				if(sign > 0)
				{
					ofs << contact->Xj[0].q * Vec3d(-1, 0, 0) << endl;
				}
				else
				{
					ofs << contact->Xj[1].q * Vec3d(-1, 0, 0) << endl;
				}
				ofs << " and it is converted to wolrd coordinate like ";

				Vec3d f;

				if(sign > 0)
				{
					f =  contact->solid[0]->GetOrientation() * contact->Xj[0].q * Vec3d(-1, 0, 0);
					ofs << f << endl;
					ofs << " and its re-converted by multiplications with conjugeted quaternion ";
					ofs << contact->Xj[0].q.Conjugated() * contact->solid[0]->GetOrientation().Conjugated() * f << endl;
				}
				else 
				{
					f = contact->solid[1]->GetOrientation() * contact->Xj[1].q * Vec3d(-1, 0, 0);
					ofs << f << endl;
					ofs << " and its re-converted by multiplications with conjugeted quaternion ";
					ofs << contact->Xj[1].q.Conjugated() * contact->solid[1]->GetOrientation().Conjugated() * f << endl;
				}
			}
*/
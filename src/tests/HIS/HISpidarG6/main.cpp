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
  また、SPIDARとつなぎ、球体、直方体及び地面と力覚インタラクションを行う。
  
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
#include "Physics/PHConstraint.h"
#include "Physics/PHContactPoint.h"
#include <Physics/PHSpatial.h>

#include <mmsystem.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;

#define ESC				27			// ESC key
//#define EXIT_TIMER		10000		// 実行ステップ数
#define WINSIZE_WIDTH	480			// ウィンドウサイズ(width)
#define WINSIZE_HEIGHT	360			// ウィンドウサイズ(height)
#define NUM_OBJECTS		3			// object数
#define SPIDAR_SCALE	70			// SPIDARのVE内での動作スケール

#define POINTER_RADIUS 0.5f			// ポインタの半径
#define EPSILON 0.15					// ポインタに接触しそうな剛体を予測するためにポインタを膨らませて接触判定をするときの膨らませる量
									// 大きくするほどたくさんの接触を予想できるが、その分の判定処理も増えるので
									// 膨らませすぎるのもよくない

#define LIMIT_DEPTH 100				// 予測シミュレーションを行う剛体取得の深さ上限
#define NUM_PREDICT_ITERATE 15		// 予測シミュレーションのイテレート回数

#define NUM_COLLISIONS 100			// ポインタへの許容接触数
#define NUM_COL_SOLIDS 100			// ポインタへの許容接触剛体数　
									// NUM_COLLISIONSと区別するのはプログラムを読みやすくするため。実質的な存在意義はない

#define COLLISION_DENSITY 0.05		// 力覚の計算に使用する接触点の分布量を調節する値
									// 接触点はこの値より大きい距離で分布する

#ifdef _DEBUG
	#define SIMULATION_FREQ	60		// シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		500		// 力覚スレッドの周期Hz
	float Km = 550;					// virtual couplingの係数
	float Bm = 40;					// 並進

	float Kr = 550;					// 回転
	float Br = 116;

#elif _WINDOWS
	#define SIMULATION_FREQ	60		// シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000	// 力覚スレッドの周期Hz
	float Km = 4500;				// virtual couplingの係数
	float Bm = 150;					// 並進

	float Kr = 4500;				// 回転
	float Br = 160;

//	float Km = 1500;
//	float Bm = 80;

//	float Kr = 1500;
//	float Br = 100;
#endif

// 提示力と剛体に提示する力を直接変化させる定数
double FORCE_COEFF =		1.0;

#ifdef _WIN32		//	Win32版(普通はこっち)
	#include <Device/DRUsb20Simple.h>
	#include <Device/DRUsb20Sh4.h>
	#include <conio.h>
#endif

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
PHSolidIf* soPointerCopy;

// SPIDAR用の変数
DVDeviceManager devMan;						//	D/Aやカウンタなどのデバイスの管理
HISpidarG6X3 spidarG6;						//	SPIDARに対応するクラス

// その他の変数
Vec3f spidar_pos = Vec3f();
Matrix3f view_rot;
Matrix3f view_haptic;


// multimedia timerのハンドラ
MMRESULT FTimerId1;


// 提示力を出すかどうか
bool bforce = false;

// 逐次反映処理のフラグ
bool bGradualReflection = false;

// 局所的動力学計算
bool bLocalDynamics = true;

// debug出力用fstream
ofstream ofs;
bool bOutput = false;

// debug用タイマー
UTPreciseTimer timer;

// シミュレーションをするかどうかをあらわすフラグ
// mmtimerで動くHapticRenderingが時間をカウントして
// 適切な時期にONにして
// idle functionがシミュレーションを行う
bool bSimulation = true;

// 周囲の影響を有効にするかどうかを決めるフラグ
bool bSurroundEffect = false;

// SPIDARの位置を表示するかどうか
bool bDisplayPointer = true;

// KとBの値どちらの変更を有効にするか
// trueの場合並進
// falseの場合回転
bool bMode = true;

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

	Vec3d solid_current_center_positions[NUM_COL_SOLIDS];
	Vec3d solid_future_center_positions[NUM_COL_SOLIDS];
	Quaterniond solid_current_orientations[NUM_COL_SOLIDS];
	Quaterniond solid_future_orientations[NUM_COL_SOLIDS];

	double solid_massinvs[NUM_COL_SOLIDS];
	Matrix3d solid_inertiainvs[NUM_COL_SOLIDS];

	// collision data
	Vec3d future_col_positions[NUM_COLLISIONS];
	Vec3d future_col_normals[NUM_COLLISIONS];
	int num_collisions;
	
	Vec3d current_col_positions[NUM_COLLISIONS];
	Vec3d current_col_normals[NUM_COLLISIONS];

	Vec3d pointer_current_col_positions[NUM_COLLISIONS];
	Vec3d pointer_future_col_positions[NUM_COLLISIONS];

	// 周囲の影響の結果を格納する変数
	Matrix3d vel_effect[NUM_COL_SOLIDS][NUM_COLLISIONS];
	Vec3d vel_constant[NUM_COLLISIONS];

	Matrix3d ang_effect[NUM_COL_SOLIDS][NUM_COLLISIONS];
	Vec3d ang_constant[NUM_COLLISIONS];

	// 実際に接触しているかあらわすフラグ
	// MakeHapticInfoで前回接触していた場合、の判断に使う
	bool bCollide[NUM_COLLISIONS];
} HapticInfo;


// pointer data
Vec3d pointer_pos;
Vec3d pointer_vel;
double pointer_massinv;
Quaterniond pointer_ori;
Vec3d pointer_angvel;
Matrix3d pointer_inertiainv;


// １と２を用意するのはスレッドで必要な排他アクセスを避け（待ちが発生するため）、
// 各プロセスの更新速度を保つため
HapticInfo info1;
HapticInfo info2;

// trueのときはinfo１を参照
// falseのときはinfo２を参照
bool current_valid_data = true; 


// 与えられた接触で、引数のsolidに接している剛体をしらべ、あれば返す
PHSolid* getAdjacentSolid(PHConstraint* constraint, PHSolid* solid, int* sign = NULL)
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

// 提示力の計算および提案手法に必要な情報を集めて構造体を作成する
void MakeHapticInfo(HapticInfo *new_info, HapticInfo *current_info, 
					vector<pair<PHConstraint *, int> > pointer_consts, vector<pair<PHConstraint *, int> > pointer_static_consts, 
					vector<pair<PHConstraint *, int> > col_candidate_consts, vector<pair<PHConstraint *, int> > col_candidate_static_consts,
					vector<pair<PHConstraint *, int> > current_consts, vector<pair<PHConstraint *, int> >current_static_consts, 
					map<PHConstraint *, int> NewcolToCol, map<PHSolid *, int> NewsolToSol,
					map<PHSolid *, vector<pair<Matrix3d, Matrix3d> > > matrices, map<PHSolid *, SpatialVector> vecs)
{
	// 初期化処理
	// 前々回のデータはもう必要ないので初期化する
	// これ以外のデータは上書きし、さらに個数でアクセスを制限するので
	// 初期化の必要はない
	for(int i = 0; i < new_info->num_solids; ++i) 
	{
		new_info->nearest_solids[i] = NULL;
		new_info->num_col_per_sol[i] = 0;
	}
	new_info->num_solids = 0;
	new_info->num_collisions = 0;

	int pointer_consts_size = (int)pointer_consts.size();
	int pointer_static_consts_size = (int)pointer_static_consts.size();

	int col_candidate_consts_size = (int)col_candidate_consts.size();
	int col_candidate_static_consts_size = (int)col_candidate_static_consts.size();

	int current_consts_size = (int)current_consts.size();
	int current_static_consts_size = (int)current_static_consts.size();

	// 力覚レンダリングに必要なデータの作成
	for(int i = 0; i < pointer_consts_size + pointer_static_consts_size + col_candidate_consts_size + col_candidate_static_consts_size + current_consts_size + current_static_consts_size; ++i)
	{
		// 許容数を超える場合は打ち切り
		if(i > NUM_COLLISIONS) break;

		PHConstraint* constraint;
		int sign;
		PHSolid* so;
		
		bool bCollide = true;
		bool bPreviousCollide = false;

		// 実際に接触し、動く剛体を含む接触
		if(pointer_consts_size > i)
		{
			constraint = pointer_consts[i].first;
			sign = pointer_consts[i].second;

			if(sign > 0) so = pointer_consts[i].first->solid[0];
			else so = pointer_consts[i].first->solid[1];
		}
		// 前ステップで接触していた
		else if(pointer_consts_size + current_consts_size > i)
		{
			int i_local = i - pointer_consts_size;

			constraint = current_consts[i_local].first;
			sign = current_consts[i_local].second;

			if(sign > 0) so = current_consts[i_local].first->solid[0];
			else so = current_consts[i_local].first->solid[1];

			bPreviousCollide = true;
		}
		// 実際に接触しており、かつ動かない剛体を含む接触
		else if(pointer_consts_size + pointer_static_consts_size + current_consts_size > i)
		{
			int i_local = i - (pointer_consts_size + current_consts_size);
			constraint = pointer_static_consts[i_local].first;
			sign = pointer_static_consts[i_local].second;

			if(sign > 0) so = pointer_static_consts[i_local].first->solid[0];
			else so = pointer_static_consts[i_local].first->solid[1];
		}
		// 前ステップで接触しており、かつ動かないもの pointer_static_consts_size + 
		else if(pointer_consts_size + pointer_static_consts_size + current_consts_size + current_static_consts_size > i)
		{
			int i_local = i - (pointer_consts_size + current_consts_size + pointer_static_consts_size);
			constraint = current_static_consts[i_local].first;
			sign = current_static_consts[i_local].second;

			if(sign > 0) so = current_static_consts[i_local].first->solid[0];
			else so = current_static_consts[i_local].first->solid[1];

			bPreviousCollide = true;
		}
		// ポインタには接触していなく、動く剛体を含む接触
		// ポインタに接触しそうなものたち
		else if(pointer_consts_size + pointer_static_consts_size + current_consts_size + current_static_consts_size + col_candidate_consts_size > i)
		{
			int i_local = i - (pointer_consts_size + pointer_static_consts_size + current_consts_size + current_static_consts_size);
			constraint = col_candidate_consts[i_local].first;
			sign = col_candidate_consts[i_local].second;

			if(sign > 0) so = col_candidate_consts[i_local].first->solid[0];
			else so = col_candidate_consts[i_local].first->solid[1];

			// この接触は実際には接触していないのでfalse
			bCollide = false;
		}
		// 接触しておらず、かつ動かない剛体を含む接触
		// ポインタに接触しそうな接触
		else 
		{
			int i_local = i - (pointer_consts_size + col_candidate_consts_size + pointer_static_consts_size + current_consts_size + current_static_consts_size);
			constraint = col_candidate_static_consts[i_local].first;
			sign = col_candidate_static_consts[i_local].second;

			if(sign > 0) so = col_candidate_static_consts[i_local].first->solid[0];
			else so = col_candidate_static_consts[i_local].first->solid[1];

			// この接触は実際には接触していないのでfalse
			bCollide = false;
		}


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


				// 今回と前回の接触で共通の剛体があるか調べる
				map<PHSolid *, int>::iterator it = NewsolToSol.find(so);

				// 無い可能性もあるので一応
				// あれば添え字を追加する。なければ-1で初期化
				if(it != NewsolToSol.end()) new_info->NewsolToSol[counter] = (*it).second;
				else new_info->NewsolToSol[counter] = -1;


				new_info->num_solids = counter+1;

				// 速度等も剛体ごとに追加
				new_info->solid_velocity[counter] = so->GetVelocity();
				new_info->solid_angular_velocity[counter] = so->GetAngularVelocity();
				new_info->solid_massinvs[counter] = so->GetMassInv();
				new_info->solid_inertiainvs[counter] = so->GetInertiaInv();

				// 周囲の影響をあらわす変数をコピーする
				// この変数は剛体固有の値なので、
				// 剛体が追加された時点で格納処理を行う
				if(so->IsDynamical())
				{
					map<PHSolid *, SpatialVector>::iterator it1 = vecs.find(so);
					SpatialVector sv = (*it1).second;
					new_info->vel_constant[counter] = sv.v();
					new_info->ang_constant[counter] = sv.w();

					map<PHSolid*, vector<pair<Matrix3d, Matrix3d> > >::iterator it2 = matrices.find(so);
					vector<pair<Matrix3d, Matrix3d> > mm_vector = (*it2).second;
					for(int j = 0; j < (int)mm_vector.size(); ++j)
					{
						new_info->vel_effect[counter][j] = mm_vector[j].first;
						new_info->ang_effect[counter][j] = mm_vector[j].second;
					}
				}
				else
				{
					new_info->vel_constant[counter] = Vec3d();
					new_info->ang_constant[counter] = Vec3d();
				}


				bool current_exists = false;
				
				/*
				if(bLocalDynamics && so->IsDynamical())
				{
					// 前回もこの剛体とポインタの接触が存在したかチェックし、
					// 存在した場合は徐々に戻すようにデータを格納する
					for(int j = 0; j < current_info->num_solids; ++j)
					{
						// 現時点で接触しているかチェックして高速化を図る
						if(!current_info->bCollide[j]) continue;

						if(current_info->nearest_solids[j] == so)
						{
							new_info->solid_current_center_positions[counter] = current_info->solid_current_center_positions[j];
							new_info->solid_future_center_positions[counter] = so->GetCenterPosition() - new_info->solid_current_center_positions[counter];
							new_info->solid_current_orientations[counter] = current_info->solid_current_orientations[j];
							new_info->solid_future_orientations[counter] = so->GetOrientation();

							current_exists = true;				
							break;
						}
					}
				}
				*/

				// 前回は存在しなかった
				if(!current_exists)
				{
					new_info->solid_current_center_positions[counter] = so->GetCenterPosition();
					new_info->solid_future_center_positions[counter] = Vec3d();
					new_info->solid_current_orientations[counter] = so->GetOrientation();
					new_info->solid_future_orientations[counter] = Quaterniond();
				}

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

		new_info->constraint[i] = constraint;
		new_info->sign[i] = sign;
		new_info->shapePair[i] = DCAST(PHContactPoint, constraint)->shapePair;
		new_info->bPreviousCollide[i] = bPreviousCollide;

		// 実際に接触しているかどうかを現すフラグ
		new_info->bCollide[i] = bCollide;

		PHContactPoint* contact = DCAST(PHContactPoint, constraint);
		new_info->solid[i][0] = contact->solid[0];
		new_info->solid[i][1] = contact->solid[1];

		bool current_exists = false;

		if(bPreviousCollide)
		{
			// この接触から現在のデータへの引数を取得する
			int index = NewcolToCol.find(constraint)->second;

			// 現在の値を次も引き継ぐ
			new_info->current_col_positions[i] = current_info->current_col_positions[index];
			new_info->future_col_positions[i] = Vec3d();

			new_info->current_col_normals[i] = current_info->current_col_normals[index];
			new_info->future_col_normals[i] = Vec3d();

			new_info->pointer_current_col_positions[i] = current_info->pointer_current_col_positions[index];
			new_info->pointer_future_col_positions[i] = Vec3d();

			// 添え字の変換表を作成
			new_info->NewcolToCol[i] = index;

			current_exists = true;
		}

		// 前回同じ接触が存在しなかったので
		// 目標の接触の法線と接触点はなし
		if(!current_exists)
		{
			// 現時点で接触している
			if(bCollide)
			{
				// ポインタの接触候補点は接触点と同じ
				new_info->pointer_current_col_positions[i] = contact->pos;
			}
			// 実際には接触していない
			// 接触候補点
			// ポインタ側の接触点の位置を調節する
			else
			{
				// ポインタの中心から接触点までのベクトルを計算し、
				// そのベクトルをポインタの大きさに縮小する（膨らませたポインタと接触判定を行ったため
				// その結果の点を接触候補点として
				// HapticRenderingで接触したかどうかの判断を行う
				Vec3d vec_to_contact = contact->pos - soPointer->GetFramePosition();				
				double j, k, l;

				if(vec_to_contact.x != 0.0) j = fabs(POINTER_RADIUS / vec_to_contact.x);
				else j = 0.0;

				if(vec_to_contact.y != 0.0) k = fabs(POINTER_RADIUS / vec_to_contact.y);
				else k = 0.0;

				if(vec_to_contact.z != 0.0) l = fabs(POINTER_RADIUS / vec_to_contact.z);
				else l = 0.0;

				double c = j;

				if(k != 0.0)
				{
					if(c != 0.0)c = std::min(c, k);
					else c = k;
				}
				if(l != 0.0)
				{
					if(c != 0.0) c = std::min(c, l);
					else c = k;
				}

				// c倍に縮小したベクトルを設定
				new_info->pointer_current_col_positions[i] = vec_to_contact * c + soPointer->GetFramePosition();
			}

			new_info->current_col_positions[i] = contact->pos;
			new_info->current_col_normals[i] = contact->shapePair->normal * sign;
			new_info->future_col_positions[i] = Vec3d();
			new_info->future_col_normals[i] = Vec3d();
			new_info->pointer_future_col_positions[i] = Vec3d();

			// 一応-1で初期化する
			new_info->NewcolToCol[i] = -1;
		}
	}

	new_info->num_collisions = pointer_consts_size + pointer_static_consts_size + col_candidate_consts_size + col_candidate_static_consts_size + current_consts_size + current_static_consts_size;

	// debug info 
//	ofs << "num collision = " << new_info->num_collisions << endl;

	if(new_info->num_collisions >= NUM_COLLISIONS) new_info->num_collisions = NUM_COLLISIONS-1;
}

/**
 brief     	物体の接触点を取得する関数
 param		なし
 return 	接触点の集合
*/
PHConstraints GetContactPoints(PHSceneIf* scene1)
{
	return scene1->GetConstraintEngine()->GetContactPoints();
}

// 再帰的に接している剛体を取得する関数
void RecursiveSolidRetrieval(vector<pair<PHConstraint *, bool> >* csm, PHSolid* solid, vector<PHConstraint *>* relative_consts, set<PHSolid *>* relative_solids, int depth)
{
	for(vector<pair<PHConstraint *, bool> >::iterator it = csm->begin(); it != csm->end(); ++it)
	{
		// 処理済の場合は次にすすむ
		if(it->second) continue;

		// 与えられた剛体から、その剛体に接触する剛体を取得する
		PHSolid* col_solid = getAdjacentSolid(it->first, solid);

		// もしその剛体が存在した場合
		if(col_solid)
		{
			// ポインタの場合はGetSolidsFromPointerで取得する
			if(col_solid == DCAST(PHSolid, soPointer)) continue;
			
			// もしsoPointerCopyとの接触だったら
			// 本来この接触は影響を与えないはずなので
			// 追加しない
			else if(col_solid != DCAST(PHSolid, soPointerCopy))
			{
				// 処理済みにする
				it->second = true;

				// 接触自体を追加
				PHConstraint* c = it->first;
				relative_consts->push_back(c);

				// PHSolidを保存
				// 重複している場合はsetなので自動的に排除される
				relative_solids->insert(c->solid[0]);
				relative_solids->insert(c->solid[1]);
				
				// 動かない剛体ではなく、かつ取得上限まで達していなかった場合、次の剛体にすすむ
				if(col_solid->IsDynamical() && depth < LIMIT_DEPTH) 
					RecursiveSolidRetrieval(csm, col_solid, relative_consts, relative_solids, depth++);
			}
		}
	}
}

// ポインタからスタートして必要な剛体をすべて取得してくる関数
void GetCollisionsFromPointer(vector<pair<PHConstraint *, int> >* pointer_consts, vector<pair<PHConstraint *, int> >* pointer_static_consts,  
						  vector<pair<PHConstraint *, int> >* col_candidate_consts, vector<pair<PHConstraint *, int> >* col_candidate_static_consts, 
						  multimap<double, PHContactPoint*> points,
						  vector<PHConstraint *>* relative_consts, set<PHSolid *>* relative_solids, set<PHSolid *>* nearest_solids)
{
	// 接触点を取得
	PHConstraints cs = GetContactPoints(scene);
	vector<pair<PHConstraint *, bool> > csm;

	// 計算時間を短縮するために接触に処理済フラグをつけて保存する
	// 接触数が少ない場合は無駄が多いが、
	// 接触数が多くなったときの伸びはこちらのほうが断然少ないはず
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); ++it)
	{
		csm.push_back(pair<PHConstraint *, bool>(*it, false));
	}

	// すべての衝突点について調査
	for(vector<pair<PHConstraint *, bool> >::iterator it = csm.begin(); it != csm.end(); ++it)
	{
		// 処理済の場合は次にすすむ
		if(it->second) continue;

		for(int i = 0; i < 2; ++i)
		{
			PHSolid* target1;
			PHSolid* target2;

			// ループカウンタによって検索を切り替える
			// 接触判定用の方がポインタより大きいので、
			// 接触判定用の方が接触をたくさんもっている可能性がある
			// そこでポインタを先に行う
			switch(i)
			{
			case 0:
				target1 = DCAST(PHSolid, soPointer);
				target2 = DCAST(PHSolid, soPointerCopy);
				break;
			case 1:
				target1 = DCAST(PHSolid, soPointerCopy);
				target2 = DCAST(PHSolid, soPointer);
				break;
			}
	
			int sign = 1;

			// ポインタもしくは接触判定用の剛体を含む接触を取得
			PHSolid* col_solid = getAdjacentSolid(it->first, DCAST(PHSolid, target1), &sign);
			
			// 発見でき、かつポインタと接触判定用の剛体のペアではなかった
			// target1 && target2 => ポインタと接触判定用剛体のペア
			if(col_solid && col_solid != DCAST(PHSolid, target2))
			{
				// 処理済にする
				it->second = true;

				PHSolid* so;

				// ポインタと接している剛体のPHSolidを保存
				if(sign > 0) so = it->first->solid[0];
				else so = it->first->solid[1];

				relative_solids->insert(so);
				nearest_solids->insert(so);

				// ポインタとの接触
				if(i == 0)
				{
					PHConstraint* consts = it->first;
					PHContactPoint* contact = DCAST(PHContactPoint, consts);
					bool bAdd = true;
				
					// 引き継ぐ接触と新しく作る接触の位置が近すぎる場合は
					// 新しい接触は追加しない
					// すでに引き継ぐ接触がこの代わりとして考えられるので、
					// 他の点は考える必要はない
					for(multimap<double, PHContactPoint*>::iterator it2 = points.lower_bound(contact->pos.z - COLLISION_DENSITY); it2 != points.upper_bound(contact->pos.z + COLLISION_DENSITY); it2++)
					{
						double norm = (contact->pos - (*it2).second->pos).norm();

						// 引継ぎの接触点の一つと、追加しようとする一つの接触点の距離（ベクトルのノルム）が
						// 小さすぎる場合は追加しない
						if(norm < COLLISION_DENSITY)
						{
//							bAdd = false;
							break;
						}
					}

					// もし動ける剛体であれば、接触自体を格納
					// 動けない剛体は別なベクトルに追加
					// 分ける理由は後で行うシミュレーションで、動かない剛体とポインタの接触を含めるのは意味がないため
					if(bAdd)
					{
						if(col_solid->IsDynamical()) pointer_consts->push_back(pair<PHConstraint *, int>(consts, sign));
						else pointer_static_consts->push_back(pair<PHConstraint *, int>(consts, sign));
					}
				}
				// 判定用ポインタとの接触
				else
				{
					if(col_solid->IsDynamical())col_candidate_consts->push_back(pair<PHConstraint *, int>(it->first, sign));
					else col_candidate_static_consts->push_back(pair<PHConstraint *, int>(it->first, sign));
				}

// 周囲の影響を計算するためのデータ作成処理
// 関係のある剛体と接触を再帰的にすべてとってくる
#if 1
				relative_consts->push_back(it->first);

				// この剛体と生じた接触が次の剛体に影響を及ぼす可能性がある場合は、次の剛体にすすむ
				if(col_solid->IsDynamical())
					RecursiveSolidRetrieval(&csm, col_solid, relative_consts, relative_solids, 0);
#endif
				break;
			}
		}
	}
}

vector<SpatialVector> lambda;

// ラムダの値を保存しておく関数
void SaveLambda(vector<PHConstraint *> relative_consts)
{
	// lambdaを初期化
	lambda.clear();

	for(vector<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{
		// ラムダの値(fv, fw)を保存しておく
		lambda.push_back((*it)->f);
	}
}

// 先送りシミュレーションをする関数
vector<SpatialVector> PredictSimulation(vector<pair<PHConstraint *, int> > pointer_consts, vector<pair<PHConstraint *, int> > col_candidate_consts, vector<pair<PHConstraint *, int> > current_consts, 
										set<PHSolid *> nearest_solids,
										vector<PHConstraint *> relative_consts, set<PHSolid *> relative_solids, 
										Vec3d force_vec, map<PHSolid *, Vec3d> coeff, int index = -1)
{
	SpatialVector dv_save;
	PHSolid* si;

	// ポインタが接する剛体のうちindex番目の接触に単位力を加える
	if(index != -1)
	{
		double dt = scene->GetTimeStep();
		int size1 = (int)pointer_consts.size();
		int size2 = (int)col_candidate_consts.size();

		PHConstraint* constraint;
		int sign;

		if(size1 > index)
		{
			constraint = pointer_consts[index].first;
			sign = pointer_consts[index].second;
		}
		else if(size2 + size1 > index)
		{
			constraint = col_candidate_consts[index - size1].first;
			sign = col_candidate_consts[index - size1].second;
		}
		else
		{
			constraint = current_consts[index - size1 - size2].first;
			sign = current_consts[index - size1 - size2].second;
		}

		PHContactPoint* contact = DCAST(PHContactPoint, constraint);
			
		if(sign > 0) si = DCAST(PHSolid, constraint->solid[0]);
		else si = DCAST(PHSolid, constraint->solid[1]);

		// 事前に計算しておいた値を参照
		map<PHSolid *, Vec3d>::iterator cef = coeff.find(si);

		// 単位力を加える前のdvを保存しておく
		dv_save = si->dv;

		// 引数で与えられた単位力を加える
		si->dv.v() += si->GetMassInv() * force_vec * dt;
		si->dv.w() += si->GetInertiaInv() * (((contact->pos - si->GetCenterPosition()) ^ force_vec) - cef->second) * dt;
	}

	// シミュレーションに必要な情報をセットアップ
	for(vector<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{
		(*it)->SetupLCP();
	}

	for(int i = 0; i < NUM_PREDICT_ITERATE; ++i)
	{
		for(vector<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
		{
			// すべての接触をiterate
			(*it)->IterateLCP();
		}
	}

	vector<SpatialVector> effects;

	// 影響を観測
	// 剛体の速度変化を取得
	for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		effects.push_back((*it)->dv);
	}

	// dvの値を元に戻す
	if(index != -1)si->dv = dv_save;

	int local_counter = 0;
	// fvとfwの値を元にもどす
	// lambdaの保存の過程でrelative_constsのiteratorでvectorに格納しているはずなので
	// このiteratorでループをまわせば接触がわからなくても検索せずに参照できるはず
	for(vector<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{
		(*it)->f = lambda[local_counter++];
	}

	// 影響をreturn
	return effects;
}

// 予測シミュレーションをセットアップする
void SetupPredictSimulation(vector<PHConstraint *> relative_consts, set<PHSolid *> relative_solids, set<PHSolid *> nearest_solids, map<PHSolid *, Vec3d> *coeff)
{
	double dt = scene->GetTimeStep();

	// すべての剛体をセットアップ
	for(set<PHSolid *>::iterator it = relative_solids.begin(); it != relative_solids.end(); ++it)
	{
		(*it)->UpdateCacheLCP(dt);
	}

	// ポインタに接する剛体と
	// ポインタに接する可能性のある剛体に
	// 単位力を加える用意をして、マップに格納する
	for(set<PHSolid *>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		PHSolid* solid = *it;

		// 単位力を加える処理のところで使う値(=加えた力をワールド座標から拘束座標に変換するもの)を今のうちに計算しておく
		Vec3d w = solid->GetOrientation().Conjugated() * solid->GetAngularVelocity();
		Vec3d calc = w % (solid->GetInertia() * w);
		coeff->insert(pair<PHSolid *, Vec3d>(solid, calc));
	}

	// すべての接触についてヤコビアンを計算
	// ヤコビアンの計算は一度だけでよい
	for(vector<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); ++it)
	{
		(*it)->CompJacobian();
	}

	// シミュレーションで上書きしてしまう変数を今のうちに保存する
	SaveLambda(relative_consts);
}


// 提案手法のために先送りシミュレーションを計画的に行ってくれる関数
void PredictSimulations(vector<pair<PHConstraint *, int> > pointer_consts, vector<pair<PHConstraint *, int> > col_candidate_consts, vector<pair<PHConstraint *, int> > current_consts, 
						set<PHSolid *> nearest_solids,
						vector<PHConstraint *> relative_consts, set<PHSolid *> relative_solids,
						map<PHSolid *, vector<pair<Matrix3d, Matrix3d> > > *mat, map<PHSolid *, SpatialVector> *vec)
{
	map<PHSolid *, Vec3d> coeff;

	// 必要な情報の準備
	SetupPredictSimulation(relative_consts, relative_solids, nearest_solids, &coeff);

	// 加える力を０ベクトルとして定数項を取得
	vector<SpatialVector> b = PredictSimulation(pointer_consts, col_candidate_consts, current_consts,
												nearest_solids, 
												relative_consts, relative_solids, 
												Vec3d(), coeff);

	map<PHSolid*, SpatialVector> c;
	int local_counter = 0;

	// 適切にデータを格納するmap-vectorの準備
	// 剛体数だけ空のvectorを宣言してmapに追加する
	map<PHSolid*, vector<pair<Matrix3d, Matrix3d> > > mm_map;
	for(set<PHSolid*>::iterator it = nearest_solids.begin(); it != nearest_solids.end(); ++it)
	{
		mm_map.insert(pair<PHSolid*, vector<pair<Matrix3d, Matrix3d> > >((*it), vector<pair<Matrix3d, Matrix3d> >()));

		// ついでにvectorからmapへの変換処理も行う
		c.insert(pair<PHSolid*, SpatialVector>((*it), b[local_counter++]));
	}

	// 接触数＝列数だけ繰り返す
	for(int i = 0; i < (int)pointer_consts.size() + (int)col_candidate_consts.size() + (int)current_consts.size(); ++i)
	{
		// ある単位ベクトルを加えてその結果をあらわすベクトルを取得
		vector<SpatialVector> vec_x = PredictSimulation(pointer_consts, col_candidate_consts, current_consts, nearest_solids,
														relative_consts, relative_solids, 
														Vec3d(10, 0, 0), coeff, i);
		vector<SpatialVector> vec_y = PredictSimulation(pointer_consts, col_candidate_consts, current_consts, nearest_solids, 
														relative_consts, relative_solids, 
														Vec3d(10, 10, 0), coeff, i);
		vector<SpatialVector> vec_z = PredictSimulation(pointer_consts, col_candidate_consts, current_consts, nearest_solids, 
														relative_consts, relative_solids, 
														Vec3d(10, 0, 10), coeff, i);

		// このループで剛体soの接触pointer_consts[i](もしくはcol_candidate_consts[i])に関する行列を計算し格納
		// この接触のすべての剛体に対する影響をこのループで処理する
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
			// なおトルクについては半径と力の外積が分解されて
			// 半径の外積が行列化されてすでに反映されているはずである
			Matrix3d v = Matrix3d(sv_x.v() / 10.0 - sv.v(), (sv_y.v() - sv_x.v()) / 10.0 - sv.v(), (sv_z.v() - sv_x.v()) / 10.0 - sv.v());
			Matrix3d w = Matrix3d(sv_x.w() / 10.0 - sv.w(), (sv_y.w() - sv_x.w()) / 10.0 - sv.w(), (sv_z.w() - sv_x.w()) / 10.0 - sv.w());

			// データの格納。			
			pair<Matrix3d, Matrix3d> p = pair<Matrix3d, Matrix3d>(v, w);

			// 剛体ごとに接触と係数行列のマップを作成する
			// 接触の順番はわかるが剛体の順番はわからないため。
			// 剛体ごとに接触の順番に行列が格納されていく
			mm_map.find(so)->second.push_back(p);
		}
	}

	*mat = mm_map;
	*vec = c;
}

void CreateConstraintFromCurrentInfo(HapticInfo* current_info, vector<pair<PHConstraint *, int> > *current_consts, vector<pair<PHConstraint *, int> > *current_static_consts, map<PHConstraint *, int>* NewcolToCol, map<PHSolid *, int>* NewsolToSol,
									set<PHSolid *> *relative_solids, vector<PHConstraint *> *relative_consts, set<PHSolid *> *nearest_solids)
{

	// 2ステップ前に作った接触の削除処理
	// 次で新しく接触を作成する
	for(multimap<double, PHContactPoint*>::iterator it = current_info->points.begin(); it != current_info->points.end(); ++it)
	{
		PHContactPoint* cp = (*it).second;
		delete cp;
	}
	current_info->points.clear();

	// 現在使用しているデータから
	// 実際に接触しているもの
	// 接触していないものを調べ
	// そのデータを次に使うかどうか判定する
	for(int i = 0; i < current_info->num_collisions; i++)
	{
		// 実際には接触していない接触
 		if(!current_info->bCollide[i]) continue;

		// 現在使用しているcontactpoint(=constraint)のほとんどはstepが呼ばれるたびに削除されるので、
		// 毎回新しいcontactpointを作る必要がある
		// ここでは現在の位置と向きを持ったcontact pointを新たに作成する
		// ここで作られたcontactpointはhapticinfo->pointsに格納され、手動で消す（上の方でやってる）

		Matrix3d local;	//	contact coodinate system 接触の座標系
		Vec3d normal = current_info->current_col_normals[i];
		Vec3d center_pos = current_info->current_col_positions[i];

		Vec3d v0 = current_info->solid[i][0]->GetPointVelocity(center_pos);
		Vec3d v1 = current_info->solid[i][1]->GetPointVelocity(center_pos);

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

		PHContactPoint *point = DBG_NEW PHContactPoint(local, current_info->shapePair[i], current_info->current_col_positions[i], current_info->solid[i][0], current_info->solid[i][1]);
		point->scene = DCAST(PHScene, scene);
		point->engine = scene->GetConstraintEngine();

		// 現在のポインタ側の接触点を格納する
//		NewcolToCol->insert(pair<PHConstraint *, int>(point, i));

		PHSolid* so = getAdjacentSolid(point, DCAST(PHSolid, soPointer));
//		NewsolToSol->insert(pair<PHSolid *, int>(so, current_info->ColToSol[i]));

		if(scene->GetConstraintEngine()->IsInactiveSolid(current_info->solid[i][0]->Cast())) point->SetInactive(1, false);
		if(scene->GetConstraintEngine()->IsInactiveSolid(current_info->solid[i][1]->Cast())) point->SetInactive(0, false);

		if(current_info->solid[i][0]->IsDynamical() && current_info->solid[i][1]->IsDynamical()) 
		{
//			current_consts->push_back(pair<PHConstraint *, int>(point, current_info->sign[i]));
		}
		else
		{
//			current_static_consts->push_back(pair<PHConstraint *, int>(point, current_info->sign[i]));
		}

		// 今作った接触の登録
//		relative_consts->push_back(point);
//		relative_solids->insert(current_info->nearest_solids[current_info->ColToSol[i]]);
//		nearest_solids->insert(current_info->nearest_solids[current_info->ColToSol[i]]);

		// 次の回で削除に使うためにリンクを保存しておく
//		current_info->points.insert(pair<double, PHContactPoint*>(point->pos.z, point));
	}
}

// 周囲の影響を計算し、引数の変数に格納する関数
void calculate_surround_effect(HapticInfo* new_info, HapticInfo* current_info)
{
	// ポインタに関する接触を格納する変数
	vector<pair<PHConstraint *, int> > pointer_consts;
	vector<pair<PHConstraint *, int> > pointer_static_consts;

	// 接触予測用のデータを格納する変数
	vector<pair<PHConstraint *, int> > col_candidate_consts;
	vector<pair<PHConstraint *, int> > col_candidate_static_consts;

	// 前回の接触を引き継ぐためのデータを格納する変数
	vector<pair<PHConstraint *, int> > current_consts;
	vector<pair<PHConstraint *, int> > current_static_consts;

	// 新しく作った接触と現在の接触を関連付けるマップ
	map<PHConstraint *, int> NewcolToCol;
	map<PHSolid *, int> NewsolToSol;

	set<PHSolid *> relative_solids;
	vector<PHConstraint *> relative_consts;
	set<PHSolid *> nearest_solids;

	// 現在使っている力覚情報の接触のうちで、次も使いそうな接触を取り出す関数
//	CreateConstraintFromCurrentInfo(current_info, &current_consts, &current_static_consts, &NewcolToCol,&NewsolToSol,
//									&relative_solids, &relative_consts, &nearest_solids);

	// ポインタからスタートしてポインタに関係する剛体と接触をすべて取得してくる関数
	GetCollisionsFromPointer(&pointer_consts, &pointer_static_consts, 
						&col_candidate_consts, &col_candidate_static_consts,
						current_info->points,
						&relative_consts, &relative_solids, &nearest_solids);

	// 周囲の影響を格納する変数
	map<PHSolid *, vector<pair<Matrix3d, Matrix3d> > > matrices;
	map<PHSolid *, SpatialVector> vecs;

	// 力を加えてみて動かし、影響を観測する関数
	PredictSimulations(pointer_consts, col_candidate_consts, current_consts, nearest_solids, 
						relative_consts, relative_solids,
						&matrices, &vecs);

	// 接触の情報を計算する
	MakeHapticInfo(new_info, current_info, 
					pointer_consts, pointer_static_consts, 
					col_candidate_consts, col_candidate_static_consts, 
					current_consts, current_static_consts, 
					NewcolToCol, NewsolToSol,
					matrices, vecs);

}

/**
 brief     	物体の接触点の情報を表示する関数
 param		なし
 return 	なし
*/
void show_collision_info()
{
	// 衝突点を取得
	PHConstraints cp = GetContactPoints(scene);

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



// 処理にかかる時間を計測。一時的なもの
static int start, end;

void t_start()
{
	start = timer.CountUS();
}

void t_end()
{
	// 一周にかかる時間をチェック
	end = timer.CountUS();
	if(end - start > 0 && bOutput)ofs << "time = " << end - start << endl;
}

/**
 brief  	提示力の計算とHaptic Device	へ反映. multimedia timerによって呼び出される
			内部的にはポインタとSPIDARをvirtual couplingでつなぎ、ポインタも更新している
 param	 	なし
 return 	なし
*/
void CALLBACK HapticRendering(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
//	t_start();
	
//	t_end();
//	t_start();

	// シミュレーションと力覚レンダリングの更新周期の比率をあらわす変数
	static const double ratio = (double)SIMULATION_FREQ / HAPTIC_FREQ;

	// HAPTICREDERINGの更新幅
	static const double dt = (double)1.0 / HAPTIC_FREQ;


	///////////////////////////////////////////////////////////////////////////////////////
	// シミュレーションの管理と実行

	// シミュレーションを管理するカウンターを作成
	static int step_counter = 0;
	if(step_counter == (int)ratio)
	{
		// シミュレーションが終わっていたら新しいデータに切り替える処理をする
		// 終わっていなかったら仕方がないので前回のデータをそのまま使う
		// (無理に飛ばすと力覚に違和感が生じる
		// カウンターが更新されないので次でもう一度ここにくる
		if(!bSimulation)
		{

			// 新しい情報に切り替える前に
			// 新しい情報で現在の情報を引き継ぐ部分を更新しなおす
			// これは新しい情報が作られたときにはその情報が古いためで
			// 力覚に違和感を出さないためにも
			// 接触点の位置や法線は一致させるようにする
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
				info_next = &info2;
			}

			for(int i = 0; i< info_next->num_solids; ++i)
			{
				// 次回の剛体から今回の剛体の添え字を検索
				int prev_sol_index = info_next->NewsolToSol[i];

				// 見つからなければ次にすすむ
				// 今回新しく剛体が加わった場合はありえる
				if(prev_sol_index == -1)continue;

				// 今回と次回のデータの間での剛体の誤差を計算する
				Vec3d pos_error = info_next->solid_current_center_positions[i] - info_cur->solid_current_center_positions[prev_sol_index];
				Quaterniond ori_error = info_next->solid_current_orientations[i] * info_cur->solid_current_orientations[prev_sol_index].Inv();

				// 今回のデータのうちで、この誤差によって影響を受ける引き継いだ接触を調べる
				for(int j = 0; j < info_next->num_col_per_sol[i]; j++)
				{
					if(info_next->bPreviousCollide[j])
					{
						// mapを使って新しい接触から古い情報の添え字を検索する
						int prev_col_index = info_next->NewcolToCol[info_next->SolToCol[i][j]];

						// 無いはずは無いが一応
						if(prev_col_index == -1) continue;

						// 接触点の情報をコピーする処理と剛体の誤差を反映させる処理
						info_next->current_col_normals[j] = info_cur->current_col_normals[prev_col_index];
						info_next->current_col_normals[j] += ori_error * info_next->current_col_normals[j];
						info_next->current_col_normals[j] = info_next->current_col_normals[j].unit();

						info_next->current_col_positions[j] = info_cur->current_col_positions[prev_col_index];
						info_next->current_col_positions[j] += pos_error;
						info_next->current_col_positions[j] += ori_error * (info_next->current_col_positions[j] - info_next->solid_current_center_positions[j]);

						info_next->pointer_current_col_positions[j] = info_cur->pointer_current_col_positions[prev_col_index];						
					}
				}
			}
			
			bSimulation = true;
			step_counter = 0;
			current_valid_data = !current_valid_data;
		}
//		else if(bOutput)ofs << "error : simulation could not be finished in time" << endl;
	}
	else ++step_counter;


	////////////////////////////////////////////////////////////////////////////////////////
	// 物理情報の取得

	// シミュレーションから得られる情報を格納する変数
	HapticInfo* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data) info = &info1;
	else info = &info2;


	spidarG6.Update(dt);//	USBの通信を行う．


	/////////////////////////////////////////////////////////////////////////////////////////
	// Virtual Coupling
	// 並進
	// SPIDARの位置の更新
	spidar_pos = view_rot * spidarG6.GetPos();		// SPIDAR座標からワールド座標にする

	// SPIDARの情報を格納する変数
	static Vec3d old_pos = spidar_pos;

	// SPIDARの速度を計算
	// 速度の定義では v = dx / dt だが
	// dtで割らないほうが安定みたいなので
	// 差分だけを使うことにする
	Vec3d SPIDARVel = spidar_pos - old_pos;
	old_pos = spidar_pos;

	// SPIDARの位置までのベクトルを作成
	Vec3d goal = spidar_pos - pointer_pos;

	// VR空間のポインタとSPIDARをvirtual couplingでつなげる
	Vec3d VCForce = Km * goal + Bm * (SPIDARVel - pointer_vel);


	/////////////////////////////////////////////////////////////////////////////////////////
	// Virtual Coupling
	// 回転
	// SPIDARの姿勢を更新
	Quaterniond qv;
	qv.FromMatrix(view_rot);

	static Quaterniond spidar_ori;
	spidar_ori = qv * spidarG6.GetOri();

	static Quaterniond old_ori = spidar_ori;

	// SPIDARの回転速度を計算
	// 前の姿勢から現在の姿勢に変換する４元数を計算
	Quaterniond SPIDARAngVel = spidar_ori * old_ori.Inv();
	old_ori = spidar_ori;

	// 現在のポインタの姿勢からSPIDARの姿勢までの回転を表す４元数を計算
	Quaterniond ang_goal = spidar_ori * pointer_ori.Inv();

	// 回転についてのバーチャルカップリング
	Vec3d VCTorque = Kr * ang_goal.Rotation() + Br * (SPIDARAngVel.Rotation() - pointer_angvel);


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// バーチャルカップリングに従ってポインタの位置・姿勢更新
	pointer_vel += pointer_massinv * VCForce * dt;
	Vec3d pointer_dx = pointer_vel * dt;
	pointer_pos += pointer_dx;

	pointer_angvel += pointer_inertiainv * VCTorque * dt;
	Vec3d pointer_dth = pointer_angvel * dt;
	pointer_ori = Quaterniond::Rot(pointer_dth) * pointer_ori;
	pointer_ori = pointer_ori.unit();


	// 登録された接触があっても、現在本当に接触しているかはわからないのでフラグを用意
	bool feedback = false;

	// 局所的な動力学計算を行う場合
	if(bLocalDynamics)
	{
		
		////////////////////////////////////////////////////////////////////////////////////////
		// 局所的な動力学計算
		// 接触剛体の接触力による速度・角速度更新

		for(int i = 0; i < info->num_collisions; ++i)
		{
			// 面の法線と、ポインタ上の点から剛体上の点までを結んだベクトルの内積を計算
			// これが０以上なら（ゼロベクトルも含む。ちょうど接している）接触がある
			if(dot(info->current_col_positions[i] - info->pointer_current_col_positions[i], info->current_col_normals[i]) >= 0)
			{
				// とにかく接触しているのでフラグをONにする
				info->bCollide[i] = true;

				// 力を加えてよい対象かチェック
				// 力の向きと剛体の接する側のチェック
				if(dot(info->current_col_normals[i], -VCForce) >= 0)
				{
					// ユーザに力覚を提示
					feedback = true;

					// 剛体の添え字
					int sol_index = info->ColToSol[i];
			
					// 外力で動く剛体であれば速度が変化するはずなので加速度を計算して速度を更新する
					if(info->nearest_solids[sol_index]->IsDynamical())
					{
						Vec3d accel;
						Vec3d ang_accel;

						// 局所的な動力学計算のみ
						if(!bSurroundEffect)
						{
							// 運動方程式に従って加速度を計算する
							accel = info->solid_massinvs[sol_index] * FORCE_COEFF * VCForce;
							ang_accel = info->solid_inertiainvs[sol_index] * ((info->current_col_positions[i] - info->solid_current_center_positions[sol_index]) ^ (FORCE_COEFF * VCForce));

							// 得られた加速度で速度を更新
							// 座標は後で更新する
							info->solid_velocity[sol_index] += accel * dt;
										
							// 角速度を更新
							info->solid_angular_velocity[sol_index] += ang_accel * dt;
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
							Vec3d q_f = FORCE_COEFF * VCForce * ratio;

							// この力が及ぼす影響をすべての剛体について計算する
							for(int j = 0; j < info->num_solids; ++j)
							{
								// 外力で動かない剛体がひとつでも出たら
								// その先はすべて外力で動かない剛体なのでbreakする
								if(!info->nearest_solids[j]->IsDynamical()) break;

								// 行列を用いて加速度を計算して速度を更新
								info->solid_velocity[j] += info->vel_effect[j][i] * q_f;
								info->solid_angular_velocity[j] += info->ang_effect[j][i] * q_f;
							}
						}
					}
				}
			}
			else info->bCollide[i] = false;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// 局所的動力学計算による剛体の位置・姿勢を更新
		Vec3d correct_vector = Vec3d();
		Vec3d correct_torque = Vec3d();
		int num_cols = 0;

		vector<Vec3d> r_vectors;
		vector<Vec3d> c_vectors;

		for(int i = 0; i < info->num_solids; ++i)
		{
			// 局所的な動力学計算では重力が考慮されていないので
			// 重力の分加速度を更新する
			if(info->nearest_solids[i]->IsDynamical()) info->solid_velocity[i] += scene->GetGravity() * dt;

			if(bSurroundEffect)
			{
				// 周囲の影響のうちで、定数項を徐々に加えていく
				// IsDynamical == falseの場合はそれぞれVec3d()で初期化してあるので
				// 条件分岐は必要ない
				info->solid_velocity[i] += info->vel_constant[i] * ratio;
				info->solid_angular_velocity[i] += info->ang_constant[i] * ratio;
			}

			// 速度による移動・回転
			Vec3d dx_local = info->solid_velocity[i] * dt;
			info->solid_current_center_positions[i] += dx_local;

			Vec3d dth_local = info->solid_angular_velocity[i] * dt;
			info->solid_current_orientations[i] = Quaterniond::Rot(dth_local) * info->solid_current_orientations[i];


			//////////////////////////////////////////////////////////////////////////////////////////
			// 逐次反映処理
			// シミュレーションから得られた各剛体の理想の位置に
			// 徐々に動かしていく処理を行う
			// ただし今回のポインタとの接触力によって動くベクトルで
			// 理想の位置まで動かすベクトルに水平な成分は加えない
			// 回転は徐々に回転軸を動かし、また角度も徐々に近づけていく
			if(bGradualReflection)
			{
				// 並進
				if(info->solid_future_center_positions[i] != Vec3d())
				{
					// futureに近づけていく
					Vec3d bias_center_mov_vec = ratio * (fabs(dot(dx_local, info->solid_future_center_positions[i])) * info->solid_future_center_positions[i].unit() - dx_local);
					
					info->solid_current_center_positions[i] += bias_center_mov_vec;
					dx_local += bias_center_mov_vec;
				}
				// 回転
				if(info->solid_future_orientations[i] != Quaterniond())
				{
					// futureの姿勢に近づけていく
					Vec3d bias_center_ori_vec = ratio * (info->solid_future_orientations[i].Axis() - info->solid_current_orientations[i].Axis());
					double theta = ratio * (info->solid_future_orientations[i].Theta() - info->solid_current_orientations[i].Theta());

					info->solid_current_orientations[i] = Quaterniond::Rot(theta + info->solid_current_orientations[i].Theta(), (bias_center_ori_vec + info->solid_current_orientations[i].Axis()).unit());
 					if(bias_center_ori_vec != Vec3d())dth_local += Quaterniond::Rot(theta, bias_center_ori_vec).Rotation();
				}
			}
			
			// 剛体上で起こった接触を検索する		
			for(int j = 0; j < info->num_col_per_sol[i]; ++j)
			{
				// この剛体上にある接触を剛体の移動に伴って更新する
				// 一覧表から接触の添え字を検索
				int col_index = info->SolToCol[i][j];

				info->current_col_positions[col_index] += dx_local;
				Vec3d dx_rotation = dth_local ^ (info->current_col_positions[col_index] - info->solid_current_center_positions[i]);
				info->current_col_positions[col_index] += dx_rotation;
				info->current_col_normals[col_index] += dx_rotation;
				info->current_col_normals[col_index] = info->current_col_normals[col_index].unit();

				// ポインタの接触点更新
				// ポインタの移動量に従って平行移動
				// この情報はポインタが持つのではなく接触がそれぞれ持っている
				info->pointer_current_col_positions[col_index] += pointer_dx;
				info->pointer_current_col_positions[col_index] += pointer_dth ^ (info->pointer_current_col_positions[col_index] - pointer_pos);

				// 次に行うめり込み解消処理の準備
				Vec3d col_vector = info->current_col_positions[col_index] - info->pointer_current_col_positions[col_index];
				double vector_coeff = dot(col_vector, info->current_col_normals[col_index]);

//				ofs << "all vectors " << vector_coeff << endl;

				// めり込んでいたら補正用のデータを追加
				if(vector_coeff > 0)
				{
					r_vectors.push_back(Vec3d(info->pointer_current_col_positions[col_index] - pointer_pos));
					c_vectors.push_back(Vec3d(vector_coeff * info->current_col_normals[col_index].unit()));

					// めり込んでた接触の個数
					++num_cols;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// ６自由度のめり込み解消処理

		// 計算に使う変数の型を定義
		typedef PTM::TMatrixCol<99, 6, double> MatrixN6d;
		typedef PTM::TMatrixCol<6, 99, double> Matrix6Nd;
		typedef PTM::TVector<99, double> VecNd;

		// ポインタのめり込みに対して擬似逆行列を計算し、
		// 適切な補正量を求めることで、めり込みを補正していく
		// ポインタ自身を並進・回転させてめり込みを解消させるようにする
		if(num_cols)
		{
			Vec3d M_vec = Vec3d();
			Vec3d R_vec = Vec3d();

			if(num_cols > 33) num_cols = 33;

			// データとして使用したベクトルを格納する変数
			vector<Vec3d> used_r_vecs;
			vector<Vec3d> used_c_vecs;

			MatrixN6d M;
			VecNd C;

			// ノルムが大きい順に３つの接触を取り出し、
			// それらの情報から擬似逆行列の計算に必要な
			// 行列とベクトルを作っていく
			for(int k = 0; k < num_cols; ++k)
			{				
				Vec3d c = c_vectors[k];
				Vec3d r = r_vectors[k];

				// ３x６行列の作成と大きさ３のベクトルの作成を３回行う
				for(int i = 0; i < 3; ++i)
				{						
					switch(i)
					{
					// 1, 4, 7行目
					case 0:
						C[k * 3 + i] = c[0];

						for(int j = 0; j < 6; ++j)
						{
							switch(j)
							{
							case 0:
							case 4:
							case 5:
								M[k * 3 + i][j] = 0;
								break;
							case 1:
								M[k * 3 + i][j] = r[2];
								break;
							case 2:
								M[k * 3 + i][j] = - r[1];
								break;
							case 3:
								M[k * 3 + i][j] = 1;
								break;
							}
						}
						break;
					// 2, 5, 8行目
					case 1:
						C[k * 3 + i] = c[1];
						for(int j = 0; j < 6; ++j)
						{
							switch(j)
							{
							case 1:
							case 3:
							case 5:
								M[k * 3 + i][j] = 0;
								break;
							case 0:
								M[k * 3 + i][j] = - r[2];
								break;
							case 2:
								M[k * 3 + i][j] = r[0];
								break;
							case 4:
								M[k * 3 + i][j] = 1;
								break;
							}
						}
						break;
					// 3, 6, 9行目
					case 2:
						C[k * 3 + i] = c[2];
						for(int j = 0; j < 6; ++j)
						{
							switch(j)
							{
							case 2:
							case 3:
							case 4:
								M[k * 3 + i][j] = 0;
								break;
							case 0:
								M[k * 3 + i][j] = r[1];
								break;
							case 1:
								M[k * 3 + i][j] = - r[0];
								break;
							case 5:
								M[k * 3 + i][j] = 1;
								break;
							}
						}
						break;
					}
				}
			}

			Matrix6Nd Mt = M.trans();
			Matrix6d MTM = Mt * M;
			Matrix6d m = Matrix6d();

			// 対角要素に値を足すテスト
			for(int i = 0; i < 6; ++i)
			{
				for(int j = 0; j < 6; ++j)
				{
					if(i == j) m[i][j] = 0.00001;
					else m[i][j] = 0;
				}
			}
			MTM += m;

			Matrix6d MTMinv = MTM.inv();

			for(int i = 0; i < 9; ++i)
			{
				for(int j = 0; j < 6; ++j)
				{
					// QNaNをチェックする
					// ある変数x = QNaNのとき
					// x == xは失敗するらしい

					// QNaNが発生する原因は
					// ２点の接触のときにdeterminantのオーダが低く(10e-300とか）なり
					// invの計算で発散に近づいてしまうため
					if(MTMinv[i][j] != MTMinv[i][j]) 
					{
						MTMinv[i][j] = 1e-300;
					}
				}
			}

			// 擬似逆行列
			Matrix6Nd Pinv = MTMinv * Mt;

			// 並進と回転のベクトル
			// 第一～三要素が回転ベクトルのxyz
			// 第四～六要素が並進ベクトルのxyz
			Vec6d Ans = Pinv * C;
			R_vec = Ans.sub_vector(TSubVectorDim<0,3>());
			M_vec = Ans.sub_vector(TSubVectorDim<3,3>());

			/*
			pointer_vel += P_CORRECTION_COEFF * M_vec / dt;
			pointer_angvel += P_CORRECTION_COEFF * R_vec / dt;
			*/

			// ポインタ自身の位置を移動
			pointer_pos += M_vec;

			// ポインタ自身の姿勢を補正
			Quaterniond correct_q = Quaterniond::Rot(R_vec);
			pointer_ori = pointer_ori * correct_q;

			bool bDebugOutput = false;
			vector<double> norm_array;
			vector<double> col_pos_y_array;
			vector<double> pcol_pos_array;

			// すべての接触もその方向に移動・回転
			for(int i = 0; i < info->num_collisions; ++i)
			{
				info->pointer_current_col_positions[i] += M_vec;
				info->pointer_current_col_positions[i] += R_vec ^ (info->pointer_current_col_positions[i] - pointer_pos);

				double norm_val = dot(info->current_col_positions[i] - info->pointer_current_col_positions[i], info->current_col_normals[i]);
				if(norm_val > 0.00001)
				{
					bDebugOutput = true;
					norm_array.push_back(norm_val);
					col_pos_y_array.push_back(info->current_col_positions[i].y);
					pcol_pos_array.push_back(info->pointer_current_col_positions[i].y);
				}
			}

			if(bDebugOutput)
			{
				ofs << "debug information!!! -----------------------" << endl;
				for(int i = 0; i < (int)norm_array.size(); i++)
				{
//					ofs << "norm val = " << norm_array[i] << " y coord = " << col_pos_y_array[i] << " pointer y coord = " << pcol_pos_array[i] << endl;
				}
				ofs << "num_cols = " << num_cols << endl;
//				ofs << "M = " << endl << M << endl;
//				ofs << "C = " << C << endl;
//				ofs << "MTM = " << endl << MTM << endl;
//				ofs << "MTMinv = " << endl << MTMinv << endl;
//				ofs << "Pinv = " << endl << Pinv << endl;
				ofs << "Pinv * M = " << endl << Pinv * M << endl;
				ofs << "M_vec = " << M_vec << " R_vec = " << R_vec << endl;
			}
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 純粋なバーチャルカップリング
	// 剛体に接触力を加える
	else
	{
		// 接触があれば接触している剛体に提示力の逆方向のベクトルを加える
		// 剛体の挙動は物理シミュレーションに任せる
		for(int i = 0; i < info->num_collisions; ++i)
		{
			// 力覚フィードバックON
			feedback = true;

			int sol_index = info->ColToSol[i];

			// ratioをかける理由は、
			// シミュレーションが加えた力の総量をシミュレーションの周期で力積にするため
			info->nearest_solids[sol_index]->AddForce(FORCE_COEFF * VCForce * ratio, info->current_col_positions[i]);
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// ユーザへの提示力の計算と設定
	if(feedback && bforce)
	{
		// SPIDARの空間と見ている空間が違うので行列を掛けて射影する
		Vec3d f = - view_haptic * VCForce;
		Vec3d t = - view_haptic * VCTorque;

		spidarG6.SetForce(FORCE_COEFF * f, FORCE_COEFF * t);

		// debug出力処理
//		if(bOutput)	ofs << FORCE_COEFF * f << " " << FORCE_COEFF * t << endl;
	}
	else spidarG6.SetForce(Vec3d(), Vec3d());

//	t_end();
}

/**
 brief		特定のソリッドから重力をなくす
 param		重力をなくすソリッド
 return		なし
*/
void RemoveGravity(PHSolidIf* solid)
{
	solid->SetGravity(false);
}

// 提案手法によって生じた剛体の位置の誤差を修正する関数
void ErrorCorrection()
{
	HapticInfo* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data) info = &info1;
	else info = &info2;

	Vec3d gravity = scene->GetGravity() * scene->GetTimeStep();

	// 力覚レンダリングからシミュレーションへの局所的動力学計算の結果反映処理
	// すべての剛体について行う
	for(int i = 0; i < info->num_solids; ++i)
	{
		// 誤差の分平行移動・回転する
		Vec3d pos_error = info->solid_current_center_positions[i] - info->nearest_solids[i]->GetCenterPosition();
		Quaterniond ori_error = info->solid_current_orientations[i] * info->nearest_solids[i]->GetOrientation().Inv();

		info->nearest_solids[i]->SetFramePosition(info->nearest_solids[i]->GetFramePosition() + pos_error);
		info->nearest_solids[i]->SetOrientation(info->nearest_solids[i]->GetOrientation() * ori_error);
		info->nearest_solids[i]->SetCenterPosition(info->solid_current_center_positions[i]);
		info->nearest_solids[i]->SetVelocity(info->solid_velocity[i]);

		// この修正によってシミュレーションされたのでupdateをtrueにする
		info->nearest_solids[i]->SetUpdated(true);

		// もし局所的な動力学計算を行っていた場合
		// HapticRendering内で重力計算を行う
		// さらにシミュレーションで重力計算を行うと二重で重力を追加してしまうので
		// GravityEngineが重力を加えることを見越して
		// その加える分を先に引いて相殺させる
		// 直接重力をON/OFFさせてもよいが、その操作だけでコストがかかってしまうので
		// とりあえずこのようにした
		if(info->nearest_solids[i]->IsDynamical() && bLocalDynamics)
			info->nearest_solids[i]->SetVelocity(info->nearest_solids[i]->GetVelocity() 
				- gravity * info->nearest_solids[i]->GetMass());

		info->nearest_solids[i]->SetAngularVelocity(info->solid_angular_velocity[i]);
	}
}

/**
 brief		VR内のプロキシの位置を更新する関数。
			この関数と力覚レンダリングを組み合わせることでポインタの更新をHAPTIC_FREQの周波数で更新できる。
 param		なし
 return		なし
 */
void UpdatePointer()
{
	HapticInfo* info = NULL;

	// 状態によって参照先を切り替える
	if(current_valid_data) info = &info1;
	else info = &info2;

	soPointer->SetFramePosition(pointer_pos);
	soPointer->SetVelocity(pointer_vel);

	soPointer->SetOrientation(pointer_ori);
	soPointer->SetAngularVelocity(pointer_angvel);
}

void SetupCollisionPrediction()
{
	// soPointerCopyの座標をsoPointerにあわせる
	// この二つはinactive同士なのでめりこみ、
	// soPointerCopyはsoPointerより一回り大きいので、
	// この接触点をみればsoPointerの接触しそうな点がわかる
	soPointerCopy->SetFramePosition(pointer_pos);
	soPointerCopy->SetVelocity(pointer_vel);
	soPointerCopy->SetOrientation(pointer_ori);
	soPointerCopy->SetAngularVelocity(pointer_angvel);
}

void StepSimulation()
{
//	t_end();
//	t_start();

//	if(bOutput) ofs << "start simulation" << endl;

	scene->ClearForce();

	// ポインタの位置を更新
	UpdatePointer();

	// 局所的動力学計算によって生じた差を修正する
	ErrorCorrection();

	// ポインタに接触しそうな点を探すための準備
	SetupCollisionPrediction();

	scene->GenerateForce();
	scene->Integrate();

	// 衝突点情報を表示
//	show_collision_info();

	// 衝突点から周囲の影響を計算
	// 基本的に前回のデータを引き継ぐ
	// １を参照中。２を更新
	if(current_valid_data) calculate_surround_effect(&info2, &info1);
	// ２を参照中。１を更新
	else calculate_surround_effect(&info1, &info2);

//	if(bOutput) ofs << "finish simulation " << endl;

	// シミュレーションが終わったのでフラグを下げる
	bSimulation = false;

	//	t_end();
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	// 再描画
	glutPostRedisplay();

	if(bSimulation)StepSimulation();
}

/**
 brief		解析法の影響を受けない剛体を登録する。この剛体が生む接触は相手に接触力を与えない
 param		登録する剛体
 return		なし
*/
void AddInactiveSolid(PHSceneIf* scene1, PHSolidIf* soSolid)
{
	scene1->GetConstraintEngine()->AddInactiveSolid(soSolid);
}

// SPIDARの位置と原点をあらわす点を描画する関数
void displayPointer()
{
	GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat orange[] = {1.0, 0.2, 0.0, 1.0};
	GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
	GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat *color;

	// 状態によってSPIDARの球の色を変える
	if(bforce)
	{
		if(bSurroundEffect)color = orange;
		else color = red;
	}
	else
	{
		if(bSurroundEffect)color = blue;
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
void displayCollisions()
{
	HapticInfo* info;

	if(current_valid_data)
	{
		info = &info1;
	}
	else
	{
		info = &info2;
	}

	GLfloat purple[] = {1.0, 0.0, 1.0, 1.0};
	GLfloat color2[] = {0.0, 1.0, 1.0, 1.0};
	GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
	GLfloat red[] = {1.0, 0.0, 0.0, 1.0};

	for(int i = 0; i < info->num_collisions; ++i)
	{
		glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			Affinef col_affine;
			Posed::Unit(info->current_col_positions[i]).ToAffine(col_affine);
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
			Posed::Unit(info->pointer_current_col_positions[i]).ToAffine(col_affine);
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
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);

	// フラグがONのときSPIDARの軌跡をデバッグ出力
	if(bDisplayPointer) displayPointer();

	displayCollisions();

	render->EndScene();
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

		info1.points.clear();
		info2.points.clear();

		cout << "EXIT from Key Input" << endl;
		exit(0);
	}
	// デバッグ出力の許可・不可の切り替え
	else if(key == 'o')
	{
		bOutput = !bOutput;
		cout << "File Output is ";
		if(bOutput)cout << "ENABLED" << endl;
		else cout << "DISABLED" << endl;
	}
	// ポインタのキャリブレーション
	else if(key == 'c')
	{
		spidarG6.Calib();
	}
	// toggle force output
	else if(key == ' ')
	{
		bforce = !bforce;

		// メッセージを出力
		cout << "Feedback Force ";
		if(bforce) cout << "ON" << endl;
		else cout << "OFF" << endl;
	}
	// soPointerとSPIDARの場所を初期値に戻し、キャリブレーションを行う(virtual coupling用）
	else if(key == 'i')
	{
		soPointer->SetVelocity(Vec3d());
		soPointer->SetAngularVelocity(Vec3d());
		soPointer->SetPose(Posed());
		soPointer->SetFramePosition(Vec3d());
		spidarG6.Calib();
	}
	// sphereの場所をリセット
	else if(key == 'r')
	{
		for(int i = 0; i < NUM_OBJECTS; ++i)
		{
			soObject[i]->SetPose(Posed());
			soObject[i]->SetFramePosition(Vec3f(0, 5+5*(i+1), 0));
			soObject[i]->SetAngularVelocity(Vec3d());
			soObject[i]->SetVelocity(Vec3d());
		}
	}
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
	// SPIDARの位置と原点をあらわす球の表示をON/OFFする
	else if(key == 'd')
	{
		bDisplayPointer = !bDisplayPointer;
		cout << "Pointer Display ";
		if(!bDisplayPointer)cout << "OFF " << endl;
		else cout << "ON " << endl;
	}
	// パラメータ編集対象の切り替え処理
	else if(key == 't')
	{
		bMode = !bMode;
		if(bMode)cout << "MOVEMENT ";
		else cout << "ROTATION ";
		cout << "Edit Mode" << endl;
	}
	// 逐次反映処理のフラグ変更
	else if(key == 'g')
	{
		bGradualReflection = !bGradualReflection;
		cout << "Gradual Reflection ";
		if(bGradualReflection)
		{
			cout << "ON " << endl;
			
			if(!bLocalDynamics)
			{
				bLocalDynamics = true;
				cout << "Local Dynamics ON" << endl;
			}
		}
		else cout << "OFF " << endl;
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

			bGradualReflection = false;
			bSurroundEffect = false;

			cout << "Correct Penetration OFF" << endl;
			cout << "Gradual Reflection OFF" << endl;
			cout << "Surround Effect OFF " << endl;
		}
	}
	// すべてのフラグのデバッグ表示
	else if(key == 'f')
	{
		cout << "--- Current Status ---" << endl;
		keyboard('e', 0, 0);
		cout << "coeff = " << FORCE_COEFF << endl;

		cout << "Feedback Force ";
		if(bforce)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Surrounding Effect ";
		if(bSurroundEffect)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Pointer Display ";
		if(bDisplayPointer)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "Gradual Reflection ";
		if(bGradualReflection)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "File Output ";
		if(bOutput)cout << "ON" << endl;
		else cout << "OFF" << endl;

		cout << "----------------------" << endl;
	}
	// 設定のプレセット
	else if(key == '1')
	{
		bSurroundEffect = true;
		bGradualReflection = true;
		bLocalDynamics = true;

		cout << "PRESET 1: Local Dynamics With Surrounding Effect, Penetration Correction, and Gradual Reflection" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '2')
	{
		bSurroundEffect = true;
		bGradualReflection = false;
		bLocalDynamics = true;
		
		cout << "PRESET 2: Local Dynamics With Surrounding Effect" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '3')
	{
		bSurroundEffect = false;
		bGradualReflection = true;
		bLocalDynamics = true;

		cout << "PRESET 3: Local Dynamics With Penetration Correction and Gradual Reflection" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '4')
	{
		bSurroundEffect = false;
		bGradualReflection = false;
		bLocalDynamics = true;

		cout << "PRESET 4: Local Dynamics" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '5')
	{
		bSurroundEffect = false;
		bGradualReflection = false;
		bLocalDynamics = false;

		cout << "PRESET 5: Virtual Coupling" << endl;
		keyboard('f', 0, 0);
	}
}

void InitScene();
void InitRendering(int*, char**);
void InitDeviceManager();

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){

	ofs.open("log.txt");
	ofs.precision(12);

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

//	timer.CountUS();

	// hapticスレッドの生成・開始
	FTimerId1 = timeSetEvent(1000 / HAPTIC_FREQ,    // タイマー間隔[ms]
	                        1,   // 時間分解能
		                    HapticRendering,//コールバック関数
		                    (DWORD)&time,//ユーザー定義の値
		                    TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
		                    );

	// openGL main loop
	glutMainLoop();
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
	sd.timeStep = (double)1.0 / SIMULATION_FREQ;
	scene = phSdk->CreateScene(sd);				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 0.3f;

	// inertiaの計算式
	// 直方体の場合 I = mass * (r1^2 + r2^2) / 12
	// 球の場合 I = mass * r~2 * 2/5
	desc.inertia = Matrix3d(0.2,0,0,0,0.2,0,0,0,0.2);
/*
	soObject.push_back(scene->CreateSolid(desc));

	desc.mass = 1.0f;
	desc.inertia = Matrix3d(0.375, 0.0, 0.0, 0.0, 0.375, 0.0, 0.0, 0.0, 0.375);
	soObject.push_back(scene->CreateSolid(desc));
*/

	// Solidの作成
	for (unsigned int objectCnt=0; objectCnt<NUM_OBJECTS; ++objectCnt){
		soObject.push_back(scene->CreateSolid(desc));		// 剛体をdescに基づいて作成
	}


	desc.mass = 0.1f;
	desc.inertia = Matrix3d(0.16, 0, 0, 0, 0.16, 0, 0, 0, 0.16);

	for(int i = 0; i < 1; ++i)
	{
		PHSolidIf *soWall = scene->CreateSolid(desc);
		soWall->SetDynamical(false);
		soWalls.push_back(soWall);
//		soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
//		soFloor->SetDynamical(false);
	}
	soPointer = scene->CreateSolid(desc);
	soPointerCopy = scene->CreateSolid(desc);

	//	形状の作成
	CDBoxIf* floor=NULL;
	CDSphereIf* sphere=NULL;

	{
		CDSphereDesc sd;
		sd.radius = 1.25f;
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));

		// 球形のポインタ
		/*
		soPointer->AddShape(sphere);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));
		*/

		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf,phSdk->CreateShape(sd));

		CDBoxDesc bd;
		for(int i = 0; i < 1; ++i)
		{
			Vec3f position;

			switch(i)
			{
			case 0:
				bd.boxsize = Vec3f(30.0f, 10.0f, 30.0f);
				position = Vec3f(0, -6.0f, 0);
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

		// ポインタよりも一回り大きなポインタを作成し、
		// 接触予想用に使う
		bd.boxsize += Vec3f(EPSILON, EPSILON, EPSILON);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soPointerCopy->AddShape(floor);
		soPointerCopy->SetFramePosition(soPointer->GetFramePosition() + Vec3f(1, 1, 1));

		bd.boxsize = Vec3f(1.5f, 1.5f, 1.5f);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
	}	

	for (unsigned int sphereCnt=0; sphereCnt<NUM_OBJECTS; ++sphereCnt){
		soObject[sphereCnt]->AddShape(floor);
		soObject[sphereCnt]->SetFramePosition(Vec3f(0, 5.0f*(sphereCnt+1), 2));
	}
	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	// pointerの重力を無効にする
	RemoveGravity(soPointer);
	RemoveGravity(soPointerCopy);

	// pointerと接触する剛体との解析法による計算をやめ、
	// 力覚で処理するようにする
	AddInactiveSolid(scene, soPointer);

	// soPointerCopyもinactiveにする
	// こうするとsoPointerにめりこむようになるはず
	AddInactiveSolid(scene, soPointerCopy);

	pointer_pos = soPointer->GetFramePosition();
	pointer_vel = soPointer->GetVelocity();
	pointer_massinv = soPointer->GetMassInv();
	pointer_angvel = soPointer->GetAngularVelocity();
	pointer_inertiainv = soPointer->GetInertiaInv();
	pointer_ori = soPointer->GetOrientation();

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
	grDevice = grSdk->CreateDeviceGL(window);

	// 初期設定
	grDevice->Init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	render->SetDevice(grDevice);	// デバイスの設定

	// 視点を設定する
	Affinef view;
	view.Pos() = Vec3f(0.0, 2.0, -8.0);
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);
	
	setLight();

	/// SPIDAR座標をビュー座標に直す変数
	view_rot = view.inv().Rot() * SPIDAR_SCALE;
	view_haptic = view.Rot();
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

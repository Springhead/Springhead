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
  DrawScene API で設定されているマテリアルサンプルを用いて、カラフルな球体をレンダリングする。　
  また、SPIDARとつなぎ、球体及び地面と力覚インタラクションを行う。
  
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
#define NUM_SPHERES		2			// sphere数
#define SPIDAR_SCALE	70			// SPIDARのVE内での動作スケール


#define LIMIT_DEPTH 100				// 予測シミュレーションを行う剛体取得の深さ上限
#define NUM_PREDICT_ITERATE 5		// 予測シミュレーションのイテレート回数

#define NUM_COLLISIONS 20			// ポインタへの許容接触数
#define NUM_COL_SOLIDS 20			// ポインタへの許容接触剛体数　NUM_COLLISIONSと区別するのはプログラムを読みやすくするため。実質的な存在意義はない


#ifdef _DEBUG
	#define SIMULATION_FREQ	60         // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		500			// 力覚スレッドの周期Hz
	float Km = 550;						// virtual couplingの係数
	float Bm = 30;						// 並進

	float Kr = 150;						// 回転
	float Br = 13;

#elif _WINDOWS
	#define SIMULATION_FREQ	60          // シミュレーションの更新周期Hz
	#define HAPTIC_FREQ		1000		// 力覚スレッドの周期Hz
	float Km = 1000;						// virtual couplingの係数
	float Bm = 60;						// 並進

	float Kr = 1000;						// 回転
	float Br = 150;
#endif

// 提示力と剛体に提示する力を直接変化させる定数
double FORCE_COEFF =		0.5;

#ifdef _WIN32		//	Win32版(普通はこっち)
	#include <Device/DRUsb20Simple.h>
	#include <Device/DRUsb20Sh4.h>
	#include <conio.h>
#endif

// グラフィック用の変数
GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

// 物理計算用の変数
PHSdkIf* phSdk;
PHSceneIf* scene;

// 仮想世界で扱う剛体
PHSolidIf* soFloor;
vector<PHSolidIf*> soSphere; 
PHSolidIf* soPointer;

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

// めり込み補正のフラグ
bool bCorrectPenetration = true;

// 逐次反映処理のフラグ
bool bGradualReflection = true;

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

// 力覚計算に必要なデータを集めた構造体
// 力覚計算を高速で行えるように
// シミュレーションでデータを格納し、
// HapticRenderingに渡す
typedef struct {
	// collision data
	PHConstraint* constraint[NUM_COLLISIONS];

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

	// pointer data
	Vec3d pointer_pos;

	Vec3d pointer_vel;
	double pointer_massinv;

	Quaterniond pointer_ori;
	Vec3d pointer_angvel;
	Matrix3d pointer_inertiainv;

	// flag for gradual reflection
	bool bLocalReflection1[NUM_COL_SOLIDS];
	bool bLocalReflection2[NUM_COL_SOLIDS];
} HapticInfo;

// １と２を用意するのはスレッドで必要な排他アクセスを避け（待ちが発生するため）、
// 各プロセスの更新速度を保つため
HapticInfo info1;
HapticInfo info2;

// trueのときは構造体１を参照
// falseのときは構造体２を参照
bool current_valid_data = true; 

// 周囲の影響を有効にするかどうかを決めるフラグ
bool bSurroundEffect = false;

// SPIDARの位置を表示するかどうか
bool bDisplayPointer = true;

// KとBの値どちらの変更を有効にするか
// trueの場合並進
// falseの場合回転
bool bMode = true;

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
void MakeHapticInfo(HapticInfo *info, HapticInfo *prev_info, vector<pair<PHConstraint *, int> > pointer_consts, map<PHSolid *, vector<pair<Matrix3d, Matrix3d> > > matrices, map<PHSolid *, SpatialVector> vecs, vector<pair<PHConstraint *, int> > pointer_static_consts)
{
	// 初期化処理
	// 前々回のデータはもう必要ないので初期化する
	// これ以外のデータは上書きし、さらに個数でアクセスを制限するので
	// 初期化の必要はない
	for(int i = 0; i < info->num_solids; i++) 
	{
		info->nearest_solids[i] = NULL;
		info->num_col_per_sol[i] = 0;
	}
	info->num_solids = 0;

	int consts_size = (int)pointer_consts.size();
	int static_consts_size = (int)pointer_static_consts.size();

	// 力覚レンダリングに必要なデータの作成
	for(int i = 0; i < consts_size + static_consts_size; i++)
	{
		// 許容数を超える場合は打ち切り
		if(i > NUM_COLLISIONS) break;

		PHConstraint* constraint;
		int sign;
		PHSolid* so;

		// 動く剛体を含む接触
		if(consts_size > i)
		{
			constraint = pointer_consts[i].first;
			sign = pointer_consts[i].second;

			if(sign > 0) so = pointer_consts[i].first->solid[0];
			else so = pointer_consts[i].first->solid[1];
		}
		// 動かない剛体を含む接触
		else
		{
			constraint = pointer_static_consts[i - consts_size].first;
			sign = pointer_static_consts[i - consts_size].second;

			if(sign > 0) so = pointer_static_consts[i - consts_size].first->solid[0];
			else so = pointer_static_consts[i - consts_size].first->solid[1];
		}


		int counter = 0;
		// 配列にPHSolidを追加
		// 剛体と接触が１対１ではないため、接触から剛体へ添え字を変換するマップを作成する
		while(1)
		{
			// まだ追加されていない剛体は追加する
			if(!info->nearest_solids[counter])
			{
				info->nearest_solids[counter] = so;
				
				// 剛体から接触へ
				// また接触から剛体へ時間をかけずに変換できるように
				// 配列に添え字を格納しておく
				info->SolToCol[counter][info->num_col_per_sol[counter]] = i;
				info->num_col_per_sol[counter]++;

				info->ColToSol[i] = counter;
				info->num_solids = counter+1;

				// 速度等も剛体ごとに追加
				info->solid_velocity[counter] = so->GetVelocity();
				info->solid_angular_velocity[counter] = so->GetAngularVelocity();
				info->solid_massinvs[counter] = so->GetMassInv();
				info->solid_inertiainvs[counter] = so->GetInertiaInv();

				// 周囲の影響をあらわす変数をコピーする
				// この変数は剛体固有の値なので、
				// 剛体が追加された時点で格納処理を行う
				if(so->IsDynamical())
				{
					map<PHSolid *, SpatialVector>::iterator it1 = vecs.find(so);
					SpatialVector sv = (*it1).second;
					info->vel_constant[counter] = sv.v;
					info->ang_constant[counter] = sv.w;

					map<PHSolid*, vector<pair<Matrix3d, Matrix3d> > >::iterator it2 = matrices.find(so);
					vector<pair<Matrix3d, Matrix3d> > mm_vector = (*it2).second;
					for(int j = 0; j < (int)mm_vector.size(); j++)
					{
						info->vel_effect[counter][j] = mm_vector[j].first;
						info->ang_effect[counter][j] = mm_vector[j].second;
					}
				}
				else
				{
					info->vel_constant[counter] = Vec3d();
					info->ang_constant[counter] = Vec3d();
				}


				bool current_exists = false;
				// 前回もこの剛体とポインタの接触が存在したかチェックし、
				// 存在した場合は徐々に戻すようにデータを格納する
				for(int j = 0; j < prev_info->num_solids; j++)
				{
					if(prev_info->nearest_solids[j] == so && so->IsDynamical() && bLocalDynamics)
					{
						info->solid_current_center_positions[counter] = prev_info->solid_current_center_positions[j];
						info->solid_future_center_positions[counter] = so->GetCenterPosition() - info->solid_current_center_positions[counter];
						info->solid_current_orientations[counter] = prev_info->solid_current_orientations[j];
						info->solid_future_orientations[counter] = so->GetOrientation();

						current_exists = true;				
						break;
					}
				}

				// 前回は存在しなかった
				if(!current_exists)
				{
					info->solid_current_center_positions[counter] = so->GetCenterPosition();
					info->solid_future_center_positions[counter] = Vec3d();
					info->solid_current_orientations[counter] = so->GetOrientation();
					info->solid_future_orientations[counter] = Quaterniond();
				}

				// シミュレーションの結果を力覚レンダリングのプロセスで徐々に反映させる処理を現すフラグを初期化
				info->bLocalReflection1[counter] = true;
				info->bLocalReflection2[counter] = true;

				break;
			}
			// もし剛体がすでに追加済みの場合は追加しない
			// 添え字変換表のみ作成
			else if(info->nearest_solids[counter] == so)
			{
				info->SolToCol[counter][info->num_col_per_sol[counter]] = i;
				info->num_col_per_sol[counter]++;

				info->ColToSol[i] = counter;

				break;
			}

			// 次の登録された剛体に進む
			counter++;
		}

		info->constraint[i] = constraint;
		PHContactPoint* contact = DCAST(PHContactPoint, constraint);

		bool current_exists = false;

		// 同じ接触が前回存在したかチェック
		for(int j = 0; j < prev_info->num_collisions; j++)
		{
			// 見つかった場合前回の法線と接触点を保存し
			// 今回のデータを目標として徐々に更新を行う
			if(prev_info->constraint[j] == constraint && bLocalDynamics)
			{
				info->current_col_positions[i] = prev_info->current_col_positions[j];
				info->future_col_positions[i] = contact->pos;

				info->current_col_normals[i] = prev_info->current_col_normals[j];
				info->future_col_normals[i] = contact->shapePair->normal * sign;

				info->pointer_current_col_positions[i] = prev_info->pointer_current_col_positions[j];
				info->pointer_future_col_positions[i] = contact->pos;

				current_exists = true;
				break;
			}
		}

		// 前回同じ接触が存在しなかったので
		// 目標の接触の法線と接触点はなし
		if(!current_exists)
		{
			info->current_col_positions[i] = contact->pos;
			info->future_col_positions[i] = Vec3d();

			info->current_col_normals[i] = contact->shapePair->normal * sign;
			info->future_col_normals[i] = Vec3d();

			// ポインタの接触候補点は接触点と同じ
			info->pointer_current_col_positions[i] = info->current_col_positions[i];
			info->pointer_future_col_positions[i] = Vec3d();
		}
	}

	info->num_collisions = consts_size + static_consts_size;

	// 剛体ポインタの情報を格納
	info->pointer_pos = soPointer->GetFramePosition();
	info->pointer_vel = soPointer->GetVelocity();
	info->pointer_massinv = soPointer->GetMassInv();
	info->pointer_angvel = soPointer->GetAngularVelocity();
	info->pointer_inertiainv = soPointer->GetInertiaInv();
	info->pointer_ori = soPointer->GetOrientation();
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
void RecursiveSolidRetrieval(map<PHConstraint *, bool>* csm, PHSolid* solid, set<PHConstraint *>* relative_consts, set<PHSolid *>* relative_solids, int depth)
{
	for(map<PHConstraint *, bool>::iterator it = csm->begin(); it != csm->end(); it++)
	{
		// 処理済の場合は次にすすむ
		if(it->second) continue;

		// 与えられた剛体から、その剛体に接触する剛体を取得する
		PHSolid* col_solid = getAdjacentSolid(it->first, solid);

		// もしその剛体が存在し、ポインタではない場合
		// ポインタの場合はGetSolidsFromPointerで取得する
		if(col_solid && col_solid != DCAST(PHSolid, soPointer))
		{
			// 処理済みにする
			it->second = true;

			// 接触自体を追加
			PHConstraint* c = it->first;
			pair<set<PHConstraint *>::iterator, bool> p = relative_consts->insert(c);

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

// ポインタからスタートして必要な剛体をすべて取得してくる関数
void GetSolidsFromPointer(vector<pair<PHConstraint *, int> >* pointer_consts, set<PHSolid *>* pointer_relative_solids, set<PHSolid *>* relative_solids, set<PHConstraint *>* relative_consts, vector<pair<PHConstraint *, int> >* pointer_static_consts)
{
	// 衝突点を取得
	PHConstraints cs = GetContactPoints(scene);
	map<PHConstraint *, bool> csm;

	// 計算時間を短縮するためにマップに処理済フラグをつけて保存する
	// 接触数が少ない場合は無駄が多いが、
	// 接触数が多くなったときの伸びはこちらのほうが断然少ないはず
	for(PHConstraints::iterator it = cs.begin(); it != cs.end(); it++)
	{
		csm.insert(pair<PHConstraint *, bool>(*it, false));
	}

	// すべての衝突点について調査
	for(map<PHConstraint *, bool>::iterator it = csm.begin(); it != csm.end(); it++)
	{
		// 処理済の場合は次にすすむ
		if(it->second) continue;

		int sign = 1;

		// ポインタを含む接触を取得
		PHSolid* col_solid = getAdjacentSolid(it->first, DCAST(PHSolid, soPointer), &sign);
		
		// 発見できた
		if(col_solid)
		{
			// 処理済にする
			it->second = true;

			// ポインタと接している剛体のPHSolidを保存
			if(sign > 0)
			{
				relative_solids->insert(it->first->solid[0]);
				pointer_relative_solids->insert(it->first->solid[0]);
			}
			else
			{
				relative_solids->insert(it->first->solid[1]);
				pointer_relative_solids->insert(it->first->solid[1]);
			}

			// もし動ける剛体であれば、接触自体を格納
			// 動けない剛体は別なベクトルに追加
			// 分ける理由は後で行うシミュレーションで、動かない剛体とポインタの接触を含めるのは意味がないため
			if(col_solid->IsDynamical())pointer_consts->push_back(pair<PHConstraint *, int>(it->first, sign));
			else pointer_static_consts->push_back(pair<PHConstraint *, int>(it->first, sign));

// 周囲の影響を計算するためのデータ作成処理
#if 1
			relative_consts->insert(it->first);

			// この剛体と生じた接触が次の剛体に影響を及ぼす可能性がある場合は、次の剛体にすすむ
			if(col_solid->IsDynamical())
				RecursiveSolidRetrieval(&csm, col_solid, relative_consts, relative_solids, 0);
#endif
		}
	}
}

map<PHConstraint *, SpatialVector> lambda;

// ラムダの値を保存しておく関数
void SaveLambda(set<PHConstraint *> relative_consts)
{
	// lambdaを初期化
	lambda.clear();

	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); it++)
	{
		// ラムダの値(fv, fw)を接触とペアで保存しておく
		lambda.insert(pair<PHConstraint *, SpatialVector>(*it, (*it)->f));
	}
}

// 先送りシミュレーションをする関数
map<PHSolid *, SpatialVector> PredictSimulation(vector<pair<PHConstraint *, int> > pointer_consts, set<PHSolid *> pointer_relative_solids, set<PHConstraint *> relative_consts, set<PHSolid *> relative_solids, Vec3d force_vec, map<PHSolid *, Vec3d> coeff, int index = -1)
{
	SpatialVector dv_save;
	PHSolid* si;

	// ポインタが接する剛体のうちindex番目の接触に単位力を加える
	if(index != -1)
	{
		double dt = scene->GetTimeStep();

		PHConstraint *constraint = pointer_consts[index].first;
		PHContactPoint* contact = DCAST(PHContactPoint, constraint);
			
		if(pointer_consts[index].second > 0) si = DCAST(PHSolid, constraint->solid[0]);
		else si = DCAST(PHSolid, constraint->solid[1]);

		// 事前に計算しておいた値を参照
		map<PHSolid *, Vec3d>::iterator cef = coeff.find(si);

		// 単位力を加える前のdvを保存しておく
		dv_save = si->dv;

		// 引数で与えられた単位力を加える
		si->dv.v += si->GetMassInv() * force_vec * dt;
		si->dv.w += si->GetInertiaInv() * (((contact->pos - si->GetCenterPosition()) ^ force_vec) - cef->second) * dt;
	}

	// シミュレーションに必要な情報をセットアップ
	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); it++)
	{
		(*it)->SetupLCP();
	}

	for(int i = 0; i < NUM_PREDICT_ITERATE; i++)
	{
		for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); it++)
		{
			// すべての接触をiterate
			(*it)->IterateLCP();
		}
	}

	map<PHSolid *, SpatialVector> effects;

	// 影響を観測
	// 剛体の速度変化を取得
	for(set<PHSolid *>::iterator it = pointer_relative_solids.begin(); it != pointer_relative_solids.end(); it++)
	{
		effects.insert(pair<PHSolid *, SpatialVector>(*it, (*it)->dv));
	}

	// dvの値を元に戻す
	if(index != -1)si->dv = dv_save;

	// fvとfwの値を元にもどす
	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); it++)
	{
		(*it)->f = lambda.find(*it)->second;
	}

	// 影響をreturn
	return effects;
}

// 予測シミュレーションをセットアップする
void SetupPredictSimulation(set<PHConstraint *> relative_consts, set<PHSolid *> relative_solids, set<PHSolid *> pointer_relative_solids, map<PHSolid *, Vec3d> *coeff)
{
	double dt = scene->GetTimeStep();

	// すべての剛体をセットアップ
	for(set<PHSolid *>::iterator it = relative_solids.begin(); it != relative_solids.end(); it++)
	{
		(*it)->UpdateCacheLCP(dt);
	}

	for(set<PHSolid *>::iterator it = pointer_relative_solids.begin(); it != pointer_relative_solids.end(); it++)
	{
		PHSolid* solid = *it;

		// 単位力を加える処理のところで使う値(=加えた力をワールド座標から拘束座標に変換するもの)を今のうちに計算しておく
		Vec3d w = solid->GetOrientation().Conjugated() * solid->GetAngularVelocity();
		Vec3d calc = w % (solid->GetInertia() * w);
		coeff->insert(pair<PHSolid *, Vec3d>(*it, calc));
	}

	// すべての接触についてヤコビアンを計算
	// ヤコビアンの計算は一度だけでよい
	for(set<PHConstraint *>::iterator it = relative_consts.begin(); it != relative_consts.end(); it++)
	{
		(*it)->CompJacobian();
	}

	// シミュレーションで上書きしてしまう変数を今のうちに保存する
	SaveLambda(relative_consts);
}


// 提案手法のために先送りシミュレーションを計画的に行ってくれる関数
void PredictSimulations(vector<pair<PHConstraint *, int> > pointer_consts, set<PHSolid *> pointer_relative_solids, set<PHConstraint *> relative_consts, set<PHSolid *> relative_solids, map<PHSolid *, vector<pair<Matrix3d, Matrix3d> > > *mat, map<PHSolid *, SpatialVector> *vec)
{
	map<PHSolid *, Vec3d> coeff;

	// 必要な情報の準備
	SetupPredictSimulation(relative_consts, relative_solids, pointer_relative_solids, &coeff);

	// 加える力を０ベクトルとして定数項を取得
	map<PHSolid*, SpatialVector> b = PredictSimulation(pointer_consts, pointer_relative_solids, relative_consts, relative_solids, Vec3d(), coeff);

	// 適切にデータを格納するmap-vectorの準備
	// 剛体数だけ空のvectorを宣言してmapに追加する
	map<PHSolid*, vector<pair<Matrix3d, Matrix3d> > > mm_map;
	for(map<PHSolid*, SpatialVector>::iterator it = b.begin(); it != b.end(); it++)
	{
		mm_map.insert(pair<PHSolid*, vector<pair<Matrix3d, Matrix3d> > >((*it).first, vector<pair<Matrix3d, Matrix3d> >()));
	}

	// 接触数＝列数だけ繰り返す
	for(int i = 0; i < (int)pointer_consts.size(); i++)
	{
		// ある単位ベクトルを加えてその結果をあらわすベクトルを取得
		map<PHSolid *, SpatialVector> vec_x = PredictSimulation(pointer_consts, pointer_relative_solids, relative_consts, relative_solids, Vec3d(1, 0, 0), coeff, i);
		map<PHSolid *, SpatialVector> vec_y = PredictSimulation(pointer_consts, pointer_relative_solids, relative_consts, relative_solids, Vec3d(1, 1, 0), coeff, i);
		map<PHSolid *, SpatialVector> vec_z = PredictSimulation(pointer_consts, pointer_relative_solids, relative_consts, relative_solids, Vec3d(1, 0, 1), coeff, i);

		// このループで剛体soの接触pointer_consts[i]に関する行列を計算し格納
		// この接触のすべての剛体に対する影響をこのループで処理する
		for(map<PHSolid *, SpatialVector>::iterator it = b.begin(); it != b.end(); it++)
		{
			PHSolid *so = it->first;
			SpatialVector sv = it->second;
			SpatialVector sv_x = vec_x.find(so)->second;
			SpatialVector sv_y = vec_y.find(so)->second;
			SpatialVector sv_z = vec_z.find(so)->second;

			// 先ほど加えた単位力から、影響を求めて行列を作成する
			// なおトルクについては半径と力の外積が分解されて
			// 半径の外積が行列化されてすでに反映されているはずである
			Matrix3d v = Matrix3d(sv_x.v - sv.v, sv_y.v - sv_x.v - sv.v, sv_z.v - sv_x.v - sv.v);
			Matrix3d w = Matrix3d(sv_x.w - sv.w, sv_y.w - sv_x.w - sv.w, sv_z.w - sv_x.w - sv.w);

			// データの格納。			
			pair<Matrix3d, Matrix3d> p = pair<Matrix3d, Matrix3d>(v, w);

			// 剛体ごとに接触と係数行列のマップを作成する
			// 接触の順番はわかるが剛体の順番はわからないため。
			// 剛体ごとに接触の順番に行列が格納されていく
			mm_map.find(so)->second.push_back(p);
		}
	}

	*mat = mm_map;
	*vec = b;
}


// 周囲の影響を計算し、引数の変数に格納する関数
void calculate_surround_effect(HapticInfo* info, HapticInfo* prev_info)
{
	vector<pair<PHConstraint *, int> > pointer_consts;
	vector<pair<PHConstraint *, int> > pointer_static_consts;
	set<PHSolid *> pointer_relative_solids;

	set<PHSolid *> relative_solids;
	set<PHConstraint *> relative_consts;

	// ポインタからスタートして必要な剛体をすべて取得してくる関数
	GetSolidsFromPointer(&pointer_consts, &pointer_relative_solids, &relative_solids, &relative_consts, &pointer_static_consts);

	map<PHSolid *, vector<pair<Matrix3d, Matrix3d> > > matrices;
	map<PHSolid *, SpatialVector> vecs;

	// 力を加えてみて動かし、影響を観測する関数
	PredictSimulations(pointer_consts, pointer_relative_solids, relative_consts, relative_solids, &matrices, &vecs);

	// 接触の情報を計算する
	MakeHapticInfo(info, prev_info, pointer_consts, matrices, vecs, pointer_static_consts);
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
		for(PHConstraints::iterator it = cp.begin(); it != cp.end(); it++)
		{
			// ループ内のカウンターを更新
			counter++;
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
				for(int i = 0; i < info1.num_solids; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolidIf* nearest_address = (PHSolidIf *)&(*info1.nearest_solids[i]);
					if(nearest_address == (PHSolidIf *)((*it)->solid[0]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			else 
			{
				for(int i = 0; i < info2.num_solids; i++)
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
				for(int i = 0; i < info1.num_solids; i++)
				{
					// 最近傍物体のアドレスを取得
					PHSolid* nearest_address = &(*info1.nearest_solids[i]);
					if(nearest_address == ((*it)->solid[1]))cout << " --this solid is nearest object!!" << endl;
				}
			}
			else
			{
				for(int i = 0; i < info2.num_solids; i++)
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
	if(time > 0 && bOutput)ofs << "time = " << end - start << endl;
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
			bSimulation = true;
			step_counter = 0;
			current_valid_data = !current_valid_data;
		}
		else if(bOutput)ofs << "error : simulation could not be finished in time" << endl;
	}
	else step_counter++;

	// SPIDAR更新幅
	static const double dt = (double)1.0 / HAPTIC_FREQ;

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
	Vec3d goal = spidar_pos - info->pointer_pos;

	// VR空間のポインタとSPIDARをvirtual couplingでつなげる
	Vec3d VCForce = Km * goal + Bm * (SPIDARVel - info->pointer_vel);


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
	Quaterniond ang_goal = spidar_ori * info->pointer_ori.Inv();

	// 回転についてのバーチャルカップリング
	Vec3d VCTorque = Kr * ang_goal.Rotation() + Br * (SPIDARAngVel.Rotation() - info->pointer_angvel);


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// バーチャルカップリングに従ってポインタの位置・姿勢更新
	info->pointer_vel += info->pointer_massinv * VCForce * dt;
	Vec3d pointer_dx = info->pointer_vel * dt;
	info->pointer_pos += pointer_dx;

	info->pointer_angvel += info->pointer_inertiainv * VCTorque * dt;
	Vec3d pointer_dth = info->pointer_angvel * dt;
	info->pointer_ori = Quaterniond::Rot(pointer_dth) * info->pointer_ori;
	info->pointer_ori = info->pointer_ori.unit();


	////////////////////////////////////////////////////////////////////////////////////////
	// 力覚計算
	// ユーザへのフォースフィードバックと接触剛体の加速度更新

	// 登録された接触があっても、現在本当に接触しているかはわからないのでフラグを用意
	bool feedback = false;

	if(bLocalDynamics)
	{
		for(int i = 0; i < info->num_collisions; i++)
		{
			// 面の法線と、ポインタ上の点から剛体上の点までを結んだベクトルの内積を計算
			// これが０以上なら（ゼロベクトルも含む。ちょうど接している）接触がある
			if(dot(info->current_col_positions[i] - info->pointer_current_col_positions[i], info->current_col_normals[i]) >= 0)
			{
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
							accel = info->solid_massinvs[sol_index] * FORCE_COEFF * VCForce;
							ang_accel = info->solid_inertiainvs[sol_index] * ((info->current_col_positions[i] - info->solid_current_center_positions[sol_index]) ^ (VCForce));

							// 得られた加速度で速度を更新
							// 座標は後で更新する
							info->solid_velocity[sol_index] += accel * dt;
										
							// 角速度を更新
							info->solid_angular_velocity[sol_index] += ang_accel * dt;
						}
						// 局所的な動力学計算＋周囲の影響を考慮
						else
						{
							// 剛体に加える力を行列にかけて、加速度を計算する
							Vec3d q_f = info->solid_massinvs[sol_index] * dt * VCForce;

							// この力が及ぼす影響をすべての剛体について計算する
							for(int j = 0; j < info->num_solids; j++)
							{
								// 外力で動かない剛体は飛ばす
								if(!info->nearest_solids[j]->IsDynamical())continue;

								// 行列を用いて加速度を計算して速度を更新
								info->solid_velocity[j] += info->vel_effect[j][i] * q_f;
								info->solid_angular_velocity[j] += info->ang_effect[j][i] * q_f;
							}
						}
					}
				}
			}
			else
			{
				// もう接していないので、シミュレーションから得られた目標地点も意味がなくなる
				info->bLocalReflection1[i] = false;
				info->bLocalReflection2[i] = false;
			}
		}
	}
	// 純粋なバーチャルカップリング
	else
	{
		// 接触があれば接触している剛体に提示力の逆を加える
		// 剛体の挙動は物理シミュレーションに任せる
		if(info->num_collisions)
		{
			// 力覚フィードバックON
			feedback = true;
			for(int i = 0; i < info->num_collisions; i++)
			{
				int sol_index = info->ColToSol[i];
				info->nearest_solids[sol_index]->AddForce(FORCE_COEFF * VCForce, info->current_col_positions[i]);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// ユーザへの力の提示
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


	if(bLocalDynamics)
	{
		Vec3d correct_vector = Vec3d();
		Vec3d correct_torque = Vec3d();

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// 局所的シミュレーションによる接触する剛体の位置・姿勢を更新
		for(int i = 0; i < info->num_solids; i++)
		{
			if(!bSurroundEffect)
			{
				if(info->nearest_solids[i]->IsDynamical()) info->solid_velocity[i] += scene->GetGravity() * dt;
			}
			else
			{
				// 周囲の影響のうちで、定数項を徐々に加えていく
				info->solid_velocity[i] += info->vel_constant[i] * ratio;
				info->solid_angular_velocity[i] += info->ang_constant[i] * ratio;
			}

			// 速度による移動・回転
			Vec3d dx_local = info->solid_velocity[i] * dt;
			info->solid_current_center_positions[i] += dx_local;

			Vec3d dth_local = info->solid_angular_velocity[i] * dt;
			info->solid_current_orientations[i] = Quaterniond::Rot(dth_local) * info->solid_current_orientations[i];

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
			for(int j = 0; j < info->num_col_per_sol[i]; j++)
			{
				// この剛体上にある接触を剛体の移動に伴って更新する
				int col_index = info->SolToCol[i][j];
				info->current_col_positions[col_index] += dx_local;
				info->current_col_normals[col_index] += dth_local ^ (info->current_col_positions[col_index] - info->solid_current_center_positions[i]);
				info->current_col_normals[col_index] = info->current_col_normals[col_index].unit();

				// ポインタの接触点更新
				// ポインタの移動量に従って平行移動
				// この情報はポインタが持つのではなく接触がそれぞれ持っている
				info->pointer_current_col_positions[col_index] += pointer_dx;
				info->pointer_current_col_positions[col_index] += pointer_dth ^ (info->pointer_current_col_positions[col_index] - info->pointer_pos);

				if(bCorrectPenetration)
				{
					// めり込み解消処理
					// すべての接触のめり込みを加え、ポインタの位置に反映させる
					Vec3d col_vector = info->current_col_positions[col_index] - info->pointer_current_col_positions[col_index];
					Vec3d col_normal = dot(col_vector, info->current_col_normals[col_index]) * info->current_col_normals[col_index].unit();
					correct_vector += col_normal;

//					correct_torque += (info->pointer_current_col_positions[col_index] - info->pointer_pos) ^ col_normal;
				}
			}
		}

		// ポインタの位置補正。めり込みを解消
		if(bCorrectPenetration)
		{
			// ポインタ自身の位置を移動
			info->pointer_pos += 0.2 * correct_vector;

//			Quaterniond correct_q = 0.2 * Quaterniond::Rot(correct_torque);
//			info->pointer_ori = info->pointer_ori * correct_q;

			// すべての接触もその方向に移動
			for(int i = 0; i < info->num_collisions; i++)
			{
//				info->pointer_current_col_positions[i] += correct_torque ^ (info->current_col_positions[i] - info->pointer_current_col_positions[i]);
				info->pointer_current_col_positions[i] += 0.2 * correct_vector;
			}
		}
	}
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

	// 力覚レンダリングからシミュレーションへの局所的動力学計算の結果反映処理
	// すべての剛体について行う
	for(int i = 0; i < info->num_solids; i++)
	{
		// 誤差の分平行移動・回転する
		Vec3d pos_error = info->solid_current_center_positions[i] - info->nearest_solids[i]->GetCenterPosition();
		Quaterniond ori_error = info->solid_current_orientations[i] * info->nearest_solids[i]->GetOrientation().Inv();

		info->nearest_solids[i]->SetFramePosition(info->nearest_solids[i]->GetFramePosition() + pos_error);
		info->nearest_solids[i]->SetOrientation(info->nearest_solids[i]->GetOrientation() * ori_error);
		info->nearest_solids[i]->SetCenterPosition(info->solid_current_center_positions[i]);
		info->nearest_solids[i]->SetVelocity(info->solid_velocity[i]);
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

	soPointer->SetFramePosition(info->pointer_pos);
	soPointer->SetVelocity(info->pointer_vel);

	soPointer->SetOrientation(info->pointer_ori);
	soPointer->SetAngularVelocity(info->pointer_angvel);
}

void StepSimulation()
{
	// ポインタの位置を更新
	UpdatePointer();

	// 局所的動力学計算によって生じた差を修正する
	ErrorCorrection();

	// シミュレーションを１ステップ進める
 	scene->Step();

	// 衝突点情報を表示
//	show_collision_info();

	// 衝突点から周囲の影響を計算
	// 基本的に前回のデータを引き継ぐ
	// １を参照中。２を更新
	if(current_valid_data) calculate_surround_effect(&info2, &info1);
	// ２を参照中。１を更新
	else calculate_surround_effect(&info1, &info2);

	// 力覚スレッドのシミュレーションの変数の参照先を変更
//	current_valid_data = !current_valid_data;
	bSimulation = false;
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

		glutSolidSphere(1.0, 12, 9);
	glPopMatrix();

	// 原点を描画
	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		Vec3f te = Vec3f();
		Affinef af = Affinef();
		Posef::Unit(te).ToAffine(af);
		glMultMatrixf(af);
		glutSolidSphere(1.0, 12, 9);
	glPopMatrix();
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
		timeKillEvent(FTimerId1);
		timeEndPeriod(1);

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
	else if(key == 'c')
	{
		// cでキャリブレーション
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
		spidarG6.Calib();
	}
	// sphereの場所をリセット
	else if(key == 'r')
	{
		for(int i = 0; i < NUM_SPHERES; i++)
		{
			soSphere[i]->SetPose(Posed());
			soSphere[i]->SetFramePosition(Vec3f(0, 5+5*(i+1), 0));
			soSphere[i]->SetAngularVelocity(Vec3d());
			soSphere[i]->SetVelocity(Vec3d());
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
	// めり込み補正処理のフラグ変更
	else if(key == 'p')
	{
		bCorrectPenetration = !bCorrectPenetration;
		cout << "Correct Penetration ";
		if(bCorrectPenetration)
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

			bCorrectPenetration = false;
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
		cout << "File Output ";
		if(bOutput)cout << "ON" << endl;
		else cout << "OFF" << endl;
		cout << "Feedback Force ";
		if(bforce)cout << "ON" << endl;
		else cout << "OFF" << endl;
		cout << "Surrounding Effect ";
		if(bSurroundEffect)cout << "ON" << endl;
		else cout << "OFF" << endl;
		cout << "Pointer Display ";
		if(bDisplayPointer)cout << "ON" << endl;
		else cout << "OFF" << endl;
		cout << "Penetration Correct ";
		if(bCorrectPenetration)cout << "ON" << endl;
		else cout << "OFF" << endl;
		cout << "Gradual Reflection ";
		if(bGradualReflection)cout << "ON" << endl;
		else cout << "OFF" << endl;
		cout << "----------------------" << endl;
	}
	// 設定のプレセット
	else if(key == '1')
	{
		bSurroundEffect = true;
		bCorrectPenetration = true;
		bGradualReflection = true;
		bLocalDynamics = true;

		cout << "PRESET 1: Local Dynamics With Surrounding Effect, Penetration Correction, and Gradual Reflection" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '2')
	{
		bSurroundEffect = true;
		bCorrectPenetration = false;
		bGradualReflection = false;
		bLocalDynamics = true;
		
		cout << "PRESET 2: Local Dynamics With Surrounding Effect" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '3')
	{
		bSurroundEffect = false;
		bCorrectPenetration = true;
		bGradualReflection = true;
		bLocalDynamics = true;

		cout << "PRESET 3: Local Dynamics With Penetration Correction and Gradual Reflection" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '4')
	{
		bSurroundEffect = false;
		bCorrectPenetration = false;
		bGradualReflection = false;
		bLocalDynamics = true;

		cout << "PRESET 4: Local Dynamics" << endl;
		keyboard('f', 0, 0);
	}
	else if(key == '5')
	{
		bSurroundEffect = false;
		bCorrectPenetration = false;
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

	timer.CountUS();

	// hapticスレッドの生成・開始
	FTimerId1 = timeSetEvent(1000 / HAPTIC_FREQ,    // タイマー間隔[ms]
	                        1,   // 時間分解能
		                    HapticRendering,//コールバック関数
		                    (DWORD)&time,//ユーザー定義の値
		                    TIME_PERIODIC //単発(TIME_ONESHOT) or 繰り返し(TIME_PERIODIC)
		                    );

	// openGL main loop
	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
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
	desc.mass = 0.3;

	// inertiaの計算式
	// 直方体の場合 I = mass * (r1^2 + r2^2) / 12
	// 球の場合 I = mass * r~2 * 2/5
	desc.inertia = Matrix3d(0.45,0,0,0,0.45,0,0,0,0.45);

	// Solidの作成
	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere.push_back(scene->CreateSolid(desc));		// 剛体をdescに基づいて作成
	}

	desc.mass = 0.1;
	desc.inertia = Matrix3d(0.66, 0, 0, 0, 0.66, 0, 0, 0, 0.66);

	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);

	soPointer = scene->CreateSolid(desc);

	//	形状の作成
	CDBoxIf* floor=NULL;
	CDSphereIf* sphere=NULL;

	{
		CDSphereDesc sd;
		sd.radius = 1.25;
		sphere = DCAST(CDSphereIf, phSdk->CreateShape(sd));

		// 球形のポインタ
		/*
		soPointer->AddShape(sphere);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));
		*/

		sd.radius = 2.0;
		sphere = DCAST(CDSphereIf,phSdk->CreateShape(sd));

		CDBoxDesc bd;
		bd.boxsize = Vec3f (30.0, 1.0, 30.0);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soFloor->AddShape(floor);
		soFloor->SetFramePosition(Vec3f(0,-2.5,0));

		// 四角のポインタ
		bd.boxsize = Vec3f(2, 2, 2);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
		soPointer->AddShape(floor);
		soPointer->SetFramePosition(Vec3f(0, 0, 0));

		bd.boxsize = Vec3f(3, 3, 3);
		floor = DCAST(CDBoxIf, phSdk->CreateShape(bd));
	}	

	for (unsigned int sphereCnt=0; sphereCnt<NUM_SPHERES; ++sphereCnt){
		soSphere[sphereCnt]->AddShape(floor);
		soSphere[sphereCnt]->SetFramePosition(Vec3f(0, 5*(sphereCnt+1), 0));
	}
	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	// pointerの重力を無効にする
	RemoveGravity(soPointer);

	// pointerと接触する剛体との解析法による計算をやめ、
	// 力覚で処理するようにする
	AddInactiveSolid(scene, soPointer);
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
	view.Pos() = Vec3f(0.0, 5.0, -20.0);								// eye
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
